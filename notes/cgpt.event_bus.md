# Prompt

“Generate a header‑only EventBus that lets me Subscribe<WindowResized>(handler), Emit(evt), and Unsubscribe(id) using std::unordered_map<type_index, vector<Handler>>.”

# Response

Here’s a **header‑only** `EventBus` implementation. It supports:

* **Subscribe\<T>(handler)** → returns a **SubscriptionID** (an integer).
* **Unsubscribe(id)** → removes that subscription.
* **Emit(evt)** → immediately invokes all handlers for `T`.

It uses `std::unordered_map<std::type_index, std::vector<Handler>>` under the hood.

```cpp
#pragma once

#include <functional>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <atomic>

namespace Ace {

using SubscriptionID = std::size_t;

class EventBus {
public:
    /// Subscribe to events of type T. Returns an ID which can be used to unsubscribe.
    template<typename T>
    static SubscriptionID Subscribe(std::function<void(const T&)> handler) {
        std::lock_guard lock(s_mutex);

        auto& entry = s_handlers[std::type_index(typeid(T))];
        SubscriptionID id = s_nextId++;
        entry.emplace_back(Handler{
            id,
            [fn = std::move(handler)](const BaseEvent& base) {
                // safe downcast because we only store T here
                fn(static_cast<const T&>(base));
            }
        });
        return id;
    }

    /// Unsubscribe a previously registered handler.
    static void Unsubscribe(SubscriptionID id) {
        std::lock_guard lock(s_mutex);
        for (auto it = s_handlers.begin(); it != s_handlers.end(); ++it) {
            auto& vec = it->second;
            vec.erase(
                std::remove_if(vec.begin(), vec.end(),
                    [id](auto& h){ return h.id == id; }),
                vec.end()
            );
            // Optionally erase empty vectors to free memory:
            if (vec.empty()) {
                s_handlers.erase(it);
                break;
            }
        }
    }

    /// Emit an event of type T immediately to all subscribers.
    template<typename T>
    static void Emit(const T& evt) {
        std::vector<Handler> copy;
        {
            std::lock_guard lock(s_mutex);
            auto it = s_handlers.find(std::type_index(typeid(T)));
            if (it == s_handlers.end()) return;
            copy = it->second;  // copy handlers to invoke outside lock
        }
        for (auto& h : copy) {
            h.callback(evt);
        }
    }

private:
    // A non‑polymorphic base for type‑erased dispatch
    struct BaseEvent {};

    struct Handler {
        SubscriptionID             id;
        std::function<void(const BaseEvent&)> callback;
    };

    // Map from event‑type to list of handlers
    static inline std::unordered_map<std::type_index, std::vector<Handler>> s_handlers;

    // Protects s_handlers
    static inline std::mutex s_mutex;

    // Generates unique subscription IDs
    static inline std::atomic<SubscriptionID> s_nextId{1};
};

} // namespace Ace
```

### Usage Example

```cpp
#include "EventBus.hpp"

struct WindowResized {
    int width, height;
};

int main() {
    using namespace Ace;

    // Subscribe
    SubscriptionID sub = EventBus::Subscribe<WindowResized>(
        [](const WindowResized& e) {
            printf("Window resized to %dx%d\n", e.width, e.height);
        });

    // Emit
    EventBus::Emit(WindowResized{1280, 720});

    // Unsubscribe
    EventBus::Unsubscribe(sub);

    // Emitting now does nothing
    EventBus::Emit(WindowResized{800, 600});
}
```

---

**Notes & Extensions**

* We copy the handler list before invocation to allow handlers to unsubscribe themselves safely.
* This is **not** yet optimized for high‑frequency or lock‑free deferred dispatch—just simple, type‑safe immediate dispatch.
* In future iterations you can add:

  * Priorities, one‑shot handlers, deferred publishing via a lock‑free queue, and automatic weak‑ref unsubscription.
