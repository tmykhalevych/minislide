#pragma once

#include <variant>

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

template <class... A>
Alternatives(A...) -> Alternatives<A...>;

template <typename T, typename... TVisitors>
inline void dispatch(T variant, TVisitors&&... visitors)
{
    std::visit(cmn::Alternatives{std::forward<TVisitors>(visitors)..., Alternative::ignore}, variant);
}

}  // namespace cmn
