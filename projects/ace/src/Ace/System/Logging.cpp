/**
 * @file    Ace/System/Logging.cpp
 */

#include <Ace/Precompiled.hpp>
#include <Ace/System/Logging.hpp>

#if defined(ACE_USE_OWN_LOGGING)

#else
    #include <spdlog/sinks/stdout_color_sinks.h>
#endif

namespace ace
{

    /* Static Members *****************************************************************************/

    Shared<Logger> Logging::sEngineLogger = nullptr;
    Shared<Logger> Logging::sClientLogger = nullptr;

    /* Public Methods *****************************************************************************/

    void Logging::initialize (
        const LoggingSpecification& pSpec
    )
    {
        #if defined(ACE_USE_OWN_LOGGING)

        #else
            spdlog::set_pattern("%^[%T] %n: %v%$");
            sEngineLogger = spdlog::stdout_color_mt(pSpec.mEngineName);
            sClientLogger = spdlog::stdout_color_mt(pSpec.mClientName);

            sEngineLogger->set_level(spdlog::level::trace);
            sClientLogger->set_level(spdlog::level::trace);
        #endif
    }

}
