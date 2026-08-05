#include "register_watch.h"

#include <Arduino.h>

#include "config.h"
#include "developer_registers.h"
#include "modbus_manager.h"
#include "register_probe.h"
#include "register_recorder.h"

unsigned long lastUpdate = 0;

void registerWatchBegin()
{
}

void registerWatchLoop()
{
    if (!watchEnabled)
        return;

    if (!modbusConnected())
        return;

    if (millis() - lastUpdate < 1000)
        return;

    lastUpdate = millis();

    Serial.println();
    Serial.println("==============================================================");

    for (uint16_t reg = watchStartRegister;
         reg <= watchEndRegister;
         reg++)
    {
        uint16_t value;

        if (!readHoldingRegister(watchUnit, reg, value))
            continue;

        registerRecorderUpdate(reg, value);

        const DeveloperRegister *info =
            findDeveloperRegister(watchUnit, reg);

        if (info)
        {
            Serial.printf("%-28s %8d %s\n",
                          info->name,
                          (int16_t)value,
                          info->units);
        }
        else
        {
            if (value == 0 || value == 0xFFFF)
                continue;

            Serial.printf("R%-26u %8d\n",
                          reg,
                          (int16_t)value);
        }

        delay(2);
    }

    Serial.println();
    Serial.println("==============================================================");
}