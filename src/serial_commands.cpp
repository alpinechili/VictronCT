#include "serial_commands.h"

#include <Arduino.h>

#include "config.h"
#include "modbus_manager.h"
#include "register_probe.h"
#include "register_scanner.h"
#include "register_recorder.h"

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

    //--------------------------------------------------
    // WRITE
    //
    // write <unit> <register> <value>
    //--------------------------------------------------

    if (cmd.startsWith("write "))
    {
        int p1 = cmd.indexOf(' ');
        int p2 = cmd.indexOf(' ', p1 + 1);
        int p3 = cmd.indexOf(' ', p2 + 1);

        if (p3 < 0)
        {
            Serial.println();
            Serial.println("Usage:");
            Serial.println("write <unit> <register> <value>");
            Serial.println();

            return;
        }

        uint8_t unit =
            cmd.substring(p1 + 1, p2).toInt();

        uint16_t reg =
            cmd.substring(p2 + 1, p3).toInt();

        uint16_t value =
            cmd.substring(p3 + 1).toInt();

        Serial.println();

        if (writeHoldingRegister(
                unit,
                reg,
                value))
        {
            Serial.println("Write successful.");
        }
        else
        {
            Serial.println("Write failed.");
        }

        Serial.println();

        return;
    }

    //--------------------------------------------------
    // PROBE OFF
    //--------------------------------------------------

    if (cmd == "probe off")
    {
        probeEnabled = false;

        Serial.println();
        Serial.println("Probe disabled.");
        Serial.println();

        return;
    }

    //--------------------------------------------------
    // PROBE
    //--------------------------------------------------

    if (cmd.startsWith("probe "))
    {
        int p1 = cmd.indexOf(' ');
        int p2 = cmd.indexOf(' ', p1 + 1);

        if (p2 < 0)
        {
            Serial.println();
            Serial.println("Usage:");
            Serial.println("probe <unit> <register>");
            Serial.println("probe off");
            Serial.println();

            return;
        }

        probeUnit = cmd.substring(p1 + 1, p2).toInt();
        probeRegister = cmd.substring(p2 + 1).toInt();

        probeEnabled = true;
        watchEnabled = false;

        Serial.println();
        Serial.printf("Now probing Unit %u Register %u\n",
                      probeUnit,
                      probeRegister);
        Serial.println();

        return;
    }

    //--------------------------------------------------
    // WATCH
    //--------------------------------------------------

    if (cmd.startsWith("watch "))
    {
        String args = cmd.substring(6);
        args.trim();

        if (args == "off")
        {
            watchEnabled = false;
            resetRegisterWatch();

            Serial.println();
            Serial.println("Watch disabled.");
            Serial.println();

            return;
        }

        int p1 = args.indexOf(' ');
        int p2 = args.indexOf(' ', p1 + 1);

        if (p1 < 0 || p2 < 0)
        {
            Serial.println();
            Serial.println("Usage:");
            Serial.println("watch <unit> <start> <end>");
            Serial.println("watch off");
            Serial.println("record start");
            Serial.println("record stop");
            Serial.println();

            return;
        }

        watchUnit = (uint8_t)args.substring(0, p1).toInt();
        watchStartRegister = args.substring(p1 + 1, p2).toInt();
        watchEndRegister = args.substring(p2 + 1).toInt();

        probeEnabled = false;

        resetRegisterWatch();

        watchEnabled = true;

        Serial.println();
        Serial.printf("Watching Unit %u Registers %u -> %u\n",
                      watchUnit,
                      watchStartRegister,
                      watchEndRegister);
        Serial.println();

        return;
    }

    //--------------------------------------------------
    // RECORD
    //--------------------------------------------------

    if (cmd == "record start")
    {
        registerRecorderStart();

        Serial.println();
        Serial.println("Recorder started.");
        Serial.println();

        return;
    }

    if (cmd == "record stop")
    {
        registerRecorderStop();

        Serial.println();
        Serial.println("Recorder stopped.");
        Serial.println();

        return;
    }

    //--------------------------------------------------
    // HELP
    //--------------------------------------------------

    if (cmd == "help")
    {
        Serial.println();
        Serial.println("Available Commands");
        Serial.println("------------------");
        Serial.println("help");
        Serial.println("version");
        Serial.println("scan");
        Serial.println();
        Serial.println("write <unit> <register> <value>");
        Serial.println();
        Serial.println("probe <unit> <register>");
        Serial.println("probe off");
        Serial.println("watch <unit> <start> <end>");
        Serial.println("watch off");
        Serial.println("record start");
        Serial.println("record stop");
        Serial.println();

        return;
    }

    //--------------------------------------------------
    // VERSION
    //--------------------------------------------------

    if (cmd == "version")
    {
        Serial.println();

        Serial.printf("%s v%s\n",
                      PROJECT_NAME,
                      PROJECT_VERSION);

        Serial.println();

        return;
    }

    //--------------------------------------------------
    // SCAN
    //--------------------------------------------------

    if (cmd == "scan")
    {
        registerScannerStart();

        Serial.println();
        Serial.println("Starting register scan...");
        Serial.println();

        return;
    }

    //--------------------------------------------------

    Serial.print("Unknown command: ");
    Serial.println(cmd);
}