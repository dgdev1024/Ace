/**
 * @file  ace.config.hpp
 */

#pragma once

/* Macros - Operating System **************************************************/

#if defined(_WIN64) || (defined(__CYGWIN__) && !defined(_WIN32))
    #define ace_windows
    #define ace_os_string "Microsoft Windows"
#elif defined(__ANDROID__)
    #error "Andriod OS is not yet supported."
#elif defined(__linux__)
    #define ace_linux
    #define ace_os_string "Linux"
#elif defined(__APPLE__) && defined(__MACH__)
    #include <TargetConditionals.h>
    #if TARGET_OS_MAC == 1
        #define ace_mac_osx
        #define ace_os_string "Apple Mac OSX"
    #else
        #error "Apple iOS is not yet supported."
    #endif
#endif

/* Macros - Compiler **********************************************************/

#if defined(__GNUC__)
    #define ace_gnuc
    #define ace_compiler_version \
        (__GNUC__ * 100) + \
        (__GNUC_MINOR__)
    #define ace_compiler_string "GNU G++"
#elif defined(__clang__)
    #define ace_clang
    #define ace_compiler_version \
        (__clang_major__ * 100) + \
        (__clang_minor__)
    #define ace_compiler_string "Clang++"
#elif defined(_MSC_VER)
    #define ace_msvc
    #define ace_compiler_version \
        _MSC_FULL_VER
    #define ace_compiler_string "Microsoft Visual C++"
#else
    #error "This compiler is not yet supported."
#endif

/* Macros - C++ Standard ******************************************************/

#if __cplusplus < 202002L
    #error "The ACE engine requires a C++20-capable compiler."
#endif

/* Macros - Import/Export Symbols *********************************************/

#if defined(ace_build_shared)
    #if defined(ace_gnuc) || defined(ace_clang)
        #define ace_api __attribute__((visibility("default")))
    #elif defined(ace_msvc)
        #if defined(ace_export)
            #define ace_api __declspec(dllexport)
        #elif
            #define ace_api __declspec(dllimport)
        #endif
    #endif
#else
    #define ace_api
#endif
