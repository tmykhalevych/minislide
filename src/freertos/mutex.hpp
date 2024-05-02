#pragma once

#include <prohibit_copy_move.hpp>

#include <FreeRTOS.h>
#include <semphr.h>

#include <mutex>

namespace freertos
{

/// @brief Very basic std-compatible wrapper for FreeRTOS mutexes
class Mutex : common::ProhibitCopy
{
public:
    Mutex() : m_hdl(xSemaphoreCreateMutex()) { configASSERT(m_hdl); }

    void lock() { xSemaphoreTake(m_hdl, portMAX_DELAY); }
    void unlock() { xSemaphoreGive(m_hdl); }

private:
    SemaphoreHandle_t m_hdl;
};

}  // namespace freertos
