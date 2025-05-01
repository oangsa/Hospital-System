#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "Define.h"
#define MIN_PASSWORD_LEN 8

class Validator {
    private:
        u_int16 getDayInMonth(u_int16 month);
        u_int16 isLeapYear(u_int16 year);
        u_int16 isUpper(const string& str);
        u_int16 isLower(const string& str);
        u_int16 isNumber(const string& str);
        u_int16 isSpecial(const string& str);
        u_int16 isComma(const string& str);
        time_t parseStringToTime_t(const string& time);

    public:
        Validator();
        VALIDATOR_ERROR_TYPE isBirthDateValid(BirthDate b);
        VALIDATOR_ERROR_TYPE isStringValid(string line);
        VALIDATOR_ERROR_TYPE isBirthDateStringValid(string line);

        template<typename T>
        VALIDATOR_ERROR_TYPE isNegative(T number);

        VALIDATOR_ERROR_TYPE isPasswordValid(string password);
};

#endif
