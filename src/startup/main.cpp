#include "FreeRTOS.h"
#include "task.h"

#include "pico/stdlib.h"

#include <cassert>

int main()
{
    // TODO: add logic here

    NEVER_RETURN;
}

extern "C" void vApplicationMallocFailedHook()
{
    configASSERT(false);
}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t task_hdl, char* task_name)
{
    configASSERT(false);
}

extern "C" void vAssertCalled(const char* file_name, unsigned line)
{
    assert(false);
    NEVER_RETURN;
}
