/**
 * @file    Ace/System/EventBus.hpp
 * @brief   Provides a centralized interface for subscribing to, publishing and dispatching events.
 */

#pragma once
#include <Ace/Common.hpp>

namespace ace
{

    /**
     * @typedef `ace::EventHandler`
     * @brief   Defines a function which is called when an event of type `T` is dispatched.
     * 
     * @tparam  T   The type of event being dispatched.
     */
    template <typename T>
    using EventHandler = std::function<bool(const T&)>;

    /**
     * @class   `ace::EventBus`
     * @brief   A static class providing a centralized interface for subscribing to, publishing and
     *          dispatching events.
     */
    class ACE_API EventBus final
    {
    public:

        /**
         * @fn      `ace::EventBus::subscribe`
         * @brief   Subscribes an @a `EventHandler` function to respond to events of type `T` when
         *          dispatched.
         * 
         * @tparam  T   The type of event which the @a `EventHandler` is subscribing to.
         * 
         * @param   pCallback   The @a `EventHandler` function to be called to respond to the event.
         * 
         * @return  A handler ID coresponding to the callback function.
         */
        template <typename T>
        inline static std::size_t subscribe (const EventHandler<T>& pCallback)
        {   
            std::unique_lock lLock { getHandlersMutex<T>() };

            auto& lHandlers = getHandlers<T>();
            lHandlers.push_back(std::move(pCallback));
            return lHandlers.size() - 1;
        }

        /**
         * @fn      `ace::EventBus::unsubscribe`
         * @brief   Unsubscribes an event callback with the given handler ID, so that it no longer
         *          responds to events of that type.
         * 
         * @tparam  T   The type of event which the event callback was subscribing to.
         * 
         * @param   pHandlerId  The handler ID associated with the callback to unsubscribe from.
         * 
         * @note    The unsubscribed callback will still be called in response to any events of type
         *          `T` which were published before this call to @a `unsubscribe` and before the next 
         *          call to @a `dispatch`.
         */
        template <typename T>
        inline static void unsubscribe (const std::size_t pHandlerId)
        {
            std::unique_lock lLock { getHandlersMutex<T>() };

            auto& lHandlers = getHandlers<T>();
            if (pHandlerId < lHandlers.size())
            {
                lHandlers[pHandlerId] = std::move(lHandlers.back());
                lHandlers.pop_back();
            }
        }

        /**
         * @fn      `ace::EventBus::publish`
         * @brief   Publishes an event of type `T` to be dispatched later, constructing the event
         *          in-place with the given arguments.
         * 
         * @tparam  T   The type of event being published.
         * @tparam  As  The types of the arguments to construct the event in-place with.
         * 
         * @param   pArgs   The arguments passed into the event object's constructor.
         */
        template <typename T, typename... As>
        inline static void publish (As&&... pArgs)
        {

            // Construct the event in-place.
            T lEvent { std::forward<As>(pArgs)... };

            // Get a snapshot of the appropriate handlers array. A lock is needed only for this.
            std::vector<EventHandler<T>> lSnapshot;
            {
                std::shared_lock lLock { getHandlersMutex<T>() };
                lSnapshot = getHandlers<T>();
            }

            // Enqueue all event handler callbacks.
            enqueueAll(lSnapshot, lEvent);
        }

        /**
         * @fn      `ace::EventBus::dispatch`
         * @brief   Dispatches all published events, calling all subscribed callbacks responding to
         *          each respective event.
         */
        inline static void dispatch ()
        {

            // Get and clear the event queue under a lock. This is done via a swap.
            std::vector<std::function<bool()>> lQueue;
            {
                std::lock_guard lLock { getEventQueueMutex() };
                lQueue.swap(getEventQueue());
            }

            // Run each event callback function.
            for (auto lIter = lQueue.rbegin(); lIter != lQueue.rend(); ++lIter)
            {
                if ((*lIter)() == true)
                {
                    break;
                }
            }

        }

    private:

        /**
         * @fn      `ace::EventBus::enqueueAll`
         * @brief   Enqueues a batch of callables into the event queue.
         * 
         * @tparam  T   The type of event the callables are responding to.
         * @tparam  U   The type of callable being enqueued.
         * 
         * @param   pSnapshot   A snapshot of the list of callables being enqueued.
         * @param   pEvent      The event being handled.
         */
        template <typename T, typename U>
        inline static void enqueueAll (const std::vector<U>& pSnapshot, const T& pEvent)
        {

            // Get a handle to the event queue, then lock it down.
            auto& lEventQueue = getEventQueue();
            std::lock_guard lEventQueueLock { getEventQueueMutex() };

            // Reserve space in the event queue for calls to the event's subscribed handlers.
            lEventQueue.reserve(lEventQueue.size() + pSnapshot.size());

            // Iterate over the event's handlers. Add functions which call these handlers to the
            // event queue.
            for (auto& lHandler : pSnapshot)
            {
                lEventQueue.emplace_back(
                    [lHandler, pEvent] () mutable
                    {
                        return lHandler(pEvent);
                    }
                );
            }
        }

        /**
         * @fn      `ace::EventBus::getHandlers`
         * @brief   Retrieves a static list of event callback functions responding to events of
         *          type `T`.
         * 
         * @return  The static list of event callback functions.
         */
        template <typename T>
        inline static std::vector<EventHandler<T>>& getHandlers ()
        {
            static std::vector<EventHandler<T>> sInst;
            return sInst;
        }

        /**
         * @fn      `ace::EventBus::getEventQueue`
         * @brief   Retrieves a static handle to the event bus's event queue.
         * 
         * @return  A handle to the event queue.
         */
        inline static std::vector<std::function<bool()>>& getEventQueue ()
        {
            static std::vector<std::function<bool()>> sInst;
            return sInst;
        }

        /**
         * @fn      `ace::EventBus::getHandlersMutex`
         * @brief   Retrieves a static mutex used to lock down an event handlers list.
         * 
         * @tparam  T   The type of events whose handlers list needs to be locked down.
         * 
         * @return  A handle to the mutex.
         */
        template <typename T>
        inline static std::shared_mutex& getHandlersMutex ()
        {
            static std::shared_mutex sMutex;
            return sMutex;
        }

        /**
         * @fn      `ace::EventBus::getEventQueueMutex`
         * @brief   Retrieves a static mutex used to lock down the event queue.
         * 
         * @return  A handle to the mutex.
         */
        inline static std::mutex& getEventQueueMutex ()
        {
            static std::mutex sMutex;
            return sMutex;
        }
        
    };

}

// "9 To 5" - Dolly Parton, 1980
// "Need A Favor" - Jelly Roll, 2023
// "Runnin' Down A Dream" (Tom Petty cover) - Luke Combs, 2024
// "If You're Gonna Play in Texas (You Gotta Have a Fiddle in the Band)" - Alabama, 1984
// "Beer For My Horses" - Toby Keith ft. Willie Nelson, 2002
// "Things You Can't Live With" - Chris Janson ft. Travis Tritt, 2022
// "Song of the South" - Alabama, 1988
// "The Lost" - Jelly Roll, 2023
// "It's a Long Way to the Top (If You Wanna Rock 'N' Roll)" (AC/DC cover) - Cody Jinks, 2025
// "What's Wrong With Me" - Jelly Roll, 2024
// "Son of a Sinner" - Jelly Roll, 2021
// "Piano Man" - Billy Joel, 1973
