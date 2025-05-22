/**
 * @file    Ace/System/FileWatcher.cpp
 */

#if defined(ACE_LINUX)
    #include <sys/inotify.h>
    #include <unistd.h>
#endif

#include <Ace/System/EventBus.hpp>
#include <Ace/System/FileWatcher.hpp>

namespace ace
{

    /* `FileWatcherContext` Structure *****************************************/

    struct FileWatcherContext
    {
    #if defined(ACE_LINUX)
        std::int32_t mNotifyDescriptor = -1;
        std::unordered_map<std::int32_t, fs::path> mWatchDescriptors;
    #endif
    };

    /* Constructors and Destructor ********************************************/

    FileWatcher::FileWatcher () :
        mContext { std::make_shared<FileWatcherContext>() }
    {

    }

    FileWatcher::~FileWatcher ()
    {
        Stop();   
    }

    /* Public Methods *********************************************************/

    void FileWatcher::Start (
        const std::vector<fs::path>&    pDirectories,
        const bool                      pRecursive
    )
    {
        // If the file watcher is currently running, then early out.
        if (mRunning.exchange(true) == true)
        {
            return;
        }

        // Iterate over the directories and add them to the directories list.
        for (const auto& lDirectory : pDirectories)
        {
            mDirectories.push_back(lDirectory);
        }

        // Start the worker thread.
        mThread = std::thread {
            [this] -> void
            {
                this->Run();
            }
        };
    }

    void FileWatcher::Stop ()
    {
        // If the file watcher is not running, early out.
        if (mRunning.exchange(false) == false)
        {
            return;
        }

        #if defined(ACE_LINUX)
        {
            if (mContext->mNotifyDescriptor >= 0)
            {
                ::close(mContext->mNotifyDescriptor);
                mContext->mWatchDescriptors.clear();
            }
        }
        #endif

        if (mThread.joinable() == true)
        {
            mThread.join();
        }

        mDirectories.clear();
    }

    /* Private Methods ********************************************************/

    void FileWatcher::Run ()
    {
        #if defined(ACE_LINUX)
        {
            // The size of the watch buffer.
            constexpr std::size_t WATCH_BUFFER_SIZE =
                1024 * (sizeof(inotify_event) + 16);

            // A buffer to hold the watcher's event data.
            astd::byte_buffer lWatchBuffer;
            lWatchBuffer.resize(WATCH_BUFFER_SIZE);

            // Create the watcher's descriptor.
            mContext->mNotifyDescriptor = ::inotify_init1(IN_NONBLOCK);

            // Add a watch descriptor for each directory being watched.
            for (const auto& lDirectory : mDirectories)
            {
                std::int32_t lWatchDescriptor =
                    inotify_add_watch(
                        mContext->mNotifyDescriptor,
                        lDirectory.c_str(),
                        IN_CREATE | IN_MODIFY | IN_DELETE
                    );
                mContext->mWatchDescriptors[lWatchDescriptor] = lDirectory;
            }

            // Watch for file changes.
            while (mRunning == true)
            {
                // Read from the notify descriptor. The data read here will
                // contain `inotify_event` structures which indicate file
                // changes.
                std::int32_t lBytesRead = ::read(
                    mContext->mNotifyDescriptor,
                    lWatchBuffer.data(),
                    WATCH_BUFFER_SIZE
                );

                // If nothing was read, then wait a little bit before continuing.
                if (lBytesRead <= 0)
                {
                    std::this_thread::sleep_for(
                        std::chrono::milliseconds(100)
                    );
                    continue;
                }

                // Process any changes which turned up.
                std::int32_t lBytesProcessed = 0;
                while (lBytesProcessed < lBytesRead)
                {
                    // Get a notify event from the buffer, and the watch
                    // directory involved.
                    inotify_event* lEventPtr =
                        reinterpret_cast<inotify_event*>(
                            lWatchBuffer.data() + lBytesProcessed
                        );
                    auto lDirectory = mContext->mWatchDescriptors[
                        lEventPtr->wd
                    ];

                    // Determine the full name of the file which changed.
                    fs::path lChanged = lDirectory / lEventPtr->name;

                    // Determine the change method.
                    FileChangeMethod lMethod = FileChangeMethod::Updated;
                    if (lEventPtr->mask & IN_CREATE)
                        { lMethod = FileChangeMethod::Created; }
                    else if (lEventPtr->mask & IN_DELETE)
                        { lMethod = FileChangeMethod::Deleted; }

                    // Publish an event to announce the file change.
                    EventBus::Publish(
                        FileChangedEvent { lChanged.string(), lMethod }
                    );

                    // Move the cursor to the next event, if any.
                    lBytesProcessed += sizeof(inotify_event) + lEventPtr->len;
                }
            }
        }
        #endif
    }

}
