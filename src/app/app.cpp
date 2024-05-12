#include <FreeRTOS.h>
#include <task.h>

#include <app.hpp>
#include <assert.hpp>
#include <logger.hpp>
#include <platform.hpp>
#include <priority.hpp>
#include <service.hpp>
#include <state.hpp>

#include <led_receiver.hpp>
#include <led_sender.hpp>

#define REQUIRED(val) \
    if (!(val)) return false

namespace app
{

App::App()
{
    const BaseType_t status =
        xTaskCreate(cmn::bind_to<App, &App::init>, "app", configMINIMAL_STACK_SIZE, this, fr::prio::REAL_TIME, nullptr);

    ASSERT(status == pdPASS);
}

void App::start() const
{
    vTaskStartScheduler();
}

bool App::init_platform() const
{
    bool res = pfm::init();

    logger::create_and_start(logger::Severity::DEBUG);
    LOG_INFO("init platform");

    return res;
}

bool App::init_firmware() const
{
    LOG_INFO("init firmware");

    REQUIRED(svc::create_and_start<LedReceiver>());
    REQUIRED(svc::create_and_start<LedSender>());

    auto led_sender_state = svc::get_state_for<LedSender>();
    LOG_INFO("led sender init state: %u", led_sender_state);

    return true;
}

void App::init()
{
    ASSERT(init_platform());
    ASSERT(init_firmware());

    vTaskDelete(nullptr);
}

}  // namespace app
