#include <numeric>
#include <iomanip>
#include <cmath>
#include "Utility.h"
#include "Teacher.h"

// Define global variables
map<string, Admin> admins;
map<string, Teacher> teachers;
map<string, Dean> deans;

// Function to save all data to files
void saveData() {
    ofstream file("users.txt");
    if (!file) {
        cerr << "Error: Unable to save data. Please check file permissions.\n";
        return;
    }

    // Save Admins
    for (const auto& admin : admins) {
        file << "Admin," << admin.first << "," << admin.second.getPassword() << "\n";
    }

    // Save Teachers (including Program Directors)
    for (const auto& teacher : teachers) {
        file << "Teacher," << teacher.first << "," << teacher.second.getPassword() << "," 
             << (teacher.second.getIsProgramDirector() ? "PD" : "Teacher") << "\n";
    }

    // Save Deans
    for (const auto& dean : deans) {
        file << "Dean," << dean.first << "," << dean.second.getPassword() << "\n";
    }

    file.close();
    cout << "Data saved successfully.\n";
}

// Function to load all data from files
void loadData() {
    ifstream file("users.txt");
    if (!file) {
        cerr << "Error: Unable to load data. Starting with an empty dataset.\n";
        return;
    }
    
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string role, username, password, type;
        getline(ss, role, ',');
        getline(ss, username, ',');
        getline(ss, password, ',');

        if (role == "Admin") {
            admins[username] = Admin(username, password);
        } else if (role == "Teacher") {
    		bool isPD = false;
    		if (getline(ss, type, ',')) {
        		isPD = (type == "PD");
    		}
    		teachers[username] = Teacher(username, password, isPD);

    		// Parse batch and subject assignments
    		string assignments;
    		while (getline(ss, assignments, ',')) {
        		size_t colonPos = assignments.find(':');
       				 if (colonPos != string::npos) {
            			string batch = assignments.substr(0, colonPos);
            			string subject = assignments.substr(colonPos + 1);
            			teachers[username].assignBatchAndSubject(batch, subject);
        				}
    			}
        } else if (role == "Dean") {
            deans[username] = Dean(username, password);
        } else {
            cerr << "Error: Invalid role in data file. Skipping entry.\n";
        }
    }

    file.close();
    cout << "Data loaded successfully.\n";
}

// Utility functions for Admin
void viewAllBatches() {
    vector<string> batchNames = loadBatchNames();
    cout << "\n--- All Batches ---\n";
    for (const string& batch : batchNames) {
        cout << "- " << batch << "\n";
    }
}

void viewAllTeachers() {
    cout << "\n--- All Teachers ---\n";
    for (const auto& teacher : teachers) {
        cout << "- " << teacher.first;
        if (teacher.second.getIsProgramDirector()) {
	    cout << " (Program Director)";
	}
        cout << "\n";
    }
}

void viewDeans() {
    cout << "\n--- All Deans ---\n";
    for (const auto& dean : deans) {
        cout << "- " << dean.first << "\n";
    }
}

void viewProgramDirectors() {
    cout << "\n--- Program Directors for Batches ---\n";

    // Load the batch-to-Program Director mapping
    map<string, string> batchAssignments = loadProgramDirectorAssignments();

    // Iterate through the teachers and display Program Directors
    for (const auto& [teacherName, teacherObj] : teachers) {
        if (teacherObj.getIsProgramDirector()) { // Display only teachers who are PDs
            cout << "- " << teacherName << " (Assigned batches: ";
            bool hasBatches = false;

            // Find batches assigned to this Program Director
            for (const auto& [batchName, programDirector] : batchAssignments) {
                if (programDirector == teacherName) {
                    cout << batchName << " ";
                    hasBatches = true;
                }
            }

            if (!hasBatches) {
                cout << "None";
            }
            cout << ")\n";
        }
    }
}




// Function to load batch data from a specific file
vector<string> loadBatchData(const string& batchName) {
    ifstream file(batchName + ".txt");
    vector<string> data;

    if (!file) {
        cerr << "Error: Unable to load batch data for " << batchName << ". File not found.\n";
        return data;
    }

    string line;
    while (getline(file, line)) {
        data.push_back(line);
    }

    file.close();
    return data;
}

// Function to save batch data to a specific file
void saveBatchData(const string& batchName, const vector<string>& data) {
    ofstream file(batchName + ".txt");
    if (!file) {
        cerr << "Error: Unable to save batch data for " << batchName << ". Please check file permissions.\n";
        return;
    }

    for (const string& entry : data) {
        file << entry << "\n";
    }

    file.close();
    cout << "Batch data for " << batchName << " saved successfully.\n";
}

// Function to load batch issues
vector<string> loadBatchIssues(const string& batchName) {
    ifstream file(batchName + "_issues.txt");
    vector<string> issues;

    if (!file) {
        cerr << "Error: Unable to load batch issues for " << batchName << ". File not found.\n";
        return issues;
    }

    string line;
    while (getline(file, line)) {
        issues.push_back(line);
    }

    file.close();
    return issues;
}

// Function to save batch issues
void saveBatchIssues(const string& batchName, const vector<string>& issues) {
    ofstream file(batchName + "_issues.txt");
    if (!file) {
        cerr << "Error: Unable to save batch issues for " << batchName << ". Please check file permissions.\n";
        return;
    }

    for (const string& issue : issues) {
        file << issue << "\n";
    }

    file.close();
    cout << "Batch issues for " << batchName << " saved successfully.\n";
}

// Function to load batch names
vector<string> loadBatchNames() {
    ifstream file("batches.txt");
    vector<string> batchNames;

    if (!file) {
        cerr << "Error: Unable to load batch names. File not found.\n";
        return batchNames;
    }

    string batchName;
    while (getline(file, batchName)) {
        batchNames.push_back(batchName);
    }

    file.close();
    return batchNames;
}

// Function to save batch names
void saveBatchNames(const vector<string>& batchNames) {
    ofstream file("batches.txt");
    if (!file) {
        cerr << "Error: Unable to save batch names. Please check file permissions.\n";
        return;
    }

    for (const string& batchName : batchNames) {
        file << batchName << "\n";
    }

    file.close();
    cout << "Batch names saved successfully.\n";
}

void saveProgramDirectorAssignments(const map<string, string>& batchAssignments) {
    ofstream file("program_directors.txt");
    if (!file) {
        cerr << "Error: Unable to save Program Director assignments.\n";
        return;
    }

    for (const auto& [batch, programDirector] : batchAssignments) {
        file << batch << "," << programDirector << "\n";
    }

    file.close();
    cout << "Program Director assignments saved successfully.\n";
}

map<string, string> loadProgramDirectorAssignments() {
    map<string, string> batchAssignments;
    ifstream file("program_directors.txt");
    if (!file) {
        cerr << "Error: Unable to load Program Director assignments. File not found.\n";
        return batchAssignments;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string batch, programDirector;
        getline(ss, batch, ',');
        getline(ss, programDirector, ',');

        if (!batch.empty() && !programDirector.empty()) {
            batchAssignments[batch] = programDirector;
        }
    }

    file.close();
    cout << "Program Director assignments loaded successfully.\n";
    return batchAssignments;
}

void viewReport(const string& batchName) {
    ifstream reportFile(batchName + "_report.txt");
    if (!reportFile.is_open()) {
        cout << "Error: Report for batch \"" << batchName << "\" does not exist.\n";
        return;
    }

    string line;
    cout << "\n--- Report for Batch: " << batchName << " ---\n";
    while (getline(reportFile, line)) {
        cout << line << "\n";
    }
    reportFile.close();
}
void saveResultStatuses(const map<string, bool>& resultStatuses) {
    ofstream file("results_status.txt");
    if (!file) {
        cerr << "Error: Unable to save results declaration status.\n";
        return;
    }

    for (const auto& [batch, isDeclared] : resultStatuses) {
        file << batch << "," << isDeclared << "\n";
    }

    file.close();
    cout << "Results declaration status saved successfully.\n";
}

map<string, bool> loadResultStatuses() {
    map<string, bool> resultStatuses;
    ifstream file("results_status.txt");
    if (!file) {
        cerr << "Error: Unable to load results declaration status. Starting with default values.\n";
        return resultStatuses;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string batch;
        string status;
        getline(ss, batch, ',');
        getline(ss, status, ',');

        if (!batch.empty()) {
            resultStatuses[batch] = (status == "1");
        }
    }

    file.close();
    return resultStatuses;
}

