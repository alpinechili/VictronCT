#include "ct_channel.h"

#include <Arduino.h>
#include <math.h>

CTChannel::CTChannel()
{
    m_adcPin = 0;

    m_calibration = 1.0f;
    m_mainsVoltage = 230.0f;

    m_raw = 0.0f;
    m_offset = 2048.0f;

    m_rms = 0.0f;
    m_current = 0.0f;
    m_power = 0.0f;
}

void CTChannel::begin(
    uint8_t adcPin,
    float calibration,
    float mainsVoltage)
{
    m_adcPin = adcPin;

    m_calibration = calibration;
    m_mainsVoltage = mainsVoltage;

    pinMode(m_adcPin, INPUT);
}

void CTChannel::update()
{
    constexpr uint16_t SAMPLE_COUNT = 400;

    double sumSquares = 0.0;

    for (uint16_t i = 0; i < SAMPLE_COUNT; i++)
    {
        m_raw = analogRead(m_adcPin);

        // Slowly track the ADC midpoint
        m_offset =
            (m_offset * 0.999f) +
            (m_raw * 0.001f);

        float sample = m_raw - m_offset;

        sumSquares += sample * sample;
    }

    m_rms = sqrt(sumSquares / SAMPLE_COUNT);

    // Convert ADC RMS into current
    m_current = m_rms * m_calibration;

    // Calculate apparent power
    m_power = m_current * m_mainsVoltage;
}

float CTChannel::raw() const
{
    return m_raw;
}

float CTChannel::offset() const
{
    return m_offset;
}

float CTChannel::rms() const
{
    return m_rms;
}

float CTChannel::current() const
{
    return m_current;
}

float CTChannel::power() const
{
    return m_power;
}