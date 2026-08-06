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

bool modbusBegin()
{
    connected = false;
    return true;
}

bool modbusConnected()
{
    return connected;
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

    if (readHoldingRegister(Victron::Unit::SYSTEM, 840, raw))
        victron.batteryVoltage = raw / 10.0f;
    else
        ok = false;

    if (readHoldingRegister(Victron::Unit::SYSTEM, 841, raw))
        victron.batteryCurrent = (int16_t)raw / 10.0f;
    else
        ok = false;

    if (readHoldingRegister(Victron::Unit::SYSTEM, 842, raw))
        victron.batteryPower = (int16_t)raw;
    else
        ok = false;

    if (readHoldingRegister(Victron::Unit::SYSTEM, 843, raw))
        victron.batterySOC = (float)raw;
    else
        ok = false;

    if (readHoldingRegister(Victron::Unit::SYSTEM, 817, raw))
        victron.acConsumptionL1 = raw;
    else
        ok = false;

    if (readHoldingRegister(Victron::Unit::SYSTEM, 820, raw))
        victron.gridPowerL1 = (int16_t)raw;
    else
        ok = false;

    if (readHoldingRegister(Victron::Unit::SYSTEM, 808, raw))
        victron.pvInverter1Power = raw;
    else
        ok = false;

    if (readHoldingRegister(Victron::Unit::SYSTEM, 893, raw))
        victron.pvInverter2Power = raw;
    else
        ok = false;

    victron.pvTotalPower =
        victron.pvInverter1Power +
        victron.pvInverter2Power;

    if (readHoldingRegister(Victron::Unit::SYSTEM, 850, raw))
        victron.pvDcPower = raw;
    else
        ok = false;

    if (readHoldingRegister(Victron::Unit::SYSTEM, 855, raw))
        victron.chargerPower = raw;
    else
        ok = false;

    victron.online = ok;
}

bool readHoldingRegister(
    uint8_t unitId,
    uint16_t address,
    uint16_t& value)
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

bool readHoldingRegisters(
    uint8_t unitId,
    uint16_t startAddress,
    uint16_t count,
    uint16_t* values)
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
            Serial.println("Modbus write timeout");
            connected = false;
            victron.online = false;
            return false;
        }

        delay(1);
    }

    Serial.printf(
        "Modbus write OK - Unit %u Register %u = %u\n",
        unitId,
        address,
        value);

    return true;
}

bool readRegister(
    uint8_t unitId,
    const Victron::RegisterInfo& reg,
    float& value)
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