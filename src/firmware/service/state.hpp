#pragma once

#include <concepts>
#include <type_traits>

#include <mutex.hpp>

namespace fw::svc
{

// clang-format off

template <typename I>
concept StateKeeperImplConcept = requires(I mr)
{
    { mr.init_state() } -> std::same_as<typename I::State>;
};

// clang-format on

template <typename T>
concept StateConcept = std::is_enum_v<T> || std::is_integral_v<T>;

template <typename TDerived, StateConcept TState>
class StateKeeper
{
public:
    using State = TState;

    State get_state() const
    {
        State state;
        {
            std::lock_guard lock(m_state_mutex);
            state = m_state;
        }
        return state;
    }

protected:
    StateKeeper() { m_state = impl().init_state(); }

    void set_state(State state)
    {
        std::lock_guard lock(m_state_mutex);
        m_state = state;
    }

private:
    StateKeeperImplConcept auto& impl() { return static_cast<TDerived&>(*this); }

    mutable fr::Mutex m_state_mutex;
    State m_state;
};

template <typename Derived, template <typename, typename> class Base>
struct is_derived_from_state_keeper_impl
{
    template <typename T>
    static constexpr auto check(T*) -> std::true_type;
    static constexpr auto check(...) -> std::false_type;
    using type = decltype(check(std::declval<Base<Derived, int>*>()));
};

template <typename Derived, template <typename, typename> class Base>
using is_derived_from_state_keeper = typename is_derived_from_state_keeper_impl<Derived, Base>::type;

template <typename T>
concept StateKeeperConcept = is_derived_from_state_keeper<T, StateKeeper>::value;

template <StateKeeperConcept T>
typename T::State get_state_for()
{
    typename cmn::Singleton<T>::Ptr service = cmn::Singleton<T>::instance();
    ASSERT(service);
    return service->get_state();
}

}  // namespace fw::svc
