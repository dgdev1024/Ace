/**
 * @file    Ace/System/LoggerConsoleSink.hpp
 * @brief   Provides a logger sink for writing console output.
 */

#pragma once
#include <Ace/System/Logger.hpp>

namespace ace
{

    /**
     * @brief   An "enumeration" of ANSI color code strings.
     */
    namespace ConsoleColor
    {
        constexpr const char* RESET   = "\x1B[0m";
        constexpr const char* RED     = "\x1B[31m";
        constexpr const char* YELLOW  = "\x1B[33m";
        constexpr const char* GREEN   = "\x1B[32m";
        constexpr const char* CYAN    = "\x1B[36m";
        constexpr const char* MAGENTA = "\x1B[35m";
        constexpr const char* BLUE    = "\x1B[34m";
        constexpr const char* WHITE   = "\x1B[37m";
    }

    /**
     * @brief   Provides a logger sink for writing to `stdout` and `stderr`.
     */
    class ACE_API LoggerConsoleSink final : public ILogSink
    {
    public:

        /**
         * @brief   The default destructor.
         */
        virtual ~LoggerConsoleSink () override = default;

        /**
         * @brief   Writes a formatted message to either `stdout` or `stderr`.
         * 
         * Messages written by this method will be formatted as follows:
         * `[HH:MM:SS | LEVEL | THREAD_ID] FILE:FUNCTION:LINE - MESSAGE`
         * 
         * @param   pEvent  The log event containing the message to write.
         */
        void Write (const LogEvent& pEvent) override;

    };  

}
