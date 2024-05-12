#include <FreeRTOS.h>
#include <task.h>

#include <firmware.hpp>

#include <cstdio>

int main()
{
    auto firmware = fw::Firmware();
    firmware.start();
    return 0;
}

extern "C" void vApplicationMallocFailedHook()
{
    std::printf("allocation failure\n");
    while (true) {
    }
}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t task_hdl, char* task_name)
{
    (void)task_hdl;
    std::printf("stach overflow [tsk:%s]\n", task_name);
    while (true) {
    }
}

extern "C" void vAssertCalled(const char* file_name, unsigned line)
{
    std::printf("assert at %s:%u\n", file_name, line);
    while (true) {
    }
}
