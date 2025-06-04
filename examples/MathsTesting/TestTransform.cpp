/**
 * @file    MathsTesting/TestTransform.cpp
 */

#include <Ace/System/Testing.hpp>
#include <MathsTesting/TestTransform.hpp>

namespace AceTransform
{
    static constexpr float HALF_PI = ace::PI<float> / ace::TWO<float>;

    bool TestScale ()
    {
        ace::Vector3f lFactors { 2.0f, 3.0f, 4.0f };
        ace::Matrix4f lScale = ace::Scale(lFactors);
        ace::Vector3f lVector { 8.0f, 3.0f, 10.0f };
        ace::Vector3f lOut = lScale * lVector;
        ace::Vector3f lExpected { 16.0f, 9.0f, 40.0f };

        ACE_EXPECT(
            ace::EpsilonEqual(lOut, lExpected),
            "Scale test failed."
        );

        return true;
    }

    bool TestTranslate ()
    {
        ace::Vector3f lFactors { 2.0f, 3.0f, 4.0f };
        ace::Matrix4f lTranslate = ace::Translate(lFactors);
        ace::Vector3f lVector { 8.0f, 3.0f, 10.0f };
        ace::Vector3f lOut = lTranslate * lVector;
        ace::Vector3f lExpected { 10.0f, 6.0f, 14.0f };

        ACE_EXPECT(
            ace::EpsilonEqual(lOut, lExpected),
            "Translation test failed."
        );

        return true;
    }

    bool TestRotateX ()
    {
        ace::Matrix4f lRotation = ace::RotateX(HALF_PI);
        ace::Vector3f lPoint = ace::Vector3f::Up();
        ace::Vector3f lOut = lRotation * lPoint;
        
        ACE_EXPECT(
            ace::EpsilonEqual(lOut, ace::Vector3f::Front()),
            "X-axis rotation test failed."
        );

        return true;
    }

    bool TestRotateY ()
    {
        ace::Matrix4f lRotation = ace::RotateY(HALF_PI);
        ace::Vector3f lPoint = ace::Vector3f::Right();
        ace::Vector3f lOut = lRotation * lPoint;
        
        ACE_EXPECT(
            ace::EpsilonEqual(lOut, -ace::Vector3f::Front()),
            "Y-axis rotation test failed."
        );

        return true;
    }

    bool TestRotateZ ()
    {
        ace::Matrix4f lRotation = ace::RotateZ(HALF_PI);
        ace::Vector3f lPoint = ace::Vector3f::Right();
        ace::Vector3f lOut = lRotation * lPoint;

        ACE_EXPECT(
            ace::EpsilonEqual(lOut, ace::Vector3f::Up()),
            "Z-axis rotation test failed."
        );

        return true;
    }

    bool TestRotateArbitrary ()
    {
        ace::Vector3f lAxis = ace::Vector3f { 1.0f, 1.0f, 0.0f }.Normalize();
        ace::Matrix4f lRotation = ace::Rotate(HALF_PI, lAxis);

        ace::Vector3f lPerpen { 1.0f, -1.0f, 0.0f };
        ace::Vector3f lOut = lRotation * lPerpen;
        ace::Vector3f lExpected { 0.0f, 0.0f, -std::sqrt(2.0f) };

        ACE_EXPECT(
            ace::EpsilonEqual(lOut, lExpected),
            "Arbitrary axis rotation test failed."
        );

        return true;
    }
}
