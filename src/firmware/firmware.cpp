#include <FreeRTOS.h>
#include <task.h>

#include <assert.hpp>
#include <bsp.hpp>
#include <firmware.hpp>
#include <logger.hpp>
#include <priority.hpp>
#include <service.hpp>
#include <state.hpp>

#include <led_receiver.hpp>
#include <led_sender.hpp>
#include <shell.hpp>

// clang-format off
#define REQUIRE(val) if (!(val)) return false
// clang-format on

namespace fw
{

Firmware::Firmware()
{
    const BaseType_t status = xTaskCreate(cmn::bind_to<Firmware, &Firmware::init>, "fw", configMINIMAL_STACK_SIZE, this,
                                          fr::prio::REAL_TIME, nullptr);

    ASSERT(status == pdPASS);
}

void Firmware::start() const
{
    vTaskStartScheduler();
}

bool Firmware::init_bsp() const
{
    logger::create_and_start(logger::Severity::DEBUG);

    bool res = bsp::init();
    LOG_INFO("init bsp");

    return res;
}

bool Firmware::init_services() const
{
    LOG_INFO("init firmware");

    REQUIRE(svc::create_and_start<Shell>());
    REQUIRE(svc::create_and_start<LedReceiver>());
    REQUIRE(svc::create_and_start<LedSender>());

    auto led_sender_state = svc::get_state_for<LedSender>();
    LOG_INFO("led sender init state: %u", led_sender_state);

    return true;
}

void Firmware::init()
{
    ASSERT(init_bsp());
    ASSERT(init_services());

    vTaskDelete(nullptr);
}

}  // namespace fw
