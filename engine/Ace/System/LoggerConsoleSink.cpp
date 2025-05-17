/**
 * @file    Ace/System/LoggerConsoleSink.cpp
 */

#include <iostream>
#include <sstream>
#include <iomanip>
#include <Ace/System/LoggerConsoleSink.hpp>

namespace ace
{

    /* Static Functions *******************************************************/

    namespace
    {

        static constexpr const char* ResolveConsoleColor (const LogLevel& pLevel)
        {
            switch (pLevel)
            {
                case LogLevel::TRACE:       return ConsoleColor::RESET;
                case LogLevel::DEBUG:       return ConsoleColor::WHITE;
                case LogLevel::INFO:        return ConsoleColor::CYAN;
                case LogLevel::WARNING:     return ConsoleColor::YELLOW;
                case LogLevel::ERROR:       return ConsoleColor::RED;
                case LogLevel::CRITICAL:    return ConsoleColor::MAGENTA;
                default:                    return ConsoleColor::RESET; 
            }
        }

    }

    /* Public Methods *********************************************************/

    void LoggerConsoleSink::Write (const LogEvent& pEvent)
    {
        std::ostringstream lStream;

        // Get the log event's timestamp and convert it into a time structure.
        std::time_t lTime       = std::chrono::system_clock::to_time_t(pEvent.mTimestamp);
        std::tm     lTimeStruct;
        #if defined(ACE_WINDOWS)
        
        #else
            localtime_r(&lTime, &lTimeStruct);
        #endif
        
        // Colorize the log message, then write the timestamp, level and thread
        // ID to the stream.
        lStream << ResolveConsoleColor(pEvent.mLevel)
                << '['
                << std::setfill('0') << std::setw(2) << lTimeStruct.tm_hour << ':'
                << std::setfill('0') << std::setw(2) << lTimeStruct.tm_min  << ':'
                << std::setfill('0') << std::setw(2) << lTimeStruct.tm_sec  << " | "
                << Logger::StringifyLevel(pEvent.mLevel) << " | "
                << pEvent.mThreadID << "] ";

        // Non-distribute builds: Write the source location to the stream.
        #if !defined(ACE_DISTRIBUTE)
            lStream << ConsoleColor::RESET
                    << pEvent.mFunction << ':'
                    << pEvent.mFile << ':'
                    << pEvent.mLine << " - ";
        #endif

        // Now output the message.
        lStream << ConsoleColor::WHITE
                << pEvent.mMessage
                << ConsoleColor::RESET;

        // Emit to `stdout` or `stderr` depending on severity.
        switch (pEvent.mLevel)
        {
            case LogLevel::WARNING:
            case LogLevel::ERROR:
            case LogLevel::CRITICAL:
                std::cerr << lStream.str() << '\n';
                break;
            default:
                std::cout << lStream.str() << '\n';
                break;
        }
        
    }

}
