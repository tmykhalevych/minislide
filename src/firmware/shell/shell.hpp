#pragma once

#include <command.hpp>
#include <service.hpp>

#include <etl/vector.h>

#include <string_view>

namespace fw
{

class Shell : public svc::Service<Shell>, public svc::DefaultSetup, public svc::DefaultSuspendResume
{
public:
    Shell() : Service("shell") {}

    void main();

private:
    using tokens = etl::vector<std::string_view, SHELL_COMMANDS_DEPTH + MAX_SHELL_COMMAND_ARGS_NUM>;
    using arguments = std::pair<tokens::const_iterator, tokens::const_iterator>;

    static constexpr auto MAX_LINE_LENGTH = 64;
    static constexpr auto READ_TIMEOUT_US = 50;

    void dispatch_command();

    void on_echo();
    void on_sys_info();
    void on_sys_state();

    std::optional<tokens> tokenize_line() const;

    etl::vector<char, MAX_LINE_LENGTH> m_line;
};

}  // namespace fw
