#pragma once

#include <stdint.h>

class CTChannel
{
public:

    CTChannel();

    void begin(
        uint8_t adcPin,
        float calibration,
        float mainsVoltage);

    void update();

    float raw() const;
    float offset() const;
    float rms() const;
    float current() const;
    float power() const;

private:

    uint8_t m_adcPin;

    float m_calibration;
    float m_mainsVoltage;

    float m_raw;
    float m_offset;

    float m_rms;
    float m_current;
    float m_power;
};