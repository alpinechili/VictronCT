#include "ct_channel.h"

CTChannel::CTChannel()
{
    m_power = 0.0f;
}

void CTChannel::begin()
{
    m_power = 0.0f;
}

void CTChannel::update()
{
    //
    // ADC sampling will go here
    //
}

float CTChannel::power() const
{
    return m_power;
}

void CTChannel::setPower(float watts)
{
    m_power = watts;
}