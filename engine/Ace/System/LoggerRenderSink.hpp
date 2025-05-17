/**
 * @file    Ace/System/LoggerRenderSink.hpp
 * @brief   Provides a logger sink for writing output to a queue of strings to
 *          be rendered to an external source.
 */

#pragma once
#include <Ace/System/Logger.hpp>

namespace ace
{

    /**
     * @brief   A structure containing attributes which define a logger sink
     *          for writing to rendered output.
     */
    struct LoggerRenderSinkSpec
    {
        std::size_t mMaxHistory = 1000; ///< @brief The maximum number of log events which can be rendered at a time.
    };

    /**
     * @brief   Provides a logger sink used to writing messages into a circular
     *          buffer, which is forwarded into a queue of strings which are
     *          later rendered to an external source.
     */
    class ACE_API LoggerRenderSink final : public ILogSink
    {
    public:

        /**
         * @brief   The maximum capacity of the render sink's circular buffer.
         */
        static constexpr std::size_t RING_BUFFER_CAPACITY = (1 << 10);

    public:

        /**
         * @brief   The default constructor constructs a render sink with the
         *          given specification.
         * 
         * @param   pSpec   The render sink's specification.
         */
        LoggerRenderSink (
            const LoggerRenderSinkSpec& pSpec = {}
        );

    public:

        /**
         * @brief   Records the given log event to be enqueued for rendering.
         * 
         * @param   pEvent  The log event to be processed.
         */
        void Write (
            const LogEvent& pEvent
        ) override;

        /**
         * @brief   Formats all enqueued log events, then uses the given
         *          function to render the resultant strings.
         * 
         * @param   pFunction   The render function to use for rendering the
         *                      formatted strngs.
         */
        void Render (
            const std::function<void(std::string_view)>&    pFunction
        );

    private:

        /**
         * @brief   Generates a formatted string from the given log event.
         * 
         * @param   pEvent  The log event to format.
         * 
         * @return  The generated format string.
         */
        std::string FormatEvent (
            const LogEvent& pEvent
        );

    private:
        RingBuffer<LogEvent, RING_BUFFER_CAPACITY>  mBuffer;            ///< @brief The circular buffer containing pending log events.
        std::size_t                                 mMaxHistory;        ///< @brief The maximum number of log events which can be rendered at a time.
        std::deque<std::string>                     mHistory;           ///< @brief A double-ended queue containing formatted strings to be rendered.
        std::mutex                                  mHistoryMutex;      ///< @brief The mutex used to lock down the @a `mHistory` double-ended queue.

    };

}
