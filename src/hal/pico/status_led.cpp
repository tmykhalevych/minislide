#include <hal/status_led.hpp>
#include <pico/cyw43_arch.h>

using namespace hal;

void StatusLed::set_state(State state)
{
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, (state == State::ON) ? true : false);
}
