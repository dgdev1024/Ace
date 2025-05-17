/**
 * @file    Ace/System/LoggerRenderSink.cpp
 */

#include <iostream>
#include <Ace/System/LoggerRenderSink.hpp>

namespace ace
{

    /* Constructors and Destructor ********************************************/

    LoggerRenderSink::LoggerRenderSink (
        const LoggerRenderSinkSpec& pSpec
    ) :
        mMaxHistory { pSpec.mMaxHistory }
    {

    }

    /* Public Methods *********************************************************/

    void LoggerRenderSink::Write (
        const LogEvent& pEvent
    )
    {
        mBuffer.Enqueue(pEvent);
    }

    void LoggerRenderSink::Render (
        const std::function<void(std::string_view)>&    pFunction
    )
    {
        // Format, then drain any new events into the history deque.
        while (auto lEventOptional = mBuffer.Dequeue())
        {
            // Format the event.
            std::string lFormatted = FormatEvent(*lEventOptional);

            // Under a lock, emplace the formatted string into the histroy
            // deque.
            {
                std::lock_guard lGuard { mHistoryMutex };
                mHistory.emplace_back(std::move(lFormatted));
                
                // If there are more entries than the maximum allowed, pop the
                // string in front.
                if (mHistory.size() > mMaxHistory)
                {
                    mHistory.pop_front();
                }
            }
        }

        // Under a lock, iterate over all strings in the history deque and call
        // the given render function on them, if one is provided.
        if (pFunction != nullptr)
        {
            std::lock_guard lGuard { mHistoryMutex };
            for (const auto& lMessage : mHistory)
            {
                pFunction(lMessage);
            }
        }
    }

    /* Private Methods ********************************************************/

    std::string LoggerRenderSink::FormatEvent (
        const LogEvent& pEvent
    )
    {
        std::ostringstream lStream;

        // Get the log event's timestamp and convert it into a time structure.
        auto    lTime = std::chrono::system_clock::to_time_t(pEvent.mTimestamp);
        std::tm lTimeStruct;
        #if defined(ACE_WINDOWS)
        
        #else
            localtime_r(&lTime, &lTimeStruct);
        #endif
        
        // Write the timestamp and level to the stream.
        lStream << '['
                << std::setfill('0') << std::setw(2) << lTimeStruct.tm_hour << ':'
                << std::setfill('0') << std::setw(2) << lTimeStruct.tm_min  << ':'
                << std::setfill('0') << std::setw(2) << lTimeStruct.tm_sec  << " | "
                << Logger::StringifyLevel(pEvent.mLevel) << "] ";

        // Non-distribute builds: Write the source location to the stream.
        #if !defined(ACE_DISTRIBUTE)
            lStream << pEvent.mFunction << ':'
                    << pEvent.mFile << ':'
                    << pEvent.mLine << " - ";
        #endif

        // Now output the message.
        lStream << pEvent.mMessage;
        return lStream.str();
    }

}
