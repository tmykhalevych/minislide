#pragma once

#include <FreeRTOS.h>
#include <task.h>

namespace fr::prio
{

enum : UBaseType_t
{
    LOW = 0,
    MEDIUM,
    HIGH,
    REAL_TIME,
    MAX
};

}  // namespace fr::prio

static_assert(fr::prio::MAX == configMAX_PRIORITIES);
