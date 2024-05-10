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
    platform::StatusLed::State value;
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
        std::visit(common::Alternatives{[this](SetLedState s) {
                                            LOG_INFO("LED is %s",
                                                     (s.value == platform::StatusLed::State::ON) ? "ON" : "OFF");
                                            platform::StatusLed::set_state(s.value);
                                        },
                                        common::Alternative::ignore},
                   msg);
    }

    void handle_signal(Signal sig) { LOG_INFO("Reseived signal: %u", sig); }
};

}  // namespace app
