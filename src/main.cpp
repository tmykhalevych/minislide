#include <FreeRTOS.h>
#include <task.h>

#include <hal/hal.hpp>
#include <hal/status_led.hpp>

#include <cassert>

int main()
{
    hal::init();

    auto test_task = [](void*) {
        using State = hal::StatusLed::State;
        auto state = State::ON;
        while (true) {
            hal::StatusLed::set_state(state);
            vTaskDelay(pdMS_TO_TICKS(2'000));
            state = (state == State::ON) ? State::OFF : State::ON;
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
