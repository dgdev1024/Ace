/**
 * @file    ace.system.asset_writer.hpp
 * @brief   Contains a class used for packing assets into `.ace` asset files.
 */

#pragma once
#include <ace.system.file_io.hpp>
#include <ace.system.asset_information.hpp>

namespace ace
{

    /**
     * @brief   The `asset_writer` class provides an interface for packing
     *          a client application's assets into an `.ace` asset file.
     */
    class asset_writer
    {
    public:

        /**
         * @brief   Saves the compressed asset data to the given file.
         * @param   p_path      The path to the asset file to write.
         * @return  `true` if the asset file is saved successfully; `false`
         *          otherwise.
         */
        bool save_to_file (const fs::path& p_path);

        /**
         * @brief   Compresses the provided asset data, then appends the compressed
         *          data to the asset data buffer.
         * @param   p_name      The name of the asset to be added.
         * @param   p_data      The asset data to be added.
         * @return  `true` if the asset data is added successfully; `false`
         *          otherwise.
         */
        bool add_asset_data (const std::string& p_name, const byte_buffer& p_data);

        /**
         * @brief   Compresses the provded asset file, then appends the compressed
         *          data to the asset data buffer.
         * @param   p_name      The name of the asset to be added.
         * @param   p_path      The path to the asset file to be added.
         * @return  `true` if the asset data is added successfully; `false`
         *          otherwise.
         */
        bool add_asset_file (const std::string& p_name, const fs::path& p_path);

        /**
         * @brief   Compresses the provded asset file, then appends the compressed
         *          data to the asset data buffer.
         * @param   p_path      The path to the asset file to be added.
         * @return  `true` if the asset data is added successfully; `false`
         *          otherwise.
         */
        bool add_asset_file (const fs::path& p_path);

    private:

        /**
         * @brief   Writes the asset file's metadata to the given file stream.
         * @param   p_stream    A handle to the file stream to write to.
         * @return  `true` if the metadata is written successfully; `false`
         *          otherwise.
         */
        bool write_metadata (std::fstream& p_stream) const;

        /**
         * @brief   Writes the asset file's asset information table to the
         *          given file stream.
         * @param   p_stream    A handle to the file stream to write to.
         * @return  `true` if the asset information table is written successfully;
         *          `false` otherwise.
         */
        bool write_asset_info (std::fstream& p_stream) const;

        /**
         * @brief   Writes the asset file's asset data to the given file stream.
         * @param   p_stream    A handle to the file stream to write to.
         * @return  `true` if the asset data is written successfully;
         *          `false` otherwise.
         */
        bool write_asset_data (std::fstream& p_stream) const;

    private:

        /**
         * @brief   Contains the asset file's asset information table.
         */
        std::vector<asset_information> m_asset_info;

        /**
         * @brief   The byte buffer containing the asset file's compressed asset
         *          data.
         */
        byte_buffer m_asset_data;

        /**
         * @brief   The total size of the asset file's compressed data, in bytes.
         */
        std::size_t m_compressed_size = 0;

        /**
         * @brief   The expected total size of the asset file's data when
         *          decompressed, in bytes.
         */
        std::size_t m_decompressed_size = 0;

    };

}
