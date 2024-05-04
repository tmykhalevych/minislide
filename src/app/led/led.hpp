#pragma once

#include <hal/status_led.hpp>
#include <service.hpp>

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
        run([this] {
            m_led_state = (m_led_state == State::ON) ? State::OFF : State::ON;
            LOG_INFO("LED is %s", (m_led_state == State::ON) ? "ON" : "OFF");
            hal::StatusLed::set_state(m_led_state);
        });
    }

private:
    using State = hal::StatusLed::State;

    State m_led_state = State::OFF;
};

}  // namespace app
