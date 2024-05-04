
#include <assert.hpp>
#include <event_loop.hpp>
#include <logger.hpp>

#include <functional>

namespace
{

using el_bound_fn_t = void (service::EventLoop::*)();
template <el_bound_fn_t F>
void Bound(void* context)
{
    std::invoke(F, static_cast<service::EventLoop*>(context));
}

}  // namespace

namespace service
{

EventLoop::EventLoop(std::string_view name, size_t stack_size, size_t prio, get_time_cb_t get_time_cb)
    : m_name(name), m_get_time_cb(std::move(get_time_cb)), m_worker_events(xEventGroupCreate())
{
    const BaseType_t status =
        xTaskCreate(Bound<&EventLoop::worker_thread>, m_name.data(), static_cast<configSTACK_DEPTH_TYPE>(stack_size),
                    this, static_cast<UBaseType_t>(prio), &m_worker_handle);

    ASSERT(m_worker_events);
    ASSERT(status == pdPASS);
}

void EventLoop::TaskHandle::cancel()
{
    std::lock_guard lock(m_loop.m_deferred_mutex);
    m_loop.m_deferred_handles.erase(m_value);
    m_canceled = true;
}

void EventLoop::run(Task&& task)
{
    {
        std::lock_guard lock(m_immediate_mutex);
        m_immediate_tasks.push(std::move(task));
    }

    xEventGroupSetBits(m_worker_events, Event::WAKE);
}

EventLoop::TaskHandle EventLoop::run_after(Task&& task, Timepoint delay_ms)
{
    const Timepoint now = m_get_time_cb();
    const DeferredTaskHandle handle = DeferredTask::get_next_handle();

    {
        std::lock_guard lock(m_deferred_mutex);
        m_deferred_tasks.push(DeferredTask{.handle = handle, .task = std::move(task), .deadline = now + delay_ms});
        m_deferred_handles.insert(handle);
    }

    xEventGroupSetBits(m_worker_events, Event::WAKE);
    return TaskHandle{*this, handle};
}

EventLoop::TaskHandle EventLoop::run_periodic(Task&& task, Timepoint period_ms, Timepoint delay_ms)
{
    const Timepoint now = m_get_time_cb();
    const DeferredTaskHandle handle = DeferredTask::get_next_handle();

    {
        std::lock_guard lock(m_deferred_mutex);
        m_deferred_tasks.push(
            DeferredTask{.handle = handle, .task = std::move(task), .deadline = now + delay_ms, .period = period_ms});
        m_deferred_handles.insert(handle);
    }

    xEventGroupSetBits(m_worker_events, Event::WAKE);
    return TaskHandle{*this, handle};
}

void EventLoop::suspend()
{
    m_worker_stopped = true;
    xEventGroupSetBits(m_worker_events, Event::WAKE);
    xEventGroupWaitBits(m_worker_events, Event::STOP, pdTRUE, pdFALSE, portMAX_DELAY);
    vTaskSuspend(m_worker_handle);

    {
        std::lock_guard lock(m_immediate_mutex);
        if (!m_immediate_tasks.empty()) {
            LOG_WARN("Clearing immediate queue for %s", name().data());
            m_immediate_tasks.clear();
        }
    }
}

void EventLoop::resume()
{
    m_worker_stopped = false;
    vTaskResume(m_worker_handle);
    xEventGroupSetBits(m_worker_events, Event::WAKE);
}

void EventLoop::worker_thread()
{
    while (m_worker_alive) {
        const Timepoint now = m_get_time_cb();
        Timepoint next_delay = 0;

        if (m_worker_stopped) {
            xEventGroupSetBits(m_worker_events, Event::STOP);
        }
        else {
            process_immediate_tasks();
            next_delay = process_deferred_tasks(now);
        }

        xEventGroupWaitBits(m_worker_events, Event::WAKE, pdTRUE, pdFALSE, pdMS_TO_TICKS(next_delay));
    }
    xEventGroupSetBits(m_worker_events, Event::EXIT);
}

void EventLoop::process_immediate_tasks()
{
    std::lock_guard lock(m_immediate_mutex);
    while (!m_immediate_tasks.empty()) {
        Task task;
        m_immediate_tasks.pop_into(task);
        m_immediate_mutex.unlock();
        task();
        m_immediate_mutex.lock();
    }
}

EventLoop::Timepoint EventLoop::process_deferred_tasks(Timepoint start_tp)
{
    // TODO: Implement
    return 0;
}

}  // namespace service
