#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>
#include <ctime>
#include <vector>
#include "../Libs/Define.h"
#include "../Libs/Tree.h"

using namespace std;

class User {
    private:
        // User information
        u_int64 _id;
        string _name;
        BirthDate _birthdate;
        Gender _gender;
        UserType _type;

        // User credentials
        string _username;
        string _password;

        // User history
        Tree<PatientHistory> _history;

    public:
        User(user_t user);
        void displayInfo();
        void displayHistory();

        // Getter
        string getName();
        u_int16 getAge();
        Gender getGender();
        UserType getType();
        string getTypeString();
        PatientHistory getHistory(time_t timestamp);
        string getGenderString();
        u_int64 getID();
        string getUsername();
        string getPassword();
        string getWriteFileData();

        void addHistory(PatientHistory history);
        void setID(u_int64 id);

        ESI_LEVEL diagESI(NurseDiagnosis& diagnosis);
};

#endif
