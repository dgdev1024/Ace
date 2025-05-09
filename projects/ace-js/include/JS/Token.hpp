/**
 * @file    JS/Token.hpp
 * @brief   Contains a class representing a single token extracted from a source file.
 */

#pragma once
#include <JS/TokenType.hpp>

namespace js
{

    /**
     * @class   `js::Token`
     * @brief   Contains a single token extracted from a source file.
     */
    class JS_API Token final
    {
    public:
        
        /**
         * @fn      `js::Token::Token`
         * @brief   Constructs a new @a `Token` with the given contents.
         * 
         * @param   pType       The type of the token being constructed.
         * @param   pLexeme     The token's raw contents.
         * @param   pLine       The token's source line.
         * @param   pColumn     The token's source column.
         */
        explicit Token (
            const TokenType pType, 
            std::string_view pLexeme,
            std::size_t pLine   = 0, 
            std::size_t pColumn = 0
        );

        /**
         * @fn      `js::Token::Token`
         * @brief   Copies the given @a `Token` into this one.
         * 
         * @param   pToken      The token to be copied.
         */
        Token (const Token& pToken);

        /**
         * @fn      `js::Token::Token`
         * @brief   Moves the given @a `Token` into this one.
         * 
         * @param   pToken      The token to be moved.
         */
        explicit Token (Token&& pToken);

    public:

        /**
         * @fn      `js::Token::getType`
         * @brief   Retrieves the type of this token.
         * 
         * @return  The type of this token.
         */
        inline const TokenType& getType () const { return mType; }

        /**
         * @fn      `js::Token::getLexeme`
         * @brief   Retrieves the raw contents of this token.
         * 
         * @return  A string with the raw contents of this token.
         */
        inline std::string_view getLexeme () const { return mLexeme; }

        /**
         * @fn      `js::Token::hasLiteral`
         * @brief   Retrieves wheter or not this token's type is a literal type.
         * 
         * @return  `true` if the optional @a `mLiteral` contains a value; `false` otherwise.
         */
        inline bool hasLiteral () const { return mLiteral.has_value(); }

        /**
         * @fn      `js::Token::getLiteral`
         * @brief   If this token's type is a literal, retrieves that literal's value.
         * 
         * @return  The literal's `std::variant` value, if present.
         */
        Literal& getLiteral ();
        const Literal& getLiteral () const;

        /**
         * @fn      `js::Token::getLiteralAs`
         * @brief   If this token's type is a literal, retrieves that literal's value, cast into
         *          type `T`.
         * 
         * @tparam  T   The type to cast the literal's variant value into.
         * 
         * @return  The literal's variant value, cast into type `T`.
         */
        template <typename T>
        inline const T& getLiteralAs () const
        {
            return std::get<T>(getLiteral());
        }

        /**
         * @fn      `js::Token::getLine`
         * @brief   Retrieves the line in the source file where this token was encountered.
         * 
         * @return  The token's source line.
         */
        inline const std::size_t& getLine () const { return mLine; }

        /**
         * @fn      `js::Token::getColumn`
         * @brief   Retrieves the column of the line in the source file where this token was
         *          encountered.
         * 
         * @return  The token's source column.
         */
        inline const std::size_t& getColumn () const { return mColumn; }

        /**
         * @fn      `js::Token::toString`
         * @brief   Retrieves a string representation of this token's type and value.
         * 
         * @return  The string representation of this token.
         */
        std::string toString () const;

    private:

        /**
         * @def     `js::Token::mType`
         * @brief   Indicates the type of the token.
         */
        TokenType mType = TokenType::Unknown;

        /**
         * @def     `js::Token::mLexeme`
         * @brief   Contains the raw contents of the token.
         */
        std::string_view mLexeme;

        /**
         * @def     `js::Token::mLiteral`
         * @brief   If @a `mType` satisfies @a `IsLiteralToken`, this will contain the literal's
         *          value.
         * 
         * @sa      @a `js::IsLiteralToken`
         */
        std::optional<Literal> mLiteral = std::nullopt;

        /**
         * @def     `js::Token::mLine`
         * @brief   The line of the source file where this token was encountered.
         */
        std::size_t mLine = 0;

        /**
         * @def     `js::Token::mColumn`
         * @brief   The column of the line in the source file where this token was encountered.
         */
        std::size_t mColumn = 0;

    };

}
