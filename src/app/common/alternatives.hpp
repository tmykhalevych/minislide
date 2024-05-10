#pragma once

namespace common
{

struct Alternative
{
    static constexpr auto ignore = [](auto) {};
};

template <class... A>
struct Alternatives : A...
{
    using A::operator()...;
};

template <class... A>
Alternatives(A...) -> Alternatives<A...>;

}  // namespace common
