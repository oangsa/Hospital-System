#include "Features/FileManager.h"
#include "Features/User.h"
#include "Features/UserManager.h"
#include "Libs/Map.h"
#include <iostream>
#include <string>
#include <fstream>
#include "Libs/PriorityQueue.h"
#include <sys/stat.h>
#include "Libs/Queue.h"

void que(User& user){
    Queue<User> que;

    que.enqueue(user);


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
    que(*user);
    return 0;
}