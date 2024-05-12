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

class LedSender : public svc::Service<LedSender>,
                  public svc::StateKeeper<LedSender, LedSenderState>,
                  public svc::DefaultSetup,
                  public svc::DefaultSuspendResume
{
public:
    LedSender() : Service("LedSender") {}

    State init_state() { return State::ANY; }

    void main()
    {
        run_periodic([this] { svc::send_message_to<LedReceiver>(next_state()); }, 2'000);
        run_periodic([] { svc::send_signal_to<LedReceiver>(LedSignal::HELLO); }, 5'000);
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
