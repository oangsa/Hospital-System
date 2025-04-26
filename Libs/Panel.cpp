#include "Panel.h"
#include "../Features/FileManager.h"
#include "Define.h"
#include <cstdlib>
#include <iostream>
#include <string>
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
            // Show all users except the current logged in user
            if (current->data()->getID() != this->LoggedUser->getID()) cout << current->data()->getID() << "\t" << current->data()->getUsername() << "\t" << current->data()->getTypeString() << "\n";
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
void Panel::loginPanel(u_int16 attempt, u_int8 isFileExist) {
    string username;
    string password;
    u_int64 userId;
    FileManager fileManager;
    u_int16 i;

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

    for (i = 0; i < attempt; i++) {
        this->clearScreen();

        if (i != 0) cout << (i) << "/" << attempt << " attempted." << "\n";
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

    if (i == attempt) {
        this->clearScreen();
        cout << "Too many attempts." << "\n";
        cout << "Exiting..." << "\n";
        this->delay(1);
        this->clearScreen();
        return;
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
    char choice;
    while (1)
    {
        cout << "========< Patient Panel >========" << "\n\n";
        if (!this->userManager.uniqueIds.contains(this->LoggedUser->getID(), this->LoggedUser->getType())) cout << "   1. Enqueue" << "\n";
        if (this->userManager.uniqueIds.contains(this->LoggedUser->getID(), this->LoggedUser->getType())) cout << "   2. Check Remaining"<< "\n";
        cout << "   3. View History" << "\n";
        cout << "   E. Exit (without Logout)" << "\n";
        cout << "   L. Logout" << "\n\n";
        cout << "================================" << "\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 'E':
            case 'e':
                this->clearScreen();
                this->delay(1);
                this->clearScreen();
                return;

            case 'L':
            case 'l':
                this->clearScreen();
                this->delay(1);
                this->clearScreen();
                this->loginPanel(3, 0);

            case '1':
                if (this->userManager.uniqueIds.contains(this->LoggedUser->getID(), this->LoggedUser->getType())) {
                    this->clearScreen();
                    break;
                }

                this->clearScreen();
                cout << "Adding...\n";
                this->userManager.userEnqueue(this->LoggedUser->getUser_t());
                this->delay(1);
                this->clearScreen();
                break;

            case '2':
                if (!this->userManager.uniqueIds.contains(this->LoggedUser->getID(), this->LoggedUser->getType())) {
                    this->clearScreen();
                    break;
                }

                this->clearScreen();
                this->showRemaining();
                this->delay(1);
                this->clearScreen();
                break;

            case '3':
                this->clearScreen();
                this->showHistories();
                break;

            default:
                cout << "Invalid choice: \n";
                this->delay(1);
                this->clearScreen();
                break;
        }
    }
}

void Panel::showHistories() {
    char options;
    this->LoggedUser->displayHistory();
    cout << "Press e or E to exit: ";
    cin >> options;

    switch (options) {
        case 'e':
        case 'E':
            this->clearScreen();
            this->delay(1);
            return;

        default:
            this->clearScreen();
            this->showHistories();
            break;

    }

}

void Panel::showRemaining() {
    char options;
    int16 idx = this->userManager.userIdQueue.getIndexOf(this->LoggedUser->getID());
    cout << "There (are/is) " << idx << " queue(s) ahead you.\n";
    cout << "Press e or E to exit: ";
    cin >> options;

    switch (options) {
        case 'e':
        case 'E':
            this->clearScreen();
            this->delay(1);
            return;

        default:
            this->clearScreen();
            this->showRemaining();
            break;

    }
}

void Panel::doctorMenu() { cout << "========< Doctor Panel >========" << "\n"; }

void Panel::nurseMenu() { cout << "========< Nurse Panel >========" << "\n"; }

void Panel::adminMenu() {
    char choice;

    while (1) {
        cout << "========< Admin Panel >========" << "\n\n";
        cout << "   1. Add user" << "\n";
        cout << "   2. Remove user" << "\n";
        cout << "   3. Edit User" << "\n";
        cout << "   L. Logout" << "\n";
        if (this->undoManager.peekAction().actionType != ACTION_TYPE::EMPTY_ACTION) cout << "   R. Revert Last Change \n";
        if (this->undoManager.peekAction().actionType != ACTION_TYPE::EMPTY_ACTION) cout << "   S. Save \n";
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
                    if (this->undoManager.peekAction().actionType != ACTION_TYPE::EMPTY_ACTION) {
                        askSave2:
                        cout << "The file is not saved yet.\n";
                        cout << "Do you want to save the file? (y/n): ";
                        cin >> choice;
                        switch (choice) {
                            case 'y':
                            case 'Y':
                                this->clearScreen();
                                cout << "Saved" << "\n";
                                this->userManager.saveToFile("Database/Users/users.csv");
                                this->undoManager.clear();
                                this->delay(2);
                                this->clearScreen();
                                break;
                            case 'n':
                            case 'N':
                                break;
                            default:
                                cout << "Invalid choice" << "\n";
                                this->delay(2);
                                this->clearScreen();
                                goto askSave2;
                                break;

                        }
                    }
                    cout << "Exiting..." << "\n";
                    this->userManager.saveToFile("Database/Users/users.csv");
                    this->delay(2);
                    this->clearScreen();
                    return;

                case 'l':
                case 'L':
                    this->clearScreen();

                    if (this->undoManager.peekAction().actionType != ACTION_TYPE::EMPTY_ACTION) {
                        askSave:
                        cout << "The file is not saved yet.\n";
                        cout << "Do you want to save the file? (y/n): ";
                        cin >> choice;
                        switch (choice) {
                            case 'y':
                            case 'Y':
                                this->clearScreen();
                                cout << "Saved" << "\n";
                                this->userManager.saveToFile("Database/Users/users.csv");
                                this->undoManager.clear();
                                this->delay(2);
                                this->clearScreen();
                                break;
                            case 'n':
                            case 'N':
                                break;
                            default:
                                cout << "Invalid choice" << "\n";
                                this->delay(2);
                                this->clearScreen();
                                goto askSave;
                                break;

                        }

                    }

                    cout << "Logging out..." << "\n";

                    this->userManager.logout();
                    this->delay(2);
                    this->clearScreen();
                    this->loginPanel(3, 0);
                    break;

                case 's':
                case 'S':
                    save:
                    this->clearScreen();
                    cout << "Saved" << "\n";
                    this->userManager.saveToFile("Database/Users/users.csv");
                    this->undoManager.clear();
                    this->delay(1);
                    this->clearScreen();
                    break;

                case '1':
                    this->clearScreen();
                    this->addUserPanel();
                    break;

                case '2':
                    this->clearScreen();
                    this->removeUserPanel();
                    break;

                case '3':
                    this->clearScreen();
                    this->updateUserPanel();
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

void Panel::updateUserPanel() {
    string id;
    char choice;
    undo_t change;
    string data;
    vector<string> vs;
    string word = "";
    user_t newUser;

    this->showUserInfo();
    cout << "========< Update User >========" << "\n";
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
    newUser = user->getUser_t();

    this->clearScreen();

    updateUser:
    cout << "========< Update User >========" << "\n\n";
    cout << "1. Name: ";
    cout << newUser.name << "\n";
    cout << "2. Birthdate: ";
    cout << newUser.birthDate._day << " " << newUser.birthDate._month << " " << newUser.birthDate._year << "\n";
    cout << "S. Save" << "\n\n";
    cout << "================================" << "\n";
    cout << "Enter the field number to update (press e to exit): ";
    cin >> choice;

    cin.ignore();

    this->clearScreen();

    switch (choice) {
        case 'e':
        case 'E':
            this->clearScreen();
            return;

        case 's':
        case 'S':
            this->clearScreen();
            cout << "Saved" << "\n";
            this->userManager.updateUser(newUser);
            this->delay(1);
            this->clearScreen();
            break;

        case '2':
            cout << "Enter the new birthdate (dd mm yyyy): ";
            getline(cin, data);

            vs.clear();
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
            goto updateUser;

            break;


        case '1':
            cout << "Name: " << user->getName() << "\n";
            cout << "Enter the new name: ";
            getline(cin, newUser.name);

            this->clearScreen();
            goto updateUser;
            break;
    }


    change.actionType = ACTION_TYPE::USER_UPDATE;
    change.user = user->getUser_t();

    this->undoManager.pushAction(&change);
}

void Panel::addUserPanel() {
    user_t newUser;
    string data;
    string word = "";
    vector<string> vs;
    undo_t undo;
    VALIDATOR_ERROR_TYPE error = VALIDATOR_ERROR_TYPE::NO_ERROR;
    cout << "========< Add User >========" << "\n";

    cin.ignore();

    cout << "Name (title - name - surname): ";
    getline(cin, data);

    newUser.name = data;

    getBirthDate:
    if (error != VALIDATOR_ERROR_TYPE::NO_ERROR) cout << "Latest: " << newUser.birthDate._day << " " << newUser.birthDate._month << " " << newUser.birthDate._year << "\n";
    cout << "Birthdate (dd mm yyy) ex: 05 07 2015: ";
    getline(cin, data);
    logger.log("%s", data.c_str());

    // NOTE: SPLIT USER INPUT BY 'SPACE';

    for (char charc : data) {
        if (charc == ' ') {
            if (!word.empty()) {
                vs.push_back(word);
                logger.log("%s", word.c_str());
                word.clear();
            }
        }
        else {
            word += charc;
        }
    }

    if (!word.empty()) vs.push_back(word);

    newUser.birthDate._day = atoi(vs[0].c_str());
    newUser.birthDate._month = atoi(vs[1].c_str());
    newUser.birthDate._year = atoi(vs[2].c_str());

    vs.clear();
    word.clear();
    word = "";

    error = validator.isBirthDateValid(newUser.birthDate);
    this->clearScreen();

    switch (error) {
        case VALIDATOR_ERROR_TYPE::DAY_ERROR:
            cout << "Invalid day.\n";
            this->delay(1);
            this->clearScreen();
            goto getBirthDate;

        case VALIDATOR_ERROR_TYPE::MONTH_ERROR:
            cout << "Invalid month.\n";
            this->delay(1);
            this->clearScreen();
            goto getBirthDate;

        case VALIDATOR_ERROR_TYPE::YEAR_ERROR:
            cout << "Invalid year.\n";
            this->delay(1);
            this->clearScreen();
            goto getBirthDate;

        // PREVENT WARNING
        case VALIDATOR_ERROR_TYPE::NEGATIVE_NUMBER_ERROR:
        case VALIDATOR_ERROR_TYPE::NO_ERROR:
        case VALIDATOR_ERROR_TYPE::NOT_ENOUGH_LEN_ERROR:
        case VALIDATOR_ERROR_TYPE::NO_LOWER_ERROR:
        case VALIDATOR_ERROR_TYPE::NO_NUMBER_ERROR:
        case VALIDATOR_ERROR_TYPE::NO_UPPER_ERROR:
        case VALIDATOR_ERROR_TYPE::NO_SPECIAL_ERROR:
            break;

    }

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

    if (validator.isPasswordValid(newUser.password) == VALIDATOR_ERROR_TYPE::NO_ERROR) goto regis;

    GetPassword:
    this->clearScreen();
    cout << "Username: " << newUser.username << "\n";
    cout << "Password: ";
    cin >> newUser.password;

    error = validator.isPasswordValid(newUser.password);
    this->clearScreen();

    switch (error) {
        case VALIDATOR_ERROR_TYPE::NOT_ENOUGH_LEN_ERROR:
            cout << "Password must be at least 8 characters long.\n";
            this->delay(1);
            goto GetPassword;

        case VALIDATOR_ERROR_TYPE::NO_LOWER_ERROR:
            cout << "Password must have at least 1 lower character.\n";
            this->delay(1);
            goto GetPassword;

        case VALIDATOR_ERROR_TYPE::NO_UPPER_ERROR:
            cout << "Password must have at least 1 upper character.\n";
            this->delay(1);
            goto GetPassword;

        case VALIDATOR_ERROR_TYPE::NO_SPECIAL_ERROR:
            cout << "Password must have at least 1 special character.\n";
            this->delay(1);
            goto GetPassword;

        case VALIDATOR_ERROR_TYPE::NO_NUMBER_ERROR:
            cout << "Password must have at least 1 number.\n";
            this->delay(1);
            goto GetPassword;

        // TO PREVENT WARNING
        case VALIDATOR_ERROR_TYPE::NO_ERROR:
        case VALIDATOR_ERROR_TYPE::DAY_ERROR:
        case VALIDATOR_ERROR_TYPE::MONTH_ERROR:
        case VALIDATOR_ERROR_TYPE::YEAR_ERROR:
        case VALIDATOR_ERROR_TYPE::NEGATIVE_NUMBER_ERROR:
            break;

    }

    regis:
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
    this->delay(1);

    undo.actionType = ACTION_TYPE::USER_ADD;
    undo.user = newUser;
    this->undoManager.pushAction(&undo);

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
