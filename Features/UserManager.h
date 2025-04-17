#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "../Libs/Map.h"
#include "User.h"
#include <vector>

typedef struct counter {
    u_int32 opd;
    u_int32 ipd;
    u_int32 doctor;
    u_int32 nurse;
    u_int32 admin;
} Counter;

using namespace std;

class UserManager {
private:
    Map<User> &userMap;
    Counter _counter;

public:
    UserManager(Map<User> &userMap);

    void addUser(User &user);
    void removeUser(u_int64 id);
    void updateUser(User &user);
    vector<User> getAllUsers();
    User* find(u_int64 id);

    // Loader
    void loadUsersFromFile(const string &filename);
    void saveToFile(const string &filename);
    void loadHistoryFromFile();

    u_int64 generateID(UserType type);
    u_int32 getCounter(UserType type);

    u_int64 getHashNumber(User &user);

    u_int64 login(string &username, string &password);
    u_int8 logout(User &user);
    u_int8 registerUser(User &user);
};

#endif
