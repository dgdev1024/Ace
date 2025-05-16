/**
 * @file    Ace/System/Logger.cpp
 */

#include <iostream>
#include <Ace/System/Logger.hpp>

namespace ace
{

    /* Static Members *********************************************************/

    std::atomic<bool>                           Logger::sRunning { false };     
    std::thread                                 Logger::sWorkerThread;
    RingBuffer<LogEvent, Logger::MAX_CAPACITY>  Logger::sQueue;    
    std::mutex                                  Logger::sSinksMutex;  
    std::vector<std::shared_ptr<ILogSink>>      Logger::sSinks;       

    /* Public Methods *********************************************************/

    void Logger::Initialize ()
    {
        // `std::atomic::exchange` returns the atomic's value before that value
        // is changed to whatever is passed into the function.
        //
        // If this call to `exchange` returns `true`, then that means the logger
        // has already been initialized - early out.
        if (sRunning.exchange(true) == true)
        {
            return;
        }

        // Start the logger's background worker thread, and have it process the
        // log event queue when needed.
        sWorkerThread = std::thread { ProcessQueue };
    }

    void Logger::Shutdown ()
    {
        // If the logger has already been shut down - or not yet initialized,
        // then early out.
        if (sRunning.exchange(false) == false)
        {
            return;
        }

        // Join (stop) the logger's background worker thread, if allowed.
        if (sWorkerThread.joinable() == true)
        {
            sWorkerThread.join();
        }
    }

    void Logger::RegisterSink (
        std::shared_ptr<ILogSink> pSink
    )
    {
        // Under a lock, move the new sink into the sinks container.
        std::lock_guard lGuard { sSinksMutex };
        sSinks.emplace_back(std::move(pSink));
    }

    void Logger::Publish (
        LogLevel            pLevel,
        const char*         pFunction,
        const char*         pFile,
        std::int32_t        pLine,
        std::string         pMessage
    )
    {
        // If the logging subsystem has not yet been initialized, initialize
        // it now.
        if (sRunning.load() == false)
        {
            Initialize();
        }

        // Create the log event.
        LogEvent lEvent {
            .mTimestamp     = std::chrono::system_clock::now(),
            .mThreadID      = std::this_thread::get_id(),
            .mFunction      = (pFunction != nullptr) ? pFunction : "",
            .mFile          = (pFile != nullptr) ? pFile : "",
            .mLine          = pLine,
            .mMessage       = std::move(pMessage)
        };

        // Enqueue the event.
        sQueue.enqueue(lEvent);
    }

    /* Private Methods ********************************************************/

    void Logger::ProcessQueue ()
    {
        // While the logging subsystem is still running, dispatch any log
        // events as they become available.
        while (sRunning.load(std::memory_order_acquire) == true)
        {
            while (auto lEvent = sQueue.dequeue())
            {
                Dispatch(*lEvent);
            }

            // Yield the thread to reduce CPU spin.
            std::this_thread::yield();
        }

        // When the logging subsystem shuts down, dispatch any outstanding log
        // events.
        while (auto lEvent = sQueue.dequeue())
        {
            Dispatch(*lEvent);
        }
    }

    void Logger::Dispatch (
        const LogEvent& pEvent
    )
    {
        // Under a lock, iterate over all registered sinks and write this log
        // event's formatted message to each sink.
        std::lock_guard lGuard { sSinksMutex };
        for (auto& lSink : sSinks)
        {
            lSink->Write(pEvent);
        }
    }

}
