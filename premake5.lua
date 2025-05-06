--
-- @file    premake5.lua
-- @brief   The Premake build file is responsible for generating the files and information needed
--          to build the Ace Engine library, its dependencies and its client applications.
--

workspace "Ace"
    location "generated"
    language "C++"
    cppdialect "C++23"
    warnings "Extra"
    configurations { "debug", "release" }
    filter { "configurations:debug" }
        defines { "ACE_DEBUG" }
        symbols "On"
    filter { "configurations:release" }
        defines { "ACE_RELEASE" }
        optimize "On"
    filter { "system:linux" }
        defines { "ACE_LINUX" }
    filter {}

    defines { "SPDLOG_COMPILED_LIB" }

    project "spdlog"
        kind "StaticLib"
        location "generated/spdlog"
        targetdir "build/bin/spdlog/%{cfg.buildcfg}"
        objdir "build/obj/spdlog/%{cfg.buildcfg}"
        includedirs {
            "vendor/spdlog/include"
        }
        files {
            "vendor/spdlog/src/**.cpp"
        }

    project "lz4"
        kind "StaticLib"
        language "C"
        cdialect "C17"
        location "generated/lz4"
        targetdir "build/bin/lz4/%{cfg.buildcfg}"
        objdir "build/obj/lz4/%{cfg.buildcfg}"
        includedirs {
            "vendor/lz4/lib"
        }
        files {
            "vendor/lz4/lib/*.c"
        }

    project "ace-js"
        kind "StaticLib"
        location "generated/ace-js"
        targetdir "build/bin/ace-js/%{cfg.buildcfg}"
        objdir "build/obj/ace-js/%{cfg.buildcfg}"
        pchheader "projects/ace-js/include/JS/Precompiled.hpp"
        pchsource "projects/ace-js/src/JS/Precompiled.cpp"
        includedirs { "projects/ace-js/include" }
        files {"projects/ace-js/src/**.cpp" }
    
    project "ace"
        kind "StaticLib"
        location "generated/ace"
        targetdir "build/bin/ace/%{cfg.buildcfg}"
        objdir "build/obj/ace/%{cfg.buildcfg}"
        pchheader "projects/ace/include/Ace/Precompiled.hpp"
        pchsource "projects/ace/src/Ace/Precompiled.cpp"
        includedirs {
            "vendor/spdlog/include",
            "vendor/lz4/lib",
            "projects/ace-js/include",
            "projects/ace/include"
        }
        files {
            "projects/ace/src/**.cpp"
        }

    project "acebox"
        kind "ConsoleApp"
        location "generated/acebox"
        targetdir "build/bin/acebox/%{cfg.buildcfg}"
        objdir "build/obj/acebox/%{cfg.buildcfg}"
        pchheader "projects/acebox/include/AceBox/Precompiled.hpp"
        pchsource "projects/acebox/src/AceBox/Precompiled.cpp"
        includedirs {
            "vendor/spdlog/include",
            "vendor/lz4/lib",
            "projects/ace-js/include",
            "projects/ace/include",
            "projects/acebox/include"
        }
        files {
            "projects/acebox/src/**.cpp"
        }
        libdirs {
            "build/bin/spdlog/%{cfg.buildcfg}",
            "build/bin/ace/%{cfg.buildcfg}"
        }
        links {
            "ace", "ace-js", "spdlog", "lz4"
        }
