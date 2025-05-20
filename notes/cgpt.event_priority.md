# Prompt

“Modify `EventBus` so subscriptions take an optional priority integer (preferably unsigned, fixed-width, `std::uint64_t`), and handlers are invoked in descending priority order.”

# Response

Below is a patch to **`ace::EventBus`** that adds a **priority** (`std::uint64_t`) to each subscription and ensures handlers are invoked in **descending** priority order. Handlers with higher numeric priority run first; equal‑priority handlers run in subscription order.

```cpp
#pragma once

#include <algorithm>
#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>
#include <cstdint>

namespace ace {

class ACE_API EventBus final {
public:
    /**
     * @brief   Subscribes the given function to events of type `T`, with an optional priority.
     * 
     * Higher priority handlers are invoked first. Default priority is 0.
     */
    template <typename T>
    static std::size_t Subscribe(
        std::function<bool(const T&)> pFunction,
        std::uint64_t                 pPriority = 0
    ) {
        std::lock_guard lock(sHandlersMutex);
        std::type_index key(typeid(T));

        // Create handler
        auto handler = std::make_shared<EventHandler<T>>(
            sNextID.fetch_add(1, std::memory_order_relaxed),
            std::move(pFunction),
            pPriority
        );

        auto& vec = sHandlers[key];
        // Insert in descending priority order
        auto it = std::find_if(vec.begin(), vec.end(),
            [&](auto& basePtr) {
                return static_cast<EventHandler<T>*>(basePtr.get())->mPriority < pPriority;
            });
        vec.insert(it, handler);

        return handler->mID;
    }

    /** 
     * @brief   Unsubscribe the handler with the given ID.
     */
    static void Unsubscribe(const std::size_t& pID) {
        std::lock_guard lock(sHandlersMutex);
        for (auto it = sHandlers.begin(); it != sHandlers.end(); ) {
            auto& vec = it->second;
            vec.erase(std::remove_if(vec.begin(), vec.end(),
                [&](auto& basePtr) { return basePtr->mID == pID; }),
                vec.end());
            if (vec.empty()) it = sHandlers.erase(it);
            else             ++it;
        }
    }

    /**
     * @brief   Immediately emits an event of type `T`, invoking handlers in priority order.
     */
    template <typename T>
    static void Emit(const T& pEvent) {
        std::vector<std::shared_ptr<BaseEventHandler>> handlersCopy;
        {
            std::lock_guard lock(sHandlersMutex);
            auto it = sHandlers.find(std::type_index(typeid(T)));
            if (it == sHandlers.end()) return;
            handlersCopy = it->second;  // already in descending priority
        }

        for (auto& basePtr : handlersCopy) {
            auto derived = static_cast<EventHandler<T>*>(basePtr.get());
            if (!derived->mFunction(pEvent))
                break;  // stop propagation if handler returns false
        }
    }

private:
    struct BaseEventHandler {
        explicit BaseEventHandler(std::size_t id, std::uint64_t prio)
          : mID(id), mPriority(prio) {}
        virtual ~BaseEventHandler() = default;
        std::size_t    mID;
        std::uint64_t  mPriority;
    };

    template <typename T>
    struct EventHandler : BaseEventHandler {
        EventHandler(
            std::size_t            id,
            std::function<bool(const T&)> fn,
            std::uint64_t          prio
        )
          : BaseEventHandler(id, prio)
          , mFunction(std::move(fn))
        {}
        std::function<bool(const T&)> mFunction;
    };

    using HandlerList = std::vector<std::shared_ptr<BaseEventHandler>>;
    using EventMap     = std::unordered_map<std::type_index, HandlerList>;

    static inline EventMap                 sHandlers;
    static inline std::mutex               sHandlersMutex;
    static inline std::atomic<std::size_t> sNextID{1};
};

} // namespace ace
```

### Key Changes

1. **`BaseEventHandler` and `EventHandler<T>`** now carry a `std::uint64_t mPriority` set at construction.
2. **`Subscribe`** signature gains a `pPriority` parameter (default `0`).
3. On subscription we **insert** the new handler into the vector **before** the first element with *lower* priority, keeping the list sorted in descending order.
4. **`Emit`** copies and invokes handlers in that vector order, so higher‑priority ones run first.
5. Unsubscription logic is unchanged except it now erases by `mID` regardless of priority.

With this in place, users can do:

```cpp
// High‑priority handler
EventBus::Subscribe<MyEvent>(
    [](const MyEvent& e){ /* ... */ return true; },
    /* priority: */ 100
);

// Default‑priority handler (0)
EventBus::Subscribe<MyEvent>(
    [](const MyEvent& e){ /* ... */ return true; }
);
```

and know the priority‑100 handler will always be called before the priority‑0 handler.
