#pragma once

#include <string_view>
#include <tuple>
#include <utility>

#include <std_helpers.hpp>

namespace fw
{

namespace details
{

template <typename... Children>
inline constexpr auto node(std::string_view name, Children&&... children)
{
    return std::make_pair(name, std::make_tuple(std::forward<Children>(children)...));
}

}  // namespace details

/// @brief Enumeration of shell command IDs
enum class CommandId : uint8_t
{
    ECHO,
    SYSTEM_INFO,
    SYSTEM_STATE
};

// clang-format off

/// @brief Definition of supported shell commands
static constexpr auto SHELL_COMMANDS = std::make_tuple(
    details::node("echo"sv, CommandId::ECHO),
    details::node("sys"sv,
        details::node("info"sv, CommandId::SYSTEM_INFO),
        details::node("state"sv, CommandId::SYSTEM_STATE)));

// clang-format on

/// @brief Maximum number of command arguments
static constexpr auto MAX_SHELL_COMMAND_ARGS_NUM = 5;

namespace details
{

template <typename T>
constexpr size_t command_tree_depth(const T&);

template <typename T>
constexpr size_t element_depth(const T&)
{
    if constexpr (!std::is_same_v<T, CommandId> && !std::is_same_v<T, std::string_view>) {
        return 1 + command_tree_depth(std::get<1>(T{}));
    }
    return 0;
}

template <typename T, size_t... I>
constexpr size_t command_tree_depth_impl(const T&, std::index_sequence<I...>)
{
    size_t max_depth = 0;
    ((max_depth = std::max(max_depth, element_depth(std::get<I>(T{})))), ...);
    return max_depth;
}

template <typename T>
constexpr size_t command_tree_depth(const T& tree)
{
    return command_tree_depth_impl(tree, std::make_index_sequence<std::tuple_size_v<T>>{});
}

}  // namespace details

/// @brief Shell command tree depth
static constexpr auto SHELL_COMMANDS_DEPTH = details::command_tree_depth(SHELL_COMMANDS);

}  // namespace fw
