/**
 * @file    MathsTesting/TestMatrix4.cpp
 */

#include <Ace/System/Testing.hpp>
#include <MathsTesting/TestMatrix4.hpp>

namespace AceMatrix4
{
    ace::Matrix4f a {
        1.0f,  2.0f,  3.0f,  4.0f,
        5.0f,  6.0f,  7.0f,  8.0f,
        9.0f,  10.0f, 11.0f, 12.0f,
        13.0f, 14.0f, 15.0f, 16.0f
    }, b {
        16.0f, 15.0f, 14.0f, 13.0f,
        12.0f, 11.0f, 10.0f, 9.0f, 
        8.0f,  7.0f,  6.0f,  5.0f, 
        4.0f,  3.0f,  2.0f,  1.0f
    }, c {
        5.0f, 4.0f, 2.0f, 6.0f,
        4.0f, 1.0f, 6.0f, 3.0f,
        2.0f, 4.0f, 8.0f, 9.0f,
        6.0f, 5.0f, 7.0f, 4.0f
    }, d {
        2.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 3.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 4.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 5.0f
    };

    bool TestBasic ()
    {
        ace::Matrix4f   lIdentityDefault,
                        lIdentityStatic = ace::Matrix4f::Identity();

        ACE_EXPECT(lIdentityDefault == lIdentityStatic,
            "Mismatching default-constructed and explicit identity matrices.");

        return true;
    }

    bool TestAccessors ()
    {
        const float& lTwoOne = a(2, 1);
        float& lThreeTwo = a(3, 2);

        ACE_EXPECT(lTwoOne == 7.0f,
            "Row 2, Column 1 should equal 7.0f; got {} instead.", lTwoOne);
        ACE_EXPECT(lThreeTwo == 12.0f,
            "Row 3, Column 2 should equal 12.0f; got {} instead.", lThreeTwo);

        return true;
    }

    bool TestAddSubtract ()
    {
        ace::Matrix4f lSum = a + b;
        ace::Matrix4f lDiff = a - b;
        ace::Matrix4f lExpectedSum {
            17.0f, 17.0f, 17.0f, 17.0f,
            17.0f, 17.0f, 17.0f, 17.0f,
            17.0f, 17.0f, 17.0f, 17.0f,
            17.0f, 17.0f, 17.0f, 17.0f
        }, lExpectedDiff {
            -15.0f, -13.0f, -11.0f, -9.0f,
            -7.0f, -5.0f, -3.0f, -1.0f,
            1.0f, 3.0f, 5.0f, 7.0f,
            9.0f, 11.0f, 13.0f, 15.0f
        };

        ACE_EXPECT(lSum == lExpectedSum, "Addition of matrices failed.");
        ACE_EXPECT(lDiff == lExpectedDiff, "Subtraction of matrices failed.");

        return true;
    }

    bool TestMatrixMultiply ()
    {
        ace::Matrix4f lProduct = a * b;
        ace::Matrix4f lExpectedProduct {
            386.0f, 444.0f, 502.0f, 560.0f,
            274.0f, 316.0f, 358.0f, 400.0f,
            162.0f, 188.0f, 214.0f, 240.0f,
            50.0f,  60.0f,  70.0f,  80.0f
        };

        ACE_EXPECT(lProduct == lExpectedProduct,
            "Matrix multiplication failed.");

        return true;
    }

    bool TestMatrixVectorMultiply ()
    {
        // 1. Column-vector multiplication
        ace::Vector4f lColumnVector {
            1.0f, 2.0f, 3.0f, 4.0f
        };
        ace::Vector4f lColumnResult = a * lColumnVector;
        ace::Vector4f lExpectedColumnResult {
            90.0f, 100.0f, 110.0f, 120.0f
        };
        ACE_EXPECT(lColumnResult == lExpectedColumnResult,
            "Column-vector multiplication failed.");

        // 2. Row-vector multiplication
        ace::Vector4f lRowVector {
            1.0f, 2.0f, 3.0f, 4.0f
        };
        ace::Vector4f lRowResult = lRowVector * a;
        ace::Vector4f lExpectedRowResult {
            30.0f, 70.0f, 110.0f, 150.0f
        };
        ACE_EXPECT(lRowResult == lExpectedRowResult,
            "Row-vector multiplication failed.");

        return true;
    }

    bool TestMatrixScalarMultiplyDivide ()
    {
        ace::Matrix4f lMultiplied = a * 2.0f;
        ace::Matrix4f lExpectedMultiplied {
            2.0f,  4.0f,  6.0f,  8.0f,
            10.0f, 12.0f, 14.0f, 16.0f,
            18.0f, 20.0f, 22.0f, 24.0f,
            26.0f, 28.0f, 30.0f, 32.0f
        };
        ACE_EXPECT(lMultiplied == lExpectedMultiplied,
            "Matrix-scalar multiplication failed.");

        ace::Matrix4f lRighthandMultiplied = 2.0f * b;
        ace::Matrix4f lExpectedRighthandMultiplied {
            32.0f, 30.0f, 28.0f, 26.0f,
            24.0f, 22.0f, 20.0f, 18.0f,
            16.0f, 14.0f, 12.0f, 10.0f,
            8.0f,  6.0f,  4.0f,  2.0f
        };
        ACE_EXPECT(lRighthandMultiplied == lExpectedRighthandMultiplied,
            "Righthand matrix-scalar multiplication failed.");

        ace::Matrix4f lDivided = a / 2.0f;
        ace::Matrix4f lExpectedDivided {
            0.5f,   1.0f,   1.5f,   2.0f,
            2.5f,   3.0f,   3.5f,   4.0f,
            4.5f,   5.0f,   5.5f,   6.0f,
            6.5f,   7.0f,   7.5f,   8.0f
        };
        ACE_EXPECT(lDivided == lExpectedDivided,
            "Matrix-scalar division failed.");

        return true;
    }

    bool TestDeterminant ()
    {
        float lDeterminant = a.Determinant();
        float lExpectedDeterminant = 0.0f; // Determinant of the given matrix is 0.
        ACE_EXPECT(lDeterminant == lExpectedDeterminant,
            "Determinant calculation failed; expected {}, got {}.",
            lExpectedDeterminant, lDeterminant);

        float lDeterminantC = c.Determinant();
        float lExpectedDeterminantC = 699.0f; // Determinant of matrix c.
        ACE_EXPECT(lDeterminantC == lExpectedDeterminantC,
            "Determinant calculation for matrix c failed; expected {}, got {}.",
            lExpectedDeterminantC, lDeterminantC);

        return true;
    }

    bool TestInverse ()
    {
        ace::Matrix4f lInverse = d.Inverse();
        ace::Matrix4f lExpectedInverse {
            0.5f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f / 3.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f / 4.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f / 5.0f
        };

        ACE_EXPECT(ace::EpsilonEqual(lInverse, lExpectedInverse),
            "Matrix inverse calculation failed.");

        // A singular matrix (determinant 0) should return a zero matrix.
        ace::Matrix4f lSingularInverse = a.Inverse();
        ace::Matrix4f lZeroMatrix = ace::Matrix4f::Zero();
        ACE_EXPECT(lSingularInverse == lZeroMatrix,
            "Inverse of a singular matrix should be a zero matrix.");

        // Inverse of an identity matrix should be the identity matrix itself.
        ace::Matrix4f lIdentityInverse = ace::Matrix4f::Identity().Inverse();
        ACE_EXPECT(lIdentityInverse == ace::Matrix4f::Identity(),
            "Inverse of an identity matrix should be the identity matrix itself.");

        return true;
    }

    bool TestTranspose ()
    {
        ace::Matrix4f lExpectedTranspose {
            1.0f, 5.0f, 9.0f, 13.0f,
            2.0f, 6.0f, 10.0f, 14.0f,
            3.0f, 7.0f, 11.0f, 15.0f,
            4.0f, 8.0f, 12.0f, 16.0f
        };
        ace::Matrix4f lTranspose = a.Transpose();
        ACE_EXPECT(lTranspose == lExpectedTranspose,
            "Matrix transpose failed.");

        // Transpose of an identity matrix should be the identity matrix itself.
        ace::Matrix4f lIdentityTranspose = ace::Matrix4f::Identity().Transpose();
        ACE_EXPECT(lIdentityTranspose == ace::Matrix4f::Identity(),
            "Transpose of an identity matrix should be the identity matrix itself.");

        // Transpose of a zero matrix should be the zero matrix itself.
        ace::Matrix4f lZeroTranspose = ace::Matrix4f::Zero().Transpose();
        ACE_EXPECT(lZeroTranspose == ace::Matrix4f::Zero(),
            "Transpose of a zero matrix should be the zero matrix itself.");

        return true;
    }
}
