/**
 * @file MathsTesting/TestMatrix2.cpp
 */

#include <Ace/System/Testing.hpp>
#include <MathsTesting/TestMatrix2.hpp>

namespace AceMatrix2
{
    ace::Matrix2f a {
        1.0f, 2.0f,
        3.0f, 4.0f
    }, b {
        9.0f, 1.0f,
        1.0f, 12.0f
    }, c {
        68.0f, 32.0f,
        48.0f, 28.0f
    }, d {
        1.0f, 2.0f,
        2.0f, 4.0f
    };

    bool TestBasic ()
    {
        ace::Matrix2f   lIdentityDefault,
                        lIdentityStatic = ace::Matrix2f::Identity();

        ACE_EXPECT(lIdentityDefault == lIdentityStatic,
            "Mismatching default-constructed and explicit identity matrices.");

        return true;
    }

    bool TestAccessors ()
    {
        float&          lZeroOne = c(0, 1);
        const float&    lOneZero = b(1, 0);

        ACE_EXPECT(lZeroOne == 48.0f, "float& accessor test failed.");
        ACE_EXPECT(lOneZero == 1.0f, "const float& accessor test failed.");

        return true;
    }

    bool TestAddSubtract ()
    {
        ace::Matrix2f lSum = a + b;
        ace::Matrix2f lDiff = b - c;

        ace::Matrix2f lExpectedSum {
            10.0f, 3.0f,
            4.0f, 16.0f
        };
        ace::Matrix2f lExpectedDiff {
            -59.0f, -31.0f,
            -47.0f, -16.0f
        };

        ACE_EXPECT(lSum == lExpectedSum, "Matrix addition test failed.");
        ACE_EXPECT(lDiff == lExpectedDiff, "Matrix subtraction test failed.");

        return true;
    }

    bool TestMatrixMultiply ()
    {
        ace::Matrix2f lProduct = a * c;
        ace::Matrix2f lExpectedProduct {
            164.0f, 264.0f,
            132.0f, 208.0f
        };

        ACE_EXPECT(lProduct == lExpectedProduct, 
            "Matrix multiplication test failed.");

        return true;
    }

    bool TestMatrixVectorMultiply ()
    {
        ace::Vector2f lColumnVector = c * ace::Vector2f { 4.0f, 2.5f };
        ace::Vector2f lExpectedColumnVector {
            392.0f, 198.0f
        };

        ACE_EXPECT(lColumnVector == lExpectedColumnVector, 
            "Matrix vs column vector multiplication test failed.");

        ace::Vector2f lRowVector = ace::Vector2f { 7.0f, 6.0f } * a;
        ace::Vector2f lExpectedRowVector {
            19.0f, 45.0f
        };

        ACE_EXPECT(lRowVector == lExpectedRowVector, 
            "Row vector vs matrix multiplication test failed.");

        return true;
    }

    bool TestMatrixScalarMultiplyDivide ()
    {
        ace::Matrix2f lProduct = b * 2.0f;
        ace::Matrix2f lExpectedProduct {
            18.0f, 2.0f,
            2.0f, 24.0f
        };

        ACE_EXPECT(lProduct == lExpectedProduct, 
            "Matrix vs. scalar multiplication failed.");

        ace::Matrix2f lQuotient = c / 2.0f;
        ace::Matrix2f lExpectedQuotient {
            34.0f, 16.0f,
            24.0f, 14.0f
        };

        ACE_EXPECT(lQuotient == lExpectedQuotient, 
            "Matrix vs. scalar division failed.");

        return true;
    }

    bool TestDeterminant ()
    {
        float lDeterminant = c.Determinant();
        float lExpected = 368.0f;

        ACE_EXPECT(lDeterminant == lExpected,
            "Matrix determinant test failed.");

        return true;
    }

    bool TestInverse ()
    {
        ace::Matrix2f lInverse = c.Inverse();
        ace::Matrix2f lExpectedInverse {
            7.0f / 92.0f, -2.0f / 23.0f,
            -3.0f / 23.0f, 17.0f / 92.0f
        };
        ACE_EXPECT(lInverse == lExpectedInverse,
            "Inverse of a matrix is not producing the correct result.");

        // Inverse of an identity matrix should be the identity matrix itself.
        ace::Matrix2f lIdentityInverse = ace::Matrix2f::Identity().Inverse();
        ACE_EXPECT(lIdentityInverse == ace::Matrix2f::Identity(),
            "Inverse of an identity matrix should be the identity matrix itself.");

        // Inverse of a singular matrix (determinant 0) should return a zero
        // matrix.
        ace::Matrix2f lSingularInverse = d.Inverse();
        ace::Matrix2f lExpectedSingularInverse = ace::Matrix2f::Zero();
        ACE_EXPECT(lSingularInverse == lExpectedSingularInverse,
            "Inverse of a singular matrix should return a zero matrix.");

        return true;
    }

    bool TestTranspose ()
    {
        ace::Matrix2f lTranspose = b.Transpose();
        ACE_EXPECT(lTranspose == b, "Matrix transpose test failed.");

        return true;
    }
}