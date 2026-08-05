#pragma once

#include <stdint.h>

void registerMqttBegin();

void registerMqttPublish(
    uint8_t unit,
    uint16_t reg,
    int16_t value);

void registerMqttLoop();