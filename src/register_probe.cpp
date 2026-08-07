#include "register_probe.h"

#include <Arduino.h>

#include "developer_registers.h"
#include "modbus_manager.h"
#include "register_recorder.h"
#include "config.h"

//==================================================
// Globals
//==================================================

bool probeEnabled = false;

uint8_t probeUnit = CERBO_UNIT;
uint16_t probeRegister = 808;

bool watchEnabled = false;
uint8_t watchUnit = CERBO_UNIT;

uint16_t watchStartRegister = 800;
uint16_t watchEndRegister = 900;

//==================================================
// Change Detection
//==================================================

constexpr uint16_t MAX_REGISTER = 1000;

uint16_t previousValue[MAX_REGISTER];
bool firstWatch = true;

static uint16_t lastProbeHi = 0;
static uint16_t lastProbeLo = 0;
static bool firstProbe = true;

//==================================================

namespace
{
    unsigned long lastUpdate = 0;
}

//==================================================

void registerProbeBegin()
{
}

void resetRegisterWatch()
{
    firstWatch = true;
}

//==================================================

static void printValue(const DeveloperRegister *info, uint16_t raw)
{
    float value;

    if (info->type == RegisterType::INT16)
        value = (float)((int16_t)raw);
    else
        value = (float)raw;

    value /= info->scale;

    switch (info->decimals)
    {
        case 0:
            Serial.printf("%.0f", value);
            break;

        case 1:
            Serial.printf("%.1f", value);
            break;

        case 2:
            Serial.printf("%.2f", value);
            break;

        default:
            Serial.printf("%f", value);
            break;
    }
}

//==================================================

void registerProbeLoop()
{
    if (!probeEnabled)
        return;

    if (!modbusConnected())
        return;

    if (millis() - lastUpdate < 1000)
        return;

    lastUpdate = millis();

    //--------------------------------------------------
    // WATCH MODE
    //--------------------------------------------------

    if (watchEnabled)
    {
        bool changed = firstWatch;

        for (uint16_t reg = watchStartRegister;
             reg <= watchEndRegister;
             reg++)
        {
            if (reg >= MAX_REGISTER)
                break;

            uint16_t value;

            if (!readHoldingRegister(watchUnit, reg, value))
                continue;

            registerRecorderUpdate(reg, value);

            if (firstWatch)
            {
                previousValue[reg] = value;
                changed = true;
            }
            else if (previousValue[reg] != value)
            {
                previousValue[reg] = value;
                changed = true;
            }
        }

        firstWatch = false;

        if (!changed)
            return;

        Serial.println("==============================================================");

        //------------------------------------------------------
        // Known registers
        //------------------------------------------------------

        for (uint16_t reg = watchStartRegister;
             reg <= watchEndRegister;
             reg++)
        {
            uint16_t value;

            if (!readHoldingRegister(watchUnit, reg, value))
                continue;

            const DeveloperRegister *info =
                findDeveloperRegister(watchUnit, reg);

            if (!info)
                continue;

            Serial.printf("%-28s ", info->name);

            printValue(info, value);

            Serial.printf(" %s\n", info->units);

            delay(2);
        }

        Serial.println();
        Serial.println("--------------------------------------------------------------");
        Serial.println("Unknown Registers");
        Serial.println("--------------------------------------------------------------");

        //------------------------------------------------------
        // Unknown registers
        //------------------------------------------------------

        for (uint16_t reg = watchStartRegister;
             reg <= watchEndRegister;
             reg++)
        {
            uint16_t value;

            if (!readHoldingRegister(watchUnit, reg, value))
                continue;

            if (value == 0 || value == 0xFFFF)
                continue;

            const DeveloperRegister *info =
                findDeveloperRegister(watchUnit, reg);

            if (info)
                continue;

            Serial.printf("R%-6u %8d\n",
                          reg,
                          (int16_t)value);

            delay(2);
        }

        Serial.println();
        Serial.println("==============================================================");

        return;
    }

    //--------------------------------------------------
    // PROBE MODE
    //--------------------------------------------------

    uint16_t hi;
    uint16_t lo;

    if (!readHoldingRegister(probeUnit, probeRegister, hi))
        return;

    if (!readHoldingRegister(probeUnit, probeRegister + 1, lo))
        return;

    if (!firstProbe &&
        hi == lastProbeHi &&
        lo == lastProbeLo)
    {
        return;
    }

    firstProbe = false;
    lastProbeHi = hi;
    lastProbeLo = lo;

    uint32_t u32 = ((uint32_t)hi << 16) | lo;

    Serial.println();
    Serial.println("==============================================================");
    Serial.printf("Unit %u Register %u\n",
                  probeUnit,
                  probeRegister);
    Serial.println("--------------------------------------------------------------");

    const DeveloperRegister *info =
        findDeveloperRegister(probeUnit, probeRegister);

    if (info)
    {
        Serial.printf("%s\n\n", info->name);

        Serial.print("Value : ");

        printValue(info, hi);

        Serial.printf(" %s\n\n", info->units);
    }

    Serial.printf("Reg %-4u : %6u (%6d) 0x%04X\n",
                  probeRegister,
                  hi,
                  (int16_t)hi,
                  hi);

    Serial.printf("Reg %-4u : %6u (%6d) 0x%04X\n",
                  probeRegister + 1,
                  lo,
                  (int16_t)lo,
                  lo);

    Serial.println();

    Serial.printf("32-bit Unsigned : %lu\n",
                  (unsigned long)u32);

    Serial.printf("32-bit Signed   : %ld\n",
                  (long)((int32_t)u32));

    Serial.println("==============================================================");
}