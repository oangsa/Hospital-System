#ifndef TRIAGE_SYSTEM_H
#define TRIAGE_SYSTEM_H

#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

struct PatientTriage {
    int id;
    int ESI; // ESI 1 (critical) -> 5 (least)

    bool operator<(const PatientTriage& other) const {
        return ESI > other.ESI; // Higher ESI -> lower priority
    }
};

class TriageSystem {
private:
    priority_queue<PatientTriage> triageQueue;
    string inputFilename;
    string outputFilename;
    vector<int> patientIDs;
    size_t currentIndex = 0;

    bool askYesNo(const string& question);
    int determineESI();
    vector<int> loadPatientIDs(const string& filename);
    void saveToPaPQ(const string& filename, priority_queue<PatientTriage> patientQueue);
    void removeUsedIDFromPaQ(const string& filename, int usedID);
    void addPatient();
    void displayQueue(priority_queue<PatientTriage> triageQueue);

public:
    TriageSystem(const string& inputFile, const string& outputFile);
    void run();
};

#endif // TRIAGE_SYSTEM_H
