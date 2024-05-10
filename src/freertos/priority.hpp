#pragma once

#include <FreeRTOS.h>
#include <task.h>

namespace freertos::prio
{

enum : UBaseType_t
{
    LOW = 0,
    MEDIUM,
    HIGH,
    REAL_TIME,
    MAX
};

}  // namespace freertos::prio

static_assert(freertos::prio::MAX == configMAX_PRIORITIES);
