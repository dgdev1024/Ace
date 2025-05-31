/**
 * @file    Ace/Maths/Vector4.hpp
 * @brief   Contains a 4D euclidian vector structure.
 */

#pragma once
#include <Ace/Maths/MathsUtils.hpp>

namespace ace
{

    /**
     * @brief   A 4D euclidian vector with X, Y, Z and W components.
     * 
     * @tparam  T   The numeric types of the vector's components.
     */
    template <Numeric T>
    struct Vector4
    {
        T mX;               ///< @brief The vector's X component.
        T mY;               ///< @brief The vector's Y component.
        T mZ;               ///< @brief The vector's Z component.
        T mW;               ///< @brief The vector's W component.

    public:

        /**
         * @brief   The default constructor initializes the vector's components
         *          to zero.
         */
        constexpr Vector4<T> () :
            mX      { ZERO<T> },
            mY      { ZERO<T> },
            mZ      { ZERO<T> },
            mW      { ZERO<T> }
        {}

        /**
         * @brief   Initializes the vector's components to the given values.
         * 
         * @param   pX  The vector's X component.
         * @param   pY  The vector's Y component.
         * @param   pZ  The vector's Z component.
         * @param   pW  The vector's W component.
         */
        constexpr Vector4<T> (
            const T pX,
            const T pY,
            const T pZ,
            const T pW
        ) :
            mX  { pX },
            mY  { pY },
            mZ  { pZ },
            mW  { pW }
        {
            
        }

    public:
        
        /**
         * @brief   Constructs a vector with all components set to zero.
         * 
         * @return  The constructed vector.
         */
        static constexpr Vector4<T> Zero () noexcept
        {
            return Vector4<T> { ZERO<T>, ZERO<T>, ZERO<T>, ZERO<T> };
        }

        /**
         * @brief   Constructs a vector with all components set to one.
         * 
         * @return  The constructed vector.
         */
        static constexpr Vector4<T> One () noexcept
        {
            return Vector4<T> { ONE<T>, ONE<T>, ONE<T>, ONE<T> };
        }

        /**
         * @brief   Constructs an upwards-facing vector, in which the Y
         *          component is set to one.
         * 
         * @return  The constructed vector.
         */
        static constexpr Vector4<T> Up () noexcept
        {
            return Vector4<T> { ZERO<T>, ONE<T>, ZERO<T>, ZERO<T> };
        }

        /**
         * @brief   Constructs a right-facing vector, in which the X
         *          component is set to one.
         * 
         * @return  The constructed vector.
         */
        static constexpr Vector4<T> Right () noexcept
        {
            return Vector4<T> { ONE<T>, ZERO<T>, ZERO<T>, ZERO<T> };
        }

        /**
         * @brief   Constructs a front-facing vector, in which the Z
         *          component is set to one.
         * 
         * @return  The constructed vector.
         */
        static constexpr Vector4<T> Front () noexcept
        {
            return Vector4<T> { ZERO<T>, ZERO<T>, ONE<T>, ZERO<T> };
        }

    public:

        /**
         * @brief   Checks to see if this vector's and the given vector's
         *          components are equal.
         * 
         * @param   pOther  The other vector.
         * 
         * @return  `true` if the vectors are equal; `false` otherwise.
         */
        constexpr bool operator== (
            const Vector4<T>&   pOther
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
         * @brief   Negates the components of this vector.
         * 
         * @return  A vector with the negated components.
         */
        constexpr Vector4<T> operator- () const noexcept
        {
            return Vector4<T> { -mX, -mY, -mZ, -mW };
        }

        /**
         * @brief   Adds the given scalar value to the components of 
         *          this vector.
         * 
         * @param   pOther  The scalar value.
         * 
         * @return  This vector.
         */
        constexpr Vector4<T>& operator+= (
            const T pScalar
        ) noexcept
        {
            mX += pScalar;
            mY += pScalar;
            mZ += pScalar;
            mW += pScalar;
            return *this;
        }

        /**
         * @brief   Adds the given scalar value to the components of
         *          the given vector. Lefthand version.
         * 
         * @param   pLeft   The lefthand vector.
         * @param   pRight  The righthand scalar.
         * 
         * @return  A vector with the component sums.
         */
        friend constexpr Vector4<T> operator+ (
            Vector4<T>      pLeft,
            const T         pRight
        ) noexcept
        {
            return pLeft += pRight;
        }

        /**
         * @brief   Adds the given scalar value to the components of
         *          the given vector. Righthand version.
         * 
         * @param   pLeft   The lefthand scalar.
         * @param   pRight  The righthand vector.
         * 
         * @return  A vector with the component sums.
         */
        friend constexpr Vector4<T> operator+ (
            const T         pLeft,
            Vector4<T>      pRight
        ) noexcept
        {
            return pRight += pLeft;
        }

        /**
         * @brief   Subtracts the given scalar value from the components of 
         *          this vector.
         * 
         * @param   pOther  The scalar value.
         * 
         * @return  This vector.
         */
        constexpr Vector4<T>& operator-= (
            const T pScalar
        ) noexcept
        {
            mX -= pScalar;
            mY -= pScalar;
            mZ -= pScalar;
            mW -= pScalar;
            return *this;
        }

        /**
         * @brief   Subtracts the given scalar value from the components of
         *          the given vector. Lefthand version.
         * 
         * @param   pLeft   The lefthand vector.
         * @param   pRight  The righthand scalar.
         * 
         * @return  A vector with the component differences.
         */
        friend constexpr Vector4<T> operator- (
            Vector4<T>      pLeft,
            const T         pRight
        ) noexcept
        {
            return pLeft -= pRight;
        }

        /**
         * @brief   Multiplies the given scalar value with the components of 
         *          this vector.
         * 
         * @param   pOther  The scalar value.
         * 
         * @return  This vector.
         */
        constexpr Vector4<T>& operator*= (
            const T pScalar
        ) noexcept
        {
            mX *= pScalar;
            mY *= pScalar;
            mZ *= pScalar;
            mW *= pScalar;
            return *this;
        }

        /**
         * @brief   Multiplies the given scalar value with the components of
         *          the given vector. Lefthand version.
         * 
         * @param   pLeft   The lefthand vector.
         * @param   pRight  The righthand scalar.
         * 
         * @return  A vector with the component products.
         */
        friend constexpr Vector4<T> operator* (
            Vector4<T>      pLeft,
            const T         pRight
        ) noexcept
        {
            return pLeft *= pRight;
        }

        /**
         * @brief   Multiplies the given scalar value with the components of
         *          the given vector. Righthand version.
         * 
         * @param   pLeft   The lefthand scalar.
         * @param   pRight  The righthand vector.
         * 
         * @return  A vector with the component products.
         */
        friend constexpr Vector4<T> operator* (
            const T         pLeft,
            Vector4<T>      pRight
        ) noexcept
        {
            return pRight *= pLeft;
        }

        /**
         * @brief   Divides the given scalar value with the components of 
         *          this vector.
         * 
         * @param   pOther  The scalar value.
         * 
         * @return  This vector.
         */
        constexpr Vector4<T>& operator/= (
            const T pScalar
        ) noexcept
        {
            assert(pScalar != ZERO<T>);

            mX /= pScalar;
            mY /= pScalar;
            mZ /= pScalar;
            mW /= pScalar;
            return *this;
        }

        /**
         * @brief   Divides the given scalar value with the components of
         *          the given vector. Lefthand version.
         * 
         * @param   pLeft   The lefthand vector.
         * @param   pRight  The righthand scalar.
         * 
         * @return  A vector with the component quotients.
         */
        friend constexpr Vector4<T> operator/ (
            Vector4<T>      pLeft,
            const T         pRight
        ) noexcept
        {
            return pLeft /= pRight;
        }

        /**
         * @brief   Adds the components of the given vector to those of this
         *          vector.
         * 
         * @param   pOther  The other vector.
         * 
         * @return  This vector.
         */
        constexpr Vector4<T>& operator+= (
            const Vector4<T>&   pOther
        ) noexcept
        {
            mX += pOther.mX;
            mY += pOther.mY;
            mZ += pOther.mZ;
            mW += pOther.mW;
            return *this;
        }

        /**
         * @brief   Adds the components of two vectors.
         * 
         * @param   pLeft   The lefthand vector.
         * @param   pRight  The righthand vector.
         * 
         * @return  A vector with the component sums.
         */
        friend constexpr Vector4<T> operator+ (
            Vector4<T>          pLeft,
            const Vector4<T>&   pRight
        ) noexcept
        {
            return pLeft += pRight;
        }

        /**
         * @brief   Subtracts the components of the given vector from those of 
         *          this vector.
         * 
         * @param   pOther  The other vector.
         * 
         * @return  This vector.
         */
        constexpr Vector4<T>& operator-= (
            const Vector4<T>&   pOther
        ) noexcept
        {
            mX -= pOther.mX;
            mY -= pOther.mY;
            mZ -= pOther.mZ;
            mW -= pOther.mW;
            return *this;
        }

        /**
         * @brief   Subtracts the components of two vectors.
         * 
         * @param   pLeft   The lefthand vector.
         * @param   pRight  The righthand vector.
         * 
         * @return  A vector with the component differences.
         */
        friend constexpr Vector4<T> operator- (
            Vector4<T>          pLeft,
            const Vector4<T>&   pRight
        ) noexcept
        {
            return pLeft -= pRight;
        }

        /**
         * @brief   Multiplies the components of the given vector with those of 
         *          this vector.
         * 
         * @param   pOther  The other vector.
         * 
         * @return  This vector.
         */
        constexpr Vector4<T>& operator*= (
            const Vector4<T>&   pOther
        ) noexcept
        {
            mX *= pOther.mX;
            mY *= pOther.mY;
            mZ *= pOther.mZ;
            mW *= pOther.mW;
            return *this;
        }

        /**
         * @brief   Multiplies the components of two vectors.
         * 
         * @param   pLeft   The lefthand vector.
         * @param   pRight  The righthand vector.
         * 
         * @return  A vector with the component products.
         */
        friend constexpr Vector4<T> operator* (
            Vector4<T>          pLeft,
            const Vector4<T>&   pRight
        ) noexcept
        {
            return pLeft *= pRight;
        }

    public:

        /**
         * @brief   Calculates the square length of this vector.
         * 
         * @return  The vector's length, squared.
         */
        constexpr T LengthSquared () const noexcept
        {
            return (mX * mX) + (mY * mY) + (mZ * mZ) + (mW * mW);
        }

        /**
         * @brief   Calculates the length of this vector.
         * 
         * @return  The vector's length.
         */
        constexpr T Length () const noexcept
        {
            return std::sqrt(LengthSquared());
        }

        /**
         * @brief   Calculates the dot product of this vector and the given
         *          vector.
         * 
         * @param   pOther  The other vector.
         * 
         * @return  The dot product of the two vectors.
         */
        constexpr T Dot (
            const Vector4<T>&   pOther
        ) const noexcept
        {
            return 
                (mX * pOther.mX) +
                (mY * pOther.mY) +
                (mZ * pOther.mZ) +
                (mW * pOther.mW);
        }

        /**
         * @brief   Normalizes this vector, so that it becomes a unit vector,
         *          with a length of exactly one.
         * 
         * @return  This vector.
         */
        constexpr Vector4<T>& Normalize () noexcept
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
         * @brief   Calculates the normalized form of this vector, which is a
         *          unit vector with a length of exactly one.
         * 
         * @return  The normalized form of this vector.
         */
        constexpr Vector4<T> Normalized () const noexcept
        {
            Vector4<T> lVector = *this;
            return lVector.Normalize();
        }

        /**
         * @brief   Calculates the distance from this vector to the given
         *          vector.
         * 
         * @param   pOther  The other vector.
         * 
         * @return  The distance between the two vectors.
         */
        constexpr T Distance (
            const Vector4<T>&   pOther
        ) const noexcept
        {
            return ((*this) - pOther).Length();
        }

        /**
         * @brief   Calculates the projection of this vector onto the given
         *          vector.
         * 
         * The formula for projecting vector `X` onto vector `Y` is as follows:
         *  `P = Y * ((X dot Y) / (len(Y) pow 2))`
         * 
         * @param   pOther  The vector to be projected onto.
         * 
         * @return  The calculated projection of this vector.
         */
        constexpr Vector4<T> Project (
            const Vector4<T>&   pOther
        ) const noexcept
        {
            const T lLengthSquared = pOther.LengthSquared();
            return (lLengthSquared > ZERO<T>) ?
                pOther * (Dot(pOther) / lLengthSquared) :
                Vector4<T>::Zero();
        }

        /**
         * @brief   Calculates the reflection of this vector around a normalized
         *          unit vector.
         * 
         * @param   pNormal     The vector to reflect around. This should be a
         *                      normalized unit vector.
         * 
         * @return  The calculated reflection vector.
         */
        constexpr Vector4<T> Reflect (
            const Vector4<T>&   pNormal
        ) const noexcept
        {
            return *this - (TWO<T> * Dot(pNormal)) * pNormal;
        }

        /**
         * @brief   Calculates the refraction (bend) of a ray represented by
         *          this vector as it passes through a plane represented by the
         *          given vector.
         * 
         * @param   pNormal     The vector representing the plane through which
         *                      this vector would be bent. This should be a 
         *                      normalized unit vector.
         * @param   pRatio      The ratio of the refractive indices of the
         *                      mediums that the ray is coming from and that the
         *                      ray is passing through.
         * 
         * @return  The calculated refraction vector (or reflection vector in
         *          case of a total internal reflection).
         */
        constexpr Vector4<T> Refract (
            const Vector4<T>&   pNormal,
            const T             pRatio
        ) const noexcept
        {
            const T lInverseCosine = -Dot(pNormal);
            const T lSineSquared = ONE<T> - pRatio * pRatio *
                (ONE<T> - lInverseCosine * lInverseCosine);

            if (lSineSquared < ZERO<T>)
            {
                // Total internal reflection; reflect instead.
                return Reflect(pNormal);
            }

            // Refract.
            return
                (*this * pRatio) +
                (pNormal * (pRatio * lInverseCosine - std::sqrt(lSineSquared)));
        }

    };

    /**
     * @brief   Calculates the dot product of two vectors.
     * 
     * @tparam  T       The numeric type of the vectors' components.
     * 
     * @param   pLeft   The lefthand vector.
     * @param   pRight  The righthand vector.
     * 
     * @return  The dot product of the two vectors.
     */
    template <Numeric T>
    inline constexpr T Dot (
        const Vector4<T>&   pLeft,
        const Vector4<T>&   pRight
    ) noexcept
    {
        return pLeft.Dot(pRight);
    }

    /**
     * @brief   Calculates the cross product of two vectors.
     * 
     * @tparam  T       The numeric type of the vectors' components.
     * 
     * @param   pLeft   The lefthand vector.
     * @param   pRight  The righthand vector.
     * 
     * @return  The cross product of the two vectors.
     */
    template <Numeric T>
    inline constexpr Vector4<T> Cross (
        const Vector4<T>&   pLeft,
        const Vector4<T>&   pRight
    ) noexcept
    {
        return pLeft.Cross(pRight);
    }

    /**
     * @brief   Calculates the distance between two vectors.
     * 
     * @tparam  T       The numeric type of the vectors' components.
     * 
     * @param   pStart  The starting point vector.
     * @param   pEnd    The ending point vector.
     * 
     * @return  The distance between the two vectors.
     */
    template <Numeric T>
    inline constexpr T Distance (
        const Vector4<T>&   pStart,
        const Vector4<T>&   pEnd
    ) noexcept
    {
        return pStart.Distance(pEnd);
    }

    /**
     * @brief   Calculates the projection of one vector onto another.
     * 
     * @tparam  T           The numeric type of the vectors' components.
     * 
     * @param   pProjector  The vector to project.
     * @param   pProjectee  The vector to be projected onto.
     * 
     * @return  The calculated projection of projector onto projectee.
     */
    template <Numeric T>
    inline constexpr Vector4<T> Project (
        const Vector4<T>&   pProjector,
        const Vector4<T>&   pProjectee
    ) noexcept
    {
        return pProjector.Project(pProjectee);
    }

    /**
     * @brief   Calculates the reflection of a vector around a normalized
     *          unit vector.
     * 
     * @tparam  T           The numeric type of the vectors' components.
     * 
     * @param   pStart      The starting vector.
     * @param   pNormal     The vector to reflect around. This should be a
     *                      normalized unit vector.
     * 
     * @return  The calculated reflection vector.
     */
    template <Numeric T>
    inline constexpr Vector4<T> Reflect (
        const Vector4<T>&   pStart,
        const Vector4<T>&   pNormal
    ) noexcept
    {
        return pStart.Reflect(pNormal);
    }

    /**
     * @brief   Calculates the refraction (bend) of a given ray vector as
     *          it passes through a plane represented by a given normal
     *          vector.
     * 
     * @tparam  T           The numeric type of the vectors' components and the
     *                      refraction ratio.
     * 
     * @param   pRay        The vector representing the ray passing through
     *                      the plane. This should be a normalized unit
     *                      vector.
     * @param   pNormal     The vector representing the plane through which
     *                      the ray vector would be bent. This should be a 
     *                      normalized unit vector.
     * @param   pRatio      The ratio of the refractive indices of the
     *                      mediums that the ray is coming from and that the
     *                      ray is passing through.
     * 
     * @return  The calculated refraction vector (or reflection vector in
     *          case of a total internal reflection).
     */
    template <Numeric T>
    inline constexpr Vector4<T> Refract (
        const Vector4<T>&   pRay,
        const Vector4<T>&   pNormal,
        const T             pRatio
    ) noexcept
    {
        return pRay.Refract(pNormal, pRatio);
    }

    /**
     * @brief   Checks to see if two vectors are close enough to equal, using
     *          an epsilon value.
     * 
     * @param   pFirst      The first vector.
     * @param   pSecond     The second vector.
     * 
     * @return  `true` if the vectors are close enough to equal; `false` if not.
     */
    template <typename T>
    inline constexpr bool EpsilonEqual (
        const Vector4<T>&   pFirst,
        const Vector4<T>&   pSecond
    ) noexcept
    {
        return
            EpsilonEqual<T>(pFirst.mX, pSecond.mX) &&
            EpsilonEqual<T>(pFirst.mY, pSecond.mY) &&
            EpsilonEqual<T>(pFirst.mZ, pSecond.mZ) &&
            EpsilonEqual<T>(pFirst.mW, pSecond.mW);
    }

    using Vector4f  = Vector4<float>;
    using Vector4d  = Vector4<double>;
    using Vector4i  = Vector4<std::int32_t>;
    using Vector4u  = Vector4<std::uint32_t>;
    using Vector4b  = Vector4<bool>;

}
