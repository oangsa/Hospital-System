#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>
#include <ctime>
#include <vector>

using namespace std;

typedef enum {
    MALE,
    FEMALE
} Gender;

typedef enum {
    ADMIN,
    PATIENT,
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
    int _day;
    int _month;
    int _year;
} BirthDate;

class User {
    private:
        // User information
        string _name;
        BirthDate _birthdate;
        Gender _gender;
        UserType _type;

        // User credentials
        string _username;
        string _password;

        // User history
        vector<PatientHistory> _history;

    public:
        User(string name, BirthDate birthdate, Gender gender, UserType type, string username, string password);
        ~User();
        void displayInfo();
        void displayHistory();

        // Getter
        string getName();
        int getAge();
        Gender getGender();
        UserType getType();
        PatientHistory getHistory(time_t timestamp);
        string getGenderString();

        int getHashNumber();

        void addHistory(string& diagnosis, string& treatment, string& prescription);
};

#endif
