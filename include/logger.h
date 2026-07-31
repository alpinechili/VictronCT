#pragma once

void logInfo(const char* message);
void logWarn(const char* message);
void logError(const char* message);

void logInfof(const char* format, ...);
void logWarnf(const char* format, ...);
void logErrorf(const char* format, ...);