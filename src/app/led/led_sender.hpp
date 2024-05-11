#pragma once

#include <led_receiver.hpp>
#include <message.hpp>
#include <service.hpp>
#include <signal.hpp>
#include <state.hpp>
#include <status_led.hpp>

namespace app
{

enum class LedSenderState : uint
{
    ANY
};

class LedSender : public service::Service<LedSender>,
                  public service::StateKeeper<LedSender, LedSenderState>,
                  public service::DefaultSetup,
                  public service::DefaultSuspendResume
{
public:
    LedSender() : Service("LedSender") {}

    State init_state() { return State::ANY; }

    void main()
    {
        run_periodic([this] { service::send_message_to<LedReceiver>(next_state()); }, 2'000);
        run_periodic([] { service::send_signal_to<LedReceiver>(LedSignal::HELLO); }, 5'000);
    }

private:
    using LedState = pfm::StatusLed::State;

    SetLedState next_state()
    {
        m_led_state = (m_led_state == LedState::ON) ? LedState::OFF : LedState::ON;
        return SetLedState{.value = m_led_state};
    }

    LedState m_led_state = LedState::OFF;
};

}  // namespace app
