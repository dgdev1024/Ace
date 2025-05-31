/**
 * @file    Ace/Maths/MathsUtils.hpp
 * @brief   Contains include files, constants and functions commonly used by the
 *          Ace Engine's mathematics library.
 */

#pragma once
#include <algorithm>
#include <array>
#include <initializer_list>

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>

namespace ace
{

    /**
     * @brief   A concept requiring template types to be numeric - either
     *          integer or floating-point.
     */
    template <typename T>
    concept Numeric = (
        std::is_integral_v<T> ||
        std::is_floating_point_v<T>
    );

    /**
     * @brief   A concept requiring template types to be floating-point.
     */
    template <typename T>
    concept FloatingPoint = (
        std::is_floating_point_v<T>
    );

    /**
     * @brief   Constants representing zero and one in the given templated type.
     * 
     * @tparam  T   The numeric type represented.
     */
    template <Numeric T> constexpr T ZERO   = T { 0 };
    template <Numeric T> constexpr T ONE    = T { 1 };
    template <Numeric T> constexpr T TWO    = T { 2 };

    /**
     * @brief   Checks to see if two scalar values are close enough to equal, 
     *          using an epsilon value.
     * 
     * @tparam  T   The numeric types of the values to check.
     * 
     * @param   pFirst      The frist value.
     * @param   pSecond     The second value.
     * 
     * @return  `true` if the values are close enough to equal; `false` if not.
     */
    template <Numeric T>
    inline constexpr bool EpsilonEqual (
        const T pFirst,
        const T pSecond
    ) noexcept
    {
        return std::fabs(pFirst - pSecond) <= 
            std::numeric_limits<T>::epsilon();
    }

}
