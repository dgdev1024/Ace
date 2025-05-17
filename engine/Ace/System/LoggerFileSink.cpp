/**
 * @file    Ace/System/LoggerFileSink.cpp
 */

#include <iomanip>
#include <regex>
#include <Ace/System/LoggerFileSink.hpp>

namespace ace
{

    /* Constructors and Destructor ********************************************/

    LoggerFileSink::LoggerFileSink (
        const LoggerFileSinkSpec&   pSpec
    ) :
        mLogDirectory       { pSpec.mLogDirectory },
        mBaseName           { pSpec.mBaseName },
        mMaxSize            { pSpec.mMaxSize },
        mMaxArchives        { pSpec.mMaxArchives }
    {
        // Create the logging directory, if it doesn't already exist, then
        // open a log file.
        fs::create_directories(mLogDirectory);
        OpenLogFile();
    }

    /* Public Methods *********************************************************/

    void LoggerFileSink::Write (const LogEvent& pEvent)
    {
        // Create the format string.
        std::string lLine = FormatEvent(pEvent);

        // Check to see if the log file needs to be rotated out.
        mCurrentSize += lLine.size();
        if (mCurrentSize > mMaxSize)
        {
            RotateFiles();
            OpenLogFile();
            mCurrentSize = lLine.size();
        }

        // Write the message to the file, flushing the stream afterwards to
        // ensure that it is written entirely.
        mFileStream << lLine;
        mFileStream.flush();
    }

    /* Private Methods ********************************************************/

    std::string LoggerFileSink::GenerateTimestampString ()
    {
        // Get the current system time and get a time structure from it.
        auto    lNow        = std::chrono::system_clock::now();
        auto    lTime       = std::chrono::system_clock::to_time_t(lNow);
        std::tm lTimeStruct;
        #if defined(ACE_WINDOWS)
        
        #else
            localtime_r(&lTime, &lTimeStruct);
        #endif

        // Format the time into a string and return it.
        std::ostringstream lStream;
        lStream << std::put_time(&lTimeStruct, "%Y%m%d_%H%M%S");
        return lStream.str();
    }

    std::string LoggerFileSink::FormatEvent (const LogEvent& pEvent)
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

    void LoggerFileSink::OpenLogFile ()
    {
        // Open the new log file.
        fs::path lPath = mLogDirectory / mBaseName;
        mFileStream.open(lPath, std::ios::out | std::ios::trunc);
        if (mFileStream.is_open() == false)
        {
            throw std::runtime_error {
                std::format("Could not open log file '{}' for writing!",
                    lPath.string())
            };
        }

        mCurrentSize = fs::file_size(lPath);
    }

    void LoggerFileSink::RotateFiles ()
    {
        // Close the current file.
        mFileStream.close();

        // Rename the just-closed log file.
        fs::rename(
            mLogDirectory / mBaseName,
            mLogDirectory /
                ("ace_" + GenerateTimestampString() + ".log")
        );

        // Prune the old archived log files.
        PruneArchives();
    }

    void LoggerFileSink::PruneArchives ()
    {
        // A regular expression used for matching log filenames.
        static const std::regex LOG_FILENAME_REGEX {
            R"(ace_\d{8}_\d{6}\.log)"
        };

        // Look for all archived log files in the log directory and record them.
        std::vector<fs::directory_entry> lArchives;
        for (const auto& lEntry : fs::directory_iterator { mLogDirectory })
        {
            if (lEntry.is_regular_file() == false)
            {
                continue;
            }
            else if (
                std::regex_match(lEntry.path().filename().string(),
                    LOG_FILENAME_REGEX) == true
            )
            {
                lArchives.push_back(lEntry);
            }
        }

        // Sort the filenames in descending order by time.
        std::sort(
            lArchives.begin(),
            lArchives.end(),
            [] (
                fs::directory_entry& pFirst, 
                fs::directory_entry& pSecond
            )
            {
                return
                    pFirst.path().filename().string() >
                    pSecond.path().filename().string();
            }
        );

        // If there are any archived log files beyond the maximum allowed number
        // thereof, delete them.
        for (std::size_t i = mMaxArchives; i < lArchives.size(); ++i)
        {
            std::error_code lErrorCode;
            fs::remove(lArchives[i], lErrorCode);   // If there are any errors deleting the file, ignore them.
        }
    }

}
