/**
 * @file    Ace/System/Testing.hpp
 * @brief   Contains functions and macros useful for unit testing purposes.
 */

#pragma once
#include <iostream>
#include <format>

#define ACE_EXPECT(pClause, ...) \
    if (!(pClause)) \
    { \
        std::cerr << std::format(__VA_ARGS__) << std::endl; \
        return false; \
    }
