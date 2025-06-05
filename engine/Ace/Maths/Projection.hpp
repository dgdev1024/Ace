/**
 * @file    Ace/Maths/Projection.hpp
 * @brief   Contains functions for generating projection matrices.
 */

#pragma once
#include <Ace/Maths/Matrix4.hpp>

namespace ace
{

    /**
     * @brief   Generates an orthographic project matrix with the given explicit
     *          bounds.
     * 
     * @tparam  T   The floating-point type represented.
     * 
     * @param   pLeft   The left end of the X-plane.
     * @param   pRight  The right end of the X-plane.
     * @param   pBottom The bottom end of the Y-plane.
     * @param   pTop    The top end of the Y-plane.
     * @param   pNear   The near end of the Z-plane.
     * @param   pEnd    The far end of the Z-plane.
     * 
     * @return  The generated orthographic projection matrix.
     */
    template <FloatingPoint T>
    inline constexpr Matrix4<T> Ortho (
        const T pLeft,
        const T pRight,
        const T pBottom,
        const T pTop,
        const T pNear,
        const T pFar
    ) noexcept
    {
        // Scale factors...
        T lInverseWidth  = ONE<T> / (pRight - pLeft);
        T lInverseHeight = ONE<T> / (pTop - pBottom);
        T lInverseDepth  = ONE<T> / (pFar - pNear);

        T lScaleX = TWO<T> * lInverseWidth;
        T lScaleY = TWO<T> * lInverseHeight;
        T lScaleZ = -TWO<T> * lInverseDepth;

        T lTranslateX = -(pRight + pLeft)   * lInverseWidth;
        T lTranslateY = -(pTop + pBottom)   * lInverseHeight;
        T lTranslateZ = -(pFar + pNear)     * lInverseDepth;

        return Matrix4<T> {
            lScaleX,    ZERO<T>,    ZERO<T>,    ZERO<T>,
            ZERO<T>,    lScaleY,    ZERO<T>,    ZERO<T>,
            ZERO<T>,    ZERO<T>,    lScaleZ,    ZERO<T>,
            lTranslateX,lTranslateY,lTranslateZ,ONE<T>
        };
    }

    /**
     * @brief   Generates an orthographic projection matrix using the given
     *          half-height extent and aspect ratio.
     * 
     * @tparam  T   The floating-point type represented.
     * 
     * @param   pExtent         The half-height extent; half of the vertical 
     *                          span at the near plane.
     * @param   pAspectRatio    The aspect ratio; `width / height`.
     * @param   pNear           The near end of the Z-plane.
     * @param   pFar            The far end of the Z-plane.
     * 
     * @return  The generated orthographic projection matrix.
     */
    template <FloatingPoint T>
    inline constexpr Matrix4<T> Ortho (
        const T pExtent,
        const T pAspectRatio,
        const T pNear,
        const T pFar
    ) noexcept
    {
        const T lTop    = pExtent;
        const T lBottom = -pExtent;
        const T lRight  = pAspectRatio * pExtent;
        const T lLeft   = -lRight;
        return Ortho<T>(lLeft, lRight, lBottom, lTop, pNear, pFar);
    }

    /**
     * @brief   Generates an perspective projection matrix using the given
     *          field of view angle and aspect ratio.
     * 
     * @tparam  T   The floating-point type represented.
     * 
     * @param   pFieldOfView    The vertical field of view angle, in radians.
     * @param   pAspectRatio    The aspect ratio; `width / height`.
     * @param   pNear           The near end of the Z-plane.
     * @param   pFar            The far end of the Z-plane.
     * 
     * @return  The generated perspective projection matrix.
     */
    template <FloatingPoint T>
    inline constexpr Matrix4<T> Perspective (
        const T pFieldOfView,
        const T pAspectRatio,
        const T pNear,
        const T pFar
    ) noexcept
    {
        // Calculate the cotangent of half the field of view angle.
        const T lCotangent = ONE<T> / std::tan(pFieldOfView / TWO<T>);

        // Invert the depth range.
        const T lInverseDepth = ONE<T> / (pNear - pFar);

        // Generate the perspective projection matrix.
        return Matrix4<T> {
            lCotangent / pAspectRatio,
            ZERO<T>,
            ZERO<T>,
            ZERO<T>,

            ZERO<T>,
            lCotangent,
            ZERO<T>,
            ZERO<T>,

            ZERO<T>,
            ZERO<T>,
            (pNear + pFar) * lInverseDepth,
            -ONE<T>,

            ZERO<T>,
            ZERO<T>,
            (TWO<T> * pFar * pNear) * lInverseDepth,
            ZERO<T>
        };
    }

}
