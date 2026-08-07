#include "modbus_manager.h"

#include <Arduino.h>
#include <WiFi.h>
#include <ModbusIP_ESP8266.h>

#include "config.h"
#include "secrets.h"
#include "wifi_manager.h"
#include "victron_data.h"

ModbusIP mb;

static const IPAddress CERBO_IP(192, 168, 0, 200);

static bool connected = false;
static unsigned long lastAttempt = 0;
static bool readScaledRegister(
    uint8_t unitId,
    uint16_t address,
    float &value);

bool modbusBegin()
{
    connected = false;
    return true;
}

bool modbusConnected()
{
    return connected;
}

static bool modbusWaitForTransaction(uint16_t transaction)
{
    if (transaction == 0)
    {
        connected = false;
        victron.online = false;
        return false;
    }

    unsigned long start = millis();

    while (mb.isTransaction(transaction))
    {
        mb.task();

        if (millis() - start > 1000)
        {
            Serial.println("Modbus timeout");

            connected = false;
            victron.online = false;

            return false;
        }

        delay(1);
    }

    return true;
}

void modbusLoop()
{
    if (!wifiConnected())
    {
        connected = false;
        victron.online = false;
        return;
    }

    mb.task();

    if (!connected)
    {
        if (millis() - lastAttempt < 5000)
            return;

        lastAttempt = millis();

        Serial.print("Connecting to Cerbo GX... ");

        if (mb.connect(CERBO_IP, VICTRON_MODBUS_PORT))
        {
            connected = true;
            Serial.println("Connected");
        }
        else
        {
            Serial.println("Failed");
        }

        return;
    }

    static unsigned long lastRead = 0;

    if (millis() - lastRead < 5000)
        return;

    lastRead = millis();

    uint16_t raw;
    bool ok = true;

    float value;

    if (readScaledRegister(
            CERBO_UNIT,
            VictronRegisters::BATTERY_VOLTAGE,
            value))
        victron.batteryVoltage = value;
    else
        ok = false;

    if (readScaledRegister(
            CERBO_UNIT,
            VictronRegisters::BATTERY_CURRENT,
            value))
        victron.batteryCurrent = value;
    else
        ok = false;

    if (readScaledRegister(
            CERBO_UNIT,
            VictronRegisters::BATTERY_POWER,
            value))
        victron.batteryPower = value;
    else
        ok = false;

    if (readScaledRegister(
            CERBO_UNIT,
            VictronRegisters::BATTERY_SOC,
            value))
        victron.batterySOC = value;
    else
        ok = false;

    if (readScaledRegister(
            CERBO_UNIT,
            VictronRegisters::AC_CONSUMPTION_L1,
            value))
        victron.acConsumptionL1 = value;
    else
        ok = false;

    if (readScaledRegister(
            CERBO_UNIT,
            VictronRegisters::GRID_POWER,
            value))
        victron.gridPowerL1 = value;
    else
        ok = false;

    if (readScaledRegister(
            CERBO_UNIT,
            VictronRegisters::PV_INVERTER1_POWER,
            value))
        victron.pvInverter1Power = value;
    else
        ok = false;

    if (readScaledRegister(
            CERBO_UNIT,
            VictronRegisters::PV_INVERTER2_POWER,
            value))
        victron.pvInverter2Power = value;
    else
        ok = false;

    victron.pvTotalPower =
        victron.pvInverter1Power +
        victron.pvInverter2Power;

    if (readScaledRegister(
            CERBO_UNIT,
            VictronRegisters::PV_DC_POWER,
            value))
        victron.pvDcPower = value;
    else
        ok = false;

    if (readScaledRegister(
            CERBO_UNIT,
            VictronRegisters::CHARGER_POWER,
            value))
        victron.chargerPower = value;
    else
        ok = false;

    victron.online = ok;
}

bool readHoldingRegister(
    uint8_t unitId,
    uint16_t address,
    uint16_t &value)
{
    value = 0;

    if (!connected)
        return false;

    uint16_t transaction = mb.readHreg(
        CERBO_IP,
        address,
        &value,
        1,
        nullptr,
        unitId);

    return modbusWaitForTransaction(transaction);
}

bool readHoldingRegisters(
    uint8_t unitId,
    uint16_t startAddress,
    uint16_t count,
    uint16_t *values)
{
    if (!connected)
        return false;

    uint16_t transaction = mb.readHreg(
        CERBO_IP,
        startAddress,
        values,
        count,
        nullptr,
        unitId);

    return modbusWaitForTransaction(transaction);
}

bool writeHoldingRegister(
    uint8_t unitId,
    uint16_t address,
    uint16_t value)
{
    if (!connected)
        return false;

    uint16_t transaction = mb.writeHreg(
        CERBO_IP,
        address,
        value,
        nullptr,
        unitId);

    if (!modbusWaitForTransaction(transaction))
        return false;

    Serial.printf(
        "Modbus write OK - Unit %u Register %u = %u\n",
        unitId,
        address,
        value);

    return true;
}

bool readRegister(
    uint8_t unitId,
    const RegisterDefinition &reg,
    float &value)
{
    uint16_t raw;

    if (!readHoldingRegister(unitId, reg.address, raw))
        return false;

    if (reg.isSigned)
        value = (float)((int16_t)raw) / reg.scale;
    else
        value = (float)raw / reg.scale;

    return true;
}

bool readScaledRegister(
    uint8_t unitId,
    uint16_t address,
    float &value)
{
    const RegisterDefinition *reg =
        VictronRegisters::find(unitId, address);

    if (reg == nullptr)
        return false;

    return readRegister(unitId, *reg, value);
}