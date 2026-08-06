#include "ota_manager.h"

#include <ArduinoOTA.h>
#include <WiFi.h>

#include "config.h"

void otaBegin()
{
    ArduinoOTA.setHostname(PROJECT_NAME);

    ArduinoOTA
        .onStart([]()
        {
            Serial.println();
            Serial.println("======================================");
            Serial.println("OTA Update Started");
            Serial.println("======================================");
        });

    ArduinoOTA
        .onEnd([]()
        {
            Serial.println();
            Serial.println("======================================");
            Serial.println("OTA Update Complete");
            Serial.println("======================================");
        });

    ArduinoOTA
        .onProgress([](unsigned int progress,
                       unsigned int total)
        {
            Serial.printf(
                "\rOTA Progress: %u%%",
                (progress * 100) / total);
        });

    ArduinoOTA
        .onError([](ota_error_t error)
        {
            Serial.printf(
                "\nOTA Error %u\n",
                error);
        });

    ArduinoOTA.begin();Serial.print("OTA listening on: ");
    Serial.println(WiFi.localIP());

    Serial.println("OTA Ready");
}

void otaLoop()
{
    ArduinoOTA.handle();
}