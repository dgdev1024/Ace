/**
 * @file    ace.system.asset_writer.cpp
 */

#include <ace.pch.hpp>
#include <ace.system.asset_writer.hpp>

namespace ace
{

    /* Public Methods *********************************************************/

    bool asset_writer::save_to_file (const fs::path& p_path)
    {
        std::fstream l_file = file_io::open_binary_file(p_path, true);
        if (l_file.is_open() == false)
        {
            _ace_error("Cannot save asset file '{}'.", p_path.string());
            return false;
        }

        write_metadata(l_file);
        write_asset_info(l_file);
        write_asset_data(l_file);

        l_file.close();
        return true;
    }

    bool asset_writer::add_asset_data (const std::string& p_name,
        const byte_buffer& p_data)
    {
        // Make sure the asset has a name.
        if (p_name.empty() == true)
        {
            _ace_error("Cannot add asset data - asset has no name.");
            return false;
        }

        // Make sure the asset name is not too long.
        if (p_name.length() >= asset_name_max_strlen)
        {
            _ace_error("Cannot add asset data - asset name length is too long.");
            return false;
        }

        // Make sure there is asset data to compress, first.
        if (p_data.empty() == true)
        {
            _ace_error("Cannot add asset data - asset data buffer is empty.");
            return false;
        }

        // Determine the maximum size of the data to compress.
        const std::int32_t l_max_compression_size =
            LZ4_compressBound(static_cast<std::int32_t>(p_data.size()));
        if (l_max_compression_size <= 0)
        {
            _ace_error("Cannot add asset data - cannot determine compression size.");
            return false;
        }

        // Resize the asset data buffer to accomidate this maximum compression
        // size.
        const auto l_old_size = m_asset_data.size();
        m_asset_data.resize(l_old_size + l_max_compression_size);

        // Compress the asset data. Perform the compression in-place within the
        // asset data buffer.
        const std::int32_t l_compressed_data_size =
            LZ4_compress_default(
                reinterpret_cast<const char*>(p_data.data()),
                reinterpret_cast<char*>(m_asset_data.data() + l_old_size),
                static_cast<std::int32_t>(p_data.size()),
                l_max_compression_size
            );
        if (l_compressed_data_size <= 0)
        {
            _ace_error("Cannot add asset data - compression error.");
            m_asset_data.resize(l_old_size);
            return false;
        }

        // Resize the asset buffer to accomidate the *actual* compression size.
        m_asset_data.resize(l_old_size + l_compressed_data_size);

        // Add the asset's information.
        asset_information l_info = {
            .offset = l_old_size,
            .compressed_size = static_cast<std::size_t>(l_compressed_data_size),
            .decompressed_size = p_data.size(),
            .checksum = 0
        };
        std::strncpy(l_info.name, p_name.c_str(), asset_name_max_strlen);
        m_asset_info.push_back(std::move(l_info));

        // Update the total compressed and de-compressed size.
        m_compressed_size += l_compressed_data_size;
        m_decompressed_size += p_data.size();

        return true;
    }

    bool asset_writer::add_asset_file (const std::string& p_name, 
        const fs::path& p_path)
    {
        if (p_name.empty() == true)
        {
            _ace_error("Cannot add asset file - no filename provided.");
            return false;
        }

        byte_buffer l_asset_buffer;
        if (file_io::load_binary_file(p_path, l_asset_buffer) == false)
        {
            _ace_error("Cannot add asset file - unable to open binary file.");
            return false;
        }

        return add_asset_data(p_name, l_asset_buffer);
    }

    bool asset_writer::add_asset_file (const fs::path& p_path)
    {
        return add_asset_file(p_path.string(), p_path);
    }

    /* Private Methods ********************************************************/

    bool asset_writer::write_metadata (std::fstream& p_stream) const
    {
        byte_buffer l_metadata_buffer;
        byte_view   l_metadata { l_metadata_buffer };

        std::size_t l_data_start_point = 
            asset_metadata_size +
            (sizeof(asset_information) * m_asset_info.size());

        l_metadata.push<std::uint32_t>(asset_magic_number);
        l_metadata.push<std::uint8_t>(engine_major_version);
        l_metadata.push<std::uint8_t>(engine_minor_version);
        l_metadata.push<std::uint16_t>(engine_revision);
        l_metadata.push<std::uint64_t>(m_asset_info.size());
        l_metadata.push<std::uint64_t>(m_compressed_size);
        l_metadata.push<std::uint64_t>(m_decompressed_size);
        l_metadata.push<std::uint64_t>(l_data_start_point);

        p_stream.write(reinterpret_cast<const char*>(l_metadata_buffer.data()),
            l_metadata_buffer.size());
        return true;
    }

    bool asset_writer::write_asset_info (std::fstream& p_stream) const
    {
        byte_buffer l_info_buffer;
        byte_view   l_info { l_info_buffer };

        for (const auto& l_asset : m_asset_info)
        {
            l_info.push<std::uint64_t>(l_asset.offset);
            l_info.push<std::uint64_t>(l_asset.compressed_size);
            l_info.push<std::uint64_t>(l_asset.decompressed_size);
            l_info.push<std::uint64_t>(l_asset.checksum);
            l_info.push_raw(l_asset.name, asset_name_max_strlen);
        }

        p_stream.write(reinterpret_cast<const char*>(l_info_buffer.data()),
            l_info_buffer.size());
        return true;
    }

    bool asset_writer::write_asset_data (std::fstream& p_stream) const
    {
        p_stream.write(reinterpret_cast<const char*>(m_asset_data.data()),
            m_asset_data.size());
        return true;
    }

}
