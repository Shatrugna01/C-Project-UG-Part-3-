#include "Teacher.h"

class Teacher;

// Constructor
Teacher::Teacher(const string& uname, const string& pass, bool isPD)
    : User(uname, pass), isProgramDirector(isPD) {}

// Dashboard
void Teacher::dashboard() {
    int choice;
    do {
        cout << "\n--- " << (isProgramDirector ? "Program Director and Teacher" : "Teacher") << " Dashboard ---\n";
        cout << "1. Add Marks to Batch\n";
        cout << "2. Resolve Student Issues\n";
        cout << "3. View Assigned Subjects\n";
        if (isProgramDirector) {
            cout << "4. Assign Teachers to Batches and Subjects\n";
            cout << "5. View Assigned and Unassigned Teachers\n";
            cout << "6. View Assigned Batches\n";
            cout << "7. Generate Batch Report\n";
        }
        cout << "8. Submit Marks to Program Director\n";
        cout << "9. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addMarksToBatch(); break;
            case 2: resolveStudentIssues(); break;
            case 3: viewAssignedSubjects(); break;
            case 4:
                if (isProgramDirector) {
                    string teacherUsername, batchName, subject, semester;
                    cout << "Enter teacher username: ";
                    cin >> teacherUsername;
                    cout << "Enter batch name: ";
                    cin >> batchName;
                    cout << "Enter subject: ";
                    cin >> subject;
                    cout << "Enter semester: ";
                    cin >> semester;
                    assignTeacherToBatch(teacherUsername, batchName, subject, semester);
                }
                break;
            case 5:
                if (isProgramDirector) viewAssignedAndUnassignedTeachers();
                break;
            case 6:
                if (isProgramDirector) viewAssignedBatches();
                break;
            case 7:
                if (isProgramDirector) {
                    string batchName;
                    cout << "Enter batch name to generate report: ";
                    cin >> batchName;
                    generateBatchReport(batchName);
                }
                break;
            case 8: submitMarksToProgramDirector(); break;
            case 9: cout << "Logging out...\n"; return;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (true);
}

void Teacher::addMarksToBatch() {
    string batchName, subject, semester;
    int totalMarks;

    // Get batch name, subject, and semester from the teacher
    cout << "Enter batch name: ";
    cin >> batchName;
    cout << "Enter Subject: ";
    cin >> subject;
    cout << "Enter Semester: ";
    cin >> semester;

    // Check if the teacher is assigned to this batch and subject
    if (find(assignedBatches.begin(), assignedBatches.end(), batchName) == assignedBatches.end() || 
        batchSubjects[batchName] != subject) {
        cout << "Error: You are not assigned to this batch or subject.\n";
        return;
    }

    // Get total marks for the subject
    cout << "Enter total marks for " << subject << ": ";
    while (!(cin >> totalMarks) || totalMarks <= 0) {
        cout << "Invalid input. Please enter a positive integer for total marks: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Load the batch data (list of students)
    vector<string> students = loadBatchData(batchName);
    if (students.empty()) {
        cout << "Batch does not exist or no students found in batch.\n";
        return;
    }


    // Open the batch file to append the marks for each student
    ofstream batchFile(batchName + ".txt", ios::app); // Open in append mode
    if (!batchFile.is_open()) {
        cerr << "Error: Unable to open file for writing marks.\n";
        return;
    }

    // Loop through each student and ask for their marks
    for (string& student : students) {
        string username, password;
        stringstream ss(student);
        getline(ss, username, ',');
        getline(ss, password, ',');

        int marks;
        cout << "Enter marks for " << username << " in " << subject << " (" << semester << ") (out of " << totalMarks << "): ";

        while (!(cin >> marks) || marks < 0 || marks > totalMarks) {
            cout << "Invalid input. Marks should be between 0 and " << totalMarks << ": ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        // Append marks to the student record
        student += (student.empty() ? "" : ",") + semester + ":" + subject + ":" + to_string(marks) + "/" + to_string(totalMarks);
    }

    // Save the updated student data back to the batch file
    saveBatchData(batchName, students);
    cout << "Marks updated successfully for batch " << batchName << ".\n";

    // Exit the loop once all students' marks have been entered
}




// Assign Teacher to Batch
void Teacher::assignTeacherToBatch(const string& teacherUsername, const string& batchName, const string& subject, const string& semester) {
    // Validate teacher existence
    if (teachers.find(teacherUsername) == teachers.end()) {
    	cout << "Error: Teacher \"" << teacherUsername << "\" does not exist.\n";
    	cout << "Available teachers: ";
    	for (const auto& [username, teacher] : teachers) {
    	    cout << username << " ";
    		}
    	cout << "\n";
    	return;
	}


    // Assign the batch and subject to the teacher
    teachers[teacherUsername].assignBatchAndSubject(batchName, subject);
    cout << "Assigned " << teacherUsername << " to batch " << batchName
         << " for subject " << subject << " in semester " << semester << ".\n";

    // Persist the updated data to users.txt
    ofstream file("users.txt");
    if (!file) {
        cerr << "Error: Could not open users.txt for writing.\n";
        return;
    }

    for (const auto& [username, teacher] : teachers) {
        file << "Teacher," << username << "," << teacher.getPassword() << ",";
        file << (teacher.getIsProgramDirector() ? "PD" : "Teacher");
        for (const auto& [batch, subject] : teacher.getBatchSubjects()) {
            file << "," << batch << ":" << subject;
        }
        file << "\n";
    }

    file.close();
}

void Teacher::assignBatchAndSubject(const string& batchName, const string& subject) {
    if (find(assignedBatches.begin(), assignedBatches.end(), batchName) == assignedBatches.end()) {
        assignedBatches.push_back(batchName); // Add batch to the list
    }
    batchSubjects[batchName] = subject; // Assign subject to batch
}


// View Assigned Subjects
void Teacher::viewAssignedSubjects() const {
    cout << "\n--- Assigned Subjects ---\n";
    for (const auto& [batch, subject] : batchSubjects) {
        cout << "Batch: " << batch << ", Subject: " << subject << "\n";
    }
    if (batchSubjects.empty()) {
        cout << "You have no assigned subjects.\n";
    }
}

// Submit Marks to Program Director
void Teacher::submitMarksToProgramDirector() {
    string batchName;
    cout << "Enter batch name to submit marks: ";
    cin >> batchName;

    // Validate if the teacher is assigned to this batch
    if (find(assignedBatches.begin(), assignedBatches.end(), batchName) == assignedBatches.end()) {
        cout << "Error: You are not assigned to this batch.\n";
        return;
    }

    // Submit marks (e.g., save submission status)
    cout << "Marks for batch " << batchName << " have been submitted to the Program Director.\n";
    batchSubjects.erase(batchName); // Remove assignment after submission
}

void Teacher::resolveStudentIssues() {
    string batchName;
    cout << "Enter batch name: ";
    cin >> batchName;

    // Load the issues for the batch
    vector<string> issues = loadBatchIssues(batchName);
    if (issues.empty()) {
        cout << "No issues reported for this batch.\n";
        return;
    }

    // Display the list of issues
    for (size_t i = 0; i < issues.size(); ++i) {
        cout << i + 1 << ". " << issues[i] << "\n";
    }

    // Ask the teacher to select an issue to resolve
    int issueIndex;
    cout << "Select issue to resolve (or 0 to skip): ";
    cin >> issueIndex;

    if (issueIndex > 0 && issueIndex <= issues.size()) {
        // Find the student associated with this issue
        string selectedIssue = issues[issueIndex - 1];
        stringstream ss(selectedIssue);
        string studentUsername, subject, semester;
        int originalMarks, totalMarks;

        // Parsing the issue string (assuming format: studentUsername,subject,semester,originalMarks,totalMarks)
        getline(ss, studentUsername, ',');
        getline(ss, subject, ',');
        getline(ss, semester, ',');
        ss >> originalMarks;
        ss.ignore();  // Ignore the comma separating originalMarks and totalMarks
        ss >> totalMarks;

        cout << "Resolved issue for " << studentUsername << " in " << subject << " (" << semester << ").\n";
        cout << "Original marks: " << originalMarks << " out of " << totalMarks << ".\n";

        // Ask for the updated marks
        int updatedMarks;
        cout << "Enter updated marks for " << studentUsername << " in " << subject << ": ";
        cin >> updatedMarks;

        // Check for valid marks
        if (updatedMarks < 0 || updatedMarks > totalMarks) {
            cout << "Error: Invalid marks. Marks should be between 0 and " << totalMarks << ".\n";
            return;
        }

        // Load the batch data (list of students)
        vector<string> students = loadBatchData(batchName);
        bool studentFound = false;

        // Iterate over students and find the matching student
        for (string& student : students) {
            stringstream studentStream(student);
            string username, password, studentSemester, studentSubject;
            int currentMarks, currentTotalMarks;

            getline(studentStream, username, ',');
            getline(studentStream, password, ',');
            getline(studentStream, studentSemester, ',');
            getline(studentStream, studentSubject, ',');
            studentStream >> currentMarks;
            studentStream.ignore();  // Ignore the comma separating currentMarks and currentTotalMarks
            studentStream >> currentTotalMarks;

            // If this is the correct student and subject, update the marks
            if (username == studentUsername && studentSubject == subject && studentSemester == semester) {
                studentStream.str(""); // Clear stringstream
                studentStream.clear(); // Clear flags
                studentStream << username << "," << password << "," << studentSemester << "," << studentSubject
                              << "," << updatedMarks << "," << totalMarks;
                student = studentStream.str(); // Update the student data
                studentFound = true;
                break;
            }
        }

        if (studentFound) {
            // Save the updated student data back to the batch file
            saveBatchData(batchName, students);
            cout << "Marks updated successfully for " << studentUsername << " in " << subject << ".\n";
        } else {
            cout << "Error: Student not found in batch.\n";
        }
    }
}


void Teacher::viewAssignedAndUnassignedTeachers() {
    cout << "\n--- Assigned Teachers ---\n";

    bool foundAssigned = false; // Track if any assigned teachers are found

    for (const auto& [teacherName, teacherObj] : teachers) {
        if (!teacherObj.getBatchSubjects().empty()) { // Check if the teacher has assigned subjects
            foundAssigned = true;
            cout << "- Teacher: " << teacherName << "\n";
            for (const auto& [batch, subject] : teacherObj.getBatchSubjects()) {
                cout << "  Batch: " << batch << ", Subject: " << subject << "\n";
            }
        }
    }

    if (!foundAssigned) {
        cout << "No assigned teachers found.\n";
    }

    cout << "\n--- Unassigned Teachers ---\n";
    bool foundUnassigned = false; // Track if any unassigned teachers are found

    for (const auto& [teacherName, teacherObj] : teachers) {
        if (teacherObj.getBatchSubjects().empty()) { // Check if the teacher has no assigned subjects
            foundUnassigned = true;
            cout << "- Teacher: " << teacherName << "\n";
        }
    }

    if (!foundUnassigned) {
        cout << "No unassigned teachers found.\n";
    }
}

void Teacher::viewAssignedBatches() const {
    cout << "\n--- Assigned Batches ---\n";

    bool hasAssignedBatches = false;  // Track if any batches are found
    string line;
    vector<string> assignedBatches;  // Vector to store batches for the Program Director

    // Open Program Directors.txt to check which batches are assigned to this Program Director
    ifstream file("program_directors.txt");
    if (!file.is_open()) {
        cerr << "Error: Unable to open Program Directors.txt.\n";
        return;
    }

    // Read through each line in the file
    while (getline(file, line)) {
        stringstream ss(line);
        string batchName, pdName;

        // Read the batch name and Program Director name from the line
        getline(ss, batchName, ',');
        getline(ss, pdName);

        // Check if this Program Director matches the current teacher's username
        if (pdName == username) {
            hasAssignedBatches = true;
            assignedBatches.push_back(batchName);  // Add batch to the list of assigned batches
        }
    }

    // If batches are found for the Program Director, display them
    if (hasAssignedBatches) {
        cout << "Program Director: " << username << "\n";
        cout << "Assigned Batches: ";
        for (const auto& batch : assignedBatches) {
            cout << batch << " ";
        }
        cout << "\n";
    } else {
        cout << "You have no assigned batches.\n";
    }

    file.close();  // Close the file after reading
}

void Teacher::generateBatchReport(const string& batchName) {
    // Check if the current teacher is a Program Director (PD) and assigned to the batch
    ifstream pdFile("program_directors.txt");
    if (!pdFile.is_open()) {
        cerr << "Error: Unable to open Program Directors.txt.\n";
        return;
    }

    bool isProgramDirector = false;
    string line;
    while (getline(pdFile, line)) {
        stringstream ss(line);
        string batch, pdName;

        getline(ss, batch, ',');
        getline(ss, pdName);

        // Check if this batch is assigned to the current teacher
        if (batch == batchName && pdName == username) {
            isProgramDirector = true;
            break;
        }
    }

    pdFile.close();  // Close the file after checking

    if (!isProgramDirector) {
        cout << "Error: You are not the Program Director for batch \"" << batchName << "\".\n";
        return;
    }

    // Load batch data from batchName.txt (student marks)
    vector<string> students = loadBatchData(batchName);
    if (students.empty()) {
        cout << "Error: No data found for batch \"" << batchName << "\".\n";
        return;
    }

    int totalPassed = 0, totalFailed = 0;
    double totalGPA = 0.0;
    int studentCount = 0;

    cout << "\n--- Batch Report: " << batchName << " ---\n";
    for (const string& studentData : students) {
        stringstream ss(studentData);
        string username, password, marksData;
        getline(ss, username, ',');
        getline(ss, password, ',');

        bool hasFailed = false;
        double studentGPA = 0.0;
        int subjectCount = 0;

        while (getline(ss, marksData, ',')) {
            size_t firstColon = marksData.find(':');
            size_t slashPos = marksData.find('/');
            if (firstColon != string::npos && slashPos != string::npos) {
                string subject = marksData.substr(0, firstColon);
                string marksStr = marksData.substr(firstColon + 1, slashPos - (firstColon + 1));
                string totalMarksStr = marksData.substr(slashPos + 1);

                // Attempt to convert marks and total marks to integers
                try {
                    int marks = stoi(marksStr);
                    int totalMarks = stoi(totalMarksStr);

                    // Ensure valid marks and totalMarks
                    if (marks < 0 || totalMarks <= 0 || marks > totalMarks) {
                        cout << "Invalid marks or totalMarks for " << subject << " in student " << username << ". Skipping.\n";
                        continue;  // Skip this subject if data is invalid
                    }

                    studentGPA += static_cast<double>(marks) / totalMarks * 4.0; // GPA calculation
                    subjectCount++;

                    if (marks < 40) { // Fail threshold
                        hasFailed = true;
                    }
                } catch (const std::invalid_argument& e) {
                    cout << "Error: Invalid mark data for " << username << " in subject " << marksData << ". Skipping.\n";
                    continue;  // Skip this subject if conversion fails
                }
            }
        }

        if (subjectCount > 0) {
            studentGPA /= subjectCount; // Average GPA
        }

        cout << "Student: " << username << ", GPA: " << studentGPA << ", Result: "
             << (hasFailed ? "Fail" : "Pass") << "\n";

        if (hasFailed) {
            totalFailed++;
        } else {
            totalPassed++;
        }

        totalGPA += studentGPA;
        studentCount++;
    }

    if (studentCount > 0) {
        totalGPA /= studentCount;
    }

    cout << "\nSummary:\n";
    cout << "Total Students: " << studentCount << "\n";
    cout << "Passed: " << totalPassed << "\n";
    cout << "Failed: " << totalFailed << "\n";
    cout << "Average GPA: " << totalGPA << "\n";

    // Save the report for the Dean in batchName_report.txt
    ofstream reportFile(batchName + "_report.txt");
    if (reportFile.is_open()) {
        reportFile << "Batch: " << batchName << "\n";
        reportFile << "Total Students: " << studentCount << "\n";
        reportFile << "Passed: " << totalPassed << "\n";
        reportFile << "Failed: " << totalFailed << "\n";
        reportFile << "Average GPA: " << totalGPA << "\n";
        reportFile.close();
        cout << "Batch report saved successfully.\n";
    } else {
        cerr << "Error: Failed to save the batch report.\n";
    }
}

