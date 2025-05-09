/**
 * @file    JS/Parser.cpp
 */

#include <JS/Precompiled.hpp>
#include <JS/Parser.hpp>

namespace js
{

    /* Constructors and Destructors ***************************************************************/

    Parser::Parser (std::vector<Token>& pTokens) :
        mTokens { std::move(pTokens) }
    {
        if (mTokens.empty())
        {
            mTokens.emplace_back(TokenType::EndOfFile, "");
        }
    }

    /* Public Methods *****************************************************************************/

    bool Parser::parse (Program& pProgram)
    {
        try
        {
            Program lProgram;
            while (isAtEnd() == false)
            {
                lProgram.push_back(parseDeclaration());
            }

            std::swap(pProgram, lProgram);
            return true;
        }
        catch (std::exception& lEx)
        {
            std::cerr << lEx.what() << std::endl;
            return false;
        }
    }

    /* Private Methods ****************************************************************************/

    bool Parser::isAtEnd () const
    {
        return
            mPointer >= mTokens.size() ||
            mTokens[mPointer].getType() == TokenType::EndOfFile;
    }

    const Token& Parser::peek () const
    {
        return (isAtEnd()) ? mTokens.back() : mTokens[mPointer];
    }

    const Token& Parser::advance ()
    {
        return (isAtEnd()) ? mTokens.back() : mTokens[mPointer++];
    }

    Token& Parser::previous ()
    {
        if (mPointer == 0)
        {
            return mTokens.front();
        }
        else if (mPointer - 1 >= mTokens.size())
        {
            return mTokens.back();
        }
        else
        {
            return mTokens[mPointer - 1];
        }
    }

    const Token& Parser::previous () const
    {
        if (mPointer == 0)
        {
            return mTokens.front();
        }
        else if (mPointer - 1 >= mTokens.size())
        {
            return mTokens.back();
        }
        else
        {
            return mTokens[mPointer - 1];
        }
    }

    bool Parser::check (const TokenType& pType)
    {
        return peek().getType() == pType;
    }

    bool Parser::match (const std::initializer_list<TokenType>& pTypes)
    {
        for (const auto& lType : pTypes)
        {
            if (check(lType))
            {
                advance();
                return true;
            }
        }

        return false;
    }

    Token& Parser::expect (const TokenType& pType, const std::string& pMessage)
    {
        if (check(pType))
        {
            return (isAtEnd()) ? mTokens.back() : mTokens[mPointer++];
        }

        throw std::runtime_error {
            std::format("Parse Error: {}\n  At Token ({}).", pMessage, peek().toString())
        };
    }

    Token& Parser::expect (const std::initializer_list<TokenType>& pTypes, const std::string& pMessage)
    {
        for (const auto& lType : pTypes)
        {
            if (check(lType))
            {
                return (isAtEnd()) ? mTokens.back() : mTokens[mPointer++];
            }
        }

        throw std::runtime_error {
            std::format("Parse Error: {}\n  At Token ({}).", pMessage, peek().toString())
        };
    }

    /* Private Methods - Parsing Declarations *****************************************************/

    Statement::Ptr Parser::parseDeclaration ()
    {
        if (match({ TokenType::Function }))
        {
            return parseFunctionDeclaration();
        }
        else if (match({ TokenType::Var, TokenType::Let, TokenType::Const }))
        {
            return parseVariableDeclaration();
        }

        return parseStatement();
    }

    Statement::Ptr Parser::parseFunctionDeclaration ()
    {
        
        // Consume the function's name.
        auto& lName = expect(TokenType::Identifier, "Expected a function name");
        
        // If the function has any parameters, then store them here.
        std::vector<Token> lParameters;
        expect(TokenType::OpenParen, "Expected '(' after function name");
        if (check(TokenType::CloseParen) == false)
        {
            do
            {
                auto& lParam = expect(TokenType::Identifier, "Expected a parameter name");
                lParameters.push_back(std::move(lParam));
            } while (match({ TokenType::Comma }));
        }
        expect(TokenType::CloseParen, "Expected a ')' before function body");
        expect(TokenType::OpenBrace, "Expected a '{' before function body");

        // Parse the function's body.
        Statement::Ptr lBody = parseBlock();

        return std::make_shared<FunctionDeclarationStatement>(
            lName,
            lParameters,
            std::static_pointer_cast<BlockStatement>(lBody)
        );

    }

    Statement::Ptr Parser::parseVariableDeclaration ()
    {

        // The kind of variable being declared is in the previous token.
        TokenType lKind = previous().getType();

        // Expect the variable's name.
        auto& lName = expect(TokenType::Identifier, "Expected a variable name");

        // Check for the presence of an initializer. If it's there, parse it.
        Expression::Ptr lValue = nullptr;
        if (match({ TokenType::AssignEqual }))
        {
            lValue = parseExpression();
        }

        // This statement requires a semicolon at the end.
        expect(TokenType::Semicolon, "Expected ';' after variable declaration");

        return std::make_shared<VariableDeclarationStatement>(
            lName,
            lKind,
            lValue
        );

    }

    /* Private Methods - Parsing Statements *******************************************************/

    Statement::Ptr Parser::parseStatement ()
    {
        if      (match({ TokenType::If }))          { return parseIfStatement(); }
        else if (match({ TokenType::For }))         { return parseForStatement(); }
        else if (match({ TokenType::While }))       { return parseWhileStatement(); }
        else if (match({ TokenType::Return }))      { return parseReturnStatement(); }
        else if (match({ TokenType::OpenBrace }))   { return parseBlock(); }
        else                                        { return parseExpressionStatement(); }
    }

    Statement::Ptr Parser::parseIfStatement ()
    {

        // The `if` statement's condition is wrapped in parentheses.
        expect(TokenType::OpenParen, "Expected a '(' before condition in 'if' statement");
        Expression::Ptr lCondition = parseExpression();
        expect(TokenType::CloseParen, "Expected a ')' after condition in 'if' statement");

        // Parse the `if` statement's `then` branch, to be executed if the condition evaluates to
        // `true`.
        Statement::Ptr lThen = parseStatement();

        // Check for the presence of an `else` token. If it's found, then parse the `if` statement's
        // `else` branch, to be executed if the condition evaluates to `false`.
        Statement::Ptr lElse = (match({ TokenType::Else }) == true) ?
            parseStatement() : nullptr;

        return std::make_shared<IfStatement>(lCondition, lThen, lElse);

    }

    Statement::Ptr Parser::parseWhileStatement ()
    {

        // The `while` statement's condition is wrapped in parentheses.
        expect(TokenType::OpenParen, "Expected a '(' before condition in 'while' statement");
        Expression::Ptr lCondition = parseExpression();
        expect(TokenType::CloseParen, "Expected a ')' after condition in 'while' statement");

        // Parse the loop's body.
        Statement::Ptr lBody = parseStatement();

        return std::make_shared<WhileStatement>(lCondition, lBody);

    }

    Statement::Ptr Parser::parseForStatement ()
    {
    
        // The `for` loop's initializer, condition and increment portions are wrapped in
        // parentheses. Each portion is also semicolon-delimited.
        //
        // Check for the opening '('.
        expect(TokenType::OpenParen, "Expected '(' before initializer in 'for' loop");

        // Parse the initializer statement. It could be:
        // - Not present at all (`nullptr`).
        // - A variable declaration statement.
        // - An expression statement.
        Statement::Ptr lInitializer = nullptr;
        if (match({ TokenType::Semicolon }))
        {
            // No initializer. `lInitializer` stays `nullptr`.
        }
        else if (match({ TokenType::Var, TokenType::Let, TokenType::Const }))
        {
            lInitializer = parseVariableDeclaration();
        }
        else
        {
            lInitializer = parseExpressionStatement();
        }
        // If an initializer statement was parsed above, their repsective parsing methods will check
        // for the presence of the semicolon ';' at their end.

        // Check to see if a conditional expression is present. If so, then parse it.
        Expression::Ptr lConditional = nullptr;
        if (check(TokenType::Semicolon) == false)
        {
            lConditional = parseExpression();
        }
        expect(TokenType::Semicolon, "Expected ';' after conditional expression in 'for' loop");

        // Now check for the presence of an increment expression, and parse that if present.
        Expression::Ptr lIncrement = nullptr;
        if (check(TokenType::CloseParen) == false)
        {
            lIncrement = parseExpression();
        }
        expect(TokenType::CloseParen, "Expected ')' after increment expression in 'for' loop");

        // Now parse the for loop's body.
        Statement::Ptr lBody = parseStatement();

        return std::make_shared<ForStatement>(lInitializer, lConditional, lIncrement, lBody);

    }

    Statement::Ptr Parser::parseReturnStatement ()
    {

        // Grab the 'return' keyword token.
        auto& lKeyword = previous();

        // Check for the presence of a return value. Parse if present.
        Expression::Ptr lReturnValue = nullptr;
        if (check(TokenType::Semicolon) == false)
        {
            lReturnValue = parseExpression();
        }
        expect(TokenType::Semicolon, "Expected ';' at end of 'return' statement");

        return std::make_shared<ReturnStatement>(lKeyword, lReturnValue);

    }

    Statement::Ptr Parser::parseBlock ()
    {

        // Parse the statements in this block, placing them in a list.
        std::vector<Statement::Ptr> lStatements;
        while (isAtEnd() == false && check(TokenType::CloseBrace) == false)
        {
            lStatements.push_back(parseDeclaration());
        }
        expect(TokenType::CloseBrace, "Expected '}' at end of block statement");

        return std::make_shared<BlockStatement>(lStatements);

    }

    Statement::Ptr Parser::parseExpressionStatement ()
    {

        // Parse an expression, then expect a ';' after it.
        Expression::Ptr lExpression = parseExpression();
        expect(TokenType::Semicolon, "Expected ';' after expression statement");

        return std::make_shared<ExpressionStatement>(lExpression);

    }
    
    /* Private Methods - Parsing Expressions ******************************************************/

    Expression::Ptr Parser::parseExpression ()
    {
        return parseAssignment();
    }

    Expression::Ptr Parser::parseAssignment()
    {
        Expression::Ptr lExpression = parseLogicalOr();
        if (match({ TokenType::AssignEqual }))
        {
            const auto&     lEqual      = previous();
            Expression::Ptr lRighthand  = parseAssignment();
            if (auto lVariable = std::static_pointer_cast<VariableAccessExpression>(lExpression))
            {
                return std::make_shared<VariableAssignExpression>(lVariable->mName, lRighthand);
            }

            throw std::runtime_error { 
                std::format("Invalid assignment target at '{}'", lEqual.getLexeme())
            };
        }

        return lExpression;
    }

    Expression::Ptr Parser::parseLogicalOr()
    {
        Expression::Ptr lExpression = parseLogicalAnd();
        while (match({ TokenType::LogicalOr }))
        {
            auto& lOperator = previous();
            lExpression = std::make_shared<LogicalExpression>(lExpression, parseLogicalAnd(),
                lOperator);
        }

        return lExpression;
    }

    Expression::Ptr Parser::parseLogicalAnd()
    {
        Expression::Ptr lExpression = parseEquality();
        while (match({ TokenType::LogicalAnd }))
        {
            auto& lOperator = previous();
            lExpression = std::make_shared<LogicalExpression>(lExpression, parseEquality(),
                lOperator);
        }

        return lExpression;
    }

    Expression::Ptr Parser::parseEquality()
    {
        Expression::Ptr lExpression = parseComparison();
        while (
            match({
                TokenType::CompareEqual,
                TokenType::CompareStrictEqual,
                TokenType::CompareNotEqual,
                TokenType::CompareStrictNotEqual
            })
        )
        {
            auto& lOperator = previous();
            lExpression = std::make_shared<BinaryExpression>(lExpression, parseComparison(),
                lOperator);
        }

        return lExpression;
    }

    Expression::Ptr Parser::parseComparison()
    {
        Expression::Ptr lExpression = parseAdditive();
        while (
            match({
                TokenType::CompareGreater,
                TokenType::CompareGreaterEqual,
                TokenType::CompareLess,
                TokenType::CompareLessEqual
            })
        )
        {
            auto& lOperator = previous();
            lExpression = std::make_shared<BinaryExpression>(lExpression, parseAdditive(),
                lOperator);
        }

        return lExpression;
    }

    Expression::Ptr Parser::parseAdditive ()
    {
        Expression::Ptr lExpression = parseMultiplicative();
        while (
            match({
                TokenType::Plus,
                TokenType::Minus
            })
        )
        {
            auto& lOperator = previous();
            lExpression = std::make_shared<BinaryExpression>(lExpression, parseMultiplicative(),
                lOperator);
        }

        return lExpression;
    }

    Expression::Ptr Parser::parseMultiplicative ()
    {
        Expression::Ptr lExpression = parseUnary();
        while (
            match({
                TokenType::Times,
                TokenType::Exponent,
                TokenType::Divide,
                TokenType::Modulo
            })
        )
        {
            auto& lOperator = previous();
            lExpression = std::make_shared<BinaryExpression>(lExpression, parseUnary(),
                lOperator);
        }

        return lExpression;
    }

    Expression::Ptr Parser::parseUnary ()
    {
        if (
            match({
                TokenType::LogicalNot,
                TokenType::BitwiseNot,
                TokenType::Plus,
                TokenType::Minus,
                TokenType::Increment,
                TokenType::Decrement
            })
        )
        {
            auto& lOperator = previous();
            Expression::Ptr lRighthand = parseUnary();

            return std::make_shared<UnaryExpression>(lRighthand, lOperator, true);
        }

        return parseCall();
    }

    Expression::Ptr Parser::parseCall ()
    {
        Expression::Ptr lExpression = parsePrimary();
        while (true)
        {
            if (match({ TokenType::OpenParen }))
            {
                lExpression = finishCall(lExpression);
            }
            else if (match({ TokenType::Increment, TokenType::Decrement }))
            {
                auto& lOperator = previous();
                lExpression = std::make_shared<UnaryExpression>(lExpression, lOperator, false);
            }
            else if (match({ TokenType::Period }))
            {
                auto& lName = expect(TokenType::Identifier, 
                    "Expected property name after '.' in object access expression");
                lExpression = std::make_shared<AccessExpression>(lExpression, lName);
            }
            else if (match({ TokenType::OpenBracket }))
            {
                Expression::Ptr lIndex = parseExpression();
                expect(TokenType::CloseBracket, "Expected ']' after index in index expression");
                lExpression = std::make_shared<IndexExpression>(lExpression, lIndex);
            }
            else
            {
                break;
            }
        }

        return lExpression;
    }

    Expression::Ptr Parser::parseFunctionExpression ()
    {

        // The function's name is optional, here. Parse it if it exists.
        std::optional<Token> lName = std::nullopt;
        if (check(TokenType::Identifier))
        {
            auto& lToken = expect(TokenType::Identifier, "Unexpected error parsing function expression");
            lName.emplace(std::move(lToken));
        }
        
        // If the function has any parameters, then store them here.
        std::vector<Token> lParameters;
        expect(TokenType::OpenParen, "Expected '(' before parameter list in function expression");
        if (check(TokenType::CloseParen) == false)
        {
            do
            {
                auto& lParam = expect(TokenType::Identifier, "Expected a parameter name");
                lParameters.push_back(std::move(lParam));
            } while (match({ TokenType::Comma }));
        }
        expect(TokenType::CloseParen, "Expected a ')' before function expression body");
        expect(TokenType::OpenBrace, "Expected a '{' before function expression body");

        // Parse the function's body.
        Statement::Ptr lBody = parseBlock();

        return std::make_shared<FunctionDeclarationExpression>(
            lName, 
            lParameters, 
            std::static_pointer_cast<BlockStatement>(lBody)
        );

    }

    Expression::Ptr Parser::parseArrowFunctionExpression ()
    {

        // Keep track of the old token pointer.
        std::size_t lCurrentPointer = mPointer;

        // Capture the prospective function's arguments.
        // - If the current token is an identifier, then there is only one argument.
        // - If the current token is an open parenthesis, then there may or may not be arguments.
        std::vector<Token> lParamTokens;
        if (match({ TokenType::Identifier }))
        {
            lParamTokens.push_back(previous());
        }
        else if (match({ TokenType::OpenParen }))
        {
            if (check(TokenType::CloseParen) == false)
            {
                do
                {
                    if (check(TokenType::Identifier) == false)
                    {
                        mPointer = lCurrentPointer;
                        return nullptr;
                    }

                    lParamTokens.push_back(advance());
                } while (match({ TokenType::Comma }));
            }
            
            if (match({ TokenType::CloseParen }) == false)
            {
                mPointer = lCurrentPointer;
                return nullptr;
            }
        }

        // Check for the arrow '=>'. This indicates that we are actually parsing an arrow function
        // expression.
        if (match({ TokenType::Arrow }))
        {
            // The arrow function's body can be a single expression or a block statement.
            // If the body is a single expression, then the return value of that function is the
            // value produced by that expression.
            std::variant<Expression::Ptr, BlockStatement::Ptr> lBody;
            if (match({ TokenType::OpenBrace }))
            {
                lBody = std::static_pointer_cast<BlockStatement>(parseBlock());
            }
            else
            {
                lBody = parseExpression();
            }

            return std::make_shared<ArrowFunctionExpression>(lParamTokens, lBody);
        }

        // Not an arrow function. Rewind pointer and return null.
        mPointer = lCurrentPointer;
        return nullptr;

    }

    Expression::Ptr Parser::parseArray ()
    {
        std::vector<Expression::Ptr> lElements;
        if (check(TokenType::CloseBracket) == false)
        {
            do
            {
                lElements.push_back(parseExpression());
            } while (match({ TokenType::Comma }));
        }
        expect(TokenType::CloseBracket, "Expected ']' at end of array");
        return std::make_shared<ArrayExpression>(lElements);
    }

    Expression::Ptr Parser::parseObject ()
    {
        std::vector<Property> lProperties;
        if (check(TokenType::CloseBracket) == false)
        {
            do
            {
                auto& lKey = expect({ TokenType::Identifier, TokenType::StringLiteral },
                    "Expected a property name in object");
                
                if (match({ TokenType::Colon }))
                {
                    Expression::Ptr lValue = parseExpression();
                    lProperties.push_back({ lKey, lValue });
                }
                else
                {
                    auto lKeyCopy = lKey;
                    Expression::Ptr lValue = std::make_shared<VariableAccessExpression>(lKeyCopy);
                    lProperties.push_back({ lKey, lValue });
                }

            } while (match({ TokenType::Comma }));
        }
        expect(TokenType::CloseBrace, "Expected '}' at end of object");
        return std::make_shared<ObjectExpression>(lProperties);
    }

    Expression::Ptr Parser::parsePrimary ()
    {
        // Special case: check for an arrow function expression.
        if (check(TokenType::Identifier) || check(TokenType::OpenParen))
        {
            auto lArrowFunction = parseArrowFunctionExpression();
            if (lArrowFunction != nullptr)
            {
                return lArrowFunction;
            }
        }

        if (
            match({
                TokenType::BooleanLiteral,
                TokenType::NullLiteral,
                TokenType::StringLiteral,
                TokenType::NumericLiteral,
                TokenType::BinaryLiteral,
                TokenType::OctalLiteral,
                TokenType::HexadecimalLiteral
            })
        )
        {
            return std::make_shared<LiteralExpression>(previous().getLiteral());
        }
        else if (match({ TokenType::Identifier }))
        {
            return std::make_shared<VariableAccessExpression>(previous());
        }
        else if (match({ TokenType::Function }))
        {
            return parseFunctionExpression();
        }
        else if (match({ TokenType::OpenBracket }))
        {
            return parseArray();
        }
        else if (match({ TokenType::OpenBrace }))
        {
            return parseObject();
        }
        else if (match({ TokenType::OpenParen }))
        {
            Expression::Ptr lExpression = parseExpression();
            expect(TokenType::CloseParen, "Expected ')' at end of parenthesis-enclosed expression");
            return lExpression;
        }

        throw std::runtime_error {
            std::format("Unexpected token: '{}'.", peek().getLexeme())
        };
    }

    /* Private Methods - Helpers ******************************************************************/

    Expression::Ptr Parser::finishCall (const Expression::Ptr& pCallee)
    {
        std::vector<Expression::Ptr> lArguments;
        if (check(TokenType::CloseParen) == false)
        {
            do
            {
                lArguments.push_back(parseExpression());
            } while (match({ TokenType::Comma }));
        }

        auto& lCloseParen = expect(TokenType::CloseParen, "Expected ')' after arguments in function call");
        return std::make_shared<CallExpression>(pCallee, lCloseParen, lArguments);
    }

}
