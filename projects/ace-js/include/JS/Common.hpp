/**
 * @file    JS/Common.hpp
 * @brief   Common includes and macros for the Ace Engine's Javascript and JSON library.
 */

#pragma once

/* Include Files **********************************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <span>
#include <memory>
#include <filesystem>
#include <functional>
#include <optional>
#include <variant>

namespace fs = std::filesystem;

/* Macros *****************************************************************************************/

#define JS_API

/* Helpers ****************************************************************************************/

// TODO: Somehow convince someone with clout to include this in a future C++ standard.
namespace std
{
    template <typename T>
    using raw_ptr = T*;
}

namespace js
{

    template <typename... Ts>
    struct Overloaded : Ts...
    {
        using Ts::operator()...;
    };

    template <typename... Ts>
    Overloaded(Ts...) -> Overloaded<Ts...>;

}
