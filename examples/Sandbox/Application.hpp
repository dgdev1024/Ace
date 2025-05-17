/**
 * @file    Sandbox/Application.hpp
 */

#pragma once
#include <Ace/Ace.hpp>

namespace sandbox
{

    class Application : public ace::IApplication
    {
    public:
        explicit Application (const ace::ApplicationSpec& pSpec);
        virtual ~Application () override;
        
    };

}
