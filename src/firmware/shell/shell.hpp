#pragma once

#include <service.hpp>

namespace fw
{

class Shell : public svc::Service<Shell>, public svc::DefaultSetup, public svc::DefaultSuspendResume
{
public:
    Shell() : Service("shell") {}
    void main();
};

}  // namespace fw
