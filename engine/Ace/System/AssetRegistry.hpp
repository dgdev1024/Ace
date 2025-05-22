/**
 * @file    Ace/System/AssetRegistry.hpp
 * @brief   Provides a static class used for loading and accessing game assets.
 */

#pragma once
#include <Ace/System/VirtualFilesystem.hpp>

namespace ace
{

    /**
     * @brief   A thin wrapper around an `std::shared_ptr` to an asset of type
     *          `T`.
     * 
     * @tparam  T           The type of asset contained.
     */
    template <typename T>
    class AssetHandle final
    {
    public:

        /**
         * @brief   Constructs an asset handle pointing to no asset data.
         */
        AssetHandle () = default;

        /**
         * @brief   Constructs an asset handle pointing to the given asset.
         * 
         * @param   pPtr    An `std::shared_ptr` to the asset being referenced.
         */
        explicit AssetHandle (
            std::shared_ptr<T> pPtr
        ) :
            mPtr    { std::move(pPtr) }
        {}

        /**
         * @brief   Retrieves whether or not this asset handle is referencing
         *          a valid asset.
         * 
         * @return  `true` if @a `mPtr` is not `nullptr`; `false` otherwise.
         */
        inline bool IsValid () const
        {
            return (mPtr != nullptr);
        }

        /**
         * @brief   Retrieves a raw pointer to the asset data.
         * 
         * @return  A raw pointer to the asset data.
         */
        inline astd::raw_ptr<T> Get ()              { return mPtr.get(); }
        inline const astd::raw_ptr<T> Get () const  { return mPtr.get(); }

        /**
         * @brief   Retrieves a raw reference to the asset data.
         * 
         * @return  A raw handle to the asset data.
         */
        inline astd::raw_ref<T> Ref ()              { return *mPtr; }
        inline const astd::raw_ref<T> Ref () const  { return *mPtr; }

    public:
        inline astd::raw_ptr<T> operator-> ()               { return mPtr.get(); }
        inline const astd::raw_ptr<T> operator-> () const   { return mPtr.get(); }
        inline astd::raw_ref<T> operator* ()                { return *mPtr; }
        inline const astd::raw_ref<T> operator* () const    { return *mPtr; }
        inline operator bool () const                       { return (mPtr != nullptr); }

    private:
        std::shared_ptr<T>  mPtr = nullptr; ///< @brief An `std::shared_ptr` to the asset.
        
    };

    /**
     * @brief   An interface for loading assets of type `T`.
     * 
     * @tparam  T       The type of asset being loaded.
     */
    template <typename T>
    class IAssetLoader
    {
    public:

        /**
         * @brief   The default destructor.
         */
        virtual ~IAssetLoader () = default;

        /**
         * @brief   Checks to see if this asset loader can handle the given
         *          virtual asset file.
         * 
         * This check can be performed in one of two different ways (or both):
         * 
         * - Check for a valid file extension at the end of @a `pLogicalPath`.
         * 
         * - Read the first bytes of @a `pVirtualFile` and check for a valid
         *   file header.
         * 
         * @param   pLogicalPath    The asset file's logical path string.
         * @param   pVirtualFile    A handle to the opened virtual file.
         * 
         * @return  `true` if this loader can handle the file being loaded;
         *          `false` otherwise.
         */
        virtual bool CanLoad (
            const std::string&  pLogicalPath,
            const IVirtualFile& pVirtualFile
        ) const = 0;

        /**
         * @brief   Loads an asset's data from a virtual file loaded from the
         *          virtual filesystem (VFS).
         * 
         * @param   pVirtualFile    An `std::unique_ptr` to the opened virtual
         *                          file.
         * 
         * @return  An `std::shared_ptr` to the loaded asset data if successful;
         *          `nullptr` otherwise.
         */
        virtual std::shared_ptr<T> Load (
            std::unique_ptr<IVirtualFile> pVirtualFile
        ) = 0;

    };

    /**
     * @brief   A static class used for loading and accessing game assets.
     */
    class AssetRegistry final
    {
    public:

        /**
         * @brief   Registers an asset loader for assets of type `T`.
         * 
         * Asset loaders need to be registered with the asset registry before
         * they can be used by the registry to load assets. More than one asset
         * loader can be registered to load one type of asset, with the order by
         * which they handle the asset determined by a priority value.
         * 
         * @param   pLoader     An `std::shared_ptr` to the asset loader to use.
         * @param   pPriority   The asset loader's priority. Asset loaders with
         *                      a higher priority get first crack at the asset.
         */
        template <typename T>
        static void RegisterAssetLoader (
            std::shared_ptr<IAssetLoader<T>>    pLoader,
            const std::size_t&                  pPriority = 0
        )
        {
            // Under a lock, register the asset loader.
            std::lock_guard lGuard { sLoadersMutex };

            // Emplace the loader into the loader list, then re-sort the list
            // based on the loaders' priority values.
            LoaderList<T>::sLoaders.emplace_back(pPriority, pLoader);
            std::sort(
                LoaderList<T>::sLoaders.begin(),
                LoaderList<T>::sLoaders.end(),
                [] (auto& pFirst, auto& pSecond)
                {
                    return pFirst.first > pSecond.first;
                }
            );
        }

        /**
         * @brief   Attempts to load an asset of type `T` from the given logical
         *          path.
         * 
         * @param   pLogicalPath    The logical path to the asset's data.
         * 
         * @return  An `AssetHandle<T>` referencing the loaded asset if found;
         *          an empty `AssetHandle<T>` otherwise.
         */
        template <typename T>
        static AssetHandle<T> Load (
            const std::string&  pLogicalPath
        )
        {
            
            // First, check to see if the asset is already cached.
            AssetKey lKey { ACE_TYPEID(T), pLogicalPath };
            {
                std::lock_guard lGuard { sCacheMutex };  // Under a lock, of course.

                auto lIter = sCache.find(lKey);
                if (lIter != sCache.end())
                {
                    if (
                        auto lExisting =
                            std::static_pointer_cast<T>(lIter->second.lock())
                    )
                    {
                        return AssetHandle<T>(lExisting);
                    }
                }
            }

            // The asset is not pre-cached. Attempt to open it via the VFS.
            auto lAssetFile = VFS::OpenFile(pLogicalPath);
            if (lAssetFile == nullptr)
            {
                return AssetHandle<T> {};
            }

            // Under a lock, get a snapshot of the appropriate loaders list.
            std::vector<std::shared_ptr<IAssetLoader<T>>> lLoaderSnapshot;
            {
                std::lock_guard lGuard { sLoadersMutex };
                for (auto& [_, lLoader] : LoaderList<T>::sLoaders)
                {
                    lLoaderSnapshot.push_back(lLoader);
                }
            }


            // Iterate over the loaders in the snapshot, looking for the
            // appropriate loader to load the asset with.
            for (auto& lLoader : lLoaderSnapshot)
            {
                if (lLoader->CanLoad(pLogicalPath, *lAssetFile) == true)
                {
                    // In case the above call to `CanLoad` read some bytes
                    // from the file's header, rewind the file.
                    lAssetFile->Seek(0);

                    // Attempt to load the asset file.
                    auto lAssetData = std::static_pointer_cast<T>(
                        lLoader->Load(std::move(lAssetFile))
                    );
                    if (lAssetData == nullptr)
                    {
                        continue;
                    }

                    // Attempt to cache the asset under a lock.
                    {
                        std::lock_guard lGuard { sCacheMutex };

                        // Although not likely, there is a possibility that this
                        // method on a separate thread has loaded and cached the
                        // very same asset loaded by this method on the current
                        // thread before it has the chance to cache it itself.
                        //
                        // While the cache mutex is still under lock, we will 
                        // account for this possibility here, by re-checking
                        // the cache for the asset.
                        auto lIter2 = sCache.find(lKey);
                        if (lIter2 != sCache.end())
                        {
                            if (
                                auto lCachedOnAnotherThread =
                                    std::static_pointer_cast<T>(
                                        lIter2->second.lock()
                                    )
                            )
                            {
                                return AssetHandle<T> { lCachedOnAnotherThread };
                            }
                        }

                        sCache[lKey] = lAssetData;
                    }

                    // Return a handle to the asset data.
                    return AssetHandle<T> { lAssetData };
                    
                }
            }

            // No available asset loader claimed this asset.
            return AssetHandle<T> {};
        }

    private:

        /**
         * @brief   A templated static structure containing the map of
         *          registered asset loaders.
         * 
         * @param   T       The type of asset being loaded by the given list of
         *                  loaders.
         */
        template <typename T>
        struct LoaderList
        {
            static inline astd::vector_map<
                std::size_t,
                std::shared_ptr<IAssetLoader<T>>
            > sLoaders;
        };

        /**
         * @brief   A structure representing the internal key mapped to an asset.
         */
        struct AssetKey
        {
            std::type_index mType;
            std::string     mLogicalPath;

            inline bool operator== (const AssetKey& pKey) const
                { return mType == pKey.mType && mLogicalPath == pKey.mLogicalPath; }
        };

        /**
         * @brief   A structure used for hashing the internal asset key structure.
         * 
         * This allows the @a `AssetKey` structure to be used as the key in the
         * @a `sCache` map.
         */
        struct AssetKeyHash
        {
            inline std::size_t operator () (const AssetKey& pKey) const noexcept
            {
                return
                    pKey.mType.hash_code() ^
                    (std::hash<std::string>()(pKey.mLogicalPath) << 1);
            }
        };

    private:

        /**
         * @brief   A type map of weak pointers to the most-recently-loaded
         *          asset of each given type.
         */
        static inline std::unordered_map<
            AssetKey,
            std::weak_ptr<void>,
            AssetKeyHash
        > sCache;

        /**
         * @brief   The mutex used to lock down the asset registry's loader map.
         */
        static inline std::mutex sLoadersMutex;

        /**
         * @brief   The mutex used to lock down the asset registry's cache map.
         */
        static inline std::mutex sCacheMutex;

    };

}
