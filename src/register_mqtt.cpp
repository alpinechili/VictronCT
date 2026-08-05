#include "register_mqtt.h"

#include <Arduino.h>
#include <string.h>

#include "developer_registers.h"
#include "mqtt_discovery.h"
#include "mqtt_manager.h"

struct CacheEntry
{
    uint8_t unit;
    uint16_t reg;
    int16_t value;
    bool valid;
};

constexpr uint16_t CACHE_SIZE = 64;

static CacheEntry cache[CACHE_SIZE];

void registerMqttBegin()
{
    memset(cache, 0, sizeof(cache));
}

void registerMqttLoop()
{
}

void registerMqttPublish(
    uint8_t unit,
    uint16_t reg,
    int16_t value)
{
    bool firstPublish = false;

    for (uint16_t i = 0; i < CACHE_SIZE; i++)
    {
        if (!cache[i].valid)
            continue;

        if (cache[i].unit == unit &&
            cache[i].reg == reg)
        {
            if (cache[i].value == value)
                return;

            cache[i].value = value;
            break;
        }
    }

    bool found = false;

    for (uint16_t i = 0; i < CACHE_SIZE; i++)
    {
        if (!cache[i].valid)
            continue;

        if (cache[i].unit == unit &&
            cache[i].reg == reg)
        {
            found = true;
            break;
        }
    }

    if (!found)
    {
        for (uint16_t i = 0; i < CACHE_SIZE; i++)
        {
            if (!cache[i].valid)
            {
                cache[i].valid = true;
                cache[i].unit = unit;
                cache[i].reg = reg;
                cache[i].value = value;
                firstPublish = true;
                break;
            }
        }
    }

    const DeveloperRegister* info =
        findDeveloperRegister(unit, reg);

    if (firstPublish && info)
    {
        mqttDiscoveryPublishRegister(
            unit,
            reg,
            info);
    }

    char payload[32];

    if (info)
    {
        float scaled;

        if (info->type == RegisterType::INT16)
            scaled = (float)value / info->scale;
        else
            scaled = (float)((uint16_t)value) / info->scale;

        dtostrf(
            scaled,
            0,
            info->decimals,
            payload);
    }
    else
    {
        snprintf(
            payload,
            sizeof(payload),
            "%d",
            value);
    }

    //--------------------------------------------------
    // Raw register topic
    //--------------------------------------------------

    char topic[64];

    snprintf(
        topic,
        sizeof(topic),
        "VictronCT/register/%u/%u",
        unit,
        reg);

    mqttPublish(topic, payload);

    //--------------------------------------------------
    // Friendly topic
    //--------------------------------------------------

    if (info && info->mqttTopic)
    {
        char friendlyTopic[96];

        snprintf(
            friendlyTopic,
            sizeof(friendlyTopic),
            "VictronCT/%s",
            info->mqttTopic);

        mqttPublish(
            friendlyTopic,
            payload);
    }
}