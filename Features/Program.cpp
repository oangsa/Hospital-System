#include "Program.h"
#include "../Libs/Map.h"
#include "FileManager.h"

using namespace std;

Program::Program(u_int32 size): userMap(size), userManager(userMap), panel(userManager, userMap) {}

u_int8 Program::Init() {
    FileManager fileManager;
    Logger logger;
    u_int8 isFileExist = fileManager.checkAllFilesAndCreate();
    logger.log("Check All Files and Create");
    this->userManager.loadUsersFromFile("Database/Users/users.csv");
    logger.log("Users Loaded");
    this->userManager.loadPatientQueue("Database/Temp/_PaQ.csv");
    logger.log("PatientQueue Loaded");
    this->userManager.loadPatientPriorityQueue("Database/Temp/PaPQ.csv");
    logger.log("PatientPriorityQueue Loaded");

    this->panel.mainMenu(isFileExist);

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
