#pragma once

#include <FreeRTOS.h>
#include <semphr.h>

#include <assert.hpp>
#include <prohibit_copy_move.hpp>

#include <mutex>

namespace freertos
{

/// @brief Very basic std-compatible wrapper for FreeRTOS mutexes
class Mutex : common::ProhibitCopy
{
public:
    Mutex() : m_hdl(xSemaphoreCreateMutex()) { ASSERT(m_hdl); }

    void lock() { ASSERT(xSemaphoreTake(m_hdl, portMAX_DELAY)); }
    void unlock() { ASSERT(xSemaphoreGive(m_hdl)); }

private:
    SemaphoreHandle_t m_hdl;
};

/// @brief Very basic std-compatible wrapper for FreeRTOS recursive mutexes
class RecursiveMutex : common::ProhibitCopy
{
public:
    RecursiveMutex() : m_hdl(xSemaphoreCreateRecursiveMutex()) { ASSERT(m_hdl); }

    void lock() { ASSERT(xSemaphoreTakeRecursive(m_hdl, portMAX_DELAY)); }
    void unlock() { ASSERT(xSemaphoreGiveRecursive(m_hdl)); }

private:
    SemaphoreHandle_t m_hdl;
};

}  // namespace freertos
