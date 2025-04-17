/**
 * @file  ace.system.byte_view.hpp
 * @brief Provides a wrapper class for reading from and writing to a byte
 *        buffer.
 */

#pragma once
#include <ace.common.hpp>

namespace ace
{

    /**
     * @brief   Defines the byte buffer which is looked at by the `byte_view`
     *          class.
     */
    using byte_buffer = std::vector<std::uint8_t>;

    /**
     * @brief   The `byte_view` class is a wrapper class used for reading data
     *          from, and writing data to, a `byte_buffer`.
     */
    class ace_api byte_view
    {
    public:

        static constexpr std::size_t npos = static_cast<std::size_t>(-1);

    public:
        
        /**
         * @brief   Constructs a `byte_view` which views the given byte buffer.
         * @param   p_buffer    The byte buffer to view into.
         */
        explicit byte_view (byte_buffer& p_buffer);

        /**
         * @brief   Resets the read cursor to the given position.
         * @param   p_cursor    The new position of the read cursor.
         */
        void reset_read_cursor (const std::size_t p_cursor = 0);

        /**
         * @brief   Reads some binary data of the given size from the byte buffer
         *          being viewed, copying that data into a separate byte buffer
         *          to be returned.
         * @param   p_size      The size of the data to be read and copied.
         * @return  A byte buffer containing the copied data.
         */
        byte_buffer read_binary (std::size_t p_size = npos) const;

        /**
         * @brief   Reads some raw data of the given size from the byte buffer
         *          being viewed.
         * @param   p_data      If successful, a pointer to the buffer to hold
         *                      the data which was read.
         * @param   p_size      The size of the data being read, in bytes.
         * @warning Do not use this function unless you know EXACTLY what you
         *          are doing!
         */
        void read_raw (void* p_data, const std::size_t p_size) const;

        /**
         * @brief   Pushes some binary data of the given size into the byte buffer
         *          being viewed.
         * @param   p_buffer    A handle to the byte buffer to be copied.
         * @param   p_size      The size of the data to be pushed.
         */
        void push_binary (const byte_buffer& p_buffer, std::size_t p_size = npos);

        /**
         * @brief   Pushes some raw data into the byte buffer being viewed.
         * @param   p_data      A pointer to the raw data being pushed.
         * @param   p_size      The size, in bytes, of the data being pushed.
         * @warning Do not use this function unless you know EXACTLY what you
         *          are doing!
         */
        void push_raw (const void* p_data, const std::size_t p_size);

        /**
         * @brief   Reads some data of the given `type` from the byte buffer
         *          being viewed, then advances the view's read index.
         * @tparam  type        The type of the data being read.
         * @return  The data which was read from the viewed buffer.
         */
        template <typename type>
        const type read () const;

        /**
         * @brief   Pushes some data of the given `type` into the end of the
         *          byte buffer being viewed.
         * @tparam  type        The type of the data being pushed.
         * @param   p_data      The data being pushed.
         */
        template <typename type>
        void push (const type& p_data);

    private:
        
        /**
         * @brief   The byte buffer being view and manipulated by this
         *          `byte_view`.
         */
        byte_buffer& m_buffer;

        /**
         * @brief   The index of the next read from the byte buffer being
         *          viewed.
         */
        mutable std::size_t m_read_index = 0;

    };

}
