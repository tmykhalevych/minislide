#include <FreeRTOS.h>
#include <task.h>

#include <hal/hal.hpp>
#include <hal/status_led.hpp>
#include <logger.hpp>

int main()
{
    hal::init();

    logger::create_and_start(logger::Severity::DEBUG);

    auto test_task = [](void*) {
        using State = hal::StatusLed::State;
        auto state = State::ON;
        while (true) {
            LOG_INFO("LED is %s", (state == State::ON) ? "ON" : "OFF");
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
    LOG_FATAL("allocation failure");
}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t task_hdl, char* task_name)
{
    (void)task_hdl;
    LOG_FATAL("stach overflow [tsk:%s]", task_name);
}

extern "C" void vAssertCalled(const char* file_name, unsigned line)
{
    std::printf("assert at %s:%u", file_name, line);
    while (true) {
    }
}
