#pragma once

#include <FreeRTOS.h>

#include <hal/status_led.hpp>
#include <logger.hpp>
#include <service.hpp>

// clang-format off

#define REQUIRED(val) if (!(val)) return false
#define OPTIONAL(val) val

// clang-format on

class TestService : public service::Service<TestService>
{
public:
    TestService() : Service("test") {}

    bool setup() { return true; }
    bool try_suspend() { return true; }
    bool try_resume() { return true; }

    void main()
    {
        run_periodic(
            [this] {
                m_led_state = (m_led_state == State::ON) ? State::OFF : State::ON;
                LOG_INFO("LED is %s", (m_led_state == State::ON) ? "ON" : "OFF");
                hal::StatusLed::set_state(m_led_state);
            },
            2'000);
    }

private:
    using State = hal::StatusLed::State;

    State m_led_state = State::OFF;
};

namespace app
{

[[nodiscard]] inline bool init()
{
    logger::create_and_start(logger::Severity::DEBUG);

    REQUIRED(service::create_and_start<TestService>());

    return true;
}

inline void start()
{
    vTaskStartScheduler();
}

}  // namespace app
