#include "web_manager.h"

#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>

#include "config.h"
#include "modbus_manager.h"
#include "victron_data.h"

WebServer server(80);

static void sendStatusJson()
{
    String json = "{";

    json += "\"firmware\":\"";
    json += PROJECT_VERSION;
    json += "\",";

    json += "\"ip\":\"";
    json += WiFi.localIP().toString();
    json += "\",";

    json += "\"rssi\":";
    json += WiFi.RSSI();
    json += ",";

    json += "\"modbus\":";
    json += modbusConnected() ? "true" : "false";
    json += ",";

    json += "\"soc\":";
    json += String(victron.batterySOC,0);
    json += ",";

    json += "\"voltage\":";
    json += String(victron.batteryVoltage,1);
    json += ",";

    json += "\"current\":";
    json += String(victron.batteryCurrent,1);
    json += ",";

    json += "\"grid\":";
    json += String(victron.gridPowerL1);
    json += ",";

    json += "\"house\":";
    json += String(victron.acConsumptionL1);
    json += ",";

    json += "\"pv\":";
    json += String(victron.pvTotalPower);
    json += ",";

    json += "\"heap\":";
    json += String(ESP.getFreeHeap()/1024);
    json += ",";

    json += "\"uptime\":";
    json += String(millis()/1000);

    json += "}";

    server.send(200,"application/json",json);
}

static String html()
{
    String page;

    page += R"rawliteral(
<!DOCTYPE html>
<html>
<head>

<meta charset="utf-8">
<meta http-equiv="refresh" content="5">

<title>VictronCT</title>

<style>

body{
    background:#222;
    color:#eee;
    font-family:Arial;
    margin:20px;
}

h2{
    color:#4fc3f7;
}

nav{
    margin-bottom:20px;
}

nav a{
    color:white;
    padding:8px 12px;
    text-decoration:none;
    background:#444;
    margin-right:5px;
}

table{
    border-collapse:collapse;
}

td{
    padding:6px 12px;
    border-bottom:1px solid #444;
}

button{
    margin-top:20px;
    padding:8px 14px;
}

</style>

</head>

<body>

<h2>VictronCT Gateway</h2>

<nav>

<a href="/">Status</a>

<a href="/api/status">JSON</a>

<a href="/restart">Restart</a>

</nav>

<table>
)rawliteral";

    page += "<tr><td>Firmware</td><td>"+String(PROJECT_VERSION)+"</td></tr>";
    page += "<tr><td>IP Address</td><td>"+WiFi.localIP().toString()+"</td></tr>";
    page += "<tr><td>RSSI</td><td>"+String(WiFi.RSSI())+" dBm</td></tr>";
    page += "<tr><td>Modbus</td><td>"+String(modbusConnected()?"Connected":"Disconnected")+"</td></tr>";

    page += "<tr><td>Battery SOC</td><td>"+String(victron.batterySOC,0)+" %</td></tr>";
    page += "<tr><td>Battery Voltage</td><td>"+String(victron.batteryVoltage,1)+" V</td></tr>";
    page += "<tr><td>Battery Current</td><td>"+String(victron.batteryCurrent,1)+" A</td></tr>";

    page += "<tr><td>Grid Power</td><td>"+String(victron.gridPowerL1)+" W</td></tr>";
    page += "<tr><td>House Load</td><td>"+String(victron.acConsumptionL1)+" W</td></tr>";
    page += "<tr><td>PV Power</td><td>"+String(victron.pvTotalPower)+" W</td></tr>";

    page += "<tr><td>Heap</td><td>"+String(ESP.getFreeHeap()/1024)+" KB</td></tr>";
    page += "<tr><td>Uptime</td><td>"+String(millis()/1000)+" sec</td></tr>";

    page += "</table>";

    page += R"rawliteral(

<button onclick="location.href='/restart'">
Restart ESP32
</button>

</body>
</html>

)rawliteral";

    return page;
}

void webBegin()
{
    server.on("/",HTTP_GET,[]()
    {
        server.send(200,"text/html",html());
    });

    server.on("/api/status",HTTP_GET,[]()
    {
        sendStatusJson();
    });

    server.on("/restart",HTTP_GET,[]()
    {
        server.send(200,"text/plain","Restarting...");
        delay(1000);
        ESP.restart();
    });

    server.on("/favicon.ico",HTTP_GET,[]()
    {
        server.send(204);
    });

    server.onNotFound([]()
    {
        server.send(404,"text/plain","404");
    });

    server.begin();

    Serial.println("Web Server Ready");
}

void webLoop()
{
    server.handleClient();
}