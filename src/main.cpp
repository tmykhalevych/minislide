#include <FreeRTOS.h>
#include <task.h>

#include <hal/gpio.hpp>

#include <cassert>

#define LED_GPIO 25

int main()
{
    auto test_task = [](void*) {
        hal::GpioOut led(LED_GPIO);

        hal::GpioState state = hal::GpioState::ON;
        while (true) {
            led.write(state);
            vTaskDelay(pdMS_TO_TICKS(500));
            state = (state == hal::GpioState::ON) ? hal::GpioState::OFF : hal::GpioState::ON;
        }
    };

    xTaskCreate(test_task, "test", configMINIMAL_STACK_SIZE, nullptr, tskIDLE_PRIORITY, nullptr);

    vTaskStartScheduler();
    return 0;
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
    while (true) {
    }
}
