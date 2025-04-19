#include "Panel.h"
#include "Define.h"
#include "../Features/FileManager.h"
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
Panel::Panel(UserManager& userManager, Map<User>& userMap) : userManager(userManager), userMap(userMap), undoManager(userManager) {};

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


void Panel::showUserInfo() {
    for (u_int32 i = 0; i < this->userMap.size(); i++) {
        Node<User> *current = this->userMap.getList(i);
        if (!current) continue;
        while (current != NULL) {
            cout << current->data()->getID() << "\t" << current->data()->getUsername() << "\t" << current->data()->getTypeString() << "\n";
            current = current->next();
        }
    }
}

/*
    Login panel function propts the user for username and password
    if username and password are valid, it shows welcome message
    and show the panel base on the user role (UserType)
    parameters:
        u_int8 attempt: Number of attempts for login
*/
void Panel::loginPanel(u_int8 attempt, u_int8 isFileExist) {
    string username;
    string password;
    u_int64 userId;
    FileManager fileManager;

    if (isFileExist) {
        this->LoggedUser = this->userManager.loadLoggedUser();
        if (this->LoggedUser != NULL) {
            this->clearScreen();
            cout << "Welcome back, " << this->LoggedUser->getUsername() << "\n";
            this->delay(2);
            this->clearScreen();
            goto SwitchPanel;
        }
    }

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
    fileManager.writeUserCache(userId);

    SwitchPanel:
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
                this->delay(1);
                this->clearScreen();
                this->adminMenu();
                break;
        }
}

void Panel::patientMenu() {
    u_int8 choice;
    cout << "========< Patient Panel >========" << "\n";
    cout << "   1. " << "\n";
}

void Panel::doctorMenu() {
    cout << "========< Doctor Panel >========" << "\n";
}

void Panel::nurseMenu() {
    cout << "========< Nurse Panel >========" << "\n";
}

void Panel::adminMenu() {
    u_int16 choice;

    while (1) {
        cout << "========< Admin Panel >========" << "\n\n";
        cout << "   1. Add user" << "\n";
        cout << "   2. Remove user" << "\n";
        cout << "   3. Exit (without Logout)" << "\n";
        if (this->undoManager.peekAction().actionType != ACTION_TYPE::EMPTY_ACTION) cout << "   4. Revert Last Change \n";
        cout << "   0. Logout" << "\n\n";
        cout << "================================" << "\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 4:
                this->clearScreen();
                cout << "Reverting last change..." << "\n";
                this->undoManager.undoAction();
                this->delay(2);
                this->clearScreen();
                break;
            case 3:
                this->clearScreen();
                cout << "Exiting..." << "\n";
                this->userManager.saveToFile("Database/Users/users.csv");
                this->delay(2);
                this->clearScreen();
                return;
            case 0:
                this->clearScreen();
                cout << "Logging out..." << "\n";
                this->userManager.saveToFile("Database/Users/users.csv");
                this->userManager.logout();
                this->delay(2);
                this->clearScreen();
                return;
            case 1:
                this->clearScreen();
                this->addUserPanel();
                break;
            case 2:
                this->clearScreen();
                this->removeUserPanel();
                break;
            default:
                this->clearScreen();
                cout << "Invalid choice" << "\n";
                this->delay(1);
                this->clearScreen();
                break;
        }
    }
}

void Panel::addUserPanel() {
    cout << "========< Add User >========" << "\n";
    return;
}

void Panel::removeUserPanel() {
    string id;
    undo_t change;
    this->showUserInfo();
    cout << "========< Remove User >========" << "\n";
    cout << "Enter the user ID (press e to exit): ";
    cin >> id;

    if (id == "e") {
        this->clearScreen();
        return;
    }

    User* user = this->userManager.find(stoull(id));

    if (user == NULL) {
        cout << "User not found" << "\n";
        this->delay(2);
        this->clearScreen();
        return;

    }

    change.actionType = ACTION_TYPE::USER_DELETE;
    change.user = user->getUser_t();

    this->undoManager.pushAction(&change);

    this->userManager.removeUser(user->getID());
    cout << "User removed successfully" << "\n";
    this->delay(2);
    this->clearScreen();
}
