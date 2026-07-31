#pragma once

#include <Arduino.h>

namespace Victron
{
    struct RegisterInfo
    {
        uint16_t address;
        float scale;
        bool isSigned;
        const char* name;
        const char* units;
    };

    namespace Unit
    {
        constexpr uint8_t SYSTEM  = 100;
        constexpr uint8_t BATTERY = 225;
        constexpr uint8_t VEBUS   = 227;
        constexpr uint8_t PV1     = 34;
        constexpr uint8_t PV2     = 35;
    }

    namespace System
    {
        constexpr RegisterInfo BatteryVoltage =
        {
            840,
            100.0f,
            false,
            "Battery Voltage",
            "V"
        };

        constexpr RegisterInfo BatteryCurrent =
        {
            841,
            10.0f,
            true,
            "Battery Current",
            "A"
        };

        constexpr RegisterInfo BatteryPower =
        {
            842,
            1.0f,
            true,
            "Battery Power",
            "W"
        };

        constexpr RegisterInfo BatterySOC =
        {
            843,
            1.0f,
            false,
            "Battery SOC",
            "%"
        };
    }

    namespace Battery
    {
        constexpr RegisterInfo Voltage =
        {
            259,
            100.0f,
            false,
            "Battery Voltage",
            "V"
        };

        constexpr RegisterInfo Current =
        {
            261,
            10.0f,
            true,
            "Battery Current",
            "A"
        };

        constexpr RegisterInfo Power =
        {
            260,
            1.0f,
            true,
            "Battery Power",
            "W"
        };

        constexpr RegisterInfo SOC =
        {
            266,
            10.0f,
            false,
            "Battery SOC",
            "%"
        };
    }
}