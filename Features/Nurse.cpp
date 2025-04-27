#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip> // For formatting

using namespace std;

struct PatientTriage {
    int id;
    int ESI; // ESI 1 (critical) -> 5 (least)

    // Priority Queue
    bool operator<(const PatientTriage& other) const {
        return ESI > other.ESI; // Higher ESI means lower priority
    }
};

// yes or no
bool askYesNo(string question) {
    char ans;
    cout << question << " (y/n): ";
    cin >> ans;
    return (ans == 'y' || ans == 'Y');
}

// Determine ESI
int determineESI() {
    cout << "\n=========================================\n";
    cout << "        Patient Triage Questionnaire      \n";
    cout << "=========================================\n";

    if (askYesNo("Is the patient unconscious or not breathing?")) return 1;
    if (askYesNo("Does the patient have severe pain or bleeding?")) return 2;
    if (askYesNo("Does the patient need multiple resources (e.g., labs, x-ray, IV)?")) return 3;
    if (askYesNo("Does the patient have moderate complaints (e.g., fever, headache)?")) return 4;

    return 5; // Minor complaints or routine
}

// Save patient data to PaPQ.csv using priority queue
void saveToPaPQ(const string& filename, priority_queue<PatientTriage> patientQueue) {
    ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        cerr << "Error: Unable to open file " << filename << " for writing." << endl;
        return;
    }

    while (!patientQueue.empty()) {
        PatientTriage p = patientQueue.top();
        patientQueue.pop();
        outputFile << p.id << "," << (p.ESI-1)<<"\n"; // Save ID and ESI-1
    }
}

// Remove used ID from _PaQ.csv
void removeUsedIDFromPaQ(const string& filename, int usedID) {
    vector<int> patientIDs ;
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << " for writing." << endl;
        return;
    }

    for (int id : patientIDs) {
        if (id != usedID) {
            file << id << "\n";
        }
    }
}

// Add patient to the queue
void addPatient(priority_queue<PatientTriage>& triageQueue, const string& inputFilename, const string& outputFilename, size_t& currentIndex, vector<int>& patientIDs) {
    if (currentIndex >= patientIDs.size()) {
        cout << "No more patients in the list.\n";
        return;
    }

    PatientTriage p;
    p.id = patientIDs[currentIndex++];
    cout << "\n=========================================\n";
    cout << "           Add New Patient               \n";
    cout << "=========================================\n";
    cout << " Patient ID: " << p.id << "\n";

    p.ESI = determineESI();
    cout << "\n-----------------------------------------\n";
    cout << " ESI level assigned: " << p.ESI << "\n";
    cout << "-----------------------------------------\n";

    // Add to Priority Queue
    triageQueue.push(p);

    // Save to PaPQ.csv
    saveToPaPQ(outputFilename, triageQueue);

    // Remove used ID from _PaQ.csv
    removeUsedIDFromPaQ(inputFilename, p.id);

    cout << " Patient added to queue successfully!\n";
}

// Display patients in the queue
void displayQueue(priority_queue<PatientTriage> triageQueue) {
    cout << "\n=========================================\n";
    cout << "       Queue of Patients by Priority     \n";
    cout << "=========================================\n";

    if (triageQueue.empty()) {
        cout << " No patients in the queue.\n";
        return;
    }

    cout << left << setw(10) << "ID"
         << setw(10) << "ESI" << "\n";
    cout << "-----------------------------------------\n";

    while (!triageQueue.empty()) {
        PatientTriage p = triageQueue.top();
        triageQueue.pop();
        cout << left << setw(10) << p.id
             << setw(10) << p.ESI << "\n";
    }
}

int TT() {
    priority_queue<PatientTriage> triageQueue;
    string inputFilename = "c:\\Hospital feature\\Hospital-System\\Database\\Temp\\_PaQ.csv";
    string outputFilename = "c:\\Hospital feature\\Hospital-System\\Database\\Temp\\PaPQ.csv";

    return 0;
}
