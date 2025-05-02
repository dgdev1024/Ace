/**
 * @file    Ace/System/Logging.hpp
 * @brief   Contains a static class used for accessing logging functionality for the engine and
 *          client application.
 */

#pragma once
#include <Ace/Common.hpp>

#if defined(ACE_USE_OWN_LOGGING)

#else
    #include <spdlog/spdlog.h>
#endif

namespace ace
{

    #if !defined(ACE_USE_OWN_LOGGING)
        using Logger = spdlog::logger;
    #endif

    /**
     * @struct  `ace::LoggingSpecification`
     * @brief   A structure containing important information used for initializing the engine's
     *          logging functionality.
     */
    struct LoggingSpecification
    {

        /**
         * @def     `ace::LoggingSpecification::mEngineName`
         * @brief   The string name used to identify the engine library's logger.
         */
        String mEngineName = "ACE";
        
        /**
         * @def     `ace::LoggingSpecification::mClientName`
         * @brief   The string name used to identify the client application's logger.
         */
        String mClientName = "APP";

    };

    /**
     * @class   `ace::Logging`
     * @brief   A static class used for accessing logging functionality used by the engine and
     *          the client application.
     */
    class ACE_API Logging
    {
    public:

        /**
         * @fn      `ace::Logging::initialize`
         * @brief   Initializes the engine's logging system.
         * 
         * @param   pSpec   The @a `LoggingSpecification` used to initialize the logging system.
         */
        static void initialize (
            const LoggingSpecification& pSpec
        );

        /**
         * @fn      `ace::Logging::getEngineLogger`
         * @brief   Retrieves the logger used by the engine library.
         * 
         * @return  A pointer to the logger used by the engine library.
         */
        inline static Shared<Logger> getEngineLogger ()
        {
            return sEngineLogger;
        }

        /**
         * @fn      `ace::Logging::getClientLogger`
         * @brief   Retrieves the logger used by the client application.
         * 
         * @return  A pointer to the logger used by the client application.
         */
        inline static Shared<Logger> getClientLogger ()
        {
            return sClientLogger;
        }

    private:

        /**
         * @def     `ace::Logging::sEngineLogger`
         * @brief   A pointer to the logger used by the engine library.
         */
        static Shared<Logger> sEngineLogger;

        /**
         * @def     `ace::Logging::sClientLogger`
         * @brief   A pointer to the logger used by the client application.
         */
        static Shared<Logger> sClientLogger;

    };

}

#if defined(ACE_USE_OWN_LOGGING)

#else
    #if defined(ACE_DEBUG)
        #define ACE_ENGINE_TRACE(...)   ::ace::Logging::getEngineLogger()->trace(__VA_ARGS__)
        #define ACE_APP_TRACE(...)      ::ace::Logging::getClientLogger()->trace(__VA_ARGS__)
    #else
        #define ACE_ENGINE_TRACE(...)
        #define ACE_APP_TRACE(...)
    #endif

    #define ACE_ENGINE_INFO(...)        ::ace::Logging::getEngineLogger()->info(__VA_ARGS__)
    #define ACE_ENGINE_WARN(...)        ::ace::Logging::getEngineLogger()->warn(__VA_ARGS__)
    #define ACE_ENGINE_ERROR(...)       ::ace::Logging::getEngineLogger()->error(__VA_ARGS__)
    #define ACE_ENGINE_CRITICAL(...)    ::ace::Logging::getEngineLogger()->critical(__VA_ARGS__)

    #define ACE_APP_INFO(...)           ::ace::Logging::getClientLogger()->info(__VA_ARGS__)
    #define ACE_APP_WARN(...)           ::ace::Logging::getClientLogger()->warn(__VA_ARGS__)
    #define ACE_APP_ERROR(...)          ::ace::Logging::getClientLogger()->error(__VA_ARGS__)
    #define ACE_APP_CRITICAL(...)       ::ace::Logging::getClientLogger()->critical(__VA_ARGS__)
#endif
