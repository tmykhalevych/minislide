#pragma once

#include <service.hpp>
#include <status_led.hpp>

namespace app
{

class LedService : public service::Service<LedService>
{
public:
    LedService() : Service("LedService") {}

    bool setup() { return true; }
    bool try_suspend() { return true; }
    bool try_resume() { return true; }

    void main()
    {
        run_periodic(
            [this] {
                m_led_state = (m_led_state == State::ON) ? State::OFF : State::ON;
                LOG_INFO("LED is %s", (m_led_state == State::ON) ? "ON" : "OFF");
                platform::StatusLed::set_state(m_led_state);
            },
            2'000);
    }

private:
    using State = platform::StatusLed::State;

    State m_led_state = State::OFF;
};

}  // namespace app
