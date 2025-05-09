/**
 * @file    JS/AST.cpp
 */

#include <JS/Precompiled.hpp>
#include <JS/AST.hpp>

namespace js
{

    /* `LiteralExpression` Methods ****************************************************************/

    LiteralExpression::LiteralExpression (Literal& pLiteral) :
        mLiteral { std::move(pLiteral) }
    {

    }
    
    std::string LiteralExpression::toString () const
    {
        std::ostringstream lStream;
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
                    lStream << std::quoted(pValue);
                }
            },
            mLiteral
        );

        return lStream.str();
    }

    /* `VariableAccessExpression` Methods *********************************************************/

    VariableAccessExpression::VariableAccessExpression (Token& pName) :
        mName { std::move(pName) }
    {

    }

    std::string VariableAccessExpression::toString () const
    {
        return std::string { mName.getLexeme() };
    }

    /* `VariableAssignExpression` Methods *********************************************************/

    VariableAssignExpression::VariableAssignExpression (Token& pName, const Expression::Ptr& pValue) :
        mName   { std::move(pName) },
        mValue  { pValue }
    {
        if (mValue == nullptr)
        {
            throw std::invalid_argument { "Value expression cannot be null!" };
        }
    }

    std::string VariableAssignExpression::toString () const
    {
        return std::format("{} = {}", mName.getLexeme(), mValue->toString());
    }

    /* `BinaryExpression` Methods *****************************************************************/

    BinaryExpression::BinaryExpression (const Expression::Ptr& pLeft, const Expression::Ptr& pRight,
        Token& pOperator) :
        mLeft       { pLeft },
        mRight      { pRight },
        mOperator   { std::move(pOperator) }
    {
        if (mLeft == nullptr || mRight == nullptr)
        {
            throw std::invalid_argument { "Binary operand expressions cannot be null!" };
        }
    }

    std::string BinaryExpression::toString () const
    {
        return std::format("{} {} {}", mLeft->toString(), mOperator.getLexeme(), mRight->toString());
    }

    /* `UnaryExpression` Methods ******************************************************************/

    UnaryExpression::UnaryExpression (const Expression::Ptr& pOperand, Token& pOperator,
        bool pIsPrefix) :
        mOperand    { pOperand },
        mOperator   { std::move(pOperator) },
        mIsPrefix   { pIsPrefix }
    {
        if (mOperand == nullptr)
        {
            throw std::invalid_argument { "Unary operand expression cannot be null!" };
        }
    }
    
    std::string UnaryExpression::toString () const
    {
        if (mIsPrefix == true)
        {
            return std::format("{}{}", mOperator.getLexeme(), mOperand->toString());
        }
        else
        {
            return std::format("{}{}", mOperand->toString(), mOperator.getLexeme());
        }
    }

    /* `LogicalExpression` Methods ****************************************************************/

    LogicalExpression::LogicalExpression (const Expression::Ptr& pLeft, const Expression::Ptr& pRight,
        Token& pOperator) :
        BinaryExpression { pLeft, pRight, pOperator }
    {
        if (mLeft == nullptr || mRight == nullptr)
        {
            throw std::invalid_argument { "Logical operand expressions cannot be null!" };
        }
    }

    /* `CallExpression` Methods *******************************************************************/

    CallExpression::CallExpression (const Expression::Ptr& pCallee, Token& pCloseParen,
        std::vector<Expression::Ptr>& pArguments) :
        mCallee         { pCallee },
        mCloseParen     { std::move(pCloseParen) },
        mArguments      { std::move(pArguments) }
    {
        if (mCallee == nullptr)
        {
            throw std::invalid_argument { "Function callee expression cannot be null!" };
        }
    }

    std::string CallExpression::toString () const
    {
        std::ostringstream lStream;
        lStream << '(';
        for (std::size_t lIndex = 0; lIndex < mArguments.size(); ++lIndex)
        {
            lStream << mArguments[lIndex]->toString();
            if (lIndex + 1 < mArguments.size())
            {
                lStream << ", ";
            }
        }
        lStream << ')';

        return std::format("{}{}", mCallee->toString(), lStream.str());
    }

    /* `ArrayExpression` Methods ******************************************************************/

    ArrayExpression::ArrayExpression (std::vector<Expression::Ptr>& pElements) :
        mElements { std::move(pElements) }
    {

    }

    std::string ArrayExpression::toString () const
    {
        std::ostringstream lStream;
        lStream << "[ ";
        for (std::size_t lIndex = 0; lIndex < mElements.size(); ++lIndex)
        {
            lStream << mElements[lIndex]->toString();
            if (lIndex + 1 < mElements.size())
            {
                lStream << ", ";
            }
        }
        lStream << " ]";
        return lStream.str();
    }

    /* `ObjectExpression` Methods *****************************************************************/

    ObjectExpression::ObjectExpression (std::vector<Property>& pProperties) :
        mProperties { std::move(pProperties) }
    {

    }

    std::string ObjectExpression::toString () const
    {   
        std::ostringstream lStream;
        lStream << "{ ";
        for (std::size_t lIndex = 0; lIndex < mProperties.size(); ++lIndex)
        {
            const auto& [lKey, lValue] = mProperties[lIndex];
            lStream << std::format("{}: {}", lKey.getLexeme(), lValue->toString());
            if (lIndex + 1 < mProperties.size())
            {
                lStream << ", ";
            }
        }
        lStream << " }";
        return lStream.str();
    }

    /* `AccessExpression` Methods *****************************************************************/

    AccessExpression::AccessExpression (const Expression::Ptr& pObject, Token& pName) :
        mObject     { pObject },
        mName       { std::move(pName) }
    {
        if (mObject == nullptr)
        {
            throw std::invalid_argument { "Access expression object cannot be null!" };
        }
    }

    std::string AccessExpression::toString () const
    {
        return std::format("{}.{}", mObject->toString(), mName.getLexeme());
    }

    /* `IndexExpression` Methods ******************************************************************/

    IndexExpression::IndexExpression (const Expression::Ptr& pObject, const Expression::Ptr& pIndex) :
        mObject     { pObject },
        mIndex      { pIndex }
    {
        if (mObject == nullptr)
        {
            throw std::invalid_argument { "Index expression object cannot be null!" };
        }
        else if (mIndex == nullptr)
        {
            throw std::invalid_argument { "Index expression index cannot be null!" };
        }
    }

    std::string IndexExpression::toString () const
    {
        return std::format("{}[{}]", mObject->toString(), mIndex->toString());
    }

    /* `ExpressionStatement` Methods **************************************************************/

    ExpressionStatement::ExpressionStatement (const Expression::Ptr& pExpression) :
        mExpression { pExpression }
    {
        if (mExpression == nullptr)
        {
            throw std::invalid_argument { "Function callee expression cannot be null!" };
        }
    }

    std::string ExpressionStatement::toString () const
    {
        std::ostringstream lStream;
        lStream << mExpression->toString() << "; ";
        return lStream.str();
    }

    /* `VariableDeclarationStatement` Methods *****************************************************/

    VariableDeclarationStatement::VariableDeclarationStatement (Token& pName, 
        const TokenType& pType, const Expression::Ptr& pValue) :
        mName   { std::move(pName) },
        mType   { pType },
        mValue  { pValue }
    {

    }

    std::string VariableDeclarationStatement::toString () const
    {
        std::ostringstream lStream;
        switch (mType)
        {
            case TokenType::Var:    lStream << "var " << mName.getLexeme(); break;
            case TokenType::Let:    lStream << "let " << mName.getLexeme(); break;
            case TokenType::Const:  lStream << "const " << mName.getLexeme(); break;
            default:                return "???";
        }

        if (mValue != nullptr)
        {
            lStream << " = " << mValue->toString();
        }

        lStream << "; ";
        return lStream.str();
    }

    /* `BlockStatement` Methods *******************************************************************/

    BlockStatement::BlockStatement (std::vector<Statement::Ptr>& pStatements) :
        mStatements { std::move(pStatements) }
    {

    }

    std::string BlockStatement::toString () const
    {
        std::ostringstream lStream;
        lStream << "{ ";
        for (const auto& lStatement : mStatements)
        {
            lStream << lStatement->toString();
        }
        lStream << "}";
        return lStream.str();
    }

    /* `IfStatement` Methods **********************************************************************/

    IfStatement::IfStatement (const Expression::Ptr& pCondition, const Statement::Ptr& pThen,
        const Statement::Ptr& pElse) :
        mCondition  { pCondition },
        mThen       { pThen },
        mElse       { pElse }
    {
        if (mCondition == nullptr)
        {
            throw std::invalid_argument { "If statement condition expression cannot be null!" };
        }
        else if (mThen == nullptr)
        {
            throw std::invalid_argument { "If statement 'then' branch cannot be null!" };
        }
    }

    std::string IfStatement::toString () const
    {
        std::ostringstream lStream;
        lStream << std::format("if ({}) {} ", mCondition->toString(), mThen->toString());

        if (mElse != nullptr)
        {
            lStream << std::format("else {} ", mElse->toString());
        }

        return lStream.str();
    }

    /* `WhileStatement` Methods *******************************************************************/

    WhileStatement::WhileStatement (const Expression::Ptr& pCondition, const Statement::Ptr& pBody) :
        mCondition  { pCondition },
        mBody       { pBody }
    {
        if (mCondition == nullptr)
        {
            throw std::invalid_argument { "While statement condition expression cannot be null!" };
        }
        else if (mBody == nullptr)
        {
            throw std::invalid_argument { "While statement branch cannot be null!" };
        }
    }

    std::string WhileStatement::toString () const
    {
        return std::format("while ({}) {} ", mCondition->toString(), mBody->toString());
    }

    /* `ForStatement` Methods *********************************************************************/

    ForStatement::ForStatement (const Statement::Ptr& pInitializer, const Expression::Ptr& pCondition, 
        const Expression::Ptr& pIncrement, const Statement::Ptr& pBody) :
        mInitializer    { pInitializer },
        mCondition      { pCondition },
        mIncrement      { pIncrement },
        mBody           { pBody }
    {
        if (mBody == nullptr)
        {
            throw std::invalid_argument { "For statement branch cannot be null!" };
        }
    }

    std::string ForStatement::toString () const
    {
        return std::format("for ({}; {}; {}) {} ",
            (mInitializer != nullptr) ? mInitializer->toString() : "",
            (mCondition != nullptr) ? mCondition->toString() : "",
            (mIncrement != nullptr) ? mIncrement->toString() : "",
            mBody->toString()
        );
    }

    /* `FunctionDeclarationStatement` Methods *****************************************************/

    FunctionDeclarationStatement::FunctionDeclarationStatement (Token& pName, 
        std::vector<Token>& pParamTokens, const BlockStatement::Ptr& pBody) :
        mName           { std::move(pName) },
        mParamTokens    { std::move(pParamTokens) },
        mBody           { pBody }
    {
        if (pBody == nullptr)
        {
            throw std::invalid_argument { "Function declaration statement body cannot be null!" };
        }
    }

    std::string FunctionDeclarationStatement::toString () const
    {
        std::ostringstream lStream;

        lStream << std::format("function {} (", mName.getLexeme());
        for (std::size_t lIndex = 0; lIndex < mParamTokens.size(); ++lIndex)
        {
            lStream << mParamTokens[lIndex].getLexeme();
            if (lIndex + 1 < mParamTokens.size())
            {
                lStream << ", ";
            }
        }
        lStream << std::format(") {} ", mBody->toString());

        return lStream.str();
    }

    /* `FunctionDeclarationExpression` Methods ****************************************************/

    FunctionDeclarationExpression::FunctionDeclarationExpression (std::optional<Token>& pName, 
        std::vector<Token>& pParamTokens, const BlockStatement::Ptr& pBody) :
        mName           { std::move(pName) },
        mParamTokens    { std::move(pParamTokens) },
        mBody           { pBody }
    {
        if (pBody == nullptr)
        {
            throw std::invalid_argument { "Function declaration expression body cannot be null!" };
        }
    }

    std::string FunctionDeclarationExpression::toString () const
    {
        std::ostringstream lStream;

        lStream << "function (";
        if (mName.has_value() == true)
        {
            lStream << mName->getLexeme() << ' ';
        }

        for (std::size_t lIndex = 0; lIndex < mParamTokens.size(); ++lIndex)
        {
            lStream << mParamTokens[lIndex].getLexeme();
            if (lIndex + 1 < mParamTokens.size())
            {
                lStream << ", ";
            }
        }
        lStream << std::format(") {}", mBody->toString());

        return lStream.str();
    }

    /* `ArrowFunctionExpression` Methods **********************************************************/

    ArrowFunctionExpression::ArrowFunctionExpression (std::vector<Token>& pParamTokens,
        std::variant<Expression::Ptr, BlockStatement::Ptr>& pBody) :
        mParamTokens    { std::move(pParamTokens) },
        mBody           { std::move(pBody) }
    {

    }

    std::string ArrowFunctionExpression::toString () const
    {
        std::ostringstream lStream;
        lStream << '(';
        for (std::size_t lIndex = 0; lIndex < mParamTokens.size(); ++lIndex)
        {
            lStream << mParamTokens[lIndex].getLexeme();
            if (lIndex + 1 < mParamTokens.size())
            {
                lStream << ", ";
            }
        }
        lStream << ") => ";

        if (std::holds_alternative<Expression::Ptr>(mBody))
        {
            lStream << std::get<Expression::Ptr>(mBody)->toString();
        }
        else
        {
            lStream << std::get<BlockStatement::Ptr>(mBody)->toString();
        }

        return lStream.str();
    }

    /* `ReturnStatement` Methods ******************************************************************/

    ReturnStatement::ReturnStatement (Token& pKeyword, const Expression::Ptr& pValue) :
        mKeyword    { std::move(pKeyword) },
        mValue      { pValue }
    {

    }

    std::string ReturnStatement::toString () const
    {
        if (mValue == nullptr)
        {
            return "return; ";
        }

        return std::format("return {}; ", mValue->toString());
    }

}
