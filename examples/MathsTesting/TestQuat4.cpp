/**
 * @file    MathsTesting/TestQuat4.cpp
 */

#include <Ace/System/Testing.hpp>
#include <MathsTesting/TestQuat4.hpp>

namespace AceQuat4
{
    static const float HALF_PI = ace::PI<float> / 2.0f;
    static const float QUARTER_PI = ace::PI<float> / 4.0f;

    bool TestIdentity ()
    {
        ace::Quaternion4f lQuat;
        ACE_EXPECT(
            lQuat.mX == 0.0f &&
            lQuat.mY == 0.0f &&
            lQuat.mZ == 0.0f &&
            lQuat.mW == 1.0f,
            "Quaternion default constructor test failed."
        );

        ACE_EXPECT(
            lQuat.ToMatrix() == ace::Matrix4f::Identity(),
            "Quaternion to identity matrix test failed."
        );

        return true;
    }

    bool TestFromAxisAngle ()
    {
        ace::Vector3f       lAxis { 0.0f, 0.0f, 1.0f };     // Rotate along the `z` axis.
        float               lAngle = ace::PI<float> / 2.0f; // 90 degrees in radians.
        ace::Quaternion4f   lQuat { lAxis, lAngle };

        // Expected quaternion: { 0, 0, sin(PI / 4), cos(PI / 4) }
        float               lExpectedZ = std::sin(lAngle / 2.0f);
        float               lExpectedW = std::cos(lAngle / 2.0f);
        ace::Quaternion4f   lExpectedQuat { 0.0f, 0.0f, lExpectedZ, lExpectedW };
        ACE_EXPECT(
            ace::EpsilonEqual(lQuat, lExpectedQuat),
            "Quaternion from axis angle test failed."
        );

        // Convert this into a matrix and test rotation.
        ace::Matrix4f lRotation = lQuat.ToMatrix();
        ace::Vector4f lVector { 1.0f, 0.0f, 0.0f, 1.0f };
        ace::Vector4f lOut = lRotation * lVector;
        ace::Vector4f lExpectedOut { 0.0f, 1.0f, 0.0f, 1.0f };
        ACE_EXPECT(
            ace::EpsilonEqual(lOut, lExpectedOut),
            "Quaternion from axis angle rotation test failed."
        );

        return true;
    }

    bool TestFromEulerAngles ()
    {
        float               lAngle = ace::PI<float> / 2.0f; // 90 degrees in radians.
        float               lExpectedSine = std::sin(lAngle / 2.0f);
        float               lExpectedCosine = std::cos(lAngle / 2.0f);

        ace::Quaternion4f   lRollQuat { 0.0f, 0.0f, lAngle };
        ace::Quaternion4f   lExpectedRollQuat { ace::Vector3f::Front(), lAngle };
        ACE_EXPECT(
            ace::EpsilonEqual(lRollQuat, lExpectedRollQuat),
            "Quaternion from euler angles roll test failed."
        );

        ace::Quaternion4f   lPitchQuat { lAngle, 0.0f, 0.0f };
        ace::Quaternion4f   lExpectedPitchQuat { lExpectedSine, 0.0f, 0.0f, lExpectedCosine };
        ACE_EXPECT(
            ace::EpsilonEqual(lPitchQuat, lExpectedPitchQuat),
            "Quaternion from euler angles pitch test failed."
        );

        ace::Quaternion4f   lYawQuat { 0.0f, lAngle, 0.0f };
        ace::Quaternion4f   lExpectedYawQuat { 0.0f, lExpectedSine, 0.0f, lExpectedCosine };
        ACE_EXPECT(
            ace::EpsilonEqual(lYawQuat, lExpectedYawQuat),
            "Quaternion from euler angles yaw test failed."
        );

        return true;
    }

    bool TestToVector4 ()
    {
        ace::Quaternion4f   lQuat { 1.0f, 2.0f, 3.0f, 4.0f };
        ace::Vector4f       lVector { 1.0f, 2.0f, 3.0f, 4.0f };
        ACE_EXPECT(
            lQuat.ToVector4() == lVector,
            "Quaternion to Vector4 conversiont test failed."
        );

        return true;
    }

    bool TestNormalized ()
    {
        ace::Quaternion4f   lQuat1 { 2.0f, 0.0f, 0.0f, 0.0f };
        ace::Quaternion4f   lQuat2 { 2.0f, 0.0f, 0.0f, 0.0f };
        ace::Quaternion4f   lExpected { 1.0f, 0.0f, 0.0f, 0.0f };

        lQuat2.Normalize();

        ACE_EXPECT(
            lQuat1.Normalized() == lExpected &&
            lQuat2 == lExpected,
            "Quaternion normalization test failed."
        );

        return true;
    }

    bool TestConjugateInverse ()
    {
        ace::Quaternion4f   lQuat { 1.0f, 2.0f, 3.0f, 4.0f };
        ace::Quaternion4f   lConjugate = lQuat.Conjugate();
        ace::Quaternion4f   lExpectedConjugate { -1.0f, -2.0f, -3.0f, 4.0f };
        ACE_EXPECT(
            ace::EpsilonEqual(lConjugate, lExpectedConjugate),
            "Quaternion conjugate test failed."
        );

        float               lLengthSquared = lQuat.LengthSquared();
        ace::Quaternion4f   lInverse = lQuat.Inverse();
        ace::Quaternion4f   lExpectedInverse {
            -1.0f / lLengthSquared,
            -2.0f / lLengthSquared,
            -3.0f / lLengthSquared,
            4.0f  / lLengthSquared
        };
        ACE_EXPECT(
            ace::EpsilonEqual(lInverse, lExpectedInverse),
            "Quaternion inverse test failed."
        );

        lQuat.Normalize();
        ace::Quaternion4f   lUnitInverse = lQuat.Inverse();
        ace::Quaternion4f   lUnitConjugate = lQuat.Conjugate();
        ACE_EXPECT(
            ace::EpsilonEqual(lUnitInverse, lUnitConjugate),
            "Quaternion unit inverse-vs-conjugate test failed."
        );

        return true;
    }

    bool TestNormalizedLerp ()
    {
        ace::Quaternion4f   lQuat1;
        ace::Quaternion4f   lQuat2 = lQuat1;
        ace::Quaternion4f   lNlerp = ace::NormalizedLerp(lQuat1, lQuat2, 0.5f);

        ACE_EXPECT(
            ace::EpsilonEqual(lNlerp, lQuat1),
            "Quaternion normalized lerp test failed."
        );

        return true;
    }

    bool TestSphericalLerp ()
    {
        ace::Quaternion4f   lQuat1;
        ace::Quaternion4f   lQuat2 = lQuat1;
        ace::Quaternion4f   lSlerp = ace::SphericalLerp(lQuat1, lQuat2, 0.5f);

        ACE_EXPECT(
            ace::EpsilonEqual(lSlerp, lQuat1),
            "Quaternion spherical lerp test failed."
        );

        return true;
    }

    bool TestSphericalLerp180 ()
    {
        float lSine     = std::sin(ace::PI<float> / 4.0f);
        float lCosine   = std::cos(ace::PI<float> / 4.0f);

        ace::Quaternion4f   lQuat1;
        ace::Quaternion4f   lQuat2 { ace::Vector3f::Right(), ace::PI<float> };
        ace::Quaternion4f   lSlerp = ace::SphericalLerp(lQuat1, lQuat2, 0.5f);
        ace::Quaternion4f   lExpected { lSine, 0.0f, 0.0f, lCosine };

        ACE_EXPECT(
            ace::EpsilonEqual(lSlerp, lExpected),
            "Quaternion halfway spherical lerp test failed."
        );

        return true;
    }

    bool TestRotate ()
    {
        {
            ace::Vector3f       lAxis = ace::Vector3f::Front();
            ace::Quaternion4f   lQuat { lAxis, HALF_PI };
            ace::Vector3f       lVector = ace::Vector3f::Right();
            ace::Vector3f       lOut = ace::Rotate(lQuat, lVector);
            ace::Vector3f       lExpected = ace::Vector3f::Up();

            ACE_EXPECT(
                ace::EpsilonEqual(lOut, lExpected),
                "90 degree Z-axis quaternion rotation test failed."
            );
        }

        {
            ace::Vector3f       lAxis = ace::Vector3f::Right();
            ace::Quaternion4f   lQuat { lAxis, ace::PI<float> };
            ace::Vector3f       lVector = ace::Vector3f::Up();
            ace::Vector3f       lOut = ace::Rotate(lQuat, lVector);
            ace::Vector3f       lExpected = -ace::Vector3f::Up();

            ACE_EXPECT(
                ace::EpsilonEqual(lOut, lExpected),
                "180 degree X-axis quaternion rotation test failed."
            );
        }
        {
            ace::Vector3f       lAxis = ace::Vector3f::Front();
            ace::Quaternion4f   lQuat { lAxis, QUARTER_PI };
            ace::Vector3f       lVector { 1.0f, 1.0f, 0.0f };
            ace::Vector3f       lOut = ace::Rotate(lQuat, lVector.Normalize());
            ace::Vector3f       lExpected = ace::Vector3f::Up();

            ACE_EXPECT(
                ace::EpsilonEqual(lOut, lExpected),
                "45 degree Z-axis quaternion rotation test failed."
            );
        }

        return true;
    }

}
