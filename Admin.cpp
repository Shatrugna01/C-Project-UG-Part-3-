#include "User.h"
#include "Admin.h"
#include "Utility.h"
#include <iostream>
#include <sstream>
#include <limits>
#include <algorithm>

using namespace std;
string Default_pass="12345";

Admin::Admin(const string& uname, const string& pass)
    : User(uname, pass) {}

void Admin::dashboard() {
    int choice;
    do {
        cout << "\n--- Admin Dashboard ---\n";
        cout << "1. Add Teacher\n";
        cout << "2. Add Dean\n";
        cout << "3. Add Batch\n";
        cout << "4. View All Batches\n";
        cout << "5. View All Teachers\n";
        cout << "6. View All Deans\n";
        cout << "7. View Program Directors\n";
        cout << "8. Change Password\n";
        cout << "9. Logout\n";
        cout << "Enter your choice: ";

        while (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1: addTeacher(); break;
            case 2: addDean(); break;
            case 3: addBatch(); break;
            case 4: viewAllBatches(); break;
            case 5: viewAllTeachers(); break;
            case 6: viewDeans(); break;
            case 7: viewProgramDirectors(); break;
            case 8: {
                string newPassword;
                cout << "Enter new password: ";
                cin >> newPassword;
                changePassword(newPassword);
                break;
            }
            case 9: cout << "Logging out...\n"; return;
            default: cout << "Invalid choice! Try again.\n";
        }
    } while (true);
}

void Admin::addTeacher() {
    string username, password=Default_pass;
    bool isProgramDirector = false;
    char pdChoice;

    cout << "Enter teacher username: ";
    cin >> username;
    cout << "Password saved with default password: "<<Default_pass <<endl;

    cout << "Is this teacher also a Program Director? (y/n): ";
    cin >> pdChoice;
    if (pdChoice == 'y' || pdChoice == 'Y') {
        isProgramDirector = true;
    }

    if (username.empty() || password.empty()) {
        cout << "Error: Username and password cannot be empty.\n";
        return;
    }

    teachers[username] = Teacher(username, password, isProgramDirector);
    saveData();
    cout << "Teacher " << username << (isProgramDirector ? " (Program Director)" : "") << " added successfully.\n";
}

void Admin::addDean() {
    string username, password =Default_pass;
    cout << "Enter dean username: ";
    cin >> username;
    cout << "Password saved with default password: "<<Default_pass <<endl;

    if (username.empty() || password.empty()) {
        cout << "Error: Username and password cannot be empty.\n";
        return;
    }

    deans[username] = Dean(username, password);
    saveData();
    cout << "Dean " << username << " added successfully.\n";
}

void Admin::addBatch() {
    string batchName;
    int numStudents;
    cout << "Enter batch name: ";
    cin >> batchName;
    cout << "Enter number of students: ";


    while (!(cin >> numStudents)) {
        cout << "Invalid input. Please enter a number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }


    vector<string> students;
    for (int i = 0; i < numStudents; ++i) {
        string username, password = "12345";
        cout << "Enter username for student " << (i + 1) << ": ";
        cin >> username;
        cout << "Password saved with default password: "<<Default_pass<<endl;
        students.push_back(username + "," + password);
    }

    saveBatchData(batchName, students);

    // Save batch name to a global file
    ofstream batchFile("batches.txt", ios::app);
    if (batchFile.is_open()) {
        batchFile << batchName << "\n";
        batchFile.close();
    } else {
        cerr << "Error saving batch name.\n";
    }

    cout << "Batch added successfully.\n";
}

void Admin::viewAllBatches() {
    ::viewAllBatches(); // Call utility function
}

void Admin::viewAllTeachers() {
    ::viewAllTeachers(); // Call utility function
}

void Admin::viewDeans() {
    ::viewDeans(); // Call utility function
}

void Admin::viewProgramDirectors() {
    ::viewProgramDirectors(); // Call utility function
}

