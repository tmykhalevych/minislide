#pragma once

#include <FreeRTOS.h>

#include <assert.hpp>
#include <event_loop.hpp>
#include <logger.hpp>
#include <prohibit_copy_move.hpp>
#include <singleton.hpp>

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

namespace service
{

// clang-format off

template <typename I>
concept ServiceImplConcept = requires(I s)
{
    { s.setup() } -> std::same_as<bool>;
    { s.main() } -> std::same_as<void>;
    { s.try_suspend() } -> std::same_as<bool>;
    { s.try_resume() } -> std::same_as<bool>;
};

// clang-format on

[[nodiscard]] inline Milliseconds get_boot_time()
{
    return pdTICKS_TO_MS(xTaskGetTickCount());
}

static constexpr size_t DEFAULT_TASK_QUEUE_CAPACITY = 10;
static constexpr size_t DEFAULT_STACK_SIZE = configMINIMAL_STACK_SIZE * 2;

template <typename TDerived, size_t StackSize = DEFAULT_STACK_SIZE,
          size_t ImmediateQueueCap = DEFAULT_TASK_QUEUE_CAPACITY, size_t DeferredQueueCap = DEFAULT_TASK_QUEUE_CAPACITY>
class Service : public EventLoop<StackSize, ImmediateQueueCap, DeferredQueueCap>
{
public:
    bool start();

    void suspend();
    void resume();

    [[nodiscard]] bool is_running() const { return m_running; };

protected:
    explicit Service(std::string_view name, get_time_cb_t get_time_cb = get_boot_time, size_t prio = tskIDLE_PRIORITY)
        : EventLoop<StackSize, ImmediateQueueCap, DeferredQueueCap>(name, prio, get_time_cb)
    {}

private:
    ServiceImplConcept auto& impl() { return static_cast<TDerived&>(*this); }

    // TODO: consider making these atomic
    bool m_running = false;
    bool m_started = false;
};

template <typename Impl, size_t S, size_t I, size_t D>
bool Service<Impl, S, I, D>::start()
{
    ASSERT(!m_started);

    if (!impl().setup()) {
        LOG_ERROR("failed to setup");
        return false;
    }

    this->run_immediate([this] {
        m_running = true;
        impl().main();
    });

    m_started = true;
    return true;
}

template <typename Impl, size_t S, size_t I, size_t D>
void Service<Impl, S, I, D>::suspend()
{
    if (!m_running) return;

    if (!impl().try_suspend()) {
        LOG_ERROR("failed to suspend");
        return;
    }

    EventLoop<S, I, D>::suspend();
    m_running = false;
}

template <typename Impl, size_t S, size_t I, size_t D>
void Service<Impl, S, I, D>::resume()
{
    if (m_running) return;

    if (!impl().try_resume()) {
        LOG_ERROR("failed to resume");
        return;
    }

    EventLoop<S, I, D>::resume();
    m_running = true;
}

/// @brief Utility to check if a class derives from Service with any parameters
template <typename Derived, template <typename, size_t, size_t, size_t> class Base>
struct is_derived_from_service_impl
{
    template <typename T>
    static constexpr auto check(T*) -> std::true_type;
    static constexpr auto check(...) -> std::false_type;
    using type = decltype(check(std::declval<Base<Derived, 0, 0, 0>*>()));
};

template <typename Derived, template <typename, size_t, size_t, size_t> class Base>
using is_derived_from_service = typename is_derived_from_service_impl<Derived, Base>::type;

template <typename S>
concept ServiceConcept = is_derived_from_service<S, Service>::value;

template <ServiceConcept S>
bool create_and_start()
{
    common::Singleton<S>::emplace();
    return common::Singleton<S>::instance()->start();
}

template <ServiceConcept S>
common::Singleton<S>::Ptr access()
{
    typename common::Singleton<S>::Ptr service = common::Singleton<S>::instance();
    ASSERT(service);
    return service;
}

}  // namespace service
