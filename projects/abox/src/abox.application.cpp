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

    }

    application::~application ()
    {
        
    }

}
