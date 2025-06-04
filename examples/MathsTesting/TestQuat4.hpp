/**
 * @file    MathsTesting/TestQuat4.hpp
 */

#pragma once
#include <Ace/Maths/Quaternion4.hpp>

namespace AceQuat4
{
    bool TestIdentity ();
    bool TestFromAxisAngle ();
    bool TestFromEulerAngles ();
    bool TestToVector4 ();
    bool TestNormalized ();
    bool TestConjugateInverse ();
    bool TestNormalizedLerp ();
    bool TestSphericalLerp ();
    bool TestSphericalLerp180 ();
}
