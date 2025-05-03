/**
 * @file    AceBox/Application.cpp
 */

#include <thread>
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

    template <ace::Uint32 M>
    struct NumberEvent
    {
        ace::Uint32 mNumber = 0;

        NumberEvent (ace::Uint32 pNumber) : mNumber { pNumber * M } {}
    };

    /* Constructors and Destructor ****************************************************************/

    Application::Application (
        const ace::ApplicationSpecification& pSpec
    ) :
        ace::Application    { pSpec }
    {
        using namespace std::chrono_literals;

        ace::EventBus::subscribe<NumberEvent<1>>(
            [&] (const NumberEvent<1>& pEvent)
            {
                ACE_APP_INFO("Number Event <1>: {}", pEvent.mNumber);
                return false;
            }
        );

        ace::EventBus::subscribe<NumberEvent<3>>(
            [&] (const NumberEvent<3>& pEvent)
            {
                ACE_APP_INFO("Number Event <3>: {}", pEvent.mNumber);
                return false;
            }
        );

        std::jthread lThread1 {
            [&] ()
            {
                for (ace::Uint32 i = 0; i < 10; ++i)
                {
                    ace::EventBus::publish<NumberEvent<1>>(i);
                    std::this_thread::sleep_for(2s);
                }
            }
        };

        std::jthread lThread2 {
            [&] ()
            {
                for (ace::Uint32 i = 0; i < 10; ++i)
                {
                    ace::EventBus::publish<NumberEvent<3>>(i);
                    std::this_thread::sleep_for(1s);
                }
            }
        };
    }

    Application::~Application ()
    {
        ace::EventBus::dispatch();
    }

}
