/**
 * @file    Ace/System/Filesystem.cpp
 */

#include <Ace/Precompiled.hpp>
#include <Ace/System/Filesystem.hpp>

namespace ace
{

    /* Static Members *****************************************************************************/

    std::shared_mutex           Filesystem::sMutex;
    std::vector<fs::path>       Filesystem::sLooseFolders;
    std::vector<AssetBundle>    Filesystem::sAssetBundles;

    /* Public Methods *****************************************************************************/

    void Filesystem::mountLooseFolder (const fs::path& pPath)
    {
        std::lock_guard lLock { sMutex };
        sLooseFolders.push_back(pPath);
    }

    void Filesystem::mountAssetBundle (const fs::path& pPath)
    {
        std::lock_guard lLock { sMutex };
        sAssetBundles.emplace_back(pPath);
    }

    std::vector<std::uint8_t> Filesystem::readAsset (const fs::path& pPath)
    {
        std::shared_lock lLock { sMutex };

        // Iterate over the mounted loose folders and look for the asset in there, first.
        // The specified `pPath` should be relative to one of these folders.
        for (const fs::path& lLooseFolder : sLooseFolders)
        {
            fs::path lFullPath = lLooseFolder / pPath;
            if (fs::exists(lFullPath) == true)
            {
                return readLooseFile(lFullPath);
            }
        }

        // Iteate over the mounted asset bundle files and look for the asset in there.
        for (auto& lAssetBundle : sAssetBundles)
        {
            if (auto lAssetData = lAssetBundle.read(pPath.string()))
            {
                return *lAssetData;
            }
        }

        // Throw if the asset could not be found.
        ACE_ENGINE_THROW(std::out_of_range, "Asset '{}' could not be found!", pPath.string());
    }

    /* Private Methods ****************************************************************************/

    std::vector<std::uint8_t> Filesystem::readLooseFile (const fs::path& pPath)
    {

        // Attempt to load the file.
        std::fstream lFile { pPath, std::ios::in | std::ios::binary | std::ios::ate };
        if (lFile.is_open() == false)
        {
            ACE_ENGINE_THROW(std::runtime_error, "Cannot open loose file '{}' for reading!",
                pPath.string());
        }

        // Get the file's size.
        auto lSize = lFile.tellg();
        lFile.seekg(0, lFile.beg);

        // Create the byte buffer and read the file's contents into it.
        std::vector<std::uint8_t> lBuffer(lSize);
        lFile.read(reinterpret_cast<char*>(lBuffer.data()), lSize);
        if (!lFile)
        {
            ACE_ENGINE_THROW(std::runtime_error, "Error reading loose file '{}'!", pPath.string());
        }

        // Return the buffer.
        return lBuffer;

    }

}
