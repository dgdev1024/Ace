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

    }

    Application::~Application ()
    {

    }

}
