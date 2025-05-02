/**
 * @file    Ace/System/Application.cpp
 */

#include <Ace/Precompiled.hpp>
#include <Ace/System/Application.hpp>

namespace ace
{

    /* Constructors and Destructor ****************************************************************/

    Application::Application (
        const ApplicationSpecification& pSpec
    )
    {
        aceUnused(pSpec);
    }

    Application::~Application ()
    {

    }

    /* Public Methods *****************************************************************************/

    Int32 Application::start ()
    {
        return 0;
    }

}
