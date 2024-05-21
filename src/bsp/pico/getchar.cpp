#include <getchar.hpp>

#include <pico/stdlib.h>

namespace bsp
{

std::optional<char> getchar(unsigned timeout_us)
{
    int res = getchar_timeout_us(timeout_us);
    if (res == PICO_ERROR_TIMEOUT) {
        return std::nullopt;
    }

    return static_cast<char>(res);
}

}  // namespace bsp
