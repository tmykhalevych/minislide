#include <hal/gpio.hpp>
#include <iostream>

using namespace hal;

Gpio::Gpio(GpioId id) : m_id(id)
{}

Gpio::~Gpio()
{}

GpioIn::GpioIn(GpioId id) : Gpio(id)
{}

GpioState GpioIn::read() const
{
    std::cout << "Reading ON from GPIO:" << std::to_string(get_id()) << '\n';
    return GpioState::ON;
}

GpioOut::GpioOut(GpioId id) : Gpio(id)
{}

void GpioOut::write(GpioState state) const
{
    const auto state_str = (state == GpioState::ON) ? "ON" : "OFF";
    std::cout << "Writing " << state_str << " to GPIO:" << std::to_string(get_id()) << '\n';
}
