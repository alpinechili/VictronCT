#pragma once

#include <stdint.h>

struct RegisterDefinition
{
    uint8_t unit;
    uint16_t address;

    const char* name;
    const char* units;

    float scale;

    bool isSigned;
    bool writable;
};

namespace VictronRegisters
{
    //==================================================
    // Named Register Addresses
    //==================================================

    constexpr uint16_t AC_CONSUMPTION_L1 = 817;
    constexpr uint16_t GRID_POWER         = 820;

    constexpr uint16_t PV_INVERTER1_POWER = 808;
    constexpr uint16_t PV_INVERTER2_POWER = 893;

    constexpr uint16_t BATTERY_VOLTAGE    = 840;
    constexpr uint16_t BATTERY_CURRENT    = 841;
    constexpr uint16_t BATTERY_POWER      = 842;
    constexpr uint16_t BATTERY_SOC        = 843;

    constexpr uint16_t PV_DC_POWER        = 850;
    constexpr uint16_t CHARGER_POWER      = 855;

    //==================================================
    // Register Database
    //==================================================

    constexpr RegisterDefinition Table[] =
    {
        {100, AC_CONSUMPTION_L1, "AC Consumption L1", "W",  1.0f, false, false},
        {100, GRID_POWER,        "Grid Power",        "W",  1.0f, true,  false},

        {100, BATTERY_VOLTAGE,   "Battery Voltage",   "V", 10.0f, false, false},
        {100, BATTERY_CURRENT,   "Battery Current",   "A", 10.0f, true,  false},
        {100, BATTERY_POWER,     "Battery Power",     "W",  1.0f, true,  false},
        {100, BATTERY_SOC,       "Battery SOC",       "%",  1.0f, false, false},

        {100, PV_INVERTER1_POWER,"PV Inverter 1",     "W",  1.0f, false, false},
        {100, PV_INVERTER2_POWER,"PV Inverter 2",     "W",  1.0f, false, false},

        {100, PV_DC_POWER,       "PV DC Power",       "W",  1.0f, false, false},
        {100, CHARGER_POWER,     "Charger Power",     "W",  1.0f, false, false},
    };

    constexpr uint16_t Count =
        sizeof(Table) / sizeof(Table[0]);

    inline const RegisterDefinition* find(
        uint8_t unit,
        uint16_t address)
    {
        for (uint16_t i = 0; i < Count; i++)
        {
            if (Table[i].unit == unit &&
                Table[i].address == address)
            {
                return &Table[i];
            }
        }

        return nullptr;
    }
}