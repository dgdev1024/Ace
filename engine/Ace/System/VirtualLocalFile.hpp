/**
 * @file    Ace/System/VirtualLocalFile.hpp
 * @brief   Contains a class representing a local file loaded from disk by the
 *          virtual filesystem (VFS).
 */

#pragma once
#include <Ace/System/IVirtualFile.hpp>

namespace ace
{

    /**
     * @brief   A class representing a local file loaded from disk by the
     *          virtual filesystem (VFS).
     */
    class ACE_API VirtualLocalFile final : public IVirtualFile
    {
    public:

        /**
         * @brief   This constructor opens a file on disk at the given physical
         *          path.
         * 
         * @param   pPath   The path to the local file to open.
         * 
         * @throw   `std::runtime_error` if the file at `pPath` could not be
         *          opened.
         */
        explicit VirtualLocalFile (
            const fs::path& pPath
        );

        /**
         * @brief   The default destructor.
         */
        virtual ~VirtualLocalFile () override;

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
        mutable std::ifstream   mFileStream;    ///< @brief The file stream at which the file is opened.
                std::size_t     mSize = 0;      ///< @brief The size of the opened file, in bytes.        

    };

}
