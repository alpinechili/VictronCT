#pragma once

#include <stdint.h>

struct VictronData
{
    //==================================================
    // Connection
    //==================================================

    bool online = false;

    //==================================================
    // Battery
    //==================================================

    float batteryVoltage = 0.0f;
    float batteryCurrent = 0.0f;
    int16_t batteryPower = 0;
    float batterySOC = 0.0f;

    //==================================================
    // Grid
    //==================================================

    int16_t gridPowerL1 = 0;

    //==================================================
    // AC Consumption
    //==================================================

    uint16_t acConsumptionL1 = 0;
    int32_t acConsumptionInputL1 = 0;
    int32_t acConsumptionOutputL1 = 0;

    //==================================================
    // AC Coupled PV (Hoymiles)
    //==================================================

    uint16_t pvInverter1Power = 0;     // Unit 100 Register 808
    uint16_t pvInverter2Power = 0;     // Unit 100 Register 893
    uint16_t pvTotalPower = 0;         // Calculated

    //==================================================
    // DC PV (Victron MPPT)
    //==================================================

    uint16_t pvDcPower = 0;
    float pvDcCurrent = 0.0f;

    //==================================================
    // Charger
    //==================================================

    uint16_t chargerPower = 0;

    //==================================================
    // DC System
    //==================================================

    int16_t dcSystemPower = 0;

    //==================================================
    // VE.Bus
    //==================================================

    float veBusCurrent = 0.0f;
    int16_t veBusPower = 0;
};

extern VictronData victron;