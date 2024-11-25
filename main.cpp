#include <iostream>
#include <numeric>
#include <iomanip>
#include <cmath>
#include <limits> // For input stream clearing
#include "User.h"
#include "Utility.h"
#include "Admin.h"
#include "Teacher.h"
#include "Dean.h"
#include "Student.h"


using namespace std;

// Function declarations
void login();
void mainMenu();

// Helper function to validate integer input
int getValidatedChoice(int min, int max) {
    int choice;
    while (true) {
        cin >> choice;
        if (cin.fail() || choice < min || choice > max) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number between " << min << " and " << max << ": ";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the buffer
            return choice;
        }
    }
}

void login() {
    string username, password;
    cout << "\n--- Login ---\n";

    // Username input
    cout << "Enter username: ";
    getline(cin, username);
    if (username.empty()) {
        cout << "Username cannot be empty. Please try again.\n";
        return;
    }

    // Password input
    cout << "Enter password: ";
    getline(cin, password);
    if (password.empty()) {
        cout << "Password cannot be empty. Please try again.\n";
        return;
    }

    // Admin login validation
    if (admins.count(username) && admins[username].getPassword() == password) {
        cout << "Login successful as Admin.\n";
        admins[username].dashboard();
        return;
    }

    // Dean login validation
    if (deans.count(username) && deans[username].getPassword() == password) {
        cout << "Login successful as Dean.\n";
        deans[username].dashboard();
        return;
    }

    // Teacher/Program Director login validation
    if (teachers.count(username) && teachers[username].getPassword() == password) {
        cout << "Login successful as " << (teachers[username].getIsProgramDirector() ? "Program Director" : "Teacher") << ".\n";
        teachers[username].dashboard();
        return;
    }
    
    // Student login validation
    vector<string> batchNames = loadBatchNames();
    for (const string& batchName : batchNames) {
        vector<string> students = loadBatchData(batchName);
        for (const string& student : students) {
            stringstream ss(student);
            string storedUsername, storedPassword;
            getline(ss, storedUsername, ',');
            getline(ss, storedPassword, ',');

            if (username == storedUsername && password == storedPassword) {
                cout << "Login successful as Student.\n";
                Student studentUser(username, password, batchName);
                studentUser.dashboard();
                return;
            }
        }
    }

    // Invalid login
    cout << "Invalid username or password. Please try again.\n";
}

void mainMenu() {
    int choice;
    do {
        cout << "\n--- Main Menu ---\n";
        cout << "1. Login\n";
        cout << "2. Exit\n";
        cout << "Enter your choice: ";

        // Validate the menu choice input
        choice = getValidatedChoice(1, 2);

        switch (choice) {
            case 1:
                login();
                break;
            case 2:
                cout << "Exiting the program. Goodbye!\n";
                return;
            default: // This is technically unreachable now
                cout << "Invalid choice! Please try again.\n";
        }
    } while (true);
}

int main() {
    try {
        loadData(); // Load existing data from files
    } catch (const exception& e) {
        cerr << "Error loading data: " << e.what() << "\n";
        return 1; // Exit with an error code
    }
    mainMenu(); // Start the main menu
    return 0;
}

