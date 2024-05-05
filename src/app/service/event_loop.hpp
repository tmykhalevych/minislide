#pragma once

#include <FreeRTOS.h>
#include <event_groups.h>
#include <task.h>

#include <inplace_function.hpp>
#include <mutex.hpp>
#include <prohibit_copy_move.hpp>

#include <etl/priority_queue.h>
#include <etl/queue.h>
#include <etl/unordered_set.h>

#include <optional>
#include <string_view>

namespace service
{

class EventLoop : public common::ProhibitCopyMove
{
private:
    using DeferredTaskHandle = uint;

public:
    static constexpr size_t TASK_QUEUE_CAPACITY = 10;

    using Task = common::InplaceFunction<void()>;
    using Timepoint = uint;

    using get_time_cb_t = common::InplaceFunction<Timepoint()>;

    class TaskHandle
    {
    public:
        explicit TaskHandle(EventLoop& loop, DeferredTaskHandle value) : m_loop(loop), m_value(value) {}
        void cancel();

        [[nodiscard]] bool is_canceled() const { return m_canceled; }

    private:
        EventLoop& m_loop;
        DeferredTaskHandle m_value;
        bool m_canceled = false;
    };

    void run(Task&& task);
    TaskHandle run_after(Task&& task, Timepoint delay_ms);
    TaskHandle run_periodic(Task&& task, Timepoint period_ms, Timepoint delay_ms = 0);

    ~EventLoop();

protected:
    explicit EventLoop(std::string_view name, size_t stack_size, size_t prio, get_time_cb_t get_time_cb);

    void suspend();
    void resume();

    [[nodiscard]] std::string_view name() const { return m_name; }

private:
    struct DeferredTask
    {
        DeferredTaskHandle handle;
        Task task;
        Timepoint deadline;
        std::optional<Timepoint> period = std::nullopt;

        [[nodiscard]] bool operator<(const DeferredTask& other) const { return deadline < other.deadline; }
        [[nodiscard]] static DeferredTaskHandle get_next_handle() { return next_handle++; }

    private:
        inline static DeferredTaskHandle next_handle = 0;
    };

    enum Event : EventBits_t
    {
        WAKE = (1 << 0),
        STOP = (1 << 1),
        EXIT = (1 << 2)
    };

    void worker_thread();
    void process_immediate_tasks();
    Timepoint process_deferred_tasks(Timepoint start_tp);

    std::string m_name;
    get_time_cb_t m_get_time_cb;

    etl::queue<Task, TASK_QUEUE_CAPACITY> m_immediate_tasks;
    freertos::RecursiveMutex m_immediate_mutex;

    etl::priority_queue<DeferredTask, TASK_QUEUE_CAPACITY> m_deferred_tasks;
    etl::unordered_set<DeferredTaskHandle, TASK_QUEUE_CAPACITY> m_deferred_handles;
    freertos::RecursiveMutex m_deferred_mutex;

    TaskHandle_t m_worker_handle;
    EventGroupHandle_t m_worker_events;
    bool m_worker_stopped = false;
    bool m_worker_alive = true;
};

}  // namespace service
