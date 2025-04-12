/**
 * @file  abox.application.cpp
 */

#include <abox.application.hpp>

namespace abox
{

    /* Test Event Structures **************************************************/

    struct ping_event
    {
        std::string message = "";
    };

    struct count_event
    {
        int count = 0;
    };

    /* Constructors and Destructor ********************************************/

    application::application (const ace::application_specification& p_spec) :
        ace::application_base { p_spec }
    {
        ace::event_dispatcher l_dispatcher;
        l_dispatcher.subscribe<ping_event>([&] (const ping_event& p_event)
        {
            ace_info("Dispatching ping event: {}", p_event.message);
        });
        l_dispatcher.subscribe<count_event>([&] (const count_event& p_event)
        {
            ace_info("Dispatching count event: {}", p_event.count);
        });

        l_dispatcher.publish(ping_event { "Hello, World!" });
        l_dispatcher.publish(count_event { 99 });
        l_dispatcher.publish(ping_event { "Another Ping!!" });

        ace_info("Dispatching events...");
        l_dispatcher.dispatch();
    }

    application::~application ()
    {
        
    }

}
