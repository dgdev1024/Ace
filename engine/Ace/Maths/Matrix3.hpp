/**
 * @file    Ace/Maths/Matrix3.hpp
 */

#pragma once
#include <Ace/Maths/Vector3.hpp>

namespace ace
{

    /**
     * @brief   A 3x3 matrix with elements laid out in column-major order.
     * 
     * @tparam  T   The numeric types of the matrix's elements.
     */
    template <Numeric T>
    struct Matrix3
    {
    public:
        static constexpr std::size_t SIZE = 3;              ///< @brief The size of the matrix, going across and down, in elements.
        static constexpr std::size_t COUNT = SIZE * SIZE;   ///< @brief The number of elements in the matrix.
    
    public:
        std::array<T, COUNT> mI; ///< @brief The matrix's elements, laid out in column-major order.

    public: /* Constructors and Destructor ************************************/
        
        /**
         * @brief   The default constructor creates an identity matrix.
         */
        constexpr Matrix3<T> () noexcept :
            mI {
                ONE<T>,  ZERO<T>, ZERO<T>,
                ZERO<T>, ONE<T>,  ZERO<T>,
                ZERO<T>, ZERO<T>, ONE<T>
            }
        {}

        /**
         * @brief   Constructs a matrix from the given array of elements.
         * 
         * @param   pI  The array of the matrix's elements, laid out in
         *              column-major order.
         */
        constexpr Matrix3<T> (
            const std::array<T, COUNT>& pI
        ) noexcept :
            mI  { pI }
        {}

        /**
         * @brief   Constructs a matrix from the given initializer list.
         * 
         * @param   pI  The initializer list containing some or all of the
         *              matrix's elements, laid out in column-major order.
         */
        constexpr Matrix3<T> (
            const std::initializer_list<T>& pI
        ) noexcept
        {
            std::copy_n(pI.begin(), std::min(pI.size(), COUNT), mI.begin());
        }

    public: /* Named Constructors *********************************************/

        /**
         * @brief   Creates an identity matrix.
         * 
         * @return  The identity matrix.
         */
        static constexpr Matrix3<T> Identity () noexcept
        {
            return Matrix3<T> {};
        }

        /**
         * @brief   Creates a zero matrix.
         * 
         * @return  The zero matrix.
         */
        static constexpr Matrix3<T> Zero () noexcept
        {
            return Matrix3<T> {
                ZERO<T>, ZERO<T>, ZERO<T>,
                ZERO<T>, ZERO<T>, ZERO<T>,
                ZERO<T>, ZERO<T>, ZERO<T>           
            };
        }

    public: /* Equality Operations ********************************************/

        /**
         * @brief   Checks to see if the elements of this matrix and those of
         *          the given matrix are all equal.
         * 
         * @param   pOther  The other matrix.
         * 
         * @return  `true` if the matrices' elements are equal; `false` if not.
         */
        constexpr bool operator== (
            const Matrix3<T>&   pOther
        ) const noexcept
        {
            for (std::size_t i = 0; i < COUNT; ++i)
            {
                if (mI[i] != pOther.mI[i])
                {
                    return false;
                }
            }

            return true;
        }

        /**
         * @brief   Checks to see if not all of the elements of this matrix and
         *          those of the given matrix are equal.
         * 
         * @param   pOther  The other matrix.
         * 
         * @return  `true` if not all of the matrices' elements are equal;
         *          `false` otherwise.
         */
        constexpr bool operator!= (
            const Matrix3<T>&   pOther
        ) const noexcept
        {
            for (std::size_t i = 0; i < COUNT; ++i)
            {
                if (mI[i] != pOther.mI[i])
                {
                    return true;
                }
            }

            return false;
        }

    public: /* Element Accessors **********************************************/

        /**
         * @brief   Accesses the matrix element at the given position.
         * 
         * @param   pRow    The row of the element to access.
         * @param   pColumn The column of the element to access.
         * 
         * @return  The accessed element.
         */
        constexpr T& operator() (
            const std::size_t   pRow,
            const std::size_t   pColumn
        ) noexcept
        {
            return mI[pColumn * SIZE + pRow];
        }

        /**
         * @brief   Accesses the matrix element at the given position. `const`
         *          version.
         * 
         * @param   pRow    The row of the element to access.
         * @param   pColumn The column of the element to access.
         * 
         * @return  The accessed element.
         */
        constexpr const T& operator() (
            const std::size_t   pRow,
            const std::size_t   pColumn
        ) const noexcept
        {
            return mI[pColumn * SIZE + pRow];
        }

    public: /* Matrix vs. Matrix Operations ***********************************/

        /**
         * @brief   Adds the elements of the given matrix to those of 
         *          this matrix.
         * 
         * @param   pOther  The other matrix.
         * 
         * @return  This matrix.
         */
        constexpr Matrix3<T>& operator+= (
            const Matrix3<T>&   pOther
        ) noexcept
        {
            for (std::size_t i = 0; i < COUNT; ++i)
            {
                mI[i] += pOther.mI[i];
            }

            return *this;
        }

        /**
         * @brief   Adds together the elements of two matrices.
         * 
         * @param   pLeft   The left matrix.
         * @param   pRight  The right matrix.
         * 
         * @return  A matrix containing the element sums.
         */
        friend constexpr Matrix3<T> operator+ (
            Matrix3<T>          pLeft,
            const Matrix3<T>&   pRight
        ) noexcept
        {
            return pLeft += pRight;
        }

        /**
         * @brief   Subtracts the elements of the given matrix from those of 
         *          this matrix.
         * 
         * @param   pOther  The other matrix.
         * 
         * @return  This matrix.
         */
        constexpr Matrix3<T>& operator-= (
            const Matrix3<T>&   pOther
        ) noexcept
        {
            for (std::size_t i = 0; i < COUNT; ++i)
            {
                mI[i] -= pOther.mI[i];
            }

            return *this;
        }

        /**
         * @brief   Subtracts together the elements of two matrices.
         * 
         * @param   pLeft   The left matrix.
         * @param   pRight  The right matrix.
         * 
         * @return  A matrix containing the element differences.
         */
        friend constexpr Matrix3<T> operator- (
            Matrix3<T>          pLeft,
            const Matrix3<T>&   pRight
        ) noexcept
        {
            return pLeft -= pRight;
        }

        /**
         * @brief   Performs a matrix multiplication between two matrices.
         * 
         * @param   pLeft   The left matrix.
         * @param   pRight  The right matrix.
         * 
         * @return  A matrix containing the product of the two matrices.
         */
        friend constexpr Matrix3<T> operator* (
            const Matrix3<T>&   pLeft,
            const Matrix3<T>&   pRight
        ) noexcept
        {
            Matrix3<T> lResult;

            for (std::size_t lColumn = 0; lColumn < Matrix3<T>::SIZE; ++lColumn)
            {
                for (std::size_t lRow = 0; lRow < Matrix3<T>::SIZE; ++lRow)
                {
                    T lSum = ZERO<T>;
                    for (std::size_t i = 0; i < Matrix3<T>::SIZE; ++i)
                    {
                        lSum += pLeft(lRow, i) * pRight(i, lColumn);
                    }

                    lResult(lRow, lColumn) = lSum;
                }
            }

            return lResult;
        }

    public: /* Matrix vs. Vector Operations ***********************************/

        /**
         * @brief   Performs a multiplication between this matrix and the given
         *          column-vector.
         * 
         * @param   pLeft   The left matrix.
         * @param   pRight  The right column-vector.
         *
         * @return  A column-vector containing the product of the two.
         */
        friend constexpr Vector3<T> operator* (
            const Matrix3<T>&   pLeft,
            const Vector3<T>&   pRight
        ) noexcept
        {
            return Vector3<T> {
                pLeft(0, 0) * pRight.mX + pLeft(0, 1) * pRight.mY + pLeft(0, 2) * pRight.mZ,
                pLeft(1, 0) * pRight.mX + pLeft(1, 1) * pRight.mY + pLeft(1, 2) * pRight.mZ,
                pLeft(2, 0) * pRight.mX + pLeft(2, 1) * pRight.mY + pLeft(2, 2) * pRight.mZ
            };
        }

        /**
         * @brief   Performs a multiplication between the given row-vector and
         *          this matrix.
         * 
         * @param   pLeft   The left row-vector.
         * @param   pRight  The right matrix.
         * 
         * @return  A row-vector containing the product of the two.
         */
        friend constexpr Vector3<T> operator* (
            const Vector3<T>&   pLeft,
            const Matrix3<T>&   pRight
        ) noexcept
        {
            return Vector3<T> {
                pLeft.mX * pRight(0, 0) + pLeft.mY * pRight(1, 0) +
                pLeft.mZ * pRight(2, 0),
                
                pLeft.mX * pRight(0, 1) + pLeft.mY * pRight(1, 1) +
                pLeft.mZ * pRight(2, 1),
                
                pLeft.mX * pRight(0, 2) + pLeft.mY * pRight(1, 2) +
                pLeft.mZ * pRight(2, 2),
            };
        }

    public: /* Matrix vs. Scalar Operations ***********************************/

        /**
         * @brief   Multiplies the elements of this matrix with the given scalar
         *          value.
         * 
         * @param   pScalar The scalar value.
         * 
         * @return  This matrix.
         */
        constexpr Matrix3<T>& operator*= (
            const T pScalar
        ) noexcept
        {
            for (std::size_t i = 0; i < COUNT; ++i)
            {
                mI[i] *= pScalar;
            }

            return *this;
        }

        /**
         * @brief   Multiplies the elements of the given matrix with the given
         *          scalar value. Lefthand version.
         * 
         * @param   pLeft   The lefthand matrix.
         * @param   pRight  The righthand scalar value.
         * 
         * @return  A matrix containing the element products.
         */
        friend constexpr Matrix3<T> operator* (
            Matrix3<T>      pLeft,
            const T         pRight
        ) noexcept
        {
            return pLeft *= pRight;
        }

        /**
         * @brief   Multiplies the elements of the given scalar value with the
         *          elements of the given matrix. Righthand version.
         * 
         * @param   pLeft   The lefthand scalar value.
         * @param   pRight  The righthand matrix.
         * 
         * @return  A matrix containing the element products.
         */
        friend constexpr Matrix3<T> operator* (
            const T         pLeft,
            Matrix3<T>      pRight
        ) noexcept
        {
            return pRight *= pLeft;
        }

        /**
         * @brief   Divides the elements of this matrix by the given scalar
         *          value.
         * 
         * @param   pScalar The scalar value.
         * 
         * @return  This matrix.
         */
        constexpr Matrix3<T>& operator/= (
            const T pScalar
        ) noexcept
        {
            assert(pScalar != ZERO<T>);

            for (std::size_t i = 0; i < COUNT; ++i)
            {
                mI[i] /= pScalar;
            }

            return *this;
        }

        /**
         * @brief   Divides the elements of the given matrix by the given scalar
         *          value. Lefthand version.
         * 
         * @param   pLeft   The lefthand matrix.
         * @param   pRight  The righthand scalar value.
         * 
         * @return  A matrix containing the element quotients.
         */
        friend constexpr Matrix3<T> operator/ (
            Matrix3<T>      pLeft,
            const T         pRight
        ) noexcept
        {
            return pLeft /= pRight;
        }

    public: /* Public Methods *************************************************/

        /**
         * @brief   Calculates the transpose of this matrix, which is a matrix
         *          with the rows and columns swapped.
         * 
         * @return  The transposed matrix.
         */
        constexpr Matrix3<T> Transpose () const noexcept
        {
            Matrix3<T> lResult;

            for (std::size_t lRow = 0; lRow < SIZE; ++lRow)
            {
                for (std::size_t lColumn = 0; lColumn < SIZE; ++lColumn)
                {
                    lResult(lRow, lColumn) = (*this)(lColumn, lRow);
                }
            }

            return lResult;
        }

        /**
         * @brief   Calculates the determinant of this matrix.
         * 
         * @return  The determinant of the matrix.
         */
        constexpr T Determinant () const noexcept
        {
            #define M(r, c) (*this)(r, c)
            return M(0, 0) * (M(1, 1) * M(2, 2) - M(1, 2) * M(2, 1)) -
                   M(0, 1) * (M(1, 0) * M(2, 2) - M(1, 2) * M(2, 0)) +
                   M(0, 2) * (M(1, 0) * M(2, 1) - M(1, 1) * M(2, 0));
            #undef M
        }

        /**
         * @brief   Calculates the inverse of this matrix, if it exists.
         * 
         * @return  The inverse of the matrix, if it exists; otherwise, a zero
         *          matrix.
         */
        constexpr Matrix3<T> Inverse () const noexcept
        { 
            // Check the determinant first. If zero, then this is a singular
            // matrix, and we cannot calculate the inverse. Return a zero
            // matrix instead.
            T lDeterminant = Determinant();
            if (lDeterminant == ZERO<T>)
            {
                return Zero();
            }

            // If the determinant is one, then this is an identity matrix, in
            // which case the inverse is itself.
            else if (lDeterminant == ONE<T>)
            {
                return *this;
            }

            // Helper macro to access this matrix's elements.
            #define M(r, c) (*this)(r, c)

            // Calculate the cofactors for the adjugate matrix.
            Matrix3<T> lAdjugate;
            lAdjugate(0, 0) =   M(1, 1) * M(2, 2) - M(1, 2) * M(2, 1);
            lAdjugate(0, 1) =  -M(1, 0) * M(2, 2) + M(1, 2) * M(2, 0);
            lAdjugate(0, 2) =   M(1, 0) * M(2, 1) - M(1, 1) * M(2, 0);
            lAdjugate(1, 0) =  -M(0, 1) * M(2, 2) + M(0, 2) * M(2, 1);
            lAdjugate(1, 1) =   M(0, 0) * M(2, 2) - M(0, 2) * M(2, 0);
            lAdjugate(1, 2) =  -M(0, 0) * M(2, 1) + M(0, 1) * M(2, 0);
            lAdjugate(2, 0) =   M(0, 1) * M(1, 2) - M(0, 2) * M(1, 1);
            lAdjugate(2, 1) =  -M(0, 0) * M(1, 2) + M(0, 2) * M(1, 0);
            lAdjugate(2, 2) =   M(0, 0) * M(1, 1) - M(0, 1) * M(1, 0);
            #undef M

            // Divide the adjugate matrix by the determinant to get the inverse.
            return lAdjugate / lDeterminant;
        }

    };

    /**
     * @brief   Checks to see if the elements of two matrices are close enough
     *          to equal, using an epsilon value.
     * 
     * @param   pFirst      The first matrix.
     * @param   pSecond     The second matrix.
     * 
     * @return  `true` if the matrices' elements are close enough to equal; 
     *          `false` if not.
     */
    template <typename T>
    inline constexpr bool EpsilonEqual (
        const Matrix3<T>&   pFirst,
        const Matrix3<T>&   pSecond
    ) noexcept
    {
        for (std::size_t i = 0; i < Matrix3<T>::COUNT; ++i)
        {
            if (EpsilonEqual<T>(pFirst.mI[i], pSecond.mI[i]) == false)
            {
                return false;
            }
        }

        return true;
    }
    
    using Matrix3f = Matrix3<float>;
    using Matrix3d = Matrix3<double>;
    using Matrix3i = Matrix3<std::int32_t>;
    using Matrix3u = Matrix3<std::uint32_t>;
    using Matrix3b = Matrix3<bool>;

}
