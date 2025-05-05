#include "Logger.h"
#include <cstdio>
#include <cstdarg>
#include <cstdlib>

Logger::Logger() {};

/*
    Function to log messages to a file
    Parameters:
        const char* fmt: Format string for the message
        ...: Additional arguments for the format string
*/
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

/*
    Function to print log messages to a file
    Parameters:
        const char* msg: Message to be logged
*/

void Logger::printLog(const char* msg) {
    time_t current_time;
    std::ofstream file;
    std::string str;
    std::stringstream ss;

    file.open("log.txt", std::ios::app | std::ios::out);

    current_time = time(NULL);
    struct tm *tm_local = localtime(&current_time);

    ss << "[" << (tm_local->tm_hour < 10 ? "0" : "") << tm_local->tm_hour << ":" << (tm_local->tm_min < 10 ? "0" : "") << tm_local->tm_min << ":" << (tm_local->tm_sec < 10 ? "0" : "") << tm_local->tm_sec << "] " << msg << "\n";

    file << ss.str();

    file.close();
}
