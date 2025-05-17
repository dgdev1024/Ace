/**
 * @file    Ace/System/Logger.hpp
 * @brief   Provides a static class for thread-safe logging.
 */

#pragma once
#include <Ace/System/RingBuffer.hpp>

namespace ace
{

    /**
     * @brief   Enumerates the logger's severity levels.
     */
    enum class LogLevel
    {
        TRACE,      ///< @brief Log level for verbose debugging messages.
        DEBUG,      ///< @brief Log level for debugging messages.
        INFO,       ///< @brief Log level for informational messages.
        WARNING,    ///< @brief Log level for informational messages which need the user's attention.
        ERROR,      ///< @brief Log level for error messages.
        CRITICAL    ///< @brief Log level for exceptions and critical error messages.
    };

    /**
     * @brief   A structure containing a single log event.
     */
    struct LogEvent final
    {
        std::chrono::system_clock::time_point   mTimestamp;             ///< @brief Contains the time this log event was produced.
        std::thread::id                         mThreadID;              ///< @brief The ID of the `std::thread` which produced this log event.
        const char*                             mFunction = nullptr;    ///< @brief The name of the function in which this log event was produced.
        const char*                             mFile = nullptr;        ///< @brief The name of the source file in which this log event was produced.
        std::int32_t                            mLine = 0;              ///< @brief The line in the source file in which this log event was produced.
        LogLevel                                mLevel;                 ///< @brief The log event's severity level.
        std::string                             mMessage = "";          ///< @brief The formatted message which is to be logged.
    };

    /**
     * @brief   An interface class for writing log messages to a source.
     */
    class ACE_API ILogSink
    {
    public:

        /**
         * @brief   The virtual destructor.
         */
        virtual ~ILogSink () = default;

        /**
         * @brief   Writes the message in the given @a `LogEvent` to this sink's
         *          source
         * 
         * @param   pEvent  A handle to the event to be processed.
         */
        virtual void Write (
            const LogEvent& pEvent
        ) = 0;
        
    };

    /**
     * @brief   Provides a static class for logging messages.
     */
    class ACE_API Logger final
    {
    public:

        /**
         * @brief   The maximum capacity of the logger's ring buffer.
         */
        static constexpr std::size_t MAX_CAPACITY = 1 << 10;

    public:

        /**
         * @brief   Initializes the @a `Logger` and starts its background worker
         *          thread.
         */
        static void Initialize ();

        /**
         * @brief   Shuts down the @a `Logger`, flushing any pending log events
         *          and stopping its background worker thread.
         */
        static void Shutdown ();

        /**
         * @brief   Registers a new log sink, into which log events can be
         *          dispatched.
         * 
         * @param   pSink   An `std::shared_ptr` to the log sink to register.
         */
        static void RegisterSink (
            std::shared_ptr<ILogSink> pSink
        );

        /**
         * @brief   Publishes a new log event to be dispatched.
         * 
         * @param   pLevel      The event's log severity level.
         * @param   pFunction   The name of the function producing the log event.
         * @param   pFile       The source file producing the log event.
         * @param   pLine       The line in the source file producing the log event.
         * @param   pMessage    The message string to be logged.
         * 
         * @note    This method is not intended to be used directly by the
         *          client application. Use the `ACE_LOG` and `ACE_LOG_*` macros
         *          instead.
         */
        static void Publish (
            LogLevel            pLevel,
            const char*         pFunction,
            const char*         pFile,
            std::int32_t        pLine,
            std::string         pMessage
        );

    public:

        /**
         * @brief   Publishes a new log event with a formatted string to be
         *          dispatched.
         * 
         * @tparam  T           The types of the variadic arguments passed in.
         * 
         * @param   pLevel      The event's log severity level.
         * @param   pFunction   The name of the function producing the log event.
         * @param   pFile       The source file producing the log event.
         * @param   pLine       The line in the source file producing the log event.
         * @param   pFormat     A string to be formatted.
         * @param   pArgs       The variadic arguments to format the string with.
         * 
         * @note    This method is not intended to be used directly by the
         *          client application. Use the `ACE_LOG` and `ACE_LOG_*` macros
         *          instead.
         */
        template <typename... Ts>
        static void Publish (
            LogLevel            pLevel,
            const char*         pFunction,
            const char*         pFile,
            std::int32_t        pLine,
            std::string_view    pFormat,
            Ts&&...             pArgs
        )
        {
            Publish(
                pLevel,
                pFunction,
                pFile,
                pLine,
                std::vformat(
                    pFormat,
                    std::make_format_args(pArgs...)
                )
            );
        }

        /**
         * @brief   Retrieves a string representation of the given log severity
         *          level.
         * 
         * @param   pLevel      The log severity level enumeration to stringify.
         * 
         * @return  The log level's string representation.
         */
        inline static constexpr const char* StringifyLevel (
            LogLevel    pLevel
        )
        {
            switch (pLevel)
            {
                case LogLevel::TRACE:       return "TRACE";
                case LogLevel::DEBUG:       return "DEBUG";
                case LogLevel::INFO:        return "INFO";
                case LogLevel::WARNING:     return "WARNING";
                case LogLevel::ERROR:       return "ERROR";
                case LogLevel::CRITICAL:    return "CRITICAL";
                default:                    return "???";
            }
        }

    private:

        /**
         * @brief   Processes the @a `Logger`'s log event queue, dispatching
         *          each event into its registered sinks.
         */
        static void ProcessQueue ();

        /**
         * @brief   Dispatches a log event, writing its formatted message to all
         *          registered event sinks.
         * 
         * @param   pEvent  A handle to the log event being dispatched.
         */
        static void Dispatch (
            const LogEvent& pEvent
        );

    private:
        static std::atomic<bool>                        sRunning;       ///< @brief Has the logger been initialized?
        static std::thread                              sWorkerThread;  ///< @brief The background worker thread responsible for processing the log event queue.
        static RingBuffer<LogEvent, MAX_CAPACITY>       sQueue;         ///< @brief The circular queue of log events.
        static std::mutex                               sSinksMutex;    ///< @brief The mutex used to lock down the sinks container.
        static std::vector<std::shared_ptr<ILogSink>>   sSinks;         ///< @brief The container of log sinks.

    };

}

#define ACE_LOG(pLevel, ...)            ::ace::Logger::Publish(pLevel, __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__)
#define ACE_LOG_CRITICAL(...)           ACE_LOG(ace::LogLevel::CRITICAL,    __VA_ARGS__)
#define ACE_LOG_ERROR(...)              ACE_LOG(ace::LogLevel::ERROR,       __VA_ARGS__)
#define ACE_LOG_WARNING(...)            ACE_LOG(ace::LogLevel::WARNING,     __VA_ARGS__)
#define ACE_LOG_INFO(...)               ACE_LOG(ace::LogLevel::INFO,        __VA_ARGS__)

#if defined(ACE_DEBUG)
    #define ACE_LOG_DEBUG(...)          ACE_LOG(ace::LogLevel::DEBUG,       __VA_ARGS__)
    #define ACE_LOG_TRACE(...)          ACE_LOG(ace::LogLevel::TRACE,       __VA_ARGS__)
#else
    #define ACE_LOG_DEBUG(...)
    #define ACE_LOG_TRACE(...)
#endif
