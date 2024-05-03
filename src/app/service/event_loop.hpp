#pragma once

#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>

#include <assert.hpp>
#include <inplace_function.hpp>
#include <logger.hpp>
#include <mutex.hpp>
#include <prohibit_copy_move.hpp>

#include <etl/priority_queue.h>
#include <etl/queue.h>

#include <string_view>

namespace service
{

class EventLoop : public common::ProhibitCopyMove
{
public:
    static constexpr size_t TASK_QUEUE_CAPACITY = 10;

    using Task = common::InplaceFunction<void()>;
    using Timepoint = uint;

    void run(Task&& task) {}
    void run_after(Task&& task, Timepoint delay_ms) {}
    void run_periodic(Task&& task, Timepoint period_ms) {}

protected:
    EventLoop(std::string_view name) : m_name(name) {}

    void suspend() {}
    void resume() {}

    [[nodiscard]] std::string_view name() const { return m_name; }

private:
    class DeferredTask : public Task
    {
    public:
        using Task::Task;

        void set_deadline(Timepoint deadline) { m_deadline = deadline; }

        [[nodiscard]] Timepoint get_deadline() const { return m_deadline; }
        [[nodiscard]] bool operator<(const DeferredTask& other) const { return m_deadline < other.m_deadline; }

    private:
        Timepoint m_deadline = 0;
    };

    std::string m_name;

    etl::queue<Task, TASK_QUEUE_CAPACITY> m_immediate_tasks;
    freertos::Mutex m_immediate_mutex;

    etl::priority_queue<DeferredTask, TASK_QUEUE_CAPACITY> m_deferred_tasks;
    freertos::Mutex m_deferred_mutex;
};

}  // namespace service
