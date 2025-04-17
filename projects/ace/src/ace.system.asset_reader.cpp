/**
 * @file  ace.system.asset_reader.cpp
 */

#include <ace.pch.hpp>
#include <ace.system.file_io.hpp>
#include <ace.system.asset_reader.hpp>

namespace ace
{

    /* Public Methods *********************************************************/

    bool asset_reader::load_from_file (const fs::path& p_path)
    {
        m_file = file_io::open_binary_file(p_path);
        if (m_file.is_open() == false)
        {
            _ace_error("Cannot load asset file '{}'.", p_path.string());
            return false;
        }

        std::size_t l_asset_count = 0;
        bool l_result = 
            read_metadata(l_asset_count) &&
            read_asset_info(l_asset_count);

        if (l_result == false)
        {
            _ace_error("While attempting to load asset file '{}'.", 
                p_path.string());
        }

        return l_result;
    }

    bool asset_reader::has_asset (const std::string& p_name) const
    {
        if (p_name.empty() == true)
        {
            _ace_error("Cannot check for asset - asset name string is blank.");
            return false;
        }
        else if (p_name.length() >= asset_name_max_strlen)
        {
            _ace_error("Cannot check for asset - asset name string is too long.");
            return false;
        }

        auto l_iter = std::find_if(m_asset_info.begin(), m_asset_info.end(),
            [&] (const asset_information& l_info)
            {
                return std::strncmp(p_name.c_str(), l_info.name,
                    asset_name_max_strlen) == 0;
            }
        );
        return (l_iter != m_asset_info.end());
    }
    
    bool asset_reader::read_asset (const std::string& p_name, byte_buffer& p_buffer) const
    {
        // Ensure that the given asset name is neither blank nor too long.
        if (p_name.empty() == true)
        {
            _ace_error("Cannot read asset - asset name string is blank.");
            return false;
        }
        else if (p_name.length() >= asset_name_max_strlen)
        {
            _ace_error("Cannot read asset - asset name string is too long.");
            return false;
        }

        // Ensure that an asset with that name is present in the asset file's
        // information table.
        auto l_iter = std::find_if(m_asset_info.begin(), m_asset_info.end(),
            [&] (const asset_information& l_info)
            {
                return std::strncmp(p_name.c_str(), l_info.name,
                    asset_name_max_strlen) == 0;
            }
        );
        if (l_iter == m_asset_info.end())
        {
            return false;
        }

        // Read the compressed asset data from the opened asset file.
        byte_buffer l_source(l_iter->compressed_size);
        m_file.seekg(m_start_offset + l_iter->offset, m_file.beg);
        m_file.read(reinterpret_cast<char*>(l_source.data()), l_source.size());
        if (m_file.fail() == true)
        {
            _ace_error("Cannot read asset - error reading asset data from file.");
            return false;
        }

        // De-compress the compressed asset data.
        byte_buffer l_dest(l_iter->decompressed_size);
        const std::int32_t l_decompressed_size =
            LZ4_decompress_safe(
                reinterpret_cast<const char*>(l_source.data()),
                reinterpret_cast<char*>(l_dest.data()),
                l_iter->compressed_size,
                l_dest.size()
            );
        if (l_decompressed_size < 0)
        {
            _ace_error("Cannot read asset - error de-compressing asset data.");
            return false;
        }
        else if (l_decompressed_size != l_iter->decompressed_size)
        {
            _ace_error("Cannot read asset - decompressed size mismatch: {} != {}.",
                l_decompressed_size, l_iter->decompressed_size);
            return false;
        }

        std::swap(p_buffer, l_dest);
        return true;
    }

    /* Private Methods ********************************************************/

    bool asset_reader::read_metadata (std::size_t& p_asset_count)
    {
        byte_buffer l_metadata_buffer(asset_metadata_size, 0x00);
        byte_view   l_metadata { l_metadata_buffer };

        m_file.seekg(0, m_file.beg);
        m_file.read(reinterpret_cast<char*>(l_metadata_buffer.data()),
            l_metadata_buffer.size());
        if (m_file.eof() == true)
        {
            _ace_error("Unexpected end-of-file while reading asset metadata.");
            return false;
        }
        else if (m_file.good() == false)
        {
            _ace_error("Read error while reading asset metadata.");
            return false;
        }

        std::uint32_t l_magic_number = l_metadata.read<std::uint32_t>();
        if (l_magic_number != asset_magic_number)
        {
            _ace_error("Magic number missing from asset metadata.");
            return false;
        }

        std::uint8_t    l_major_version = l_metadata.read<std::uint8_t>(),
                        l_minor_version = l_metadata.read<std::uint8_t>();
        std::uint16_t   l_revision      = l_metadata.read<std::uint16_t>();
        if (l_major_version != engine_major_version)
        {
            _ace_error("Major version mismatch in asset metadata: {} != {}.",
                l_major_version, engine_major_version);
            return false;
        }
        else if (l_minor_version > engine_minor_version)
        {
            _ace_error("Minor version mismatch in asset metadata: {} > {}.",
                l_minor_version, engine_minor_version);
            return false;
        }
        else if (l_revision > engine_revision)
        {
            _ace_debug("Revision mismatch in asset metadata: {} > {}.",
                l_revision, engine_revision);
        }

        p_asset_count = l_metadata.read<std::size_t>();
        m_compressed_size = l_metadata.read<std::size_t>();
        m_decompressed_size = l_metadata.read<std::size_t>();
        m_start_offset = l_metadata.read<std::size_t>();

        std::size_t l_actual_start_offset = asset_metadata_size +
            (p_asset_count * sizeof(asset_information));
        if (m_start_offset != l_actual_start_offset)
        {
            _ace_error("Start offset mismatch after reading metadata: {} != {}.", 
                l_actual_start_offset, m_start_offset);
            return false;
        }

        return true;
    }

    bool asset_reader::read_asset_info (const std::size_t& p_asset_count)
    {
        byte_buffer l_info_buffer(p_asset_count * sizeof(asset_information), 0x00);
        byte_view   l_info { l_info_buffer };
        m_file.read(reinterpret_cast<char*>(l_info_buffer.data()),
            l_info_buffer.size());
        if (m_file.eof() == true)
        {
            _ace_error("Unexpected end-of-file while reading asset information.");
            return false;
        }
        else if (m_file.good() == false)
        {
            _ace_error("Read error while reading asset information.");
            return false;
        }

        std::size_t l_compressed_size = 0, l_decompressed_size = 0;
        asset_information l_asset;
        for (std::size_t i = 0; i < p_asset_count; ++i)
        {
            l_asset.offset                  = l_info.read<std::size_t>();
            l_asset.compressed_size         = l_info.read<std::size_t>();
            l_asset.decompressed_size       = l_info.read<std::size_t>();
            l_asset.checksum                = l_info.read<std::size_t>();
            l_info.read_raw(l_asset.name, asset_name_max_strlen);
            m_asset_info.push_back(l_asset);

            l_compressed_size += l_asset.compressed_size;
            l_decompressed_size += l_asset.decompressed_size;
        }

        if (l_compressed_size != m_compressed_size)
        {
            _ace_error("Total compressed size mismatch: {} != {}.",
                l_compressed_size, m_compressed_size);
            return false;
        }

        if (l_decompressed_size != m_decompressed_size)
        {
            _ace_error("Total decompressed size mismatch: {} != {}.",
                l_decompressed_size, m_decompressed_size);
            return false;
        }

        auto l_current_pos = m_file.tellg();
        if (l_current_pos != m_start_offset)
        {
            _ace_error("Start offset mismatch after reading asset info.");
            return false;
        }

        return true;
    }

}
