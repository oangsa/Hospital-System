#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "Define.h"
#define MIN_PASSWORD_LEN 8

class Validator {
    private:
        u_int16 getDayInMonth(u_int16 month);
        u_int16 isUpper(const string& str);
        u_int16 isLower(const string& str);
        u_int16 isNumber(const string& str);
        u_int16 isSpecial(const string& str);

    public:
        Validator();
        VALIDATOR_ERROR_TYPE isBirthDateValid(BirthDate b);

        template<typename T>
        VALIDATOR_ERROR_TYPE isNegative(T number);

        VALIDATOR_ERROR_TYPE isPasswordValid(string password);
};

#endif
