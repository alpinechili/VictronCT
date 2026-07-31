#include "modbus_manager.h"

#include <Arduino.h>
#include <WiFi.h>
#include <ModbusIP_ESP8266.h>

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

    //
    // ===== SYSTEM SERVICE (Unit 100) =====
    //

    if (readHoldingRegister(Victron::Unit::SYSTEM, 840, raw))
        victron.batteryVoltage = raw / 100.0f;
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

    victron.online = ok;

    if (!ok)
    {
        Serial.println("System register read failed");
        return;
    }

    Serial.println();
    Serial.println("==============================");
    Serial.println("     VICTRON SYSTEM DATA");
    Serial.println("==============================");

    Serial.printf("Battery Voltage : %.2f V\n",
                  victron.batteryVoltage);

    Serial.printf("Battery Current : %.1f A\n",
                  victron.batteryCurrent);

    Serial.printf("Battery Power   : %.0f W\n",
                  victron.batteryPower);

    Serial.printf("Battery SOC     : %.1f %%\n",
                  victron.batterySOC);

    Serial.println();
}

bool readHoldingRegister(uint8_t unitId,
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
        return false;

    unsigned long start = millis();

    while (mb.isTransaction(transaction))
    {
        mb.task();

        if (millis() - start > 1000)
        {
            Serial.println("Modbus timeout");
            return false;
        }

        delay(1);
    }

    return true;
}

bool readHoldingRegisters(uint8_t unitId,
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
        return false;

    unsigned long start = millis();

    while (mb.isTransaction(transaction))
    {
        mb.task();

        if (millis() - start > 1000)
        {
            Serial.println("Modbus timeout");
            return false;
        }

        delay(1);
    }

    return true;
}

bool readRegister(uint8_t unitId,
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