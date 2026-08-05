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

    const DeveloperRegister* reg = developerRegisterAt(currentIndex);

    if (!reg)
    {
        Serial.println("End of register table");
        currentIndex = 0;
        return;
    }

    Serial.printf(
        "Index=%u  Unit=%u  Reg=%u  Auto=%s\n",
        currentIndex,
        reg->unit,
        reg->address,
        reg->autoPoll ? "YES" : "NO");

    currentIndex++;

    if (!reg->autoPoll)
        return;

    uint16_t value;

    if (!readHoldingRegister(reg->unit, reg->address, value))
    {
        Serial.println("Read FAILED");
        return;
    }

    Serial.printf("Value=%d\n", (int16_t)value);

    registerMqttPublish(
        reg->unit,
        reg->address,
        (int16_t)value);
}