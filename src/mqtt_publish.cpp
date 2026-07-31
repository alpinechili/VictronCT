#include "mqtt_publish.h"

#include <Arduino.h>

#include "mqtt_manager.h"
#include "victron_data.h"

void publishFloat(
    const char* topic,
    float value,
    uint8_t decimals)
{
    char payload[20];

    dtostrf(value, 0, decimals, payload);

    mqttPublish(topic, payload);
}

void publishInt(
    const char* topic,
    int value)
{
    char payload[20];

    sprintf(payload, "%d", value);

    mqttPublish(topic, payload);
}

void publishBool(
    const char* topic,
    bool value)
{
    mqttPublish(topic, value ? "true" : "false");
}

void mqttPublishVictronData()
{
    publishFloat(
        "VictronCT/battery/voltage",
        victron.batteryVoltage,
        2);

    publishFloat(
        "VictronCT/battery/current",
        victron.batteryCurrent,
        1);

    publishFloat(
        "VictronCT/battery/power",
        victron.batteryPower,
        0);

    publishFloat(
        "VictronCT/battery/soc",
        victron.batterySOC,
        1);
}