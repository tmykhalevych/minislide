#include <getchar.hpp>
#include <logger.hpp>
#include <recursive_lambda.hpp>
#include <scope_guard.hpp>
#include <shell.hpp>

#include <optional>
#include <string_view>
#include <tuple>

namespace
{

std::optional<fw::CommandId> match_command(const auto& node, const auto& tokens)
{
    fw::CommandId id;
    auto next_token_iter = tokens.begin();

    if (next_token_iter == tokens.end()) {
        return std::nullopt;
    }

    cmn::RecursiveLambda check_node([&](const auto& self, const auto& node) {
        using node_decay_t = std::decay_t<decltype(node)>;

        if constexpr (cmn::is_tuple_v<node_decay_t>) {
            return for_each_or(node, self);
        }

        if constexpr (cmn::is_pair_v<node_decay_t>) {
            if (node.first == *next_token_iter) {
                ++next_token_iter;
                return for_each_or(node.second, self);
            }
            // keep iterating
            return false;
        }

        if constexpr (std::is_same_v<node_decay_t, fw::CommandId>) {
            id = node;
            // stop iterating
            return true;
        }
    });

    if (!for_each_or(node, check_node)) {
        return std::nullopt;
    }

    return std::make_optional(id);
}

}  // namespace

namespace fw
{

void Shell::main()
{
    cmn::ScopeGuard next_read([this] { run_after([this] { main(); }, 10); });

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
    auto tokens = tokenize_line();
    if (!tokens) {
        LOG_ERROR("too many tokens");
        return;
    }

    auto command = match_command(SHELL_COMMANDS, tokens.value());
    if (!command) {
        LOG_ERROR("unknown command");
        return;
    }

    switch (command.value()) {
        case CommandId::ECHO: {
            on_echo();
            break;
        }
        case CommandId::SYSTEM_INFO: {
            on_sys_info();
            break;
        }
        case CommandId::SYSTEM_STATE: {
            on_sys_state();
            break;
        }
    }
}

void Shell::on_echo()
{
    // TODO: Implement
}

void Shell::on_sys_info()
{
    // TODO: Implement
}

void Shell::on_sys_state()
{
    // TODO: Implement
}

std::optional<Shell::tokens> Shell::tokenize_line() const
{
    std::string_view input(m_line.data(), m_line.size() - 1);
    tokens tokens;
    bool in_quotes = false;
    size_t token_start_pos = 0;

    auto add_token = [&](size_t start, size_t end) {
        if (tokens.full() || end <= start) {
            return false;
        }

        tokens.emplace_back(input.substr(start, end - start));
        return true;
    };

    for (size_t pos = 0; pos < input.size(); ++pos) {
        char ch = input[pos];
        if (ch == '"') {
            if (in_quotes) {
                // end of quoted token
                if (!add_token(token_start_pos, pos)) {
                    return std::nullopt;
                }
                in_quotes = false;
                token_start_pos = pos + 1;
            }
            else {
                // start of quoted token
                in_quotes = true;
                token_start_pos = pos + 1;
            }
        }
        else if (!in_quotes && std::isspace(ch)) {
            if (!add_token(token_start_pos, pos)) {
                return std::nullopt;
            }
            // skip over consecutive spaces
            while (pos < input.size() && std::isspace(input[pos])) {
                ++pos;
            }
            token_start_pos = pos;
            --pos;
        }
    }

    // add the last token if any
    add_token(token_start_pos, input.size());

    return std::make_optional(std::move(tokens));
}

}  // namespace fw
