/**
 * @file    Ace/Common.hpp
 */

#pragma once

#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <cstddef>
#include <cstdint>

#define ACE_API

namespace fs = std::filesystem;

static_assert(sizeof(std::size_t) == sizeof(std::uintmax_t),
    "'std::size_t' != 'std::uintmax_t'!");
