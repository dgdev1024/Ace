/**
 * @file    Ace/System/Logger.cpp
 */

#include <iostream>
#include <Ace/System/Logger.hpp>

namespace ace
{

    /* Static Members *********************************************************/

    std::atomic<bool>                        Logger::sRunning { false };     
    std::thread                              Logger::sWorkerThread;
    std::mutex                               Logger::sQueueMutex;  
    std::condition_variable                  Logger::sQueueMutexCV;
    std::queue<LogEvent>                     Logger::sQueue;       
    std::mutex                               Logger::sSinksMutex;  
    std::vector<std::shared_ptr<ILogSink>>   Logger::sSinks;       

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

        // If a thread is "put to sleep" using the
        // `std::condition_variable::wait` method, any lock currently locking
        // that thread down is temporarly released.
        //
        // The `std::condition_variable::notify_one` and
        // `std::condition_variable::notify_all` methods are used to "wake up"
        // that thread, causing that temporarly-relased lock to be re-acquired.
        // The latter of these methods is used here in order to process any
        // outstanding log events before shutting down the logging subsystem.
        sQueueMutexCV.notify_all();

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
        const std::string&  pMessage
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
            .mMessage       = pMessage
        };

        // Under a lock, move the log event into the event queue.
        {
            std::unique_lock lGuard { sQueueMutex };
            sQueue.push(std::move(lEvent));
        }

        // If the logger's background worker thread is asleep, wake it up here.
        sQueueMutexCV.notify_one();
    }

    /* Private Methods ********************************************************/

    void Logger::ProcessQueue ()
    {
        // Under a lock, the calling thread will continue processing the log
        // event queue as long as it's not empty and the logger subsystem is
        // still running.
        std::unique_lock lGuard { sQueueMutex };
        
        // `std::atomic::load` retrieves the value of an atomic variable - in
        // this case, whether or not the logging subsystem is still running.
        while (sRunning.load() == true || sQueue.empty() == false)
        {

            // The `std::condition_variable` is used here to temporarly release 
            // the `std::unique_lock` we set up at the start of this method, then
            // re-acquire that lock when one of the following becomes true:
            // - The logging subsystem has shut down.
            // - The log event queue is no longer empty - there are log events to process.
            sQueueMutexCV.wait(
                lGuard,
                []
                {
                    return
                        sRunning.load() == false ||
                        sQueue.empty() == false;
                }
            );

            // Once we reach here, the `std::unique_lock` which was temporarly
            // released by the above `std::condition_variable` has been
            // re-acquired, at which point the log events currently enqueued,
            // if any, need to be processed.
            while (sQueue.empty() == false)
            {

                // Move, then pop the event at the front of the queue.
                LogEvent lEvent = std::move(sQueue.front());
                sQueue.pop();

                // Temporarly release this method's `std::unique_lock` in order
                // to dispatch this event. Once dispatched, re-acquire the lock.
                lGuard.unlock();
                Dispatch(lEvent);
                lGuard.lock();

            }

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
