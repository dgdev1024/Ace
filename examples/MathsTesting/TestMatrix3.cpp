/**
 * @file MathsTesting/TestMatrix3.cpp
 */

#include <Ace/System/Testing.hpp>
#include <MathsTesting/TestMatrix3.hpp>

namespace AceMatrix3
{
    ace::Matrix3f a {
        1.0f, 2.0f, 3.0f,
        4.0f, 5.0f, 6.0f,
        7.0f, 8.0f, 9.0f
    }, b {
        9.0f, 8.0f, 7.0f, 
        6.0f, 5.0f, 4.0f, 
        3.0f, 2.0f, 1.0f
    }, c {
        2.0f, 0.0f, 0.0f,
        0.0f, 3.0f, 0.0f,
        0.0f, 0.0f, 4.0f
    }, d {
        15.0f, 17.0f, 1.0f,
        2.0f,  2.0f,  3.0f,
        12.0f, 14.0f, 2.0f
    };

    bool TestBasic ()
    {
        ace::Matrix3f   lIdentityDefault,
                        lIdentityStatic = ace::Matrix3f::Identity();

        ACE_EXPECT(lIdentityDefault == lIdentityStatic,
            "Mismatching default-constructed and explicit identity matrices.");

        return true;
    }

    bool TestAccessors ()
    {
        const float& lTwoOne = a(2, 1);
        float& lTwoTwo = b(2, 2);

        ACE_EXPECT(lTwoOne == 6.0f,
            "Row 2, Column 1 should equal 6.0f; got {} instead.", lTwoOne);
        ACE_EXPECT(lTwoTwo == 1.0f,
            "Row 2, Column 2 should equal 1.0f; got {} instead.", lTwoTwo);

        return true;
    }

    bool TestAddSubtract ()
    {
        ace::Matrix3f lSum = a + b, lDiff = b - c;
        ace::Matrix3f lExpectedSum {
            10.0f, 10.0f, 10.0f,
            10.0f, 10.0f, 10.0f,
            10.0f, 10.0f, 10.0f
        }, lExpectedDiff {
            7.0f, 8.0f, 7.0f,
            6.0f, 2.0f, 4.0f,
            3.0f, 2.0f, -3.0f
        };

        ACE_EXPECT(lSum == lExpectedSum, "Matrix addition failed.");
        ACE_EXPECT(lDiff == lExpectedDiff, "Matrix subtraction failed.");

        return true;
    }

    bool TestMatrixMultiply ()
    {
        ace::Matrix3f lProduct = a * b;
        ace::Matrix3f lExpectedProduct {
            90.0f, 114.0f, 138.0f,
            54.0f, 69.0f, 84.0f,
            18.0f, 24.0f, 30.0f
        };

        ACE_EXPECT(lProduct == lExpectedProduct, 
            "Matrix vs. matrix multiplication failed.");

        return true;
    }

    bool TestMatrixVectorMultiply ()
    {
        ace::Vector3f lProduct1 = a * ace::Vector3f { 3.0f, 9.0f, 5.0f };
        ace::Vector3f lExpected1 { 74.0f, 91.0f, 108.0f };

        ACE_EXPECT(lProduct1 == lExpected1,
            "Matrix vs. column vector multiplication failed.");

        ace::Vector3f lProduct2 = ace::Vector3f { 6.0f, 11.0f, 38.0f } * b;
        ace::Vector3f lExpected2 { 408.0f, 243.0f, 78.0f };

        ACE_EXPECT(lProduct2 == lExpected2,
            "Row vector vs. matrix multiplication failed.");

        return true;
    }

    bool TestMatrixScalarMultiplyDivide ()
    {
        ace::Matrix3f lProduct = a * 3.0f;
        ace::Matrix3f lExpectedProduct {
            3.0f, 6.0f, 9.0f,
            12.0f, 15.0f, 18.0f,
            21.0f, 24.0f, 27.0f
        };

        ACE_EXPECT(lProduct == lExpectedProduct, 
            "Matrix vs. scalar multiplication failed.");

        ace::Matrix3f lQuotient = b / 2.0f;
        ace::Matrix3f lExpectedQuotient {
            4.5f, 4.0f, 3.5f,
            3.0f, 2.5f, 2.0f,
            1.5f, 1.0f, 0.5f
        };

        ACE_EXPECT(lQuotient == lExpectedQuotient, 
            "Matrix vs. scalar division failed.");

        return true;
    }

    bool TestDeterminant ()
    {
        float lDeterminant = d.Determinant();
        float lExpectedDeterminant = -22.0f;

        ACE_EXPECT(lDeterminant == lExpectedDeterminant,
            "Matrix determinant failed.");

        return true;
    }

    bool TestInverse ()
    {
        ace::Matrix3f lInverse = c.Inverse();
        ace::Matrix3f lExpectedInverse {
            0.5f, 0.0f, 0.0f,
            0.0f, 1.0f / 3.0f, 0.0f,
            0.0f, 0.0f, 1.0f / 4.0f
        };
        ACE_EXPECT(lInverse == lExpectedInverse,
            "Inverse of a matrix is not producing the correct result.");

        // Inverse of an identity matrix should be the identity matrix itself.
        ace::Matrix3f lIdentityInverse = ace::Matrix3f::Identity().Inverse();
        ACE_EXPECT(lIdentityInverse == ace::Matrix3f::Identity(),
            "Inverse of an identity matrix should be the identity matrix itself.");

        // Inverse of a singular matrix (determinant 0) should return a zero
        // matrix.
        ace::Matrix3f lSingularInverse = a.Inverse();
        ace::Matrix3f lZeroMatrix = ace::Matrix3f::Zero();
        ACE_EXPECT(lSingularInverse == lZeroMatrix,
            "Inverse of a singular matrix should be a zero matrix.");

        return true;
    }

    bool TestTranspose ()
    {
        ace::Matrix3f   lTranspose = a.Transpose(),
                        lExpectedTranspose {
                            1.0f, 4.0f, 7.0f,
                            2.0f, 5.0f, 8.0f,
                            3.0f, 6.0f, 9.0f
                        };

        ACE_EXPECT(lTranspose == lExpectedTranspose,
            "lTranspose != lExpectedTranspose");

        // Transpose of an identity matrix should be the identity matrix itself.
        ace::Matrix3f lIdentityTranspose = ace::Matrix3f::Identity().Transpose();
        ACE_EXPECT(lIdentityTranspose == ace::Matrix3f::Identity(),
            "Transpose of an identity matrix should be the identity matrix itself.");

        // Transpose of a zero matrix should be the zero matrix itself.
        ace::Matrix3f lZeroTranspose = ace::Matrix3f::Zero().Transpose();
        ACE_EXPECT(lZeroTranspose == ace::Matrix3f::Zero(),
            "Transpose of a zero matrix should be the zero matrix itself.");

        return true;
    }
}