#include "Panel.h"
#include "../Features/FileManager.h"
#include "Define.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

using namespace std;

/*
    Contructor
    Parameters:
        UserManager& userManager: Reference to the UserManager object
        Map<User>& userMap: Reference to the Map object containing User objects
*/
Panel::Panel(UserManager &userManager, Map<User> &userMap): userManager(userManager), userMap(userMap), undoManager(userManager) {};

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

            goto SwitchPanel; // Skipped Loop if user exits
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
    switch (this->LoggedUser->getType()) {
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
  cout << "========< Patient Panel >========" << "\n";
}

void Panel::doctorMenu() { cout << "========< Doctor Panel >========" << "\n"; }

void Panel::nurseMenu() { cout << "========< Nurse Panel >========" << "\n"; }

void Panel::adminMenu() {
    char choice;

    while (1) {
        cout << "========< Admin Panel >========" << "\n\n";
        cout << "   1. Add user" << "\n";
        cout << "   2. Remove user" << "\n";
        cout << "   L. Logout" << "\n";
        if (this->undoManager.peekAction().actionType != ACTION_TYPE::EMPTY_ACTION) cout << "   R. Revert Last Change \n";
        cout << "   E. Exit (without Logout)" << "\n\n";
        cout << "================================" << "\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 'r':
            case 'R':
                this->clearScreen();
                cout << "Reverting last change..." << "\n";
                this->undoManager.undoAction();
                this->delay(2);
                this->clearScreen();
                break;

            case 'e':
            case 'E':
                this->clearScreen();
                cout << "Exiting..." << "\n";
                this->userManager.saveToFile("Database/Users/users.csv");
                this->delay(2);
                this->clearScreen();
                return;

            case 'l':
            case 'L':
                this->clearScreen();
                cout << "Logging out..." << "\n";
                this->userManager.saveToFile("Database/Users/users.csv");
                this->userManager.logout();
                this->delay(2);
                this->clearScreen();
                this->loginPanel(3, 0);
                break;

            case '1':
                this->clearScreen();
                this->addUserPanel();
                break;

            case '2':
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
    user_t newUser;
    string data;
    string word = "";
    vector<string> vs;
    undo_t undo;
    cout << "========< Add User >========" << "\n";

    cin.ignore();

    cout << "Name (title - name - surname): ";
    getline(cin, data);

    newUser.name = data;

    cout << "Birthdate (dd mm yyy) ex: 05 07 2015: ";
    getline(cin, data);
    
    // NOTE: SPLIT USER INPUT BY 'SPACE';

    for (char charc : data) {
        if (charc == ' ') {
            vs.push_back(word);
            word.clear();
            word = "";
        }
        word += charc;
    }

    vs.push_back(word);
    

    newUser.birthDate._day = atoi(vs[0].c_str());
    newUser.birthDate._month = atoi(vs[1].c_str());
    newUser.birthDate._year = atoi(vs[2].c_str());
    
    this->clearScreen();

    while (1) {
        cout << "Gender: \n";
        cout << "1. Male\n2. Female\n";
        cout << ">> ";
        cin >> data;
        if (!(data != "1" && data != "2")) break;

        cout << "Invalid Input.\n";
    }
    
    newUser.gender = (data == "1") ? Gender::MALE : Gender::FEMALE;
    this->clearScreen();
    
    while (1) {
        cout << "User Type: \n";
        cout << "1. IPD\n2. OPD\n3. NURSE\n4. DOCTOR\n5. ADMIN\n";
        cout << ">> ";
        cin >> data;

        if (!(data != "1" && data != "2" && data != "3" && data != "4" && data != "5")) break;
    }

    newUser.userType = (data == "1") ? UserType::IPD : (data == "2") ? UserType::OPD : (data == "3") ? UserType::NURSE : (data == "4") ? UserType::DOCTOR : UserType::ADMIN;
    
    this->clearScreen();

    GetUsername:
    cout << "Username: ";
    cin >> newUser.username;

    cout << "Password: ";
    cin >> newUser.password;
    cin.ignore();

    User n(newUser);

    n.setID(this->userManager.generateID(newUser.userType));
    newUser.id = n.getID();
    u_int8 isSuccess = this->userManager.registerUser(n);
    this->clearScreen();
    this->delay(1);

    if (!isSuccess) {
        cout << "Username is already in use\n";
        this->delay(2);
        this->clearScreen();
        goto GetUsername;
    }
    

    cout << "Success!\n";
    
    undo.actionType = ACTION_TYPE::USER_ADD;
    undo.user = newUser;
    this->undoManager.pushAction(&undo);
    this->userManager.saveToFile("Database/Users/users.csv");

    this->clearScreen();
    this->delay(1);

    return;
}

void Panel::removeUserPanel() {
    string id;
    undo_t change;
    this->showUserInfo();
    cout << "========< Remove User >========" << "\n";
    cout << "Enter the user ID (press e to exit): ";
    cin >> id;

    if (id == "e" || id == "E") {
        this->clearScreen();
        return;
    }

    User *user = this->userManager.find(stoull(id));

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
