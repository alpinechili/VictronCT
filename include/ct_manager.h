#pragma once

#include <stdint.h>

bool ctManagerBegin();
void ctManagerLoop();

bool ctManagerEnabled();
void ctManagerEnable(bool enabled);

float ctImportPower();
float ctExportPower();

float ctNetPower();