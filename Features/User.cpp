/**************************************************
 *                                                *
 *       User Class Implementation File           *
 *       Latest Update: May 5, 2025               *
 *                                                *
 **************************************************/


#include "User.h"
#include "FileManager.h"
#include "../Libs/Tree.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>

using namespace std;

/*
    Constructor
    Params: name, birthdate, gender, type, username, password
*/
User::User(user_t user): _name(user.name), _birthdate(user.birthDate), _gender(user.gender), _type(user.userType), _username(user.username), _password(user.password), _history(nullptr, 0) {
    this->_id = user.id;
}

/*
    Display user information
*/
void User::displayInfo() {
    cout << "ID: " << this->_id << "\n";
    cout << "Name: " << this->_name << "\n";
    cout << "Birthdate: " << this->_birthdate._year << "-" << this->_birthdate._month << "-" << this->_birthdate._day << "\n";
    cout << "Gender: " << (this->_gender == Gender::MALE ? "Male" : "Female") << "\n";
    cout << "Type: " << (UserType::ADMIN == this->_type ? "Admin" : UserType::DOCTOR == this->_type ? "Doctor" : UserType::NURSE == this->_type ? "Nurse" : "Patient") << "\n";
    cout << "Username: " << this->_username << "\n";
}

/*
    Display user history
*/
void User::displayHistory() {
    this->_history.inOrder([](PatientHistory& history) {
        time_t time = history._timestamp;
        struct tm* t = localtime(&time);
        cout << "========================================\n\n";
        cout << "Time: " << (t->tm_year + 1900) << "-" << t->tm_mon << "-" << t->tm_mday << " " << t->tm_hour << ":" << t->tm_min << "\n";
        cout << "Diagnosis: " << history._diagnosis << "\n";
        cout << "Treatment: " << history._treatment << "\n";
        cout << "Prescription: " << history._prescription << "\n";
        cout << "\n========================================\n";
    });
}

/*
    Add user history
    Params: diagnosis, treatment, prescription
*/
void User::addHistory(PatientHistory history) {
    PatientHistory* newHistory = new PatientHistory(history);
    this->_history.insert(newHistory->_timestamp, newHistory);
}

// Getter
/*
    Get user gender
    return gender as Gender
*/
Gender User::getGender() {
    return this->_gender;
}

/*
    Get user gender as a string
    return string gender
*/
string User::getGenderString() {
    return this->_gender == Gender::MALE ? "Male" : "Female";
}

/*
    Get user type as a string
    return string userType
*/
string User::getTypeString() {
    return this->_type == UserType::ADMIN ? "ADMIN" : this->_type == UserType::DOCTOR ? "DOCTOR" : this->_type == UserType::IPD ? "IPD" : this->_type == UserType::NURSE ? "NURSE" : "OPD";
}

/*
    Get user age as an integer
    return int age
*/
u_int16 User::getAge() {
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
    return: PatientHistory
*/
PatientHistory User::getHistory(time_t timestamp) {
    PatientHistory* history = this->_history.find(timestamp);

    if (!history) throw "History not found";

    return *history;
}

/*
    Get user name
    Params: none
    return: string name
*/
string User::getName() {
    return this->_name;
}

/*
    Get user ID
    Params: string id
*/
void User::setID(u_int64 id) {
    this->_id = id;
}

/*
    Get user ID
    Params: none
    return: u_int64 id
*/
u_int64 User::getID() {
    return this->_id;
}

/*
    Get username
    Params: none
    return: string username
*/
string User::getUsername() {
    return this->_username;
}

/*
    Get password
    Params: none
    return: string password
*/
string User::getPassword() {
    return this->_password;
}

/*
    Get user data
    Params: none
    return: user_t user
    NOTE: This function is used to get the user data in a struct form
*/
user_t User::getUser_t() {
    user_t user = {
        .id = this->_id,
        .name = this->_name,
        .birthDate = this->_birthdate,
        .gender = this->_gender,
        .userType = this->_type,
        .username = this->_username,
        .password = this->_password,
    };

    return user;
};

/*
    Get data in this form
    // id,name,day,month,year,gender,userType,username,password
    return string
*/
string User::getWriteFileData() {
    string data;

    // Hard code god
    data.append(to_string(this->_id));
    data.append(",");
    data.append(this->_name);
    data.append(",");
    data.append(to_string(this->_birthdate._day));
    data.append(",");
    data.append(to_string(this->_birthdate._month));
    data.append(",");
    data.append(to_string(this->_birthdate._year));
    data.append(",");
    data.append(this->getGenderString());
    data.append(",");
    data.append(this->getTypeString());
    data.append(",");
    data.append(this->_username);
    data.append(",");
    data.append(this->_password);

    return data;
}
/*
    Get user birthdate
    return: BirthDate
*/
BirthDate User::getBirthDate() {
    return this->_birthdate;
}

/*
    Load user history from file
    The file is in the format of "Database/History/<user_id>.csv"
*/
void User::loadHistory() {
    FileManager fileManager;
    string base = "Database/History/";
    base += to_string(this->_id) + ".csv";

    if (!fileManager.isFileExists(base)) {
        return;
    }

    ifstream infile(base);
    string line;

    getline(infile, line);

    while (getline(infile, line)) {
        stringstream ss(line);
        string token;
        PatientHistory record;

        getline(ss, token, ',');
        record._timestamp = static_cast<time_t>(stoll(token));

        getline(ss, token, ',');
        record._diagnosis = token;

        getline(ss, token, ',');
        record._treatment = token;

        getline(ss, token, ',');
        record._prescription = token;

        this->addHistory(record);

    }

    infile.close();
}
