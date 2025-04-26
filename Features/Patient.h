#pragma once
#ifndef PATIENT_H
#define PATIENT_H

#include "User.h"

class Patient: public User {
    private:
        NurseDiagnosis diagnosis;
        PatientHistory history;

    public:
        Patient(user_t user);
        void setDiagnosis(NurseDiagnosis diagnosis);
        void setHistory(PatientHistory history);
        void displayInfo();
        void showHistory();

};

#endif
