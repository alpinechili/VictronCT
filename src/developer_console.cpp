#include "developer_console.h"

#include <Arduino.h>

bool DeveloperConsole::firstScreen = true;

//--------------------------------------------------

void DeveloperConsole::begin()
{
    // Blank lines instead of ANSI clear screen so it
    // works in every serial monitor.
    for (int i = 0; i < 30; i++)
        Serial.println();

    Serial.println("==============================================================");
    Serial.println("                 VictronCT Developer Console");
    Serial.println("==============================================================");
    Serial.println();
}

//--------------------------------------------------

void DeveloperConsole::clear()
{
    for (int i = 0; i < 30; i++)
        Serial.println();
}

//--------------------------------------------------

void DeveloperConsole::title(const char *text)
{
    Serial.println();
    Serial.println(text);
    Serial.println("--------------------------------------------------------------");
}

//--------------------------------------------------

void DeveloperConsole::line()
{
    Serial.println("--------------------------------------------------------------");
}

//--------------------------------------------------

void DeveloperConsole::value(const DeveloperRegister *info,
                             uint16_t rawValue)
{
    if (info == nullptr)
        return;

    float value;

    if (info->type == RegisterType::INT16)
        value = (float)((int16_t)rawValue);
    else
        value = (float)rawValue;

    value /= info->scale;

    char number[20];

    switch (info->decimals)
    {
        case 0:
            snprintf(number,
                     sizeof(number),
                     "%.0f",
                     value);
            break;

        case 1:
            snprintf(number,
                     sizeof(number),
                     "%.1f",
                     value);
            break;

        case 2:
            snprintf(number,
                     sizeof(number),
                     "%.2f",
                     value);
            break;

        default:
            snprintf(number,
                     sizeof(number),
                     "%f",
                     value);
            break;
    }

    Serial.printf("%-24s %10s %-3s\n",
                  info->name,
                  number,
                  info->units);
}

//--------------------------------------------------

void DeveloperConsole::unknown(uint16_t reg,
                               int16_t value)
{
    Serial.printf("R%-22u %10d\n",
                  reg,
                  value);
}

//--------------------------------------------------

void DeveloperConsole::footer()
{
    Serial.println();
    Serial.println("==============================================================");
}