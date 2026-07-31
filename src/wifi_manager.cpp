#include <Arduino.h>
#include <WiFi.h>

#include "wifi_manager.h"
#include "secrets.h"

static bool wasConnected = false;

bool wifiBegin()
{
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(false);

    Serial.println();
    Serial.println("--------------------------------");
    Serial.println("Connecting to WiFi...");
    Serial.print("SSID: ");
    Serial.println(SECRET_WIFI_SSID);

    WiFi.begin(SECRET_WIFI_SSID, SECRET_WIFI_PASSWORD);

    return true;
}

void wifiLoop()
{
    bool connected = (WiFi.status() == WL_CONNECTED);

    if (connected && !wasConnected)
    {
        Serial.println();
        Serial.println("WiFi Connected");
        Serial.print("IP Address : ");
        Serial.println(WiFi.localIP());

        Serial.print("RSSI       : ");
        Serial.print(WiFi.RSSI());
        Serial.println(" dBm");

        wasConnected = true;
    }

    if (!connected && wasConnected)
    {
        Serial.println("WiFi Lost - reconnecting...");
        wasConnected = false;
    }

    if (!connected)
    {
        static unsigned long lastRetry = 0;

        if (millis() - lastRetry > 10000)
        {
            lastRetry = millis();

            Serial.println("Retrying WiFi...");
            WiFi.disconnect();
            WiFi.begin(SECRET_WIFI_SSID, SECRET_WIFI_PASSWORD);
        }
    }
}

bool wifiConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

String wifiIP()
{
    if (!wifiConnected())
        return "";

    return WiFi.localIP().toString();
}

String wifiSSID()
{
    return WiFi.SSID();
}

long wifiRSSI()
{
    return WiFi.RSSI();
}