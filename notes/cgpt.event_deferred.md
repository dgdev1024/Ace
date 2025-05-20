# Prompt

“Extend my `EventBus` to add `Publish<T>(evt)` which enqueues events into a 
lock‑free MPSC queue, and `ProcessDeferred()` to drain and dispatch all deferred 
events.”

# Response

Here’s an extension of your `ace::EventBus` that adds:

* **`Publish<T>(evt)`**: enqueues a deferred dispatch into a lock‑free MPSC queue.
* **`ProcessDeferred()`**: to be called on your main thread, drains that queue and invokes each deferred event.

We’ll use our `MpscRingBuffer<std::function<void()>, 1<<14>` to hold arbitrary deferred tasks.

```cpp
#pragma once

#include <functional>
#include <mutex>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>
#include <memory>
#include <atomic>
#include <algorithm>
#include "ring_buffer.hpp"  // your MpscRingBuffer<T,Capacity>

namespace ace {

class ACE_API EventBus final {
public:
    // … (Subscribe, Unsubscribe, Emit as before) …

    /**
     * @brief   Publishes an event of type T for deferred handling.
     *          The wrapped handlers will be invoked when ProcessDeferred() is called.
     */
    template<typename T>
    static void Publish(const T& pEvent) {
        // Capture a copy of the handlers and the event
        std::vector<std::shared_ptr<BaseEventHandler>> handlersCopy;
        {
            std::lock_guard lock(sHandlersMutex);
            auto it = sHandlers.find(std::type_index(typeid(T)));
            if (it == sHandlers.end()) return;
            handlersCopy = it->second;
        }

        // Create a deferred task
        auto task = [handlers = std::move(handlersCopy), evt = pEvent]() mutable {
            for (auto& basePtr : handlers) {
                auto derived = static_cast<EventHandler<T>*>(basePtr.get());
                if (!derived->mFunction(evt))
                    break;
            }
        };

        // Enqueue; if full, drop the task
        (void)sDeferredQueue.enqueue(std::move(task));
    }

    /**
     * @brief   Call on your main/update thread to drain and execute all deferred events.
     */
    static void ProcessDeferred() {
        while (auto taskOpt = sDeferredQueue.dequeue()) {
            (*taskOpt)();
        }
    }

private:
    // … (BaseEventHandler, EventHandler<T>, sHandlers, sHandlersMutex, sNextID) …

    // lock‑free queue of deferred tasks
    static inline MpscRingBuffer<std::function<void()>, 1 << 14> sDeferredQueue;
};

} // namespace ace
```

### What’s Happening

1. **`Publish<T>(pEvent)`**

   * Grabs a **snapshot** of the current subscribers for `T` under the same mutex protection as `Emit`.
   * Creates a **lambda** (`std::function<void()>`) that, when later called, will:

     * Cast each `BaseEventHandler` back to `EventHandler<T>*`.
     * Invoke its `mFunction(evt)`, stopping early if any handler returns `false`.
   * **Enqueues** that lambda into `sDeferredQueue`. If the queue is full, the event is dropped.

2. **`ProcessDeferred()`**

   * On your main thread (or whichever thread you choose), repeatedly **dequeues** each deferred task and calls it.
   * Since `MpscRingBuffer` is **single‑consumer**, only one thread should call `ProcessDeferred()`.

---

#### Integration

* Call `ace::EventBus::Publish(yourEvent)` from any thread to defer.
* Once per frame (e.g. in your game loop), call:

  ```cpp
  ace::EventBus::ProcessDeferred();
  ```

  to dispatch all pending deferred events in order.

This pattern cleanly separates immediate dispatch (`Emit`) from cross‑thread, deferred dispatch (`Publish` + `ProcessDeferred`) using a lock‑free queue.
