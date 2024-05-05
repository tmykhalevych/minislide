#pragma once

#include <functional>

namespace common
{

template <typename T>
using BoundMember = void (T::*)();

template <typename T, BoundMember<T> F>
void bind_to(void* context)
{
    std::invoke(F, static_cast<T*>(context));
}

}  // namespace common
