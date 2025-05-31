#define ACE_USE_OWN_MAIN
#include <iostream>
#include <string>
#include <functional>
#include <MathsTesting/TestVector2.hpp>
#include <MathsTesting/TestVector3.hpp>
#include <MathsTesting/TestVector4.hpp>

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
        FN(AceVector4::TestRefraction)
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
