#include "serial_commands.h"

#include <Arduino.h>

#include "config.h"

void serialCommandsBegin()
{
    Serial.println();
    Serial.println("Serial command interface ready.");
    Serial.println("Type 'help' for available commands.");
    Serial.println();
}

void serialCommandsLoop()
{
    if (!Serial.available())
        return;

    String cmd = Serial.readStringUntil('\n');

    cmd.trim();
    cmd.toLowerCase();

    if (cmd == "help")
    {
        Serial.println();
        Serial.println("Available commands");
        Serial.println("------------------");
        Serial.println("help");
        Serial.println("version");
        Serial.println("scan");
        Serial.println("compare");
        Serial.println("watch");
        Serial.println();
        return;
    }

    if (cmd == "version")
    {
        Serial.println();
        Serial.println(PROJECT_NAME);
        Serial.println(PROJECT_VERSION);
        Serial.println();
        return;
    }

    if (cmd == "scan")
    {
        Serial.println();
        Serial.println("Scan requested.");
        Serial.println();
        return;
    }

    if (cmd == "compare")
    {
        Serial.println();
        Serial.println("Compare requested.");
        Serial.println();
        return;
    }

    if (cmd == "watch")
    {
        Serial.println();
        Serial.println("Watch mode requested.");
        Serial.println();
        return;
    }

    Serial.print("Unknown command: ");
    Serial.println(cmd);
}