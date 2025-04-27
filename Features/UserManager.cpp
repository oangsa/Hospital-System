#include "UserManager.h"
#include "FileManager.h"
#include "../Libs/Define.h"
#include "../Libs/Map.h"
#include "User.h"
#include <cctype>
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

        u_int32 ctr = user.id % 1000;

        // Set counter
        switch (user.userType) {
            case UserType::OPD:
                this->_counter.opd = ((ctr+1) >= this->_counter.opd) ? (++ctr) : this->_counter.opd;
                break;
            case UserType::IPD:
                this->_counter.ipd = ((ctr+1) >= this->_counter.ipd) ? ++ctr : this->_counter.ipd;
                break;
            case UserType::DOCTOR:
                this->_counter.doctor = ((ctr+1) >= this->_counter.doctor) ? ++ctr : this->_counter.doctor;
                break;
            case UserType::NURSE:
                this->_counter.nurse = ((ctr+1) >= this->_counter.nurse) ? ++ctr : this->_counter.nurse;
                break;
            case UserType::ADMIN:
                this->_counter.admin = ((ctr+1) >= this->_counter.admin) ? ++ctr : this->_counter.admin;
                break;
        }

        User newUser(user);
        // Load their history before add them
        newUser.loadHistory();
        this->addUser(newUser);
    }

    file.close();
}

/*
    Add user to the user map
    NOTE: DON'T USE IN THE CASE OF ADDING NEW USER, UNLESS IT FROM THE FILE
    Params: User& user
*/
void UserManager::addUser(User &user) {
    // Generate ID if the user doesn't have one
    if (user.getID() == 0) {
        user.setID(UserManager::generateID(user.getType()));
    }
    // Generate ID if the user doesn't have one
    if (user.getID() == 0) {
        user.setID(UserManager::generateID(user.getType()));
    }

    userMap.put(user.getID(), user);
}

/*
    Update user in the user map
    Params: User& user
    return 1 if success, 0 if failed
*/
u_int16 UserManager::updateUser(user_t user) {
    if (userMap.get(user.id) == NULL) {
        return 0;
    }

    // Remove old user data and add the new one with the new data but has the same id
    userMap.remove(user.id);
    userMap.put(user.id, user);

    return 1;
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

        // Username is unique, so we can register the user
        this->addUser(user);

        return 1;
    }

    return 0;
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

    // Rename the file
    remove(filename.c_str());
    rename(cFilename.c_str(), filename.c_str());
}

/*
    Load logged user from file

    NOTE: Use this function if and only if cache file is exists and there is a user id in it

    Params: None
    return: User& user
*/
User* UserManager::loadLoggedUser() {
    fstream file;
    string line;

    //Inside the file will have only one line contain the user id
    file.open("Database/Cache/user.csv", ios::in);
    file >> line;

    User* user = this->find(stoull(line));

    return user;
}

void UserManager::logout() {
    FILE* f;
    f = fopen("Database/Cache/user.csv", "w");
    fclose(f);
}

void UserManager::loadPatientQueue(const string &filename) {
    FileManager fileManager;
    string fName = filename;

    if (!fileManager.isFileExists(fName)) {
        return;
    }

    ifstream infile(fName);
    string line;

    while (getline(infile, line)) {
        u_int64 userId = stoull(line);

        if (!this->uniqueIds.contains(userId, UserType::OPD)) {
            this->uniqueIds.add(userId, UserType::OPD);
            this->userIdQueue.enqueue(userId);
        }
    }
    infile.close();
}

void UserManager::loadPatientPriorityQueue(const string &filename) {
    FileManager fileManager;
    string fName = filename;

    if (!fileManager.isFileExists(fName)) {
        return;
    }

    ifstream infile(fName);
    string line;

    while (getline(infile, line)) {
        vector<string> vs;
        stringstream ss(line);

        while (ss.good()) {
            string substr;
            getline(ss, substr, ',');
            vs.push_back(substr);
        }

        u_int64 userId = stoull(vs[0]);
        u_int32 priority = stoul(vs[1]);

        this->UserIdPQ.enqueue(userId, priority);
    }
    infile.close();
}

u_int16 UserManager::userEnqueue(user_t user) {
    if (this->uniqueIds.contains(user.id, user.userType)) return 0;

    ofstream outFile("Database/Temp/_PaQ.csv", std::ios::app);

    if (!outFile.is_open()) {
        return -1;
    }

    outFile << user.id << "\n";
    outFile.close();
    this->userIdQueue.enqueue(user.id);
    this->uniqueIds.add(user.id, user.userType);

    return 1;
}

u_int16 UserManager::nurseEnqueuePatient(Patient user) {
    ofstream file ("Database/Temp/PaPQ.csv", std::ios::app | std::ios::out | std::ios::in);

    if (!file.is_open()) {
        return -1;
    }

    file << user.getID() << "," << user.getESI() << "\n";
    file.close();
    this->UserIdPQ.enqueue(user.getID(), user.getESI());

    return 1;
}

void UserManager::writeHistory(Patient p) {
    FileManager fileManager;
    string base = "Database/History/";
    base += to_string(p.getID()) + ".csv";

    if (!fileManager.isFileExists(base)) {
        ofstream file;
        file.open(base, std::ios::out);
        file << "time,diagnosis,treatment,prescription\n";
        file.close();
    }
    ofstream file;
    file.open(base, std::ios::app);

    PatientHistory h = p.getHistory();

    file << h._timestamp << "," << h._diagnosis << "," << h._treatment << "," << h._prescription << "\n";
    file.close();
}
