#pragma once

#include <stdint.h>

#include "developer_registers.h"

void mqttDiscoveryBegin();

void mqttDiscoveryLoop();

void mqttDiscoveryPublishRegister(
    uint8_t unit,
    uint16_t reg,
    const DeveloperRegister* info);