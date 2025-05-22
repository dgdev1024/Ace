/**
 * @file    Ace/System/VirtualArchiveFile.cpp
 */

#include <miniz.h>
#include <Ace/System/VirtualArchiveFile.hpp>

namespace ace
{

    /* Constructors and Destructor ********************************************/

    VirtualArchiveFile::VirtualArchiveFile (
        const fs::path&     pArchivePath,
        const std::string&  pEntryName
    ) :
        IVirtualFile {}
    {

        // Ensure that an archive path and entry name were provided.
        if (pArchivePath.empty() == true)
        {
            ACE_THROW(
                std::invalid_argument,
                "{}: No archive path provided!",
                "VirtualArchiveFile"
            );
        }
        else if (pEntryName.empty() == true)
        {
            ACE_THROW(
                std::invalid_argument,
                "{}: No entry name provided!",
                "VirtualArchiveFile"
            );
        }
        
        // Attempt to open the archive file.
        mz_zip_archive lZip { };
        if (
            mz_zip_reader_init_file(&lZip, pArchivePath.c_str(), 
                0) == MZ_FALSE
        )
        {
            ACE_THROW(
                std::runtime_error,
                "{}: Could not open archive file '{}' - {}!",
                "VirtualArchiveFile", pArchivePath.string(),
                mz_zip_get_error_string(mz_zip_get_last_error(&lZip))
            );
        }

        // Locate the file with the given entry name inside the archive. Get its
        // index if it exists.
        auto lIndex = mz_zip_reader_locate_file(
            &lZip,
            pEntryName.c_str(),
            nullptr,
            0
        );
        if (lIndex < 0)
        {
            auto lErrorMsg = mz_zip_get_error_string(mz_zip_get_last_error(&lZip));
            mz_zip_reader_end(&lZip);
            ACE_THROW(
                std::out_of_range,
                "{}: Entry '{}' not found in archive file '{}' - {}!",
                "VirtualArchiveFile", pEntryName, pArchivePath.string(), lErrorMsg
            );
        }

        // Get the expected size of the decompressed data.
        mz_zip_archive_file_stat lStat;
        mz_zip_reader_file_stat(&lZip, lIndex, &lStat);

        // Attempt to read the decompressed data into this file's buffer.
        mBuffer.resize(lStat.m_uncomp_size);
        if (
            mz_zip_reader_extract_to_mem(&lZip, lIndex, mBuffer.data(),
                lStat.m_uncomp_size, 0) == MZ_FALSE
        )
        {
            auto lErrorMsg = mz_zip_get_error_string(mz_zip_get_last_error(&lZip));
            mz_zip_reader_end(&lZip);
            ACE_THROW(
                std::runtime_error,
                "{}: Could not extract '{}' from archive file '{}' - {}!",
                "VirtualArchiveFile", pEntryName, pArchivePath.string(), lErrorMsg
            );
        }

        mSize = lStat.m_uncomp_size;
        mPosition = 0;

        mz_zip_reader_end(&lZip);

    }

    /* Public Methods *********************************************************/

    std::size_t VirtualArchiveFile::Read (
        void*               pBuffer, 
        const std::size_t&  pBytes
    )
    {
        if (pBuffer == nullptr)
        {
            throw std::invalid_argument { "Read buffer is null!" };
        }
        else if (mPosition >= mSize)
        {
            return 0;
        }

        // Determine the number of bytes which need to be read.
        std::size_t lBytesToRead = std::min(
            (pBytes == astd::npos) ? mSize : pBytes, 
            mSize - mPosition
        );
        std::memcpy(pBuffer, mBuffer.data() + mPosition, lBytesToRead);

        // Move the read cursor by the number of bytes read, then return the
        // number of bytes read.
        mPosition += lBytesToRead;
        return lBytesToRead;
    }

    bool VirtualArchiveFile::Seek (
        const std::size_t&  pOffset, 
        FileSeekPoint       pPoint
    )
    {
        std::size_t lNewPosition = 0;
        switch (pPoint)
        {
            case FileSeekPoint::Start:      lNewPosition = pOffset; break;
            case FileSeekPoint::End:        lNewPosition = mSize - pOffset; break;
            case FileSeekPoint::Current:    lNewPosition = mPosition + pOffset; break;
        }

        if (lNewPosition > mSize)
        {
            return false;
        }

        mPosition = lNewPosition;
        return true;
    }

    std::size_t VirtualArchiveFile::Tell () const
    {
        return mPosition;
    }
    
    std::size_t VirtualArchiveFile::GetSize () const
    {
        return mSize;
    }
    
    void VirtualArchiveFile::Close ()
    {
        mBuffer.clear();
        mPosition = mSize = 0;
    }

}
