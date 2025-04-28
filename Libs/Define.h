#ifndef DEFINE_H
#define DEFINE_H

#include <ctime>
#include <string>

using namespace std;

/*
    Define the data types
    u = unsigned
    int = integer
    f = float

    8, 16, 32, 64 = number of bits
    // Higher bits mean it can store larger numbers

    Example:
        u_int32 = unsigned integer with 32 bits
*/
typedef unsigned long long int u_int64;
typedef unsigned int u_int32;
typedef unsigned short int u_int16;
typedef unsigned char u_int8;

typedef short int int16;
typedef int int32;
typedef long long int int64;

typedef float f32;
typedef double f64;

typedef enum { MALE, FEMALE } Gender;

typedef enum {
    ADMIN,
    OPD, // ผู้ป่วยนอก
    IPD, // ผู้ป่วยใน
    DOCTOR,
    NURSE
} UserType;

/*
    ESI = Emergency Severity Index
    ESI_1 = Dying
    ESI_2 = Shouldn't wait
    ESI_3 = Need multiple resources
    ESI_4 = Need single resource
    ESI_5 = Don't need resources
*/
typedef enum {
    ESI_1, // DYING 0
    ESI_2, // SHOULD'NT WAIT 1
    ESI_3, // NEED MULTIPLE RESOURCES 2
    ESI_4, // NEED SINGLE RESOURCE 3
    ESI_5, // DONT NEED RESOURCES 4
} ESI_LEVEL;

typedef enum {
    EMPTY_ACTION,
    USER_DELETE,
    USER_UPDATE,
    USER_ADD,

    // MIGHT BE IMPLEMENTED LATER
    HISTORY_DELETE,
} ACTION_TYPE;

typedef enum {
    // Birthday
    DAY_ERROR,
    MONTH_ERROR,
    YEAR_ERROR,
    NEGATIVE_NUMBER_ERROR,

    // Pasword
    NO_SPECIAL_ERROR,
    NOT_ENOUGH_LEN_ERROR,
    NO_UPPER_ERROR,
    NO_LOWER_ERROR,
    NO_NUMBER_ERROR,

    // String
    NOT_VALID_STRING,

    NO_ERROR,
} VALIDATOR_ERROR_TYPE;


// 1713158400,Hypertension,Lifestyle modification,Lisinopril 10mg daily

typedef struct _PatientHistory {
    time_t _timestamp; // 1713158400
    string _diagnosis; // Hypertension
    string _treatment;
    string _prescription;
} PatientHistory;

typedef struct _NurseDiagnosis {
    u_int16 _bloodPressure; // mmHg
    u_int16 _oxygenSaturation; // %
    u_int16 _heartRate; // bpm
    u_int8 _temperature; // Celsius
    u_int8 _isResponsive; // 0 = No, 1 = Yes
} NurseDiagnosis;

typedef struct _BirthDate {
    u_int16 _day;
    u_int16 _month;
    u_int16 _year;
} BirthDate;

typedef struct _User {
    u_int64 id;
    string name;
    BirthDate birthDate;
    Gender gender;
    UserType userType;

    // User credentials
    string username;
    string password;
} user_t;

typedef struct _UndoStruct {
    ACTION_TYPE actionType;
    user_t user;
} undo_t;

#endif
