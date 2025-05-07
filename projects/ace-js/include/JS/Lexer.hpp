/**
 * @file    JS/Lexer.hpp
 * @brief   Contains a class used to extract tokens from source files.
 */

#pragma once
#include <JS/Token.hpp>

namespace js
{

    /**
     * @class   `js::Lexer`
     * @brief   Provides an interface for extracting tokens from source files.
     */
    class JS_API Lexer final
    {
    public:

        /**
         * @fn      `js::Lexer::Lexer`
         * @brief   Default constructor. 
         */    
        explicit Lexer ();

    public:

        /**
         * @fn      `js::Lexer::tokenizeFile`
         * @brief   Extracts tokens from a source file at the given path.
         * 
         * @param   pPath       The path to the source file to be lexed.
         * 
         * @return  `true` if the file is loaded successfully, and its contents lexed with no
         *          errors; `false` otherwise.
         */
        bool tokenizeFile (const fs::path& pPath);

        /**
         * @fn      `js::Lexer::tokenizeString`
         * @brief   Extracts tokens from the given string.
         * 
         * @param   pSource     A string containing the source code to be lexed.
         * 
         * @return  `true` if the string is lexed with no errors; `false` otherwise.
         */
        bool tokenizeString (const std::string& pSource);

        /**
         * @fn      `js::Lexer::getTokens`
         * @brief   Retrieves the list of extracted tokens.
         * 
         * @return  The list of extracted tokens.
         */
        const std::vector<Token>& getTokens () const;

    private:

        /**
         * @fn      `js::Lexer::tokenize`
         * @brief   Lexes the source code in @a `mSource`.
         * 
         * @return  `true` if the code is lexed with no errors; `false` otherwise.
         */
        bool tokenize ();

        /**
         * @fn      `js::Lexer::isAtEnd`
         * @brief   Checks to see if the character cursor is at or beyond the end of the source code
         *          string being tokenized.
         * 
         * @param   `true` if the cursor is at the end of the source code string; `false` if not.
         */
        bool isAtEnd () const;

        /**
         * @fn      `js::Lexer::advance`
         * @brief   Retrieves the current character in the source string, then advances the cursor
         *          to the next character.
         * 
         * @return  The character in the source code string at the current index before it was
         *          advanced.
         */
        char advance ();

        /**
         * @fn      `js::Lexer::addToken`
         * @brief   Adds a new token to the tokens list.
         * 
         * @param   pType       The type of token to add.
         */
        void addToken (const TokenType& pType);

        /**
         * @fn      `js::Lexer::addToken`
         * @brief   Adds a new token with the given lexeme to the tokens list.
         * 
         * @param   pType       The type of token to add.
         * @param   pLexeme     The token's lexeme.
         */
        void addToken (const TokenType& pType, std::string_view pLexeme);

        /**
         * @fn      `js::Lexer::match`
         * @brief   Checks to see if the given characters is the next character in the source
         *          code string.
         * 
         * The character cursor will be advanced by one place if this returns `true`.
         * 
         * @param   lChar       The character to check for.
         * 
         * @return  `true` if the next character is the given character; `false` otherwise.
         */
        bool match (const char lChar);

        /**
         * @fn      `js::Lexer::peek`
         * @brief   Retrieves the character at the @a `mCurrent` position in the source code string.
         * 
         * @return  The current character in the source code string, or `\0` if the @a `mCurrent`
         *          position is past the end of the string.
         */
        char peek () const;

        /**
         * @fn      `js::Lexer::peekNext`
         * @brief   Retrieves the character just after the @a `mCurrent` position in the source code 
         *          string.
         * 
         * @return  The next character in the source code string, or `\0` if the @a `mCurrent`
         *          position, plus one, is past the end of the string.
         */
        char peekNext () const;

        /**
         * @fn      `js::Lexer::skipLineComment`
         * @brief   Skips characters in the source string which are part of a line comment.
         * 
         * Line comments in JavaScript begin with a double slash `//`, and continue until the end
         * of the current line.
         */
        void skipLineComment ();

        /**
         * @fn      `js::Lexer::skipBlockComment`
         * @brief   Skips characters in the source string which are part of a block comment.
         * 
         * Block comments in JavaScript begin with a slash-and-star `/ *` and continue until a
         * star-and-slash are encountered `* /`.
         */
        void skipBlockComment ();

        /**
         * @fn      `js::Lexer::scanToken`
         * @brief   Extracts the next token from the source string.
         * 
         * @return  `true` if the token is collected successfully; `false` otherwise.
         */
        bool scanToken ();

        /**
         * @fn      `js::Lexer::scanStringLiteral`
         * @brief   Extracts a string literal token from the source string.
         * 
         * String literals can begin with a single-quote `'`, a double-quote `"` or a backtick
         * quote `` ` ``, but must end with a matching closing quote.
         * 
         * @param   pQuote  The character used as the string's quotes.
         * 
         * @return  `true` if the string literal is collected successfully; `false` otherwise.
         */
        bool scanStringLiteral (const char pQuote);

        /**
         * @fn      `js::Lexer::scanNumericLiteral`
         * @brief   Extracts a numeric literal token from the source string.
         * 
         * Numeric literals can be a 64-bit floating-point number (eg. `12.345678`, `3.14159`) or
         * a base-10 integer (eg. `42`, `54622`).
         * 
         * @return  `true` if the numeric literal is collected successfully; `false` otherwise. 
         */
        bool scanNumericLiteral ();

        /**
         * @fn      `js::Lexer::scanBinaryLiteral`
         * @brief   Extracts a binary literal token from the source string.
         * 
         * Binary literals are base-2 integers (eg. `0b01`, `0b10011011`).
         * 
         * @return  `true` if the binary literal is collected successfully; `false` otherwise. 
         */
        bool scanBinaryLiteral ();

        /**
         * @fn      `js::Lexer::scanOctalLiteral`
         * @brief   Extracts a octal literal token from the source string.
         * 
         * Octal literals are base-8 integers (eg. `0o1234567`, `0o5334`).
         * 
         * @return  `true` if the octal literal is collected successfully; `false` otherwise. 
         */
        bool scanOctalLiteral ();

        /**
         * @fn      `js::Lexer::scanHexadecimalLiteral`
         * @brief   Extracts a hexadecimal literal token from the source string.
         * 
         * Hexadecimal literals are base-16 integers (eg. `0xDEADBEEF`, `0xC0FFEE`).
         * 
         * @return  `true` if the hexadecimal literal is collected successfully; `false` otherwise. 
         */
        bool scanHexadecimalLiteral ();

        /**
         * @fn      `js::Lexer::scanIdentifier`
         * @brief   Extracts an identifier token from the source string.
         * 
         * Identifiers are string tokens used to indicate keywords, variable/constant names,
         * function names, class names, etc.
         * 
         * @return  `true` if the identifier is collected successfully; `false` otherwise.
         */
        bool scanIdentifier ();

    private:

        /**
         * @def     `js::Lexer::mSources`
         * @brief   Contains all of the source code strings which have and are being lexed.
         */
        std::vector<std::string> mSources;

        /**
         * @def     `js::Lexer::mCurrentSource`
         * @brief   A pointer to the source code string currently being lexed.
         */
        std::string* mCurrentSource = nullptr;

        /**
         * @def     `js::Lexer::mStart`
         * @brief   Contains the starting index of the current token in the source code string.
         */
        std::size_t mStart = 0;

        /**
         * @def     `js::Lexer::mCurrent`
         * @brief   Contains the index of the current character being lexed in the source code
         *          string.
         */
        std::size_t mCurrent = 0;

        /**
         * @def     `js::Lexer::mLine`
         * @brief   The current line in the source code string.
         */
        std::size_t mLine = 1;

        /**
         * @def     `js::Lexer::mColumn`
         * @brief   The current column in the current line in the source code string.
         */
        std::size_t mColumn = 1;

        /**
         * @fn      `js::Lexer::mTokens`
         * @brief   Contains the tokens extracted from the source code.
         */
        std::vector<Token> mTokens;

    };

}
