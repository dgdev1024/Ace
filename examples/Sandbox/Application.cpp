/**
 * @file    Sandbox/Application.cpp
 */

#include <iostream>
#include <Sandbox/Application.hpp>

std::unique_ptr<ace::IApplication> ace::MakeApplication ()
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
        ace::IApplication   { pSpec }
    {
    }

    Application::~Application ()
    {
        
    }

}
