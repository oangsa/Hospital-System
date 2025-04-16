#include "Program.h"
#include "../Libs/Map.h"
#include "FileManager.h"

using namespace std;

Program::Program(u_int32 size): userMap(size), userManager(userMap), panel(userManager, userMap) {}

u_int8 Program::Init() {
    FileManager fileManager;
    fileManager.checkAllFilesAndCreate();
    this->userManager.loadUsersFromFile("Database/Users/users.csv");

    this->panel.loginPanel();

    // Login Example
    // string username = "taylormiller0";
    // string password = "pass8374";
    //
    // u_int64 userId = this->userManager.login(username, password);
    //
    // if (userId == 0) {
    //     cout << "Login failed" << "\n";
    // }
    // else {
    //     cout << "Login success, user id: " << userId << "\n";
    // }

    return 1;
}
