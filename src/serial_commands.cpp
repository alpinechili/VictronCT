#include "serial_commands.h"

#include <Arduino.h>

#include "config.h"
#include "register_scanner.h"

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
        Serial.println();
        return;
    }

    if (cmd == "version")
    {
        Serial.printf("\n%s v%s\n\n",
                      PROJECT_NAME,
                      PROJECT_VERSION);
        return;
    }

    if (cmd == "scan")
    {
        registerScannerStart();

        Serial.println();
        Serial.println("Starting register scan...");
        Serial.println();

        return;
    }

    Serial.print("Unknown command: ");
    Serial.println(cmd);
}