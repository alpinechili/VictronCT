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
        default:                            return "measurement";
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

    const char* name = nullptr;
    const char* units = "";
    const char* icon = "mdi:chip";
    const char* deviceClass = nullptr;
    const char* stateClass = "measurement";

    uint8_t decimals = 0;
    bool writable = false;

    char unknownName[32];

    if (info)
    {
        name = info->name;
        units = info->units;
        icon = info->icon;
        decimals = info->decimals;
        writable = info->writable;

        deviceClass =
            deviceClassToString(info->deviceClass);

        stateClass =
            stateClassToString(info->stateClass);
    }
    else
    {
        snprintf(
            unknownName,
            sizeof(unknownName),
            "Register %u",
            reg);

        name = unknownName;
    }


    //--------------------------------------------------
    // Sensor entity
    //--------------------------------------------------

    char topic[128];

    snprintf(
        topic,
        sizeof(topic),
        "homeassistant/sensor/victronct_%u_%u/config",
        unit,
        reg);

    String json = "{";

    json += "\"name\":\"";
    json += name;
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

    if (strlen(units))
    {
        json += "\"unit_of_measurement\":\"";
        json += units;
        json += "\",";
    }

    if (deviceClass)
    {
        json += "\"device_class\":\"";
        json += deviceClass;
        json += "\",";
    }

    json += "\"state_class\":\"";
    json += stateClass;
    json += "\",";

    json += "\"icon\":\"";
    json += icon;
    json += "\",";

    json += "\"suggested_display_precision\":";
    json += decimals;
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

    mqttPublish(
        topic,
        json.c_str());


    //--------------------------------------------------
    // Writable number entity
    //--------------------------------------------------

    if (writable)
    {
        snprintf(
            topic,
            sizeof(topic),
            "homeassistant/number/victronct_%u_%u/config",
            unit,
            reg);

        String number = "{";

        number += "\"name\":\"";
        number += name;
        number += "\",";

        number += "\"unique_id\":\"victronct_number_";
        number += unit;
        number += "_";
        number += reg;
        number += "\",";

        number += "\"state_topic\":\"VictronCT/register/";
        number += unit;
        number += "/";
        number += reg;
        number += "\",";


        //--------------------------------------------------
        // Scale raw register value for HA display
        //--------------------------------------------------

        if (info && info->scale != 1.0f)
        {
            number += "\"value_template\":\"{{ value | float / ";
            number += String(info->scale);
            number += " }}\",";
        }


        number += "\"command_topic\":\"VictronCT/write/";
        number += unit;
        number += "/";
        number += reg;
        number += "\",";

        number += "\"mode\":\"slider\",";

        number += "\"min\":0,";
        number += "\"max\":100,";
        number += "\"step\":0.1,";

        number += "\"icon\":\"mdi:tune\",";

        number += "\"device\":{";
        number += "\"identifiers\":[\"VictronCT\"],";
        number += "\"name\":\"VictronCT\",";

        number += "\"manufacturer\":\"AlpineChili\",";
        number += "\"model\":\"Victron Modbus Gateway\"";

        number += "}}";

        mqttPublish(
            topic,
            number.c_str());
    }
}