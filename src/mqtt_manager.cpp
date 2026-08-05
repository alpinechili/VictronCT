#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "modbus_manager.h"
#include "secrets.h"

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

//------------------------------------------------------
// MQTT Topics
//------------------------------------------------------

static const char* TOPIC_STATUS    = "VictronCT/status";
static const char* TOPIC_HEARTBEAT = "VictronCT/heartbeat";
static const char* TOPIC_WRITE     = "VictronCT/write/#";

//------------------------------------------------------

unsigned long lastHeartbeat = 0;

//------------------------------------------------------
// MQTT Callback
//------------------------------------------------------

static void mqttCallback(
    char* topic,
    byte* payload,
    unsigned int length)
{
    String topicString = topic;

    if (!topicString.startsWith("VictronCT/write/"))
        return;

    String path = topicString.substring(
        strlen("VictronCT/write/"));

    int separator = path.indexOf('/');

    if (separator < 0)
        return;

    uint8_t unit =
        path.substring(0, separator).toInt();

    uint16_t reg =
        path.substring(separator + 1).toInt();

    char valueBuffer[32];

    if (length >= sizeof(valueBuffer))
        return;

    memcpy(
        valueBuffer,
        payload,
        length);

    valueBuffer[length] = 0;

    uint16_t value =
        atoi(valueBuffer);

    Serial.println();
    Serial.println("MQTT WRITE REQUEST");

    Serial.printf(
        "Unit     : %u\n",
        unit);

    Serial.printf(
        "Register : %u\n",
        reg);

    Serial.printf(
        "Value    : %u\n",
        value);

    if (writeHoldingRegister(
            unit,
            reg,
            value))
    {
        Serial.println("Write OK");
    }
    else
    {
        Serial.println("Write FAILED");
    }

    Serial.println();
}

//------------------------------------------------------

bool mqttBegin()
{
    mqttClient.setServer(
        SECRET_MQTT_SERVER,
        SECRET_MQTT_PORT);

    mqttClient.setCallback(
        mqttCallback);

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

            mqttClient.subscribe(
                TOPIC_WRITE);

            lastHeartbeat = millis();
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
    // Heartbeat every 30 seconds
    //--------------------------------------------------

    if (millis() - lastHeartbeat >= 30000)
    {
        lastHeartbeat = millis();

        char uptime[20];

        sprintf(
            uptime,
            "%lu",
            millis() / 1000);

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

bool mqttPublish(
    const char* topic,
    const char* payload)
{
    if (!mqttConnected())
    {
        Serial.println("MQTT publish FAILED (not connected)");
        return false;
    }

    bool ok = mqttClient.publish(
        topic,
        payload,
        true);

    Serial.print(ok ? "MQTT OK: " : "MQTT FAILED: ");
    Serial.println(topic);

    return ok;
}