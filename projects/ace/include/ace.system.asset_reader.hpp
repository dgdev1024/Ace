/**
 * @file    ace.system.asset_reader.hpp
 * @brief   Contains a class used for un-packing assets from an `.ace` asset
 *          file.
 */

#pragma once
#include <ace.system.byte_view.hpp>
#include <ace.system.asset_information.hpp>

namespace ace
{

    /**
     * @brief   The `asset_reader` class provides an interface for un-packing
     *          asset data from an `.ace` asset file.
     */
    class ace_api asset_reader
    {
    public:
        
        /**
         * @brief   Loads an asset file from the given path.
         * @param   p_path  The path to the asset file to load.
         * @return  `true` if the asset file is loaded and validated
         *          successfully; `false` otherwise.
         */
        bool load_from_file (const fs::path& p_path);

        /**
         * @brief   Checks to see if an asset mapped to the given name is
         *          present in this asset file.
         * @param   p_name      The name of the asset to check for.
         * @return  `true` if the asset is present; `false` otherwise.
         */
        bool has_asset (const std::string& p_name) const;

        /**
         * @brief   Un-packs an asset with the given name from the loaded asset
         *          file.
         * @param   p_name      The name of the asset to un-pack.
         * @param   p_buffer    If un-packed successfully, will contain the
         *                      un-packed asset data.
         * @return  `true` if the asset is un-packed successfully; `false`
         *          otherwise.
         */
        bool read_asset (const std::string& p_name, byte_buffer& p_buffer) const;

    private:

        /**
         * @brief   Reads the metadata at the start of the opened asset file.
         * @param   p_asset_count   The number of assets expected to be found in
         *                          the asset file.
         * @return  `true` if the metadata is read successfully; `false`
         *          otherwise.
         */
        bool read_metadata (std::size_t& p_asset_count);

        /**
         * @brief   Reads the opened asset file's asset information table.
         * @param   p_asset_count   The number of assets expected to be found in
         *                          the asset file.
         * @return  `true` if the information table is read successfully; `false`
         *          otherwise.
         */
        bool read_asset_info (const std::size_t& p_asset_count);

    private:
        
        /**
         * @brief   The file stream handle to the opened asset file.
         */
        mutable std::fstream m_file;

        /**
         * @brief   The loaded asset file's asset information table.
         */
        std::vector<asset_information> m_asset_info;

        /**
         * @brief   The expected size of the asset file's compressed data,
         *          in bytes.
         */
        std::size_t m_compressed_size = 0;

        /**
         * @brief   The expected total size of the asset file's data when
         *          de-compressed, in bytes.
         */
        std::size_t m_decompressed_size = 0;

        /**
         * @brief   The expected starting offset of the asset file's compressed
         *          asset data.
         */
        std::size_t m_start_offset = 0;

    };

}
