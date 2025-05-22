/**
 * @file    Ace/Common.hpp
 */

#pragma once

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <random>
#include <ranges>
#include <string>
#include <string_view>
#include <thread>
#include <typeindex>
#include <typeinfo>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include <cstddef>
#include <cstdint>
#include <cstring>

#define ACE_API
#define ACE_TYPEID(pType) std::type_index(typeid(pType))
#define ACE_THROW(pEx, ...) \
    throw pEx { std::format(__VA_ARGS__) }

namespace fs = std::filesystem;

static_assert(sizeof(std::size_t) == sizeof(std::uintmax_t),
    "'std::size_t' != 'std::uintmax_t'!");
static_assert(sizeof(std::size_t) == sizeof(std::uint64_t),
    "'std::size_t' != 'std::uint64_t'!");

namespace astd
{

    constexpr std::size_t npos = static_cast<std::size_t>(-1);

                                        using byte_buffer = std::vector<std::uint8_t>;
    template <typename T>               using raw_ptr = T*;
    template <typename T>               using raw_ref = T&;
    template <typename T, typename U>   using vector_map = std::vector<std::pair<T, U>>;
    template <typename T>               using type_map = std::unordered_map<std::type_index, T>;

    template <typename... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };
    template <typename... Ts>
    overloaded(Ts...) -> overloaded<Ts...>;
    
}