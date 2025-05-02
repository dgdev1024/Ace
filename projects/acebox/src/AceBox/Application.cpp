/**
 * @file    AceBox/Application.cpp
 */

#include <AceBox/Precompiled.hpp>
#include <AceBox/Application.hpp>

namespace ace
{

    Unique<Application> makeApplication ()
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
        ACE_APP_INFO("Hello From Application!");
    }

    Application::~Application ()
    {

    }

}
