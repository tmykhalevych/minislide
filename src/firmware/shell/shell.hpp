#pragma once

#include <service.hpp>

#include <etl/vector.h>

#include <string_view>

namespace fw
{

class Shell : public svc::Service<Shell>, public svc::DefaultSetup, public svc::DefaultSuspendResume
{
public:
    Shell() : Service("shell") {}

    void main() { read_char(); }

private:
    static constexpr auto MAX_LINE_LENGTH = 64;
    static constexpr auto READ_TIMEOUT_US = 50;

    void read_char();
    void dispatch_command();

    void on_echo(std::string_view args_view);
    void on_sys_info(std::string_view args_view);
    void on_sys_state(std::string_view args_view);

    etl::vector<char, MAX_LINE_LENGTH> m_line;
};

}  // namespace fw
