/**
 * @file    MathsTesting/TestMatrix3.hpp
 */

#pragma once
#include <Ace/Maths/Matrix3.hpp>

namespace AceMatrix3
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
