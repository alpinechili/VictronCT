#pragma once

#include <stdint.h>

#include "victron_registers.h"

//==================================================
// Probe
//==================================================

extern bool probeEnabled;

extern uint8_t probeUnit;
extern uint16_t probeRegister;

void registerProbeBegin();
void registerProbeLoop();

//==================================================
// Watch
//==================================================

extern bool watchEnabled;

extern uint8_t watchUnit;

extern uint16_t watchStartRegister;
extern uint16_t watchEndRegister;

void resetRegisterWatch();