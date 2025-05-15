/**
 * @file    Ace/System/Application.cpp
 */

#include <Ace/System/Application.hpp>

namespace ace
{

    /* Constructors and Destructor ********************************************/

    Application::Application (
        const ApplicationSpec& pSpec
    ) :
        mFixedTimestep  { 1.0f / pSpec.mFramerate }
    {
        
    }

    Application::~Application ()
    {

    }

    /* Public Methods *********************************************************/

    std::int32_t Application::start ()
    {
        return 0;
    }

}
