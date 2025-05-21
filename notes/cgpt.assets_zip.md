# Prompt

“Show me how to integrate `miniz` into `VFS` as an archive mount, exposing `Open()` that reads compressed file entries.”

# Response

Below is an example of how you can plug **miniz** (a single‑header ZIP library) into the existing `VFS`, so that you can:

1. **Mount** a ZIP archive under a logical mount point:

   ```cpp
   VFS::MountArchive("assets", "game.pak");
   ```
2. **Open** a file inside it via the same `VFS::Open("assets/textures/hero.png")` call.

---

### 1. Add a ZIP‑backed IVfsFile

First, create a `ZipVfsFile` that implements `IVfsFile` by uncompressing the requested entry into a memory buffer and then serving reads from that buffer:

```cpp
#pragma once

#include "IVfsFile.hpp"
#define MINIZ_HEADER_FILE_ONLY
#include "miniz.h"         // https://github.com/richgel999/miniz
#include <vector>
#include <stdexcept>

namespace ace {

class ZipVfsFile : public IVfsFile {
public:
    /// Constructs and extracts `entryName` from `archivePath`
    ZipVfsFile(const std::filesystem::path& archivePath,
               const std::string&           entryName)
    {
        mz_zip_archive zip{};
        if (!mz_zip_reader_init_file(&zip, archivePath.string().c_str(), 0)) {
            throw std::runtime_error("ZipVfsFile: failed to open " + archivePath.string());
        }

        int index = mz_zip_reader_locate_file(&zip,
                                              entryName.c_str(),
                                              nullptr,
                                              0);
        if (index < 0) {
            mz_zip_reader_end(&zip);
            throw std::runtime_error("ZipVfsFile: entry not found: " + entryName);
        }

        // Get uncompressed size
        mz_zip_archive_file_stat stat;
        mz_zip_reader_file_stat(&zip, index, &stat);
        _size = static_cast<std::size_t>(stat.m_uncomp_size);

        // Read into buffer
        _buffer.resize(_size);
        if (!mz_zip_reader_extract_to_mem(&zip,
                                          index,
                                          _buffer.data(),
                                          _size,
                                          0)) {
            mz_zip_reader_end(&zip);
            throw std::runtime_error("ZipVfsFile: failed to extract " + entryName);
        }

        mz_zip_reader_end(&zip);
    }

    ~ZipVfsFile() override = default;

    std::size_t Read(void* buffer, std::size_t bytes) override {
        if (_pos >= _size) return 0;
        std::size_t toRead = std::min(bytes, _size - _pos);
        std::memcpy(buffer, _buffer.data() + _pos, toRead);
        _pos += toRead;
        return toRead;
    }

    bool Seek(std::size_t offset, SeekOrigin origin) override {
        std::size_t newPos = origin == SeekOrigin::Begin   ? offset
                          : origin == SeekOrigin::Current ? _pos + offset
                          : /* End */                     _size - offset;
        if (newPos > _size) return false;
        _pos = newPos;
        return true;
    }

    std::size_t Tell() const override { return _pos; }
    std::size_t Size() const override { return _size; }
    void Close() override { /* nothing to do for memory buffer */ }

private:
    std::vector<std::uint8_t> _buffer;
    std::size_t               _size = 0;
    std::size_t               _pos  = 0;
};

} // namespace ace
```

* We use **miniz**’s in‑memory API to locate and extract a single entry.
* The entire uncompressed file lives in `_buffer`, and we implement `Read`, `Seek`, etc., against that.

---

### 2. Extend `VFS` to Support Archive Mounts

Modify your `VFS` to store mounts as either **directory** or **archive**. You can use a small tagged struct:

```cpp
#pragma once

#include "IVfsFile.hpp"
#include "LocalVfsFile.hpp"
#include "ZipVfsFile.hpp"
#include <filesystem>
#include <memory>
#include <string>
#include <vector>
#include <variant>

namespace ace {

class VFS {
public:
    /// Mount a real directory under a logical point
    static void MountDirectory(std::string mountPoint,
                               std::filesystem::path realPath)
    {
        normalize(mountPoint);
        if (!std::filesystem::is_directory(realPath))
            throw std::runtime_error("Not a directory: " + realPath.string());
        s_mounts.emplace_back(DirMount{mountPoint, realPath});
    }

    /// Mount a zip archive under a logical point
    static void MountArchive(std::string mountPoint,
                             std::filesystem::path archivePath)
    {
        normalize(mountPoint);
        if (!std::filesystem::exists(archivePath))
            throw std::runtime_error("Archive not found: " + archivePath.string());
        s_mounts.emplace_back(ZipMount{mountPoint, archivePath});
    }

    /// Open a logical path, searching mounts LIFO
    static std::unique_ptr<IVfsFile> Open(const std::string& logicalPath) {
        std::string path = normalize(logicalPath);

        // Try in reverse (LIFO)
        for (auto it = s_mounts.rbegin(); it != s_mounts.rend(); ++it) {
            if (auto file = tryOpenOnMount(*it, path))
                return file;
        }
        return nullptr;
    }

private:
    struct DirMount {
        std::string              mountPoint;
        std::filesystem::path    realPath;
    };
    struct ZipMount {
        std::string              mountPoint;
        std::filesystem::path    archivePath;
    };

    using Mount = std::variant<DirMount, ZipMount>;
    static inline std::vector<Mount> s_mounts;

    static std::string normalize(std::string p) {
        // remove leading/trailing slashes, convert backslashes to '/'
        // (implementation omitted for brevity)
        return p;
    }

    static std::unique_ptr<IVfsFile> tryOpenOnMount(const Mount& m,
                                                    const std::string& path)
    {
        return std::visit([&](auto const& mount) -> std::unique_ptr<IVfsFile> {
            const auto& mp = mount.mountPoint;
            if (!starts_with(path, mp)) return nullptr;
            // get the remainder "subpath" after "mp/"
            std::string sub = path.substr(mp.size());
            if (!sub.empty() && (sub[0]=='/'||sub[0]=='\\')) sub.erase(0,1);

            if constexpr (std::is_same_v<decltype(mount), const DirMount&>) {
                auto real = mount.realPath / sub;
                if (std::filesystem::is_regular_file(real))
                    return std::make_unique<LocalVfsFile>(real);
                else
                    return nullptr;
            } else {
                // ZipMount
                // Attempt to open "sub" inside archive
                try {
                    return std::make_unique<ZipVfsFile>(mount.archivePath, sub);
                } catch (...) {
                    return nullptr;
                }
            }
        }, m);
    }

    static bool starts_with(const std::string& s, const std::string& prefix) {
        return s.size() >= prefix.size()
            && std::equal(prefix.begin(), prefix.end(), s.begin());
    }
};

} // namespace ace
```

#### What This Does

* **`MountArchive`** adds a `ZipMount{mountPoint, archivePath}` to `s_mounts`.
* **`Open`** iterates mounts LIFO. For each:

  * If it’s a `DirMount`, we attempt `LocalVfsFile` on the real path + subpath.
  * If it’s a `ZipMount`, we construct a `ZipVfsFile`, which will throw if the entry doesn’t exist—so we catch and return `nullptr` to keep searching.

---

### 3. Usage

```cpp
// 1) Mount both loose assets and the .pak
ace::VFS::MountDirectory("assets", "content/assets");
ace::VFS::MountArchive  ("assets", "content/game.pak");

// 2) Open transparently
if (auto file = ace::VFS::Open("assets/models/hero.obj")) {
    // Works whether hero.obj is in the directory or inside game.pak
}
```

This integration gives you a **seamless VFS** where ZIP‑packed assets and loose files coexist, all via the same `VFS::Open()` call.
