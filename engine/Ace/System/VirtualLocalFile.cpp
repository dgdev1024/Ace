/**
 * @file    Ace/System/VirtualLocalFile.cpp
 */

#include <Ace/System/VirtualLocalFile.hpp>

namespace ace
{

    /* Constructors and Destructor ********************************************/

    VirtualLocalFile::VirtualLocalFile (
        const fs::path& pPath
    ) :
        IVirtualFile    {},
        mFileStream     { pPath, std::ios::binary }
    {
        if (mFileStream.is_open() == false)
        {
            throw std::runtime_error {
                std::format("VirtualLocalFile: '{}' could not be opened!",
                    pPath.string())
            };
        }

        mSize = fs::file_size(pPath);
        mFileStream.exceptions(std::ios::badbit | std::ios::failbit);
        mFileStream.seekg(0, std::ios::beg);
    }

    VirtualLocalFile::~VirtualLocalFile ()
    {
        Close();
    }

    /* Public Methods *********************************************************/

    std::size_t VirtualLocalFile::Read (
        void*               pBuffer, 
        const std::size_t&  pBytes
    )
    {
        if (pBuffer == nullptr)
        {
            throw std::invalid_argument { "Read buffer is null!" };
        }

        // Read the requested number of bytes into the buffer.
        mFileStream.read(
            reinterpret_cast<char*>(pBuffer),
            (pBytes == astd::npos) ? mSize : pBytes
        );

        // Return the number of bytes read.
        return mFileStream.gcount();
    }

    bool VirtualLocalFile::Seek (
        const std::size_t&  pOffset, 
        FileSeekPoint       pPoint
    )
    {
        std::ios_base::seekdir lSeekDirection;
        switch (pPoint)
        {
            case FileSeekPoint::Start:      lSeekDirection = std::ios::beg; break;
            case FileSeekPoint::End:        lSeekDirection = std::ios::end; break;
            case FileSeekPoint::Current:    lSeekDirection = std::ios::cur; break;
        }

        mFileStream.clear();    // Clear any EOF or failure bits, if present.
        mFileStream.seekg(pOffset, lSeekDirection);

        // Return true if there are no EOF or failure bits after the seek.
        return mFileStream.good();
    }

    std::size_t VirtualLocalFile::Tell () const
    {
        return mFileStream.tellg();
    }
    
    std::size_t VirtualLocalFile::GetSize () const
    {
        return mSize;
    }
    
    void VirtualLocalFile::Close ()
    {
        if (mFileStream.is_open())
        {
            mFileStream.close();
        }
    }

}
