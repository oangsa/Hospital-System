#pragma once
#ifndef PATIENT_H
#define PATIENT_H

#include "User.h"

class Patient: public User {
    private:
        ESI_LEVEL levelESI;
        PatientHistory history;
        DrugsAllergy drugsAllergy;

    public:
        Patient(user_t user);
        void setESI(ESI_LEVEL esi);
        void setHistory(PatientHistory history);

        u_int32 getESI();
        PatientHistory getHistory();
        vector<string> getDrugsAllergy();

        void displayInfo();
        void showHistory();
        void showDrugsAllergy();

        void loadDrugsAllergy();
        void saveDrugsAllergy();
        void addDrugAllergy(string drug);

        u_int8 isAllergicTo(string drug);

};

#endif
