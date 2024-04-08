#pragma once

#include <cinttypes>

namespace hal
{

using GpioId = uint8_t;

enum class GpioState : uint8_t
{
    OFF = 0,
    ON
};

class Gpio
{
protected:
    explicit Gpio(GpioId id);
    ~Gpio();

    GpioId get_id() const { return m_id; }

private:
    GpioId m_id;
};

class GpioIn : public Gpio
{
public:
    explicit GpioIn(GpioId id);

    [[nodiscard]] GpioState read() const;
};

class GpioOut : public Gpio
{
public:
    explicit GpioOut(GpioId id);

    void write(GpioState state) const;
};

}  // namespace hal
