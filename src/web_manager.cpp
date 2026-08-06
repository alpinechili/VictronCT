#include "web_manager.h"

#include <WebServer.h>
#include <WiFi.h>

#include "config.h"
#include "modbus_manager.h"
#include "mqtt_manager.h"
#include "victron_data.h"

WebServer server(80);

static String html()
{
    String page;

    page += "<!DOCTYPE html><html>";
    page += "<head>";
    page += "<meta charset='utf-8'>";
    page += "<meta http-equiv='refresh' content='5'>";
    page += "<title>VictronCT Gateway</title>";
    page += "<style>";
    page += "body{font-family:Arial;background:#222;color:#eee;padding:20px;}";
    page += "table{border-collapse:collapse;}";
    page += "td{padding:6px 12px;border-bottom:1px solid #444;}";
    page += "h2{color:#6cf;}";
    page += "</style>";
    page += "</head><body>";

    page += "<h2>";
    page += PROJECT_NAME;
    page += "</h2>";

    page += "<table>";

    page += "<tr><td>Firmware</td><td>";
    page += PROJECT_VERSION;
    page += "</td></tr>";

    page += "<tr><td>IP Address</td><td>";
    page += WiFi.localIP().toString();
    page += "</td></tr>";

    page += "<tr><td>WiFi RSSI</td><td>";
    page += WiFi.RSSI();
    page += " dBm</td></tr>";

    page += "<tr><td>Modbus</td><td>";
    page += modbusConnected() ? "Connected" : "Disconnected";
    page += "</td></tr>";

    page += "<tr><td>Battery SOC</td><td>";
    page += String(victron.batterySOC,0);
    page += " %</td></tr>";

    page += "<tr><td>Battery Voltage</td><td>";
    page += String(victron.batteryVoltage,1);
    page += " V</td></tr>";

    page += "<tr><td>Battery Current</td><td>";
    page += String(victron.batteryCurrent,1);
    page += " A</td></tr>";

    page += "<tr><td>Grid Power</td><td>";
    page += String(victron.gridPowerL1);
    page += " W</td></tr>";

    page += "<tr><td>House Load</td><td>";
    page += String(victron.acConsumptionL1);
    page += " W</td></tr>";

    page += "<tr><td>PV Power</td><td>";
    page += String(victron.pvTotalPower);
    page += " W</td></tr>";

    page += "<tr><td>Free Heap</td><td>";
    page += String(ESP.getFreeHeap()/1024);
    page += " KB</td></tr>";

    page += "<tr><td>Uptime</td><td>";
    page += String(millis()/1000);
    page += " sec</td></tr>";

    page += "</table>";

    page += "<br><button onclick=\"location.href='/restart'\">Restart ESP32</button>";

    page += "</body></html>";

    return page;
}

void webBegin()
{
    server.on("/", []()
    {
        server.send(200,"text/html",html());
    });

    server.on("/restart", []()
    {
        server.send(200,"text/plain","Restarting...");
        delay(1000);
        ESP.restart();
    });

    server.begin();

    Serial.println("Web Server Ready");
}

void webLoop()
{
    server.handleClient();
}