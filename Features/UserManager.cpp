#include "UserManager.h"
#include "../Libs/Define.h"
#include "../Libs/Map.h"
#include "User.h"
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

/*
    Helper Function Use To Trim unused string
    Params: string& str
    return string
*/
string trim(string &str) {
    u_int8 first = str.find_first_not_of(" \t\n\r\f\v");
    u_int8 last = str.find_last_not_of(" \t\n\r\f\v");

    return str.substr(first, (last - first + 1));
}

/*
    Constructor
    Params: Map<User>& userMap
*/
UserManager::UserManager(Map<User> &userMap) : userMap(userMap) {
    _counter.opd = 0;
    _counter.ipd = 0;
    _counter.doctor = 0;
    _counter.nurse = 0;
    _counter.admin = 0;
}

/*
    load users from file
    Params: string& filename
*/
void UserManager::loadUsersFromFile(const string &filename) {
    ifstream file;
    string line;
    string word;
    user_t user;
    u_int8 i;

    file.open(filename);

    // Get rid of the first line
    getline(file, line);

    while (file.good()) {
        // id,name,day,month,year,gender,userType,username,password
        getline(file, line);

        i = 0;

        while (!line.empty()) {
            u_int16 pos = line.find(",");

            word = line.substr(0, pos);

            switch (i) {
            case 0:
                user.id = (u_int64)atoll(word.c_str());
                break;
            case 1:
                user.name = word;
                break;
            case 2:
                user.birthDate._day = (u_int16)atoi(word.c_str());
                break;
            case 3:
                user.birthDate._month = (u_int16)atoi(word.c_str());
                break;
            case 4:
                user.birthDate._year = (u_int16)atoi(word.c_str());
                break;
            case 5:
                user.gender = (Gender)(word == "Male" ? Gender::MALE : Gender::FEMALE);
                break;
            case 6:
                user.userType = (UserType)(word == "OPD"      ? UserType::OPD
                                        : word == "DOCTOR" ? UserType::DOCTOR
                                        : word == "IPD"    ? UserType::IPD
                                        : word == "NURSE"  ? UserType::NURSE
                                                            : UserType::ADMIN);
                break;
            case 7:
                user.username = word;
                break;
            }

            line = line.substr(pos + 1);

            if (line.find(",") == string::npos) {
                line = trim(line);
                user.password = line;
                break;
            }
            i++;
        }

        User newUser(user);
        this->addUser(newUser);
    }

    file.close();
}

/*
    Add user to the user map
    Params: User& user
*/
void UserManager::addUser(User &user) {
    // Generate ID if the user doesn't have one
    if (user.getID() == 0) {
        user.setID(UserManager::generateID(user.getType()));
    }

    userMap.put(user.getID(), user);
}

/*
    Remove user in the user map
    Params: User& user
*/
void UserManager::removeUser(u_int64 id) {
    userMap.remove(id);
}

/*
    Load history from file
*/
void UserManager::loadHistoryFromFile() {
    // Load history from file

    // Person who have to do anything with this function please implement this
    // function (I think It's Wit's responsibility.)

    cout << "This method is not implemented yet." << "\n";
}

/*
    Get all Users
    return: vector<User>
*/
vector<User> UserManager::getAllUsers() {
    // Implement this method later;
    vector<User> users;
    cout << "This method is not implemented yet." << "\n";
    return users;
}

/*
    Generate ID for the user
    Params: UserType type
    return: long ID
*/
u_int64 UserManager::generateID(UserType type) {
    // YY0??____
    // ?? 10 >> OPD
    // ?? 20 >> IPD
    // ?? 30 >> DOCTOR
    // ?? 40 >> NURSE
    // ?? 99 >> ADMIN

    // Suppose the counter is 0
    // And the year is 2025
    // User type is Admin
    // The ID should be: 250990000

    int id = 0;

    switch (type) {
    case UserType::OPD:
        id = 10;
        break;
    case UserType::IPD:
        id = 20;
        break;
    case UserType::DOCTOR:
        id = 30;
        break;
    case UserType::NURSE:
        id = 40;
        break;
    case UserType::ADMIN:
        id = 99;
        break;
    }

    time_t now = time(0);
    tm *localTime = localtime(&now);

    int year = localTime->tm_year + 1900;
    int counter = getCounter(type);

    year = year % 100;
    year *= 1000;
    year += id;
    year *= 10000;
    year += counter;

    return year;
}

/*
    Get the counter of the user with the given type
    Params: UserType type
    return: int counter
*/
u_int32 UserManager::getCounter(UserType type) {
    switch (type) {
        case UserType::OPD:
            return _counter.opd++;

        case UserType::IPD:
            return _counter.ipd++;

        case UserType::DOCTOR:
            return _counter.doctor++;

        case UserType::NURSE:
            return _counter.nurse++;

        case UserType::ADMIN:
            return _counter.admin++;

        default:
            // It should not reach here
            // If it does, please fix;
            throw runtime_error("Invalid user type");
            return 0;
    }
}

/*
    Get the hash key of the user
    Params: User& user
    return: int hash key
*/
u_int64 UserManager::getHashNumber(User &user) {
    return user.getID() % UserManager::userMap.size();
}

/*
    Login the user with the given username and password
    Params: string& username, string& password
    return: 1 if success, 0 if failed
*/
u_int64 UserManager::login(string &username, string &password) {

    for (u_int32 i = 0; i < this->userMap.size(); i++) {
        Node<User> *userList = this->userMap.getList(i);

        while (userList != nullptr) {
        string un = userList->data()->getUsername();
        string pd = userList->data()->getPassword();

        if (username == un && password == pd) {
            return userList->data()->getID();
        }
        userList = userList->next();
        }
    }

    return 0;
}

/*
    Register the user
    Params: User& user
    return: 1 if success, 0 if failed
*/
u_int8 UserManager::registerUser(User &user) {
  // Prevent duplicate username
  for (u_int32 i = 0; i < this->userMap.size(); i++) {
            Node<User> *userList = this->userMap.getList(i);

            while (userList != NULL) {
            if (userList->data()->getUsername() == user.getUsername()) {
                // Decrease the counter of the user type
                switch (user.getType()) {
                    case UserType::OPD:
                        if (user.getID()) this->_counter.opd--;
                        this->userMap.remove(user.getID());
                        break;
                    case UserType::IPD:
                        if (user.getID()) this->_counter.ipd--;
                        this->userMap.remove(user.getID());
                        break;
                    case UserType::DOCTOR:
                        if (user.getID()) this->_counter.doctor--;
                        this->userMap.remove(user.getID());
                        break;
                    case UserType::NURSE:
                        if (user.getID()) this->_counter.nurse--;
                        this->userMap.remove(user.getID());
                        break;
                    case UserType::ADMIN:
                        if (user.getID()) this->_counter.admin--;
                        this->userMap.remove(user.getID());
                        break;
                }
                return 0;
            }
            userList = userList->next();
        }
  }

    // Username is unique, so we can register the user
    this->addUser(user);

    return 1;
}

/*
    Find user by given id
    Params: u_int64 id
    return nullptr if user is not found, else User*
*/
User *UserManager::find(u_int64 id) {
    return this->userMap.get(id);
}

/*
    Save users data to file
    **This Function is not yet finished**
    Params: const string& filename
*/
void UserManager::saveToFile(const string &filename) {
    ofstream file;
    string header = "id,name,day,month,year,gender,userType,username,password";
    string cFilename = "Database/Users/_users.csv";

    file.open(cFilename);

    file << header << "\n";

    for (u_int32 i = 0; i < this->userMap.size() - 1; i++) {
        Node<User> *current = this->userMap.getList(i);

        if (!current) continue;

        while (current != NULL) {
            string userData = current->data()->getWriteFileData();

            file << userData << "\n";

            current = current->next();
        }
    }

    file.close();
}
