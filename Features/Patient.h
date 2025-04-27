#pragma once
#ifndef PATIENT_H
#define PATIENT_H

#include "User.h"

class Patient: public User {
    private:
        ESI_LEVEL levelESI;
        PatientHistory history;

    public:
        Patient(user_t user);
        void setESI(ESI_LEVEL esi);
        void setHistory(PatientHistory history);
        u_int32 getESI();
        PatientHistory getHistory();
        void displayInfo();
        void showHistory();

};

#endif
