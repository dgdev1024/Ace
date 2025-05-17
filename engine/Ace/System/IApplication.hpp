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
        float mFramerate = 60.0f;       ///< @brief The client application's maximum framerate.
    };
    
    /**
     * @brief   The base interface for the Ace Engine's client application.
     * 
     * This interface is not to be used standalone. It is intended to be derived 
     * from by the client application.
     */
    class ACE_API IApplication
    {
    protected:

        /**
         * @brief   Constructs an @a `Application` with the given specification.
         * 
         * @param   pSpec   The application's specification.
         */
        explicit IApplication (
            const ApplicationSpec& pSpec
        );

    public:

        /**
         * @brief   The virtual destructor.
         */
        virtual ~IApplication ();

        /**
         * @brief   Starts the client application's main loop.
         * 
         * The return value of this method is intended to be returned by the 
         * client application's `main` function.
         * 
         * @return  `0` if the loop exits without error; a non-zero number if an 
         *          error occurs.
         */
        virtual std::int32_t Start ();

    protected:
        float mFixedTimestep = 0.0f;    ///< @brief The application's fixed timestep; controls how frequently @a `fixedUpdate` is called.

    };

    /**
     * @brief   Creates a new instance of the client application.
     * 
     * This function is to be implemented by the client application.
     * 
     * @return  An `std::unique_ptr` to the newly-created application.
     */
    std::unique_ptr<IApplication> MakeApplication ();

}
