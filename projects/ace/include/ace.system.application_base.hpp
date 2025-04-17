/**
 * @file  ace.system.application_base.hpp
 * @brief Contains the base class for the Ace Engine's client application.
 */

#pragma once
#include <ace.system.asset_manager.hpp>
#include <ace.system.event_dispatcher.hpp>

namespace ace
{

    /**
     * @brief The `application_specification` class contains information on how
     *        the client application and its components should be created and
     *        their operation.
     */
    struct application_specification
    {

        /**
         * @brief The name of the client application's logger.
         */
        std::string client_name = "acclient";
    
        /**
         * @brief The application's target framerate. This value is used to
         *        determine the client application's fixed timestep value.
         */
        float frame_rate = 60.0f;

    };

    /**
     * @brief The `application_base` class is the base class for the Ace Engine's
     *        client application.
     */
    class ace_api application_base
    {
    protected:
        
        /**
         * @brief Constructs an `application_base` with the given application
         *        specification.
         * 
         * @param p_spec    The application's `application_specification`.
         */
        explicit application_base (
            const application_specification& p_spec
        );

    public:
        
        /**
         * @brief The default destructor.
         */
        virtual ~application_base ();

        /**
         * @brief Prepares, then starts, the client application's main loop.
         * 
         * @return A status code, which is intended to be the return value for
         *         the client application's `main` function.
         */
        virtual int start ();

        /**
         * @brief Retrieves the client application's main asset manager.
         * 
         * @return A pointer to the client application's main asset manager.
         */
        asset_manager& main_asset_manager ();

        /**
         * @brief Retrieves the client application's main event dispatcher.
         * 
         * @return A pointer to the client application's main event dispatcher.
         */
        event_dispatcher& main_event_dispatcher ();

    protected:

        /**
         * @brief A pointer to the created application instance.
         * 
         * There can only ever be one instance of the ACE engine's client
         * application. If that one instance has been created, this will point
         * to that instance.
         */
        static application_base* s_instance_ptr;

        /**
         * @brief A pointer to the client application's central asset manager.
         */
        std::unique_ptr<asset_manager> m_asset_manager;

        /**
         * @brief A pointer to the client application's main event dispatcher.
         */
        std::unique_ptr<event_dispatcher> m_event_dispatcher;

        /**
         * @brief The application's fixed timestep.
         */
        float m_fixed_timestep = 0.0f;

    };  

    /**
     * @brief   Aliases for `ace::application_base`.
     */
    using application   = application_base;

}
