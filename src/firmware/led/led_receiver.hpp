#pragma once

#include <logger.hpp>
#include <message.hpp>
#include <service.hpp>
#include <signal.hpp>
#include <status_led.hpp>

namespace fw
{

struct SetLedState
{
    bsp::StatusLed::State value;
};

enum class LedSignal : uint
{
    HELLO
};

class LedReceiver : public svc::Service<LedReceiver>,
                    public svc::MessageReceiver<LedReceiver, svc::MessageList<SetLedState>>,
                    public svc::SignalReceiver<LedReceiver, LedSignal>,
                    public svc::DefaultService
{
public:
    LedReceiver() : Service("LedReceiver") {}

    void handle_message(Message msg)
    {
        svc::dispatch_message(msg, [this](SetLedState s) {
            LOG_INFO("LED is %s", (s.value == bsp::StatusLed::State::ON) ? "ON" : "OFF");
            bsp::StatusLed::set_state(s.value);
        });
    }

    void handle_signal(Signal sig) { LOG_INFO("Reseived signal: %u", sig); }
};

}  // namespace fw
