# Prompt

“Demonstrate how to watch a directory for changes using `inotify` on Linux, and equivalent mechanisms on Windows and Mac OSX, and emit a `FileChanged` event into `EventBus`.”

# Response

Below is a **cross-platform directory watcher** that on Linux uses **inotify**, on Windows **ReadDirectoryChangesW**, and on macOS **FSEvents**. Whenever a file is created, modified, or deleted under your watched paths, it emits a `FileChanged` event via your `EventBus`.

---

```cpp
// FileWatcher.hpp
#pragma once

#include <string>
#include <thread>
#include <vector>
#include <atomic>
#include <unordered_map>
#include <filesystem>

#include "EventBus.hpp"   // ace::EventBus
#include "Events.hpp"     // defines struct FileChanged { std::string path; enum Type { Created, Modified, Deleted } type; };

#if defined(__linux__)
  #include <sys/inotify.h>
  #include <unistd.h>
#elif defined(_WIN32)
  #include <windows.h>
#elif defined(__APPLE__)
  #include <CoreServices/CoreServices.h>
#endif

namespace ace {

class FileWatcher {
public:
    FileWatcher() : _running(false) {}
    ~FileWatcher() { Stop(); }

    /// Start watching one or more directories (non-recursive).  
    void Start(const std::vector<std::filesystem::path>& dirs) {
        if (_running.exchange(true)) return;
        for (auto const& d : dirs) {
            _dirs.push_back(d);
        }
        _thread = std::thread([this]{ this->Run(); });
    }

    void Stop() {
        if (!_running.exchange(false)) return;
#if defined(__linux__)
        if (_inotifyFd >= 0) ::close(_inotifyFd);
#elif defined(_WIN32)
        CancelIoEx(_dirHandles[0].get(), nullptr);
#elif defined(__APPLE__)
        FSEventStreamStop(_stream);
        FSEventStreamInvalidate(_stream);
        FSEventStreamRelease(_stream);
#endif
        if (_thread.joinable()) _thread.join();
    }

private:
    void Run();

    std::atomic<bool> _running;
    std::thread       _thread;
    std::vector<std::filesystem::path> _dirs;

#if defined(__linux__)
    int _inotifyFd = -1;
    std::unordered_map<int, std::filesystem::path> _wdToPath;
#elif defined(_WIN32)
    struct HandleDeleter { void operator()(HANDLE h) { if (h) CloseHandle(h); } };
    std::vector<std::unique_ptr<std::remove_pointer<HANDLE>::type, HandleDeleter>> _dirHandles;
    std::vector<std::vector<char>> _buffers;
#elif defined(__APPLE__)
    FSEventStreamRef _stream = nullptr;
#endif
};

} // namespace ace
```

```cpp
// FileWatcher.cpp
#include "FileWatcher.hpp"

namespace ace {

void FileWatcher::Run() {
#if defined(__linux__)
    // --- Linux / inotify ---
    _inotifyFd = inotify_init1(IN_NONBLOCK);
    for (auto const& dir : _dirs) {
        int wd = inotify_add_watch(_inotifyFd, dir.c_str(),
            IN_CREATE | IN_MODIFY | IN_DELETE);
        _wdToPath[wd] = dir;
    }

    constexpr size_t BUF_LEN = 1024 * (sizeof(inotify_event) + 16);
    std::vector<char> buf(BUF_LEN);
    while (_running) {
        int len = read(_inotifyFd, buf.data(), BUF_LEN);
        if (len <= 0) { std::this_thread::sleep_for(std::chrono::milliseconds(50)); continue; }
        int i = 0;
        while (i < len) {
            auto* ev = reinterpret_cast<inotify_event*>(buf.data() + i);
            auto dir = _wdToPath[ev->wd];
            std::filesystem::path changed = dir / ev->name;
            FileChanged::Type type = FileChanged::Modified;
            if (ev->mask & IN_CREATE) type = FileChanged::Created;
            else if (ev->mask & IN_DELETE) type = FileChanged::Deleted;
            EventBus::Publish(FileChanged{ changed.string(), type });
            i += sizeof(inotify_event) + ev->len;
        }
    }

#elif defined(_WIN32)
    // --- Windows / ReadDirectoryChangesW ---
    DWORD notifyFilter = FILE_NOTIFY_CHANGE_FILE_NAME
                       | FILE_NOTIFY_CHANGE_LAST_WRITE
                       | FILE_NOTIFY_CHANGE_CREATION;
    for (auto const& dir : _dirs) {
        HANDLE h = CreateFileW(
            dir.wstring().c_str(),
            FILE_LIST_DIRECTORY,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
            nullptr
        );
        _dirHandles.emplace_back(h);
        _buffers.emplace_back(1024 * 16);
        // start first ReadDirectoryChangesW
        OVERLAPPED* ov = new OVERLAPPED{};
        ReadDirectoryChangesW(h, _buffers.back().data(),
                              DWORD(_buffers.back().size()),
                              FALSE, notifyFilter,
                              nullptr, ov, nullptr);
    }
    while (_running) {
        for (size_t i = 0; i < _dirHandles.size(); ++i) {
            DWORD bytes;
            HANDLE h = _dirHandles[i].get();
            if (!GetOverlappedResult(h, reinterpret_cast<OVERLAPPED*>(0), &bytes, FALSE))
                continue;
            char* ptr = _buffers[i].data();
            while (bytes > 0) {
                auto* info = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(ptr);
                std::wstring nameW(info->FileName, info->FileName + (info->FileNameLength/2));
                std::string name(nameW.begin(), nameW.end());
                auto changed = std::filesystem::path(_dirs[i]) / name;
                FileChanged::Type type = FileChanged::Modified;
                switch (info->Action) {
                    case FILE_ACTION_ADDED:    type = FileChanged::Created; break;
                    case FILE_ACTION_REMOVED:  type = FileChanged::Deleted; break;
                    case FILE_ACTION_MODIFIED: type = FileChanged::Modified; break;
                }
                EventBus::Publish(FileChanged{ changed.string(), type });
                if (!info->NextEntryOffset) break;
                ptr += info->NextEntryOffset;
                bytes -= info->NextEntryOffset;
            }
            // re-issue
            OVERLAPPED* ov = new OVERLAPPED{};
            ReadDirectoryChangesW(h, _buffers[i].data(),
                                  DWORD(_buffers[i].size()),
                                  FALSE, notifyFilter,
                                  nullptr, ov, nullptr);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

#elif defined(__APPLE__)
    // --- macOS / FSEvents ---
    // Build CFArray of CFStringRef paths
    std::vector<CFStringRef> paths;
    for (auto const& dir : _dirs)
        paths.push_back(CFStringCreateWithCString(NULL, dir.c_str(), kCFStringEncodingUTF8));
    CFArrayRef pathsArray = CFArrayCreate(
        nullptr,
        reinterpret_cast<const void**>(paths.data()),
        paths.size(),
        &kCFTypeArrayCallBacks
    );
    // Create context carrying `this`
    FSEventStreamContext ctx{0, this, nullptr, nullptr, nullptr};
    _stream = FSEventStreamCreate(
        nullptr,
        [](ConstFSEventStreamRef, void* userData,
           size_t numEvents, void* evPaths,
           const FSEventStreamEventFlags flags[],
           const FSEventStreamEventId[])
        {
            FileWatcher* self = static_cast<FileWatcher*>(userData);
            char** paths = static_cast<char**>(evPaths);
            for (size_t i = 0; i < numEvents; ++i) {
                std::string p(paths[i]);
                FileChanged::Type type = FileChanged::Modified;
                if (flags[i] & kFSEventStreamEventFlagItemCreated)   type = FileChanged::Created;
                else if (flags[i] & kFSEventStreamEventFlagItemRemoved) type = FileChanged::Deleted;
                EventBus::Publish(FileChanged{ p, type });
            }
        },
        &ctx,
        pathsArray,
        kFSEventStreamEventIdSinceNow,
        0.5,  // latency
        kFSEventStreamCreateFlagFileEvents
    );
    FSEventStreamScheduleWithRunLoop(_stream,
                                     CFRunLoopGetCurrent(),
                                     kCFRunLoopDefaultMode);
    FSEventStreamStart(_stream);

    // Run loop will call your callback; just spin until stopped
    while (_running) {
        CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.5, false);
    }

    // Clean up CFStringRefs
    for (auto cs : paths) CFRelease(cs);
    CFRelease(pathsArray);

#endif
}

} // namespace ace
```

---

### Notes

* **FileChanged Event**

  ```cpp
  struct FileChanged {
      std::string path;
      enum Type { Created, Modified, Deleted } type;
  };
  ```

  Add it to your `Events.hpp` and include it in `EventBus`.

* **Threading**
  The watcher runs in its own thread; when it calls `EventBus::Publish(FileChanged{…})`, those events queue for deferred dispatch on your main thread (assuming you call `ProcessDeferred()` there).

* **Error Handling & Cleanup**
  Production code should check return values closely and clean up any allocated `OVERLAPPED` structures, CFRunLoop observers, etc.

With this in place, any change under your watched directories will automatically generate `FileChanged` events that subscribers can handle via the engine’s `EventBus`.
