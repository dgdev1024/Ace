/**
 * @file    Ace/System/AssetBundle.hpp
 * @brief   Provides a class used for reading, writing and bundling assets.
 */

#pragma once
#include <Ace/Common.hpp>

namespace ace
{

    /**
     * @def     `ace::ASSET_BUNDLE_MAGIC_NUMBER`
     * @brief   The magic number identifying a file as an Ace Engine Asset Bundle file.
     */
    constexpr std::uint32_t ASSET_BUNDLE_MAGIC_NUMBER = 0xACEA55E7;

    /**
     * @class   `ace::AssetBundle`
     * @brief   Provides an interface for reading assets from, and writing assets to, an asset
     *          bundle.
     */
    class ACE_API AssetBundle final
    {
    public:

        /**
         * @struct  `ace::AssetBundle::IndexEntry`
         * @brief   Helper structure defining offset and size data for an asset.
         */
        struct IndexEntry
        {
            std::uint64_t   mOffset = 0;            ///< @brief The starting offset of the compressed data, in bytes.
            std::uint64_t   mCompressedSize = 0;    ///< @brief The size of the asset's compressed data, in bytes.
            std::uint64_t   mRawSize = 0;           ///< @brief The expected decompressed size of the asset, in bytes.
        };

    public:

        /**
         * @fn      `ace::AssetBundle::AssetBundle`
         * @brief   The default constructor.
         */
        AssetBundle () = default;

        /**
         * @fn      `ace::AssetBundle::AssetBundle`
         * @brief   This constructor opens an asset bundle file at the given path.
         * 
         * @param   pPath   The path to the asset bundle to be opened.
         * 
         * @throw   @a `std::runtime_error` if the underlying call to @a `openFromFile` returns
         *          `false`.
         */
        AssetBundle (const fs::path& pPath);

    public:

        /**
         * @fn      `ace::AssetBundle::openFromFile`
         * @brief   Opens an asset bundle file at the given path.
         * 
         * @param   pPath   The path to the asset bundle to be opened.
         * 
         * @return  `true` if the asset bundle file is opened successfully; `false` otherwise.
         */
        bool openFromFile (const fs::path& pPath);

        /**
         * @fn      `ace::AssetBundle::read`
         * @brief   Reads and decompresses an asset mapped to the given virtual path.
         * 
         * @param   pVirtualPath    The virtual path string mapped to the asset to read.
         * 
         * @return  An `std::optional` which will contain the asset's decompressed data if
         *          successful, or nothing otherwise.
         */
        std::optional<std::vector<std::uint8_t>> read (const std::string& pVirtualPath);

    public:

        /**
         * @fn      `ace::AssetBundle::write`
         * @brief   Helper function used for packing a list of asset data buffers into a new asset
         *          bundle file.
         * 
         * @param   pPath           The path to the asset bundle file to be written.
         * @param   pAssets         A list of key-value pairs, each one containing a list of raw
         *                          asset data, mapped to a virtual path string.
         * 
         * @return  `true` if the asset bundle is written successfully; `false` otherwise.
         */
        static bool write (
            const fs::path& pPath,
            const std::vector<std::pair<std::string, std::vector<std::uint8_t>>>& pAssets
        );

    private:

        /**
         * @fn      `ace::AssetBundle::validateHeader`
         * @brief   Validates the header found at the start of an asset bundle file.
         * 
         * @return  `true` if the header is validated successfully; `false` otherwise.
         */
        bool validateHeader ();

    private:

        /**
         * @def     `ace::AssetBundle::mFileStream`
         * @brief   The file stream used for reading and writing the asset bundle's compressed data.
         */
        std::fstream mFileStream;

        /**
         * @def     `ace::AssetBundle::mIndexEntries`
         * @brief   A dictionary of asset index entries mapped to strings.
         */
        std::unordered_map<std::string, IndexEntry> mIndexEntries;

        /**
         * @def     `aceAssetBundle::mStartPoint`
         * @brief   Contains the starting offset of the asset bundle's compressed asset data.
         */
        std::size_t mStartPoint = 0;

    };

}
