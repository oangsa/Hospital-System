#include "Panel.h"
#include "Define.h"
#include <string>
#include <iostream>
#include <thread>

using namespace std;

/*
    Contructor
    Parameters:
        UserManager& userManager: Reference to the UserManager object
        Map<User>& userMap: Reference to the Map object containing User objects
*/
Panel::Panel(UserManager& userManager, Map<User>& userMap) : userManager(userManager), userMap(userMap) {};

/*
    Handler function for clearing the screen
*/
void Panel::clearScreen() {
    system("clear");
}

/*
    Handler function for delaying the execution for a given number of seconds
    Parameters:
        u_int8 secs: Number of seconds to delay
*/
void Panel::delay(u_int8 secs) {
    for (u_int8 i = 0; i < secs; i++) {
        this_thread::sleep_for(1s);
    }
}

/*
    Login panel function propts the user for username and password
    if username and password are valid, it shows welcome message
    and show the panel base on the user role (UserType)
    parameters:
        u_int8 attempt: Number of attempts for login
*/
void Panel::loginPanel(u_int8 attempt) {
    string username;
    string password;
    u_int64 userId;

    for (u_int8 i = 0; i < attempt; i++) {
        this->clearScreen();
        cout << "========< Login Panel >========" << "\n";
        cout << "\n  Username: ";
        cin >> username;
        cout << "  Password: ";
        cin >> password;
        cout << "\n";

        userId = this->userManager.login(username, password);
        cout << "================================" << "\n";
        if (userId != 0) break;

        cout << "Login failed" << "\n";
        this->delay(2);
        this->clearScreen();
    }
    cout << "Login success, user id: " << userId << "\n";

    this->LoggedUser = this->userManager.find(userId);
    this->delay(2);
    this->clearScreen();

    cout << "Welcome to the system, " << this->LoggedUser->getUsername() << "\n";

    this->delay(2);
    this->clearScreen();

    switch(this->LoggedUser->getType()) {
        case UserType::OPD:
        case UserType::IPD:
            this->patientMenu();
            break;

        case UserType::DOCTOR:
            cout << "Welcome to the Doctor panel" << "\n";
            break;

        case UserType::NURSE:
            cout << "Welcome to the Nurse panel" << "\n";
            break;

        case UserType::ADMIN:
            cout << "Welcome to the Admin panel" << "\n";
            break;
    }
}

void Panel::patientMenu() {
    u_int8 choice;
    cout << "========< Patient Panel >========" << "\n";
    cout << "   1. " << "\n";
}
