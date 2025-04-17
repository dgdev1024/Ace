/**
 * @file    ace.system.asset_information.hpp
 * @brief   Provides a structure defining important information about an asset.
 */

#pragma once
#include <ace.common.hpp>

namespace ace
{

    /**
     * @brief   The `asset_information` structure defines important information
     *          describing an asset.
     */
    struct asset_information
    {

        /**
         * @brief   The starting offset of the asset's compressed data in an
         *          asset file buffer, relative to the start of its asset data
         *          table.
         */
        std::size_t offset = 0;

        /**
         * @brief   The expected size of the asset's compressed data.
         */
        std::size_t compressed_size = 0;

        /**
         * @brief   The expected size of the asset's data once it is
         *          de-compressed.
         */
        std::size_t decompressed_size = 0;

        /**
         * @brief   The asset's checksum, for validation purposes.
         * 
         * The asset checksum is calculated by adding together each byte of the
         * asset's compressed data, then performing a bitwise XOR between the
         * sum and the asset data's offset.
         */
        std::size_t checksum = 0;

        /**
         * @brief   The asset's name. It must be null-terminated.
         */
        char        name[asset_name_max_strlen] = { 0 };

    };

    static_assert(sizeof(asset_information) == 256,
        "Asset information structure must be 256 bytes in length.");

}
