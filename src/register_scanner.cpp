#include "register_scanner.h"

#include <Arduino.h>

#include "config.h"
#include "modbus_manager.h"

static uint16_t previousValues[1000];
static bool validRegister[1000];
static bool firstScan = true;

namespace
{
    constexpr uint16_t START_REGISTER = 0;
    constexpr uint16_t END_REGISTER   = 999;

    constexpr bool SHOW_ZERO = false;

    bool scanRequested = false;
    unsigned long startDelay = 0;
}

void registerScannerBegin()
{
}

void registerScannerStart()
{
    scanRequested = true;
    startDelay = millis();
}

void registerScannerLoop()
{
    if (!scanRequested)
        return;

    if (!modbusConnected())
        return;

    if (millis() - startDelay < 1000)
        return;

    Serial.println();
    Serial.println("==============================================================");

    if (firstScan)
        Serial.println("Register Discovery");
    else
        Serial.println("Changed Registers");

    Serial.printf("Unit %u   Registers %u - %u\n",
                  CERBO_UNIT,
                  START_REGISTER,
                  END_REGISTER);

    Serial.println("==============================================================");
    Serial.println();

    uint16_t foundCount = 0;
    uint16_t changedCount = 0;

    if (firstScan)
    {
        Serial.println(" Reg    Unsigned   Signed      Hex");
        Serial.println("--------------------------------------------------------------");
    }

    for (uint16_t reg = START_REGISTER; reg <= END_REGISTER; reg++)
    {
        uint16_t value;

        if (!readHoldingRegister(CERBO_UNIT, reg, value))
            continue;

        validRegister[reg] = true;
        foundCount++;

        if (!SHOW_ZERO && (value == 0 || value == 0xFFFF))
        {
            previousValues[reg] = value;
            delay(2);
            continue;
        }

        if (firstScan)
        {
            Serial.printf("R%-4u %8u %8d   0x%04X\n",
                          reg,
                          value,
                          (int16_t)value,
                          value);
        }
        else
        {
            if (previousValues[reg] != value)
            {
                Serial.printf("R%-4u %8d -> %8d   (%+d)\n",
                              reg,
                              (int16_t)previousValues[reg],
                              (int16_t)value,
                              (int16_t)value - (int16_t)previousValues[reg]);

                changedCount++;
            }
        }

        previousValues[reg] = value;

        delay(2);
    }

    Serial.println();

    Serial.printf("Registers Found   : %u\n", foundCount);

    if (!firstScan)
        Serial.printf("Registers Changed : %u\n", changedCount);

    Serial.println();
    Serial.println("==============================================================");

    firstScan = false;
    scanRequested = false;
}