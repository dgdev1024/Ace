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
#include <vector>
#include <queue>
#include <unordered_map>
#include <cstdint>
#include <cassert>
#include <ace.config.hpp>

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
