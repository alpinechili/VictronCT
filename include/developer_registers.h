#pragma once

#include <stdint.h>

enum class RegisterType
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

enum class HADeviceClass
{
    None,
    Voltage,
    Current,
    Power,
    Energy,
    Frequency,
    Temperature,
    Battery
};

enum class HAStateClass
{
    None,
    Measurement,
    Total,
    TotalIncreasing
};

struct DeveloperRegister
{
    uint8_t unit;
    uint16_t address;

    // Friendly name
    const char* name;

    // MQTT topic (nullptr = register only)
    const char* mqttTopic;

    // Scaling
    float scale;

    RegisterType type;

    // Home Assistant
    const char* units;
    uint8_t decimals;

    RegisterCategory category;
    HADeviceClass deviceClass;
    HAStateClass stateClass;

    const char* icon;

    // Auto poll
    bool autoPoll;
};

const DeveloperRegister* findDeveloperRegister(
    uint8_t unit,
    uint16_t address);

const DeveloperRegister* developerRegisterAt(
    uint16_t index);

uint16_t developerRegisterCount();