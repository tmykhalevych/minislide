#include <platform.hpp>

#include <pico/cyw43_arch.h>
#include <pico/stdlib.h>

namespace platform
{

bool init()
{
    if (!stdio_init_all()) return false;
    if (cyw43_arch_init() != 0) return false;

    return true;
}

}  // namespace platform
