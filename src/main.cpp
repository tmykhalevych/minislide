#include <FreeRTOS.h>
#include <task.h>

#include <hal/hal.hpp>
#include <hal/status_led.hpp>
#include <logger.hpp>
#include <service.hpp>

class TestService : public service::Service<TestService>
{
public:
    TestService() : Service("test") {}

    bool setup() { return true; }
    bool try_suspend() { return true; }
    bool try_resume() { return true; }

    void main()
    {
        run_periodic(
            [this] {
                m_led_state = (m_led_state == State::ON) ? State::OFF : State::ON;
                LOG_INFO("LED is %s", (m_led_state == State::ON) ? "ON" : "OFF");
                hal::StatusLed::set_state(m_led_state);
            },
            2'000);
    }

private:
    using State = hal::StatusLed::State;

    State m_led_state = State::OFF;
};

int main()
{
    hal::init();

    logger::create_and_start(logger::Severity::DEBUG);
    service::create_and_start<TestService>();

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
    std::printf("assert at %s:%u\n", file_name, line);
    while (true) {
    }
}
