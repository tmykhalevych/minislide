#pragma once

#include <cinttypes>
#include <optional>

namespace bsp
{

std::optional<char> getchar(unsigned timeout_us);

}  // namespace bsp
