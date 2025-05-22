/**
 * @file    Ace/System/ThreadPool.hpp
 * @brief   Provides a class used for managing a pool of threads working on a
 *          large number of tasks.
 */

#pragma once
#include <Ace/Common.hpp>

namespace ace
{

    /**
     * @brief   A class used for managing a pool of threads, for working on a
     *          large number of tasks concurrently.
     */
    class ACE_API ThreadPool final
    {
    public:

        /**
         * @brief   The default constructor creates the given number of new
         *          threads to work on tasks.
         * 
         * @param   pThreadCount    The number of worker threads to be created.
         */
        explicit ThreadPool (
            std::size_t pThreadCount = std::thread::hardware_concurrency()
        );
        
        /**
         * @brief   The destructor.
         */
        ~ThreadPool ();

    public:

        /**
         * @brief   Enqueues a new task for one of the worker threads to execute
         *          in the future.
         * 
         * If the task has a return value, an `std::future` will contain that
         * return value once that task has been executed.
         * 
         * @tparam  T       The type of the function to be called by the task.
         * @tparam  As...   The types of the arguments passed into the function.
         * 
         * @param   pFunction   The function to be called by the task.
         * @param   pArgs       The arguments, if any, passed into the function.
         * 
         * @return  An `std::future` which will hold the function's return value
         *          once the task has been executed.
         */
        template <typename T, typename... As>
        auto Enqueue (
            T&&         pFunction,
            As&&...     pArgs
        ) -> std::future<std::invoke_result_t<T, As...>>
        {

            // Define the return type of the function which the task to be
            // enqueued will call.
            using ReturnValue = std::invoke_result_t<T, As...>;

            // - Create a bind expression binding the provided function and its
            //   arguments. Wrap that expression in a packaged task which can be
            //   called asynchronously.
            // - Get the `std::future` from that packaged task. That will be
            //   returned from this method.
            auto lTaskPointer =
                std::make_shared<std::packaged_task<ReturnValue()>>(
                    std::bind(
                        std::forward<T>(pFunction),
                        std::forward<As>(pArgs)...
                    )
                );
            std::future<ReturnValue> lFuture = lTaskPointer->get_future();

            // Under a lock, enqueue a void function which will call the
            // bound, packaged task function.
            {
                std::lock_guard lGuard { mMutex };
                mTasks.emplace(
                    [lTaskPointer] -> void
                    {
                        (*lTaskPointer)();
                    }
                );
            }

            // Notify one of the waiting worker threads that a new task is
            // available to be executed.
            mConditional.notify_one();

            // The `std::future` returned here will contain the return value,
            // if any, of the packaged task function that is executed.
            return lFuture;

        }

    private:
        std::vector<std::thread>            mWorkerThreads;     ///< @brief The list of worker threads in this thread pool.
        std::queue<std::function<void()>>   mTasks;             ///< @brief The queue of tasks being worked on by this thread pool's worker threads.
        std::mutex                          mMutex;             ///< @brief The mutex used for locking down this class's members.
        std::condition_variable             mConditional;       ///< @brief A conditional for the worker threads to wait for new tasks on.
        bool                                mDone = false;      ///< @brief Is the thread pool finished executing tasks?

    };

}
