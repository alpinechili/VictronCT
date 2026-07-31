#pragma once

#include <Arduino.h>

bool wifiBegin();
void wifiLoop();

bool wifiConnected();

String wifiIP();
String wifiSSID();

long wifiRSSI();