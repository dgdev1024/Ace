/**
 * @file    MathsTesting/TestVector2.cpp
 */

#include <MathsTesting/TestVector2.hpp>

namespace AceVector2
{

    ace::Vector2f   a { 3.0f, 4.0f };
    ace::Vector2f   b { -1.0f, 2.0f };

    bool TestBasic ()
    {
        return
            a + b == ace::Vector2f { 2.0f, 6.0f } &&
            a - b == ace::Vector2f { 4.0f, 2.0f } &&
            a * 2.0f == ace::Vector2f { 6.0f, 8.0f } &&
            b / 2.0f == ace::Vector2f { -0.5f, 1.0f };
    }

    bool TestDot ()
    {
        float lDot = ace::Dot(a, b);
        return
            ace::EpsilonEqual(lDot, 5.0f);
    }

    bool TestLengthNormalizeDistance ()
    {
        float           lLength = a.Length();
        float           lDistance = ace::Distance(a, b);
        ace::Vector2f   lNormalized = a.Normalized();
        ace::Vector2f   lZero = ace::Vector2f::Zero();

        return
            ace::EpsilonEqual(lLength, 5.0f) &&
            ace::EpsilonEqual(lDistance, (a - b).Length()) &&
            ace::EpsilonEqual(lNormalized.Length(), 1.0f) &&
            lZero.Normalized() == lZero;
    }

    bool TestProjection ()
    {
        ace::Vector2f   lProjection = ace::Project(a, b);
        
        return
            ace::EpsilonEqual(lProjection, b);
    }

    bool TestReflection ()
    {
        ace::Vector2f   lUp = ace::Vector2f::Up();
        ace::Vector2f   lReflection = ace::Reflect(a, lUp);
        ace::Vector2f   lExpected = ace::Vector2f { 3.0f, -4.0f };

        return lReflection == lExpected;
    }

    bool TestRefraction ()
    {
        ace::Vector2f   lDirection { 0.707f, -0.707f }; // 45 degrees downward.
        ace::Vector2f   lNormal { 0.0f, 1.0f };
        float           lRatio = 1.5f;

        ace::Vector2f   lRefraction = ace::Refract(lDirection, lNormal, lRatio);
        ace::Vector2f   lExpected = ace::Reflect(lDirection, lNormal);

        return
            ace::EpsilonEqual(lRefraction, lExpected);
    }

}