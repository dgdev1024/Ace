/**
 * @file    Ace/System/VirtualArchiveFile.hpp
 * @brief   Contains a class representing a virtual file extracted from a
 *          compressed archive file and loaded via the virtual filesystem (VFS).
 */

#pragma once
#include <Ace/System/IVirtualFile.hpp>

namespace ace
{

    /**
     * @brief   A class representing a file extracted from a compressed archive
     *          (`.zip`) and loaded via the virtual filesystem (VFS).
     * 
     * File decompression is powered by the `miniz` library.
     */
    class ACE_API VirtualArchiveFile final : public IVirtualFile
    {
    public:

        /**
         * @brief   The default constructor extracts a file specified by the
         *          given entry name from a compressed archive file at the given
         *          path.
         * 
         * @param   pArchivePath    The path to the archive file to open.
         * @param   pEntryName      The name of the file entry to extract.
         * 
         * @throw   `std::invalid_argument` if `pArchivePath` or `pEntryName` is empty.
         * @throw   `std::runtime_error` if `pArchivePath` could not be opened.
         * @throw   `std::out_of_range` if `pEntryName` is not found in `pArchivePath`.
         */
        explicit VirtualArchiveFile (
            const fs::path&     pArchivePath,
            const std::string&  pEntryName
        );

        /**
         * @brief   The destructor.
         */
        ~VirtualArchiveFile () override = default;

    public:

        std::size_t Read (
            void*               pBuffer, 
            const std::size_t&  pBytes = (std::size_t) -1
        ) override;

        bool Seek (
            const std::size_t&  pOffset, 
            FileSeekPoint       pPoint = FileSeekPoint::Start
        ) override;

        std::size_t Tell () const override;
        
        std::size_t GetSize () const override;
        
        void Close () override;

    private:
        astd::byte_buffer   mBuffer;        ///< @brief Contains the file's extracted data.
        std::size_t         mSize = 0;      ///< @brief The size of the extracted data, in bytes.
        std::size_t         mPosition = 0;  ///< @brief The current position of the read cursor, in bytes.

    };

}
