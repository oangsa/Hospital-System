#include "Patient.h"
#include "FileManager.h"
#include "../Libs/Logger.h"
#include <iostream>
#include <fstream>
#include <cctype>

using namespace std;

Patient::Patient(user_t user): User(user) {}

void Patient::setESI(ESI_LEVEL esi) {
    this->levelESI = esi;
}

void Patient::setHistory(PatientHistory history) {
    this->history = history;
}

void Patient::displayInfo() {
    cout << "Name: " << this->getName() << "\n";
    cout << "Age: " << this->getAge() << "\n";
    cout << "Drugs Allergy: ";
    this->showDrugsAllergy();
    cout << "\n";
}

void Patient::showHistory() {
    time_t time = history._timestamp;
    struct tm *t = localtime(&time);
    cout << "Patient's History: \n\n";
    cout << "> Time: " << (t->tm_year + 1900) << "-" << t->tm_mon + 1 << "-" << t->tm_mday << " " << (t->tm_hour < 10 ? "0" : "") << t->tm_hour << ":" << (t->tm_min < 10 ? "0" : "") << t->tm_min << "\n";
    cout << "> Diagnosis: " << history._diagnosis << "\n";
    cout << "> Treatment: " << history._treatment << "\n";
    cout << "> Prescription: " << history._prescription << "\n";
}

u_int32 Patient::getESI() {
    return this->levelESI;
}

PatientHistory Patient::getHistory() {
    return this->history;
}

vector<string> Patient::getDrugsAllergy() {
    return this->drugsAllergy;
}

void Patient::loadDrugsAllergy() {
    FileManager fileManager;
    string base = "Database/Informations/";
    base += to_string(this->getID()) + ".csv";

    if (!fileManager.isFileExists(base)) {
        return;
    }

    ifstream infile(base);
    string line;

    while (getline(infile, line)) {
        this->addDrugAllergy(line);
    }
}

void Patient::showDrugsAllergy() {
    for (string drug : this->drugsAllergy) {
        cout << drug << " ";
    }
    cout << '\n';
}

void Patient::addDrugAllergy(string drug) {
    if (this->isAllergicTo(drug)) return;
    this->drugsAllergy.push_back(drug);
}

u_int8 Patient::isAllergicTo(string drug) {
    Logger logger;
    string lowerDrug = "";
    for (char c : drug) lowerDrug += tolower(c);
    for (string d :this->drugsAllergy) {
        string lowerAll = "";
        for (char c : d) lowerAll += tolower(c);
        logger.log("lowerall %s", lowerAll.c_str());
        logger.log("lowerDrug %s", lowerDrug.c_str());
        // string contain substring
        // Prevent Para and Paracetamol
        if (lowerAll.find(lowerDrug) != string::npos) return 1;
        if (lowerDrug.find(lowerAll) != string::npos) return 1;
        if (lowerAll == lowerDrug) return 1;
    }
    
    return 0;
}

void Patient::saveDrugsAllergy() {
    Logger logger;
    std::string base = "Database/Informations/";
    base += std::to_string(this->getID()) + ".csv";

    std::ofstream file(base);
    if (!file.is_open()) {
        logger.log("Failed to open file: %s", base.c_str());
        return;
    }

    for (const std::string& d : this->drugsAllergy) {
        logger.log("%s", d.c_str());
        file << d.c_str() << "\n";
    }

    file.close();
}
