/**
 * @file  ace.system.logging.hpp
 * @brief Provides a static class for accessing loggers for the engine library
 *        and client application.
 */

#pragma once
#include <ace.system.logger.hpp>

namespace ace
{

    /**
     * @brief   The `logger` class is a static class used for accessing console
     *          loggers for the engine library and the client application.
     */
    class ace_api logging
    {
    public:
        
        /**
         * @brief   Initializes the engine and client loggers.
         * @param   p_client_name   The name of the client logger.
         */
        static void initialize (const std::string& p_client_name);

        /**
         * @brief   Retrieves the `logger` dedicated for use within the engine
         *          library.
         * @return  A pointer to the engine logger.
         */
        static std::shared_ptr<logger>& engine_logger ();

        /**
         * @brief   Retrieves the `logger` dedicated for use within the client
         *          application.
         * @return  A pointer to the client logger.
         */
        static std::shared_ptr<logger>& client_logger ();

    private:

        /**
         * @brief Have the loggers already been initialized?
         */
        static bool s_initialized;
        
        /**
         * @brief The `logger` dedicated for use within the engine library.
         */
        static std::shared_ptr<logger> s_engine_logger;

        /**
         * @brief The `logger` dedicated for use within the client application.
         */
        static std::shared_ptr<logger> s_client_logger;

    };

}

#if defined(ace_debug_mode)
    #define _ace_trace(...) \
        ::ace::logging::engine_logger()->put_to_out(::ace::log_level::trace, \
            __VA_ARGS__)
    #define _ace_debug(...) \
        ::ace::logging::engine_logger()->put_to_out(::ace::log_level::debug, \
            __VA_ARGS__)
    #define ace_trace(...) \
        ::ace::logging::client_logger()->put_to_out(::ace::log_level::trace, \
            __VA_ARGS__)
    #define ace_debug(...) \
        ::ace::logging::client_logger()->put_to_out(::ace::log_level::debug, \
            __VA_ARGS__)
#else
    #define _ace_trace(...)
    #define _ace_debug(...)
    #define ace_trace(...)
    #define ace_debug(...)
#endif

#define _ace_info(...) \
    ::ace::logging::engine_logger()->put_to_out(::ace::log_level::info, \
        __VA_ARGS__)
#define _ace_warning(...) \
    ::ace::logging::engine_logger()->put_to_err(::ace::log_level::warning, \
        __VA_ARGS__)
#define _ace_error(...) \
    ::ace::logging::engine_logger()->put_to_err(::ace::log_level::error, \
        __VA_ARGS__)
#define _ace_critical(...) \
    ::ace::logging::engine_logger()->put_to_err(::ace::log_level::critical, \
        __VA_ARGS__)
#define _ace_throw(ex, ...) \
    _ace_critical(__VA_ARGS__); \
    throw ex { ace_funcsig };

#define ace_info(...) \
    ::ace::logging::client_logger()->put_to_out(::ace::log_level::info, \
        __VA_ARGS__)
#define ace_warning(...) \
    ::ace::logging::client_logger()->put_to_err(::ace::log_level::warning, \
        __VA_ARGS__)
#define ace_error(...) \
    ::ace::logging::client_logger()->put_to_err(::ace::log_level::error, \
        __VA_ARGS__)
#define ace_critical(...) \
    ::ace::logging::client_logger()->put_to_err(::ace::log_level::critical, \
        __VA_ARGS__)
