/**************************************************
 *                                                *
 *       Validator Class Implementation File      *
 *       Latest Update: May 5, 2025               *
 *                                                *
 **************************************************/

#include "Validator.h"
#include "Define.h"
#include <ctime>
#include <regex>
#include <sstream>
#include <iomanip>

Validator::Validator() {};

// Private
/*
    Handling function to get the number of days in a month
    Params: u_int16 month
    return u_int16 count of days in the month
*/
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

/*
    Handling function to check if the year is leap year
    Params: u_int16 year
    return u_int16 1 if leap year, else 0
*/
u_int16 Validator::isLeapYear(u_int16 year) {
    if (year % 4 != 0) return 0;
    if (year % 100 != 0) return 1;
    if (year % 400 != 0) return 0;
    return 1;
}

/*
    Handling function to check if the string has upper case char
    Params: const string& str
    return u_int16 1 if has upper case char, else 0
*/
u_int16 Validator::isUpper(const string& str) {
    return std::regex_search(str, std::regex("[A-Z]"));
}

/*
    Handling function to check if the string has lower case char
    Params: const string& str
    return u_int16 1 if has lower case char, else 0
*/
u_int16 Validator::isLower(const string& str) {
    return std::regex_search(str, std::regex("[a-z]"));
}

/*
    Handling function to check if the string has number
    Params: const string& str
    return u_int16 1 if has number, else 0
*/
u_int16 Validator::isNumber(const string& str) {
    return std::regex_search(str, std::regex("[0-9]"));
}

/*
    Handling function to check if the string has special char
    Params: const string& str
    return u_int16 1 if has special char, else 0
*/
u_int16 Validator::isSpecial(const string& str) {
    return std::regex_search(str, std::regex("[!.@$%&]"));
}

/*
    Handling function to check if the string has comma
    Params: const string& str
    return u_int16 1 if has comma, else 0
*/
u_int16 Validator::isComma(const string& str) {
    return std::regex_search(str, std::regex("[,]"));
}

/*
    Handling function to convert string to time_t
    NOTE: string should be in "2024-03-14" this form

    Params: const string& str
    return time_t
*/
time_t Validator::parseStringToTime_t(const string& str) {
    tm tm = {};
    istringstream ss(str);

    ss >> std::get_time(&tm, "%Y-%m-%d");
    time_t date = mktime(&tm);

    return date;
}

// Public
/*
    This function use to validate birth day
    Params: BirthDate b
    return VALIDATOR_ERROR_TYPE
*/
VALIDATOR_ERROR_TYPE Validator::isBirthDateValid(BirthDate b) {
    time_t curTime = time(NULL);
    struct tm* now = localtime(&curTime);
    u_int16 curYear = now->tm_year + 1900;
    u_int16 day = this->getDayInMonth(b._month);
    stringstream ss;
    ss << (b._year) << "-" << b._month << "-" << b._day;
    time_t birTime = this->parseStringToTime_t(ss.str());


    day += (b._month == 2 && this->isLeapYear(b._year));

    // No way there's people born before 1900 and still alive.
    if (birTime > curTime) return VALIDATOR_ERROR_TYPE::FUTURE_DATE_ERROR;
    if (b._year > curYear || b._year < 1900) return VALIDATOR_ERROR_TYPE::YEAR_ERROR;
    if (b._month > 12 || b._month < 1) return VALIDATOR_ERROR_TYPE::MONTH_ERROR;
    if (b._day < 1 || b._day > day) return VALIDATOR_ERROR_TYPE::DAY_ERROR;

    return VALIDATOR_ERROR_TYPE::NO_ERROR;

}

/*
    check if number is negative
    Params: T number
    return: VALIDATOR_ERROR_TYPE::NO_ERROR if number is not negative, else VALIDATOR_ERROR_TYPE::NEGATIVE_NUMBER_ERROR
*/
template<typename T>
VALIDATOR_ERROR_TYPE Validator::isNegative(T number) {
    return (number > 0) ? VALIDATOR_ERROR_TYPE::NO_ERROR : VALIDATOR_ERROR_TYPE::NEGATIVE_NUMBER_ERROR;
}

/*
    Check if password is valid
    Params: string password
    return:
        if password has not enough length, return NOT_ENOUGH_LEN_ERROR
        if password does not have lowercase char, return NO_LOWER_ERROR
        if password does not have uppercase char, return NO_UPPER_ERROR
        if password does not have number, return NO_NUMBER_ERROR
        if password does not have special character included "!.@$%&", return NO_SPECIAL_ERROR
        otherwise, return NO_ERROR
*/
VALIDATOR_ERROR_TYPE Validator::isPasswordValid(string password) {
    if (password.empty() || password.size() < MIN_PASSWORD_LEN) return VALIDATOR_ERROR_TYPE::NOT_ENOUGH_LEN_ERROR;

    if (!isLower(password)) return VALIDATOR_ERROR_TYPE::NO_LOWER_ERROR;
    if (!isUpper(password)) return VALIDATOR_ERROR_TYPE::NO_UPPER_ERROR;
    if (!isNumber(password)) return VALIDATOR_ERROR_TYPE::NO_NUMBER_ERROR;
    if (!isSpecial(password)) return VALIDATOR_ERROR_TYPE::NO_SPECIAL_ERROR;
    if (isComma(password)) return VALIDATOR_ERROR_TYPE::NOT_VALID_STRING;

    return VALIDATOR_ERROR_TYPE::NO_ERROR;
}

/*
    Check if string is valid
    Params: string line
    return:
        if line is empty, return NOT_VALID_STRING
        if line has comma, return NOT_VALID_STRING
        if line has special character included "!.@$%&", return NOT_VALID_STRING
        otherwise, return NO_ERROR
*/
VALIDATOR_ERROR_TYPE Validator::isStringValid(string line) {
    std::regex onlyNumberAndEnglish("^[a-zA-Z0-9]+$");
    if (line.empty() || this->isComma(line) || !std::regex_match(line.c_str(), onlyNumberAndEnglish)) return VALIDATOR_ERROR_TYPE::NOT_VALID_STRING;
    return VALIDATOR_ERROR_TYPE::NO_ERROR;

}

/*
    Check if birth date string is valid
    Params: string line
    return:
        if line is empty, return NOT_VALID_STRING
        if line has comma, return NOT_VALID_STRING
        if line has special character included "!.@$%&", return NOT_VALID_STRING
        otherwise, return NO_ERROR
*/
VALIDATOR_ERROR_TYPE Validator::isBirthDateStringValid(string line) {
    std::regex onlyNumberAndSpace("^[0-9 ]+$");
    if (line.empty() || this->isComma(line) || !std::regex_match(line.c_str(), onlyNumberAndSpace)) return VALIDATOR_ERROR_TYPE::NOT_VALID_STRING;
    return VALIDATOR_ERROR_TYPE::NO_ERROR;
}
