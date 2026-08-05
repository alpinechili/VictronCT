#pragma once

#include <stdint.h>

#include "victron_registers.h"

bool modbusBegin();
void modbusLoop();

bool modbusConnected();

//--------------------------------------------------
// Read Registers
//--------------------------------------------------

bool readHoldingRegister(
    uint8_t unitId,
    uint16_t address,
    uint16_t& value);

bool readHoldingRegisters(
    uint8_t unitId,
    uint16_t startAddress,
    uint16_t count,
    uint16_t* values);

bool readRegister(
    uint8_t unitId,
    const Victron::RegisterInfo& reg,
    float& value);

//--------------------------------------------------
// Write Registers
//--------------------------------------------------

bool writeHoldingRegister(
    uint8_t unitId,
    uint16_t address,
    uint16_t value);

//--------------------------------------------------
// Register Cache
//--------------------------------------------------

bool getCachedRegister(
    uint8_t unitId,
    uint16_t address,
    uint16_t& value);

void clearRegisterCache();