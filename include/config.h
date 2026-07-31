#pragma once

#include <stdint.h>

//==========================================================
// Project Information
//==========================================================

constexpr char PROJECT_NAME[]    = "VictronCT";
constexpr char PROJECT_VERSION[] = "0.3.0";

//==========================================================
// Feature Enables
//==========================================================

constexpr bool ENABLE_MODBUS           = true;
constexpr bool ENABLE_MQTT             = true;
constexpr bool ENABLE_REGISTER_SCANNER = false;
constexpr bool ENABLE_CT_MANAGER       = false;

constexpr bool ENABLE_LOGGING          = true;
constexpr bool ENABLE_DEBUG            = true;

//==========================================================
// Developer Mode
//==========================================================

// Enables register scanner, serial command interface and
// future debugging tools.
//
// false = Production firmware
// true  = Development firmware

constexpr bool ENABLE_DEVELOPER_MODE = true;

//==========================================================
// MQTT
//==========================================================

constexpr char MQTT_TOPIC[] = "victronct";

//==========================================================
// Modbus
//==========================================================

constexpr uint8_t CERBO_UNIT = 100;