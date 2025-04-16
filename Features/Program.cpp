#include "Program.h"
#include "../Libs/Map.h"
#include "FileManager.h"

using namespace std;

Program::Program(u_int32 size): userMap(size), userManager(userMap), panel(userManager, userMap) {}

u_int8 Program::Init() {
    FileManager fileManager;
    fileManager.checkAllFilesAndCreate();
    this->userManager.loadUsersFromFile("Database/Users/users.csv");

    this->panel.loginPanel(3);

    return 1;
}
