/**
 * @file    JS/TokenType.hpp
 * @brief   Contains the enumeration of token types and type-traits structs indicating literals.
 */

#pragma once
#include <JS/Common.hpp>

namespace js
{

    /**
     * @enum    `js::TokenType`
     * @brief   Enumerates possible types of tokens.
     */
    enum class TokenType
    {
        Unknown = 0,

        // Keywords
        Var,
        Let,
        Const,
        If,
        Else,
        For,
        While,
        Function,
        Return,
        Require,
        Undefined,

        // Literals
        Identifier,
        StringLiteral,
        NumericLiteral,
        BooleanLiteral,
        NullLiteral,

        // Arithmetic Operators
        Plus,
        Minus,
        Times,
        Exponent,
        Divide,
        Modulo,
        Increment,
        Decrement,

        // Bitwise Operators
        BitwiseShiftLeft,
        BitwiseShiftRight,
        BitwiseShiftRightUnsigned,
        BitwiseAnd,
        BitwiseOr,
        BitwiseXor,
        BitwiseNot,

        // Logical Operators
        LogicalAnd,
        LogicalOr,
        LogicalNot,

        // Comparison Operators
        CompareEqual,
        CompareStrictEqual,
        CompareNotEqual,
        CompareStrictNotEqual,
        CompareGreater,
        CompareGreaterEqual,
        CompareLess,
        CompareLessEqual,

        // Assignment Operators
        AssignEqual,
        AssignPlus,
        AssignMinus,
        AssignTimes,
        AssignExponent,
        AssignDivide,
        AssignModulo,
        AssignBitwiseShiftLeft,
        AssignBitwiseShiftRight,
        AssignBitwiseShiftRightUnsigned,
        AssignBitwiseAnd,
        AssignBitwiseOr,
        AssignBitwiseXor,

        // Groupers
        OpenParen,
        CloseParen,
        OpenBracket,
        CloseBracket,
        OpenBrace,
        CloseBrace,

        // Punctuators
        Period,
        Dot = Period,
        Comma,
        Semicolon,
        Colon,

        // End Of File
        EndOfFile

    };

    /**
     * @struct  `js::IsJsonToken`
     * @brief   Type traits struct indicating whether or not the given @a `TokenType` can be used
     *          in a valid JSON object.
     * 
     * @tparam  T   The @a `TokenType` being checked.
     */
    template <TokenType T>
    struct IsJsonToken
    {
        static constexpr bool Value = false;
    };

    template <> struct IsJsonToken<TokenType::StringLiteral>    { static constexpr bool Value = true; };
    template <> struct IsJsonToken<TokenType::NumericLiteral>   { static constexpr bool Value = true; };
    template <> struct IsJsonToken<TokenType::BooleanLiteral>   { static constexpr bool Value = true; };
    template <> struct IsJsonToken<TokenType::NullLiteral>      { static constexpr bool Value = true; };
    template <> struct IsJsonToken<TokenType::OpenBrace>        { static constexpr bool Value = true; };
    template <> struct IsJsonToken<TokenType::CloseBrace>       { static constexpr bool Value = true; };
    template <> struct IsJsonToken<TokenType::OpenBracket>      { static constexpr bool Value = true; };
    template <> struct IsJsonToken<TokenType::CloseBracket>     { static constexpr bool Value = true; };
    template <> struct IsJsonToken<TokenType::Comma>            { static constexpr bool Value = true; };
    template <> struct IsJsonToken<TokenType::Colon>            { static constexpr bool Value = true; };

    using StringType    = std::string;
    using NumberType    = double;
    using BooleanType   = bool;
    using NullType      = std::nullptr_t;

    /**
     * @struct  `js::IsLiteralToken`
     * @brief   Type traits struct indicating whether or not the given @a `TokenType` can be
     *          resolved to a literal.
     * 
     * @tparam  T   The @a `TokenType` being checked.
     */
    template <TokenType T>
    struct IsLiteralToken 
    { 
        using Type = void; 
        static constexpr bool Value = false; 
    };

    template <> struct IsLiteralToken<TokenType::Identifier>        { using Type = std::string; static constexpr bool Value = true; };
    template <> struct IsLiteralToken<TokenType::StringLiteral>     { using Type = std::string; static constexpr bool Value = true; }; 
    template <> struct IsLiteralToken<TokenType::NumericLiteral>    { using Type = double; static constexpr bool Value = true; }; 
    template <> struct IsLiteralToken<TokenType::BooleanLiteral>    { using Type = bool; static constexpr bool Value = true; }; 
    template <> struct IsLiteralToken<TokenType::NullLiteral>       { using Type = std::nullptr_t; static constexpr bool Value = true; }; 

    /**
     * @typedef `js::Literal`
     * @brief   Defines a variant type which can hold any literal-eligible type.
     */
    using Literal = std::variant<
        typename IsLiteralToken<TokenType::StringLiteral>::Type,
        typename IsLiteralToken<TokenType::NumericLiteral>::Type,
        typename IsLiteralToken<TokenType::BooleanLiteral>::Type,
        typename IsLiteralToken<TokenType::NullLiteral>::Type
    >;

}
