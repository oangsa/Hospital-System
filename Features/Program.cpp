#include "Program.h"
#include "../Libs/Map.h"
#include "FileManager.h"

using namespace std;

Program::Program(u_int32 size): userMap(size), userManager(userMap), panel(userManager, userMap) {}

u_int8 Program::Init() {
    FileManager fileManager;
    u_int8 isFileExist = fileManager.checkAllFilesAndCreate();
    this->userManager.loadUsersFromFile("Database/Users/users.csv");
    this->userManager.loadPatientQueue("Database/Temp/_PaQ.csv");
    this->userManager.loadPatientPriorityQueue("Database/Temp/PaPQ.csv");

    this->panel.loginPanel(3, isFileExist);

    return 1;
}

void Program::Test() {
    FileManager fileManager;
    fileManager.checkAllFilesAndCreate();
    this->userManager.loadUsersFromFile("Database/Users/users.csv");

    user_t newUser = {
        .id = 250400007,
        .name = "Test User",
        .birthDate = {
            ._day = 25,
            ._month = 4,
            ._year = 2000
        },
        .gender = Gender::MALE,
        .userType = UserType::NURSE,
        .username = "testuser",
        .password = "testpass",
    };
    User newUserObj(newUser);

    this->userManager.addUser(newUserObj);
    this->userManager.saveToFile("Database/Users/users.csv");
}
