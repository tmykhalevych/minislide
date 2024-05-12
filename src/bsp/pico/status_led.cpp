#include <pico/cyw43_arch.h>
#include <status_led.hpp>

using namespace bsp;

void StatusLed::set_state(State state)
{
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, (state == State::ON) ? true : false);
}
