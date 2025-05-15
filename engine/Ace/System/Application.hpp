/**
 * @file    Ace/System/Application.hpp
 * @brief   Contains the base class for the Ace Engine's client application.
 */

#pragma once
#include <Ace/Common.hpp>

namespace ace
{

    /**
     * @brief   A structure containing attributes which define the Ace Engine's 
     *          client application and its components.
     */
    struct ApplicationSpec
    {
        /**
         * @brief   The client application's maximum framerate.
         * 
         * The client application's fixed timestep is determined by calculating
         * `(1.0f / mFramerate)`.
         */
        float mFramerate = 60.0f;
    };

    /**
     * @brief   The base class for the Ace Engine's client application.
     * 
     * This class is not to be used standalone. It is intended to be derived 
     * from by the client application.
     */
    class ACE_API Application
    {
    protected:

        /**
         * @brief   Constructs an @a `Application` with the given specification.
         * 
         * @param   pSpec   The application's specification.
         */
        explicit Application (
            const ApplicationSpec& pSpec
        );

    public:

        /**
         * @brief   The virtual destructor.
         */
        virtual ~Application ();

        /**
         * @brief   Starts the client application's main loop.
         * 
         * The return value of this method is intended to be returned by the 
         * client application's `main` function.
         * 
         * @return  `0` if the loop exits without error; a non-zero number if an 
         *          error occurs.
         */
        virtual std::int32_t start ();

    protected:

        /**
         * @brief   The application's fixed timestep.
         * 
         * This is the frequency at which the application's @a `fixedUpdate`
         * method is to be called.
         */
        float mFixedTimestep = 0.0f;

    };

    /**
     * @brief   Creates a new instance of the client application.
     * 
     * This function is to be implemented by the client application.
     * 
     * @return  An `std::unique_ptr` to the newly-created application.
     */
    std::unique_ptr<Application> makeApplication ();

}
