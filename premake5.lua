--
-- @file  premake5.lua
-- @brief The Premake5 build script for the Ace Engine and its client libraries.
--

-- Workspace
workspace "Ace"

    -- Build Files Location
    location "generated"

    -- Language Standard
    language "C++"
    cppdialect "C++20"

    -- Build Configuration
    configurations { "debug", "release" }
    filter { "configurations:debug" }
        symbols "On"
    filter { "configurations:release" }
        optimize "On"
    filter {}

    -- "ace" - Ace Engine Library
    project "ace"

        -- Shared Library
        kind "SharedLib"

        -- Project Location
        location "generated/ace"

        -- Target Directories
        targetdir "build/bin/ace/%{cfg.buildcfg}"
        objdir "build/obj/ace/%{cfg.buildcfg}"

        -- Precompiled Header
        pchheader "projects/ace/include/ace.pch.hpp"
        pchsource "projects/ace/src/ace.pch.cpp"

        -- Include Directories
        includedirs {
            "projects/ace/include"
        }

        -- Source Files
        files {
            "projects/ace/src/ace.*.cpp"
        }

    -- "abox" - Ace Engine Sandbox Application
    project "abox"

        -- Console Application
        kind "ConsoleApp"

        -- Project Location
        location "generated/abox"

        -- Target Directories
        targetdir "build/bin/abox/%{cfg.buildcfg}"
        objdir "build/obj/abox/%{cfg.buildcfg}"

        -- Include and Library Directories
        includedirs {
            "projects/ace/include",
            "projects/abox/include"
        }

        libdirs {
            "build/bin/ace/%{cfg.buildcfg}"
        }

        -- Source Files
        files {
            "projects/abox/src/abox.*.cpp"
        }

        -- Link Libraries
        links {
            "ace"
        }
        