#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "../Libs/Map.h"
#include "../Libs/Set.h"
#include "../Libs/UserPriorityQueue.h"
#include "../Libs/UserQueue.h"
#include "Patient.h"
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
    // Variables
    Set uniqueIds;
    UserQueue userIdQueue;
    UserPriorityQueue UserIdPQ;

    UserManager(Map<User> &userMap);

    void addUser(User &user);
    void removeUser(u_int64 id);
    u_int16 updateUser(user_t user);
    vector<User> getAllUsers();
    User* find(u_int64 id);

    // Loader
    void loadUsersFromFile(const string &filename);
    void saveToFile(const string &filename);
    void loadHistoryFromFile();
    void loadPatientQueue(const string &filename);
    void loadPatientPriorityQueue(const string &filename);
    User* loadLoggedUser();

    u_int64 generateID(UserType type);
    u_int32 getCounter(UserType type);

    u_int64 getHashNumber(User &user);

    u_int64 login(string &username, string &password);
    void logout();
    u_int8 registerUser(User &user);

    u_int16 userEnqueue(user_t user);
    u_int16 nurseEnqueuePatient(Patient user);

    void writeHistory(Patient p);
};

#endif
