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

typedef struct _PatientHistory {
  time_t _timestamp;
  string _diagnosis;
  string _treatment;
  string _prescription;
} PatientHistory;

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

#endif
