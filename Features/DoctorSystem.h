#ifndef DOCTOR_SYSTEM_H
#define DOCTOR_SYSTEM_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "../Libs/Logger.h"

using namespace std;

struct PatientRecord {
    string time;
    string diagnosis;
    string treatment;
    string prescription;
};

class DoctorSystem {
private:
    Logger logger;
    vector<PatientRecord> records;

    vector<string> split(const string &str, char delimiter);
    string join(const vector<string>& vec, char delimiter);
    void loadFromCSV(int id);
    void saveToCSV(int id);
    vector<int> loadQueue(const string& filename);
    void saveQueue(const string& filename, const vector<int>& queue);
    void dequeuePatient(const string& filename, int id);

public:
    void viewPatient(int id);
    void addRecord(int id);
    void processQueue(const string& queueFilename);
};

#endif // DOCTOR_SYSTEM_H
