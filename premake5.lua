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
        defines { "ace_debug_mode" }
        symbols "On"
    filter { "configurations:release" }
        optimize "On"
    filter {}

    -- Dependency: "lz4" - Compression Library
    project "lz4"
        kind "SharedLib"
        language "C"
        cdialect "C17"
        location "generated/lz4"
        targetdir "build/bin/lz4/%{cfg.buildcfg}"
        objdir "build/obj/lz4/%{cfg.buildcfg}"
        includedirs { "vendor/lz4" }
        files { "vendor/lz4/*.c" }

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
            "vendor/lz4",
            "vendor/glm",
            "projects/ace/include"
        }

        -- Source Files
        files {
            "projects/ace/src/ace.*.cpp"
        }

    -- "acepack" - Ace Engine Asset Packaging Tool
    project "acepack"
        
        -- Console Application
        kind "ConsoleApp"

        -- Project Location
        location "generated/acepack"

        -- Target Directories
        targetdir "build/bin/acepack/%{cfg.buildcfg}"
        objdir "build/obj/acepack/%{cfg.buildcfg}"

        -- Include and Library Directories
        includedirs {
            "vendor/lz4",
            "vendor/glm",
            "projects/ace/include",
            "projects/acepack/include"
        }

        libdirs {
            "build/bin/lz4/%{cfg.buildcfg}",
            "build/bin/ace/%{cfg.buildcfg}"
        }

        -- Source Files
        files {
            "projects/acepack/src/acepack.*.cpp"
        }

        -- Link Libraries
        links {
            "ace", "lz4"
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
            "vendor/lz4",
            "vendor/glm",
            "projects/ace/include",
            "projects/abox/include"
        }

        libdirs {
            "build/bin/lz4/%{cfg.buildcfg}",
            "build/bin/ace/%{cfg.buildcfg}"
        }

        -- Source Files
        files {
            "projects/abox/src/abox.*.cpp"
        }

        -- Link Libraries
        links {
            "ace", "lz4"
        }
        