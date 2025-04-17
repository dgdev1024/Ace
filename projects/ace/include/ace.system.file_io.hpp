/**
 * @file    ace.system.file_io.hpp
 * @brief   Provides a static class containing functions facilitating file I/O.
 */

#pragma once
#include <ace.system.byte_view.hpp>

namespace ace
{

    /**
     * @brief   The `file_io` static class provides functions used for opening,
     *          loading and saving files.
     */
    class ace_api file_io
    {
    public:

        /**
         * @brief   Converts the given file path into an absolute, lexically-normal
         *          file path.
         * @param   p_path  The relative file path to convert.
         * @return  The absolute, lexically-normal file path.
         */
        static fs::path absolute (const fs::path& p_path);

        /**
         * @brief   Opens a text file found at the given path.
         * @param   p_path          The path to the text file to open.
         * @param   p_for_writing   Open the file for writing, instead of reading?
         * @return  A file stream handle to the (hopefully) opened file.
         */
        static std::fstream open_text_file (const fs::path& p_path, 
            const bool p_for_writing = false);

        /**
         * @brief   Opens a binary file found at the given path.
         * @param   p_path          The path to the binary file to open.
         * @param   p_for_writing   Open the file for writing, instead of reading?
         * @return  A file stream handle to the (hopefully) opened file.
         */
        static std::fstream open_binary_file (const fs::path& p_path, 
            const bool p_for_writing = false);

        /**
         * @brief   Processes, line-by-line, the contents of a text file found
         *          at the given path.
         * @param   p_path          The path to the text file to open.
         * @param   p_processor     A function to call on each line of the file.
         * @param   `true` if the file is opened, and each line therein processed
         *          successfully; `false` otherwise.
         */
        static bool process_text_file (const fs::path& p_path,
            const std::function<bool(std::string_view)>& p_processor);

        /**
         * @brief   Loads the contents of a text file found at the given path
         *          into the provided string.
         * @param   p_path          The path to the text file to open.
         * @param   p_contents      The string to load teh file's contents into.
         * @return  `true` if the file is opened, and its contents loaded
         *          successfully; `false` otherwise.
         */
        static bool load_text_file (const fs::path& p_path,
            std::string& p_contents);

        /**
         * @brief   Saves the contents of the given string into a text file at
         *          the given path.
         * @param   p_path          The path to the text file to write.
         * @param   p_contents      The string whose contents will be written.
         * @return  `true` if the file is written successfully; `false` otherwise.
         */
        static bool save_text_file (const fs::path& p_path,
            const std::string& p_contents);

        /**
         * @brief   Loads the contents of a binary file found at the given path
         *          into the provided byte buffer.
         * @param   p_path          The path to the binary file to open.
         * @param   p_buffer        The byte buffer to load the file's contents into.
         * @return  `true` if the file is opened, and its contents loaded
         *          successfully; `false` otherwise.
         */
        static bool load_binary_file (const fs::path& p_path,
            byte_buffer& p_buffer);

        /**
         * @brief   Saves the contents of the given byte buffer into a file at
         *          the given path.
         * @param   p_path          The path to the binary file to write.
         * @param   p_buffer        The byte buffer whose contents will be written.
         * @return  `true` if the file is written successfully; `false` otherwise.
         */
        static bool save_binary_file (const fs::path& p_path,
            const byte_buffer& p_buffer);

    };

}
