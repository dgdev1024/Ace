/**
 * @file    MathsTesting/TestMatrix4.hpp
 */

#pragma once
#include <Ace/Maths/Matrix4.hpp>

namespace AceMatrix4
{
    bool TestBasic ();
    bool TestAccessors ();
    bool TestAddSubtract ();
    bool TestMatrixMultiply ();
    bool TestMatrixVectorMultiply ();
    bool TestMatrixScalarMultiplyDivide ();
    bool TestDeterminant ();
    bool TestInverse ();
    bool TestTranspose ();
}
