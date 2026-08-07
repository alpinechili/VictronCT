/***************************************************************************************************************************
****************************************************************************************************************************
****************************************************************************************************************************
A simple library that reads an ESP32 4-Channel CT Sensor Board
Based On:   EmonLib
https://github.com/openenergymonitor/EmonLib

Author:     David Mottram
Updated:    16th May 2023
Optimised:  AlpineChili
****************************************************************************************************************************
****************************************************************************************************************************
***************************************************************************************************************************/

#include "Arduino.h"
#include "ESP32_4CH_CT.h"

//-----------------------------------------------------------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------------------------------------------------------

ESP32_4CH_CT::ESP32_4CH_CT(
    int _AD_1,
    int _AD_2,
    int _AD_3,
    int _AD_4,
    int _Samples_Taken,
    int _AD_delay,
    double _ICAL)
{
    AD_Channel_1 = _AD_1;
    AD_Channel_2 = _AD_2;
    AD_Channel_3 = _AD_3;
    AD_Channel_4 = _AD_4;

    Samples_Taken = _Samples_Taken;
    AD_delay = _AD_delay;

    ICAL = _ICAL;
}

//-----------------------------------------------------------------------------------------------------------------------------
// Report settings
//-----------------------------------------------------------------------------------------------------------------------------

void ESP32_4CH_CT::report()
{
    Serial.println("Report settings sent to library");
    Serial.println("-------------------------------");

    Serial.print("A/D Channel 1 = GPIO");
    Serial.println(AD_Channel_1);

    Serial.print("A/D Channel 2 = GPIO");
    Serial.println(AD_Channel_2);

    Serial.print("A/D Channel 3 = GPIO");
    Serial.println(AD_Channel_3);

    Serial.print("A/D Channel 4 = GPIO");
    Serial.println(AD_Channel_4);

    Serial.print("Samples = ");
    Serial.println(Samples_Taken);

    Serial.print("A/D delay(uS) = ");
    Serial.println(AD_delay);

    Serial.print("ICAL = ");
    Serial.println(ICAL);

    Serial.println("-------------------------------");
}

//-----------------------------------------------------------------------------------------------------------------------------
// Read one CT channel
//-----------------------------------------------------------------------------------------------------------------------------

double ESP32_4CH_CT::power_sample(int Input_Channel)
{
    sumI = 0;

    int adcPin;

    switch (Input_Channel)
    {
        case 0:
            adcPin = AD_Channel_1;
            break;

        case 1:
            adcPin = AD_Channel_2;
            break;

        case 2:
            adcPin = AD_Channel_3;
            break;

        default:
            adcPin = AD_Channel_4;
            break;
    }

    for (unsigned int zz = 0; zz < Samples_Taken; zz++)
    {
        sampleI = analogRead(adcPin);

        if (AD_delay > 0)
            delayMicroseconds(AD_delay);

        offsetI[Input_Channel] +=
            (sampleI - offsetI[Input_Channel]) / 1024.0;

        filteredI =
            sampleI - offsetI[Input_Channel];

        sumI +=
            filteredI * filteredI;
    }

    double I_RATIO =
        ICAL * (SupplyVoltage / ADC_COUNTS);

    Irms =
        I_RATIO * sqrt(sumI / Samples_Taken);

    return Irms;
}