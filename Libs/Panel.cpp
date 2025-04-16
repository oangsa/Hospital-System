#include "Panel.h"
#include <string>
#include <iostream>
#include <thread>

using namespace std;

Panel::Panel(UserManager& userManager, Map<User>& userMap) : userManager(userManager), userMap(userMap) {};

void Panel::clearScreen() {
    system("clear");
}

void Panel::delay(u_int8 secs) {
    for (u_int8 i = 0; i < secs; i++) {
        this_thread::sleep_for(1s);
    }
}

void Panel::loginPanel() {
    string username;
    string password;
    u_int64 userId;

    cout << "========< Login Panel >========" << "\n";
    cout << "\n  Username: ";
    cin >> username;
    cout << "  Password: ";
    cin >> password;
    cout << "\n";

    userId = this->userManager.login(username, password);
    
    cout << "================================" << "\n";

    if (userId == 0) {
        cout << "Login failed" << "\n";
    }
    else {
        cout << "Login success, user id: " << userId << "\n";

        this->LoggedUser = this->userManager.find(userId);
        this->delay(2);
        this->clearScreen();

        cout << "Welcome to the system, " << this->LoggedUser->getUsername() << "\n";

        this->delay(2);
        this->clearScreen();

        switch(this->LoggedUser->getType()) {
            case UserType::OPD:
                cout << "Welcome to the OPD panel" << "\n";
                break;

            case UserType::IPD:
                cout << "Welcome to the IPD panel" << "\n";
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
}
