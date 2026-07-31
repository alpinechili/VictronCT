#pragma once

#include <stdint.h>

struct VictronData
{
    // Connection status
    bool online = false;

    // Battery
    float batteryVoltage = 0.0f;
    float batteryCurrent = 0.0f;
    int16_t batteryPower = 0;
    float batterySOC = 0.0f;      // Keep the existing name

    // Grid
    int16_t gridPowerL1 = 0;

    // AC Consumption
    uint16_t acConsumptionL1 = 0;
    int32_t acConsumptionInputL1 = 0;
    int32_t acConsumptionOutputL1 = 0;

    // PV
    uint16_t pvAcOutputL1 = 0;
            float pvDcCurrent = 0.0f;

    // Charger
    uint16_t chargerPower = 0;

    // DC
    int16_t dcSystemPower = 0;

    // VE.Bus
    float veBusCurrent = 0.0f;
    int16_t veBusPower = 0;
};

extern VictronData victron;