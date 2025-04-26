#include "Patient.h"
#include <iostream>

using namespace std;

Patient::Patient(user_t user): User(user) {}

void Patient::setDiagnosis(NurseDiagnosis diagnosis) {
    this->diagnosis = diagnosis;
}

void Patient::setHistory(PatientHistory history) {
    this->history = history;
}

void Patient::displayInfo() {
    cout << "Name: " << this->getName() << "\n";
    cout << "Age: " << this->getAge() << "\n";
}

void Patient::showHistory() {
    time_t time = history._timestamp;
    struct tm *t = localtime(&time);
    cout << "========================================\n\n";
    cout << "Time: " << (t->tm_year + 1900) << "-" << t->tm_mon << "-" << t->tm_mday << " " << t->tm_hour << ":" << t->tm_min << "\n";
    cout << "Diagnosis: " << history._diagnosis << "\n";
    cout << "Treatment: " << history._treatment << "\n";
    cout << "Prescription: " << history._prescription << "\n";
    cout << "\n========================================\n";
}
