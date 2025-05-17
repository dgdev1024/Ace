/**
 * @file    Ace/System/LoggerFileSink.hpp
 * @brief   Provides a log sink for writing output to log files.
 */

#pragma once
#include <Ace/System/Logger.hpp>

namespace ace
{

    /**
     * @brief   A structure containing attributes which define a logger sink
     *          for writing to log files.
     */
    struct LoggerFileSinkSpec
    {
        fs::path        mLogDirectory   = "logs";           ///< @brief The path to the folder to hold the log files in.
        std::string     mBaseName       = "ace.log";        ///< @brief The name of the log file currently being written.
        std::size_t     mMaxSize        = 10 * 1024 * 1024; ///< @brief The maximum size, in bytes, of a single log file.
        std::size_t     mMaxArchives    = 5;                ///< @brief The maximum number of log files which can be archived.
    };

    /**
     * @brief   Provides a logger sink for writing output to log files.
     */
    class ACE_API LoggerFileSink final : public ILogSink
    {
    public:

        /**
         * @brief   The default constructor constructs a logger file sink with
         *          the given specification.
         * 
         * @param   pSpec   The logger sink's specification.
         */
        LoggerFileSink (
            const LoggerFileSinkSpec&   pSpec = {}
        );

    public:

        /**
         * @brief   Writes a formatted message to the currently-open log file.
         * 
         * Messages written by this method will be formatted as follows:
         * `[HH:MM:SS | LEVEL] FILE:FUNCTION:LINE - MESSAGE`
         * 
         * @param   pEvent  The log event containing the message to write.
         */
        void Write (
            const LogEvent& pEvent
        ) override;

    private:
        
        /**
         * @brief   Generates a timestamp string from the current system time.
         * 
         * @return  The generated timestamp string.
         */
        static std::string GenerateTimestampString ();

        /**
         * @brief   Generates a formatted string from the given log event.
         * 
         * @param   pEvent  The log event to format.
         * 
         * @return  The generated format string.
         */
        std::string FormatEvent (
            const LogEvent& pEvent
        );

        /**
         * @brief   Opens a new log file.
         */
        void OpenLogFile ();

        /**
         * @brief   Closes the current log file, then opens a new one.
         */
        void RotateFiles ();

        /**
         * @brief   Prunes old archived log files, removing them.
         */
        void PruneArchives ();

    private:
        std::fstream    mFileStream;        ///< @brief The file stream which opens the log file(s).
        fs::path        mLogDirectory;      ///< @brief The path to the log file(s) being written.
        std::string     mBaseName = "";     ///< @brief The base name of the log files being written.
        std::size_t     mCurrentSize = 0;   ///< @brief The size of the current log file, in bytes.
        std::size_t     mMaxSize = 0;       ///< @brief The maximum allowed size of a single log file, in bytes.
        std::size_t     mMaxArchives = 0;   ///< @brief The maximum allowed number of log files archived by this log sink.

    };

}
