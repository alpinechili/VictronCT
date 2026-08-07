#include "calibration_manager.h"

#include <Preferences.h>

static Preferences prefs;

static float calibration[4];

bool calibrationBegin()
{
    prefs.begin("ct", false);

    calibration[0] = prefs.getFloat("ct1", 1500.0f);
    calibration[1] = prefs.getFloat("ct2", 1500.0f);
    calibration[2] = prefs.getFloat("ct3", 1500.0f);
    calibration[3] = prefs.getFloat("ct4", 1500.0f);

    return true;
}

float calibrationGet(uint8_t channel)
{
    if (channel >= 4)
        return 1500.0f;

    return calibration[channel];
}

void calibrationSet(
    uint8_t channel,
    float value)
{
    if (channel >= 4)
        return;

    calibration[channel] = value;
}

void calibrationSave()
{
    prefs.putFloat("ct1", calibration[0]);
    prefs.putFloat("ct2", calibration[1]);
    prefs.putFloat("ct3", calibration[2]);
    prefs.putFloat("ct4", calibration[3]);
}