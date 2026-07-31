#include "logger.h"

#include <Arduino.h>
#include <stdarg.h>

#include "config.h"

namespace
{
    void logMessage(
        const char* level,
        const char* format,
        va_list args)
    {
        if (!ENABLE_LOGGING)
            return;

        Serial.print("[");
        Serial.print(level);
        Serial.print("] ");

        char buffer[256];
        vsnprintf(buffer, sizeof(buffer), format, args);

        Serial.println(buffer);
    }
}

void logInfo(const char* message)
{
    if (!ENABLE_LOGGING)
        return;

    Serial.print("[INFO] ");
    Serial.println(message);
}

void logWarn(const char* message)
{
    if (!ENABLE_LOGGING)
        return;

    Serial.print("[WARN] ");
    Serial.println(message);
}

void logError(const char* message)
{
    if (!ENABLE_LOGGING)
        return;

    Serial.print("[ERROR] ");
    Serial.println(message);
}

void logInfof(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    logMessage("INFO", format, args);
    va_end(args);
}

void logWarnf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    logMessage("WARN", format, args);
    va_end(args);
}

void logErrorf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    logMessage("ERROR", format, args);
    va_end(args);
}