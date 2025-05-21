/**
 * @file    Ace/System/IVirtualFile.hpp
 * @brief   Provides an abstract interface representing a file loaded from the
 *          virtual filesystem (VFS).
 */

#pragma once
#include <Ace/Common.hpp>

namespace ace
{

    /**
     * @brief   Enumerates points where to seek from in a virtual file.
     */
    enum class FileSeekPoint
    {
        Start,
        End,
        Current
    };
    
    /**
     * @brief   An abstract interface representing a file loaded from the
     *          virtual filesystem (VFS).
     */
    class ACE_API IVirtualFile
    {
    public:

        /**
         * @brief   The virtual destructor.
         */
        virtual ~IVirtualFile () = default;

        /**
         * @brief   Reads the given number of bytes into the data buffer pointed
         *          to by `pBuffer`.
         * 
         * @param   pBuffer     A raw pointer to the buffer to be read into.
         * @param   pBytes      The number of bytes to be read from the file.
         *                      Specify `-1` to read the whole file.
         * 
         * @return  The number of bytes read into `pBuffer`, or `-1` if there
         *          was an error.
         * 
         * @throws  `std::invalid_argument` should be thrown if `pBuffer` is
         *          `nullptr`.
         * 
         * @warning `pBuffer` is a pointer to raw data (`void*`). Make sure that
         *          this pointer is not `nullptr`, and that the buffer being
         *          pointed to is large enough to hold the data being read!
         */
        virtual std::size_t Read (
            void*               pBuffer, 
            const std::size_t&  pBytes = (std::size_t) -1
        ) = 0;

        /**
         * @brief   Seeks to a new position in the opened file.
         * 
         * @param   pOffset     The offset, in bytes, to seek to.
         * @param   pOrigin     The origin point, from which to start seeking.
         * 
         * @param   `true` if the seek is successful; `false` otherwise.
         */
        virtual bool Seek (
            const std::size_t&  pOffset, 
            FileSeekPoint       pPoint = FileSeekPoint::Start
        ) = 0;

        /**
         * @brief   Retrieves the current position of the read cursor in the
         *          opened file, relative to the start of the file.
         * 
         * If this method is called while the read cursor is at the end of the
         * file, then this method can be used to determine the file's size, just
         * like @a `ace::IVirtualFile::Size()`.
         * 
         * @return  The position, in bytes, of the opened file's read cursor.
         */
        virtual std::size_t Tell () const = 0;

        /**
         * @brief   Retrieves the total size of the opened file.
         * 
         * @return  The size of the opened file, in bytes.
         */
        virtual std::size_t GetSize () const = 0;

        /**
         * @brief   Closes the virtual file, rendering any subsequent reads and
         *          seeks invalid.
         */
        virtual void Close () = 0;

    };

}
