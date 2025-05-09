/**
 * @file    AceBox/Application.cpp
 */

#include <iostream>
#include <JS/Lexer.hpp>
#include <JS/Parser.hpp>
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
        if (lLexer.tokenizeFile("./notes/grab-bag-2.js") == true)
        {
            js::Parser lParser { lLexer.getTokens() };
            js::Program lProgram;
            if (lParser.parse(lProgram) == true)
            {
                for (const auto& lStatement : lProgram)
                {
                    std::cout << lStatement->toString() << std::endl;
                }
            }
        }
    }

    Application::~Application ()
    {
        
    }

}
