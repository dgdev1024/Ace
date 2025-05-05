/**
 * @file    AceBox/Application.cpp
 */

#include <iostream>
#include <AceBox/Precompiled.hpp>
#include <AceBox/Application.hpp>

namespace ace
{

    std::unique_ptr<Application> makeApplication ()
    {
        ApplicationSpecification lSpec;
        lSpec.mLogging.mClientName = "ACEBOX";
        
        return makeUnique<acebox::Application>(lSpec);
    }

}

namespace acebox
{

    struct TextAsset
    {
        std::string mContents = "";

        inline bool deserialize (std::span<std::uint8_t> pData)
        {
            mContents.assign(pData.begin(), pData.end());
            return true;
        }
    };

    /* Constructors and Destructor ****************************************************************/

    Application::Application (
        const ace::ApplicationSpecification& pSpec
    ) :
        ace::Application    { pSpec }
    {
        constexpr ace::UniqueID TEXT_ASSET { 0x1234, 0x5678 };

        ace::Filesystem::mountAssetBundle("./notes/package.ace");
        auto lText = ace::AssetManager::loadAsset<TextAsset>(TEXT_ASSET, "code/package.hpp");

        std::cout << lText.to<TextAsset>()->mContents << std::endl;
    }

    Application::~Application ()
    {
        
    }

}
