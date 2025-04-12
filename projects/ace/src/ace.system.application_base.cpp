/**
 * @file  ace.system.application_base.cpp
 */

#include <ace.pch.hpp>
#include <ace.system.application_base.hpp>

namespace ace
{

    /* Constructors and Destructor ********************************************/

    application_base::application_base (
        const application_specification& p_spec
    ) :
        m_fixed_timestep        { 1.0f / p_spec.frame_rate }
    {

    }

    application_base::~application_base ()
    {

    }

    /* Public Methods *********************************************************/

    int application_base::start ()
    {
        return 0;
    }

}
