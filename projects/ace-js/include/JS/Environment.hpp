/**
 * @file    JS/Environment.hpp
 * @brief   Provides a class used for storing, accessing and manipulating variables.
 */

#pragma once
#include <JS/Value.hpp>

namespace js
{

    /**
     * @class   `js::Environment`
     * @brief   Provides an interface for storing, accessing and manipulating variables.
     */
    class JS_API Environment final
    {
    public:

        /**
         * @fn      `js::Environment::Environment`
         * @brief   Constructs an @a `Environment` with the given parent.
         * 
         * @param   pEnclosing  A pointer to the parent environment.
         */
        explicit Environment (std::shared_ptr<Environment> pEnclosing);

    public:

        /**
         * @fn      `js::Environment::define`
         * @brief   Defines a new variable in this environment.
         * 
         * If a non-constant variable with this name already exists, then it will be overwritten.
         * 
         * @param   pName       The variable's name.
         * @param   pValue      The variable's value.
         * @param   pIsConstant Is this an immutable constant?
         * 
         * @return  `true` if the variable is defined or overwritten successfully; `false`
         *          otherwise.
         */
        bool define (const std::string& pName, const Value& pValue, bool pIsConstant = false);

        /**
         * @fn      `js::Environment::assign`
         * @brief   Assigns a new value to an existing variable in this environment, or a parent
         *          thereof.
         * 
         * If the variable does not exist in this environment, it will search for the variable in
         * its parent environments and reassign the first one it finds.
         * 
         * @param   pName       The name of the variable to reassign.
         * @param   pValue      The value to reassign the variable with.
         * 
         * @return  `true` if the variable exists and is reassigned successfully; `false` otherwise.
         */
        bool assign (const std::string& pName, const Value& pValue);

        /**
         * @fn      `js::Environment::resolve`
         * @brief   Resolves the given name to a variable in this environment, or a parent thereof.
         * 
         * If the variable does not exist in this environment, it will search for the variable in
         * its parent environments.
         * 
         * @param   pName       The name of the variable to search for.
         * 
         * @return  An `std::optional` which will contain the variable's value if found, or
         *          `std::nullopt` if not.
         */
        std::optional<Value> resolve (const std::string& pName) const;

    private:

        /**
         * @def     `js::Environment::mEnclosing`
         * @brief   A pointer to the parent environment.
         */
        std::shared_ptr<Environment> mEnclosing = nullptr;

        /**
         * @def     `js::Environment::mValues`
         * @brief   Contains all defined values.
         */
        std::unordered_map<std::string, Value> mValues;

        /**
         * @def     `js::Environment::mConstants`
         * @brief   Contains the keys associated with all constant values.
         * 
         * If a key is contained in this set, the value mapped to that key in the @a `mValues`
         * map cannot be manipulated.
         */
        std::unordered_set<std::string> mConstants;

    };

}
