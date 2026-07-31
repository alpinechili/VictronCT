#pragma once

#include <stdint.h>

enum class DataType
{
    UINT16,
    INT16
};

struct RegisterDefinition
{
    uint16_t address;
    float* destination;
    float scale;
    DataType type;
};