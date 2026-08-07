#pragma once

bool energyManagerBegin();
void energyManagerLoop();

float gridPower();
float solarPower();
float housePower();
float batteryPower();

float surplusPower();
float availableEVPower();