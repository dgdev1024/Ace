/**
 * @file    Sandbox/Application.cpp
 */

#include <iostream>
#include <Sandbox/Application.hpp>

std::unique_ptr<ace::Application> ace::makeApplication ()
{
    ace::ApplicationSpec pSpec;
    return std::make_unique<sandbox::Application>(pSpec);
}

namespace sandbox
{

    /* Constructors and Destructor ********************************************/

    Application::Application (
        const ace::ApplicationSpec& pSpec
    ):
        ace::Application    { pSpec }
    {
        
    }

    Application::~Application ()
    {
        
    }

}
