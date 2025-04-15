/**
 * @file  ace.system.application_base.cpp
 */

#include <ace.pch.hpp>
#include <ace.system.application_base.hpp>

namespace ace
{

    /* Static Members *********************************************************/

    application_base* application_base::s_instance_ptr = nullptr;

    /* Constructors and Destructor ********************************************/

    application_base::application_base (
        const application_specification& p_spec
    ) :
        m_fixed_timestep        { 1.0f / p_spec.frame_rate }
    {
        // Ensure that the application instance has not already been created.
        if (s_instance_ptr != nullptr)
        {
            throw std::runtime_error {
                "Attempt to create application instance when one already exists!"
            };
        }
        else
        {
            s_instance_ptr = this;
        }

        // Initialize logging.
        logging::initialize(p_spec.client_name);

        // Create the application's components.
        m_event_dispatcher = std::make_unique<event_dispatcher>();
    }

    application_base::~application_base ()
    {
        // Destroy the application's components in the order they were created.
        m_event_dispatcher.reset();

        s_instance_ptr = nullptr;
    }

    /* Public Methods *********************************************************/

    int application_base::start ()
    {
        return 0;
    }

    event_dispatcher& application_base::main_event_dispatcher ()
    {
        assert(m_event_dispatcher != nullptr);
        return *m_event_dispatcher;
    }

}
