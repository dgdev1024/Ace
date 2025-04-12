/**
 * @file  ace.system.logger.hpp
 * @brief Provides a class for logging to an output stream.
 */

#pragma once
#include <ace.common.hpp>

namespace ace
{

    /**
     * @brief The `log_level` enumeration enumerates the different severity
     *        levels at which messages can be logged.
     */
    enum class log_level
    {
        trace,              ///< @brief Trace messages are used for logging verbose, detailed debugging information.
        debug,              ///< @brief Debug messages are used for logging general debugging information.
        info,               ///< @brief Info messages are used for logging informational messages.
        warning,            ///< @brief Warning messages are used for logging informational messages which need the end user's attention.
        error,              ///< @brief Error messages are used for logging error messages which need the end user's attention.
        critical            ///< @brief Critical error messages are used for logging messages for errors severe enough to possibly require program termination.
    };

    /**
     * @brief The `logger` class is used for logging text to an output stream,
     *        such as one of the standard output streams or to a file.
     */
    class ace_api logger
    {
    public:
        
        /**
         * @brief Constructs a `logger` using the standard output and error
         *        streams, `std::cout` and `std::cerr`.
         * @param p_name    The name of the logger.
         */
        logger (const std::string& p_name);

        /**
         * @brief Constructs a `logger` using the given output stream for both
         *        standard and error output.
         * @param p_name    The name of the logger.
         * @param p_stream  The stream to use for standard and error output.
         */
        logger (const std::string& p_name, std::ostream& p_out);

        /**
         * @brief Constructs a `logger` using separate streams for standard and
         *        error output.
         * @param p_name    The name of the logger.
         * @brief p_out     The stream to use for standard output.
         * @param p_err     The stream to use for error output.
         */
        logger (const std::string& p_name, std::ostream& p_out, std::ostream& p_err);

        /**
         * @brief The default destructor flushes the logger's target output and
         *        error streams.
         */
        ~logger ();

        /**
         * @brief   Retrieves a string representation of the given log level
         *          enumeration.
         * @param   p_level     The log level enumeration to stringify.
         * @return  The string form of the given log level enumeration.
         */
        inline static constexpr const char* stringify_log_level (const log_level p_level)
        {
            switch (p_level)
            {
                case log_level::trace:      return "trace";
                case log_level::debug:      return "debug";
                case log_level::info:       return "info";
                case log_level::warning:    return "warning";
                case log_level::error:      return "error";
                case log_level::critical:   return "critical";
                default:                    return "unknown";
            }
        }

        /**
         * @brief   Puts a formatted string out to the target output stream.
         * @tparam  args        The types of the variadic arguments to format the string with.
         * @param   p_level     The severity level of the log.
         * @param   p_format    The string to be formatted, then output.
         * @param   p_args      The variadic arguments, if any, to format the string with.
         */
        template <typename... args>
        inline void put_to_out (const log_level p_level, std::string_view p_format, 
            args&&... p_args)
        {
            std::osyncstream(m_out)
                << "[" << m_name << " | " << logger::stringify_log_level(p_level) << "] "
                << std::vformat(p_format, std::make_format_args(p_args...))
                << '\n';
        }

        /**
         * @brief   Puts a formatted string out to the target error stream.
         * @tparam  args        The types of the variadic arguments to format the string with.
         * @param   p_level     The severity level of the log.
         * @param   p_format    The string to be formatted, then error.
         * @param   p_args      The variadic arguments, if any, to format the string with.
         */
        template <typename... args>
        inline void put_to_err (const log_level p_level, std::string_view p_format, 
            args&&... p_args)
        {
            std::osyncstream(m_err)
                << "[" << m_name << " | " << logger::stringify_log_level(p_level) << "] "
                << std::vformat(p_format, std::make_format_args(p_args...))
                << std::endl;
        }

    private:
        std::string   m_name;   ///< @brief The name of the logger.
        std::ostream& m_out;    ///< @brief The target output stream.
        std::ostream& m_err;    ///< @brief The target error stream.    

    };

}
