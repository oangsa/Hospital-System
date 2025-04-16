#include "Logger.h"
#include <cstdio>
#include <cstdarg>
#include <cstdlib>

Logger::Logger() {};

void Logger::log(const char* fmt, ...) {
    // determine required buffer size
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);
    if(len < 0) return;

    // format message
    char msg[len + 1]; // or use heap allocation if implementation doesn't support VLAs
    va_start(args, fmt);
    vsnprintf(msg, len + 1, fmt, args);
    va_end(args);

    // call printLog with formatted message
    this->printLog(msg);
}
