#pragma once

#include <logger.hpp>
#include <prohibit_copy_move.hpp>
#include <singleton.hpp>

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace service
{

// clang-format off

template <typename I>
concept ServiceImplInterface = requires(I s)
{
    { s.setup() } -> std::same_as<bool>;
    { s.main() } -> std::same_as<void>;
    { s.try_suspend() } -> std::same_as<bool>;
    { s.try_resume() } -> std::same_as<bool>;
};

// clang-format on

template <typename ServiceImpl>
class Service : public common::ProhibitCopyMove
{
public:
    void start();

    void suspend();
    void resume();

    [[nodiscard]] bool is_running() const { return m_running; };

protected:
    explicit Service(std::string_view name) : m_name(name) {}

    // add methods for tasks scheduling

private:
    ServiceImplInterface auto& impl() { return static_cast<ServiceImpl&>(*this); }

    bool m_running = false;
    bool m_satrted = false;

    std::string m_name;
};

template <typename ServiceImpl>
void Service<ServiceImpl>::start()
{
    if (!impl().setup()) {
        LOG_ERROR("failed to setup %s", m_name.data());
        return;
    }

    // create event loop
    // start service with main()

    m_running = true;
    m_satrted = true;
}

template <typename ServiceImpl>
void Service<ServiceImpl>::suspend()
{
    if (!m_running) return;

    if (!impl().try_suspend()) {
        LOG_ERROR("failed to suspend %s", m_name.data());
        return;
    }

    // suspend event loop

    m_running = false;
}

template <typename ServiceImpl>
void Service<ServiceImpl>::resume()
{
    if (m_running) return;

    if (!impl().try_resume()) {
        LOG_ERROR("failed to resume %s", m_name.data());
        return;
    }

    // resume event loop

    m_running = true;
}

template <typename S>
concept ServiceInterface = std::derived_from<S, Service<S>>;

template <ServiceInterface S>
void create_and_start()
{
    common::Singleton<S>::emplace();
    common::Singleton<S>::instance()->start();
}

template <ServiceInterface S>
common::Singleton<S>::Ptr access()
{
    typename common::Singleton<S>::Ptr service = common::Singleton<S>::instance();
    configASSERT(service);
    return service;
}

}  // namespace service
