#pragma once

#include <cinttypes>

namespace hal
{

class StatusLed
{
public:
    enum class State : uint8_t
    {
        OFF = 0,
        ON
    };

    static void set_state(State state);
};

}  // namespace hal
