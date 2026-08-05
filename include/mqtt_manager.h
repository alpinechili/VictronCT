#pragma once

#include <Arduino.h>

bool mqttBegin();

void mqttLoop();

bool mqttConnected();

bool mqttPublish(
    const char* topic,
    const char* payload);