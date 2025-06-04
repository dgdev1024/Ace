#define ACE_USE_OWN_MAIN
#include <iostream>
#include <string>
#include <functional>
#include <MathsTesting/TestVector2.hpp>
#include <MathsTesting/TestVector3.hpp>
#include <MathsTesting/TestVector4.hpp>
#include <MathsTesting/TestMatrix2.hpp>
#include <MathsTesting/TestMatrix3.hpp>
#include <MathsTesting/TestMatrix4.hpp>
#include <MathsTesting/TestQuat4.hpp>

#define FN(F) { #F, F }

static const std::unordered_map<std::string, std::function<bool()>> 
    TestFunctions = {
        FN(AceVector2::TestBasic),
        FN(AceVector2::TestDot),
        FN(AceVector2::TestLengthNormalizeDistance),
        FN(AceVector2::TestProjection),
        FN(AceVector2::TestReflection),
        FN(AceVector2::TestRefraction),
        FN(AceVector3::TestBasic),
        FN(AceVector3::TestDotCross),
        FN(AceVector3::TestLengthNormalizeDistance),
        FN(AceVector3::TestProjection),
        FN(AceVector3::TestReflection),
        FN(AceVector3::TestRefraction),
        FN(AceVector4::TestBasic),
        FN(AceVector4::TestDot),
        FN(AceVector4::TestLengthNormalizeDistance),
        FN(AceVector4::TestProjection),
        FN(AceVector4::TestReflection),
        FN(AceVector4::TestRefraction),
        FN(AceMatrix2::TestBasic),
        FN(AceMatrix2::TestAccessors),
        FN(AceMatrix2::TestAddSubtract),
        FN(AceMatrix2::TestMatrixMultiply),
        FN(AceMatrix2::TestMatrixVectorMultiply),
        FN(AceMatrix2::TestMatrixScalarMultiplyDivide),
        FN(AceMatrix2::TestDeterminant),
        FN(AceMatrix2::TestInverse),
        FN(AceMatrix2::TestTranspose),
        FN(AceMatrix3::TestBasic),
        FN(AceMatrix3::TestAccessors),
        FN(AceMatrix3::TestAddSubtract),
        FN(AceMatrix3::TestMatrixMultiply),
        FN(AceMatrix3::TestMatrixVectorMultiply),
        FN(AceMatrix3::TestMatrixScalarMultiplyDivide),
        FN(AceMatrix3::TestDeterminant),
        FN(AceMatrix3::TestInverse),
        FN(AceMatrix3::TestTranspose),
        FN(AceMatrix4::TestBasic),
        FN(AceMatrix4::TestAccessors),
        FN(AceMatrix4::TestAddSubtract),
        FN(AceMatrix4::TestMatrixMultiply),
        FN(AceMatrix4::TestMatrixVectorMultiply),
        FN(AceMatrix4::TestMatrixScalarMultiplyDivide),
        FN(AceMatrix4::TestDeterminant),
        FN(AceMatrix4::TestInverse),
        FN(AceMatrix4::TestTranspose),
        FN(AceMatrix4::TestNormalMatrix),
        FN(AceQuat4::TestIdentity),
        FN(AceQuat4::TestFromAxisAngle),
        FN(AceQuat4::TestFromEulerAngles),
        FN(AceQuat4::TestToVector4),
        FN(AceQuat4::TestNormalized),
        FN(AceQuat4::TestConjugateInverse),
        FN(AceQuat4::TestNormalizedLerp),
        FN(AceQuat4::TestSphericalLerp),
        FN(AceQuat4::TestSphericalLerp180)
    };

int main ()
{
    for (const auto& p : TestFunctions)
    {
        if (p.second() == false)
        {
            std::cerr << "Test '" << p.first << "' failed." << std::endl;
            return 1;
        }
    }

    std::cout << "All tests passed.\n";
    return 0;
}
