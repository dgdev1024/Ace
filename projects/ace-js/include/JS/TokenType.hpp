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
        Abstract,
        Arguments,
        Async,
        Await,
        Break,
        Case,
        Catch,
        Class,
        Const,
        Continue,
        Default,
        Delete,
        Do,
        Else,
        Enum,
        Export,
        Extends,
        Final,
        Finally,
        For,
        Function,
        Goto,
        If,
        Implements,
        Import,
        In,
        InstanceOf,
        Interface,
        Let,
        Native,
        New,
        Package,
        Private,
        Protected,
        Public,
        Return,
        Static,
        Super,
        Switch,
        Synchonized,
        This,
        Throw,
        Throws,
        Transient,
        Try,
        TypeOf,
        Undefined,
        Var,
        Volatile,
        While,
        With,
        Yield,

        // Literals
        Identifier,
        StringLiteral,
        TemplateLiteral,
        NumericLiteral,
        BinaryLiteral,
        OctalLiteral,
        HexadecimalLiteral,
        BooleanLiteral,
        NullLiteral,

        // Arithmetic Operators
        Plus,
        Minus,
        Hyphen = Minus,
        Dash = Minus,
        Times,
        Asterisk = Times,
        Exponent,
        Divide,
        Slash = Divide,
        Modulo,
        Percent = Modulo,
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

        // Accessing Operators
        Chain,
        Coalesce,
        Spread,

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
        Question,
        Comma,
        Semicolon,
        Colon,
        Arrow,

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

    template <> struct IsLiteralToken<TokenType::Identifier>            { using Type = StringType; static constexpr bool Value = true; };
    template <> struct IsLiteralToken<TokenType::StringLiteral>         { using Type = StringType; static constexpr bool Value = true; }; 
    template <> struct IsLiteralToken<TokenType::TemplateLiteral>       { using Type = StringType; static constexpr bool Value = true; }; 
    template <> struct IsLiteralToken<TokenType::NumericLiteral>        { using Type = NumberType; static constexpr bool Value = true; }; 
    template <> struct IsLiteralToken<TokenType::BinaryLiteral>         { using Type = NumberType; static constexpr bool Value = true; }; 
    template <> struct IsLiteralToken<TokenType::OctalLiteral>          { using Type = NumberType; static constexpr bool Value = true; }; 
    template <> struct IsLiteralToken<TokenType::HexadecimalLiteral>    { using Type = NumberType; static constexpr bool Value = true; }; 
    template <> struct IsLiteralToken<TokenType::BooleanLiteral>        { using Type = BooleanType; static constexpr bool Value = true; }; 
    template <> struct IsLiteralToken<TokenType::NullLiteral>           { using Type = NullType; static constexpr bool Value = true; }; 

    /**
     * @typedef `js::Literal`
     * @brief   Defines a variant type which can hold any literal-eligible type.
     */
    using Literal = std::variant<StringType, NumberType, BooleanType, NullType>;

}
