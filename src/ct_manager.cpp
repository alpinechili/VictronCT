#include "ct_manager.h"

#include <Arduino.h>

#include "ct_channel.h"

static bool enabled = false;

static CTChannel ct1;
static CTChannel ct2;
static CTChannel ct3;
static CTChannel ct4;

static float importPower = 0.0f;
static float exportPower = 0.0f;
static float netPower = 0.0f;

bool ctManagerBegin()
{
    enabled = true;

    importPower = 0.0f;
    exportPower = 0.0f;
    netPower = 0.0f;

    //==================================================
    // Mottram Labs ESP32 4 Channel Power Sensor
    //==================================================

    ct1.begin(34, 1.0f, 230.0f);
    ct2.begin(35, 1.0f, 230.0f);
    ct3.begin(36, 1.0f, 230.0f);
    ct4.begin(39, 1.0f, 230.0f);

    Serial.println("CT Manager Ready");

    return true;
}

void ctManagerLoop()
{
    static unsigned long lastUpdate = 0;

    if (!enabled)
        return;

    if (millis() - lastUpdate < 500)
        return;

    lastUpdate = millis();

    ct1.update();
    ct2.update();
    ct3.update();
    ct4.update();

    importPower = ct1.power();
    exportPower = 0.0f;

    netPower = importPower - exportPower;

    Serial.printf(
        "CT1 %.1fW  CT2 %.1fW  CT3 %.1fW  CT4 %.1fW\n",
        ct1.power(),
        ct2.power(),
        ct3.power(),
        ct4.power());
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