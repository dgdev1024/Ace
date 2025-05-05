/**
 * @file    Ace/Common.hpp
 * @brief   Contains commonly-used includes, macros, constants and type definitions used in the
 *          Ace Engine library, and its client applications.
 */

#pragma once

/* Include Files **********************************************************************************/

#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <string_view>
#include <vector>
#include <span>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <memory>
#include <any>
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <typeindex>
#include <filesystem>
#include <bit>
#include <random>
#include <stdfloat>
#include <concepts>
#include <future>
#include <cstdint>

namespace fs = std::filesystem;

static_assert(std::endian::native == std::endian::little,
    "The Ace Engine requires a little-endian host machine.");
static_assert(sizeof(std::size_t) == sizeof(std::uint64_t),
    "The Ace Engine requires `std::size_t` to be the same length as `std::uint64_t`.");

/* Macros *****************************************************************************************/

#define ACE_API
#define ACE_UNUSED(pVar) (void) pVar

/* Constants **************************************************************************************/

namespace ace
{

    /**
     * @def     `ace::ENGINE_MAJOR_VERSION`
     * @brief   The major version of the Ace Engine Library.
     */
    constexpr std::uint8_t  ENGINE_MAJOR_VERSION    = 0x00;

    /**
     * @def     `ace::ENGINE_MINOR_VERSION`
     * @brief   The minor version of the Ace Engine Library.
     */
    constexpr std::uint8_t  ENGINE_MINOR_VERSION    = 0x00;

    /**
     * @def     `ace::ENGINE_REVISION_NUMBER`
     * @brief   The revision number of the Ace Engine Library.
     */
    constexpr std::uint16_t ENGINE_REVISION_NUMBER  = 0x0000;

}

/* Helper Functions *******************************************************************************/

namespace ace
{

    /**
     * @fn      `ace::makeUnique`
     * @brief   Creates a heap-allocated unique smart pointer to an object of type `T`.
     * 
     * @tparam  T       The type of the object to be created.
     * @tparam  As      The types of the arguments passed into the object's constructor.
     * 
     * @param   pArgs   The arguments, if any, passed into the object's constructor.
     */
    template <typename T, typename... As>
    inline std::unique_ptr<T> makeUnique (
        As&&... pArgs
    )
    {
        return std::make_unique<T>(std::forward<As>(pArgs)...);
    }

    /**
     * @fn      `ace::makeShared`
     * @brief   Creates a heap-allocated, reference-counted shared smart pointer to an object of 
     *          type `T`.
     * 
     * @tparam  T       The type of the object to be created.
     * @tparam  As      The types of the arguments passed into the object's constructor.
     * 
     * @param   pArgs   The arguments, if any, passed into the object's constructor.
     */
    template <typename T, typename... As>
    inline std::shared_ptr<T> makeShared (
        As&&... pArgs
    )
    {
        return std::make_shared<T>(std::forward<As>(pArgs)...);
    }

}
