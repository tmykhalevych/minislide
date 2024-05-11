#pragma once

#include <cinttypes>

namespace pfm
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

}  // namespace pfm
