/**
 * @file    MathsTesting/TestVector4.cpp
 */

#include <MathsTesting/TestVector4.hpp>

namespace AceVector4
{

    static constexpr ace::Vector4f a { 1.0f, 2.0f, 3.0f, 4.0f };
    static constexpr ace::Vector4f b { 4.0f, -1.0f, 0.0f, 2.0f };

    bool TestBasic ()
    {
        ace::Vector4f   lExpectedAdd { 5.0f, 1.0f, 3.0f, 6.0f },
                        lExpectedSubtract { -3.0f, 3.0f, 3.0f, 2.0f },
                        lExpectedMultiply { 2.0f, 4.0f, 6.0f, 8.0f },
                        lExpectedDivide { 2.0f, -0.5f, 0.0f, 1.0f };

        return
            (a + b == lExpectedAdd) &&
            (a - b == lExpectedSubtract) &&
            (a * 2.0f == lExpectedMultiply) &&
            (b / 2.0f == lExpectedDivide);
    }

    bool TestDot ()
    {
        float   lDot = ace::Dot(a, b),
                lExpectedDot = 10.0f;

        return ace::EpsilonEqual(lDot, lExpectedDot);
    }

    bool TestLengthNormalizeDistance ()
    {
        ace::Vector4f   lZero                   = ace::Vector4f::Zero();
        float           lLength                 = a.Length(),
                        lExpectedLength         = std::sqrt(1 + 4 + 9 + 16);
        float           lNormalLength           = a.Normalized().Length(),
                        lExpectedNormalLength   = 1.0f;
        float           lDistance               = ace::Distance(a, b),
                        lExpectedDistance       = (a - b).Length();

        return
            lZero.Normalized() == lZero &&
            ace::EpsilonEqual(lLength, lExpectedLength) &&
            ace::EpsilonEqual(lNormalLength, lExpectedNormalLength) &&
            ace::EpsilonEqual(lDistance, lExpectedDistance);
    }

    bool TestProjection ()
    {
        ace::Vector4f   lProjection             = ace::Project(a, b),
                        lExpectedProjection     = b * (10.0f / 21.0f);

        return
            ace::EpsilonEqual(lProjection, lExpectedProjection);
    }

    bool TestReflection ()
    {
        ace::Vector4f   lNormal                 = ace::Vector4f::Up(),
                        lReflection             = ace::Reflect(a, lNormal),
                        lExpectedReflection     { 1, -2, 3, 4 };

        return
            ace::EpsilonEqual(lReflection, lExpectedReflection);
    }

    bool TestRefraction ()
    {
        float           lAirToGlass             = 1.0f / 1.5f,
                        lGlassToAir             = 1.5f / 1.0f;
        ace::Vector4f   lNormal                 = ace::Vector4f::Up(),
                        lDirection              = -lNormal,
                        lShallow                { 1.0f, -0.1f, 0.0f, 0.0f },
                        lRefraction             = ace::Refract(lDirection,
                                                    lNormal,
                                                    lAirToGlass),
                        lExpectedRefraction     = (lDirection * lAirToGlass) +
                                                    lNormal *
                                                    (lAirToGlass - 1.0f),
                        lTIR                    = ace::Refract(lShallow,
                                                    lNormal,
                                                    lGlassToAir),
                        lExpectedTIR            = ace::Reflect(lShallow,
                                                    lNormal);

        return
            ace::EpsilonEqual(lRefraction, lExpectedRefraction) &&
            ace::EpsilonEqual(lTIR, lExpectedTIR);
    }

}
