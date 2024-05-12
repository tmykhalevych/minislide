#pragma once

#include <alternatives.hpp>
#include <event_loop.hpp>
#include <service.hpp>

#include <concepts>
#include <type_traits>

namespace fw::svc
{

// clang-format off

template <typename I>
concept SignalReceiverImplConcept = requires(I mr)
{
    { mr.handle_signal(std::declval<typename I::Signal>()) } -> std::same_as<void>;
};

// clang-format on

template <typename T>
concept SignalConcept = std::is_enum_v<T> || std::is_integral_v<T>;

template <typename TDerived, SignalConcept TSignal>
class SignalReceiver
{
protected:
    using Signal = TSignal;

private:
    template <typename, typename S>
    friend void send_signal_to(S&&);

    void receive_signal(Signal signal)
    {
        event_loop().run_immediate([this, sig = std::move(signal)] { impl().handle_signal(std::move(sig)); });
    }

    EventLoopConcept auto& event_loop() { return static_cast<TDerived&>(*this); }
    SignalReceiverImplConcept auto& impl() { return static_cast<TDerived&>(*this); }
};

template <typename TSetvice, typename TSignal>
void send_signal_to(TSignal&& signal)
{
    typename cmn::Singleton<TSetvice>::Ptr service = cmn::Singleton<TSetvice>::instance();
    ASSERT(service);
    service->receive_signal(signal);
}

}  // namespace fw::svc
