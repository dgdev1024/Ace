/**
 * @file    Ace/System/FileWatcher.hpp
 * @brief   Provides a class used for watching files for changes in real time.
 */

#pragma once
#include <Ace/Common.hpp>

namespace ace
{

    /**
     * @brief   Enumerates the means by which a file being watched can be
     *          changed to emit a @a `FileChangedEvent`.
     */
    enum class FileChangeMethod
    {
        Created,
        Updated,
        Deleted
    };

    /**
     * @brief   An event structure which is raised when a @a `FileWatcher` class
     *          detects a change to a file being watched.
     */
    struct FileChangedEvent
    {
        std::string         mPath;      ///< @brief The path to the file which was changed.
        FileChangeMethod    mMethod;    ///< @brief The method by which the file was changed.
    };

    /**
     * @brief   Forward-declaration of a structure containing the @a `FileWatcher`'s
     *          platform-specific components.
     */
    struct FileWatcherContext;

    /**
     * @brief   A class used for watching files for changes in real time, and
     *          emitting/publishing events wheneven changes occur.
     */
    class ACE_API FileWatcher final
    {
    public:

        /**
         * @brief   The default constructor.
         */
        explicit FileWatcher ();

        /**
         * @brief   The destructor.
         */
        ~FileWatcher ();

    public:

        /**
         * @brief   Starts the file watcher's worker thread, watching the given
         *          list of directories for changes to the files therein.
         * 
         * @param   pDirectories    The list of directories to watch.
         * @param   pRecursive      Watch all subdirectories, as well?
         */
        void Start (
            const std::vector<fs::path>&    pDirectories,
            const bool                      pRecursive = false
        );

        /**
         * @brief   Stops the file watcher if it's currently running.
         */
        void Stop ();

    private:

        /**
         * @brief   This function is run by the file watcher's background
         *          worker thread.
         */
        void Run ();

    private:
        std::atomic<bool>       mRunning { false }; ///< @brief Indicates whether or not the file watcher is running.
        std::thread             mThread;            ///< @brief The worker thread responsible for watching for file changes.
        std::vector<fs::path>   mDirectories;       ///< @brief The list of physical directories being watched.

        /**
         * @brief   Contains the file watcher's platform-specific components.
         */
        std::shared_ptr<
            FileWatcherContext
        > mContext = nullptr;

    };

}
