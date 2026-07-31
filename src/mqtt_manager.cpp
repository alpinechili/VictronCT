#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "mqtt_publish.h"
#include "secrets.h"

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

//------------------------------------------------------
// MQTT Topics
//------------------------------------------------------

static const char* TOPIC_STATUS    = "VictronCT/status";
static const char* TOPIC_HEARTBEAT = "VictronCT/heartbeat";

//------------------------------------------------------

unsigned long lastHeartbeat = 0;
unsigned long lastPublish   = 0;

//------------------------------------------------------

bool mqttBegin()
{
    mqttClient.setServer(
        SECRET_MQTT_SERVER,
        SECRET_MQTT_PORT);

    return true;
}

//------------------------------------------------------

void mqttLoop()
{
    if (!wifiConnected())
        return;

    if (!mqttClient.connected())
    {
        Serial.print("Connecting to MQTT... ");

        bool connected = mqttClient.connect(
            DEVICE_NAME,
            SECRET_MQTT_USER,
            SECRET_MQTT_PASSWORD,
            TOPIC_STATUS,
            0,
            true,
            "offline");

        if (connected)
        {
            Serial.println("Connected");

            mqttClient.publish(
                TOPIC_STATUS,
                "online",
                true);

            lastHeartbeat = millis();
            lastPublish   = 0;
        }
        else
        {
            Serial.print("Failed (");
            Serial.print(mqttClient.state());
            Serial.println(")");

            delay(3000);
            return;
        }
    }

    mqttClient.loop();

    //--------------------------------------------------
    // Publish Victron data every 5 seconds
    //--------------------------------------------------

    if (millis() - lastPublish >= 5000)
    {
        lastPublish = millis();

        mqttPublishVictronData();
    }

    //--------------------------------------------------
    // Heartbeat every 30 seconds
    //--------------------------------------------------

    if (millis() - lastHeartbeat >= 30000)
    {
        lastHeartbeat = millis();

        char uptime[20];
        sprintf(uptime, "%lu", millis() / 1000);

        mqttClient.publish(
            TOPIC_HEARTBEAT,
            uptime,
            false);
    }
}

//------------------------------------------------------

bool mqttConnected()
{
    return mqttClient.connected();
}

//------------------------------------------------------

void mqttPublish(
    const char* topic,
    const char* payload)
{
    if (!mqttConnected())
        return;

    mqttClient.publish(
        topic,
        payload,
        true);
}