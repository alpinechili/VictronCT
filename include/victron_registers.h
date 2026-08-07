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
    constexpr RegisterDefinition Table[] =
    {
        //==================================================
        // System (Unit 100)
        //==================================================

        {100, 817, "AC Consumption L1",      "W",  1.0f, false, false},
        {100, 820, "Grid Power",             "W",  1.0f, true,  false},

        {100, 840, "Battery Voltage",        "V", 10.0f, false, false},
        {100, 841, "Battery Current",        "A", 10.0f, true,  false},
        {100, 842, "Battery Power",          "W",  1.0f, true,  false},
        {100, 843, "Battery SOC",            "%",  1.0f, false, false},

        {100, 808, "PV Inverter 1 Power",    "W",  1.0f, false, false},
        {100, 893, "PV Inverter 2 Power",    "W",  1.0f, false, false},

        {100, 850, "PV DC Power",            "W",  1.0f, false, false},
        {100, 855, "Charger Power",          "W",  1.0f, false, false},
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