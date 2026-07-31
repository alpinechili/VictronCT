#pragma once

#include <stdint.h>

void mqttPublishVictronData();

void publishFloat(
    const char* topic,
    float value,
    uint8_t decimals);

void publishInt(
    const char* topic,
    int value);

void publishBool(
    const char* topic,
    bool value);