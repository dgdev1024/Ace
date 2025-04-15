/**
 * @file  abox.application.cpp
 */

#include <abox.application.hpp>

namespace abox
{

    /* Constructors and Destructor ********************************************/

    application::application (const ace::application_specification& p_spec) :
        ace::application_base { p_spec }
    {
        ace::byte_buffer l_buffer;
        ace::byte_view   l_view { l_buffer };

        l_view.push<std::string>("Dennis W. Griffin");
        for (const std::uint8_t& l_byte : l_buffer)
        {
            ace_debug("{}", l_byte);
        }
    }

    application::~application ()
    {
        
    }

}
