/**
 * @file    ace.system.file_io.cpp
 */

#include <ace.pch.hpp>
#include <ace.system.file_io.hpp>

namespace ace
{

    /* Public Methods *********************************************************/

    fs::path file_io::absolute (const fs::path& p_path)
    {
        return fs::absolute(p_path).lexically_normal();
    }

    std::fstream file_io::open_text_file (const fs::path& p_path,
        const bool p_for_writing)
    {
        return std::fstream {
            file_io::absolute(p_path),
            (p_for_writing == true) ? 
                std::ios::out : 
                std::ios::in
        };
    }

    std::fstream file_io::open_binary_file (const fs::path& p_path,
        const bool p_for_writing)
    {
        return std::fstream {
            file_io::absolute(p_path),
            (p_for_writing == true) ? 
                (std::ios::out | std::ios::binary) : 
                (std::ios::in | std::ios::binary)
        };
    }

    bool file_io::process_text_file (const fs::path& p_path,
        const std::function<bool(std::string_view)>& p_processor)
    {
        if (p_processor == nullptr)
        {
            _ace_error("No line processor function provided.");
            return false;
        }

        fs::path l_absolute = file_io::absolute(p_path);
        if (fs::exists(l_absolute) == false)
        {
            _ace_error("Text file '{}' not found.", l_absolute.string());
            return false;
        }

        std::fstream l_file { l_absolute, std::ios::in };
        if (l_file.is_open() == false)
        {
            _ace_error("Could not open text file '{}' for processing.",
                l_absolute.string());
            return false;
        }

        std::string l_line = "";
        while (std::getline(l_file, l_line))
        {
            if (p_processor(l_line) == false)
            {
                _ace_error("Error processing text file '{}'.", l_absolute.string());
                return false;
            }
        }

        l_file.close();
        return true;
    }

    bool file_io::load_text_file (const fs::path& p_path,
        std::string& p_contents)
    {
        fs::path l_absolute = file_io::absolute(p_path);
        if (fs::exists(l_absolute) == false)
        {
            _ace_error("Text file '{}' not found.", l_absolute.string());
            return false;
        }

        std::fstream l_file { l_absolute, std::ios::in };
        if (l_file.is_open() == false)
        {
            _ace_error("Could not open text file '{}' for reading.",
                l_absolute.string());
            return false;
        }

        p_contents.clear();
        l_file >> p_contents;
        l_file.close();
        return true;
    }

    bool file_io::save_text_file (const fs::path& p_path,
        const std::string& p_contents)
    {
        fs::path l_absolute = file_io::absolute(p_path);
        std::fstream l_file { l_absolute, std::ios::out };
        if (l_file.is_open() == false)
        {
            _ace_error("Could not open text file '{}' for writing.",
                l_absolute.string());
            return false;
        }

        l_file << p_contents;
        l_file.close();
        return true;
    }

    bool file_io::load_binary_file (const fs::path& p_path, byte_buffer& p_buffer)
    {
        fs::path l_absolute = file_io::absolute(p_path);
        if (fs::exists(l_absolute) == false)
        {
            _ace_error("Binary file '{}' not found.", l_absolute.string());
            return false;
        }

        std::fstream l_file { l_absolute, std::ios::in | std::ios::binary };
        if (l_file.is_open() == false)
        {
            _ace_error("Could not open binary file '{}' for reading.",
                l_absolute.string());
            return false;
        }

        l_file.seekg(0, l_file.end);
        auto l_size = l_file.tellg();
        l_file.seekg(0, l_file.beg);

        p_buffer.resize(l_size);
        l_file.read(reinterpret_cast<char*>(p_buffer.data()), p_buffer.size());
        l_file.close();

        return true;
    }

    bool file_io::save_binary_file (const fs::path& p_path,
        const byte_buffer& p_buffer)
    {
        fs::path l_absolute = file_io::absolute(p_path);
        std::fstream l_file { l_absolute, std::ios::out | std::ios::binary };
        if (l_file.is_open() == false)
        {
            _ace_error("Could not open binary file '{}' for writing.",
                p_path.string());
            return false;
        }

        l_file.write(reinterpret_cast<const char*>(p_buffer.data()),
            p_buffer.size());
        l_file.close();
        return true;
    }

}
