#include "Features/FileManager.h"
#include "Features/User.h"
#include "Features/UserManager.h"
#include "Libs/Map.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sys/stat.h>

void history(u_int64 userId){
    FileManager fileManager;
    string base = "Database/History/";
    base.append(to_string(userId));
    base.append(".csv");

    cout <<(fileManager.isFileExists(base)? "Yes\n" : "No\n");
    std::ifstream infile(base);
    std::string line;

    std::getline(infile, line);
    
    while (std::getline(infile, line)) {
        std::cout << line << std::endl;
    }

    infile.close();
    }

int main(){
    Map<User> userMap(20);
    FileManager fileManager;
    UserManager userManager(userMap);
    cout << "bro";
    u_int64 userId;
    std::string username = "taylormiller0";
    std::string password = "pass8374";
    userManager.loadUsersFromFile("Database/Users/users.csv");
    userId = userManager.login(username, password);
    
    history(userId);
    
    return 0;
}
