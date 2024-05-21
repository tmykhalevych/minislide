#include <getchar.hpp>
#include <logger.hpp>
#include <recursive_lambda.hpp>
#include <scope_guard.hpp>
#include <shell.hpp>
#include <std_helpers.hpp>

#include <ranges>
#include <string_view>
#include <tuple>

namespace
{

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
                          if (node.command == cmn::to_string_view(*word_iter)) {
                              ++word_iter;
                              cmn::for_each(node.next, self);
                          }
                      }
                  }));

    if (command_id != CommandId::UNKNOWN) {
        const auto first_param = cmn::to_string_view(*word_iter);
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

void Shell::read_char()
{
    cmn::ScopeGuard next_read([this] { run_after([this] { read_char(); }, 10); });

    auto res = bsp::getchar(READ_TIMEOUT_US);
    if (!res) {
        return;
    }

    if (m_line.size() == (MAX_LINE_LENGTH - 1)) {
        LOG_ERROR("line buffer overflow");
        m_line.clear();
        return;
    }

    const char ch = res.value();
    if (ch == '\r' || ch == '\n') {
        m_line.push_back('\0');
        dispatch_command();
        m_line.clear();
        return;
    }

    m_line.push_back(ch);
}

void Shell::dispatch_command()
{
    auto [cmd, args_view] = parse_command({m_line.data(), m_line.size()});

    switch (cmd) {
        case CommandId::UNKNOWN: {
            LOG_ERROR("unknown command");
            break;
        }
        case CommandId::ECHO: {
            on_echo(args_view);
            break;
        }
        case CommandId::SYS_INFO: {
            on_sys_info(args_view);
            break;
        }
        case CommandId::SYS_STATUS: {
            on_sys_state(args_view);
            break;
        }
    }
}

void Shell::on_echo(std::string_view args_view)
{
    printf("%s\n", args_view.data());
}

void Shell::on_sys_info(std::string_view args_view)
{
    auto args = parse_arguments(args_view);
    if (args.begin() != args.end()) {
        LOG_WARN("excessive params");
    }

    // TODO: Implement
}

void Shell::on_sys_state(std::string_view args_view)
{
    auto args = parse_arguments(args_view);
    if (args.begin() != args.end()) {
        LOG_WARN("excessive params");
    }

    // TODO: Implement
}

}  // namespace fw
