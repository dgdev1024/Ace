/**
 * @file    Ace/System/Filesystem.hpp
 * @brief   Contains a static class used for accessing loose files and packed archives.
 */

#pragma once
#include <Ace/System/AssetBundle.hpp>

namespace ace
{

    /**
     * @class   `ace::Filesystem`
     * @brief   A static class which provides a virtual filesystem ("VFS") unifying access to assets
     *          found in loose files and in packaged archives.
     */
    class ACE_API Filesystem final
    {
    public:

        /**
         * @fn      `ace::Filesystem::mountFolder`
         * @brief   Registers a loose folder for the @a `Filesystem` to search in.
         * 
         * @param   pPath   The path to the folder to check in.
         */
        static void mountLooseFolder (const fs::path& pPath);

        /**
         * @fn      `ace::Filesystem::mountAssetBundle`
         * @brief   Registers an asset bundle file for the @a `Filesystem` to search in.
         * 
         * @param   pPath   The path to the asset bundle file to register.
         */
        static void mountAssetBundle (const fs::path& pPath);

        /**
         * @fn      `ace::Filesystem::readAsset`
         * @brief   Reads an asset from the given path, be it from a loose file or a @a `Package`.
         * 
         * @param   pPath   The virtual path to the asset.
         * 
         * @return  A byte buffer with the loaded asset's contents if successful.
         */
        static std::vector<std::uint8_t> readAsset (const fs::path& pPath);

    private:

        /**
         * @fn      `ace::Filesystem::readLooseFile`
         * @brief   Reads a loose file at the given path, loading its contents.
         * 
         * @param   pPath   The path to the loose file to load.
         * 
         * @return  A byte buffer with the loaded file's contents if successful.
         */
        static std::vector<std::uint8_t> readLooseFile (const fs::path& pPath);

    private:

        /**
         * @def     `ace::Filesystem::sMutex`
         * @brief   A shared mutex used for locking down the @a `Filesystem`'s resources when
         *          mounting folders and reading data.
         */
        static std::shared_mutex sMutex;

        /**
         * @def     `ace::Filesystem::sLooseFolders`
         * @brief   A container of mounted loose folders.
         */
        static std::vector<fs::path> sLooseFolders;

        /**
         * @def     `ace::Filesystem::sBundles`
         * @brief   A container of mounted asset bundle files.
         */
        static std::vector<AssetBundle> sAssetBundles;

    };

}
