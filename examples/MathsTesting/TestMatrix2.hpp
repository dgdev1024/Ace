/**
 * @file    MathsTesting/TestMatrix2.hpp
 */

#pragma once
#include <Ace/Maths/Matrix2.hpp>

namespace AceMatrix2
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
