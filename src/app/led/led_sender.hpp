#pragma once

#include <led_receiver.hpp>
#include <message.hpp>
#include <service.hpp>
#include <signal.hpp>
#include <status_led.hpp>

namespace app
{

class LedSender : public service::Service<LedSender>
{
public:
    LedSender() : Service("LedSender") {}

    bool setup() { return true; }
    bool try_suspend() { return true; }
    bool try_resume() { return true; }

    void main()
    {
        run_periodic([this] { service::send_message_to<LedReceiver>(next_state()); }, 2'000);
        run_periodic([] { service::send_signal_to<LedReceiver>(LedSignal::HELLO); }, 5'000);
    }

private:
    using State = platform::StatusLed::State;

    SetLedState next_state()
    {
        m_led_state = (m_led_state == State::ON) ? State::OFF : State::ON;
        return SetLedState{.value = m_led_state};
    }

    State m_led_state = State::OFF;
};

}  // namespace app
