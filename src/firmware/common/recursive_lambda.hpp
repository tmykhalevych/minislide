#pragma once

namespace cmn
{

/// @brief Helper class to allow using 'this' for lambdas
template <class F>
class RecursiveLambda
{
public:
    explicit constexpr RecursiveLambda(F&& f) : m_f(std::move(f)) {}

    template <class... Args>
    decltype(auto) operator()(Args&&... args) const
    {
        return std::invoke(m_f, *this, std::forward<Args>(args)...);
    }

private:
    F m_f;
};

}  // namespace cmn