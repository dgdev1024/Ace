/**
 * @file    Ace/System/IApplication.cpp
 */

#include <Ace/System/Logger.hpp>
#include <Ace/System/IApplication.hpp>

namespace ace
{

    /* Constructors and Destructor ********************************************/

    IApplication::IApplication (
        const ApplicationSpec& pSpec
    ) :
        mFixedTimestep  { 1.0f / pSpec.mFramerate }
    {
        Logger::Initialize();
    }

    IApplication::~IApplication ()
    {
        Logger::Shutdown();
    }

    /* Public Methods *********************************************************/

    std::int32_t IApplication::Start ()
    {
        return 0;
    }

}
