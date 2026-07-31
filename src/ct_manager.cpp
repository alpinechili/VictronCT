#include "ct_manager.h"

#include <Arduino.h>

#include "config.h"

void ctManagerBegin()
{
    Serial.println();
    Serial.println("CT Manager");

    if (!ENABLE_CT_MANAGER)
    {
        Serial.println("  Status : Disabled");
        return;
    }

    Serial.println("  Status : Enabled");

    // Future:
    // Initialise ADC
    // Load calibration
    // Configure channels
}

void ctManagerLoop()
{
    if (!ENABLE_CT_MANAGER)
        return;

    // Future:
    // Read CT channels
    // Calculate RMS current
    // Calculate power
    // Publish MQTT
}