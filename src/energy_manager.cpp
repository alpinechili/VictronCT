#include "energy_manager.h"

#include "victron_data.h"

static float grid = 0.0f;
static float solar = 0.0f;
static float house = 0.0f;
static float battery = 0.0f;

bool energyManagerBegin()
{
    return true;
}

void energyManagerLoop()
{
    grid = victron.gridPowerL1;
    solar = victron.pvTotalPower;
    house = victron.acConsumptionL1;
    battery = victron.batteryPower;
}

float gridPower()
{
    return grid;
}

float solarPower()
{
    return solar;
}

float housePower()
{
    return house;
}

float batteryPower()
{
    return battery;
}

float surplusPower()
{
    float surplus = solar - house;

    if (battery < 0)
        surplus += -battery;

    return surplus;
}

float availableEVPower()
{
    float available = surplusPower();

    if (available < 0)
        available = 0;

    return available;
}