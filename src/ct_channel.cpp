#include "ct_channel.h"

#include <Arduino.h>

CTChannel::CTChannel()
{
    m_adcPin = 0;

    m_raw = 0.0f;
    m_offset = 2048.0f;

    m_rms = 0.0f;
    m_current = 0.0f;
    m_power = 0.0f;
}

void CTChannel::begin(uint8_t adcPin)
{
    m_adcPin = adcPin;

    pinMode(m_adcPin, INPUT);
}

void CTChannel::update()
{
    m_raw = analogRead(m_adcPin);

    float sample = m_raw - m_offset;

    m_offset =
        (m_offset * 0.999f) +
        (m_raw * 0.001f);

    m_rms = sample;

    m_current = 0.0f;

    m_power = 0.0f;
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