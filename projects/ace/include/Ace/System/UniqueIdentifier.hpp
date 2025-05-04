/**
 * @file    Ace/System/UniqueIdentifier.hpp
 * @brief   Provides a structure containing a stable, unique identifier for comparisons, mappings
 *          and associations.
 */

#pragma once
#include <Ace/Common.hpp>

namespace ace
{

    /**
     * @struct  `ace::UniqueIdentifier`
     * @brief   A structure which contains a stable, unique identifier made up of two integers. This
     *          identifier is used for comparisons, mappings and associations in the engine.
     * 
     * @tparam  T   The integer type of the identifier's high and low components.
     */
    template <typename T>
    struct UniqueIdentifier
    {

        // `T` must be an integer type.
        static_assert(std::is_integral_v<T> && std::is_unsigned_v<T>, 
            "'T' must be an unsigned integer type.");

        /**
         * @brief   The high and low integers making up the unique identifier.
         */
        T mHigh = 0, mLow = 0;

        /**
         * @def     `ace::UniqueIdentifier::operator==`
         * @brief   Check to see if two @a `UniqueIdentifiers` of the same type are equal.
         * 
         * @param   pOther  The type of the other @a `UniqueIdentifier` to check against.
         * 
         * @return  `true` if the identifiers are equal; `false` otherwise.
         */
        inline bool operator== (const UniqueIdentifier<T>& pOther) const noexcept
        {
            return pOther.mHigh == mHigh && pOther.mLow == mLow;
        }

        /**
         * @def     `ace::UniqueIdentifier::operator==`
         * @brief   Check to see if two @a `UniqueIdentifiers` of differing types are equal.
         * 
         * @tparam  U   The type of the other @a `UniqueIdentifier` to check against.
         * 
         * @param   pOther  The type of the other @a `UniqueIdentifier` to check against.
         * 
         * @return  `true` if the identifiers are equal; `false` otherwise.
         */
        template <typename U>
        inline bool operator== (const UniqueIdentifier<U>& pOther) const noexcept
        {
            return pOther.mHigh == mHigh && pOther.mLow == mLow;
        }

    };

    using UID8      = UniqueIdentifier<std::uint8_t>;
    using UID16     = UniqueIdentifier<std::uint16_t>;
    using UID32     = UniqueIdentifier<std::uint32_t>;
    using UID64     = UniqueIdentifier<std::uint64_t>;
    using UniqueID  = UID64;

}

/* Specializations of `std::hash` *****************************************************************/

namespace std
{

    template <>
    struct hash<ace::UniqueIdentifier<uint64_t>>
    {
        inline size_t operator() (const ace::UniqueIdentifier<uint64_t>& pId) const noexcept
        {
            return
                (hash<uint64_t>()(pId.mHigh)) ^
                (hash<uint64_t>()(pId.mLow) << 1);
        }
    };

    template <>
    struct hash<ace::UniqueIdentifier<uint32_t>>
    {
        inline size_t operator() (const ace::UniqueIdentifier<uint32_t>& pId) const noexcept
        {
            return
                (hash<uint32_t>()(pId.mHigh)) ^
                (hash<uint32_t>()(pId.mLow) << 1);
        }
    };

    template <>
    struct hash<ace::UniqueIdentifier<uint16_t>>
    {
        inline size_t operator() (const ace::UniqueIdentifier<uint16_t>& pId) const noexcept
        {
            return
                (hash<uint16_t>()(pId.mHigh)) ^
                (hash<uint16_t>()(pId.mLow) << 1);
        }
    };

    template <>
    struct hash<ace::UniqueIdentifier<uint8_t>>
    {
        inline size_t operator() (const ace::UniqueIdentifier<uint8_t>& pId) const noexcept
        {
            return
                (hash<uint8_t>()(pId.mHigh)) ^
                (hash<uint8_t>()(pId.mLow) << 1);
        }
    };

}
