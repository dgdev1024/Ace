/**
 * @file    JS/Value.cpp
 */

#include <JS/Precompiled.hpp>
#include <JS/Value.hpp>

namespace js
{

    /* Public Methods *****************************************************************************/

    bool Value::isTruthy () const
    {
        switch (mType)
        {
            case ValueType::Undefined:
            case ValueType::Null:
                return false;
            case ValueType::Boolean:
                return std::get<bool>(mData);
            case ValueType::Number:
            {
                const auto& lData = std::get<double>(mData);
                return lData != 0.0 && (std::isnan(lData) == false);
            }
            case ValueType::String:
                return std::get<std::string>(mData).empty() == false;
            case ValueType::Object:
            case ValueType::Array:
            case ValueType::Function:
                return true;
            default:
                return false;
        }
    }

    std::string Value::toString () const
    {
        return std::visit(
            Overloaded
            {
                [] (std::monostate)
                {
                    return std::string { "undefined" };
                },
                [] (std::nullptr_t)
                {
                    return std::string { "null" };
                },
                [] (bool pValue)
                {
                    return std::string { pValue ? "true" : "false" };
                },
                [] (double pValue)
                {
                    std::ostringstream lStream;
                    lStream << pValue;
                    return lStream.str();
                },
                [] (const std::string& pValue)
                {
                    return pValue;
                },
                [] (const std::shared_ptr<Object>&)
                {
                    return std::string { "[object Object]" };
                },
                [] (const std::shared_ptr<Array>& pValue)
                {
                    std::ostringstream lStream;
                    lStream << "[";
                    for (size_t i = 0; i < pValue->mElements.size(); ++i) {
                        lStream << pValue->mElements[i].toString();
                        if (i + 1 < pValue->mElements.size()) lStream << ", ";
                    }
                    lStream << "]";
                    return lStream.str();
                },
                [] (const std::shared_ptr<Function>& pValue) {
                    return std::format("function {} ()", pValue->mName);
                }
            },
            mData
        );
    }

    /* Static Methods *****************************************************************************/

    Value Value::makeUndefined ()
    {
        return Value {};
    }

    Value Value::makeNull ()
    {
        Value v;
        v.mType = ValueType::Null;
        v.mData = nullptr;
        return v;
    }

    Value Value::makeBoolean (bool pValue)
    {
        Value v;
        v.mType = ValueType::Boolean;
        v.mData = pValue;
        return v;
    }

    Value Value::makeNumber (double pValue)
    {
        Value v;
        v.mType = ValueType::Number;
        v.mData = pValue;
        return v;
    }

    Value Value::makeString (const std::string& pValue)
    {
        Value v;
        v.mType = ValueType::String;
        v.mData = pValue;
        return v;
    }

    Value Value::makeObject (const std::shared_ptr<Object>& pValue)
    {
        Value v;
        v.mType = ValueType::Object;
        v.mData = pValue;
        return v;
    }

    Value Value::makeArray (const std::shared_ptr<Array>& pValue)
    {
        Value v;
        v.mType = ValueType::Array;
        v.mData = pValue;
        return v;
    }

    Value Value::makeFunction (const std::shared_ptr<Function>& pValue)
    {
        Value v;
        v.mType = ValueType::Function;
        v.mData = pValue;
        return v;
    }

}
