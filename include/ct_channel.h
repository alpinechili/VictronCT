#pragma once

class CTChannel
{
public:

    CTChannel();

    void begin();

    void update();

    float power() const;

    void setPower(float watts);

private:

    float m_power;
};