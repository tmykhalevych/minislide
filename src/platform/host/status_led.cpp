#include <status_led.hpp>

#include <iostream>

using namespace platform;

void StatusLed::set_state(State state)
{
    std::cout << "HOST: status_led is " << (state == State::ON) ? "ON" : "OFF" << std::endl;
}
