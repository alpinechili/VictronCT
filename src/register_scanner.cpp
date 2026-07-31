#include "register_scanner.h"

#include <Arduino.h>

#include "config.h"
#include "modbus_manager.h"

namespace
{
    constexpr uint16_t START_REGISTER = 840;
    constexpr uint16_t END_REGISTER   = 980;

    constexpr bool SHOW_ZERO = true;

    bool scanComplete = false;
    unsigned long startDelay = 0;
}

void registerScannerBegin()
{
    scanComplete = false;
    startDelay = millis();
}

void registerScannerLoop()
{
    if (scanComplete)
        return;

    if (!modbusConnected())
        return;

    // Give the Cerbo time to settle
    if (millis() - startDelay < 5000)
        return;

    Serial.println();
    Serial.println("==============================================================");
    Serial.printf("Scanning Unit %u   Registers %u - %u\n",
                  CERBO_UNIT,
                  START_REGISTER,
                  END_REGISTER);
    Serial.println("==============================================================");
    Serial.println();
    Serial.println(" Reg    Unsigned   Signed      Hex");
    Serial.println("--------------------------------------------------------------");

    for (uint16_t reg = START_REGISTER; reg <= END_REGISTER; reg++)
    {
        uint16_t value;

        if (!readHoldingRegister(CERBO_UNIT, reg, value))
            continue;

        if (!SHOW_ZERO && value == 0)
            continue;

        Serial.printf("R%-4u %8u %8d   0x%04X\n",
                      reg,
                      value,
                      (int16_t)value,
                      value);

        delay(2);
    }

    Serial.println();
    Serial.println("==============================================================");
    Serial.println("Scan complete.");
    Serial.println("==============================================================");
    Serial.println();

    scanComplete = true;
}