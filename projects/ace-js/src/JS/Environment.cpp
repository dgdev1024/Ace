/**
 * @file    JS/Environment.cpp
 */

#include <JS/Precompiled.hpp>
#include <JS/Environment.hpp>

namespace js
{

    /* Constructors and Destructors ***************************************************************/

    Environment::Environment (std::shared_ptr<Environment> pEnclosing) :
        mEnclosing  { std::move(pEnclosing) }
    {

    }

    /* Public Methods *****************************************************************************/

    bool Environment::define (const std::string& pName, const Value& pValue, bool pIsConstant)
    {
        if (pName.empty() == true)
        {
            std::cerr
                << std::format("Environment Error: Cannot define a variable with no name.")
                << std::endl;
            return false;
        }
        else if (mConstants.contains(pName) == true)
        {
            std::cerr 
                << std::format("Environment Error: '{}' is constant and cannot be redefined.", pName)
                << std::endl;
            return false;
        }

        mValues[pName] = pValue;
        if (pIsConstant == true)
        {
            mConstants.emplace(pName);
        }

        return true;
    }

    bool Environment::assign (const std::string& pName, const Value& pValue)
    {
        if (pName.empty() == true)
        {
            std::cerr
                << std::format("Environment Error: Cannot assign to a variable with no name.")
                << std::endl;
            return false;
        }
        
        auto lIter = mValues.find(pName);
        if (lIter != mValues.end())
        {
            if (mConstants.contains(pName) == true)
            {
                std::cerr
                    << std::format("Environment Error: '{}' is constant and cannot be reassigned.", pName)
                    << std::endl;
                return false;
            }

            lIter->second = pValue;
            return true;
        }
        else if (mEnclosing != nullptr)
        {
            return mEnclosing->assign(pName, pValue);
        }

        std::cerr
            << std::format("Environment Error: '{}' is undefined and cannot be reassigned.", pName)
            << std::endl;
        return false;
    }

    std::optional<Value> Environment::resolve (const std::string& pName) const
    {
        if (pName.empty() == true)
        {
            std::cerr
                << std::format("Environment Error: Cannot resolve a variable with no name.")
                << std::endl;
            return std::nullopt;
        }

        auto lIter = mValues.find(pName);
        if (lIter != mValues.end())
        {
            return lIter->second;
        }
        else if (mEnclosing != nullptr)
        {
            return mEnclosing->resolve(pName);
        }

        std::cerr
            << std::format("Environment Error: '{}' could not be resolved.", pName)
            << std::endl;
        return std::nullopt;
    }

}
