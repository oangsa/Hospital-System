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

    vector<string> split(const string &str, char delimiter) {
        vector<string> tokens;
        stringstream ss(str);
        string token;
        while (getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    string join(const vector<string>& vec, char delimiter) {
        string result;
        for (size_t i = 0; i < vec.size(); ++i) {
            result += vec[i];
            if (i != vec.size() - 1) result += delimiter;
        }
        return result;
    }

    void loadFromCSV(int id) {
        records.clear();
        string filename = "../Database/History/" + to_string(id) + ".csv";
        ifstream file(filename);
        if (!file.is_open()) {
            logger.log("Error: Unable to open file %s", filename.c_str());
            return;
        }
        string line;
        getline(file, line); // Skip header

        while (getline(file, line)) {
            stringstream ss(line);
            string time, diagnosis, treatment, prescription;

            getline(ss, time, ',');
            getline(ss, diagnosis, ',');
            getline(ss, treatment, ',');
            getline(ss, prescription, ',');

            PatientRecord record = {time, diagnosis, treatment, prescription};
            records.push_back(record);
        }
    }

    void saveToCSV(int id) {
        string filename = "../Database/History/" + to_string(id) + ".csv";
        ofstream file(filename);
        file << "time,diagnosis,treatment,prescription\n";
        for (const auto& record : records) {
            file << record.time << ","
                 << record.diagnosis << ","
                 << record.treatment << ","
                 << record.prescription << "\n";
        }
    }

    vector<int> loadQueue(const string& filename) {
        vector<int> queue;
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Unable to open file " << filename << endl;
            return queue;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string idStr, esiStr;
            getline(ss, idStr, ','); // Read ID
            queue.push_back(stoi(idStr));
        }
        return queue;
    }

    void saveQueue(const string& filename, const vector<int>& queue) {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Unable to open file " << filename << " for writing." << endl;
            return;
        }

        for (int id : queue) {
            file << id << ",1\n"; // Keep ESI as 1 for simplicity
        }
    }

    void dequeuePatient(const string& filename, int id) {
        vector<int> queue = loadQueue(filename); // load queue PaPQ
        vector<int> updatedQueue; // new queue
    
        // copy id to new queue except the id that is dequeued
        for (int patientID : queue) {
            if (patientID != id) {
                updatedQueue.push_back(patientID);
            }
        }
    
        // save q to PaPQ
        saveQueue(filename, updatedQueue);
    
        cout << "Patient ID " << id << " has been dequeued from the queue.\n";
    }

public:
    void viewPatient(int id) {
        loadFromCSV(id);
        cout << "\n=========================================\n";
        cout << "              Patient Info               \n";
        cout << "=========================================\n";
        if (records.empty()) {
            cout << "No records found for patient ID: " << id << "\n";
        } else {
            for (size_t i = 0; i < records.size(); ++i) {
                cout << "[" << i + 1 << "] Time: " << records[i].time << "\n";
                cout << "    Diagnosis: " << records[i].diagnosis << "\n";
                cout << "    Treatment: " << records[i].treatment << "\n";
                cout << "    Prescription: " << records[i].prescription << "\n";
                cout << "-----------------------------------------\n";
            }
        }
        cout << "=========================================\n";
    }

    void addRecord(int id) {
        PatientRecord record;
        cout << "\n Enter time (e.g., 2025-04-24 14:30): ";
        cin.ignore();
        getline(cin, record.time);
        cout << " Enter diagnosis: ";
        getline(cin, record.diagnosis);
        cout << " Enter treatment: ";
        getline(cin, record.treatment);
        cout << " Enter prescription: ";
        getline(cin, record.prescription);

        records.push_back(record);
        saveToCSV(id);
        cout << "\n Record added successfully!\n";
    }

    void processQueue(const string& queueFilename) {
        vector<int> queue = loadQueue(queueFilename);
        if (queue.empty()) {
            cout << "No patients in the queue.\n";
            return;
        }

        int id = queue.front(); // Get the first patient in the queue
        cout << "\nProcessing patient ID: " << id << "\n";
        viewPatient(id);

        int choice;
        do {
            cout << "\n1. Add Record\n";
            cout << "2. Finish and Dequeue Patient\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    addRecord(id);
                    break;
                case 2:
                    dequeuePatient(queueFilename, id);
                    cout << "Patient ID " << id << " has been dequeued.\n";
                    break;
                default:
                    cout << "Invalid choice. Try again.\n";
            }
        } while (choice != 2);
    }
};

int main() {
    DoctorSystem system;
    string queueFilename = "c:\\Hospital feature\\Hospital-System\\Database\\Temp\\PaPQ.csv";

    cout << "\n=========================================\n";
    cout << "         Welcome to Doctor System        \n";
    cout << "=========================================\n";

    int choice;
    do {
        cout << "\n=========================================\n";
        cout << "                  Menu                   \n";
        cout << "=========================================\n";
        cout << "1. Process Next Patient in Queue\n";
        cout << "0. Exit\n";
        cout << "=========================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                system.processQueue(queueFilename);
                break;
            case 0:
                cout << "\nExiting...\n";
                break;
            default:
                cout << "\nInvalid choice! Please try again.\n";
        }
    } while (choice != 0);

    cout << "\n=========================================\n";
    cout << "       Thank you for using the system!   \n";
    cout << "=========================================\n";

    return 0;
}
