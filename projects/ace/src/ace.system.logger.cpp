/**
 * @file  ace.system.logger.cpp
 */

#include <ace.pch.hpp>
#include <ace.system.logger.hpp>

namespace ace
{

    /* Constructors and Destructor ********************************************/

    logger::logger (const std::string& p_name) :
        m_name  { p_name },
        m_out   { std::cout },
        m_err   { std::cerr }
    {

    }

    logger::logger (const std::string& p_name, std::ostream& p_stream) :
        m_name  { p_name },
        m_out   { p_stream },
        m_err   { p_stream }
    {

    }

    logger::logger (const std::string& p_name, std::ostream& p_out, std::ostream& p_err) :
        m_name  { p_name },
        m_out   { p_out },
        m_err   { p_err }
    {

    }

    logger::~logger ()
    {
        m_out.flush();
        m_err.flush();
    }

}
