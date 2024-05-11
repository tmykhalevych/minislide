#pragma once

#include <alternatives.hpp>
#include <event_loop.hpp>
#include <service.hpp>

#include <concepts>
#include <type_traits>
#include <variant>

namespace service
{

// clang-format off

template <typename I>
concept MessageReceiverImplConcept = requires(I mr)
{
    { mr.handle_message(std::declval<typename I::Message>()) } -> std::same_as<void>;
};

// clang-format on

template <typename T>
concept MessageConcept = std::is_class_v<T>;

template <typename TDerived, MessageConcept... TMessages>
class MessageReceiver
{
protected:
    using Message = std::variant<TMessages...>;

private:
    template <typename S, typename M>
    friend void send_message_to(M&&);

    void receive_message(auto&& message)
    {
        event_loop().run_immediate([this, msg = std::move(message)] { impl().handle_message(std::move(msg)); });
    }

    EventLoopConcept auto& event_loop() { return static_cast<TDerived&>(*this); }
    MessageReceiverImplConcept auto& impl() { return static_cast<TDerived&>(*this); }
};

template <typename TSetvice, typename TMessage>
void send_message_to(TMessage&& msg)
{
    typename cmn::Singleton<TSetvice>::Ptr service = cmn::Singleton<TSetvice>::instance();
    ASSERT(service);
    service->receive_message(std::move(msg));
}

}  // namespace service
