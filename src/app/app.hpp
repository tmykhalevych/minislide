#pragma once

#include <FreeRTOS.h>

#include <led.hpp>
#include <logger.hpp>
#include <service.hpp>

// clang-format off

#define REQUIRED(val) if (!(val)) return false
#define OPTIONAL(val) val

// clang-format on

namespace app
{

[[nodiscard]] inline bool init()
{
    logger::create_and_start(logger::Severity::DEBUG);

    REQUIRED(service::create_and_start<LedService>());

    return true;
}

inline void start()
{
    vTaskStartScheduler();
}

}  // namespace app
