/**
 * @file    AceBox/Application.cpp
 */

#include <iostream>
#include <JS/Token.hpp>
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
        js::Token lToken { js::TokenType::NumericLiteral, "3.14159", 1, 1 };
        ACE_APP_INFO("Token is {}.", lToken.getLiteralAs<js::NumberType>());
    }

    Application::~Application ()
    {
        
    }

}
