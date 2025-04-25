#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "Define.h"

class Validator {
    private:
        u_int16 getDayInMonth(u_int16 month);

    public:
        Validator();
        VALIDATOR_ERROR_TYPE isBirthDateValid(BirthDate b);
};

#endif
