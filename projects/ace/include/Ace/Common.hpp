/**
 * @file    Ace/Common.hpp
 * @brief   Contains commonly-used includes, macros, constants and type definitions used in the
 *          Ace Engine library, and its client applications.
 */

#pragma once

/* Include Files **********************************************************************************/

#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <memory>
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <typeindex>
#include <filesystem>
#include <cstdint>

#if defined(ACE_USE_STDFLOAT)
    #include <stdfloat>
#endif

namespace fs = std::filesystem;

/* Macros *****************************************************************************************/

#define ACE_API
#define ACE_UNUSED(pVar) (void) pVar

/* Typedefs ***************************************************************************************/

namespace ace
{

    // Signed Integers
    using Int8          = std::int8_t;
    using Int16         = std::int16_t;
    using Int32         = std::int32_t;
    using Int64         = std::int64_t;

    // Unsigned Integers
    using Uint8         = std::uint8_t;
    using Uint16        = std::uint16_t;
    using Uint32        = std::uint32_t;
    using Uint64        = std::uint64_t;

    // Floating-Point Nubmers
    #if defined(ACE_USE_STDFLOAT)
        using Float32   = std::float32_t;
        using Float64   = std::float64_t;
    #else
        using Float32   = float;
        using Float64   = double;
    #endif

    // STL Types
    template <typename T>               using Unique = std::unique_ptr<T>;
    template <typename T>               using Shared = std::shared_ptr<T>;
    template <typename T>               using List = std::vector<T>;
    template <typename T, typename U>   using Map = std::unordered_map<T, U>;
    template <typename T>               using Dictionary = Map<std::string, T>;
    template <typename T>               using Queue = std::queue<T>;
    template <typename... Ts>           using Subroutine = std::function<void(Ts...)>;
    template <typename T>               using Function = std::function<T>;

    // Conformity with Ace Code Style
    using Boolean       = bool;
    using Char          = char;
    using String        = std::string;
    using StringView    = std::string_view;
    using Index         = std::size_t;
    using Size          = std::size_t;
    using Count         = std::size_t;
    using TypeIndex     = std::type_index;
    using Mutex         = std::mutex;
    using SharedMutex   = std::shared_mutex;

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
    inline Unique<T> makeUnique (
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
    inline Shared<T> makeShared (
        As&&... pArgs
    )
    {
        return std::make_shared<T>(std::forward<As>(pArgs)...);
    }

}
