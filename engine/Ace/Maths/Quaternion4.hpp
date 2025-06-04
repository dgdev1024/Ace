/**
 * @file    Ace/Maths/Quaternion4.hpp
 * @brief   Provides a structure used for representing rotations in 3D space.
 */

#pragma once
#include <Ace/Maths/Matrix4.hpp>

namespace ace
{

    /**
     * @brief   A structure representing a rotation in 3D space.
     * 
     * @param   T   The floating-point types of the quaternion's components.
     */
    template <FloatingPoint T>
    struct Quaternion4
    {
    public:
        T   mX; ///< @brief The X component of the quaternion's imagninary vector portion.
        T   mY; ///< @brief The Y component of the quaternion's imagninary vector portion.
        T   mZ; ///< @brief The Z component of the quaternion's imagninary vector portion.
        T   mW; ///< @brief The quaternion's real scalar portion.

    public:

        /**
         * @brief   The default constructor creates an identity quaternion,
         *          representing no rotation.
         */
        constexpr Quaternion4<T> () noexcept :
            mX { ZERO<T> },
            mY { ZERO<T> },
            mZ { ZERO<T> },
            mW { ONE<T> }
        {}

        /**
         * @brief   Constructs a quaternion with the given components.
         * 
         * @param   pX  The imaginary vector portion's X component.
         * @param   pY  The imaginary vector portion's Y component.
         * @param   pZ  The imaginary vector portion's Z component.
         * @param   pW  The real scalar portion.
         */
        constexpr Quaternion4<T> (
            const T pX,
            const T pY,
            const T pZ,
            const T pW
        ) noexcept :
            mX { pX },
            mY { pY },
            mZ { pZ },
            mW { pW }
        {}

        /**
         * @brief   Constructs a quaternion from the given 4D vector.
         * 
         * @param   pVector The vector to create the quaternion from.
         */
        constexpr Quaternion4 (
            const Vector4<T>&   pVector
        ) noexcept :
            mX { pVector.mX },
            mY { pVector.mY },
            mZ { pVector.mZ },
            mW { pVector.mW }
        {}

        /**
         * @brief   Constructs a quaternion from a non-zero axis vector and an
         *          angle.
         * 
         * @param   pAxis       A 3D unit vector representing the axis to rotate
         *                      along.
         * @param   pAngle      The rotation angle, in radians.
         */
        constexpr Quaternion4 (
            const Vector3<T>&   pAxis,
            const T             pAngle
        ) noexcept
        {
            // Calculate the sine and cosine of one-half of the provided angle.
            T   lHalfAngle = pAngle * HALF<T>,
                lHalfAngleSine = std::sin(lHalfAngle),
                lHalfAngleCosine = std::cos(lHalfAngle);

            mX = pAxis.mX * lHalfAngleSine;
            mY = pAxis.mY * lHalfAngleSine;
            mZ = pAxis.mZ * lHalfAngleSine;
            mW = lHalfAngleCosine;
        }

        /**
         * @brief   Constructs a quaternion from the given Euler angles.
         * 
         * @param   pPitch  The pitch angle along the X axis, in radians.
         * @param   pYaw    The yaw angle along the Y axis, in radians.
         * @param   pRoll   The roll angle along the Z axis, in radians.
         */
        constexpr Quaternion4 (
            const T pPitch,
            const T pYaw,
            const T pRoll
        ) noexcept
        {
            // Calculate the sine and cosine of one-half of each of the provided
            // angles.
            T   lHalfPitch          = pPitch * HALF<T>,
                lHalfPitchSine      = std::sin(lHalfPitch),
                lHalfPitchCosine    = std::cos(lHalfPitch),
                lHalfYaw            = pYaw * HALF<T>,
                lHalfYawSine        = std::sin(lHalfYaw),
                lHalfYawCosine      = std::cos(lHalfYaw),
                lHalfRoll           = pRoll * HALF<T>,
                lHalfRollSine       = std::sin(lHalfRoll),
                lHalfRollCosine     = std::cos(lHalfRoll);

            // Q = qYaw * qPitch * qRoll
            // qYaw   = (0, sinY, 0, cosY)
            // qPitch = (sinP, 0, 0, cosP)
            // qRoll  = (0, 0, sinR, cosR)
            // Combined formula:
            mW =    lHalfYawCosine * lHalfPitchCosine * lHalfRollCosine + 
                    lHalfYawSine * lHalfPitchSine * lHalfRollSine;
            mX =    lHalfYawCosine * lHalfPitchSine * lHalfRollCosine + 
                    lHalfYawSine * lHalfPitchCosine * lHalfRollSine;
            mY =    lHalfYawSine * lHalfPitchCosine * lHalfRollCosine - 
                    lHalfYawCosine * lHalfPitchSine * lHalfRollSine;
            mZ =    lHalfYawCosine * lHalfPitchCosine * lHalfRollSine - 
                    lHalfYawSine * lHalfPitchSine * lHalfRollCosine;

            // Ensure the quaternion is a unit quaternion.
            Normalize();
        }

    public:

        /**
         * @brief   Checks to see if this quaternion's and the given quaternion's
         *          components are equal.
         * 
         * @param   pOther  The other quaternion.
         * 
         * @return  `true` if the quaternions are equal; `false` otherwise.
         */
        constexpr bool operator== (
            const Quaternion4<T>&   pOther
        ) const noexcept
        {
            return (
                mX == pOther.mX &&
                mY == pOther.mY &&
                mZ == pOther.mZ &&
                mW == pOther.mW
            );
        }

        /**
         * @brief   Checks to see if this quaternion's and the given quaternion's
         *          components are not all equal.
         * 
         * @param   pOther  The other quaternion.
         * 
         * @return  `true` if the quaternions are not equal; `false` otherwise.
         */
        constexpr bool operator!= (
            const Quaternion4<T>&   pOther
        ) const noexcept
        {
            return (
                mX != pOther.mX ||
                mY != pOther.mY ||
                mZ != pOther.mZ ||
                mW != pOther.mW
            );
        }

    public:

        /**
         * @brief   Converts this quaternion into a 4x4 column-major rotation
         *          matrix.
         * 
         * @return  The converted rotation matrix.
         * 
         * @note    This method assumes that this quaternion is normalized. Be
         *          sure to normalize this quaternion if not already.
         */
        constexpr Matrix4<T> ToMatrix () const noexcept
        {
            T lXX = mX * mX;
            T lYY = mY * mY;
            T lZZ = mZ * mZ;
            T lXY = mX * mY;
            T lXZ = mX * mZ;
            T lYZ = mY * mZ;
            T lWX = mW * mX;
            T lWY = mW * mY;
            T lWZ = mW * mZ;

            // Generate and return the matrix.
            return Matrix4<T> {
                ONE<T> - TWO<T> * (lYY + lZZ),
                TWO<T> * (lXY + lWZ),
                TWO<T> * (lXZ - lWY),
                ZERO<T>,

                TWO<T> * (lXY - lWZ),
                ONE<T> - TWO<T> * (lXX + lZZ),
                TWO<T> * (lYZ + lWX),
                ZERO<T>,

                TWO<T> * (lXZ + lWY),
                TWO<T> * (lYZ - lWX),
                ONE<T> - TWO<T> * (lXX + lYY),
                ZERO<T>,

                ZERO<T>, ZERO<T>, ZERO<T>, ONE<T>
            };
        }

        /**
         * @brief   Generates a 4D vector from this quaternion's components.
         * 
         * @return  The generated 4D vector.
         */
        constexpr Vector4<T> ToVector4 () const noexcept
        {
            return Vector4<T> { mX, mY, mZ, mW };
        }

        /**
         * @brief   Calculates the square length of this quaternion.
         * 
         * @return  The quaternion's length, squared.
         */
        constexpr T LengthSquared () const noexcept
        {
            return (mX * mX) + (mY * mY) + (mZ * mZ) + (mW * mW);
        }

        /**
         * @brief   Calculates the length of this quaternion.
         * 
         * @return  The quaternion's length.
         */
        constexpr T Length () const noexcept
        {
            return std::sqrt(LengthSquared());
        }

        /**
         * @brief   Calculates the dot product of this quaternion and the given
         *          quaternion, as if the quaternions were 4D vectors.
         * 
         * @param   pOther  The other quaternion.
         * 
         * @return  The dot product of the two quaternions.
         */
        constexpr T Dot (
            const Quaternion4<T>&   pOther
        ) const noexcept
        {
            return 
                (mX * pOther.mX) +
                (mY * pOther.mY) +
                (mZ * pOther.mZ) +
                (mW * pOther.mW);
        }

        /**
         * @brief   Normalizes this quaternion, so that it becomes a unit 
         *          quaternion, with a length of exactly one.
         * 
         * @return  This quaternion.
         */
        constexpr Quaternion4<T>& Normalize () noexcept
        {
            T lLength = Length();
            
            if (lLength != ZERO<T>)
            {
                mX /= lLength;
                mY /= lLength;
                mZ /= lLength;
                mW /= lLength;
            }

            return *this;
        }

        /**
         * @brief   Calculates the normalized form of this quaternion, which is 
         *          a unit quaternion with a length of exactly one.
         * 
         * @return  The normalized form of this quaternion.
         */
        constexpr Quaternion4<T> Normalized () const noexcept
        {
            Quaternion4<T> lQuaternion = *this;
            return lQuaternion.Normalize();
        }

        /**
         * @brief   Retrieves the conjugate of this quaternion, with its vector
         *          part negated.
         * 
         * @return  The quaternion's conjugate.
         */
        constexpr Quaternion4<T> Conjugate () const noexcept
        {
            return Quaternion4<T> { -mX, -mY, -mZ, mW };
        }

        /**
         * @brief   Retrieves the inverse of this quaternion, which is its
         *          conjugate divided by the quaternion's length squared.
         * 
         * @return  The quaternion's inverse.
         */
        constexpr Quaternion4<T> Inverse () const noexcept
        {
            T lLengthSquared = LengthSquared();
            if (lLengthSquared == ZERO<T>)
            {
                // Quaternions with zero length have no inverse. Return this
                // quaternion instead.
                return *this;
            }

            Quaternion4<T> lConjugate = Conjugate();
            return Quaternion4<T> {
                lConjugate.mX / lLengthSquared,
                lConjugate.mY / lLengthSquared,
                lConjugate.mZ / lLengthSquared,
                lConjugate.mW / lLengthSquared
            };
        }

        /**
         * @brief   Performs a normalized linear interpolation between this
         *          quaternion and the given quaternion at the given interval.
         * 
         * @param   pOther      The other quaternion.
         * @param   pInterval   The interval indicating how far along to
         *                      interpolate between the quaternions.
         * 
         * @return  The resultant quaternion.
         */
        constexpr Quaternion4<T> NormalizedLerp (
            const Quaternion4<T>&   pOther,
            const T                 pInterval
        ) const noexcept
        {
            // We'll need a copy of the given quaternion.
            Quaternion4<T> lResult = pOther;

            // Calculate the dot product between this quaternion and the given
            // quaternion. If it's negative, then the rotation angle is greater
            // than 90 degrees, causing the linear interpolation to be calculated
            // "the long way around".
            //
            // In this case, negate that dot product and the quaternion copy
            // we grabbed earlier to ensure that the interpolation takes the
            // shorter path.
            //
            // Note: In case `lDot` comes out to `-0.0`, check this value against
            // an epsilon value instead of exactly zero because appearantly
            // `-0.0 < 0.0`!
            T lDot = Dot(lResult);
            if (lDot < -EPSILON<T>)
            {
                lResult.mX = -lResult.mX;
                lResult.mY = -lResult.mY;
                lResult.mZ = -lResult.mZ;
                lResult.mW = -lResult.mW;
            }

            // Blend the quaternions, using the given interval.
            lResult.mX = mX * (ONE<T> - pInterval) + lResult.mX * pInterval;
            lResult.mY = mY * (ONE<T> - pInterval) + lResult.mY * pInterval;
            lResult.mZ = mZ * (ONE<T> - pInterval) + lResult.mZ * pInterval;
            lResult.mW = mW * (ONE<T> - pInterval) + lResult.mW * pInterval;

            // Return the resultant quaternion normalized.
            return lResult.Normalize();
        }

        /**
         * @brief   Performs a spherical linear interpolation between this
         *          quaternion and the given quaternion at the given interval.
         * 
         * @param   pOther      The other quaternion.
         * @param   pInterval   The interval indicating how far along to
         *                      interpolate between the quaternions.
         * 
         * @return  The resultant quaternion.
         */
        constexpr Quaternion4<T> SphericalLerp (
            const Quaternion4<T>&   pOther,
            const T                 pInterval
        ) const noexcept
        {
            // We'll need a copy of the given quaternion.
            Quaternion4<T> lResult = pOther;

            // Just as with the normalized lerp, get the dot product between the
            // quaternions and negate the righthand if necessary. For the
            // spherical lerp, also negate the dot product.
            T lDot = Dot(lResult);
            if (lDot < -EPSILON<T>)
            {
                lDot = -lDot;
                lResult.mX = -lResult.mX;
                lResult.mY = -lResult.mY;
                lResult.mZ = -lResult.mZ;
                lResult.mW = -lResult.mW;
            }

            // If the angle between the two quaternions is small enough, then
            // just fall back to a normalized lerp.
            if (lDot > T { 0.9995 })
            {
                return NormalizedLerp(lResult, pInterval);
            }

            // Compute the quaternions' spherical weights.
            T   lTheta = std::acos(lDot),
                lThetaSine = std::sin(lTheta),
                lWeight1 = std::sin((ONE<T> - pInterval) * lTheta) / lThetaSine,
                lWeight2 = std::sin(pInterval * lTheta) / lThetaSine;

            // Blend the quaternions along the great circle.
            lResult.mX = mX * lWeight1 + lResult.mX * lWeight2;
            lResult.mY = mY * lWeight1 + lResult.mY * lWeight2;
            lResult.mZ = mZ * lWeight1 + lResult.mZ * lWeight2;
            lResult.mW = mW * lWeight1 + lResult.mW * lWeight2;

            // Return the resultant quaternion.
            return lResult;
        }

        /**
         * @brief   Uses this quaternion to rotate the given 3D vector.
         * 
         * @param   pVector     The vector to be rotated.
         * 
         * @return  The rotated vector.
         * 
         * @note    This method assumes that this quaternion is normalized. Be
         *          sure to normalize this quaternion if not already.
         */
        constexpr Vector3<T> Rotate (
            const Vector3<T>&   pVector
        ) const noexcept
        {
            // Separate out the vector and scalar parts of the quaternion.
            Vector3<T>  lVector { mX, mY, mZ };
            T           lScalar = mW;

            // Get the dot and cross products of the vector portion with the
            // given vector. Also, get the vector portion's length squared.
            Vector3<T>  lCross = lVector.Cross(pVector);
            T           lDot = lVector.Dot(pVector);
            T           lLengthSquared = lVector.LengthSquared();

            // Perform the rotation and return the result.
            return
                (pVector * ((lScalar * lScalar) - lLengthSquared)) +
                (lVector * (TWO<T> * lDot)) +
                (lCross * (TWO<T> * lScalar));
        }

    };

    /**
     * @brief   Calculates the dot product of two quaternions.
     * 
     * @tparam  T       The floating-point type of the quaternions' components.
     * 
     * @param   pLeft   The lefthand vector.
     * @param   pRight  The righthand vector.
     * 
     * @return  The dot product of the two quaternions.
     */
    template <FloatingPoint T>
    inline constexpr T Dot (
        const Quaternion4<T>&   pLeft,
        const Quaternion4<T>&   pRight
    ) noexcept
    {
        return pLeft.Dot(pRight);
    }

    /**
     * @brief   Performs a normalized linear interpolation between two
     *          quaternions at the given interval.
     * 
     * @tparam  T       The floating-point type of the quaternions' components.
     * 
     * @param   pStart      The starting quaternion.
     * @param   pEnd        The ending quaternion.
     * @param   pInterval   The interval at which to interpolate.
     * 
     * @return  The resultant quaternion.
     */
    template <FloatingPoint T>
    inline constexpr Quaternion4<T> NormalizedLerp (
        const Quaternion4<T>&   pStart,
        const Quaternion4<T>&   pEnd,
        const T                 pInterval
    ) noexcept
    {
        return pStart.NormalizedLerp(pEnd, pInterval);
    }

    /**
     * @brief   Performs a spherical linear interpolation between two 
     *          quaternions at the given interval.
     * 
     * @tparam  T       The floating-point type of the quaternions' components.
     * 
     * @param   pStart      The starting quaternion.
     * @param   pEnd        The ending quaternion.
     * @param   pInterval   The interval at which to interpolate.
     * 
     * @return  The resultant quaternion.
     */
    template <FloatingPoint T>
    inline constexpr Quaternion4<T> SphericalLerp (
        const Quaternion4<T>&   pStart,
        const Quaternion4<T>&   pEnd,
        const T                 pInterval
    ) noexcept
    {
        return pStart.SphericalLerp(pEnd, pInterval);
    }

    /**
     * @brief   Uses the given quaternion to rotate the given vector.
     * 
     * @tparam  T   The floating-point type represented.
     * 
     * @param   pQuaternion The quaternion to rotate with.
     * @param   pVector     The vector to be rotated.
     * 
     * @return  The rotated vector.
     */
    template <FloatingPoint T>
    inline constexpr Vector3<T> Rotate (
        const Quaternion4<T>&   pQuaternion,
        const Vector3<T>&       pVector
    ) noexcept
    {
        return pQuaternion.Rotate(pVector);
    }

    /**
     * @brief   Checks to see if two quaternions are close enough to equal, 
     *          using an epsilon value.
     * 
     * @param   pFirst      The first quaternion.
     * @param   pSecond     The second quaternion.
     * @param   pEpsilon    The value to use as the epsilon.
     * 
     * @return  `true` if the quaternions are close enough to equal; `false` if 
     *          not.
     */
    template <typename T>
    inline constexpr bool EpsilonEqual (
        const Quaternion4<T>&   pFirst,
        const Quaternion4<T>&   pSecond,
        const T                 pEpsilon = EPSILON<T>
    ) noexcept
    {
        return
            EpsilonEqual<T>(pFirst.mX, pSecond.mX, pEpsilon) &&
            EpsilonEqual<T>(pFirst.mY, pSecond.mY, pEpsilon) &&
            EpsilonEqual<T>(pFirst.mZ, pSecond.mZ, pEpsilon) &&
            EpsilonEqual<T>(pFirst.mW, pSecond.mW, pEpsilon);
    }

    using Quaternion4f = Quaternion4<float>;
    using Quaternion4d = Quaternion4<double>;

}
