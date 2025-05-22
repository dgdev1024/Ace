/**
 * @file    Ace/System/ThreadPool.cpp
 */

#include <Ace/System/ThreadPool.hpp>

namespace ace
{

    /* Constructors and Destructor ********************************************/

    ThreadPool::ThreadPool (
        std::size_t pThreadCount
    ) :
        mDone   { false }
    {
        // If the thread count given is greater than the number of threads
        // supported by the running system, then correct that number.
        if (pThreadCount > std::thread::hardware_concurrency())
        {
            pThreadCount = std::thread::hardware_concurrency();
        }
        
        // Loop through the thread count.
        for (std::size_t i = 0; i < pThreadCount; ++i)
        {
            // For each iteration, emplace a thread which runs the following
            // function:
            mWorkerThreads.emplace_back(
                [this] -> void
                {
                    while (true)
                    {
                        // Under a lock, get the next task to run.
                        std::function<void()> lTask;
                        {
                            std::unique_lock lGuard { mMutex };

                            // Wait until the task queue is not empty, or the
                            // thread pool finishes.
                            mConditional.wait(
                                lGuard,
                                [this] -> bool
                                {
                                    return 
                                        mDone == true || 
                                        mTasks.empty() == false;
                                }
                            );

                            // If the thread pool finishes, and there are no
                            // more tasks to carry out, then return.
                            if (mDone == true && mTasks.empty() == true)
                            {
                                return;
                            }

                            // Move the task out of the queue and into the
                            // out-of-lock snapshot.
                            lTask = std::move(mTasks.front());
                            mTasks.pop();
                        }

                        // Execute the task.
                        lTask();
                    }
                }
            );
        }
    }

    ThreadPool::~ThreadPool ()
    {
        // Under a lock, indicate that the thread pool is done executing tasks.
        {
            std::lock_guard lGuard { mMutex };
            mDone = true;
        }

        // Use the conditional to notify all worker threads to stop running.
        mConditional.notify_all();

        // Join each thread, waiting for them to finish running.
        for (auto& lThread : mWorkerThreads)
        {
            lThread.join();
        }
    }

}
