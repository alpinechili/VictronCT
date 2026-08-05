#include <Arduino.h>

#include "config.h"
#include "modbus_manager.h"
#include "mqtt_manager.h"
#include "register_mqtt.h"
#include "register_poller.h"
#include "register_probe.h"
#include "register_recorder.h"
#include "register_scanner.h"
#include "register_watch.h"
#include "serial_commands.h"
#include "wifi_manager.h"

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("======================================");
    Serial.println(PROJECT_NAME);
    Serial.print("Version : ");
    Serial.println(PROJECT_VERSION);
    Serial.println("======================================");
    Serial.println();

    wifiBegin();
    mqttBegin();
    modbusBegin();

    registerMqttBegin();

    if (ENABLE_REGISTER_POLLER)
        registerPollerBegin();

    if (ENABLE_DEVELOPER_MODE && ENABLE_REGISTER_PROBE)
        registerProbeBegin();

    if (ENABLE_REGISTER_SCANNER)
        registerScannerBegin();

    if (ENABLE_REGISTER_WATCH)
        registerWatchBegin();

    registerRecorderBegin();
}

void loop()
{
    wifiLoop();

    if (ENABLE_MQTT)
        mqttLoop();

    if (ENABLE_MODBUS)
        modbusLoop();

    registerMqttLoop();

    if (ENABLE_REGISTER_POLLER)
        registerPollerLoop();

    if (ENABLE_DEVELOPER_MODE)
    {
        serialCommandsLoop();

        if (ENABLE_REGISTER_PROBE)
            registerProbeLoop();
    }

    if (ENABLE_REGISTER_WATCH)
        registerWatchLoop();

    registerRecorderLoop();

    delay(10);
}