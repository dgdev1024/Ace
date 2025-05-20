/**
 * @file    Ace/System/EventBus.hpp
 * @brief   Provides a static class used for subscribing to and emitting events.
 */

#pragma once
#include <Ace/System/RingBuffer.hpp>

namespace ace
{

    /**
     * @brief   A static class used for subscribing to, publishing, emitting and
     *          dispatching events of any type.
     */
    class ACE_API EventBus final
    {
    public:

        /**
         * @brief   Subscribes the given function to events of type `T` with
         *          the given priority level.
         * 
         * The subscribed function will be called in response to emitted and
         * dispatched events of the given type. These subscriber functions will
         * be called in descending order of priority. Event handlers with higher
         * priorities are called first, with ties broken by the order in which
         * they are subscribed.
         * 
         * @tparam  T           The type of event being handled.
         * 
         * @param   pFunction   The handler function to be subscribed.
         * @param   pPriority   The event handler's priority level.
         * 
         * @returns The event handler's assigned subscription ID.
         */
        template <typename T>
        static std::size_t Subscribe (
            std::function<bool(const T&)>   pFunction,
            const std::size_t&              pPriority = 0
        )
        {
            // Perform event subscription under a lock.
            std::lock_guard lGuard { sHandlersMutex };

            // Create the new handler.
            auto lHandler = std::make_shared<EventHandler<T>>(
                sNextID.fetch_add(1),
                std::move(pFunction),
                pPriority
            );
            
            // Retrieve the appropriate event handler list, creating it if
            // necessary, then insert the event handler in descending order
            // of priority.
            auto& lList = sHandlers[ACE_TYPEID(T)];
            auto lIter =
                std::find_if(
                    lList.begin(),
                    lList.end(),
                    [&] (auto& lBaseEvent)
                    {
                        return
                            static_cast<EventHandler<T>*>(lBaseEvent.get())
                                ->mPriority < pPriority;
                    }
                );
            lList.insert(lIter, lHandler);

            // Return the handler's subscription ID.
            return lHandler->mID;
        }

        /**
         * @brief   Unsubscribes a previously-subscribed event handler mapped to
         *          the given subscription ID.
         * 
         * @param   pID     The subscription ID assigned to the event handler
         *                  to be unsubscribed.
         */
        static void Unsubscribe (
            const std::size_t&  pID
        )
        {
            // Perform event handler unsubscription under a lock.
            std::lock_guard lGuard { sHandlersMutex };
            
            // Iterate over the event lists.
            for (
                auto lIter = sHandlers.begin();
                lIter != sHandlers.end();
                ++lIter
            )
            {
                // Get a handle to the event. If there is an event handler with
                // the provided subscription ID in that list, erase it.
                auto& lEventList = lIter->second;
                auto  lErased = std::erase_if(
                    lEventList,
                    [pID] (auto& lBaseEvent)
                    {
                        return lBaseEvent->mID == pID;
                    }
                );

                // If `lErased` is non-zero, then there was an event handler
                // erased from this list (exactly one, as event handlers are
                // assigned unique subscription IDs) - early out.
                if (lErased != 0)
                {
                    // If the event list is empty as a result, erase it, too.
                    if (lEventList.empty() == true)
                    {
                        sHandlers.erase(lIter);
                    }

                    return;
                }
            }
        }

        /**
         * @brief   Emits an event of type `T`.
         * 
         * Emitted events are handled immediately by all subscribers.
         * 
         * @tparam  T       The type of event being emitted.
         * 
         * @param   pEvent  A handle to the event being emitted.
         */
        template <typename T>
        static void Emit (
            const T&    pEvent
        )
        {
            // Keep a copy of the appropriate handler list. Obtain that list
            // under a lock.
            EventList lListCopy;
            {
                std::lock_guard lGuard { sHandlersMutex };

                auto lIter = sHandlers.find(ACE_TYPEID(T));
                if (lIter == sHandlers.end())
                {
                    return;
                }

                lListCopy = lIter->second;
            }

            // Iterate over the list copy outside of the lock.
            for (auto& lBaseEvent : lListCopy)
            {
                // Cast to `EventHandler<T>` and invoke. If invocation returns
                // true, then the event is consumed - stop propagation.
                auto lDerivedEvent = 
                    static_cast<EventHandler<T>*>(lBaseEvent.get());
                if (lDerivedEvent->mFunction(pEvent) == true)
                {
                    break;
                }
            }
        }

        /**
         * @brief   Publishes an event of type `T`.
         * 
         * Published events are deferred until the next call to
         * @a `EventBus::Dispatch`, at which point dispatcher functions will
         * run the respective event handlers.
         * 
         * @tparam  T       The type of event being published.
         * 
         * @param   pEvent  A handle to the event being published.
         */
        template <typename T>
        static void Publish (const T& pEvent)
        {
            // Keep a copy of the appropriate handler list. Obtain that list
            // under a lock.
            EventList lListCopy;
            {
                std::lock_guard lGuard { sHandlersMutex };

                auto lIter = sHandlers.find(ACE_TYPEID(T));
                if (lIter == sHandlers.end())
                {
                    return;
                }

                lListCopy = lIter->second;
            }

            // A dispatcher function will call the event's handler when the
            // published event is dispatched. Create that function here.
            auto lDispatcher =
                [lList = std::move(lListCopy), lEvent = pEvent] () mutable
                {
                    for (auto& lBaseEvent : lList)
                    {
                        auto lDerivedEvent = 
                            static_cast<EventHandler<T>*>(lBaseEvent.get());
                        if (lDerivedEvent->mFunction(lEvent) == true)
                        {
                            break;
                        }
                    }
                };

            // Attempt to enqueue the dispatcher function. Drop it if it fails.
            sEventBuffer.Enqueue(std::move(lDispatcher));
        }

        /**
         * @brief   Dispatches all published events, calling their event handlers.
         */
        static void Dispatch ()
        {
            while (auto lPublishedEvent = sEventBuffer.Dequeue())
            {
                (*lPublishedEvent)();
            }
        }

    private:

        /**
         * @brief   The maximum number of events which can be published.
         */
        static constexpr std::size_t MAX_PUBLISHED_EVENTS = (1 << 10);

        /**
         * @brief   The base structure for the event bus's templated event
         *          handler structures.
         */
        struct BaseEventHandler
        {
            std::size_t mID;            ///< @brief The event handler's assigned subscription ID.
            std::size_t mPriority = 0;  ///< @brief The event's priority level. Events with higher priority levels are handled first.

        public:
            explicit BaseEventHandler (
                const std::size_t& pID,
                const std::size_t& pPriority
            ) :
                mID         { pID },
                mPriority   { pPriority }
            {}
            virtual ~BaseEventHandler () = default;

        };

        /**
         * @brief   A templated structure containing a function called in
         *          response to an emitted or dispatched event.
         * 
         * @tparam  T   The type of event being handled.
         */
        template <typename T>
        struct EventHandler : BaseEventHandler
        {
            std::function<bool(const T&)> mFunction;

        public:
            EventHandler (
                const std::size_t&              pID,
                std::function<bool(const T&)>   pFunction,
                const std::size_t&              pPriority
            ) :
                BaseEventHandler    { pID, pPriority },
                mFunction           { std::move(pFunction) }
            {}

        };

        /**
         * @brief   Defines a list of event handler structures.
         */
        using EventList = std::vector<
            std::shared_ptr<BaseEventHandler>
        >;

        /**
         * @brief   Defines a map of lists of event handler structures mapped
         *          to type indices.
         */
        using EventMap = std::unordered_map<
            std::type_index,
            EventList
        >;

        /**
         * @brief   Defines a ring buffer of void functions used to handle
         *          published events.
         */
        using EventBuffer = RingBuffer<
            std::function<void()>,
            MAX_PUBLISHED_EVENTS
        >;

    private:
        static inline EventBuffer               sEventBuffer {};    ///< @brief The ring buffer of published events to be handled later.
        static inline EventMap                  sHandlers;          ///< @brief The map of event handlers.
        static inline std::mutex                sHandlersMutex;     ///< @brief The mutex used to lock down the event handlers map.
        static inline std::atomic<std::size_t>  sNextID { 1 };      ///< @brief The subscription ID to be assigned to the next event handler.
        

    };

}
