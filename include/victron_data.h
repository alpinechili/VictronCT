#pragma once

struct VictronData
{
    bool online = false;

    //======================================================
    // Battery (Victron)
    //======================================================

    float batterySOC = 0.0f;
    float batteryVoltage = 0.0f;
    float batteryCurrent = 0.0f;
    float batteryPower = 0.0f;

    //======================================================
    // Solar (Victron)
    //======================================================

    float pvPower = 0.0f;
    float pvCurrent = 0.0f;

    //======================================================
    // Grid (Victron)
    //======================================================

    float gridPower = 0.0f;
    float gridCurrent = 0.0f;

    //======================================================
    // House Loads (Victron)
    //======================================================

    float loadPower = 0.0f;
    float loadCurrent = 0.0f;

    //======================================================
    // CT Measurements
    //======================================================

    float ctGridCurrent = 0.0f;
    float ctSolarCurrent = 0.0f;
    float ctHouseCurrent = 0.0f;
    float ctImmersionCurrent = 0.0f;
};

extern VictronData victron;