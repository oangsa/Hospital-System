#include "Features/FileManager.h"
#include "Features/User.h"
#include "Features/UserManager.h"
#include "Libs/Map.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "Libs/PriorityQueue.h"
#include <sys/stat.h>

void loadPatientHistory(User& user) {
    FileManager fileManager;
    string base = "Database/History/";
    base += to_string(user.getID()) + ".csv";

    if (!fileManager.isFileExists(base)) {
        cout << "No history found for user ID \n";
    }

    ifstream infile(base);
    string line;

    getline(infile, line);

    while (getline(infile, line)) {
        std::stringstream ss(line);
        std::string token;
        PatientHistory record;

        getline(ss, token, ',');
        record._timestamp = static_cast<time_t>(stoll(token));

        getline(ss, token, ',');
        record._diagnosis = token;

        getline(ss, token, ',');
        record._treatment = token;

        getline(ss, token, ',');
        record._prescription = token;

        user.addHistory(record);

    }

    infile.close();
}

int main(){
    Map<User> userMap(20);
    FileManager fileManager;
    UserManager userManager(userMap);
    u_int64 userId;
    std::string username = "taylormiller0";
    std::string password = "pass8374";
    userManager.loadUsersFromFile("Database/Users/users.csv");
    userId = userManager.login(username, password);
    User* user = userManager.find(userId);
    loadPatientHistory(*user);
    user->displayHistory();
    return 0;
}
