/**
 * @file    Ace/System/Application.hpp
 * @brief   Contains the base class for the Ace Engine's client application.
 */

#pragma once
#include <Ace/Common.hpp>

namespace ace
{

    /**
     * @struct  `ace::ApplicationSpecification`
     * @brief   A structure containing important information defining the Ace Engine's client
     *          application, and its components.
     */
    struct ApplicationSpecification
    {

    };

    /**
     * @class   `ace::Application`
     * @brief   The base class for the Ace Engine's client application.
     */
    class ACE_API Application
    {
    protected:

        /**
         * @fn      `ace::Application::Application`
         * @brief   Constructs a new @a `Application` with the given @a `ApplicationSpecification`.
         * 
         * The @a `Application` class is intended to be derived from by the client application. As
         * such, this constructor is not public.
         * 
         * @param   pSpec   The application's specification. 
         */   
        explicit Application (
            const ApplicationSpecification& pSpec
        );

    public:

        /**
         * @fn      `ace::Application::~Application`
         * @brief   The default destructor.
         */
        virtual ~Application ();

    public:

        /**
         * @fn      `ace::Application::start`
         * @brief   Starts the application, running its application loop.
         * 
         * The return value of this method is intended to be the return value of the client
         * application's `main` function.
         * 
         * @return  `0` if the application loop exits without error; a non-zero value if the loop
         *          exits due to an error or exception.
         */
        virtual Int32 start ();

    };

    /**
     * @fn      `ace::makeApplication`
     * @brief   Creates the client application instance.
     * 
     * This function needs to be implemented by the client application.
     * 
     * @return  A unique pointer to the created application instance.
     */
    Unique<Application> makeApplication ();

}
