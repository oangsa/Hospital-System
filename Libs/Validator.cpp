#include "Validator.h"
#include "Define.h"
#include <ctime>

Validator::Validator() {};

u_int16 Validator::getDayInMonth(u_int16 month) {
    switch (month) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return 31;

        case 4:
        case 6:
        case 11:
        case 9:
            return 30;

        case 2:
            return 28;

        default:
            return 99;
    }
}

VALIDATOR_ERROR_TYPE Validator::isBirthDateValid(BirthDate b) {
    time_t curTime = time(NULL);
    struct tm* now = localtime(&curTime);
    u_int16 curYear = now->tm_year + 1900;
    u_int16 day = this->getDayInMonth(b._month);
    
    if (b._year > curYear) return VALIDATOR_ERROR_TYPE::YEAR_ERROR;
    if (b._month > 12 || b._month < 1) return VALIDATOR_ERROR_TYPE::MONTH_ERROR;
    if (b._day < 1 || b._day > day) return VALIDATOR_ERROR_TYPE::DAY_ERROR;

    return VALIDATOR_ERROR_TYPE::NO_ERROR;

}
