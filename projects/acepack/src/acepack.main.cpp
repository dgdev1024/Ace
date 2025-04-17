/**
 * @file  acepack.main.cpp
 */

#include <ace.hpp>

static bool pack_folder (ace::asset_writer& p_writer, const fs::path& p_folder, 
    const std::string& p_parent_name)
{
    for (const auto& l_dirent : fs::directory_iterator { p_folder })
    {
        if (l_dirent.is_directory() == true)
        {
            std::string l_parent_name =
                p_parent_name + "/" + l_dirent.path().stem().string();
            if (pack_folder(p_writer, l_dirent.path(), l_parent_name) == false)
            {
                return false;
            }
        }
        else if (l_dirent.is_regular_file() == true)
        {
            std::string l_asset_name = 
                p_parent_name + "/" + l_dirent.path().filename().string();
            if (p_writer.add_asset_file(l_asset_name, l_dirent.path()) == false)
            {
                ace_error("Error packing asset file '{}'.", l_dirent.path().string());
                return false;
            }
        }
    }

    return true;
}

int main (int argc, char** argv)
{
    if (argc < 3)
    {
        std::printf("Usage: %s <folder> <asset.ace>\n", argv[0]);
        return 0;
    }

    ace::logging::initialize("acepack");
    fs::path l_input_folder = fs::absolute(argv[1]).lexically_normal();
    if (fs::exists(l_input_folder) == false)
    {
        ace_error("Asset folder '{}' not found.", l_input_folder.string());
        return 1;
    }

    ace::asset_writer l_writer;
    if (
        pack_folder(l_writer, l_input_folder, l_input_folder.stem().string()) == false
    )
    {
        ace_error("Could not pack asset folder '{}'.", l_input_folder.string());
        return 1;
    }

    if (l_writer.save_to_file(argv[2]) == false)
    {
        ace_error("Could not save asset file '{}'.", argv[2]);
        return 1;
    }

    return 0;
}
