# University Marks Management System (UMMS)

A comprehensive web-based application to manage academic marks and result workflows for a university. This system simulates the official Goa University marks process with role-based access for students, faculty, program directors, deans, registrars, and the controller of examinations.

---

## 🔧 Features

### 🧑‍🎓 Student
- Login with university email and password.
- View subject-wise marks and SGPA/CGPA.
- Raise issues on marks (optional).
- Check result status after final declaration.

### 👩‍🏫 Teacher
1. Log in using employee number and password.
2. Select School → Discipline → Program → Semester → Subject.
3. Enter internal/external marks for students.
4. Review and resolve issues raised by students.
5. Submit final marks to Program Director.

### 🧑‍💼 Program Director
1. Log in and select relevant program.
2. Review submitted marks by faculty.
3. Calculate SGPA (current semester) and CGPA (cumulative).
4. Approve and declare results for the program.
5. Optionally review unresolved student issues.

### 👨‍💼 Dean
1. Log in and choose school & program.
2. View and review marks submitted by Program Directors.
3. Submit consolidated results to Controller of Examinations for final declaration and printing.

### 🧑‍💻 Registrar & Controller of Examinations (COE)
- Manage all records and final results.
- Print results after dean approval.
- Perform quality checks and verification.
- Lock and archive finalized results.

### 👨‍💼 Admin
- Register multiple students at once using loop.
- Automatically assign default password: `university123`.
- Edit student list before final save.
- Create program, subject, and batch structures.

---

## 🏫 Academic Structure

- **School > Discipline > Program > Semester > Subject**
- Each program (e.g., MSc Data Science 2024) is batch-linked.
- Subjects are tied to specific semesters and programs.

---

## 💡 Tech Stack

- C++

---

