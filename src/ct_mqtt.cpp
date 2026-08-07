#include "ct_mqtt.h"

#include <Arduino.h>

#include "mqtt_manager.h"
#include "victron_data.h"

static unsigned long lastPublish = 0;

bool ctMqttBegin()
{
    lastPublish = 0;
    return true;
}

void ctMqttLoop()
{
    if (!mqttConnected())
        return;

    if (millis() - lastPublish < 5000)
        return;

    lastPublish = millis();

    char payload[32];

    dtostrf(victron.ct1Power, 0, 2, payload);
    mqttPublish("VictronCT/ct/1/power", payload);

    dtostrf(victron.ct2Power, 0, 2, payload);
    mqttPublish("VictronCT/ct/2/power", payload);

    dtostrf(victron.ct3Power, 0, 2, payload);
    mqttPublish("VictronCT/ct/3/power", payload);

    dtostrf(victron.ct4Power, 0, 2, payload);
    mqttPublish("VictronCT/ct/4/power", payload);

    dtostrf(victron.ctImportPower, 0, 2, payload);
    mqttPublish("VictronCT/ct/import", payload);

    dtostrf(victron.ctExportPower, 0, 2, payload);
    mqttPublish("VictronCT/ct/export", payload);

    dtostrf(victron.ctNetPower, 0, 2, payload);
    mqttPublish("VictronCT/ct/net", payload);
}