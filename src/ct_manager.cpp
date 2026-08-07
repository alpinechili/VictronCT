#include "ct_manager.h"

#include <Arduino.h>

static bool enabled = false;

static float importPower = 0.0f;
static float exportPower = 0.0f;
static float netPower = 0.0f;

bool ctManagerBegin()
{
    enabled = false;

    importPower = 0.0f;
    exportPower = 0.0f;
    netPower = 0.0f;

    return true;
}

void ctManagerLoop()
{
    if (!enabled)
        return;

    //==================================================
    // Placeholder
    //==================================================

    importPower = 0.0f;
    exportPower = 0.0f;

    netPower = importPower - exportPower;
}

bool ctManagerEnabled()
{
    return enabled;
}

void ctManagerEnable(bool state)
{
    enabled = state;
}

float ctImportPower()
{
    return importPower;
}

float ctExportPower()
{
    return exportPower;
}

float ctNetPower()
{
    return netPower;
}