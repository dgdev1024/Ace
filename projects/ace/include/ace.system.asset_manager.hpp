/**
 * @file  ace.system.asset_manager.hpp
 * @brief Provides a global interface for hosting the client application's
 *        assets.
 */

#pragma once
#include <ace.system.asset_reader.hpp>

namespace ace
{

    /**
     * @brief The `asset_manager` class provides a global interface for hosting
     *        the client application's assets.
     */
    class ace_api asset_manager
    {
    public:

        /**
         * @brief   Loads an asset file at the given path.
         * @param   p_path  The path to the asset file to load.
         * @return  `true` if the asset file is loaded successfully; `false`
         *          otherwise.
         */
        bool load_asset_file (const fs::path& p_path);

        /**
         * @brief   Checks for the presence of an asset with the given name in
         *          any of the asset readers attached to this asset manager.
         * @param   p_asset_name    The name of the asset to check for.
         * @return  `true` if the asset is found; `false` otherwise.
         */
        bool has_asset (const std::string& p_asset_name) const;

        /**
         * @brief   Un-packs an asset with the given name from an asset reader
         *          attached to the asset manager.
         * @param   p_name      The name of the asset to un-pack.
         * @param   p_buffer    If un-packed successfully, will contain the
         *                      un-packed asset data.
         * @return  `true` if the asset is un-packed successfully; `false`
         *          otherwise.
         */
        bool read_asset (const std::string& p_name, byte_buffer& p_buffer) const;

    private:
        
        /**
         * @brief   The list of asset readers attached to this asset manager.
         */
        std::vector<asset_reader> m_asset_readers;

    };

}
