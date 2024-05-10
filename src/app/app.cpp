#include <FreeRTOS.h>
#include <task.h>

#include <app.hpp>
#include <assert.hpp>
#include <logger.hpp>
#include <platform.hpp>
#include <service.hpp>

#include <led_receiver.hpp>
#include <led_sender.hpp>

#define REQUIRED(val) \
    if (!(val)) return false

namespace app
{

App::App()
{
    const BaseType_t status =
        xTaskCreate(common::bind_to<App, &App::init>, "app", configMINIMAL_STACK_SIZE, this, tskIDLE_PRIORITY, nullptr);

    ASSERT(status == pdPASS);
}

void App::start() const
{
    vTaskStartScheduler();
}

bool App::init_platform() const
{
    bool res = platform::init();

    logger::create_and_start(logger::Severity::DEBUG);
    LOG_INFO("init platform");

    return res;
}

bool App::init_firmware() const
{
    LOG_INFO("init firmware");

    REQUIRED(service::create_and_start<LedReceiver>());
    REQUIRED(service::create_and_start<LedSender>());

    return true;
}

void App::init()
{
    ASSERT(init_platform());
    ASSERT(init_firmware());

    vTaskDelete(nullptr);
}

}  // namespace app
