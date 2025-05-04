/**
 * @file    AceBox/Application.cpp
 */

#include <AceBox/Precompiled.hpp>
#include <AceBox/Application.hpp>

namespace ace
{

    std::unique_ptr<Application> makeApplication ()
    {
        ApplicationSpecification lSpec;
        lSpec.mLogging.mClientName = "ACEBOX";
        
        return makeUnique<acebox::Application>(lSpec);
    }

}

namespace acebox
{

    /* Constructors and Destructor ****************************************************************/

    Application::Application (
        const ace::ApplicationSpecification& pSpec
    ) :
        ace::Application    { pSpec }
    {
        ace::Filesystem::mountLooseFolder("./notes");
        // auto lLogoBlue = ace::Filesystem::readAsset("./logo-blue.png");
        // auto lPackageHpp = ace::Filesystem::readAsset("./cgpt.package.hpp");
        // auto lLogoGreen = ace::Filesystem::readAsset("./logo-green.png");
        // auto lLogoRed = ace::Filesystem::readAsset("./logo-red.png");

        // ace::AssetBundle::write(
        //     "./notes/package.ace",
        //     {
        //         { "./logos/blue.png",       lLogoBlue },
        //         { "./code/package.hpp",     lPackageHpp },
        //         { "./logos/green.png",      lLogoGreen },
        //         { "./logos/red.png",        lLogoRed }
        //     }
        // );
    }

    Application::~Application ()
    {
        ace::EventBus::dispatch();
    }

}
