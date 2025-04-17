/**
 * @file  ace.common.hpp
 * @brief Contains commonly-used includes, typedefs and constants.
 */

#pragma once

/* Include Files **************************************************************/

#include <iostream>
#include <fstream>
#include <syncstream>
#include <string>
#include <string_view>
#include <format>
#include <memory>
#include <functional>
#include <typeindex>
#include <filesystem>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cstdint>
#include <cassert>
#include <ace.config.hpp>

namespace fs = std::filesystem;

/* Static Asserts *************************************************************/

static_assert(sizeof(std::size_t) == sizeof(std::uint64_t),
    "Size type must be an unsigned 64-bit integer.");
static_assert(sizeof(float) == sizeof(std::uint32_t),
    "Single-precision float must be 32-bit.");
static_assert(sizeof(double) == sizeof(std::uint64_t),
    "Double-precision float must be 64-bit.");

/* Constants ******************************************************************/

namespace ace
{

    static constexpr std::uint8_t   engine_major_version    = 0x01;
    static constexpr std::uint8_t   engine_minor_version    = 0x00;
    static constexpr std::uint16_t  engine_revision         = 0x0000;
    static constexpr std::uint32_t  asset_magic_number      = 0xACEA55E7;
    static constexpr std::size_t    asset_metadata_size     = 40;
    static constexpr std::size_t    asset_name_max_strlen   = 224;

}

/* Helper Functions ***********************************************************/

namespace ace
{

    /**
     * @brief   Creates a formatted string.
     * @tparam  args        The types of the variadic arguments to format the string with.
     * @param   p_format    The string to be formatted, then returned.
     * @param   p_args      The variadic arguments, if any, to format the string with.
     * @return  The formatted string.
     */
    template <typename... args>
    inline std::string format_string (std::string_view p_format, args&&... p_args)
    {
        return std::vformat(p_format, std::make_format_args(p_args...));
    }

}
