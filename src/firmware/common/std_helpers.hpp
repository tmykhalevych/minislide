#pragma once

#include <cstddef>
#include <ranges>
#include <string_view>
#include <tuple>

using std::operator""sv;

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

template <std::ranges::range TRange>
constexpr std::string_view to_string_view(TRange range)
{
    auto begin = std::ranges::begin(range);
    auto end = std::ranges::end(range);
    return std::string_view(&*begin, std::ranges::distance(begin, end));
}

}  // namespace cmn
