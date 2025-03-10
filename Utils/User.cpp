#include "user.h"
#include <iostream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

/*
    Constructor
    Params: name, birthdate, gender, type, username, password
*/
User::User(string name, BirthDate birthdate, Gender gender, UserType type, string username, string password): _name(name), _birthdate(birthdate), _gender(gender), _type(type), _username(username), _password(password) {
    this->_history = vector<PatientHistory>();
}

/*
    Destructor
*/
User::~User() {}

/*
    Display user information
*/
void User::displayInfo() {
    cout << "Name: " << this->_name << "\n";
    cout << "Birthdate: " << this->_birthdate._year << "-" << this->_birthdate._month << "-" << this->_birthdate._day << "\n";
    cout << "Gender: " << (this->_gender == Gender::MALE ? "Male" : "Female") << "\n";
    cout << "Type: " << (this->_type == UserType::ADMIN ? "Admin" : "User") << "\n";
    cout << "Username: " << this->_username << "\n";
}

/*
    Display user history
*/
void User::displayHistory() {
    cout << "History:\n";
    for (const auto& history : this->_history) {
        cout << history._timestamp << "\n";
        cout << history._diagnosis << "\n";
        cout << history._treatment << "\n";
        cout << history._prescription << "\n";
    }
}

/*
    Add user history
*/
void User::addHistory(string& diagnosis, string& treatment, string& prescription) {
    PatientHistory history;
    history._timestamp = time(nullptr);
    history._diagnosis = diagnosis;
    history._treatment = treatment;
    history._prescription = prescription;

    this->_history.push_back(history);
}

// Getter
/*
    Get user gender
*/
Gender User::getGender() {
    return this->_gender;
}

/*
    Get user gender as a string
*/
string User::getGenderString() {
    return this->_gender == Gender::MALE ? "Male" : "Female";
}

/*
    Get user age as an integer
*/
int User::getAge() {
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    int currentYear = localTime->tm_year + 1900;
    int currentMonth = localTime->tm_mon + 1;
    int currentDay = localTime->tm_mday;

    int age = currentYear - this->_birthdate._year;
    if (currentMonth < this->_birthdate._month || (currentMonth == this->_birthdate._month && currentDay < this->_birthdate._day)) {
        age--;
    }

    return age;
}

/*
    Get user type
*/
UserType User::getType() {
    return this->_type;
}

/*
    Get user history
    Params: timestamp
*/
PatientHistory User::getHistory(time_t timestamp) {
    for (const auto& history : this->_history) {
        if (history._timestamp == timestamp) {
            return history;
        }
    }
    throw runtime_error("History not found");
}

/*
    Get user hash number by iterating over the name string
    Params: none
*/
int User::getHashNumber() {
    int num = 0;
    int len = this->_name.length();

    for (int i = 0; i < len; i++) {
        num += this->_name[i];
    }

    return num;
}

/*
    Get user name
    Params: none
*/
string User::getName() {
    return this->_name;
}
