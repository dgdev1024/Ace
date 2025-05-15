/**
 * @file    Sandbox/Application.cpp
 */

#include <iostream>
#include <Sandbox/Application.hpp>

std::unique_ptr<ace::IApplication> ace::makeApplication ()
{
    ace::ApplicationSpec pSpec;
    return std::make_unique<sandbox::Application>(pSpec);
}

namespace sandbox
{

    /* Test Sink **************************************************************/

    class StdoutSink final : public ace::ILogSink
    {
    public:
        virtual ~StdoutSink () override = default;
        void Write (const ace::LogEvent& pEvent) override
        {
            std::cout << pEvent.mMessage << '\n';
        }

    };  

    /* Constructors and Destructor ********************************************/

    Application::Application (
        const ace::ApplicationSpec& pSpec
    ):
        ace::IApplication    { pSpec }
    {
        ace::Logger::RegisterSink(std::make_shared<StdoutSink>());

        ACE_LOG_INFO("My name is {} and I am {} years old.", "Dennis W. Griffin", 33);
    }

    Application::~Application ()
    {
        
    }

}
