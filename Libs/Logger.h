#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <ctime>
#include <sstream>
#include <string>

class Logger {
    public:
        Logger();
        void log(const char* fmt, ...);

    private:
        void printLog(const char* msg) {
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

};


#endif
