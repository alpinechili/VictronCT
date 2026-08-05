#pragma once

#include <Arduino.h>

void registerRecorderBegin();
void registerRecorderLoop();

void registerRecorderStart();
void registerRecorderStop();

bool registerRecorderActive();

void registerRecorderUpdate(
    uint16_t reg,
    uint16_t value);

// Snapshot / Compare

bool registerSnapshot(char id);
bool registerCompare(char first, char second);