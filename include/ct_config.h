#pragma once

#include <stdint.h>

struct CTConfig
{
    bool enabled;

    const char* name;

    uint8_t gpio;

    float calibration;

    float voltage;

    bool reverse;
};

extern CTConfig ctConfig[4];