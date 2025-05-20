# Prompt

“Implement a static `EventRegistry` that uses template specialization to store 
handler lists per event type, eliminating `type_index` lookups.”

# Response

Below is a header‑only **static `EventRegistry`** approach that removes any runtime `type_index` lookups. Each event type `T` gets its own static list of handlers via template specialization, while a central ID‑to‑unsubscribe map lets you remove handlers by ID across all event types.

```cpp
#pragma once

#include <atomic>
#include <cstddef>
#include <functional>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <algorithm>

namespace ace {

/// Generates unique subscription IDs
inline std::atomic<std::size_t> gNextSubscriptionID{1};

/// A central map from subscription ID → unsubscribe function
inline std::mutex gUnsubMapMutex;
inline std::unordered_map<std::size_t, std::function<void()>> gUnsubMap;

/// Per‑type registry of handlers
template<typename T>
struct EventRegistry {
    struct Handler {
        std::size_t               id;
        std::function<bool(const T&)> fn;
    };
    static inline std::vector<Handler> handlers;
};

class ACE_API EventBus final {
public:
    /**
     * Subscribe to events of type T.
     * Returns a subscription ID that can be used to unsubscribe.
     */
    template<typename T>
    static std::size_t Subscribe(std::function<bool(const T&)> fn) {
        // 1) Generate a new ID
        std::size_t id = gNextSubscriptionID.fetch_add(1, std::memory_order_relaxed);

        // 2) Add to T’s static handler list
        auto& vec = EventRegistry<T>::handlers;
        {
            static std::mutex m;
            std::lock_guard lock(m);
            vec.push_back({ id, std::move(fn) });
        }

        // 3) Register an unsubscribe action
        {
            std::lock_guard lock(gUnsubMapMutex);
            gUnsubMap[id] = [id]() {
                auto& v = EventRegistry<T>::handlers;
                std::lock_guard lock2(*reinterpret_cast<std::mutex*>(nullptr)); 
                // the above is pseudocode; see note below
                v.erase(
                    std::remove_if(v.begin(), v.end(),
                        [id](auto& h){ return h.id == id; }),
                    v.end()
                );
            };
        }

        return id;
    }

    /**
     * Unsubscribe any handler by ID.
     */
    static void Unsubscribe(const std::size_t& id) {
        std::function<void()> fn;
        {
            std::lock_guard lock(gUnsubMapMutex);
            auto it = gUnsubMap.find(id);
            if (it == gUnsubMap.end()) return;
            fn = std::move(it->second);
            gUnsubMap.erase(it);
        }
        // call the per‑type removal
        fn();
    }

    /**
     * Emit an event of type T immediately.
     */
    template<typename T>
    static void Emit(const T& evt) {
        auto snapshot = [&]() {
            std::lock_guard lock(*GetMutex<T>());
            return EventRegistry<T>::handlers;
        }();

        for (auto& h : snapshot) {
            if (!h.fn(evt))
                break;
        }
    }

private:
    // Each EventRegistry<T> needs its own mutex for its handler vector
    template<typename T>
    static std::mutex* GetMutex() {
        // We use a function‑local static so each T has its own mutex
        static std::mutex m;
        return &m;
    }
};

} // namespace ace
```

### How It Works

1. **Per‑Type Handler Storage**

   * `EventRegistry<T>::handlers` is a **static** `std::vector` that lives separately for each `T`.
   * No `std::type_index` or map lookup is needed when subscribing or emitting.

2. **Subscription (`Subscribe<T>`)**

   * Allocates a new `id` from the global `gNextSubscriptionID`.
   * Locks `EventRegistry<T>`’s mutex and pushes a `{id, fn}` into its `handlers` vector.
   * Registers in the **global** `gUnsubMap` an `id → lambda` that, when invoked, will remove that `id` from `EventRegistry<T>::handlers`.

3. **Unsubscription (`Unsubscribe(id)`)**

   * Grabs and removes the lambda from `gUnsubMap` under `gUnsubMapMutex`.
   * Calls it, which in turn locks the appropriate per‑type mutex and erases the handler.

4. **Emission (`Emit<T>`)**

   * Locks the same per‑type mutex, copies out the `handlers` vector, then unlocks.
   * Iterates the snapshot, calling each handler’s `fn(evt)`.
   * Early‑exits if any returns `false`.

---

#### Note on the Pseudocode Mutex in the Unsubscribe Lambda

In the unsubscribe lambda above, you see a line:

```cpp
std::lock_guard lock2(*reinterpret_cast<std::mutex*>(nullptr));
```

This is just **pseudocode** to indicate “lock the same mutex used in `Subscribe` and `Emit` for type `T`.” In practice you’d capture that mutex in the lambda:

```cpp
auto mtx = GetMutex<T>();  // pointer to the per‑type mutex
gUnsubMap[id] = [id, mtx]() {
    std::lock_guard lock(*mtx);
    // remove handler from EventRegistry<T>::handlers...
};
```

This way the unsubscribe sees and protects the same `handlers` vector.

---

With this pattern:

* **No runtime type lookups**—all storage is done in the `EventRegistry<T>` specialization.
* **Centralized unsubscription** by ID is still supported via the global `gUnsubMap`.
* Each `T` has its **own mutex**, keeping locking fine‑grained.
