#pragma once

#include <cstddef>
#include <ranges>
#include <string_view>
#include <tuple>
#include <utility>

using namespace std::literals;

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

/// @brief Helper function to apply lambda to each tuple element and OR the results
constexpr inline bool for_each_or(const auto& tuple, auto&& func)
{
    return std::apply([&func](const auto&... args) { return (func(args) || ...); }, tuple);
}

template <typename T>
struct is_tuple : std::false_type
{
};

template <typename... Ts>
struct is_tuple<std::tuple<Ts...>> : std::true_type
{
};

/// @brief Trait to determine whether T is an std::tuple
template <typename T>
static constexpr auto is_tuple_v = is_tuple<T>::value;

template <typename T>
struct is_pair : std::false_type
{
};

template <typename F, typename S>
struct is_pair<std::pair<F, S>> : std::true_type
{
};

/// @brief Trait to determine whether T is an std::pair
template <typename T>
static constexpr auto is_pair_v = is_pair<T>::value;

}  // namespace cmn
