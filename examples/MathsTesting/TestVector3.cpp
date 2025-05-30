/**
 * @file    MathsTesting/TestVector3.cpp
 */

#include <MathsTesting/TestVector3.hpp>

namespace AceVector3
{
    static constexpr ace::Vector3f a  { 1.0f, 2.0f, 3.0f };
    static constexpr ace::Vector3f b  { 4.0f, -1.0f, 2.0f };

    bool TestBasic ()
    {
        return
            a + b == ace::Vector3f { 5.0f, 1.0f, 5.0f } &&
            a - b == ace::Vector3f { -3.0f, 3.0f, 1.0f } &&
            a * 2.0f == ace::Vector3f { 2.0f, 4.0f, 6.0f } &&
            b / 2.0f == ace::Vector3f { 2.0f, -0.5f, 1.0f };
    }

    bool TestDotCross ()
    {
        float           lDot    = ace::Dot(a, b);
        ace::Vector3f   lCross  = ace::Cross(a, b);

        return
            ace::EpsilonEqual(lDot, 8.0f) &&
            ace::EpsilonEqual(lCross, ace::Vector3f { 7.0f, 10.0f, -9.0f });
    }

    bool TestLengthNormalizeDistance ()
    {
        float           lLength     = a.Length();
        ace::Vector3f   lNormalized = a.Normalized();
        float           lDistance   = ace::Distance(a, b);

        return
            ace::EpsilonEqual(lLength, std::sqrt(14.0f)) &&
            ace::EpsilonEqual(lNormalized.Length(), 1.0f) &&
            ace::EpsilonEqual(lDistance, std::sqrt(19.0f));
    }

    bool TestProjection ()
    {
        ace::Vector3f   lProjection     = ace::Project(a, b);
        ace::Vector3f   lExpected       = b * (8.0f / 21.0f);
        return ace::EpsilonEqual(lProjection, lExpected);
    }

    bool TestReflection ()
    {
        ace::Vector3f   lUp                 = ace::Vector3f::Up();
        ace::Vector3f   lReflection         = ace::Reflect(a, lUp);
        ace::Vector3f   lExpected           = ace::Vector3f { 1.0f, -2.0f, 3.0f };
        return 
            ace::EpsilonEqual(lReflection, lExpected);
    }

    bool TestRefraction ()
    {
        ace::Vector3f   lDirection  { 0.0f, -1.0f, 0.0f };
        ace::Vector3f   lNormal     = ace::Vector3f::Up();
        float           lRatio      = 1.0f / 1.5f;
        float           lRatioOut   = 1.5f / 1.0f;
        ace::Vector3f   lRefraction = ace::Refract(lDirection, lNormal, lRatio);
        ace::Vector3f   lExpectedRefraction =
            (lDirection * lRatio) + lNormal * (lRatio - 1.0f);
        ace::Vector3f   lShallow    = ace::Vector3f { 1.0f, -0.1f, 0.0f };
        lShallow.Normalize();
        ace::Vector3f   lTIR = ace::Refract(lShallow, lNormal, lRatioOut);
        ace::Vector3f   lExpectedTIR = ace::Reflect(lShallow, lNormal);

        return
            ace::EpsilonEqual(lRefraction, lExpectedRefraction) &&
            ace::EpsilonEqual(lTIR, lExpectedTIR);
    }

}