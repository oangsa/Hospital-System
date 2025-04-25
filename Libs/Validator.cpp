#include "Validator.h"
#include "Define.h"

// Validator::Validator() {};

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
