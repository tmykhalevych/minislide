#include <getchar.hpp>

#include <cstdio>

namespace bsp
{

std::optional<char> getchar(unsigned)
{
    int res = ::getchar();
    if (res == EOF) {
        return std::nullopt;
    }

    return static_cast<char>(res);
}

}  // namespace bsp
