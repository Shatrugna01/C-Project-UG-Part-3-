#include "User.h"
#include "Dean.h"
#include "Utility.h"
#include <iostream>
#include <sstream>
#include <limits>
#include <algorithm>

using namespace std;

// Dean class methods
Dean::Dean(const string& uname, const string& pass)
    : User(uname, pass) {}

void Dean::dashboard() {
    int choice;
    do {
        cout << "\n--- Dean Dashboard ---\n";
        cout << "1. Declare Results\n";
        cout << "2. View Batch Report\n";
        cout << "3. Assign Program Director to Batch\n";
        cout << "4. View All Batches\n";
        cout << "5. View All Teachers and Program Directors\n";
        cout << "6. Change Password\n";
        cout << "7. Logout\n";
        cout << "Enter your choice: ";

        cin >> choice;
        switch (choice) {
            case 1: {
                string batchName;
                cout << "Enter batch name to declare results: ";
                cin >> batchName;
                declareResults(batchName);
                break;
            }
            case 2:{
                string batchName;
                cout << "Enter batch name to view report: ";
                cin >> batchName;
                viewReport(batchName);
                break;
            }
            case 3: {
                string batchName, programDirector;
                cout << "Enter batch name: ";
                cin >> batchName;
                cout << "Enter Program Director username: ";
                cin >> programDirector;
                assignProgramDirector(batchName, programDirector);
                break;
            }
            case 4: viewAllBatches(); break;
            case 5: viewAllTeachersAndProgramDirectors(); break;
            case 6: {
                string newPassword;
                cout << "Enter new password: ";
                cin >> newPassword;
                changePassword(newPassword);
                break;
            }
            case 7: cout << "Logging out...\n"; return;
            default: cout << "Invalid choice! Try again.\n";
        }
    } while (true);
}

void Dean::assignProgramDirector(const string& batchName, const string& programDirector) {
    // Load current assignments from the program_directors.txt file
    map<string, string> batchAssignments = loadProgramDirectorAssignments();

    // Step 1: Validate Batch Existence
    vector<string> batchNames = loadBatchNames();
    if (find(batchNames.begin(), batchNames.end(), batchName) == batchNames.end()) {
        cout << "Error: Batch \"" << batchName << "\" does not exist in the system.\n";
        return;
    }

    // Step 2: Validate Program Director Existence
    if (teachers.find(programDirector) == teachers.end()) {
        cout << "Error: User \"" << programDirector << "\" does not exist in the system.\n";
        return;
    }

    // Step 3: Ensure User is a Program Director
    if (!teachers[programDirector].getIsProgramDirector()) {
        char promoteChoice;
        cout << "Error: \"" << programDirector << "\" is not currently a Program Director.\n";
        cout << "Would you like to promote this user to Program Director? (y/n): ";
        cin >> promoteChoice;
        if (promoteChoice == 'y' || promoteChoice == 'Y') {
            teachers[programDirector] = Teacher(programDirector, teachers[programDirector].getPassword(), true);
            cout << programDirector << " has been successfully promoted to Program Director.\n";
            
        } else {
            cout << "Action canceled. No changes made.\n";
            return;
        }
    }

    // Step 4: Prevent Duplicate Assignment
    if (batchAssignments.find(batchName) != batchAssignments.end()) {
        cout << "Error: Batch \"" << batchName << "\" is already assigned to Program Director \""
             << batchAssignments[batchName] << "\".\n";
        return;
    }

    // Step 5: Assign the Batch to the Program Director
    batchAssignments[batchName] = programDirector;

    // Step 6: Save the Updated Assignments
    try {
        saveProgramDirectorAssignments(batchAssignments);
        cout << "Assigned \"" << programDirector << "\" as Program Director for batch \"" << batchName << "\" successfully.\n";
    } catch (const exception& e) {
        cerr << "Error: Failed to save the Program Director assignment. " << e.what() << "\n";
        return;
    }
    saveData();
}

void Dean::viewAllTeachersAndProgramDirectors() {
    cout << "\n--- All Teachers and Program Directors ---\n";

    // Display all teachers
    cout << "Teachers:\n";
    for (const auto& [teacherName, teacherObj] : teachers) {
        if (!teacherObj.getIsProgramDirector()) { // Display only teachers who are not PDs
            cout << "- " << teacherName << "\n";
        }
    }

    // Display all Program Directors
    cout << "\nProgram Directors:\n";

    // Load batch-to-Program Director mapping
    map<string, string> batchAssignments = loadProgramDirectorAssignments();

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


void Dean::declareResults(const string& batchName) {
    // Step 1: Validate Batch Name
    vector<string> batchNames = loadBatchNames();
    if (find(batchNames.begin(), batchNames.end(), batchName) == batchNames.end()) {
        cout << "Error: Batch \"" << batchName << "\" does not exist in the system.\n";
        return;
    }

    // Step 2: Check if the Batch Report Exists
    ifstream reportFile(batchName + "_report.txt");
    if (!reportFile.is_open()) {
        cout << "Error: No report found for batch \"" << batchName << "\". Program Director must submit the report.\n";
        return;
    }

    // Step 3: Validate if Results are Already Declared
    map<string, bool> resultStatuses = loadResultStatuses();
    if (resultStatuses.find(batchName) != resultStatuses.end() && resultStatuses[batchName]) {
        cout << "Error: Results for batch \"" << batchName << "\" are already declared.\n";
        reportFile.close();
        return;
    }

    // Step 4: Read the Report for Verification
    cout << "\n--- Batch Report Preview ---\n";
    string line;
    while (getline(reportFile, line)) {
        cout << line << "\n";
    }
    reportFile.close();

    // Step 5: Confirm Declaration
    char confirmation;
    cout << "Do you want to declare results for batch \"" << batchName << "\"? (y/n): ";
    cin >> confirmation;
    if (confirmation != 'y' && confirmation != 'Y') {
        cout << "Action canceled. Results not declared.\n";
        return;
    }

    // Step 6: Declare Results
    resultStatuses[batchName] = true;
    try {
        saveResultStatuses(resultStatuses);
        cout << "Results for batch \"" << batchName << "\" have been declared successfully.\n";
    } catch (const exception& e) {
        cerr << "Error: Unable to save result statuses. " << e.what() << "\n";
    }
}

void Dean::viewAllBatches() {
    ::viewAllBatches(); // Call utility function
}

