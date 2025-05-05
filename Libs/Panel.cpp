/**************************************************
 *                                                *
 *       Panel Class Implementation File          *
 *       Latest Update: May 5, 2025               *
 *                                                *
 **************************************************/


#include "Panel.h"
#include "Define.h"
#include "../Features/FileManager.h"
#include "../Features/Patient.h"
#include "../Features/Program.h"
#include <cstdio>
#include <regex>
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
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
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
    Function to show all users in the system
    It will show all users except the current logged in user
*/
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
    Function to switch the panel based on the user type
    It will show the menu for the logged in user
*/
void Panel::switchPanel() {
    switch (this->LoggedUser->getType()) {
        case UserType::OPD:
        case UserType::IPD:
            this->clearScreen();
            this->delay(1);
            this->patientMenu();
            break;

        case UserType::DOCTOR:
            this->clearScreen();
            this->delay(1);
            this->doctorMenu();
            break;

        case UserType::NURSE:
            this->delay(1);
            this->clearScreen();
            this->nurseMenu();
            break;

        case UserType::ADMIN:
            this->delay(1);
            this->clearScreen();
            this->adminMenu();
            break;

        default:
            this->clearScreen();
            this->delay(2);
            cout << "Unexpected Error occour." << "\n";
            return;
        }
}

/*
    Function to show the main menu
    It will show the menu for the user to login or register
    Parameters:
        u_int8 isFileExist: Check if the cache file exists or not
*/
void Panel::mainMenu(u_int8 isFileExist) {
    char choice;

    this->logger.log("What?");

    if (isFileExist) {
        this->LoggedUser = this->userManager.loadLoggedUser();
        if (this->LoggedUser != NULL) {
            this->clearScreen();
            cout << "Welcome back, " << this->LoggedUser->getUsername() << "\n";
            this->delay(2);
            this->clearScreen();

            return this->switchPanel(); // Skipped Loop if user exits
        }
    }

    MainMenu:
    this->clearScreen();
    cout << "========< Main Menu >========" << "\n\n";
    cout << "   1. Login" << "\n";
    cout << "   2. Register" << "\n\n";
    cout << "=============================" << "\n";
    cout << "Enter your choice: ";
    cin >> choice;
    cin.ignore(1000, '\n');

    switch (choice) {
        case '1':
            this->delay(1);
            this->clearScreen();
            return this->loginPanel(3);

        case '2':
            this->delay(1);
            this->clearScreen();
            this->delay(1);
            return this->registerPanel();

        default:
            this->clearScreen();
            this->delay(1);
            goto MainMenu;
    }

}

/*
    Login panel function propts the user for username and password
    if username and password are valid, it shows welcome message
    and show the panel base on the user role (UserType)
    parameters:
        u_int8 attempt: Number of attempts for login
*/
void Panel::loginPanel(u_int16 attempt) {
    string username;
    string password;
    u_int64 userId;
    FileManager fileManager;
    u_int16 i;

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
    return this->switchPanel();
}

/*
    Function to register a new user
    It will prompt the user for name, birthday and user's gender
    It will also prompt the user for username and password
*/

void Panel::registerPanel() {
    user_t newUser;
    string data;
    string word = "";
    vector<string> vs;
    undo_t undo;
    VALIDATOR_ERROR_TYPE error = VALIDATOR_ERROR_TYPE::NO_ERROR;
    cout << "========< Register >========" << "\n";

    GetName:
    cout << "Name: ";
    getline(cin, data);

    if (this->validator.isStringValid(data) == VALIDATOR_ERROR_TYPE::NOT_VALID_STRING) {
        this->clearScreen();
        cout << "Name cannot contain comma or be an empty string.\n";
        this->delay(1);
        this->clearScreen();
        goto GetName;
    }

    newUser.name = data;

    getBirthDate:
    if (error != VALIDATOR_ERROR_TYPE::NO_ERROR) cout << "Latest: " << newUser.birthDate._day << " " << newUser.birthDate._month << " " << newUser.birthDate._year << "\n";
    cout << "Birthdate (dd mm yyyy) ex: 05 07 2015: ";
    getline(cin, data);

    if (this->validator.isBirthDateStringValid(data) == VALIDATOR_ERROR_TYPE::NOT_VALID_STRING) {
        this->clearScreen();
        cout << "Bro try to broke the system.\n";
        this->delay(1);
        this->clearScreen();
        data.clear();
        goto getBirthDate;
    }

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
    logger.log("size_t %llu", vs.size());

    if (vs.size() != 3) {
        this->clearScreen();
        cout << "Invalid input.\n";
        this->delay(2);
        this->clearScreen();
        vs.clear();
        word.clear();
        word = "";
        goto getBirthDate;
    }

    newUser.birthDate._day = atoi(vs[0].c_str());
    newUser.birthDate._month = atoi(vs[1].c_str());
    newUser.birthDate._year = atoi(vs[2].c_str());

    vs.clear();
    word.clear();
    word = "";

    error = validator.isBirthDateValid(newUser.birthDate);
    this->clearScreen();

    switch (error) {
        case VALIDATOR_ERROR_TYPE::FUTURE_DATE_ERROR:
            cout << "Birthday cannot be in the future.\n";
            this->delay(1);
            this->clearScreen();
            goto getBirthDate;

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
        case VALIDATOR_ERROR_TYPE::NOT_VALID_STRING:
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
        this->delay(1);
        this->clearScreen();
    }

    newUser.gender = (data == "1") ? Gender::MALE : Gender::FEMALE;
    this->clearScreen();

    newUser.userType = UserType::OPD;

    this->clearScreen();

    GetUsername:
    cin.ignore();
    cout << "Username: ";
    getline(cin, newUser.username);

    if (this->validator.isStringValid(newUser.username) == VALIDATOR_ERROR_TYPE::NOT_VALID_STRING) {
        this->clearScreen();
        cout << "Username can only contains English and number.\n";
        this->delay(3);
        this->clearScreen();
        newUser.username.clear();
        goto GetUsername;
    }

    if (validator.isPasswordValid(newUser.password) == VALIDATOR_ERROR_TYPE::NO_ERROR) goto regis;

    GetPassword:
    this->clearScreen();
    cout << "Username: " << newUser.username << "\n";
    cout << "Password: ";
    cin >> newUser.password;

    error = validator.isPasswordValid(newUser.password);
    this->clearScreen();

    if (error != VALIDATOR_ERROR_TYPE::NO_ERROR) newUser.password.clear();

    switch (error) {
        case VALIDATOR_ERROR_TYPE::NOT_ENOUGH_LEN_ERROR:
            cout << "Password must be at least 8 characters long.\n";
            this->delay(3);
            goto GetPassword;

        case VALIDATOR_ERROR_TYPE::NO_LOWER_ERROR:
            cout << "Password must have at least 1 lower character.\n";
            this->delay(3);
            goto GetPassword;

        case VALIDATOR_ERROR_TYPE::NO_UPPER_ERROR:
            cout << "Password must have at least 1 upper character.\n";
            this->delay(3);
            goto GetPassword;

        case VALIDATOR_ERROR_TYPE::NO_SPECIAL_ERROR:
            cout << "Password must have at least 1 special character.\n";
            this->delay(3);
            goto GetPassword;

        case VALIDATOR_ERROR_TYPE::NO_NUMBER_ERROR:
            cout << "Password must have at least 1 number.\n";
            this->delay(3);
            goto GetPassword;

        case VALIDATOR_ERROR_TYPE::NOT_VALID_STRING:
            cout << "Bro tryna broke the system.\n";
            this->delay(3);
            goto GetPassword;

        // TO PREVENT WARNING
        case VALIDATOR_ERROR_TYPE::NO_ERROR:
        case VALIDATOR_ERROR_TYPE::DAY_ERROR:
        case VALIDATOR_ERROR_TYPE::MONTH_ERROR:
        case VALIDATOR_ERROR_TYPE::YEAR_ERROR:
        case VALIDATOR_ERROR_TYPE::NEGATIVE_NUMBER_ERROR:
        case VALIDATOR_ERROR_TYPE::FUTURE_DATE_ERROR:
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


    this->userManager.saveToFile("Database/Users/users.csv");
    cout << "Success!\nPlease login again!";
    this->delay(2);
    this->clearScreen();
    this->delay(1);
    return this->loginPanel(3);
}

/**************************************************
 *                                                *
 *                  Patient Section               *
 *                                                *
 **************************************************/

/*
    Function to show the patient menu
    It will show the menu for the patient to enqueue, check remaining patients and view history
*/
void Panel::patientMenu() {
    char choice;
    Program program(20);
    PatientPanel:
    cout << "========< Patient Panel >========" << "\n\n";
    if (!this->userManager.uniqueIds.contains(this->LoggedUser->getID(), this->LoggedUser->getType()) && !this->userManager.UserIdPQ.has(this->LoggedUser->getID())) cout << "   1. Enqueue" << "\n";
    if (this->userManager.uniqueIds.contains(this->LoggedUser->getID(), this->LoggedUser->getType()) || this->userManager.UserIdPQ.has(this->LoggedUser->getID())) cout << "   2. Check Remaining"<< "\n";
    cout << "   3. View History" << "\n";
    cout << "   E. Exit (without Logout)" << "\n";
    cout << "   L. Logout" << "\n\n";
    cout << "================================" << "\n";
    cout << "Enter your choice: ";
    cin >> choice;
    cin.ignore(1000, '\n');


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
            this->userManager.logout();
            this->delay(2);
            this->clearScreen();
            program.Init(); // Init the program to prevent data loss during the program
            break;

        case '1':
            if (this->userManager.uniqueIds.contains(this->LoggedUser->getID(), this->LoggedUser->getType()) || this->userManager.UserIdPQ.has(this->LoggedUser->getID())) {
                this->clearScreen();
                goto PatientPanel;
                break;
            }

            this->clearScreen();
            cout << "Adding...\n";
            this->userManager.userEnqueue(this->LoggedUser->getUser_t());
            this->delay(1);
            this->clearScreen();
            goto PatientPanel;
            break;

        case '2':
            if (!this->userManager.uniqueIds.contains(this->LoggedUser->getID(), this->LoggedUser->getType()) && !this->userManager.UserIdPQ.has(this->LoggedUser->getID())) {
                this->clearScreen();
                goto PatientPanel;
                break;
            }

            this->clearScreen();
            this->showRemaining();
            this->delay(1);
            this->clearScreen();
            goto PatientPanel;
            break;

        case '3':
            this->clearScreen();
            this->showHistories();
            goto PatientPanel;
            break;

        default:
            cout << "Invalid choice: \n";
            this->delay(1);
            this->clearScreen();
            break;
    }
}

/*
    Function to show the history of the patient
    It will show the history of the patient in a list format
*/
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

/*
    Function to show the remaining patients in the queue
    It will show the number of patients in the queue ahead of the current patient
*/
void Panel::showRemaining() {
    char options;
    int16 idx = this->userManager.userIdQueue.getIndexOf(this->LoggedUser->getID());
    if (idx == -1) idx = this->userManager.UserIdPQ.getIndexOf(this->LoggedUser->getID());
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


/**************************************************
 *                                                *
 *                  Doctor Section                *
 *                                                *
 **************************************************/


/*
    Function to show the doctor menu
    It will show the menu for the doctor to process the next patient in queue
*/
void Panel::doctorMenu() {
    char choice;
    Program program(20);
    DoctorPanel:
    cout << "========< Doctor Panel >========" << "\n\n";
    if (!this->userManager.UserIdPQ.empty()) cout << "   1. Process Next Patient in queue " << "(" << this->userManager.UserIdPQ.size() << " Remaining)\n";
    cout << "   E. Exit (Without logout)\n";
    cout << "   L. Logout\n\n";
    cout << "================================" << "\n";
    cout << "Enter your choice: ";
    cin >> choice;
    cin.ignore(1000, '\n');

    switch (choice) {
        case 'e':
        case 'E':
            this->clearScreen();
            this->delay(1);
            return;

        case 'L':
        case 'l':
            this->clearScreen();
            this->userManager.logout();
            this->delay(2);
            this->clearScreen();
            program.Init(); // Init the program to prevent data loss during the program
            break;

        case '1':
            if (this->userManager.UserIdPQ.empty()) {
                this->clearScreen();
                goto DoctorPanel;
                break;
            }

            this->clearScreen();
            this->doctorProcessPatientPanel();
            goto DoctorPanel;
            break;

        default:
            this->clearScreen();
            cout << "Invalid choice: \n";
            this->delay(1);
            goto DoctorPanel;
            break;
    }
}

/*
    Function to process the first patient in queue
    It will show the patient information and allow the doctor to add a record
*/
void Panel::doctorProcessPatientPanel() {
    char choice;
    User* user = this->userManager.find(*(this->userManager.UserIdPQ.peek()));
    Patient p(user->getUser_t());
    p.loadDrugsAllergy();
    PatientHistory history;
    time_t t = time(NULL);
    history._timestamp = t;

    process:
    p.displayInfo();
    if (!history._diagnosis.empty()) p.showHistory();
    cout << "\n";
    cout << "=============================\n\n";
    if (history._diagnosis.empty()) cout << "   1. Add Record\n";
    cout << "   2. Finish And Save\n\n";
    cout << "=============================\n";
    cout << "Enter your choice: ";
    cin >> choice;
    cin.ignore(1000, '\n');

    switch (choice) {
        case '1':
            if (!history._diagnosis.empty()) {
                this->clearScreen();
                goto process;
                break;
            }

            this->clearScreen();
            this->delay(1);
            this->addRecordPanel(&p, &history);
            goto process;
            break;

        case '2':
            if (history._diagnosis.empty()) {
                this->clearScreen();
                goto process;
                break;
            }

            this->clearScreen();
            this->userManager.UserIdPQ.dequeue();
            this->userManager.UserIdPQ.saveToFile();
            this->userManager.writeHistory(p);
            cout << "Done!\n";
            this->delay(2);
            this->clearScreen();
            return;

        default:
            this->clearScreen();
            this->delay(1);
            goto process;
            break;
    }
}

/*
    Function to add a record for the patient
    It will prompt the doctor for diagnosis, treatment and prescription
    It will also check if the patient has any drug allergy
    Params:
        Patient* user: Pointer to the patient object
        PatientHistory* history: Pointer to the patient history object
*/
void Panel::addRecordPanel(Patient* user, PatientHistory* history) {
    string word = "";
    vector<string> vs;
    GetDiagnosis:
    cout << "Enter diagnosis:\n>> ";
    getline(cin, history->_diagnosis);
    if (this->validator.isStringValid(history->_diagnosis) == VALIDATOR_ERROR_TYPE::NOT_VALID_STRING) {
        this->clearScreen();
        cout << "Invalid input.\n";
        this->delay(2);
        this->clearScreen();
        goto GetDiagnosis;
    }

    GetTreatment:
    cout << "Enter treatment:\n>> ";
    getline(cin, history->_treatment);
    if (this->validator.isStringValid(history->_treatment) == VALIDATOR_ERROR_TYPE::NOT_VALID_STRING) {
        this->clearScreen();
        cout << "Invalid input.\n";
        this->delay(2);
        this->clearScreen();
        goto GetTreatment;
    }

    GetPrescription:
    cout << "Enter prescription(Use | to separate medicine):\n>> ";
    getline(cin, history->_prescription);
    if (this->validator.isStringValid(history->_prescription) == VALIDATOR_ERROR_TYPE::NOT_VALID_STRING) {
        this->clearScreen();
        cout << "Invalid input.\n";
        this->delay(2);
        this->clearScreen();
        goto GetPrescription;
    }

    // Manipulate String
    for (char c : history->_prescription) {
        // NOTE: SPLIT USER INPUT BY '|'
        if (c == '|') {
            if (!word.empty()) {
                vs.push_back(word);
                word.clear();
            }
        }
        else {
            word += c;
        }
    }

    if (!word.empty()) vs.push_back(word);

    // Check if doctor entered only one drug
    if (!vs.size()) {
        logger.log("Drug %s", history->_prescription.c_str());
        if (user->isAllergicTo(history->_prescription)) {
            cout << "Patient has drug allergy to " << history->_prescription << "\n";
            vs.clear();
            word.clear();
            this->delay(2);
            this->clearScreen();
            goto GetPrescription;
        }
    }

    // Check if patient has any drug allergy
    for (string drug : vs) {
        logger.log("Drug %s", drug.c_str());
        if (user->isAllergicTo(drug)) {
            cout << "Patient has drug allergy to " << drug << "\n";
            vs.clear();
            word.clear();
            this->delay(2);
            this->clearScreen();
            goto GetPrescription;
        }
    }

    user->setHistory(*history);
    this->clearScreen();
}

/**************************************************
 *                                                *
 *                  Nurse Section                 *
 *                                                *
 **************************************************/

/*
    Function to show the nurse menu
    It will show the menu for the nurse to process the next patient in queue
*/
void Panel::nurseMenu() {
    char choice;
    Program program(20);
    NursePanel:
    cout << "========< Nurse Panel >=========" << "\n\n";
    if (!this->userManager.userIdQueue.isEmpty()) cout << "  1. Process Next Patient in queue " << "(" << this->userManager.userIdQueue.size() << " Remaining)\n";
    cout << "  E. Exit (without logout)\n";
    cout << "  L. Logout\n\n";
    cout << "================================" << "\n";
    cout << "Enter your choice: ";
    cin >> choice;
    cin.ignore(1000, '\n');

    switch (choice) {
        case 'e':
        case 'E':
            this->clearScreen();
            this->delay(1);
            return;

        case 'l':
        case 'L':
            this->clearScreen();

            cout << "Logging out..." << "\n";

            this->userManager.logout();
            this->delay(2);
            this->clearScreen();
            program.Init();
            break;

        case '1':
            if (this->userManager.userIdQueue.isEmpty()) {
                this->clearScreen();
                goto NursePanel;
                break;
            }

            this->clearScreen();
            this->nurseProcessPatientPanel();
            goto NursePanel;
            break;

        default:
            this->clearScreen();
            cout << "Invalid choice: \n";
            this->delay(1);
            goto NursePanel;
            break;
    }

}

/*
    Function to ask the user a yes or no question
    Params:
        string question: The question to ask
    returns:
        u_int8: 1 for yes, 0 for no
*/
u_int8 Panel::askYesNo(string question) {
    char ans;
    ask:
    cout << question << " (y/n): ";
    cin >> ans;

    switch (ans) {
        case 'y':
        case 'Y':
            return 1;

        case 'n':
        case 'N':
            return 0;

        default:
            this->clearScreen();
            cout << "Invalid input.\n";
            this->delay(2);
            this->clearScreen();
            goto ask;
    }

}

/*
    Function to determine the ESI level of the patient
    It will ask the user a series of questions and return the ESI level based on the answers
    Returns:
        ESI_LEVEL: The ESI level of the patient
*/
ESI_LEVEL Panel::determine() {
    cout << "\n=========================================\n";
    cout << "        Patient Triage Questionnaire      \n";
    cout << "=========================================\n";

    if (this->askYesNo("Is the patient unconscious or not breathing?")) return ESI_LEVEL::ESI_1;
    if (this->askYesNo("Does the patient have severe pain or bleeding?")) return ESI_LEVEL::ESI_2;
    if (this->askYesNo("Does the patient need multiple resources (e.g., labs, x-ray, IV)?")) return ESI_LEVEL::ESI_3;
    if (this->askYesNo("Does the patient have moderate complaints (e.g., fever, headache)?")) return ESI_LEVEL::ESI_4;

    return ESI_LEVEL::ESI_5;
}

/*
    Function to process the first patient in queue
    It will show the patient information and allow the nurse to add patient's drug allergies
*/
void Panel::nurseProcessPatientPanel() {
    User* user = this->userManager.find(*this->userManager.userIdQueue.peek());
    user_t u = user->getUser_t();
    Patient p(u);
    // Have to load it manually. Too Lazy to make it save automatically.
    p.loadDrugsAllergy();
    p.displayInfo();
    this->askAddAllergies(&p);
    ESI_LEVEL lvl = this->determine();
    p.setESI(lvl);
    cout << "\n-----------------------------------------\n";
    cout << " ESI level assigned: " << p.getESI() + 1 << "\n";
    cout << "-----------------------------------------\n";
    this->userManager.nurseEnqueuePatient(p);
    this->userManager.userIdQueue.dequeue();
    this->userManager.userIdQueue.saveToFile();
    this->delay(2);
    this->clearScreen();
    return;
}

/*
    Function to ask the user to add drug allergies for the patient
    It will prompt the user for drug name and add it to the patient's drug allergy list
    Params:
        Patient* patient: Pointer to the patient
*/
void Panel::askAddAllergies(Patient* patient) {
    string drugName;
    char choice;

    AskAddDrug:
    cout << "========< Add Drugs Allergy >========" << "\n\n";
    cout << "   1. Add Drug" << "\n";
    cout << "   E. Exit" << "\n\n";
    cout << "================================" << "\n";
    cout << "Enter your choice: ";
    cin >> choice;
    cin.ignore(1000, '\n');

    switch (choice) {
        case 'e':
        case 'E':
            this->clearScreen();
            this->delay(1);
            return;

        case '1':
            // ToLazyToAddAnotherMethod
            this->clearScreen();
            this->delay(1);
            cout << "Total Drug(s) Allergy: " << patient->getDrugsAllergy().size() << '\n' << '\n';
            cout << "Drug Name (Type E or e to exit): ";
            getline(cin, drugName);

            if (drugName == "E" || drugName == "e") {
                this->clearScreen();
                return;
            }

            if (this->validator.isStringValid(drugName) == VALIDATOR_ERROR_TYPE::NOT_VALID_STRING) {
                cout << "Drug name cannot be only in English.\n";
                drugName.clear();
                this->clearScreen();
                goto AskAddDrug;
            }

            patient->addDrugAllergy(drugName);
            // Also have to save it manually.
            patient->saveDrugsAllergy();
            this->delay(1);
            this->clearScreen();
            goto AskAddDrug;

        default:
            this->clearScreen();
            cout << "Invalid choice.\n";
            this->delay(2);
            this->clearScreen();
            goto AskAddDrug;
            break;
    }

}

/**************************************************
 *                                                *
 *                  Admin Section                 *
 *                                                *
 **************************************************/

/*
    Function to show the admin menu
    It will show the menu for the admin to add, remove and edit users
*/
void Panel::adminMenu() {
    char choice;
    Program program(20);
    AdminPanel:
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
    cin.ignore(1000, '\n');

    switch (choice) {
        case 'r':
        case 'R':
            this->clearScreen();
            if (this->undoManager.peekAction().actionType == ACTION_TYPE::EMPTY_ACTION) {
                goto AdminPanel;
                break;
            }
            cout << "Reverting last change..." << "\n";
            this->undoManager.undoAction();
            this->delay(2);
            this->clearScreen();
            goto AdminPanel;
            break;

        case 'e':
        case 'E':
            this->clearScreen();
            if (this->undoManager.peekAction().actionType != ACTION_TYPE::EMPTY_ACTION) {
            askSave2:
                cout << "The file is not saved yet.\n";
                cout << "Do you want to save the file? (y/n): ";
                cin >> choice;
                cin.ignore(1000, '\n');
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
                        this->clearScreen();
                        goto AdminPanel;
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
                cin.ignore(1000, '\n');
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
                        this->clearScreen();
                        goto AdminPanel;
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
            program.Init(); // Init the program to prevent data loss during the program
            break;

        case 's':
        case 'S':
            this->clearScreen();
            cout << "Saved" << "\n";
            this->userManager.saveToFile("Database/Users/users.csv");
            this->undoManager.clear();
            this->delay(1);
            this->clearScreen();
            goto AdminPanel;
            break;

        case '1':
            this->clearScreen();
            this->addUserPanel();
            goto AdminPanel;
            break;

        case '2':
            this->clearScreen();
            this->removeUserPanel();
            goto AdminPanel;
            break;

        case '3':
            this->clearScreen();
            this->updateUserPanel();
            goto AdminPanel;
            break;

        default:
            this->clearScreen();
            cout << "Invalid choice" << "\n";
            this->delay(1);
            this->clearScreen();
            goto AdminPanel;
            break;
    }
}

/*
    Function to remove a user from the system
    It will prompt the user for the user ID and remove the user from the system
*/
void Panel::updateUserPanel() {
    string id;
    char choice;
    undo_t change;
    string data;
    vector<string> vs;
    string word = "";
    user_t newUser;
    std::regex digitsOnly("^[0-9]+$");

    GetID:
    this->showUserInfo();
    cout << "========< Update User >========" << "\n";
    cout << "Enter the user ID (press e to exit): ";

    cin >> id;

    if (id == "e" || id == "E") {
        this->clearScreen();
        return;

    }

    if (!std::regex_match(id, digitsOnly)) {
        this->clearScreen();
        cout << "Invalid Id.\n";
        this->delay(1);
        this->clearScreen();
        goto GetID;
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
    cin.ignore(1000, '\n');

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
            getBirthDate:
            cout << "Enter the new birthdate (dd mm yyyy): ";
            getline(cin, data);
            if (this->validator.isStringValid(data) == VALIDATOR_ERROR_TYPE::NOT_VALID_STRING) {
                this->clearScreen();
                cout << "Invalid input.\n";
                this->delay(2);
                this->clearScreen();
                goto getBirthDate;
            }

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

            if (vs.size() != 3) {
                this->clearScreen();
                cout << "Invalid input.\n";
                this->delay(2);
                this->clearScreen();
                goto getBirthDate;
            }

            newUser.birthDate._day = atoi(vs[0].c_str());
            newUser.birthDate._month = atoi(vs[1].c_str());
            newUser.birthDate._year = atoi(vs[2].c_str());

            this->clearScreen();
            goto updateUser;
            break;


        case '1':
            GetName:
            cout << "Name: " << user->getName() << "\n";
            cout << "Enter the new name: ";
            getline(cin, newUser.name);

            if (this->validator.isStringValid(newUser.name) == VALIDATOR_ERROR_TYPE::NOT_VALID_STRING) {
                this->clearScreen();
                cout << "Name cannot contain comma and cannot be an empty string.\n";
                this->delay(2);
                this->clearScreen();
                goto GetName;
            }

            this->clearScreen();
            goto updateUser;
            break;
    }


    change.actionType = ACTION_TYPE::USER_UPDATE;
    change.user = user->getUser_t();

    this->undoManager.pushAction(&change);
}

/*
    Function to add a new user to the system
    It will prompt the user for the user information and add the user to the system
*/
void Panel::addUserPanel() {
    user_t newUser;
    string data;
    string word = "";
    vector<string> vs;
    undo_t undo;
    VALIDATOR_ERROR_TYPE error = VALIDATOR_ERROR_TYPE::NO_ERROR;
    cout << "========< Add User >========" << "\n";

    GetName:
    cout << "Name (title - name - surname): ";
    cin.ignore();
    getline(cin, data);

    if (this->validator.isStringValid(data) == VALIDATOR_ERROR_TYPE::NOT_VALID_STRING) {
        this->clearScreen();
        cout << "Name cannot contain comma or be an empty string.\n";
        this->delay(1);
        this->clearScreen();
        goto GetName;
    }

    newUser.name = data;

    getBirthDate:
    if (error != VALIDATOR_ERROR_TYPE::NO_ERROR) cout << "Latest: " << newUser.birthDate._day << " " << newUser.birthDate._month << " " << newUser.birthDate._year << "\n";
    cout << "Birthdate (dd mm yyyy) ex: 05 07 2015: ";
    getline(cin, data);

    if (this->validator.isBirthDateStringValid(data) == VALIDATOR_ERROR_TYPE::NOT_VALID_STRING) {
        this->clearScreen();
        cout << "Bro try to broke the system.\n";
        this->delay(1);
        this->clearScreen();
        data.clear();
        goto getBirthDate;
    }

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
    logger.log("size_t %llu", vs.size());

    if (vs.size() != 3) {
        this->clearScreen();
        cout << "Invalid input.\n";
        this->delay(2);
        this->clearScreen();
        vs.clear();
        word.clear();
        word = "";
        goto getBirthDate;
    }

    newUser.birthDate._day = atoi(vs[0].c_str());
    newUser.birthDate._month = atoi(vs[1].c_str());
    newUser.birthDate._year = atoi(vs[2].c_str());

    vs.clear();
    word.clear();
    word = "";

    error = validator.isBirthDateValid(newUser.birthDate);
    this->clearScreen();

    switch (error) {
        case VALIDATOR_ERROR_TYPE::FUTURE_DATE_ERROR:
            cout << "Birthday cannot be in the future.\n";
            this->delay(1);
            this->clearScreen();
            goto getBirthDate;

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
        case VALIDATOR_ERROR_TYPE::NOT_VALID_STRING:
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
        this->delay(1);
        this->clearScreen();
    }

    newUser.gender = (data == "1") ? Gender::MALE : Gender::FEMALE;
    this->clearScreen();

    while (1) {
        cout << "User Type: \n";
        cout << "1. IPD\n2. OPD\n3. NURSE\n4. DOCTOR\n5. ADMIN\n";
        cout << ">> ";
        cin >> data;

        if (!(data != "1" && data != "2" && data != "3" && data != "4" && data != "5")) break;

        cout << "Invalid Input.\n";
        this->delay(1);
        this->clearScreen();
    }

    newUser.userType = (data == "1") ? UserType::IPD : (data == "2") ? UserType::OPD : (data == "3") ? UserType::NURSE : (data == "4") ? UserType::DOCTOR : UserType::ADMIN;

    this->clearScreen();

    GetUsername:
    cin.ignore();
    cout << "Username: ";
    getline(cin, newUser.username);

    if (this->validator.isStringValid(newUser.username) == VALIDATOR_ERROR_TYPE::NOT_VALID_STRING) {
        this->clearScreen();
        cout << "Username can only contains English and number.\n";
        this->delay(3);
        this->clearScreen();
        newUser.username.clear();
        goto GetUsername;
    }

    if (validator.isPasswordValid(newUser.password) == VALIDATOR_ERROR_TYPE::NO_ERROR) goto regis;

    GetPassword:
    this->clearScreen();
    cout << "Username: " << newUser.username << "\n";
    cout << "Password: ";
    cin >> newUser.password;

    error = validator.isPasswordValid(newUser.password);
    this->clearScreen();

    if (error != VALIDATOR_ERROR_TYPE::NO_ERROR) newUser.password.clear();

    switch (error) {
        case VALIDATOR_ERROR_TYPE::NOT_ENOUGH_LEN_ERROR:
            cout << "Password must be at least 8 characters long.\n";
            this->delay(3);
            goto GetPassword;

        case VALIDATOR_ERROR_TYPE::NO_LOWER_ERROR:
            cout << "Password must have at least 1 lower character.\n";
            this->delay(3);
            goto GetPassword;

        case VALIDATOR_ERROR_TYPE::NO_UPPER_ERROR:
            cout << "Password must have at least 1 upper character.\n";
            this->delay(3);
            goto GetPassword;

        case VALIDATOR_ERROR_TYPE::NO_SPECIAL_ERROR:
            cout << "Password must have at least 1 special character.\n";
            this->delay(3);
            goto GetPassword;

        case VALIDATOR_ERROR_TYPE::NO_NUMBER_ERROR:
            cout << "Password must have at least 1 number.\n";
            this->delay(3);
            goto GetPassword;

        case VALIDATOR_ERROR_TYPE::NOT_VALID_STRING:
            cout << "Bro tryna broke the system.\n";
            this->delay(3);
            goto GetPassword;

        // TO PREVENT WARNING
        case VALIDATOR_ERROR_TYPE::NO_ERROR:
        case VALIDATOR_ERROR_TYPE::DAY_ERROR:
        case VALIDATOR_ERROR_TYPE::MONTH_ERROR:
        case VALIDATOR_ERROR_TYPE::YEAR_ERROR:
        case VALIDATOR_ERROR_TYPE::NEGATIVE_NUMBER_ERROR:
        case VALIDATOR_ERROR_TYPE::FUTURE_DATE_ERROR:
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

/*
    Function to remove a user from the system
    It will prompt the user for the user ID and remove the user from the system
*/
void Panel::removeUserPanel() {
    string id;
    undo_t change;
    std::regex digitOnly("^[0-9]+$");

    GetID:
    this->showUserInfo();
    cout << "========< Remove User >========" << "\n";
    cout << "Enter the user ID (press e to exit): ";
    cin >> id;

    if (id == "e" || id == "E") {
        this->clearScreen();
        return;
    }

    if (!std::regex_match(id, digitOnly)) {
        this->clearScreen();
        cout << "Invalid Id.\n";
        this->delay(1);
        this->clearScreen();
        goto GetID;
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
