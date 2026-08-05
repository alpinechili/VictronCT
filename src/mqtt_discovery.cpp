#include "mqtt_discovery.h"

#include <Arduino.h>
#include <stdio.h>
#include <string.h>

#include "mqtt_manager.h"

struct DiscoveryEntry
{
    uint8_t unit;
    uint16_t reg;
    bool published;
};

constexpr uint16_t MAX_DISCOVERY = 128;

static DiscoveryEntry published[MAX_DISCOVERY];

static const char* deviceClassToString(HADeviceClass c)
{
    switch (c)
    {
        case HADeviceClass::Voltage:     return "voltage";
        case HADeviceClass::Current:     return "current";
        case HADeviceClass::Power:       return "power";
        case HADeviceClass::Energy:      return "energy";
        case HADeviceClass::Frequency:   return "frequency";
        case HADeviceClass::Temperature: return "temperature";
        case HADeviceClass::Battery:     return "battery";
        default:                         return nullptr;
    }
}

static const char* stateClassToString(HAStateClass c)
{
    switch (c)
    {
        case HAStateClass::Measurement:     return "measurement";
        case HAStateClass::Total:           return "total";
        case HAStateClass::TotalIncreasing: return "total_increasing";
        default:                            return nullptr;
    }
}

void mqttDiscoveryBegin()
{
    memset(published, 0, sizeof(published));
}

void mqttDiscoveryLoop()
{
}

void mqttDiscoveryPublishRegister(
    uint8_t unit,
    uint16_t reg,
    const DeveloperRegister* info)
{
    if (!info)
        return;

    for (uint16_t i = 0; i < MAX_DISCOVERY; i++)
    {
        if (published[i].published &&
            published[i].unit == unit &&
            published[i].reg == reg)
        {
            return;
        }
    }

    for (uint16_t i = 0; i < MAX_DISCOVERY; i++)
    {
        if (!published[i].published)
        {
            published[i].published = true;
            published[i].unit = unit;
            published[i].reg = reg;
            break;
        }
    }

    char topic[128];

    snprintf(topic,
             sizeof(topic),
             "homeassistant/sensor/victronct_%u_%u/config",
             unit,
             reg);

    String json = "{";

    json += "\"name\":\"";
    json += info->name;
    json += "\",";

    json += "\"unique_id\":\"victronct_";
    json += unit;
    json += "_";
    json += reg;
    json += "\",";

    json += "\"state_topic\":\"VictronCT/register/";
    json += unit;
    json += "/";
    json += reg;
    json += "\",";

    if (strlen(info->units))
    {
        json += "\"unit_of_measurement\":\"";
        json += info->units;
        json += "\",";
    }

    if (const char* dc = deviceClassToString(info->deviceClass))
    {
        json += "\"device_class\":\"";
        json += dc;
        json += "\",";
    }

    if (const char* sc = stateClassToString(info->stateClass))
    {
        json += "\"state_class\":\"";
        json += sc;
        json += "\",";
    }

    if (strlen(info->icon))
    {
        json += "\"icon\":\"";
        json += info->icon;
        json += "\",";
    }

    json += "\"suggested_display_precision\":";
    json += info->decimals;
    json += ",";

    json += "\"availability_topic\":\"VictronCT/status\",";
    json += "\"payload_available\":\"online\",";
    json += "\"payload_not_available\":\"offline\",";
    json += "\"device\":{";
    json += "\"identifiers\":[\"VictronCT\"],";
    json += "\"name\":\"VictronCT\",";
    json += "\"manufacturer\":\"AlpineChili\",";
    json += "\"model\":\"Victron Modbus Gateway\"";
    json += "}}";

    mqttPublish(topic, json.c_str());
}