#include "User.h"
#include "Student.h"
#include "Utility.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <limits>

using namespace std;

// Student class methods
Student::Student(const string& uname, const string& pass, const string& batch)
    : User(uname, pass), batchName(batch) {}

void Student::dashboard() {
    int choice;
    do {
        cout << "\n--- Student Dashboard ---\n";
        cout << "1. View Marks\n";
        cout << "2. Report an Issue\n";
        cout << "3. Change Password\n";
        cout << "4. Logout\n";
        cout << "Enter your choice: ";

        while (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1: viewMarks(); break;
            case 2: reportIssue(); break;
            case 3: {
                string newPassword;
                cout << "Enter new password: ";
                cin >> newPassword;
                changePassword(newPassword);
                break;
            }
            case 4: cout << "Logging out...\n"; return;
            default: cout << "Invalid choice! Try again.\n";
        }
    } while (true);
}

void Student::reportIssue() {
    string subject, note;
    cout << "Enter the subject with incorrect marks: ";
    cin >> subject;

    cout << "Enter a note explaining the issue: ";
    cin.ignore();
    getline(cin, note);

    vector<string> issues = loadBatchIssues(batchName);
    issues.push_back(username + "," + subject + "," + note); // Format: username,subject,note
    saveBatchIssues(batchName, issues);
    cout << "Issue reported successfully.\n";
}

void Student::viewMarks() {
    vector<string> students = loadBatchData(batchName);
    if (students.empty()) {
        cout << "No marks available for your batch.\n";
        return;
    }

    bool found = false;
    for (const string& student : students) {
        string username, password, marksData;
        stringstream ss(student);
        getline(ss, username, ',');
        getline(ss, password, ',');

        if (username == this->username) {
            found = true;

            // Collect available semesters
            vector<string> semesters;
            stringstream ss_copy(student);
            getline(ss_copy, username, ',');  // Skip username
            getline(ss_copy, password, ','); // Skip password
            while (getline(ss_copy, marksData, ',')) {
                size_t firstColon = marksData.find(':');
                if (firstColon != string::npos) {
                    string semester = marksData.substr(0, firstColon);
                    if (find(semesters.begin(), semesters.end(), semester) == semesters.end()) {
                        semesters.push_back(semester);
                    }
                }
            }

            // Semester selection
            int choice;
            cout << "\nChoose an option:\n";
            cout << "0. View all semesters\n";
            for (size_t i = 0; i < semesters.size(); ++i) {
                cout << i + 1 << ". View " << semesters[i] << " marks\n";
            }
            cout << "Enter your choice: ";
            while (!(cin >> choice) || choice < 0 || choice > semesters.size()) {
                cout << "Invalid input. Please select a valid option: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            string selectedSemester;
            if (choice != 0) {
                selectedSemester = semesters[choice - 1];
            }

            // Display marks
            cout << "Marks for " << this->username << ":\n";
            map<string, vector<pair<string, pair<int, int>>>> semesterGrades;

            while (getline(ss, marksData, ',')) {
                size_t firstColon = marksData.find(':');
                if (firstColon != string::npos) {
                    string semester = marksData.substr(0, firstColon);
                    if (choice == 0 || semester == selectedSemester) {
                        size_t secondColon = marksData.find(':', firstColon + 1);
                        string subject = marksData.substr(firstColon + 1, secondColon - (firstColon + 1));
                        string scoreWithTotal = marksData.substr(secondColon + 1);

                        size_t slashPos = scoreWithTotal.find('/');
                        if (slashPos != string::npos) {
                            string scoreStr = scoreWithTotal.substr(0, slashPos);
                            string totalStr = scoreWithTotal.substr(slashPos + 1);
                            int obtainedMarks = stoi(scoreStr);
                            int totalMarks = stoi(totalStr);

                            semesterGrades[semester].push_back({subject, {obtainedMarks, totalMarks}});
                            string grade = ((totalMarks == 100 && obtainedMarks < 40) || 
                                            (totalMarks == 50 && obtainedMarks < 20)) ? "Fail" : "Pass";

                            cout << semester << ": " << subject << ": " << obtainedMarks << "/" 
                                 << totalMarks << " | Grade: " << grade << endl;
                        }
                    }
                }
            }

            // GPA calculation and display
            cout << formatResult(choice == 0 ? semesterGrades 
                                             : map<string, vector<pair<string, pair<int, int>>>>{{selectedSemester, semesterGrades[selectedSemester]}});
            
            // Option to download marksheet
            char downloadChoice;
            cout << "\nDo you want to download your marksheet? (y/n): ";
            cin >> downloadChoice;
            if (downloadChoice == 'y' || downloadChoice == 'Y') {
                downloadMarksheet();
            }
            break;
        }
    }

    if (!found) {
        cout << "Student " << this->username << " does not exist in the system.\n";
    }
}

void Student::downloadMarksheet() {
    // Load result statuses
    map<string, bool> resultStatuses = loadResultStatuses();

    // Check if results are declared for the student's batch
    if (!resultStatuses[batchName]) {
        cout << "Error: Results for your batch \"" << batchName << "\" are not yet declared. Please contact the Dean.\n";
        return;
    }

    // Proceed with the marksheet download if results are declared
    vector<string> students = loadBatchData(batchName);
    map<string, vector<pair<string, pair<int, int>>>> allSemesterGrades;

    for (const string& student : students) {
        stringstream ss(student);
        string storedUsername, storedPassword, marksData;
        getline(ss, storedUsername, ',');
        getline(ss, storedPassword, ',');

        if (storedUsername == username) {
            while (getline(ss, marksData, ',')) {
                size_t firstColon = marksData.find(':');
                if (firstColon != string::npos) {
                    string semester = marksData.substr(0, firstColon);
                    size_t secondColon = marksData.find(':', firstColon + 1);
                    string subject = marksData.substr(firstColon + 1, secondColon - (firstColon + 1));
                    string scoreWithTotal = marksData.substr(secondColon + 1);

                    size_t slashPos = scoreWithTotal.find('/');
                    if (slashPos != string::npos) {
                        int score = stoi(scoreWithTotal.substr(0, slashPos));
                        int total = stoi(scoreWithTotal.substr(slashPos + 1));
                        allSemesterGrades[semester].push_back({subject, {score, total}});
                    }
                }
            }
            break;
        }
    }

    string marksheetContent = formatResult(allSemesterGrades);
    ofstream outfile(username + "_marksheet.txt");
    if (outfile.is_open()) {
        outfile << marksheetContent;
        outfile.close();
        cout << "Marksheet downloaded successfully as " << username << "_marksheet.txt\n";
    } else {
        cerr << "Error downloading marksheet.\n";
    }
}

string Student::formatResult(const map<string, vector<pair<string, pair<int, int>>>>& semesterGrades) {
    stringstream output;
    time_t now = time(0);
    char* dt = ctime(&now);

    output << "+-------------------------------------------------+\n";
    output << "|           Student Result - " << username << "              |\n";
    output << "+-------------------------------------------------+\n";
    output << "| Date: " << dt;

    for (const auto& semesterPair : semesterGrades) {
        string semester = semesterPair.first;
        output << "| Semester: " << semester << "\n";
        output << "+-----------------------+---------+-------+\n";
        output << "| Subject             | Marks    | Grade |\n";
        output << "+-----------------------+---------+-------+\n";

        bool failedSemester = false;
        double totalGpaPoints = 0;

        for (const auto& gradePair : semesterPair.second) {
            string subject = gradePair.first;
            int obtained = gradePair.second.first;
            int total = gradePair.second.second;

            string grade = ((total == 100 && obtained < 40) || (total == 50 && obtained < 20)) ? "Fail" : "Pass";
            output << "| " << left << setw(20) << subject << " | " << setw(7) << obtained << "/" << total << " | " << setw(5) << grade << " |\n";

            if (grade == "Fail") {
                failedSemester = true;
            }
            totalGpaPoints += (double)obtained / total * 4.0;
        }

        output << "+-----------------------+---------+-------+\n";
        output << "| Semester Result:      | " << (failedSemester ? "Failed" : "Passed") << " |\n";
        if (!failedSemester) {
            double gpa = totalGpaPoints / semesterPair.second.size();
            output << "| Semester GPA:         | " << fixed << setprecision(2) << gpa << " |\n";
        }
        output << "+-------------------------------------------------+\n";
    }
    return output.str();
}

