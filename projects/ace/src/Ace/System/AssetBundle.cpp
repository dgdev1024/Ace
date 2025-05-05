/**
 * @file    Ace/System/AssetBundle.cpp
 */

#include <Ace/Precompiled.hpp>
#include <Ace/System/AssetBundle.hpp>

namespace ace
{

    /* Private Structures *************************************************************************/

    /**
     * @brief   Helper struct containing compressed asset data. Used by the @a `AssetBundle::write`
     *          method.
     */
    struct PackContext
    {
        std::string                 mVirtualPath = "";
        std::vector<std::uint8_t>   mCompressedData;  
        AssetBundle::IndexEntry     mIndexEntry;
    };

    /* Constructors and Destructor ****************************************************************/

    AssetBundle::AssetBundle (const fs::path& pPath)
    {
        if (openFromFile(pPath) == false)
        {
            ACE_ENGINE_THROW(std::runtime_error,
                "'AssetBundle' constructor failed to open file '{}'!", pPath.string());
        }
    }

    /* Public Methods *****************************************************************************/

    bool AssetBundle::openFromFile (const fs::path& pPath)
    {
        // Configure the file stream to throw an exception for file open/read errors.
        mFileStream.exceptions(std::ios::failbit | std::ios::badbit);

        // Ensure the file exists, and is a regular file.
        if (fs::exists(pPath) == false || fs::is_regular_file(pPath) == false)
        {
            ACE_ENGINE_ERROR("Asset bundle file '{}' not found.", pPath.string());
            return false;
        }

        // Attempt to open the file.
        try
        {
            mFileStream.open(pPath, std::ios::in | std::ios::binary | std::ios::ate);
    
            // Attempt to validate the file.
            if (validateHeader() == false)
            {
                ACE_ENGINE_ERROR("Asset bundle file '{}' could not be validated.", pPath.string());
                return false;
            }
    
            // At this point, the file stream should be at the point of the file containing the asset
            // entry count. Get it.
            std::uint32_t lEntryCount = 0;
            mFileStream.read(reinterpret_cast<char*>(&lEntryCount), sizeof(std::uint32_t));
    
            // Read each asset index entry.
            for (std::uint32_t lIndex = 0; lIndex < lEntryCount; ++lIndex)
            {
    
                // Read the length of the asset's virtual path.
                std::uint16_t lPathLength = 0;
                mFileStream.read(reinterpret_cast<char*>(&lPathLength), sizeof(std::uint16_t));
    
                // Read the asset's virtual path.
                std::string lPath;
                lPath.resize(lPathLength);
                mFileStream.read(lPath.data(), lPathLength);
    
                // Read the asset's index entry data, then emplace it.
                IndexEntry lEntry;
                mFileStream.read(reinterpret_cast<char*>(&lEntry.mOffset),          sizeof(std::uint64_t));
                mFileStream.read(reinterpret_cast<char*>(&lEntry.mCompressedSize),  sizeof(std::uint64_t));
                mFileStream.read(reinterpret_cast<char*>(&lEntry.mRawSize),         sizeof(std::uint64_t));
                mIndexEntries.emplace(std::move(lPath), std::move(lEntry));            
    
            }
    
            // At this point, the file stream should now be positioned at the start of the asset data.
            // Record this point.
            mStartPoint = mFileStream.tellg();
    
            // The file stream will be kept open for the lifetime of this asset bundle.
            return true;
        }
        catch (std::exception& ex)
        {
            ACE_ENGINE_ERROR("Could not open asset bundle file '{}': '{}'.", pPath.string(),
                ex.what());
            return false;
        }
    }

    std::optional<std::vector<std::uint8_t>> AssetBundle::read (const std::string& pVirtualPath)
    {
        // Make sure that the asset bundle's file stream is currently open, first!
        if (mFileStream.is_open() == false)
        {
            ACE_ENGINE_ERROR("Cannot read asset data: asset bundle file stream is not open.");
            return std::nullopt;
        }

        // Make sure the given virtual path is not empty.
        if (pVirtualPath.empty() == true)
        {
            ACE_ENGINE_ERROR("Cannot read asset data: virtual path string is blank.");
            return std::nullopt;
        }

        // Find the asset's index entry.
        auto lIter = mIndexEntries.find(pVirtualPath);
        if (lIter == mIndexEntries.end())
        {
            ACE_ENGINE_ERROR("Cannot read asset data: virtual path string is blank.");
            return std::nullopt;
        }

        try
        {
            
            // Get the index entry.
            auto [lOffset, lCompressedSize, lRawSize] = lIter->second;

            // Read the asset's compressed data.
            std::vector<std::uint8_t> lCompressedData(lCompressedSize);
            mFileStream.seekg(lOffset, mFileStream.beg);
            mFileStream.read(reinterpret_cast<char*>(lCompressedData.data()), lCompressedSize);

            // Decompress the data.
            std::vector<std::uint8_t> lRawData(lRawSize);
            std::int32_t lDecompressedSize = LZ4_decompress_safe(
                reinterpret_cast<const char*>(lCompressedData.data()),
                reinterpret_cast<char*>(lRawData.data()),
                static_cast<std::int32_t>(lCompressedSize),
                static_cast<std::int32_t>(lRawSize)
            );
            if (
                lDecompressedSize < 0 ||
                static_cast<std::uint64_t>(lDecompressedSize) != lRawSize
            )
            {
                ACE_ENGINE_ERROR("Cannot read asset data: error decompressing asset '{}'.",
                    pVirtualPath);
                ACE_ENGINE_TRACE("Expected: {}, Got: {}", lRawSize, lDecompressedSize);
                return std::nullopt;
            }

            // Asset read OK.
            return lRawData;

        }
        catch (const std::exception& ex)
        {
            ACE_ENGINE_ERROR("Cannot read asset data: error reading asset '{}': '{}'.",
                pVirtualPath, ex.what());
            return std::nullopt;
        }
        
    }

    /* Public Static Methods **********************************************************************/

    bool AssetBundle::write (
        const fs::path& pPath,
        const std::vector<std::pair<std::string, std::vector<std::uint8_t>>>& pAssets
    )
    {

        // Create a list of packing contexts, and reserve a number of structures in it according to
        // the number of asset datas provided.
        std::vector<PackContext> lPackContexts;
        lPackContexts.reserve(pAssets.size());

        // Iterate over each of the string-buffer pairs. Compress each buffer.
        for (const auto& [lVirtualPath, lRawData] : pAssets)
        {
            // Get the size of the data buffer, and its maximum compressed size.
            std::int32_t lRawSize               = static_cast<std::int32_t>(lRawData.size());
            std::int32_t lMaxCompressionSize    = LZ4_compressBound(lRawSize);

            // Create a buffer to store the compressed data.
            std::vector<std::uint8_t> lCompressedData(lMaxCompressionSize);

            // Compress the data and get the actual compressed size.
            std::int32_t lCompressedSize = LZ4_compress_default(
                reinterpret_cast<const char*>(lRawData.data()),
                reinterpret_cast<char*>(lCompressedData.data()),
                lRawSize,
                lMaxCompressionSize
            );
            if (lCompressedSize <= 0)
            {
                ACE_ENGINE_ERROR("Could not write asset bundle: error compressing asset '{}' into '{}'.",
                    lVirtualPath, pPath.string());
                return false;
            }

            // Resize the compressed buffer to the actual compressed size. Then move that buffer into
            // a new packing context.
            lCompressedData.resize(lCompressedSize);
            lPackContexts.push_back(
                PackContext {
                    .mVirtualPath       = lVirtualPath,
                    .mCompressedData    = std::move(lCompressedData),
                    .mIndexEntry        = AssetBundle::IndexEntry {
                        .mCompressedSize    = static_cast<std::uint64_t>(lCompressedSize),
                        .mRawSize           = static_cast<std::uint64_t>(lRawSize)
                    }
                }
            );
        }

        // Calculate the size of the asset bundle header, and the index entry table, so that we can
        // assign offsets to each fo the asset datas.
        std::uint32_t   lEntryCount     = static_cast<std::uint32_t>(lPackContexts.size());
        std::size_t     lHeaderSize     = 12;       // The asset bundle header is 12 bytes long.
        for (const auto& lPackContext : lPackContexts)
        {
            lHeaderSize +=
                sizeof(std::uint16_t) +             // Virtual path length integer size
                lPackContext.mVirtualPath.size() +  // Virtual path string length
                sizeof(std::uint64_t) * 3;          // Offset, compressed size and raw size integer sizes
        }

        // Assign offsets to the asset datas, keeping track of a cursor while doing so.
        std::uint64_t lCursor = lHeaderSize;
        for (auto& lPackContext : lPackContexts)
        {
            lPackContext.mIndexEntry.mOffset = lCursor;
            lCursor += lPackContext.mCompressedData.size();
        }

        // Attempt to open the output file. Configure the resultant file stream to throw exceptions
        // when a write error occurs.
        std::fstream lFile { pPath, std::ios::out | std::ios::binary | std::ios::trunc };
        if (lFile.is_open() == false)
        {
            ACE_ENGINE_ERROR("Could not write asset bundle: could not open file '{}' for writing.",
                pPath.string());
            return false;
        }
        else
        {
            lFile.exceptions(std::ios::failbit | std::ios::badbit);
        }

        // Now it's time to write the asset bundle file. Start with the header.
        lFile.write(reinterpret_cast<const char*>(&ASSET_BUNDLE_MAGIC_NUMBER), sizeof(std::uint32_t));
        lFile.write(reinterpret_cast<const char*>(&ENGINE_MAJOR_VERSION), sizeof(std::uint8_t));
        lFile.write(reinterpret_cast<const char*>(&ENGINE_MINOR_VERSION), sizeof(std::uint8_t));
        lFile.write(reinterpret_cast<const char*>(&ENGINE_REVISION_NUMBER), sizeof(std::uint16_t));
        lFile.write(reinterpret_cast<char*>(&lEntryCount), sizeof(std::uint32_t));

        // Now write the index entries.
        for (const auto& lPackContext : lPackContexts)
        {
            std::uint16_t lVirtualPathLength = static_cast<std::uint16_t>(lPackContext.mVirtualPath.size());
            
            lFile.write(reinterpret_cast<char*>(&lVirtualPathLength), sizeof(std::uint16_t));
            lFile.write(lPackContext.mVirtualPath.data(), lVirtualPathLength);
            lFile.write(reinterpret_cast<const char*>(&lPackContext.mIndexEntry.mOffset),           sizeof(std::uint64_t));
            lFile.write(reinterpret_cast<const char*>(&lPackContext.mIndexEntry.mCompressedSize),   sizeof(std::uint64_t));
            lFile.write(reinterpret_cast<const char*>(&lPackContext.mIndexEntry.mRawSize),          sizeof(std::uint64_t));
        }

        // Now write the compressed asset datas.
        for (const auto& lPackContext : lPackContexts)
        {
            lFile.write(reinterpret_cast<const char*>(lPackContext.mCompressedData.data()),
                lPackContext.mIndexEntry.mCompressedSize);
        }

        // Asset pack OK
        return true;

    }

    /* Private Methods - Validation ***************************************************************/

    bool AssetBundle::validateHeader ()
    {

        // Asset Bundle Header
        // - (+0x00, 4 Bytes):  Magic Number            (must = `0xACEA55E7`)
        // - (+0x04, 1 Byte):   Engine Major Version    (must = engine major version)
        // - (+0x05, 1 Byte):   Engine Minor Version    (must <= engine minor version)
        // - (+0x06, 2 Bytes):  Engine Revision Number  (should <= engine revision number, but not required)
        // - (+0x08, 4 Bytes):  Asset Count
        // -                    Total = 12 Bytes

        // Get the file's size. Make sure it's at least big enough to hold the header.
        auto lFileSize = mFileStream.tellg();
        mFileStream.seekg(0, mFileStream.beg);
        if (lFileSize < 12)
        {
            ACE_ENGINE_ERROR("Asset bundle file size is too small to hold the header.");
            return false;
        }

        // Validate the magic number.
        std::uint32_t   lMagicNumber = 0;
        mFileStream.read(reinterpret_cast<char*>(&lMagicNumber), sizeof(std::uint32_t));
        if (lMagicNumber != ASSET_BUNDLE_MAGIC_NUMBER)
        {
            ACE_ENGINE_ERROR("Asset bundle magic number is not valid.");
            return false;
        }

        // Validate the major major and minor versions, and the revision number.
        std::uint8_t    lMajorVersion = 0;
        std::uint8_t    lMinorVersion = 0;
        std::uint16_t   lRevisionNumber = 0;
        mFileStream.read(reinterpret_cast<char*>(&lMajorVersion),   sizeof(std::uint8_t));
        mFileStream.read(reinterpret_cast<char*>(&lMinorVersion),   sizeof(std::uint8_t));
        mFileStream.read(reinterpret_cast<char*>(&lRevisionNumber), sizeof(std::uint16_t));
        if (lMajorVersion != ENGINE_MAJOR_VERSION || lMinorVersion > ENGINE_MINOR_VERSION)
        {
            ACE_ENGINE_ERROR("Asset bundle version is incompatible.");
            return false;
        }
        else if (lRevisionNumber > ENGINE_REVISION_NUMBER)
        {
            ACE_ENGINE_WARN("Asset bundle revision number is newer than the engine.");
        }

        return true;

    }

}
