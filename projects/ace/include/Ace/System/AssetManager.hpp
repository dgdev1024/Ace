/**
 * @file    Ace/System/AssetManager.hpp
 * @brief   Provides a static class used for loading and retrieving game assets.
 */

#pragma once
#include <Ace/System/Logging.hpp>
#include <Ace/System/UniqueIdentifier.hpp>
#include <Ace/System/AssetBundle.hpp>
#include <Ace/System/Filesystem.hpp>

namespace ace
{

    /**
     * @def     `ace::Deserializable`
     * @brief   Provides a requirement for templated objects to contain a method for deserializing
     *          data.
     * 
     * Eligible templates must have a well-formed default constructor (not deleted with ` = delete`)
     * and a method `bool deserialize (std::span<std::uint8_t>)`.
     */
    template <typename T>
    concept Deserializable = 
        std::default_initializable<T> &&
        requires (T pObject, std::span<std::uint8_t> pData)
        {
            { pObject.deserialize(pData) } -> std::same_as<bool>;
        };

    /**
     * @class   `ace::AssetManager`
     * @brief   A static class used for loading and retrieving game assets.
     */
    class ACE_API AssetManager
    {
    public:

        /**
         * @struct  `ace::AssetManager::Handle`
         * @brief   Provides a type-erased handle to a loaded game asset.
         */
        struct Handle
        {
            std::any    mAssetPointer;      ///< @brief Holds a type-erased `std::shared_ptr` to the asset's data.
            UniqueID    mID;                ///< @brief The unique identifier associated with the asset.
        
            /**
             * @fn      `ace::AssetManager::Handle::to`
             * @brief   Casts the type-erased asset pointer to an `std::shared_ptr` of type `T`.
             * 
             * @tparam  T       The type to cast the asset into.
             * 
             * @return  The cast `std::shared_ptr<T>`.
             */
            template <typename T>
            inline std::shared_ptr<T> to () const
            {
                return std::any_cast<std::shared_ptr<T>>(mAssetPointer);
            }
        };

    public:

        /**
         * @fn      `ace::AssetManager::loadAsset`
         * @brief   Reads, deserializes and stores an asset found at the given virtual path,
         *          associating it with the given unique ID.
         * 
         * @tparam  T       The type of the asset being loaded.
         * 
         * @param   pID     The unique ID of the asset being loaded.
         * @param   pPath   The virtual path to the asset to load.
         * 
         * @return  A `Handle` to the loaded asset and its ID.
         * 
         * @throw   `std::invalid_argument` if @a `pPath` is empty.
         * @throw   `std::runtime_error` if the asset object's @a `deserialize` method returns
         *          `false`.
         */
        template <Deserializable T>
        inline static Handle loadAsset (const UniqueID& pID, const std::string& pPath)
        {
            // Ensure the given path is not empty.
            if (pPath.empty() == true)
            {
                ACE_ENGINE_THROW(std::invalid_argument, "Virtual asset path cannot be blank!");
            }

            // Read the asset path from the virtual filesystem.
            auto lAssetData = Filesystem::readAsset(pPath);

            // Construct the asset object and attempt to deserialize it from the raw data.
            auto lAssetObject = makeShared<T>();
            if (lAssetObject->deserialize(std::span { lAssetData.data(), lAssetData.size() }) == false)
            {
                ACE_ENGINE_THROW(std::runtime_error, "Asset '{}' could not be deserialized!", pPath);
            }

            // Store the asset data in the registry, under a lock.
            {
                std::lock_guard lLock { sMutex };
                sRegistry[pID] = std::static_pointer_cast<void>(lAssetObject);
            }

            // Return the handle to the loaded asset and its ID.
            return Handle { lAssetObject, pID };
        }

        /**
         * @fn      `ace::AssetManager::loadAssetAsync`
         * @brief   Reads, deserializes and stores an asset found at the given virtual path,
         *          associating it with the given unique ID. This is an asynchronous version of the
         *          `loadAsset` method, which will load the asset on a different thread.
         * 
         * @tparam  T       The type of the asset being loaded.
         * 
         * @param   pID     The unique ID of the asset being loaded.
         * @param   pPath   The virtual path to the asset to load.
         * 
         * @return  An `std::future` which will hold a `Handle` to the loaded asset and its associated
         *          ID when successful.
         */
        template <Deserializable T>
        inline static std::future<Handle> loadAssetAsync (const UniqueID& pID, const std::string& pPath)
        {
            return std::async(
                std::launch::async,
                [=] ()
                {
                    return loadAsset<T>(pID, pPath);
                }
            );
        }

        /**
         * @fn      `ace::AssetManager::getAsset`
         * @brief   Retrieves a loaded asset associated with the given @a `UniqueID`.
         * 
         * @param   pID     The unique ID of the loaded asset being retrieved.
         * 
         * @return  A handle to the loaded asset if found.
         * 
         * @throw   `std::out_of_range` if there is no asset with the given @a `UniqueID`.
         */
        inline static Handle getAsset (const UniqueID& pID)
        {
            std::lock_guard lLock { sMutex };

            // Find the asset. Throw if not found.
            auto lIter = sRegistry.find(pID);
            if (lIter == sRegistry.end())
            {
                ACE_ENGINE_THROW(std::out_of_range, "Asset not loaded!");
            }

            return Handle { lIter->second, pID };
        }

        /**
         * @fn      `ace::AssetManager::unloadAsset`
         * @brief   Unloads a loaded asset associated with the given @a `UniqueID`, removing that
         *          asset from this @a `AssetManager`.
         * 
         * If no asset exists with the given ID, then this method does nothing.
         * 
         * @param   pID     The unique ID of the loaded asset being unloaded.
         */
        inline static void unloadAsset (const UniqueID& pID)
        {
            std::lock_guard lLock { sMutex };
            sRegistry.erase(pID);
        }

    private:

        /**
         * @def     `ace::AssetManager::sMutex`
         * @brief   The mutex used to lock down the asset manager's resources.
         */
        inline static std::mutex sMutex;

        /**
         * @def     `ace::AssetManager::sRegistry`
         * @brief   A map mapping unique asset IDs to blobs of asset data.
         */
        inline static std::unordered_map<UniqueID, std::shared_ptr<void>> sRegistry;

    };

}
