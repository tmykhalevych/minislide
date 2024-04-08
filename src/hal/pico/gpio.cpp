#include <hal/gpio.hpp>
#include <pico/stdlib.h>

using namespace hal;

Gpio::Gpio(GpioId id) : m_id(id)
{
    gpio_init(m_id);
}

Gpio::~Gpio()
{
    gpio_deinit(m_id);
}

GpioIn::GpioIn(GpioId id) : Gpio(id)
{
    gpio_set_dir(get_id(), GPIO_IN);
}

GpioState GpioIn::read() const
{
    return gpio_get(get_id()) ? GpioState::ON : GpioState::OFF;
}

GpioOut::GpioOut(GpioId id) : Gpio(id)
{
    gpio_set_dir(get_id(), GPIO_OUT);
}

void GpioOut::write(GpioState state) const
{
    gpio_put(get_id(), state == GpioState::ON);
}
