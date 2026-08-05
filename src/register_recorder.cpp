#include "register_recorder.h"

#include <Arduino.h>
#include <string.h>

#include "developer_registers.h"
#include "register_probe.h"
#include "register_mqtt.h"

constexpr uint16_t MAX_RECORD_REG = 1000;

struct Record
{
    bool used;
    uint16_t start;
    uint16_t current;
    uint16_t minimum;
    uint16_t maximum;
    uint16_t changes;
};

static bool recording = false;
static Record records[MAX_RECORD_REG];

static const uint16_t ignoredRegisters[] =
{
    833
};

static bool ignoreRegister(uint16_t reg)
{
    for (uint16_t r : ignoredRegisters)
    {
        if (r == reg)
            return true;
    }

    return false;
}

struct SummaryEntry
{
    uint16_t reg;
    uint16_t changes;
};

void registerRecorderBegin()
{
}

void registerRecorderLoop()
{
}

void registerRecorderStart()
{
    recording = true;

    memset(records, 0, sizeof(records));

    Serial.println();
    Serial.println("========================================");
    Serial.println("Recording Started");
    Serial.println("========================================");
}

void registerRecorderStop()
{
    recording = false;

    SummaryEntry summary[MAX_RECORD_REG];
    uint16_t summaryCount = 0;

    for (uint16_t reg = 0; reg < MAX_RECORD_REG; reg++)
    {
        if (!records[reg].used)
            continue;

        if (records[reg].changes == 0)
            continue;

        summary[summaryCount].reg = reg;
        summary[summaryCount].changes = records[reg].changes;
        summaryCount++;
    }

    for (uint16_t i = 0; i < summaryCount; i++)
    {
        for (uint16_t j = i + 1; j < summaryCount; j++)
        {
            if (summary[j].changes > summary[i].changes)
            {
                SummaryEntry temp = summary[i];
                summary[i] = summary[j];
                summary[j] = temp;
            }
        }
    }

    Serial.println();
    Serial.println("========================================");
    Serial.println("Recording Summary");
    Serial.println("========================================");

    for (uint16_t i = 0; i < summaryCount; i++)
    {
        uint16_t reg = summary[i].reg;
        Record &r = records[reg];

        const DeveloperRegister *info =
            findDeveloperRegister(watchUnit, reg);

        if (info)
        {
            Serial.printf(
                "%-22s (R%-3u) ",
                info->name,
                reg);
        }
        else
        {
            Serial.printf(
                "R%-27u",
                reg);
        }

        int16_t start = (int16_t)r.start;
        int16_t end = (int16_t)r.current;
        int16_t delta = end - start;

        Serial.printf(
            "Start:%6d  End:%6d  Δ:%6d  Min:%6d  Max:%6d  Chg:%2u\n",
            start,
            end,
            delta,
            (int16_t)r.minimum,
            (int16_t)r.maximum,
            r.changes);
    }

    Serial.println("========================================");
}

bool registerRecorderActive()
{
    return recording;
}

void registerRecorderUpdate(uint16_t reg, uint16_t value)
{
    if (!recording)
        return;

    if (reg >= MAX_RECORD_REG)
        return;

    if (ignoreRegister(reg))
        return;

    Record &r = records[reg];

    if (!r.used)
    {
        r.used = true;
        r.start = value;
        r.current = value;
        r.minimum = value;
        r.maximum = value;
        r.changes = 0;
        return;
    }

    if (value != r.current)
    {
        r.current = value;
        r.changes++;

        registerMqttPublish(
            watchUnit,
            reg,
            (int16_t)value);

        if (value < r.minimum)
            r.minimum = value;

        if (value > r.maximum)
            r.maximum = value;
    }
}