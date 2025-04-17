/**
 * @file ace.system.logging.cpp
 */

#include <ace.pch.hpp>
#include <ace.system.logging.hpp>

namespace ace
{

    /* Static Members *********************************************************/

    bool                    logging::s_initialized   = false;
    std::shared_ptr<logger> logging::s_engine_logger = nullptr;
    std::shared_ptr<logger> logging::s_client_logger = nullptr;

    /* Public Methods *********************************************************/

    void logging::initialize (const std::string& p_client_name)
    {
        if (s_initialized == true)
        {
            return;
        }

        s_engine_logger = std::make_shared<logger>("acengine");
        s_client_logger = std::make_shared<logger>(p_client_name);
        s_initialized = true;
    }

    std::shared_ptr<logger>& logging::engine_logger ()
    {
        assert(s_engine_logger != nullptr);
        return s_engine_logger;
    }

    std::shared_ptr<logger>& logging::client_logger ()
    {
        assert(s_client_logger != nullptr);
        return s_client_logger;
    }

}
