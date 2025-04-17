#include "Patient.h"
#include <iostream>

using namespace std;

Patient::Patient(user_t user): User(user) {}

void Patient::setDiagnosis(NurseDiagnosis diagnosis) {
    this->diagnosis = diagnosis;
}

void Patient::displayInfo() {
    cout << "Name: " << this->getName() << "\n";
    cout << "Age: " << this->getAge() << "\n";
}

