#pragma once

struct VictronData
{
    //--------------------------------------------------
    // Status
    //--------------------------------------------------

    bool online = false;

    //--------------------------------------------------
    // Battery
    //--------------------------------------------------

    float batteryVoltage = 0;
    float batteryCurrent = 0;
    float batteryPower = 0;
    float batterySOC = 0;

    //--------------------------------------------------
    // AC
    //--------------------------------------------------

    float gridPowerL1 = 0;
    float acConsumptionL1 = 0;

    //--------------------------------------------------
    // PV
    //--------------------------------------------------

    float pvInverter1Power = 0;
    float pvInverter2Power = 0;
    float pvTotalPower = 0;

    float pvDcPower = 0;
    float chargerPower = 0;

    //--------------------------------------------------
    // CT Sensors
    //--------------------------------------------------

    float ct1Power = 0;
    float ct2Power = 0;
    float ct3Power = 0;
    float ct4Power = 0;

    float ctImportPower = 0;
    float ctExportPower = 0;
    float ctNetPower = 0;
};

extern VictronData victron;