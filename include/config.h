#pragma once

#include <stdint.h>

//==========================================================
// Project Information
//==========================================================

constexpr char PROJECT_NAME[]    = "VictronCT";
constexpr char PROJECT_VERSION[] = "2.0.0";

//==========================================================
// Feature Enables
//==========================================================

constexpr bool ENABLE_MODBUS             = true;
constexpr bool ENABLE_MQTT               = true;
constexpr bool ENABLE_REGISTER_SCANNER   = true;
constexpr bool ENABLE_CT_MANAGER         = false;
constexpr bool ENABLE_REGISTER_POLLER    = true;

constexpr bool ENABLE_LOGGING            = true;
constexpr bool ENABLE_DEBUG              = true;

constexpr bool ENABLE_REGISTER_WATCH     = false;
constexpr bool ENABLE_REGISTER_PROBE     = true;
constexpr bool ENABLE_SYSTEM_DATA_SERIAL = false;

// MQTT serial debug
constexpr bool ENABLE_MQTT_DEBUG         = false;

//==========================================================
// Developer Mode
//==========================================================

constexpr bool ENABLE_DEVELOPER_MODE = true;

//==========================================================
// MQTT
//==========================================================

constexpr char MQTT_TOPIC[] = "victronct";

//==========================================================
// Modbus
//==========================================================

constexpr uint8_t CERBO_UNIT = 100;