/**
 * @file    JS/Parser.hpp
 * @brief   Contains a class for parsing tokens to produce an abstract syntax tree.
 */

#pragma once
#include <JS/Token.hpp>
#include <JS/AST.hpp>

namespace js
{

    /**
     * @typedef `js::Program`
     * @brief   Defines a list of statements making up a parsed program.
     */
    using Program = std::vector<Statement::Ptr>;

    /**
     * @class   `js::Parser`
     * @brief   Provides a interface for parsing tokens extracted from the lexer in order to
     *          produce an abstract syntax tree ("AST").
     */
    class JS_API Parser final
    {
    public:

        /**
         * @fn      `js::Parser::Parser`
         * @brief   Constructs a @a `Parser` by moving the given list of tokens into it.
         * 
         * The list of tokens should be provided by a @a `Lexer`, via its @a `getTokens` method.
         * 
         * @param   pTokens The list of tokens to be moved in.
         */
        explicit Parser (std::vector<Token>& pTokens);

    public:

        /**
         * @fn      `js::Parser::parse`
         * @brief   Parses the tokens stored in this parser to produce an AST.
         * 
         * @param   pProgram    If successful, will contain the parsed program.
         * 
         * @return  `true` if the tokens are parsed successfully; `false` otherwise.
         */
        bool parse (Program& pProgram);

    private:
    
        /**
         * @fn      `js::Parser::isAtEnd`
         * @brief   Checks to see if @a `mPointer` is at or beyond the end of the @a `mTokens`
         *          list.
         * 
         * The pointer is said to be at the end of the tokens list if it is greater than or equal
         * to the list's size, or if the token being pointed to is the @a `TokenType::EndOfFile`
         * token.
         * 
         * @return  `true` if the pointer is at the end of the tokens list; `false` otherwise.
         */
        bool isAtEnd () const;

        /**
         * @fn      `js::Parser::peek`
         * @brief   Retrieves the @a `Token` in the @a `mTokens` list being pointed to by
         *          @a `mPointer`.
         * 
         * If the pointer is at the end of the tokens list, then the last token in the list will be
         * returned instead.
         * 
         * @return  The token being pointed to, or the last token if the pointer is out of bounds.
         */
        const Token& peek () const;

        /**
         * @fn      `js::Parser::advance`
         * @brief   Retrieves the @a `Token` in the @a `mTokens` list being pointed to by
         *          @a `mPointer`, then advances the pointer by one place if in bounds.
         * 
         * If the pointer is at the end of the tokens list, then the last token in the list will be
         * returned instead and the pointer will not be advanced.
         * 
         * @return  The token being pointed to, or the last token if the pointer is out of bounds.
         */
        const Token& advance ();

        /**
         * @fn      `js::Parser::previous`
         * @brief   Retrieves the @a `Token` in the @a `mTokens` list being pointed to by
         *          @a `mPointer` minus one.
         * 
         * If the pointer is at the start of the tokens list, then the first token in the list will
         * be returned instead. If the pointer minus one is at or beyond the end of the tokens list,
         * then the last token in the list will be returned instead.
         * 
         * @return  The token being pointed to if in bounds, the first token if the pointer is zero,
         *          or the last token if the pointer minus one is out of bounds.
         */
        Token& previous ();
        const Token& previous () const;

        /**
         * @fn      `js::Parser::check`
         * @brief   Checks to see if the @a `Token` returned by @a `peek` is of the given
         *          @a `TokenType`.
         * 
         * @param   pType   The type of token to check for.
         * 
         * @return  `true` if the peeked token is of the given type, `false` otherwise.
         */
        bool check (const TokenType& pType);

        /**
         * @fn      `js::Parser::match`
         * @brief   Checks to see if the @a `Token` in the @a `mTokens` list pointed to by
         *          @a `mPointer` is one of a given list of @a `TokenType`s.
         * 
         * If the token does match one of the given types, then the pointer will be advanced by
         * one place.
         * 
         * @param   pTypes  An initializer list containing the token types to check for.
         * 
         * @return  `true` if the token being pointed to is of one of the given types; `false`
         *          otherwise.
         */
        bool match (const std::initializer_list<TokenType>& pTypes);

        /**
         * @fn      `js::Parser::expect`
         * @brief   Expects the @a `Token` in the @a `mTokens` list pointed to by @a `mPointer` to
         *          be of the given @a `TokenType`, retrieving it and advancing the pointer if so.
         * 
         * @param   pType       The token type to check for.
         * @param   pMessage    An error message to print if this check fails.
         * 
         * @return  The token being pointed to if it is of the given type.
         */
        Token& expect (const TokenType& pType, const std::string& pMessage);

        /**
         * @fn      `js::Parser::expect`
         * @brief   Expects the @a `Token` in the @a `mTokens` list pointed to by @a `mPointer` to
         *          be one of the given @a `TokenType`s, retrieving it and advancing the pointer if so.
         * 
         * @param   pTypes      The token types to check for.
         * @param   pMessage    An error message to print if this check fails.
         * 
         * @return  The token being pointed to if it is of one of the given types.
         */
        Token& expect (const std::initializer_list<TokenType>& pTypes, const std::string& pMessage);

    private:

        // Declarations
        Statement::Ptr  parseDeclaration ();
        Statement::Ptr  parseFunctionDeclaration ();
        Statement::Ptr  parseVariableDeclaration ();

        // Statements
        Statement::Ptr  parseStatement ();
        Statement::Ptr  parseIfStatement ();
        Statement::Ptr  parseWhileStatement ();
        Statement::Ptr  parseForStatement ();
        Statement::Ptr  parseReturnStatement ();
        Statement::Ptr  parseBlock ();
        Statement::Ptr  parseExpressionStatement ();

        // Expressions
        Expression::Ptr parseExpression ();
        Expression::Ptr parseAssignment ();
        Expression::Ptr parseLogicalOr ();
        Expression::Ptr parseLogicalAnd ();
        Expression::Ptr parseEquality ();
        Expression::Ptr parseComparison ();
        Expression::Ptr parseAdditive ();
        Expression::Ptr parseMultiplicative ();
        Expression::Ptr parseUnary ();
        Expression::Ptr parseCall ();
        Expression::Ptr parseFunctionExpression ();
        Expression::Ptr parseArrowFunctionExpression ();
        Expression::Ptr parseArray ();
        Expression::Ptr parseObject ();
        Expression::Ptr parsePrimary ();

        // Helpers
        Expression::Ptr finishCall (const Expression::Ptr& pCallee);

    private:

        /**
         * @def     `js::Parser::mTokens`
         * @brief   The list of tokens being parsed.
         */
        std::vector<Token> mTokens;    

        /**
         * @def     `js::Parser::mPointer`
         * @brief   Points to a token in the @a `mTokens` list.
         */
        std::size_t mPointer = 0;

    };

}
