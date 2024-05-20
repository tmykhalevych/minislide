#include <logger.hpp>
#include <recursive_lambda.hpp>
#include <shell.hpp>
#include <std_helpers.hpp>

#include <functional>
#include <ranges>
#include <string_view>
#include <tuple>
#include <type_traits>

namespace
{

using std::operator""sv;

enum class CommandId : uint
{
    UNKNOWN = 0,
    ECHO,
    SYS_INFO,
    SYS_STATUS
};

template <typename... T>
using Subcommands = std::tuple<T...>;
using CommandTarger = std::tuple<CommandId>;
using CommandArgsView = std::string_view;

template <typename... TNext>
struct Command
{
    std::string_view command;
    std::tuple<TNext...> next;
};

template <typename... TNext>
Command(std::string_view, std::tuple<TNext...>) -> Command<TNext...>;

constexpr std::string_view to_string_view(auto range)
{
    auto begin = std::ranges::begin(range);
    auto end = std::ranges::end(range);
    return std::string_view(&*begin, std::ranges::distance(begin, end));
}

static constexpr std::tuple SHELL_COMMANDS_TREE{
    Command{"echo"sv, CommandTarger{CommandId::ECHO}},
    Command{"sys"sv, Subcommands{Command{"info"sv, CommandTarger{CommandId::SYS_INFO}},
                                 Command{"status"sv, CommandTarger{CommandId::SYS_STATUS}}}}};

std::pair<CommandId, CommandArgsView> parse_command(std::string_view line)
{
    const auto words = std::views::split(line, " "sv);

    auto word_iter = words.begin();
    CommandId command_id = CommandId::UNKNOWN;
    CommandArgsView command_args{};

    cmn::for_each(SHELL_COMMANDS_TREE,
                  cmn::RecursiveLambda([&word_iter, &command_id](const auto& self, const auto& node) {
                      if constexpr (std::is_same_v<std::decay_t<decltype(node)>, CommandId>) {
                          command_id = node;
                          return;
                      }
                      else {
                          if (node.command == to_string_view(*word_iter)) {
                              ++word_iter;
                              cmn::for_each(node.next, self);
                          }
                      }
                  }));

    if (command_id != CommandId::UNKNOWN) {
        const auto first_param = to_string_view(*word_iter);
        command_args = std::string_view(first_param.begin(), std::distance(first_param.begin(), line.end()));
    }

    return std::make_pair(command_id, command_args);
}

auto parse_arguments(std::string_view line)
{
    return std::views::split(line, " "sv) | std::views::filter([](auto word) { return !std::ranges::empty(word); });
}

}  // namespace

namespace fw
{

void Shell::main()
{
    // TODO: Implement
}

}  // namespace fw
