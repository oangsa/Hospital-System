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
        void printLog(const char* msg);

};


#endif
