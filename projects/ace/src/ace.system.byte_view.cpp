/**
 * @file  ace.system.byte_view.cpp
 */

#include <ace.pch.hpp>
#include <ace.system.byte_view.hpp>

namespace ace
{

    /* Constructors and Destructor ********************************************/

    byte_view::byte_view (byte_buffer& p_buffer) :
        m_buffer        { p_buffer },
        m_read_index    { 0 }
    {

    }

    /* Public Methods *********************************************************/

    void byte_view::reset_read_cursor (const std::size_t p_cursor)
    {
        if (p_cursor == npos || p_cursor > m_buffer.size())
        {
            m_read_index = 0;
        }
        else
        {
            m_read_index = p_cursor;
        }
    }

    byte_buffer byte_view::read_binary (std::size_t p_size) const
    {
        if (p_size == npos)
        {
            p_size = std::distance(m_buffer.begin() + m_read_index,
                m_buffer.end());
        }
        else if (m_read_index + p_size > m_buffer.size())
        {
            _ace_throw(std::out_of_range,
                "Byte view binary read index {} is out of range!",
                    m_read_index);
        }

        byte_buffer l_return(p_size, 0x00);
        std::copy(m_buffer.begin() + m_read_index,
            m_buffer.begin() + m_read_index + p_size, l_return.begin());
        m_read_index += p_size;

        return l_return;
    }

    void byte_view::read_raw (void* p_data, const std::size_t p_size) const
    {
        if (p_data == nullptr || p_size == 0)
        {
            _ace_throw(std::invalid_argument, "Data and/or size is not valid!");
        }

        if (m_read_index + p_size > m_buffer.size())
        {
            _ace_throw(std::out_of_range,
                "Byte view raw read index {} is out of range!",
                    m_read_index);
        }

        std::memcpy(p_data, m_buffer.data() + m_read_index, p_size);
        m_read_index += p_size;
    }

    void byte_view::push_binary (const byte_buffer& p_buffer, std::size_t p_size)
    {
        if (p_size == npos || p_size > p_buffer.size())
        {
            p_size = p_buffer.size();
        }

        auto l_old_size = m_buffer.size();
        m_buffer.resize(l_old_size + p_size);
        std::copy(p_buffer.begin(), p_buffer.end(), m_buffer.begin() + l_old_size);
    }

    void byte_view::push_raw (const void* p_data, const std::size_t p_size)
    {
        if (p_data == nullptr || p_size == 0)
        {
            _ace_throw(std::invalid_argument, "Data and/or size is not valid!");
        }

        auto l_old_size = m_buffer.size();
        m_buffer.resize(l_old_size + p_size);
        std::memcpy(m_buffer.data() + l_old_size, p_data, p_size);
    }

    template <>
    const std::uint8_t byte_view::read<std::uint8_t> () const
    {
        if (m_read_index + 1 > m_buffer.size())
        {
            _ace_throw(std::out_of_range, 
                "Byte view read index {} is out of range!", m_read_index);
        }

        return m_buffer.at(m_read_index++);
    }

    template <>
    const std::uint16_t byte_view::read<std::uint16_t> () const
    {
        if (m_read_index + 2 > m_buffer.size())
        {
            _ace_throw(std::out_of_range, 
                "Byte view read index {} is out of range!", m_read_index);
        }

        std::uint8_t l_byte0 = m_buffer.at(m_read_index++);
        std::uint8_t l_byte1 = m_buffer.at(m_read_index++);

        return (
            (static_cast<std::uint16_t>(l_byte0) <<  0) |
            (static_cast<std::uint16_t>(l_byte1) <<  8)
        );
    }

    template <>
    const std::uint32_t byte_view::read<std::uint32_t> () const
    {
        if (m_read_index + 2 > m_buffer.size())
        {
            _ace_throw(std::out_of_range, 
                "Byte view read index {} is out of range!", m_read_index);
        }

        std::uint8_t l_byte0 = m_buffer.at(m_read_index++);
        std::uint8_t l_byte1 = m_buffer.at(m_read_index++);
        std::uint8_t l_byte2 = m_buffer.at(m_read_index++);
        std::uint8_t l_byte3 = m_buffer.at(m_read_index++);

        return (
            (static_cast<std::uint32_t>(l_byte0) <<  0) |
            (static_cast<std::uint32_t>(l_byte1) <<  8) |
            (static_cast<std::uint32_t>(l_byte2) << 16) |
            (static_cast<std::uint32_t>(l_byte3) << 24)
        );
    }

    template <>
    const std::uint64_t byte_view::read<std::uint64_t> () const
    {
        if (m_read_index + 2 > m_buffer.size())
        {
            _ace_throw(std::out_of_range, 
                "Byte view read index {} is out of range!", m_read_index);
        }

        std::uint8_t l_byte0 = m_buffer.at(m_read_index++);
        std::uint8_t l_byte1 = m_buffer.at(m_read_index++);
        std::uint8_t l_byte2 = m_buffer.at(m_read_index++);
        std::uint8_t l_byte3 = m_buffer.at(m_read_index++);
        std::uint8_t l_byte4 = m_buffer.at(m_read_index++);
        std::uint8_t l_byte5 = m_buffer.at(m_read_index++);
        std::uint8_t l_byte6 = m_buffer.at(m_read_index++);
        std::uint8_t l_byte7 = m_buffer.at(m_read_index++);

        return (
            (static_cast<std::uint64_t>(l_byte0) <<  0) |
            (static_cast<std::uint64_t>(l_byte1) <<  8) |
            (static_cast<std::uint64_t>(l_byte2) << 16) |
            (static_cast<std::uint64_t>(l_byte3) << 24) |
            (static_cast<std::uint64_t>(l_byte4) << 32) |
            (static_cast<std::uint64_t>(l_byte5) << 40) |
            (static_cast<std::uint64_t>(l_byte6) << 48) |
            (static_cast<std::uint64_t>(l_byte7) << 56)
        );
    }

    template <>
    const std::int8_t byte_view::read<std::int8_t> () const
    {   
        return static_cast<std::int8_t>(read<std::uint8_t>());
    }

    template <>
    const std::int16_t byte_view::read<std::int16_t> () const
    {   
        return static_cast<std::int16_t>(read<std::uint16_t>());
    }

    template <>
    const std::int32_t byte_view::read<std::int32_t> () const
    {   
        return static_cast<std::int32_t>(read<std::uint32_t>());
    }

    template <>
    const std::int64_t byte_view::read<std::int64_t> () const
    {   
        return static_cast<std::int64_t>(read<std::uint64_t>());
    }

    template <>
    const float byte_view::read<float> () const
    {
        return std::bit_cast<float>(read<std::uint32_t>());
    }

    template <>
    const double byte_view::read<double> () const
    {
        return std::bit_cast<double>(read<std::uint64_t>());
    }

    template <>
    const bool byte_view::read<bool> () const
    {
        return (read<std::uint8_t>() != 0);
    }

    template <>
    const char byte_view::read<char> () const
    {
        return static_cast<char>(read<std::uint8_t>());
    }

    template <>
    const std::string byte_view::read<std::string> () const
    {
        auto l_iter = std::find(m_buffer.begin() + m_read_index, m_buffer.end(),
            0x00);
        if (l_iter == m_buffer.end())
        {
            _ace_throw(std::out_of_range, 
                "Byte view read string at index {} is not null-terminated!", 
                    m_read_index);
        }

        std::string l_result;
        l_result.resize(l_iter - (m_buffer.begin() + m_read_index));
        std::copy(m_buffer.begin() + m_read_index, l_iter, l_result.begin());
        m_read_index = std::distance(m_buffer.begin(), l_iter) + 1;

        return l_result;
    }

    template <>
    void byte_view::push<std::uint8_t> (const std::uint8_t& p_data)
    {
        m_buffer.push_back(p_data);
    }

    template <>
    void byte_view::push<std::uint16_t> (const std::uint16_t& p_data)
    {
        m_buffer.push_back(p_data >>  0);
        m_buffer.push_back(p_data >>  8);
    }

    template <>
    void byte_view::push<std::uint32_t> (const std::uint32_t& p_data)
    {
        m_buffer.push_back(p_data >>  0);
        m_buffer.push_back(p_data >>  8);
        m_buffer.push_back(p_data >> 16);
        m_buffer.push_back(p_data >> 24);
    }

    template <>
    void byte_view::push<std::uint64_t> (const std::uint64_t& p_data)
    {
        m_buffer.push_back(p_data >>  0);
        m_buffer.push_back(p_data >>  8);
        m_buffer.push_back(p_data >> 16);
        m_buffer.push_back(p_data >> 24);
        m_buffer.push_back(p_data >> 32);
        m_buffer.push_back(p_data >> 40);
        m_buffer.push_back(p_data >> 48);
        m_buffer.push_back(p_data >> 56);
    }

    template <>
    void byte_view::push<std::int8_t> (const std::int8_t& p_data)
    {
        push<std::uint8_t>(static_cast<const std::uint8_t&>(p_data));
    }

    template <>
    void byte_view::push<std::int16_t> (const std::int16_t& p_data)
    {
        push<std::uint16_t>(static_cast<const std::uint16_t&>(p_data));
    }

    template <>
    void byte_view::push<std::int32_t> (const std::int32_t& p_data)
    {
        push<std::uint32_t>(static_cast<const std::uint32_t&>(p_data));
    }

    template <>
    void byte_view::push<std::int64_t> (const std::int64_t& p_data)
    {
        push<std::uint64_t>(static_cast<const std::uint64_t&>(p_data));
    }

    template <>
    void byte_view::push<float> (const float& p_data)
    {
        push<std::uint32_t>(std::bit_cast<std::uint32_t>(p_data));
    }

    template <>
    void byte_view::push<double> (const double& p_data)
    {
        push<std::uint64_t>(std::bit_cast<std::uint64_t>(p_data));
    }

    template <>
    void byte_view::push<bool> (const bool& p_data)
    {
        push<std::uint8_t>((p_data == true) ? 0x01 : 0x00);
    }

    template <>
    void byte_view::push<char> (const char& p_data)
    {
        push<std::uint8_t>(static_cast<const std::uint8_t&>(p_data));
    }

    template <>
    void byte_view::push<std::string> (const std::string& p_data)
    {
        auto l_old_size = m_buffer.size();
        m_buffer.resize(m_buffer.size() + p_data.length() + 1, 0x00);
        std::copy(p_data.begin(), p_data.end(), m_buffer.begin() + l_old_size);
    }

}
