/**
 * @file    Ace/System/VirtualArchiveFile.cpp
 */

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
        std::size_t lBytesToRead = std::min(pBytes, mSize - mPosition);
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
