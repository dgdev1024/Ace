#define ACE_USE_OWN_MAIN
#include <iostream>
#include <string>
#include <functional>
#include <MathsTesting/TestVector3.hpp>

#define FN(F) { #F, F }

static const std::unordered_map<std::string, std::function<bool()>> 
    TestFunctions = {
        FN(AceVector3::TestBasic),
        FN(AceVector3::TestDotCross),
        FN(AceVector3::TestLengthNormalizeDistance),
        FN(AceVector3::TestProjection),
        FN(AceVector3::TestReflection),
        FN(AceVector3::TestRefraction)
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
