/**
 * @file    JS/Token.cpp
 */

#include <JS/Precompiled.hpp>
#include <JS/Token.hpp>

namespace js
{

    /* Constructors and Destructor ****************************************************************/

    Token::Token (
        const TokenType pType, 
        const std::string& pLexeme,
        std::size_t pLine,
        std::size_t pColumn
    ) :
        mType       { pType },
        mLexeme     { pLexeme },
        mLine       { pLine },
        mColumn     { pColumn }
    {
        switch (pType)
        {
            case TokenType::Identifier:
            case TokenType::StringLiteral:
                mLiteral.emplace(std::in_place_type<StringType>, pLexeme);
                break;
            case TokenType::NumericLiteral:
                mLiteral.emplace(std::in_place_type<NumberType>, std::stod(pLexeme));
                break;
            case TokenType::BooleanLiteral:
                mLiteral.emplace(std::in_place_type<BooleanType>, (pLexeme == "true"));
                break;
            case TokenType::NullLiteral:
                mLiteral.emplace(std::in_place_type<NullType>, nullptr);
            default: break;
        }
    }

    /* Public Functions ***************************************************************************/

    const Literal& Token::getLiteral () const
    {
        if (mLiteral.has_value() == false)
        {
            throw std::runtime_error { "Attempt to retrieve literal from non-literal token!" };
        }

        return *mLiteral;
    }

    std::string Token::toString () const
    {
        std::ostringstream lStream;

        // Output the token's type.
        lStream << std::format("[{}] '{}'", static_cast<int>(mType), mLexeme);

        // If the token is a literal, then visit the literal and print its value.
        if (mLiteral.has_value() == true)
        {
            lStream << " = ";
            std::visit(
                [&lStream] (auto&& pValue)
                {
                    using DecayedType = std::decay_t<decltype(pValue)>;

                    if constexpr (std::is_same_v<DecayedType, BooleanType>)
                    {
                        lStream << std::boolalpha << pValue;
                    }
                    else if constexpr (std::is_same_v<DecayedType, NumberType>)
                    {
                        lStream << std::dec << pValue;
                    }
                    else if constexpr (std::is_same_v<DecayedType, NullType>)
                    {
                        lStream << "null";
                    }
                    else
                    {
                        lStream << pValue;
                    }
                },
                *mLiteral
            );
        }

        // Output the token's source line and column.
        lStream << std::format(" ({}:{})", mLine, mColumn);

        return lStream.str();
    }

}
