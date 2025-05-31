/**
 * @file    Ace/Maths/Vector2.hpp
 * @brief   Contains a 2D euclidian vector structure.
 */

#pragma once
#include <Ace/Maths/MathsUtils.hpp>

namespace ace
{

    /**
     * @brief   A 2D euclidian vector with X and Y components.
     * 
     * @tparam  T   The numeric types of the vector's components.
     */
    template <Numeric T>
    struct Vector2
    {
        T mX;               ///< @brief The vector's X component.
        T mY;               ///< @brief The vector's Y component.

    public:

        /**
         * @brief   The default constructor initializes the vector's components
         *          to zero.
         */
        constexpr Vector2<T> () :
            mX      { ZERO<T> },
            mY      { ZERO<T> }
        {}

        /**
         * @brief   Initializes the vector's components to the given values.
         * 
         * @param   pX  The vector's X component.
         * @param   pY  The vector's Y component.
         */
        constexpr Vector2<T> (
            const T pX,
            const T pY
        ) :
            mX  { pX },
            mY  { pY }
        {}

    public:
        
        /**
         * @brief   Constructs a vector with all components set to zero.
         * 
         * @return  The constructed vector.
         */
        static constexpr Vector2<T> Zero () noexcept
        {
            return Vector2<T> { ZERO<T>, ZERO<T> };
        }

        /**
         * @brief   Constructs a vector with all components set to one.
         * 
         * @return  The constructed vector.
         */
        static constexpr Vector2<T> One () noexcept
        {
            return Vector2<T> { ONE<T>, ONE<T> };
        }

        /**
         * @brief   Constructs an upwards-facing vector, in which the Y
         *          component is set to one.
         * 
         * @return  The constructed vector.
         */
        static constexpr Vector2<T> Up () noexcept
        {
            return Vector2<T> { ZERO<T>, ONE<T> };
        }

        /**
         * @brief   Constructs a right-facing vector, in which the X
         *          component is set to one.
         * 
         * @return  The constructed vector.
         */
        static constexpr Vector2<T> Right () noexcept
        {
            return Vector2<T> { ONE<T>, ZERO<T> };
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
            const Vector2<T>&   pOther
        ) const noexcept
        {
            return (
                mX == pOther.mX &&
                mY == pOther.mY
            );
        }

        /**
         * @brief   Negates the components of this vector.
         * 
         * @return  A vector with the negated components.
         */
        constexpr Vector2<T> operator- () const noexcept
        {
            return Vector2<T> { -mX, -mY };
        }

        /**
         * @brief   Adds the given scalar value to the components of 
         *          this vector.
         * 
         * @param   pOther  The scalar value.
         * 
         * @return  This vector.
         */
        constexpr Vector2<T>& operator+= (
            const T pScalar
        ) noexcept
        {
            mX += pScalar;
            mY += pScalar;
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
        friend constexpr Vector2<T> operator+ (
            Vector2<T>      pLeft,
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
        friend constexpr Vector2<T> operator+ (
            const T         pLeft,
            Vector2<T>      pRight
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
        constexpr Vector2<T>& operator-= (
            const T pScalar
        ) noexcept
        {
            mX -= pScalar;
            mY -= pScalar;
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
        friend constexpr Vector2<T> operator- (
            Vector2<T>      pLeft,
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
        constexpr Vector2<T>& operator*= (
            const T pScalar
        ) noexcept
        {
            mX *= pScalar;
            mY *= pScalar;
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
        friend constexpr Vector2<T> operator* (
            Vector2<T>      pLeft,
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
        friend constexpr Vector2<T> operator* (
            const T         pLeft,
            Vector2<T>      pRight
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
        constexpr Vector2<T>& operator/= (
            const T pScalar
        ) noexcept
        {
            assert(pScalar != ZERO<T>);

            mX /= pScalar;
            mY /= pScalar;
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
        friend constexpr Vector2<T> operator/ (
            Vector2<T>      pLeft,
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
        constexpr Vector2<T>& operator+= (
            const Vector2<T>&   pOther
        ) noexcept
        {
            mX += pOther.mX;
            mY += pOther.mY;
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
        friend constexpr Vector2<T> operator+ (
            Vector2<T>          pLeft,
            const Vector2<T>&   pRight
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
        constexpr Vector2<T>& operator-= (
            const Vector2<T>&   pOther
        ) noexcept
        {
            mX -= pOther.mX;
            mY -= pOther.mY;
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
        friend constexpr Vector2<T> operator- (
            Vector2<T>          pLeft,
            const Vector2<T>&   pRight
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
        constexpr Vector2<T>& operator*= (
            const Vector2<T>&   pOther
        ) noexcept
        {
            mX *= pOther.mX;
            mY *= pOther.mY;
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
        friend constexpr Vector2<T> operator* (
            Vector2<T>          pLeft,
            const Vector2<T>&   pRight
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
            return (mX * mX) + (mY * mY);
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
            const Vector2<T>&   pOther
        ) const noexcept
        {
            return 
                (mX * pOther.mX) +
                (mY * pOther.mY);
        }

        /**
         * @brief   Normalizes this vector, so that it becomes a unit vector,
         *          with a length of exactly one.
         * 
         * @return  This vector.
         */
        constexpr Vector2<T>& Normalize () noexcept
        {
            T lLength = Length();
            
            if (lLength != ZERO<T>)
            {
                mX /= lLength;
                mY /= lLength;
            }

            return *this;
        }

        /**
         * @brief   Calculates the normalized form of this vector, which is a
         *          unit vector with a length of exactly one.
         * 
         * @return  The normalized form of this vector.
         */
        constexpr Vector2<T> Normalized () const noexcept
        {
            Vector2<T> lVector = *this;
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
            const Vector2<T>&   pOther
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
        constexpr Vector2<T> Project (
            const Vector2<T>&   pOther
        ) const noexcept
        {
            const T lLengthSquared = pOther.LengthSquared();
            return (lLengthSquared > ZERO<T>) ?
                pOther * (Dot(pOther) / lLengthSquared) :
                Vector2<T>::Zero();
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
        constexpr Vector2<T> Reflect (
            const Vector2<T>&   pNormal
        ) const noexcept
        {
            return *this - (TWO<T> * Dot(pNormal)) * pNormal;
        }

        /**
         * @brief   Calculates the refraction (bend) of a ray represented by
         *          this vector as it passes through a line represented by the
         *          given vector.
         * 
         * @param   pNormal     The vector representing the line through which
         *                      this vector would be bent. This should be a 
         *                      normalized unit vector.
         * @param   pRatio      The ratio of the refractive indices of the
         *                      mediums that the ray is coming from and that the
         *                      ray is passing through.
         * 
         * @return  The calculated refraction vector (or reflection vector in
         *          case of a total internal reflection).
         */
        constexpr Vector2<T> Refract (
            const Vector2<T>&   pNormal,
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
        const Vector2<T>&   pLeft,
        const Vector2<T>&   pRight
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
    inline constexpr Vector2<T> Cross (
        const Vector2<T>&   pLeft,
        const Vector2<T>&   pRight
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
        const Vector2<T>&   pStart,
        const Vector2<T>&   pEnd
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
    inline constexpr Vector2<T> Project (
        const Vector2<T>&   pProjector,
        const Vector2<T>&   pProjectee
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
    inline constexpr Vector2<T> Reflect (
        const Vector2<T>&   pStart,
        const Vector2<T>&   pNormal
    ) noexcept
    {
        return pStart.Reflect(pNormal);
    }

    /**
     * @brief   Calculates the refraction (bend) of a given ray vector as
     *          it passes through a line represented by a given normal
     *          vector.
     * 
     * @tparam  T           The numeric type of the vectors' components and the
     *                      refraction ratio.
     * 
     * @param   pRay        The vector representing the ray passing through
     *                      the line. This should be a normalized unit
     *                      vector.
     * @param   pNormal     The vector representing the line through which
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
    inline constexpr Vector2<T> Refract (
        const Vector2<T>&   pRay,
        const Vector2<T>&   pNormal,
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
        const Vector2<T>&   pFirst,
        const Vector2<T>&   pSecond
    ) noexcept
    {
        return
            EpsilonEqual<T>(pFirst.mX, pSecond.mX) &&
            EpsilonEqual<T>(pFirst.mY, pSecond.mY);
    }

    using Vector2f  = Vector2<float>;
    using Vector2d  = Vector2<double>;
    using Vector2i  = Vector2<std::int32_t>;
    using Vector2u  = Vector2<std::uint32_t>;
    using Vector2b  = Vector2<bool>;

}
