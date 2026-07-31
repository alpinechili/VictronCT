#pragma once

#include <Arduino.h>

bool mqttBegin();

void mqttLoop();

bool mqttConnected();

void mqttPublish(
    const char* topic,
    const char* payload);

