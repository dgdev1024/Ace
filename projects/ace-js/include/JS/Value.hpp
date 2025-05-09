/**
 * @file    JS/Value.hpp
 * @brief   Contains structures representing runtime-values and garbage-collector-managed objects.
 */

#pragma once
#include <JS/TokenType.hpp>

namespace js
{

    /**
     * @enum    `js::ValueType`
     * @brief   Enumerates possible JavaScript runtime value types.
     */
    enum class ValueType
    {
        Undefined,
        Null,
        Boolean,
        Number,
        String,
        Object,
        Array,
        Function
    };

    /** @brief Forward-declarations for garbage-collector-managed objects. */
    struct Object;
    struct Function;
    struct Array;

    /**
     * @typedef `js::ValueData`
     * @brief   Defines a union for storing a @a `Value`'s data.
     */
    using ValueData = std::variant<
        std::monostate,
        std::nullptr_t,
        bool,
        double,
        std::string,
        std::shared_ptr<Object>,
        std::shared_ptr<Array>,
        std::shared_ptr<Function>
    >;

    /**
     * @class   Value
     * @brief   A structure representing a JavaScript runtime value.
     */
    class JS_API Value final
    {
        friend class Environment;
        friend class Interpreter;

    public:
        
        /**
         * @fn      `js::Value::Value`
         * @brief   The default constructor creates an @a `Undefined` value.
         */
        explicit Value () = default;

    public:

        /**
         * @fn      `js::Value::isTruthy`
         * @brief   Retrieves whether or not this value is "truthy" - that is, if the contained
         *          value would evaluate to `true`.
         * 
         * Whether or not a value is "truthy" depends upon its @a `ValueType`:
         * 
         * - @a `ValueType::Undefined` and @a `ValueType::Null` are always `false`.
         * 
         * - @a `ValueType::Boolean` is `true` if the contained value is `true`.
         * 
         * - @a `ValueType::Number` is `true` if the contained value is non-zero.
         * 
         * - @a `ValueType::String` is `true` if the contained string is not empty.
         * 
         * - @a `ValueType::Array`, @a `ValueType::Object` and @a `ValueType::Function` are always
         *   `true`.
         * 
         * @return  `true` if the contained value is a "truthy" value; `false` if not.
         */
        bool isTruthy () const;

        /**
         * @fn      `js::Value::toString`
         * @brief   Retrieves a string representation of the value's data.
         * 
         * @return  The value's data, in string form.
         */
        std::string toString () const;

    public:

        static Value makeUndefined ();
        static Value makeNull ();
        static Value makeBoolean (bool pValue);
        static Value makeNumber (double pValue);
        static Value makeString (const std::string& pValue);
        static Value makeArray (const std::shared_ptr<Array>& pValue);
        static Value makeObject (const std::shared_ptr<Object>& pValue);
        static Value makeFunction (const std::shared_ptr<Function>& pValue);

    private:

        /**
         * @def     `js::Value::mType`
         * @brief   The value's type.
         */
        ValueType mType = ValueType::Undefined;

        /**
         * @def     `js::Value::mData`
         * @brief   A union containing the value's data.
         */
        ValueData mData = std::monostate {};

    };

    /**
     * @struct  `js::Object`
     * @brief   A structure containing a garbage-collected object.
     */
    struct Object
    {
        std::unordered_map<std::string, Value> mProperties;
    };

    /**
     * @struct  `js::Array`
     * @brief   A structure containing a garbage-collected array.
     */
    struct Array
    {
        std::vector<Value> mElements;
    };

    /**
     * @struct  `js::Function`
     * @brief   TODO: Add description.
     */
    struct Function
    {
        std::string mName;              ///< @brief Placeholder...
    };

}
