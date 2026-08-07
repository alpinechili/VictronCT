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
    200,
    10,
    1500.0f);

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
    static uint8_t currentChannel = 0;

    if (!enabled)
        return;

    if (millis() - lastUpdate < 500)
        return;

    lastUpdate = millis();

    unsigned long start = millis();

    switch (currentChannel)
    {
        case 0:
            victron.ct1Power = ct.power_sample(0);
            break;

        case 1:
            victron.ct2Power = ct.power_sample(1);
            break;

        case 2:
            victron.ct3Power = ct.power_sample(2);
            break;

        case 3:
            victron.ct4Power = ct.power_sample(3);
            break;
    }

    currentChannel = (currentChannel + 1) & 3;

    importPower = victron.ct1Power;
    exportPower = 0.0f;
    netPower = importPower - exportPower;

    victron.ctImportPower = importPower;
    victron.ctExportPower = exportPower;
    victron.ctNetPower = netPower;

    Serial.printf(
        "CT sampling: %lu ms  CH:%u\n",
        millis() - start,
        currentChannel);

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