#include "Features/FileManager.h"
#include "Features/User.h"
#include "Features/UserManager.h"
#include "Libs/Map.h"
#include <iostream>
#include <string>

int main() {
    Map<User> userMap(20);
    FileManager fileManager;
    UserManager userManager(userMap);

    u_int64 userId;

    std::string fileName = "Main";
    std::string username = "taylormiller0";
    std::string password = "pass8374";
    std::cout << "File Manager Initialized" << std::endl;

    userManager.loadUsersFromFile("Database/Users/users.csv");

    userId = userManager.login(username, password);

    cout << userId << "\n";

    userManager.find(userId)->displayInfo();

    userManager.removeUser(userId);
    
    userManager.saveToFile("");

    return 0;
}
