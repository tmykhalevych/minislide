#pragma once

#include <event_loop.hpp>
#include <logger.hpp>
#include <service.hpp>
#include <std_helpers.hpp>

#include <concepts>
#include <type_traits>
#include <variant>

namespace fw::svc
{

// clang-format off
template <typename I>
concept MessageReceiverImplConcept = requires(I mr)
{
    { mr.handle_message(std::declval<typename I::Message>()) } -> std::same_as<void>;
};
// clang-format on

template <typename, typename>
class MessageReceiver;

template <typename T>
concept MessageConcept = std::is_class_v<T>;

template <MessageConcept... TMessages>
using MessageList = std::variant<TMessages...>;

template <typename TDerived, typename... TMessages>
class MessageReceiver<TDerived, MessageList<TMessages...>>
{
protected:
    using Message = MessageList<TMessages...>;

private:
    template <typename S, typename M>
    friend void send_message_to(M&&);

    void receive_message(MessageConcept auto&& message)
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

static constexpr auto DEFAULT_MESSAGE_HANDLER = [](auto) { LOG_WARN("message ignored"); };

template <MessageConcept M, typename... THandlers>
void dispatch_message(const M& message, THandlers&&... handlers)
{
    std::visit(cmn::Alternatives{std::forward<THandlers>(handlers)..., DEFAULT_MESSAGE_HANDLER}, message);
}

}  // namespace fw::svc
