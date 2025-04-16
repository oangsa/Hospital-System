#pragma once
#ifndef PATIENT_H
#define PATIENT_H

#include "User.h"

class Patient: public User {
    private:
        NurseDiagnosis diagnosis;

    public:
        Patient(user_t user);
        void setDiagnosis(NurseDiagnosis diagnosis);
        void displayInfo();

};

#endif
