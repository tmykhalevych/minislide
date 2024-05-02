#include <FreeRTOS.h>

#include <app.hpp>
#include <assert.hpp>
#include <hal/hal.hpp>

int main()
{
    ASSERT(hal::init());
    ASSERT(app::init());
    app::start();
    return 0;
}

extern "C" void vApplicationMallocFailedHook()
{
    LOG_FATAL("allocation failure");
}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t task_hdl, char* task_name)
{
    (void)task_hdl;
    LOG_FATAL("stach overflow [tsk:%s]", task_name);
}

extern "C" void vAssertCalled(const char* file_name, unsigned line)
{
    std::printf("assert at %s:%u\n", file_name, line);
    while (true) {
    }
}
