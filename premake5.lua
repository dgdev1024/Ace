--
-- @file    premake5.lua
--

-- Format Output Directories
local outputdir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"

-- Include any and all common external dependencies
local external_includes = {
    "./external/**/include"
}

-- Ace Workspace
workspace "Ace"

    -- Language and Runtime Settings
    language        "C++"
    cppdialect      "C++20"
    architecture    "x64"
    staticruntime   "On"
    startproject    "Sandbox"

    -- Build Configurations
    configurations {
        "Debug",
        "Release",
        "Distribute"
    }
    filter { "configurations:Debug" }
        defines {
            "ACE_DEBUG",
            "ACE_LOG_ENABLED"
        }
        symbols "On"
    filter { "configurations:Release" }
        defines {
            "ACE_RELEASE",
            "ACE_LOG_ENABLED"
        }
        optimize "On"
    filter { "configurations:Distribute" }
        defines {
            "ACE_DISTRIBUTE"
        }
        optimize "Full"
        symbols "Off"
    filter { "system:linux" }
        defines {
            "ACE_LINUX"
        }
    filter {}

    -- Project: `AceEngine` - Ace Engine Library
    project "AceEngine"
        kind        "StaticLib"
        location    "./build/%{outputdir}/AceEngine"
        targetdir   "./build/%{outputdir}/bin"
        objdir      "./build/%{outputdir}/obj/AceEngine"
        files       { "./engine/Ace/**.hpp", "./engine/Ace/**.cpp" }
        includedirs { "./engine", table.unpack(external_includes) }

        libdirs {

        }
        links {

        }
        
        filter { "system:windows" }
            systemversion   "latest"
            defines         { "ACE_WINDOWS" }
        filter { "system:linux" }
            pic             "On"
            defines         { "ACE_LINUX" }
        filter {}

    -- Project: `Sandbox` - Ace Engine Sandbox Application
    project "Sandbox"
        kind        "ConsoleApp"
        location    "./build/%{outputdir}/Sandbox"
        targetdir   "./build/%{outputdir}/bin"
        objdir      "./build/%{outputdir}/obj/Sandbox"
        files       { "./examples/Sandbox/**.hpp", "./examples/Sandbox/**.cpp" }
        includedirs { "./engine", "./examples", table.unpack(external_includes) }
        links       { "AceEngine" }
        
        filter { "system:windows" }
            systemversion   "latest"
        filter { "system:linux" }
            pic             "On"
        filter {}
