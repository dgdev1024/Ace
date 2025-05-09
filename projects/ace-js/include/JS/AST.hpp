/**
 * @file    JS/AST.hpp
 * @brief   Provides classes defining the abstract syntax tree (AST) nodes produced by the parser.
 */

#pragma once
#include <JS/Token.hpp>

namespace js
{

    /**
     * @class   `js::Expression`
     * @brief   The base class for all expression nodes.
     * 
     * Expression nodes are syntax nodes which produce a value.
     */
    class JS_API Expression
    {
    public:
        using Ptr = std::shared_ptr<Expression>;
        
    public:
        virtual ~Expression () = default;
        virtual std::string toString () const = 0;

    };

    /**
     * @class   `js::Statement`
     * @brief   The base class for all statement nodes.
     * 
     * Statement nodes are syntax nodes which control program flow - they don't directly produce
     * values.
     */
    class JS_API Statement
    {
    public:
        using Ptr = std::shared_ptr<Statement>;
        
    public:
        virtual ~Statement () = default;
        virtual std::string toString () const = 0;

    };

    /**
     * @class   `js::LiteralExpression`
     * @brief   An expression which contains a literal value (string, number, boolean, null).
     */
    class JS_API LiteralExpression final : public Expression
    {
    public:
        explicit LiteralExpression (Literal& pLiteral);
        std::string toString () const override;

    public:
        Literal mLiteral;

    };

    /**
     * @class   `js::VariableAccessExpression`
     * @brief   An expression used for accessing a variable by name (an identifier).
     */
    class JS_API VariableAccessExpression final : public Expression
    {
    public:
        explicit VariableAccessExpression (Token& pName);
        std::string toString () const override;

    public:
        Token mName;

    };

    /**
     * @class   `js::VariableAssignExpression`
     * @brief   An expression used for writing a value to a variable (`<name> = <value>`).
     */
    class JS_API VariableAssignExpression final : public Expression
    {
    public:
        explicit VariableAssignExpression (Token& pName, const Expression::Ptr& pValue);
        std::string toString () const override;

    public:
        Token           mName;
        Expression::Ptr mValue = nullptr;

    };

    /**
     * @class   `js::BinaryExpression`
     * @brief   An expression containing a binary operation between two sub-expressions
     *          (`<left> <operator> <right>`).
     */    
    class JS_API BinaryExpression : public Expression
    {
    public:
        explicit BinaryExpression (const Expression::Ptr& pLeft, const Expression::Ptr& pRight,
            Token& pOperator);
        std::string toString () const override;

    public:
        Expression::Ptr mLeft = nullptr;
        Expression::Ptr mRight = nullptr;
        Token mOperator;

    };

    /**
     * @class   `js::UnaryExpression`
     * @brief   An expression containing an operation on a single expression. This may be a
     *          prefix operation (`<operator> <operand>`) or a postfix operation
     *          (`<operand> <operator>`).
     */
    class JS_API UnaryExpression final : public Expression
    {
    public:
        explicit UnaryExpression (const Expression::Ptr& pOperand, Token& pOperator,
            bool pIsPrefix = true);
        std::string toString () const override;

    public:
        Expression::Ptr mOperand = nullptr;
        Token mOperator;
        bool mIsPrefix = true;

    };

    /**
     * @class   `js::LogicalExpression`
     * @brief   A type of @a `BinaryExpression` used for short-circuit logic operations.
     * 
     * @sa      `js::BinaryExpression`
     */
    class JS_API LogicalExpression final : public BinaryExpression
    {
    public:
        explicit LogicalExpression (const Expression::Ptr& pLeft, const Expression::Ptr& pRight,
            Token& pOperator);

    };

    /**
     * @class   `js::CallExpression`
     * @brief   An expression containing a function call (`<callee>(arg1, arg2, ...)`).
     */
    class JS_API CallExpression final : public Expression
    {
    public:
        explicit CallExpression (const Expression::Ptr& pCallee, Token& pCloseParen,
            std::vector<Expression::Ptr>& pArguments);
        std::string toString () const override;

    public:
        Expression::Ptr                 mCallee = nullptr;
        Token                           mCloseParen;
        std::vector<Expression::Ptr>    mArguments;

    };

    /**
     * @class   `js::ArrayExpression`
     * @brief   An expression containing an array of sub-expressions:
     *          `myArray[1, "John Doe", [ "so", "and", "so" ] ]`
     */
    class JS_API ArrayExpression final : public Expression
    {
    public:
        explicit ArrayExpression (std::vector<Expression::Ptr>& pElements);
        std::string toString () const override;

    public:
        std::vector<Expression::Ptr> mElements;

    };

    /**
     * @struct  `js::Property`
     * @brief   A token-expression pair making up a single property in an @a `ObjectExpression`.
     */
    struct Property
    {
        Token               mKey;
        Expression::Ptr     mValue;
    };

    /**
     * @class   `js::ObjectExpression`
     * @brief   An expression containing an array of key-expression pairs making up an object:
     *          `{ name: "John Doe", age: 42, hobbies: [ "Programming", "Music" ] }`
     */
    class JS_API ObjectExpression final : public Expression
    {
    public:
        explicit ObjectExpression (std::vector<Property>& pProperties);
        std::string toString () const override;

    public:
        std::vector<Property> mProperties;

    };

    /**
     * @class   `js::AccessExpression`
     * @brief   An expression used to access an object:
     *          `person.name`
     */
    class JS_API AccessExpression final : public Expression
    {
    public:
        explicit AccessExpression (const Expression::Ptr& pObject, Token& pName);
        std::string toString() const override;

    public:
        Expression::Ptr mObject;
        Token mName;
        
    };

    /**
     * @class   `js::IndexExpression`
     * @brief   An expression used to index an array or object:
     *          `numbers[19]`, `person[name]`
     */
    class JS_API IndexExpression final : public Expression
    {
    public:
        explicit IndexExpression (const Expression::Ptr& pObject, const Expression::Ptr& pIndex);
        std::string toString() const override;

    public:
        Expression::Ptr mObject;
        Expression::Ptr mIndex;
        
    };

    /**
     * @class   `js::ExpressionStatement`
     * @brief   A statement containing an expression (for instance, a function call `foo();`).
     */
    class JS_API ExpressionStatement final : public Statement
    {
    public:
        explicit ExpressionStatement (const Expression::Ptr& pExpression);
        std::string toString () const override;

    public:
        Expression::Ptr mExpression = nullptr;

    };

    /**
     * @class   `js::VariableDeclarationStatement`
     * @brief   A statement containing the declaration of a variable (eg. `var x = 5;`, `let y;`, 
     *          `const greet = "hello, world!";`).
     */
    class JS_API VariableDeclarationStatement final : public Statement
    {
    public:
        explicit VariableDeclarationStatement (Token& pName, const TokenType& pType,
            const Expression::Ptr& mValue = nullptr);
        std::string toString () const override;

    public:
        Token           mName;
        TokenType       mType = TokenType::Unknown;
        Expression::Ptr mValue = nullptr;

    };

    /**
     * @class   `js::BlockStatement`
     * @brief   A statement containing a brace-grouped, semicolon-delimited list of sub-statements.
     */
    class JS_API BlockStatement final : public Statement
    {
    public:
        using Ptr = std::shared_ptr<BlockStatement>;

    public:
        explicit BlockStatement (std::vector<Statement::Ptr>& pStatements);
        std::string toString () const override;

    public:
        std::vector<Statement::Ptr> mStatements;

    };

    /**
     * @class   `js::IfStatement`
     * @brief   A statement which contains a statement to execute if a given condition is true, and
     *          another, optional statement to execute if that condition is false.
     *          (`if (condition) thenStatement; else elseStatement;`)
     */
    class JS_API IfStatement final : public Statement
    {
    public:
        explicit IfStatement (const Expression::Ptr& pCondition, const Statement::Ptr& pThen,
            const Statement::Ptr& pElse);
        std::string toString () const override;

    public:
        Expression::Ptr mCondition = nullptr;
        Statement::Ptr mThen = nullptr;
        Statement::Ptr mElse = nullptr;

    };

    /**
     * @class   `WhileStatement`
     * @brief   A loop statement which continues to execute as long as a given condition is true
     *          (`while (condition) branch;`)
     */
    class JS_API WhileStatement final : public Statement
    {
    public:
        explicit WhileStatement (const Expression::Ptr& pCondition, const Statement::Ptr& pBody);
        std::string toString () const override;

    public:
        Expression::Ptr mCondition = nullptr;
        Statement::Ptr mBody = nullptr;

    };

    /**
     * @class   `js::ForStatement`
     * @brief   A loop statement which resembles a classic C-style `for` loop:
     *          `for (mInitializer; mCondition; mIncrement) mBody;`
     */
    class JS_API ForStatement final : public Statement
    {
    public:
        ForStatement (const Statement::Ptr& pInitializer, const Expression::Ptr& pCondition,
            const Expression::Ptr& pIncrement, const Statement::Ptr& pBody);
        std::string toString () const override;

    public:
        Statement::Ptr  mInitializer = nullptr;
        Expression::Ptr mCondition = nullptr;
        Expression::Ptr mIncrement = nullptr;
        Statement::Ptr  mBody = nullptr;

    };

    /**
     * @class   `js::FunctionDeclarationStatement`
     * @brief   A statement containing the declaration of a function:
     *          `function mName (arg1, arg2, ...) { mBody; ... }`
     */
    class JS_API FunctionDeclarationStatement final : public Statement
    {
    public:
        explicit FunctionDeclarationStatement (Token& pName, std::vector<Token>& pParamTokens,
            const BlockStatement::Ptr& pBody);
        std::string toString () const override;

    public:
        Token                   mName;
        std::vector<Token>      mParamTokens;
        BlockStatement::Ptr     mBody;
        
    };

    /**
     * @class   `js::FunctionDeclarationExpression`
     * @brief   An expression containing the declaration of a function which is placed into a
     *          variable, or used anonymously: `const add = function (a, b) { return a + b; }`,
     *          `(function (name) { return "Hello, " + name + "!"; })("John Doe");`
     * 
     * The function declared can optionally have a name, too:
     * `const myTimes = function times (a, b) { return a * b; };`
     */
    class JS_API FunctionDeclarationExpression final : public Expression
    {    
    public:
        explicit FunctionDeclarationExpression (std::optional<Token>& pName, 
            std::vector<Token>& pParamTokens, const BlockStatement::Ptr& pBody);
        std::string toString () const override;

    public:
        std::optional<Token>    mName;
        std::vector<Token>      mParamTokens;
        BlockStatement::Ptr     mBody;

    };

    /**
     * @class   `js::ArrowFunctionExpression`
     * @brief   An expression containing the declaration of an arrow function. Arrow functions work
     *          syntatically similar to regular function expressions, only they can be written
     *          as follows:
     *          `const myTimes = (a, b) => { return a * b; };`
     *          `const myGreet = name => { return "Hello, " + name + "!"; };`
     *          `const myTriple = (num) => num * 3;`
     */
    class JS_API ArrowFunctionExpression final : public Expression
    {
    public:
        explicit ArrowFunctionExpression (std::vector<Token>& pParamTokens,
            std::variant<Expression::Ptr, BlockStatement::Ptr>& pBody);
        std::string toString () const override;

    public:
        std::vector<Token> mParamTokens;
        std::variant<Expression::Ptr, BlockStatement::Ptr> mBody;

    };

    /**
     * @class   `js::ReturnStatement`
     * @brief   A statement containing a return from a function: `return;`, `return <value>;`.
     */
    class JS_API ReturnStatement final : public Statement
    {
    public:
        explicit ReturnStatement (Token& pKeyword, const Expression::Ptr& pValue);
        std::string toString () const override;

    public:
        Token           mKeyword;
        Expression::Ptr mValue = nullptr;

    };

}

// "Papus Americanus"
// Cardinal Robert Francis Prevost - Pope Leo XIV