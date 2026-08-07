#pragma once

#include <stdint.h>

bool calibrationBegin();

float calibrationGet(uint8_t channel);

void calibrationSet(
    uint8_t channel,
    float value);

void calibrationSave();