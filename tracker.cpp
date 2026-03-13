/*
 * Mental Health Case Tracking System
 * ITM Skills University - Case Study 31
 * Student : Ashish SK Gupta
 * Roll No : 150096725084
 *
 * Compile : g++ mental_health_tracker.cpp -o tracker
 * Run     : ./tracker
 */

#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;

// ── Simple password check ─────────────────────────────────────────────────────
// We store passwords as plain text in users.txt
// Format of each line: username,password
// Example: ashish,mypass123

// ── Show a menu heading ───────────────────────────────────────────────────────
void showHeading(string title) {
    cout << "\n============================" << endl;
    cout << "  " << title << endl;
    cout << "============================" << endl;
}

// ─────────────────────────────────────────────────────────────────────────────
//  REGISTER a new user
// ─────────────────────────────────────────────────────────────────────────────
void registerUser() {
    showHeading("REGISTER");

    string username, password;
    cout << "Enter a username : ";
    cin >> username;
    cout << "Enter a password : ";
    cin >> password;

    // First check if username already exists
    ifstream checkFile("users.txt");
    string line;
    while (getline(checkFile, line)) {
        // Each line is like: ashish,mypass123
        // Find the comma position
        int commaPos = line.find(",");
        string savedUser = line.substr(0, commaPos);
        if (savedUser == username) {
            cout << "\n  [!] Username already taken. Try another.\n";
            checkFile.close();
            return;
        }
    }
    checkFile.close();

    // Save the new user to users.txt
    ofstream userFile("users.txt", ios::app);
    userFile << username << "," << password << endl;
    userFile.close();

    cout << "\n  [+] Registered successfully! You can now login.\n";
}

// ─────────────────────────────────────────────────────────────────────────────
//  LOGIN
// ─────────────────────────────────────────────────────────────────────────────
string loginUser() {
    showHeading("LOGIN");

    string username, password;
    cout << "Enter username : ";
    cin >> username;
    cout << "Enter password : ";
    cin >> password;

    ifstream userFile("users.txt");
    string line;
    while (getline(userFile, line)) {
        int commaPos = line.find(",");
        string savedUser = line.substr(0, commaPos);
        string savedPass = line.substr(commaPos + 1);

        if (savedUser == username && savedPass == password) {
            cout << "\n  [+] Welcome, " << username << "!\n";
            userFile.close();
            return username;
        }
    }
    userFile.close();

    cout << "\n  [!] Wrong username or password.\n";
    return "";
}

// ─────────────────────────────────────────────────────────────────────────────
//  SHOW ALL CASES
// ─────────────────────────────────────────────────────────────────────────────
void showCases() {
    ifstream caseFile("cases.txt");

    if (!caseFile) {
        cout << "\n  No cases registered yet.\n";
        return;
    }

    cout << "\n--- All Cases ---\n";
    cout << "ID       Patient    Age   Concern            Severity   Status\n";
    cout << "--------------------------------------------------------------\n";

    string line;
    while (getline(caseFile, line)) {
        // Each line: C0001,R.S.,25,Anxiety,High,Active
        // Split by comma and print each field in its column
        stringstream ss(line);
        string id, patient, age, concern, severity, status;
        getline(ss, id,       ',');
        getline(ss, patient,  ',');
        getline(ss, age,      ',');
        getline(ss, concern,  ',');
        getline(ss, severity, ',');
        getline(ss, status,   ',');
        cout << left
             << setw(9)  << id
             << setw(11) << patient
             << setw(6)  << age
             << setw(19) << concern
             << setw(11) << severity
             << status
             << "\n";
    }
    caseFile.close();
}

// ─────────────────────────────────────────────────────────────────────────────
//  ADD A NEW CASE
// ─────────────────────────────────────────────────────────────────────────────
void addCase() {
    showHeading("REGISTER NEW CASE");

    // Count existing cases to make an ID
    int count = 0;
    ifstream countFile("cases.txt");
    string line;
    while (getline(countFile, line)) {
        count++;
    }
    countFile.close();

    // Make case ID like C0001, C0002 etc
    count = count + 1;
    string caseID = "C000" + to_string(count);

    string name, age, concern, severity;
    cout << "Patient initials       : ";
    cin >> name;
    cout << "Age                    : ";
    cin >> age;
    cout << "Primary concern        : ";
    cin >> concern;
    cout << "Severity (Low/Med/High): ";
    cin >> severity;

    // Save to cases.txt
    ofstream caseFile("cases.txt", ios::app);
    caseFile << caseID << "," << name << "," << age << ","
             << concern << "," << severity << ",Active" << endl;
    caseFile.close();

    cout << "\n  [+] Case saved! Case ID is: " << caseID << "\n";
}

// ─────────────────────────────────────────────────────────────────────────────
//  UPDATE CASE STATUS
// ─────────────────────────────────────────────────────────────────────────────
void updateStatus() {
    showHeading("UPDATE CASE STATUS");
    showCases();

    cout << "\nEnter Case ID to update (e.g. C0001) : ";
    string targetID;
    cin >> targetID;

    // Read all lines into an array
    string lines[100];
    int total = 0;
    ifstream caseFile("cases.txt");
    while (getline(caseFile, lines[total])) {
        total++;
    }
    caseFile.close();

    bool found = false;
    for (int i = 0; i < total; i++) {
        // Check if this line starts with the target ID
        if (lines[i].substr(0, targetID.length()) == targetID) {
            cout << "New status (Active / Closed / On-Hold) : ";
            string newStatus;
            cin >> newStatus;

            // Find the last comma and replace everything after it
            int lastComma = lines[i].rfind(",");
            lines[i] = lines[i].substr(0, lastComma + 1) + newStatus;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "\n  [!] Case ID not found.\n";
        return;
    }

    // Write all lines back to file
    ofstream outFile("cases.txt");
    for (int i = 0; i < total; i++) {
        outFile << lines[i] << endl;
    }
    outFile.close();

    cout << "\n  [+] Status updated!\n";
}

// ─────────────────────────────────────────────────────────────────────────────
//  ADD A SESSION
// ─────────────────────────────────────────────────────────────────────────────
void addSession() {
    showHeading("ADD SESSION");
    showCases();

    cout << "\nEnter Case ID for this session : ";
    string caseID;
    cin >> caseID;

    // Check if that case exists
    bool caseFound = false;
    ifstream caseFile("cases.txt");
    string line;
    while (getline(caseFile, line)) {
        if (line.substr(0, caseID.length()) == caseID) {
            caseFound = true;
            break;
        }
    }
    caseFile.close();

    if (!caseFound) {
        cout << "\n  [!] Case ID not found.\n";
        return;
    }

    // Count sessions to make session ID
    int count = 0;
    ifstream countFile("sessions.txt");
    string tempLine;
    while (getline(countFile, tempLine)) {
        count++;
    }
    countFile.close();

    count = count + 1;
    string sessionID = "S000" + to_string(count);

    string notes, duration;
    cout << "Session notes          : ";
    cin >> notes;
    cout << "Duration in minutes    : ";
    cin >> duration;

    ofstream sessionFile("sessions.txt", ios::app);
    sessionFile << sessionID << "," << caseID << "," << duration << "," << notes << endl;
    sessionFile.close();

    cout << "\n  [+] Session saved! Session ID is: " << sessionID << "\n";
}

// ─────────────────────────────────────────────────────────────────────────────
//  VIEW SESSIONS
// ─────────────────────────────────────────────────────────────────────────────
void viewSessions() {
    showHeading("SESSION HISTORY");

    ifstream sessionFile("sessions.txt");
    if (!sessionFile) {
        cout << "\n  No sessions recorded yet.\n";
        return;
    }

    cout << "\nFilter by Case ID (or press Enter to see all) : ";
    cin.ignore();
    string filter;
    getline(cin, filter);

    cout << "\nSess ID    Case ID    Mins    Notes\n";
    cout << "----------------------------------------------\n";

    string line;
    bool found = false;
    while (getline(sessionFile, line)) {
        if (filter == "" || line.find(filter) != string::npos) {
            cout << line << endl;
            found = true;
        }
    }
    sessionFile.close();

    if (!found) {
        cout << "  No sessions found.\n";
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  DASHBOARD (shown after login)
// ─────────────────────────────────────────────────────────────────────────────
void dashboard(string username) {
    int choice;
    do {
        showHeading("DASHBOARD  [" + username + "]");
        cout << "  1. Register New Case" << endl;
        cout << "  2. View All Cases" << endl;
        cout << "  3. Update Case Status" << endl;
        cout << "  4. Add Session" << endl;
        cout << "  5. View Sessions" << endl;
        cout << "  6. Logout" << endl;
        cout << "\n  Select: ";
        cin >> choice;

        if      (choice == 1) addCase();
        else if (choice == 2) showCases();
        else if (choice == 3) updateStatus();
        else if (choice == 4) addSession();
        else if (choice == 5) viewSessions();
        else if (choice == 6) cout << "\n  Goodbye, " << username << "!\n";
        else cout << "\n  [!] Invalid option. Try again.\n";

    } while (choice != 6);
}

// ─────────────────────────────────────────────────────────────────────────────
//  MAIN - program starts here
// ─────────────────────────────────────────────────────────────────────────────
int main() {
    int choice;
    do {
        showHeading("MENTAL HEALTH CASE TRACKING SYSTEM");
        cout << "  1. Login" << endl;
        cout << "  2. Register" << endl;
        cout << "  3. Exit" << endl;
        cout << "\n  Select: ";
        cin >> choice;

        if (choice == 1) {
            string loggedInUser = loginUser();
            if (loggedInUser != "") {
                dashboard(loggedInUser);
            }
        }
        else if (choice == 2) {
            registerUser();
        }
        else if (choice == 3) {
            cout << "\n  Bye!\n";
        }
        else {
            cout << "\n  [!] Invalid option.\n";
        }

    } while (choice != 3);

    return 0;
}