#pragma once

#include <logger.hpp>
#include <message.hpp>
#include <service.hpp>
#include <signal.hpp>
#include <status_led.hpp>

namespace app
{

struct SetLedState
{
    pfm::StatusLed::State value;
};

enum class LedSignal : uint
{
    HELLO
};

class LedReceiver : public service::Service<LedReceiver>,
                    public service::MessageReceiver<LedReceiver, SetLedState>,
                    public service::SignalReceiver<LedReceiver, LedSignal>,
                    public service::DefaultService
{
public:
    LedReceiver() : Service("LedReceiver") {}

    void handle_message(Message msg)
    {
        std::visit(cmn::Alternatives{[this](SetLedState s) {
                                         LOG_INFO("LED is %s", (s.value == pfm::StatusLed::State::ON) ? "ON" : "OFF");
                                         pfm::StatusLed::set_state(s.value);
                                     },
                                     cmn::Alternative::ignore},
                   msg);
    }

    void handle_signal(Signal sig) { LOG_INFO("Reseived signal: %u", sig); }
};

}  // namespace app
