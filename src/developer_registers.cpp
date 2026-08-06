#include "developer_registers.h"

#include <stddef.h>

static const DeveloperRegister registers[] =
{
    //--------------------------------------------------
    // Unit 100 - System
    //--------------------------------------------------

    {100, 817, "AC Consumption", "ac/consumption",
        1.0f, RegisterType::UINT16, "W",
        0, RegisterCategory::Power,
        HADeviceClass::Power,
        HAStateClass::Measurement,
        "mdi:home-lightning-bolt",
        true},

    {100, 820, "Grid Power", "grid/power",
        1.0f, RegisterType::INT16, "W",
        0, RegisterCategory::Power,
        HADeviceClass::Power,
        HAStateClass::Measurement,
        "mdi:transmission-tower",
        true},

    {100, 840, "Battery Voltage", "battery/voltage",
        10.0f, RegisterType::UINT16, "V",
        1, RegisterCategory::Voltage,
        HADeviceClass::Voltage,
        HAStateClass::Measurement,
        "mdi:battery",
        true},

    {100, 841, "Battery Current", "battery/current",
        10.0f, RegisterType::INT16, "A",
        1, RegisterCategory::Current,
        HADeviceClass::Current,
        HAStateClass::Measurement,
        "mdi:current-dc",
        true},

    {100, 842, "Battery Power", "battery/power",
        1.0f, RegisterType::INT16, "W",
        0, RegisterCategory::Power,
        HADeviceClass::Power,
        HAStateClass::Measurement,
        "mdi:battery-charging",
        true},

    {100, 843, "Battery SOC", "battery/soc",
        1.0f, RegisterType::UINT16, "%",
        0, RegisterCategory::Percentage,
        HADeviceClass::Battery,
        HAStateClass::Measurement,
        "mdi:battery-high",
        true},

    {100, 850, "PV DC Power", "pv/dc_power",
        1.0f, RegisterType::UINT16, "W",
        0, RegisterCategory::Power,
        HADeviceClass::Power,
        HAStateClass::Measurement,
        "mdi:solar-power",
        true},

    {100, 855, "Charger Power", "charger/power",
        1.0f, RegisterType::UINT16, "W",
        0, RegisterCategory::Power,
        HADeviceClass::Power,
        HAStateClass::Measurement,
        "mdi:battery-charging",
        true},

    {100, 865, "PV Inverter Status", nullptr,
        1.0f, RegisterType::INT16, "",
        0, RegisterCategory::State,
        HADeviceClass::None,
        HAStateClass::Measurement,
        "mdi:solar-power",
        false},

    {100, 866, "PV Inverter Metric", nullptr,
        1.0f, RegisterType::INT16, "",
        0, RegisterCategory::Unknown,
        HADeviceClass::None,
        HAStateClass::Measurement,
        "mdi:chart-line",
        false},

    {100, 869, "PV Inverter Status 2", nullptr,
        1.0f, RegisterType::INT16, "",
        0, RegisterCategory::State,
        HADeviceClass::None,
        HAStateClass::Measurement,
        "mdi:solar-power",
        false},

    {100, 879, "CT Load Power", "ct/load_power",
        1.0f, RegisterType::INT16, "W",
        0, RegisterCategory::Power,
        HADeviceClass::Power,
        HAStateClass::Measurement,
        "mdi:current-ac",
        true},

    {100, 885, "PV Inverter 1", "pv/inverter1",
        1.0f, RegisterType::UINT16, "W",
        0, RegisterCategory::Power,
        HADeviceClass::Power,
        HAStateClass::Measurement,
        "mdi:solar-power",
        true},

    {100, 893, "PV Inverter 2", "pv/inverter2",
        1.0f, RegisterType::UINT16, "W",
        0, RegisterCategory::Power,
        HADeviceClass::Power,
        HAStateClass::Measurement,
        "mdi:solar-power",
        true},

    {100, 903, "Load Metric Mirror", nullptr,
        1.0f, RegisterType::INT16, "",
        0, RegisterCategory::Unknown,
        HADeviceClass::None,
        HAStateClass::Measurement,
        "mdi:chart-line",
        false},

    {100, 908, "PV Status Flag", nullptr,
        1.0f, RegisterType::INT16, "",
        0, RegisterCategory::State,
        HADeviceClass::None,
        HAStateClass::Measurement,
        "mdi:flag",
        false},

    {100, 909, "PV Metric", nullptr,
        1.0f, RegisterType::INT16, "",
        0, RegisterCategory::Unknown,
        HADeviceClass::None,
        HAStateClass::Measurement,
        "mdi:chart-line",
        false},

    {100, 911, "PV Inverter 2 Signed", "pv/inverter2_signed",
        1.0f, RegisterType::INT16, "W",
        0, RegisterCategory::Power,
        HADeviceClass::Power,
        HAStateClass::Measurement,
        "mdi:solar-power",
        true},

    //--------------------------------------------------
    // Unit 227 - VE.Bus
    //--------------------------------------------------

    {227, 21, "AC Frequency", "vebus/frequency",
        100.0f, RegisterType::UINT16, "Hz",
        2, RegisterCategory::Frequency,
        HADeviceClass::Frequency,
        HAStateClass::Measurement,
        "mdi:sine-wave",
        true},

    {227, 22, "Grid Current Limit", "vebus/grid_current_limit",
        10.0f, RegisterType::UINT16, "A",
        1, RegisterCategory::Configuration,
        HADeviceClass::Current,
        HAStateClass::Measurement,
        "mdi:current-ac",
        true,
        true},

        {227, 23, "Unknown Power", "vebus/unknown_power",
        1.0f, RegisterType::INT16, "",
        0, RegisterCategory::Power,
        HADeviceClass::Power,
        HAStateClass::Measurement,
        "mdi:flash",
        true},

    {227, 26, "Battery Voltage", "battery/voltage2",
        100.0f, RegisterType::UINT16, "V",
        2, RegisterCategory::Voltage,
        HADeviceClass::Voltage,
        HAStateClass::Measurement,
        "mdi:battery",
        true},

    {227, 27, "Unknown Current", "vebus/unknown_current",
        1.0f, RegisterType::INT16, "",
        0, RegisterCategory::Current,
        HADeviceClass::Current,
        HAStateClass::Measurement,
        "mdi:current-ac",
        true},

    {227, 18, "Unknown Metric", nullptr,
        1.0f, RegisterType::INT16, "",
        0, RegisterCategory::Unknown,
        HADeviceClass::None,
        HAStateClass::Measurement,
        "mdi:chart-line",
        true},

    {227, 12, "Unknown Metric", nullptr,
        1.0f, RegisterType::INT16, "",
        0, RegisterCategory::Unknown,
        HADeviceClass::None,
        HAStateClass::Measurement,
        "mdi:chart-line",
        true},

    {227, 6, "Unknown Metric", nullptr,
        1.0f, RegisterType::INT16, "",
        0, RegisterCategory::Unknown,
        HADeviceClass::None,
        HAStateClass::Measurement,
        "mdi:chart-line",
        true},

    {227, 3, "Unknown Metric", nullptr,
        1.0f, RegisterType::INT16, "",
        0, RegisterCategory::Unknown,
        HADeviceClass::None,
        HAStateClass::Measurement,
        "mdi:chart-line",
        true},

    {227, 15, "Unknown Metric", nullptr,
        1.0f, RegisterType::INT16, "",
        0, RegisterCategory::Unknown,
        HADeviceClass::None,
        HAStateClass::Measurement,
        "mdi:chart-line",
        true},
};

static const size_t registerCount =
    sizeof(registers) / sizeof(registers[0]);

const DeveloperRegister* findDeveloperRegister(
    uint8_t unit,
    uint16_t address)
{
    for (size_t i = 0; i < registerCount; i++)
    {
        if (registers[i].unit == unit &&
            registers[i].address == address)
        {
            return &registers[i];
        }
    }

    return nullptr;
}

const DeveloperRegister* developerRegisterAt(
    uint16_t index)
{
    if (index >= registerCount)
        return nullptr;

    return &registers[index];
}

uint16_t developerRegisterCount()
{
    return (uint16_t)registerCount;
}