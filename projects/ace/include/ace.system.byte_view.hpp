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
        
        /**
         * @brief   Constructs a `byte_view` which views the given byte buffer.
         * @param   p_buffer    The byte buffer to view into.
         */
        explicit byte_view (byte_buffer& p_buffer);

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
