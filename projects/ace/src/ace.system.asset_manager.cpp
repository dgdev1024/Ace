/**
 * @file  ace.system.asset_manager.cpp
 */

#include <ace.pch.hpp>
#include <ace.system.asset_manager.hpp>

namespace ace
{

    bool asset_manager::load_asset_file (const fs::path& p_path)
    {
        return m_asset_readers.emplace_back().load_from_file(p_path);
    }

    bool asset_manager::has_asset (const std::string& p_asset_name) const
    {
        for (const auto& l_reader : m_asset_readers)
        {
            if (l_reader.has_asset(p_asset_name) == true)
            {
                return true;
            }
        }

        return false;
    }

    bool asset_manager::read_asset (const std::string& p_name, byte_buffer& p_buffer) const
    {
        for (const auto& l_reader : m_asset_readers)
        {
            if (l_reader.read_asset(p_name, p_buffer) == true)
            {
                return true;
            }
        }

        ace_error("Asset '{}' not found.", p_name);
        return false;
    }

}