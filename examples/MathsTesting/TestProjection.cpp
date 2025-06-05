/**
 * @file    MathsTesting/TestProjection.cpp
 */

#include <Ace/System/Testing.hpp>
#include <MathsTesting/TestProjection.hpp>

namespace AceProjection
{
    bool TestOrtho ()
    {
        float           lLeft       = -4.0f,
                        lRight      = 2.0f,
                        lBottom     = -2.0f,
                        lTop        = 6.0f,
                        lNear       = 1.0f,
                        lFar        = 5.0f;
        ace::Matrix4f lOrtho = ace::Ortho(lLeft, lRight, lBottom, lTop,
            lNear, lFar);

        ace::Vector4f lLeftProj { lLeft, 0.0f, -(lNear + lFar) / 2.0f, 1.0f };
        ace::Vector4f lLeftClip = lOrtho * lLeftProj;
        ACE_EXPECT(
            ace::EpsilonEqual(lLeftClip.mX / lLeftClip.mW, -1.0f),
            "Orthographic left clip test failed."
        );

        ace::Vector4f lRightProj { lRight, 0.0f, -(lNear + lFar) / 2.0f, 1.0f };
        ace::Vector4f lRightClip = lOrtho * lRightProj;
        ACE_EXPECT(
            ace::EpsilonEqual(lRightClip.mX / lRightClip.mW, 1.0f),
            "Orthographic right clip test failed."
        );

        ace::Vector4f lBottomProj { 0.0f, lBottom, -(lNear + lFar) / 2.0f, 1.0f };
        ace::Vector4f lBottomClip = lOrtho * lBottomProj;
        ACE_EXPECT(
            ace::EpsilonEqual(lBottomClip.mY / lBottomClip.mW, -1.0f),
            "Orthographic bottom clip test failed."
        );

        ace::Vector4f lTopProj { 0.0f, lTop, -(lNear + lFar) / 2.0f, 1.0f };
        ace::Vector4f lTopClip = lOrtho * lTopProj;
        ACE_EXPECT(
            ace::EpsilonEqual(lTopClip.mY / lTopClip.mW, 1.0f),
            "Orthographic top clip test failed."
        );

        ace::Vector4f lNearProj { 0.0f, 0.0f, -lNear, 1.0f };
        ace::Vector4f lNearClip = lOrtho * lNearProj;
        ACE_EXPECT(
            ace::EpsilonEqual(lNearClip.mZ / lNearClip.mW, -1.0f),
            "Orthographic near clip test failed."
        );

        ace::Vector4f lFarProj { 0.0f, 0.0f, -lFar, 1.0f };
        ace::Vector4f lFarClip = lOrtho * lFarProj;
        ACE_EXPECT(
            ace::EpsilonEqual(lFarClip.mZ / lFarClip.mW, 1.0f),
            "Orthographic far clip test failed."
        );

        return true;
    }

    bool TestOrthoExtent ()
    {
        float   lExtent = 3.0f,
                lAspectRatio = 16.0f / 9.0f,
                lNear = 1.0f,
                lFar = 5.0f;

        float   lTop = lExtent,
                lBottom = -lExtent,
                lRight = lAspectRatio * lExtent,
                lLeft = -lRight;

        ace::Matrix4f lOrtho = ace::Ortho(lExtent, lAspectRatio, lNear, lFar);
        ace::Matrix4f lExpected = ace::Ortho(lLeft, lRight, lBottom, lTop, lNear, lFar);

        ACE_EXPECT(
            lOrtho == lExpected,
            "Orthographic extent test failed."
        );

        return true;
    }

    bool TestPerspective ()
    {
        float   lFieldOfView = ace::PI<float> / 2.0f,
                lAspectRatio = 1.0f,
                lNear = 1.0f,
                lFar = 3.0f;

        ace::Matrix4f lPerspective = ace::Perspective(lFieldOfView, 
            lAspectRatio, lNear, lFar);

        ace::Matrix4f lExpected {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, -2.0f, -1.0f,
            0.0f, 0.0f, -3.0f, 0.0f
        };

        ACE_EXPECT(
            lPerspective == lExpected,
            "Perspective projection test failed."
        );

        ace::Vector4f lNearProj { 0.0f, 0.0f, -lNear, 1.0f };
        ace::Vector4f lNearClip = lPerspective * lNearProj;
        ACE_EXPECT(
            ace::EpsilonEqual(lNearClip.mZ / lNearClip.mW, -1.0f),
            "Perspective near clip test failed."
        );

        ace::Vector4f lFarProj { 0.0f, 0.0f, -lFar, 1.0f };
        ace::Vector4f lFarClip = lPerspective * lFarProj;
        ACE_EXPECT(
            ace::EpsilonEqual(lFarClip.mZ / lFarClip.mW, 1.0f),
            "Perspective far clip test failed."
        );

        return true;
    }
}
