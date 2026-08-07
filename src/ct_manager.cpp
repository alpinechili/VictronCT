#include "ct_manager.h"

#include <Arduino.h>

#include "ESP32_4CH_CT.h"
#include "victron_data.h"

static bool enabled = false;

//==================================================
// Mottram CT Library
//==================================================

static ESP32_4CH_CT ct(
    34,
    35,
    36,
    39,
    400,
    20,
    1500.0);

static float importPower = 0.0f;
static float exportPower = 0.0f;
static float netPower = 0.0f;

bool ctManagerBegin()
{
    enabled = true;

    importPower = 0.0f;
    exportPower = 0.0f;
    netPower = 0.0f;

    Serial.println();
    Serial.println("CT Manager Ready");

    ct.report();

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

    double ch1 = ct.power_sample(0);
    double ch2 = ct.power_sample(1);
    double ch3 = ct.power_sample(2);
    double ch4 = ct.power_sample(3);

    victron.ct1Power = ch1;
    victron.ct2Power = ch2;
    victron.ct3Power = ch3;
    victron.ct4Power = ch4;

    importPower = ch1;
    exportPower = 0.0f;
    netPower = importPower - exportPower;

    victron.ctImportPower = importPower;
    victron.ctExportPower = exportPower;
    victron.ctNetPower = netPower;

    Serial.printf(
        "CT1 %8.2f  CT2 %8.2f  CT3 %8.2f  CT4 %8.2f\n",
        victron.ct1Power,
        victron.ct2Power,
        victron.ct3Power,
        victron.ct4Power);
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