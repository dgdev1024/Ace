/**
 * @file    AceBox/Application.cpp
 */

#include <iostream>
#include <JS/Lexer.hpp>
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

    /* Constructors and Destructor ****************************************************************/

    Application::Application (
        const ace::ApplicationSpecification& pSpec
    ) :
        ace::Application    { pSpec }
    {
        js::Lexer lLexer;
        if (lLexer.tokenizeFile("./notes/grab-bag-1.js") == true)
        {
            for (const auto& lToken : lLexer.getTokens())
            {
                ACE_APP_INFO("{}", lToken.toString());
            }
        }
    }

    Application::~Application ()
    {
        
    }

}
