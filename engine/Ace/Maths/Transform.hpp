/**
 * @file    Ace/Maths/Transform.hpp
 * @brief   Provides functions used for generating transformation matrices.
 */

#pragma once
#include <Ace/Maths/Matrix4.hpp>

namespace ace
{

    /**
     * @brief   Generates a transformation matrix used to scale a vector by
     *          the given factors.
     * 
     * @tparam  T   The floating-point type represented.
     * 
     * @param   pVector     A 3D vector containing the scaling factors.
     * 
     * @return  The generated scaling matrix.
     */
    template <FloatingPoint T>
    inline constexpr Matrix4<T> Scale (
        const Vector3<T>&   pVector
    ) noexcept
    {
        return Matrix4<T> {
            pVector.mX, ZERO<T>, ZERO<T>, ZERO<T>,
            ZERO<T>, pVector.mY, ZERO<T>, ZERO<T>,
            ZERO<T>, ZERO<T>, pVector.mZ, ZERO<T>,
            ZERO<T>, ZERO<T>, ZERO<T>   , ONE<T>
        };
    }

    /**
     * @brief   Generates a transformation matrix used for moving a vector by
     *          the given magnitude.
     * 
     * @tparam  T   The floating-point type represented.
     * 
     * @param   pVector     A 3D vector containing the translation magnitude.
     * 
     * @return  The generated translation matrix.
     */
    template <FloatingPoint T>
    inline constexpr Matrix4<T> Translate (
        const Vector3<T>&   pVector
    ) noexcept
    {
        return Matrix4<T> {
            ONE<T>,     ZERO<T>,    ZERO<T>,    ZERO<T>,
            ZERO<T>,    ONE<T>,     ZERO<T>,    ZERO<T>,
            ZERO<T>,    ZERO<T>,    ONE<T>,     ZERO<T>,
            pVector.mX, pVector.mY, pVector.mZ, ONE<T>
        };
    }

    /**
     * @brief   Generates a transformation matrix used for rotating a point by
     *          the given angle along the X axis.
     * 
     * @tparam  T   The floating-point type of the angle.
     * 
     * @param   pAngle  The rotation angle, in radians.
     * 
     * @return  The generated rotation matrix.
     */
    template <FloatingPoint T>
    inline constexpr Matrix4<T> RotateX (
        const T pAngle
    ) noexcept
    {
        auto lSine   = std::sin(pAngle);
        auto lCosine = std::cos(pAngle);

        return Matrix4<T> {
            ONE<T>,     ZERO<T>,    ZERO<T>,    ZERO<T>,
            ZERO<T>,    lCosine,    lSine,      ZERO<T>,
            ZERO<T>,    -lSine,     lCosine,    ZERO<T>,
            ZERO<T>,    ZERO<T>,    ZERO<T>,    ONE<T>
        };
    }

    /**
     * @brief   Generates a transformation matrix used for rotating a point by
     *          the given angle along the Y axis.
     * 
     * @tparam  T   The floating-point type of the angle.
     * 
     * @param   pAngle  The rotation angle, in radians.
     * 
     * @return  The generated rotation matrix.
     */
    template <FloatingPoint T>
    inline constexpr Matrix4<T> RotateY (
        const T pAngle
    ) noexcept
    {
        auto lSine   = std::sin(pAngle);
        auto lCosine = std::cos(pAngle);

        return Matrix4<T> {
            lCosine,    ZERO<T>,    -lSine,     ZERO<T>,
            ZERO<T>,    ONE<T>,     ZERO<T>,    ZERO<T>,
            lSine,      ZERO<T>,    lCosine,    ZERO<T>,
            ZERO<T>,    ZERO<T>,    ZERO<T>,    ONE<T>
        };
    }

    /**
     * @brief   Generates a transformation matrix used for rotating a point by
     *          the given angle along the Z axis.
     * 
     * @tparam  T   The floating-point type of the angle.
     * 
     * @param   pAngle  The rotation angle, in radians.
     * 
     * @return  The generated rotation matrix.
     */
    template <FloatingPoint T>
    inline constexpr Matrix4<T> RotateZ (
        const T pAngle
    ) noexcept
    {
        auto lSine   = std::sin(pAngle);
        auto lCosine = std::cos(pAngle);

        return Matrix4<T> {
            lCosine,    lSine,      ZERO<T>,    ZERO<T>,
            -lSine,     lCosine,    ZERO<T>,    ZERO<T>,
            ZERO<T>,    ZERO<T>,    ONE<T>,     ZERO<T>,
            ZERO<T>,    ZERO<T>,    ZERO<T>,    ONE<T>
        };
    }

    /**
     * @brief   Generates a transformation matrix used for rotating a point by
     *          the given angle along an arbitrary axis.
     * 
     * @tparam  T   The floating-point type of the angle.
     * 
     * @param   pAngle  The rotation angle, in radians.
     * @param   pAxis   The arbitrary axis vector. This needs to be a normalized
     *                  unit vector.
     * 
     * @return  The generated transformation matrix.
     */
    template <FloatingPoint T>
    inline constexpr Matrix4<T> Rotate (
        const T             pAngle,
        const Vector3<T>&   pAxis
    ) noexcept
    {
        auto lSine   = std::sin(pAngle);
        auto lCosine = std::cos(pAngle);

        return Matrix4<T> {
            lCosine + (pAxis.mX * pAxis.mX * (ONE<T> - lCosine)),
            pAxis.mY * pAxis.mX * (ONE<T> - lCosine) + pAxis.mZ * lSine,
            pAxis.mZ * pAxis.mX * (ONE<T> - lCosine) - pAxis.mY * lSine,
            ZERO<T>,

            pAxis.mX * pAxis.mY * (ONE<T> - lCosine) - pAxis.mZ * lSine,
            lCosine + (pAxis.mY * pAxis.mY * (ONE<T> - lCosine)),
            pAxis.mZ * pAxis.mY * (ONE<T> - lCosine) + pAxis.mX * lSine,
            ZERO<T>,

            pAxis.mX * pAxis.mZ * (ONE<T> - lCosine) + pAxis.mY * lSine,
            pAxis.mY * pAxis.mZ * (ONE<T> - lCosine) - pAxis.mX * lSine,
            lCosine + (pAxis.mZ * pAxis.mZ * (ONE<T> - lCosine)),
            ZERO<T>,

            ZERO<T>, ZERO<T>, ZERO<T>, ONE<T>
        };
    }

}
