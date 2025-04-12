/**
 * @file  ace.system.event_dispatcher.hpp
 * @brief Provides an interface for subscribing to, publishing and dispatching
 *        events.
 */

#pragma once
#include <ace.common.hpp>

namespace ace
{

    /**
     * @brief   Defines function intended to handle an event with the given
     *          `event_type`.
     * @tparam  event_type  The type of the event being handled.
     */
    template <typename event_type>
    using event_callback = std::function<void(const event_type&)>;

    /**
     * @brief   The `event_dispatcher` class provides an interface for subscribing
     *          to, publishing and dispatching events.
     */
    class ace_api event_dispatcher
    {
    public:

        /**
         * @brief   Subscribes the given event callback function to the given
         *          event type.
         * 
         * When an event with the given `event_type` is dispatched with the
         * `dispatch` method, one or more functions "subscribed" to that event
         * will be called, in the order in which they were subscribed.
         * 
         * @tparam  event_type  The type of the event being subscribed to.
         * @param   p_callback  The callback function subscribing to the event.
         */
        template <typename event_type>
        inline void subscribe (const event_callback<event_type>& p_callback)
        {
            // Lock down the dispatcher before subscribing the callback function.
            std::lock_guard<std::mutex> l_lock { m_mutex };

            // Retrieve the subscriber list, creating it if necessary.
            auto& l_list = get_or_create_subscriber_list<event_type>();

            // Emplace the callback function.
            l_list.callbacks.emplace_back(std::move(p_callback));
        }

        /**
         * @brief   Publishes an event with the given type to be dispatched
         *          later.
         * 
         * The event, once published, is not handled immediately. It will be
         * handled at the next call to the `dispatch` method.
         * 
         * @tparam  event_type  The type of the event being published.
         * @param   p_event     The event being published.
         */
        template <typename event_type>
        inline void publish (const event_type& p_event)
        {
            // Lock down the dispatcher before enqueueing the event.
            std::lock_guard<std::mutex> l_lock { m_mutex };

            m_queue.emplace(
                std::make_unique<event<event_type>>(p_event)
            );
        }

        /**
         * @brief   Dispatches all events published since the `event_dispatcher`
         *          was created, or since the last dispatch.
         * 
         * When the `event_dispatcher` is dispatched, all events which were
         * published with the `publish` method will be handled by calling all
         * callback functions subscribing to that event type.
         */
        inline void dispatch ()
        {
            // Create a temporary event queue.
            std::queue<std::unique_ptr<event_base>> l_queue_copy;

            // Lock down the dispatcher, but only to swap the dispatcher's event
            // queue with the temporary queue created above.
            {
                std::lock_guard<std::mutex> l_lock { m_mutex };
                std::swap(l_queue_copy, m_queue);
            }

            // Dequeue each published event and dispatch them.
            while (l_queue_copy.empty() == false)
            {
                auto& l_event = l_queue_copy.front();
                l_event->dispatch_to(*this);
                l_queue_copy.pop();
            }
        }

    private:

        /**
         * @brief The base struct for the typed event subscriber list.
         */
        struct subscriber_list_base
        {
            virtual ~subscriber_list_base () = default;
        };

        /**
         * @brief   A derived, templated structure containing a list of event
         *          callbacks for the given `event_type`.
         * @tparam  event_type  The type of the event the contained callbacks 
         *                      are handling.
         */
        template <typename event_type>
        struct subscriber_list : subscriber_list_base
        {
            std::vector<event_callback<event_type>> callbacks;
        };

        /**
         * @brief   The internal base struct for an event to be dispatched.
         */
        struct event_base
        {
            virtual ~event_base () = default;
            virtual void dispatch_to (event_dispatcher& p_dispatcher) = 0;
        };

        /**
         * @brief   A derived, templated structure representing an event to be
         *          dispatched.
         * @tparam  event_type  The type of the event to be dispatched.
         */
        template <typename event_type>
        struct event : event_base
        {
            event_type data;

        public:
            explicit event (const event_type& p_event) : data { p_event } {};
            inline void dispatch_to (event_dispatcher& p_dispatcher) override
            {
                p_dispatcher.dispatch_event(data);
            }
        };

        /**
         * @brief   Dispatches an event with the given type.
         * @tparam  The type of the event being dispatched.
         * @param   p_event A handle to the event being dispatched.
         */
        template <typename event_type>
        inline void dispatch_event (const event_type& p_event)
        {
            // Lock down the dispatcher.
            std::lock_guard<std::mutex> l_lock { m_mutex };

            // Seek out the appropriate subscriber list.
            auto l_iter = 
                m_subscribers.find(std::type_index(typeid(event_type)));

            // If the subscriber list exists, then iterate over each of the
            // subscribing functions and call them.
            if (l_iter != m_subscribers.end())
            {
                auto* l_list_ptr = static_cast<subscriber_list<event_type>*>(
                    l_iter->second.get()
                );
                for (auto& l_callback : l_list_ptr->callbacks)
                {
                    l_callback(p_event);
                }
            }
        }

        /**
         * @brief   Retrieves a list of subscriber callback functions for the
         *          given event type.
         * 
         * If the subscriber list could not be found, it will be created.
         * 
         * @tparam  event_type  The type of event whose subscriber list is to be
         *                      retrieved.
         */
        template <typename event_type>
        inline subscriber_list<event_type>& get_or_create_subscriber_list ()
        {

            // Seek out the proper subscriber list.
            auto l_type_index   = std::type_index(typeid(event_type));
            auto l_iter         = m_subscribers.find(l_type_index);
            
            // If the subscriber list could not be found, then create it here.
            if (l_iter == m_subscribers.end())
            {
                // Allocate the subscriber list.
                auto l_new_list =
                    std::make_unique<subscriber_list<event_type>>();

                // Get the raw pointer to that list.
                auto* l_raw_ptr = l_new_list.get();

                // Move the list into the map of subscribers.
                m_subscribers[l_type_index] = std::move(l_new_list);

                // De-reference the raw pointer, then return it.
                return *l_raw_ptr;
            }

            // Return the existing subscriber list.
            return *static_cast<subscriber_list<event_type>*>(
                l_iter->second.get()
            );
        }

    private:

        /**
         * @brief   A mutex is needed in order to publish and dispatch events in
         *          a thread-safe manner.
         */
        std::mutex m_mutex;

        /**
         * @brief   The map of subscribing callback functions.
         */
        std::unordered_map<
            std::type_index,
            std::unique_ptr<subscriber_list_base>
        > m_subscribers;

        /**
         * @brief   The queue of published events to be dispatched.
         */
        std::queue<std::unique_ptr<event_base>> m_queue;

    };

}
