#pragma once

#include <cstddef>
#include <tuple>

namespace cmn
{

struct Alternative
{
    static constexpr auto ignore = [](auto) {};
};

template <typename... A>
struct Alternatives : A...
{
    using A::operator()...;
};

template <typename... A>
Alternatives(A...) -> Alternatives<A...>;

template <size_t I = 0, typename F, typename... TArgs>
constexpr void for_each(const std::tuple<TArgs...>& t, const F& f)
{
    f(std::get<I>(t));
    if constexpr (I + 1 != sizeof...(TArgs)) for_each<I + 1>(t, f);
}

}  // namespace cmn
