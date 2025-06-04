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
     * @brief   A constant representing one-half in the given templated type.
     * 
     * @tparam  T   The floating-point type represented.
     */
    template <FloatingPoint T> constexpr T HALF = T { 0.5 };

    /**
     * @brief   A quick constant representing `pi`.
     * 
     * @tparam  T   The floating-point type represented.
     */
    template <FloatingPoint T> constexpr T PI = std::numbers::pi_v<T>;

    /**
     * @brief   A quick constant representing an epislon value close enough
     *          to zero.
     * 
     * @tparam  T   The floating-point type represented.
     */
    template <FloatingPoint T> constexpr T EPSILON =
        std::numeric_limits<T>::epsilon();

    /**
     * @brief   Converts an angle from degrees to radians.
     * 
     * @tparam  T           The floating-point type of the angle being converted.
     * 
     * @param   pDegrees    The angle, in degrees.
     * 
     * @return  The angle, in radians.
     */
    template <FloatingPoint T>
    inline constexpr T ToRadians (
        const T pDegrees
    ) noexcept
    {
        return pDegrees * (PI<T> / static_cast<T>(180));
    }

    /**
     * @brief   Converts an angle from radians to degrees.
     * 
     * @tparam  T           The floating-point type of the angle being converted.
     * 
     * @param   pRadians    The angle, in radians.
     * 
     * @return  The angle, in degrees.
     */
    template <FloatingPoint T>
    inline constexpr T ToDegrees (
        const T pRadians
    ) noexcept
    {
        return pRadians * (static_cast<T>(180) / PI<T>);
    }

    /**
     * @brief   Checks to see if two scalar values are close enough to equal, 
     *          using an epsilon value.
     * 
     * @tparam  T   The numeric types of the values to check.
     * 
     * @param   pFirst      The frist value.
     * @param   pSecond     The second value.
     * @param   pEpsilon    The epsilon value to use.
     * 
     * @return  `true` if the values are close enough to equal; `false` if not.
     */
    template <Numeric T>
    inline constexpr bool EpsilonEqual (
        const T pFirst,
        const T pSecond,
        const T pEpsilon = EPSILON<T>
    ) noexcept
    {
        return std::fabs(pFirst - pSecond) <= pEpsilon;
    }

}
