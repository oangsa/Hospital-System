/**************************************************
 *                                                *
 *       Program Class Implementation File        *
 *       Latest Update: May 5, 2025               *
 *                                                *
 **************************************************/


#include "Program.h"
#include "../Libs/Map.h"
#include "FileManager.h"

using namespace std;

/*
    Constructor: Initialize the Program object with userMap, userManager and panel
    Parameters:
        u_int32 size: Size of the userMap
*/
Program::Program(u_int32 size): userMap(size), userManager(userMap), panel(userManager, userMap) {}

/*
    Function to initialize the program
        It will check all files and create them if they don't exist
        It will load the users from the file and load the patient queue and priority queue
        It will show the main menu of the program
    Parameters: None
    Return: 1 if success, 0 if fail
*/
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

/*
    Function to test the program
    Parameters: None
*/
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
