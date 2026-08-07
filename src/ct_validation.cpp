#include "ct_validation.h"

#include <Arduino.h>
#include <math.h>

#include "victron_data.h"

static float errorWatts = 0.0f;
static float errorPercent = 0.0f;
static bool validated = false;

bool ctValidationBegin()
{
    errorWatts = 0.0f;
    errorPercent = 0.0f;
    validated = false;

    return true;
}

void ctValidationLoop()
{
    if (!victron.online)
        return;

    float cerbo = victron.gridPowerL1;
    float ct = victron.ctImportPower;

    errorWatts = ct - cerbo;

    if (fabs(cerbo) > 50.0f)
    {
        errorPercent =
            (errorWatts / cerbo) * 100.0f;

        validated =
            fabs(errorPercent) < 5.0f;
    }
    else
    {
        errorPercent = 0.0f;
        validated = false;
    }
}

float ctErrorWatts()
{
    return errorWatts;
}

float ctErrorPercent()
{
    return errorPercent;
}

bool ctValidated()
{
    return validated;
}