#pragma once

#include <FreeRTOS.h>
#include <event_groups.h>
#include <task.h>

#include <bind_to.hpp>
#include <inplace_function.hpp>
#include <logger.hpp>
#include <mutex.hpp>
#include <prohibit_copy_move.hpp>

#include <etl/priority_queue.h>
#include <etl/queue.h>
#include <etl/unordered_set.h>

#include <optional>
#include <string_view>

namespace service
{

using Task = common::InplaceFunction<void()>;
using Milliseconds = TickType_t;
using get_time_cb_t = common::InplaceFunction<Milliseconds()>;

template <size_t StackSize, size_t ImmediateQueueCapacity, size_t DeferredQueueCapacity>
class EventLoop : public common::ProhibitCopyMove
{
private:
    using TaskHandleImpl = uint;

public:
    class TaskHandle
    {
    public:
        explicit TaskHandle(EventLoop& loop, TaskHandleImpl impl) : m_loop(loop), m_impl(impl) {}
        void cancel();

        [[nodiscard]] bool is_canceled() const { return m_canceled; }

    private:
        EventLoop& m_loop;
        TaskHandleImpl m_impl;
        bool m_canceled = false;
    };

    void run_immediate(Task&& task);
    TaskHandle run_after(Task&& task, Milliseconds delay);
    TaskHandle run_periodic(Task&& task, Milliseconds period, Milliseconds delay = 0);

    ~EventLoop();

protected:
    explicit EventLoop(std::string_view name, size_t prio, get_time_cb_t get_time_cb);

    void suspend();
    void resume();

private:
    struct DeferredTask
    {
        TaskHandleImpl handle;
        Task task;
        Milliseconds deadline;
        std::optional<Milliseconds> period = std::nullopt;

        [[nodiscard]] bool operator<(const DeferredTask& other) const { return deadline < other.deadline; }
        [[nodiscard]] static TaskHandleImpl get_next_handle() { return next_handle++; }

    private:
        inline static TaskHandleImpl next_handle = 0;
    };

    enum Event : EventBits_t
    {
        WAKE = (1 << 0),
        STOP = (1 << 1),
        EXIT = (1 << 2)
    };

    void worker_thread();
    void process_immediate_tasks();
    TickType_t process_deferred_tasks(Milliseconds start_timepoint);

    get_time_cb_t m_get_time_cb;

    etl::queue<Task, ImmediateQueueCapacity> m_immediate_tasks;
    freertos::RecursiveMutex m_immediate_mutex;

    etl::priority_queue<DeferredTask, DeferredQueueCapacity> m_deferred_tasks;
    etl::unordered_set<TaskHandleImpl, DeferredQueueCapacity> m_deferred_handles;
    freertos::RecursiveMutex m_deferred_mutex;

    TaskHandle_t m_worker_handle;
    EventGroupHandle_t m_worker_events;
    bool m_worker_stopped = false;
    bool m_worker_alive = true;
};

template <size_t S, size_t I, size_t D>
EventLoop<S, I, D>::EventLoop(std::string_view name, size_t prio, get_time_cb_t get_time_cb)
    : m_get_time_cb(std::move(get_time_cb)), m_worker_events(xEventGroupCreate())
{
    const BaseType_t status =
        xTaskCreate(common::bind_to<EventLoop<S, I, D>, &EventLoop::worker_thread>, name.data(),
                    static_cast<configSTACK_DEPTH_TYPE>(S), this, static_cast<UBaseType_t>(prio), &m_worker_handle);

    ASSERT(m_worker_events);
    ASSERT(status == pdPASS);
}

template <size_t S, size_t I, size_t D>
EventLoop<S, I, D>::~EventLoop()
{
    m_worker_alive = false;
    xEventGroupSetBits(m_worker_events, Event::WAKE);
    xEventGroupWaitBits(m_worker_events, Event::EXIT, pdTRUE, pdFALSE, portMAX_DELAY);

    vTaskDelete(m_worker_handle);
    vEventGroupDelete(m_worker_events);
}

template <size_t S, size_t I, size_t D>
void EventLoop<S, I, D>::TaskHandle::cancel()
{
    std::lock_guard lock(m_loop.m_deferred_mutex);
    m_loop.m_deferred_handles.erase(m_impl);
    m_canceled = true;
}

template <size_t S, size_t I, size_t D>
void EventLoop<S, I, D>::run_immediate(Task&& task)
{
    {
        std::lock_guard lock(m_immediate_mutex);
        m_immediate_tasks.push(std::move(task));
    }

    xEventGroupSetBits(m_worker_events, Event::WAKE);
}

template <size_t S, size_t I, size_t D>
EventLoop<S, I, D>::TaskHandle EventLoop<S, I, D>::run_after(Task&& task, Milliseconds delay)
{
    const Milliseconds now = m_get_time_cb();
    const TaskHandleImpl handle = DeferredTask::get_next_handle();

    {
        std::lock_guard lock(m_deferred_mutex);
        m_deferred_tasks.push(DeferredTask{.handle = handle, .task = std::move(task), .deadline = now + delay});
        m_deferred_handles.insert(handle);
    }

    xEventGroupSetBits(m_worker_events, Event::WAKE);
    return TaskHandle{*this, handle};
}

template <size_t S, size_t I, size_t D>
EventLoop<S, I, D>::TaskHandle EventLoop<S, I, D>::run_periodic(Task&& task, Milliseconds period, Milliseconds delay)
{
    const Milliseconds now = m_get_time_cb();
    const TaskHandleImpl handle = DeferredTask::get_next_handle();

    {
        std::lock_guard lock(m_deferred_mutex);
        m_deferred_tasks.push(
            DeferredTask{.handle = handle, .task = std::move(task), .deadline = now + delay, .period = period});
        m_deferred_handles.insert(handle);
    }

    xEventGroupSetBits(m_worker_events, Event::WAKE);
    return TaskHandle{*this, handle};
}

template <size_t S, size_t I, size_t D>
void EventLoop<S, I, D>::suspend()
{
    m_worker_stopped = true;
    xEventGroupSetBits(m_worker_events, Event::WAKE);
    xEventGroupWaitBits(m_worker_events, Event::STOP, pdTRUE, pdFALSE, portMAX_DELAY);
    vTaskSuspend(m_worker_handle);

    {
        std::lock_guard lock(m_immediate_mutex);
        if (!m_immediate_tasks.empty()) {
            LOG_WARN("clearing immediate queue");
            m_immediate_tasks.clear();
        }
    }
}

template <size_t S, size_t I, size_t D>
void EventLoop<S, I, D>::resume()
{
    m_worker_stopped = false;
    vTaskResume(m_worker_handle);
    xEventGroupSetBits(m_worker_events, Event::WAKE);
}

template <size_t S, size_t I, size_t D>
void EventLoop<S, I, D>::worker_thread()
{
    while (m_worker_alive) {
        const Milliseconds now = m_get_time_cb();
        TickType_t next_delay_ticks = portMAX_DELAY;

        if (m_worker_stopped) {
            xEventGroupSetBits(m_worker_events, Event::STOP);
        }
        else {
            process_immediate_tasks();
            next_delay_ticks = process_deferred_tasks(now);
        }

        xEventGroupWaitBits(m_worker_events, Event::WAKE, pdTRUE, pdFALSE, next_delay_ticks);
    }

    xEventGroupSetBits(m_worker_events, Event::EXIT);
}

template <size_t S, size_t I, size_t D>
void EventLoop<S, I, D>::process_immediate_tasks()
{
    std::lock_guard lock(m_immediate_mutex);

    Task current_task;

    while (!m_immediate_tasks.empty()) {
        m_immediate_tasks.pop_into(current_task);
        m_immediate_mutex.unlock();
        current_task();
        m_immediate_mutex.lock();
    }
}

template <size_t S, size_t I, size_t D>
TickType_t EventLoop<S, I, D>::process_deferred_tasks(Milliseconds start_timepoint)
{
    std::lock_guard lock(m_deferred_mutex);

    DeferredTask current_deferred_task;
    TickType_t next_delay_ticks = portMAX_DELAY;

    while (!m_deferred_tasks.empty()) {
        const Milliseconds next_deadline = m_deferred_tasks.top().deadline;

        if (next_deadline > start_timepoint) {
            next_delay_ticks = pdMS_TO_TICKS(next_deadline - start_timepoint);
            break;
        }

        m_deferred_tasks.pop_into(current_deferred_task);
        m_deferred_mutex.unlock();
        current_deferred_task.task();
        m_deferred_mutex.lock();

        // handle periodic tasks
        if (current_deferred_task.period) {
            current_deferred_task.deadline += current_deferred_task.period.value();
            m_deferred_tasks.push(std::move(current_deferred_task));
        }
    }

    return next_delay_ticks;
}

}  // namespace service
