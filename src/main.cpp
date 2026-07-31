#include <Arduino.h>

#include "config.h"
#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "modbus_manager.h"
#include "register_scanner.h"
#include "ct_manager.h"
#include "serial_commands.h"

void setup()
{
    Serial.begin(115200);

    Serial.println();
    Serial.println("======================================");
    Serial.printf("%s\n", PROJECT_NAME);
    Serial.printf("Version : %s\n", PROJECT_VERSION);
    Serial.println("======================================");

    serialCommandsBegin();

    wifiBegin();

    if (ENABLE_MQTT)
        mqttBegin();

    if (ENABLE_MODBUS)
        modbusBegin();

    if (ENABLE_REGISTER_SCANNER)
        registerScannerBegin();

    if (ENABLE_CT_MANAGER)
        ctManagerBegin();
}

void loop()
{
    serialCommandsLoop();

    wifiLoop();

    if (ENABLE_MQTT)
        mqttLoop();

    if (ENABLE_MODBUS)
        modbusLoop();

    if (ENABLE_REGISTER_SCANNER)
        registerScannerLoop();

    if (ENABLE_CT_MANAGER)
        ctManagerLoop();
}