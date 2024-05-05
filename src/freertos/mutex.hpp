#pragma once

#include <FreeRTOS.h>
#include <semphr.h>

#include <assert.hpp>
#include <prohibit_copy_move.hpp>

#include <mutex>

namespace freertos
{

namespace details
{

class MutexBase : public common::ProhibitCopy
{
public:
    MutexBase(SemaphoreHandle_t hdl) : m_hdl(hdl) { ASSERT(m_hdl); }
    ~MutexBase() { vSemaphoreDelete(m_hdl); }

protected:
    SemaphoreHandle_t m_hdl;
};

}  // namespace details

/// @brief Very basic std-compatible wrapper for FreeRTOS mutexes
class Mutex : public details::MutexBase
{
public:
    Mutex() : MutexBase(xSemaphoreCreateMutex()) {}

    void lock() { ASSERT(xSemaphoreTake(m_hdl, portMAX_DELAY)); }
    void unlock() { ASSERT(xSemaphoreGive(m_hdl)); }
};

/// @brief Very basic std-compatible wrapper for FreeRTOS recursive mutexes
class RecursiveMutex : public details::MutexBase
{
public:
    RecursiveMutex() : MutexBase(xSemaphoreCreateRecursiveMutex()) {}

    void lock() { ASSERT(xSemaphoreTakeRecursive(m_hdl, portMAX_DELAY)); }
    void unlock() { ASSERT(xSemaphoreGiveRecursive(m_hdl)); }
};

}  // namespace freertos
