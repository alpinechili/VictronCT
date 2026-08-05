#pragma once

#include <stdint.h>

#include "developer_registers.h"

class DeveloperConsole
{
public:

    static void begin();

    static void clear();

    static void title(const char *text);

    static void line();

    static void value(const DeveloperRegister *info,
                      uint16_t rawValue);

    static void unknown(uint16_t reg,
                        int16_t value);

    static void footer();

private:

    static bool firstScreen;
};