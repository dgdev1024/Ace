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

    /* Test Structure *********************************************************/

    struct WindowResized
    {
        std::uint32_t mWidth, mHeight;
    };

    /* Constructors and Destructor ********************************************/

    Application::Application (
        const ace::ApplicationSpec& pSpec
    ):
        ace::IApplication   { pSpec }
    {
        ace::Logger::RegisterSink(std::make_shared<ace::LoggerConsoleSink>());

        ace::EventBus::Subscribe<WindowResized>(
            [] (const WindowResized& pEvent)
            {
                ACE_LOG_INFO("Window Resized: {}, {}", pEvent.mWidth,
                    pEvent.mHeight);
                return false;
            }
        );

        ace::EventBus::Publish(WindowResized { 1280, 780 });
    }

    Application::~Application ()
    {
        ACE_LOG_WARNING("The destructor is running.");
        ace::EventBus::Dispatch();
    }

}
