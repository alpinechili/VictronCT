#include "register_scanner.h"

#include <Arduino.h>

#include "config.h"
#include "modbus_manager.h"

static uint16_t previousValues[1000];
static bool firstScan = true;

namespace
{
    constexpr uint16_t START_REGISTER = 800;
    constexpr uint16_t END_REGISTER   = 1100;

    constexpr bool SHOW_ZERO = true;

    bool scanRequested = false;
    unsigned long startDelay = 0;
}

void registerScannerBegin()
{
    // Automatically perform one scan after boot
    scanRequested = true;
    startDelay = millis();
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

    // Give the Cerbo time to settle
    if (millis() - startDelay < 5000)
        return;

    Serial.println();
    Serial.println("==============================================================");

    if (firstScan)
        Serial.println("Initial Register Scan");
    else
        Serial.println("Changed Registers");

    Serial.printf("Unit %u   Registers %u - %u\n",
                  CERBO_UNIT,
                  START_REGISTER,
                  END_REGISTER);

    Serial.println("==============================================================");
    Serial.println();

    if (firstScan)
    {
        Serial.println(" Reg    Unsigned   Signed      Hex");
        Serial.println("--------------------------------------------------------------");
    }

    uint16_t changedCount = 0;

    for (uint16_t reg = START_REGISTER; reg <= END_REGISTER; reg++)
    {
        uint16_t value;

        if (!readHoldingRegister(CERBO_UNIT, reg, value))
            continue;

        if (!SHOW_ZERO && value == 0)
            continue;

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

    if (!firstScan)
    {
        Serial.printf("Total Changed Registers : %u\n",
                      changedCount);
        Serial.println();
    }

    Serial.println("==============================================================");
    Serial.println("Scan complete.");
    Serial.println("==============================================================");
    Serial.println();

    firstScan = false;
    scanRequested = false;

    Serial.println("======================================");
    Serial.println("Scan finished.");
    Serial.println("Copy the output now.");
    Serial.println("Press the ESP32 RESET button to scan again.");
    Serial.println("======================================");

    // Temporary debugging pause
    while (true)
    {
        delay(1000);
    }
}