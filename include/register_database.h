#pragma once

#include <stdint.h>

enum class DataType
{
    UINT16,
    INT16
};

enum class RegisterCategory
{
    Unknown,
    Voltage,
    Current,
    Power,
    Energy,
    Frequency,
    Temperature,
    Percentage,
    State,
    Configuration
};

enum class RegisterSource
{
    Unknown,
    Measured,
    ReverseEngineered,
    VictronDocumentation
};

struct RegisterDefinition
{
    uint8_t unit;
    uint16_t address;

    const char* name;
    const char* units;

    float scale;

    DataType type;

    uint8_t decimals;

    RegisterCategory category;

    RegisterSource source;

    uint8_t confidence;      // 0-100%

    const char* notes;
};