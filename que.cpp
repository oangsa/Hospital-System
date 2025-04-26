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
#include "Libs/Set.h"

void  load_Que(){
    FileManager fileManager;
    string base = "Database/Temp/_PaQ.csv";
    Queue<u_int64> queue;
    Set uniqueIDs;
    if (!fileManager.isFileExists(base)) {
        cout << "No history found for user ID \n";
    }

    ifstream infile(base);
    string line;

    getline(infile, line);
    
        while (getline(infile, line)) {
        u_int64 userId = stoull(line);

        if (!uniqueIDs.contains(userId, UserType::OPD)) {
            uniqueIDs.add(userId, UserType::OPD);
            queue.enqueue(userId);
        }
    }

    infile.close();

    while (!queue.isEmpty()) {
        u_int64* id = queue.dequeue();
        cout << "Queued ID: " << *id << "\n";
        delete id; 
    }
}

void que(User& user,u_int64 userId){
    Queue<User> que;

    que.enqueue(user);

    std::ofstream outFile("Database/Temp/_PaQ.csv", std::ios::app);
    if (outFile.is_open()) {
        outFile << userId << std::endl; 
        outFile.close();
    } else {
        std::cerr << "Failed" << std::endl;
    }


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
    que(*user,userId);
    return 0;
}