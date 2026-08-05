#include "register_poller.h"

#include <Arduino.h>

#include "config.h"
#include "developer_registers.h"
#include "modbus_manager.h"
#include "register_mqtt.h"

static uint16_t currentIndex = 0;
static unsigned long lastPoll = 0;

void registerPollerBegin()
{
    currentIndex = 0;
}

void registerPollerLoop()
{
    if (!ENABLE_REGISTER_POLLER)
        return;

    if (!modbusConnected())
        return;

    if (millis() - lastPoll < 250)
        return;

    lastPoll = millis();

    const DeveloperRegister* reg = nullptr;

    while (true)
    {
        reg = developerRegisterAt(currentIndex);

        currentIndex++;

        if (currentIndex >= developerRegisterCount())
            currentIndex = 0;

        if (!reg)
            return;

        if (reg->autoPoll)
            break;
    }

    uint16_t value;

    if (readHoldingRegister(
            reg->unit,
            reg->address,
            value))
    {
        registerMqttPublish(
            reg->unit,
            reg->address,
            (int16_t)value);
    }
}