#pragma once

#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>

#include <assert.hpp>
#include <inplace_function.hpp>
#include <logger.hpp>
#include <prohibit_copy_move.hpp>

namespace service
{

class EventLoop : public common::ProhibitCopyMove
{
public:
    using Task = common::InplaceFunction<void()>;

    void run(Task&& task) {}
    void run_after(Task&& task, uint delay_ms) {}
    void run_periodic(Task&& task, uint period_ms) {}

protected:
    void suspend() {}
    void resume() {}
};

}  // namespace service
