/**
 * @file    JS/Lexer.cpp
 */

#include <JS/Precompiled.hpp>
#include <JS/Lexer.hpp>

namespace js
{

    /* Static Constants ***************************************************************************/

    static constexpr std::size_t LEXER_DEFAULT_TOKEN_CAPACITY = 128;

    static const std::unordered_map<std::string_view, TokenType> Keywords = {
        { "abstract",       TokenType::Abstract },
        { "arguments",      TokenType::Arguments },
        { "async",          TokenType::Async },
        { "await",          TokenType::Await },
        { "break",          TokenType::Break },
        { "case",           TokenType::Case },
        { "catch",          TokenType::Catch },
        { "class",          TokenType::Class },
        { "const",          TokenType::Const },
        { "continue",       TokenType::Continue },
        { "default",        TokenType::Default },
        { "delete",         TokenType::Delete },
        { "do",             TokenType::Do },
        { "else",           TokenType::Else },
        { "enum",           TokenType::Enum },
        { "export",         TokenType::Export },
        { "extends",        TokenType::Extends },
        { "final",          TokenType::Final },
        { "finally",        TokenType::Finally },
        { "for",            TokenType::For },
        { "function",       TokenType::Function },
        { "goto",           TokenType::Goto },
        { "if",             TokenType::If },
        { "implements",     TokenType::Implements },
        { "import",         TokenType::Import },
        { "in",             TokenType::In },
        { "instanceof",     TokenType::InstanceOf },
        { "interface",      TokenType::Interface },
        { "let",            TokenType::Let },
        { "native",         TokenType::Native },
        { "new",            TokenType::New },
        { "package",        TokenType::Package },
        { "private",        TokenType::Private },
        { "protected",      TokenType::Protected },
        { "public",         TokenType::Public },
        { "return",         TokenType::Return },
        { "static",         TokenType::Static },
        { "super",          TokenType::Super },
        { "switch",         TokenType::Switch },
        { "synchonized",    TokenType::Synchonized },
        { "this",           TokenType::This },
        { "throw",          TokenType::Throw },
        { "throws",         TokenType::Throws },
        { "transient",      TokenType::Transient },
        { "try",            TokenType::Try },
        { "typeof",         TokenType::TypeOf },
        { "undefined",      TokenType::Undefined },
        { "var",            TokenType::Var },
        { "volatile",       TokenType::Volatile },
        { "while",          TokenType::While },
        { "with",           TokenType::With },
        { "yield",          TokenType::Yield },
    };

    /* Constructors and Destructor ****************************************************************/

    Lexer::Lexer ()
    {
        mTokens.reserve(LEXER_DEFAULT_TOKEN_CAPACITY);
    }

    /* Public Methods *****************************************************************************/

    bool Lexer::tokenizeFile (const fs::path& pPath)
    {
        if (pPath.empty() == true)
        {
            std::cerr << "Lex Error: No source file path specified." << std::endl;
            return false;
        }
        
        auto lAbsolute = fs::absolute(pPath).lexically_normal();
        if (mSourcePaths.contains(lAbsolute))
        {
            return true;
        }
        else
        {
            mSourcePaths.emplace(lAbsolute);
        }
        
        if (fs::exists(pPath) == false || fs::is_regular_file(pPath) == false)
        {
            std::cerr   << std::format("Lex Error: Source file '{}' not found.", pPath.string())
                        << std::endl;
            return false;
        }

        std::fstream lFile { pPath, std::ios::in | std::ios::binary | std::ios::ate };
        if (lFile.is_open() == false)
        {
            std::cerr   << std::format("Lex Error: Source file '{}' could not be opened for reading.", pPath.string())
                        << std::endl;
            return false;
        }

        auto lFileSize = lFile.tellg();
        lFile.seekg(0, lFile.beg);

        auto& lSource = mSources.emplace_back(lFileSize, ' ');
        lFile.read(lSource.data(), lSource.size());
        lFile.close();

        mTokens.clear();
        
        bool lOK = tokenize();
        if (lOK == false)
        {
            std::cerr   << std::format("  In File '{}'.", pPath.string())
                        << std::endl;
        }

        return lOK;
    }

    bool Lexer::tokenizeString (const std::string& pSource)
    {
        mSources.emplace_back(pSource);
        mTokens.clear();
        return tokenize();
    }

    const std::vector<Token>& Lexer::getTokens () const
    {
        return mTokens;
    }

    /* Private Methods ****************************************************************************/

    bool Lexer::tokenize ()
    {
        if (mSources.empty())
        {
            std::cerr   << std::format("Lex Error: No source to lex.")
                        << std::endl;
            return false;
        }

        mCurrentSource = &mSources.back();
        mStart = mCurrent = 0;
        mLine = mColumn = 1;

        while (isAtEnd() == false)
        {
            mStart = mCurrent;
            if (scanToken() == false)
            {
                std::cerr   << std::format("  At Line {}:{}.", mLine, mColumn)
                            << std::endl;
                return false;
            }
        }

        addToken(TokenType::EndOfFile, "");
        return true;
    }

    bool Lexer::isAtEnd () const
    {
        return mCurrent >= mCurrentSource->length();
    }

    char Lexer::advance ()
    {
        char lChar = (*mCurrentSource)[mCurrent++];

        if (lChar == '\n')  
        { 
            mLine++; 
            mColumn = 1; 
        }
        else                
        { 
            mColumn++; 
        }

        return lChar;
    }

    void Lexer::addToken (const TokenType& pType)
    {
        std::string_view lSubstr { mCurrentSource->data() + mStart, mCurrent - mStart };

        mTokens.emplace_back(
            pType,
            lSubstr,
            mLine,
            mColumn
        );
    }

    void Lexer::addToken (const TokenType& pType, std::string_view pLexeme)
    {
        mTokens.emplace_back(
            pType,
            pLexeme,
            mLine,
            mColumn
        );
    }

    bool Lexer::match (const char lChar)
    {
        if (isAtEnd() == true || (*mCurrentSource)[mCurrent] != lChar)
        {
            return false;
        }

        mCurrent++;
        mColumn++;
        return true;
    }

    char Lexer::peek () const
    {
        return (isAtEnd() == true) ? '\0' : (*mCurrentSource)[mCurrent];
    }

    char Lexer::peekNext () const
    {
        return (mCurrent + 1 >= mCurrentSource->length()) ? '\0' : (*mCurrentSource)[mCurrent + 1];
    }

    void Lexer::skipLineComment ()
    {
        while (peek() != '\n' && isAtEnd() == false)
        {
            advance();
        }
    }

    void Lexer::skipBlockComment ()
    {
        while (isAtEnd() == false)
        {
            if (peek() == '*' && peekNext() == '/')
            {
                // `advance()` twice to skip past the star-and-slash.
                advance();
                advance();
                break;
            }

            advance();
        }
    }

    bool Lexer::scanToken ()
    {
        char lChar = advance();
        switch (lChar)
        {
            // Arithmetic Operators
            case '+':
                if (match('+'))         
                { 
                    addToken(TokenType::Increment); 
                }
                else if (match('='))    
                { 
                    addToken(TokenType::AssignPlus); 
                }
                else                    
                { 
                    addToken(TokenType::Plus); 
                }
                break;
            case '-':
                if (match('-'))         
                { 
                    addToken(TokenType::Decrement); 
                }
                else if (match('='))    
                { 
                    addToken(TokenType::AssignMinus); 
                }
                else                    
                { 
                    addToken(TokenType::Minus); 
                }
                break;
            case '*':
                if (match('*'))         
                { 
                    addToken(
                        match('=') 
                            ? TokenType::AssignExponent 
                            : TokenType::Exponent
                    ); 
                }
                else if (match('='))    
                { 
                    addToken(TokenType::AssignTimes); 
                }
                else                    
                { 
                    addToken(TokenType::Times); 
                }
                break;
            case '/':
                if (match('/'))         
                { 
                    skipLineComment(); 
                }
                else if (match('*'))    
                { 
                    skipBlockComment(); 
                }
                else if (match('='))    
                { 
                    addToken(TokenType::AssignDivide); 
                }
                else                    
                { 
                    addToken(TokenType::Divide); 
                }
                break;
            case '%':
                if (match('='))         
                { 
                    addToken(TokenType::AssignModulo); 
                }
                else                    
                { 
                    addToken(TokenType::Modulo); 
                }
                break;
            case '<':
                if (match('<'))         
                { 
                    addToken(
                        match('=') 
                            ? TokenType::AssignBitwiseShiftLeft 
                            : TokenType::BitwiseShiftLeft
                    ); 
                }
                else if (match('='))    
                { 
                    addToken(TokenType::CompareLessEqual); 
                }
                else                    
                { 
                    addToken(TokenType::CompareLess); 
                }
                break;
            case '>':
                if (match('>'))
                
                {

                    if (match('>'))    

                    { 
                        addToken(
                            match('=') 
                                ? TokenType::AssignBitwiseShiftRightUnsigned 
                                : TokenType::BitwiseShiftRightUnsigned
                        ); 
                    }
                    else                
                    { 
                        addToken(
                            match('=') 
                                ? TokenType::AssignBitwiseShiftRight 
                                : TokenType::BitwiseShiftRight
                        ); 
                    }
                }
                else if (match('='))    
                { 
                    addToken(TokenType::CompareGreaterEqual); 
                }
                else                    
                { 
                    addToken(TokenType::CompareGreater); 
                }
                break;
            case '&':
                if (match('&'))         
                { 
                    addToken(TokenType::LogicalAnd); 
                }
                else if (match('='))    
                { 
                    addToken(TokenType::AssignBitwiseAnd); 
                }
                else                    
                { 
                    addToken(TokenType::BitwiseAnd); 
                }
                break;
            case '|':
                if (match('|'))         
                { 
                    addToken(TokenType::LogicalOr); 
                }
                else if (match('='))    
                { 
                    addToken(TokenType::AssignBitwiseOr); 
                }
                else                    
                { 
                    addToken(TokenType::BitwiseOr); 
                }
                break;
            case '^':
                if (match('='))         
                { 
                    addToken(TokenType::AssignBitwiseXor); 
                }
                else                    
                { 
                    addToken(TokenType::BitwiseXor); 
                }
                break;
            case '~':                   
                addToken(TokenType::BitwiseNot); break;
            case '!':
                if (match('='))         
                { 
                    addToken(
                        match('=') 
                            ? TokenType::CompareStrictNotEqual 
                            : TokenType::CompareNotEqual
                    ); 
                }
                else                    
                { 
                    addToken(TokenType::LogicalNot); 
                }
                break;
            case '=':
                if (match('='))         
                { 
                    addToken(
                        match('=') 
                            ? TokenType::CompareStrictEqual 
                            : TokenType::CompareEqual
                    ); 
                }
                else if (match('>'))    
                { 
                    addToken(TokenType::Arrow); 
                }
                else                    
                { 
                    addToken(TokenType::AssignEqual); 
                }
                break;
            case '?':
                if (match('.'))         
                { 
                    addToken(TokenType::Chain); 
                }
                else if (match('?'))    
                { 
                    addToken(TokenType::Coalesce); 
                }
                else                    
                { 
                    addToken(TokenType::Question); 
                }
                break;
            case '.':
                if (match('.'))
                {
                    if (match('.'))
                    {
                        addToken(TokenType::Spread);
                        break;
                    }

                    addToken(TokenType::Period);
                }    

                addToken(TokenType::Period);
                break;
            case '(':                   
                addToken(TokenType::OpenParen); 
                break;
            case ')':                   
                addToken(TokenType::CloseParen); 
                break;
            case '[':                   
                addToken(TokenType::OpenBracket); 
                break;
            case ']':                   
                addToken(TokenType::CloseBracket); 
                break;
            case '{':                   
                addToken(TokenType::OpenBrace); 
                break;
            case '}':                   
                addToken(TokenType::CloseBrace); 
                break;
            case ',':                   
                addToken(TokenType::Comma); 
                break;
            case ';':                   
                addToken(TokenType::Semicolon); 
                break;
            case ':':                   
                addToken(TokenType::Colon); 
                break;
            case '"':
            case '\'':
            case '`':                   
                return scanStringLiteral(lChar);
            default:
            {
                if (std::isspace(lChar))                   
                {   
                    return true; 
                }
                else if (lChar == '0' && match('x'))            
                {   
                    return scanHexadecimalLiteral(); 
                }
                else if (lChar == '0' && match('o'))            
                {   
                    return scanOctalLiteral(); 
                }
                else if (lChar == '0' && match('b'))            
                {   
                    return scanBinaryLiteral(); 
                }
                else if (std::isdigit(lChar))                   
                {   
                    return scanNumericLiteral(); 
                }
                else if (lChar == '_' || std::isalpha(lChar))   
                {   
                    return scanIdentifier(); 
                }
                else
                {
                    std::cerr   << std::format("Lex Error: Unexpected character '{}'.", lChar) 
                                << std::endl;
                    return false;
                }
            }
        }

        return true;
    }

    bool Lexer::scanStringLiteral (const char pQuote)
    {
        while (isAtEnd() == false && peek() != pQuote)
        {
            if (peek() == '\n')
            {
                if (pQuote != '`')
                {
                    std::cerr   << "Lex Error: Attempted multiline string in non-template literal."
                                << std::endl;
                    return false;
                }

                mLine++; mColumn = 1;
            }

            advance();
        }

        if (isAtEnd() == true)
        {
            std::cerr << "Lex Error: Unexpected end-of-file while scanning string." << std::endl;
            return false;
        }

        advance();  // Advance past the closing quote.

        // Create a string view into the string.
        std::string_view lSubstr { mCurrentSource->data() + mStart + 1, mCurrent - mStart - 2 };

        // Add the token, trimming the surrounding quotes from its lexeme.
        addToken(
            (pQuote == '`') ? TokenType::TemplateLiteral : TokenType::StringLiteral,
            lSubstr
        );

        return true;
    }

    bool Lexer::scanNumericLiteral ()
    {
        // Scan characters until a non-numeric character is encountered.
        while (std::isdigit(peek()))
        {
            advance();
        }

        // If the non-numeric character encountered is a dot `.`, and the character after that is
        // another number, then a floating-point number is being scanned.
        if (peek() == '.' && std::isdigit(peekNext()))
        {
            advance();  // Past the '.'.
            
            // Now scan characters just the same for the fractional portion.
            while (std::isdigit(peek()))
            {
                advance();
            }
        }

        addToken(TokenType::NumericLiteral);
        return true;
    }

    bool Lexer::scanBinaryLiteral ()
    {
        // Scan characters until one that is not '0' or '1' is encountered.
        while (peek() == '0' || peek() == '1')
        {
            advance();
        }

        addToken(TokenType::BinaryLiteral);
        return true;
    }

    bool Lexer::scanOctalLiteral ()
    {
        // Scan characters until one that is not in the range ['0', '7'] is encountered.
        while (peek() >= '0' && peek() <= '7')
        {
            advance();
        }

        addToken(TokenType::OctalLiteral);
        return true;
    }

    bool Lexer::scanHexadecimalLiteral ()
    {
        // Scan characters until a non-hexadecimal character is encountered.
        while (std::isxdigit(peek()))
        {
            advance();
        }

        addToken(TokenType::HexadecimalLiteral);
        return true;
    }

    bool Lexer::scanIdentifier ()
    {
        // Scan characters unitl a non-alphanumeric, non-underscore character is encountered.
        while (std::isalnum(peek()) || peek() == '_')
        {
            advance();
        }

        // In the case of identifiers, we actually will need to extract the string ahead of time.
        std::string_view lText { mCurrentSource->data() + mStart, mCurrent - mStart };

        // Look up the string in the keyword table.
        auto lIter = Keywords.find(lText);
        if (lIter != Keywords.end())
        {
            addToken(lIter->second);
        }
        else if (lText == "true" || lText == "false")
        {
            addToken(TokenType::BooleanLiteral, lText);
        }
        else
        {
            addToken(TokenType::Identifier, lText);
        }

        return true;
    }

}
