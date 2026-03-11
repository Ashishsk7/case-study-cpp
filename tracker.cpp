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
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include <algorithm>
using namespace std;

// ── Password hashing (FNV-1a 64-bit) ─────────────────────────────────────────
string hashPwd(const string &s) {
    unsigned long long h = 14695981039346656037ULL;
    for (char c : s) { h ^= (unsigned char)c; h *= 1099511628211ULL; }
    ostringstream ss; ss << hex << setw(16) << setfill('0') << h;
    return ss.str();
}

// ── Current date-time ─────────────────────────────────────────────────────────
string now() {
    time_t t = time(nullptr);
    char buf[20]; strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", localtime(&t));
    return buf;
}

// ── File helpers ──────────────────────────────────────────────────────────────
vector<string> readFile(const string &path) {
    vector<string> lines; ifstream f(path); string line;
    while (getline(f, line)) if (!line.empty()) lines.push_back(line);
    return lines;
}
void writeFile(const string &path, const vector<string> &lines) {
    ofstream f(path); for (const string &l : lines) f << l << "\n";
}
void appendLine(const string &path, const string &line) {
    ofstream f(path, ios::app); f << line << "\n";
}
vector<string> split(const string &s) {
    vector<string> v; stringstream ss(s); string t;
    while (getline(ss, t, '|')) v.push_back(t);
    return v;
}

// ── ALL input goes through here — consistent, no buffer bugs ──────────────────
string ask(const string &prompt) {
    cout << prompt; string s; getline(cin, s); return s;
}
void heading(const string &s) {
    cout << "\n" << string(55,'=') << "\n  " << s << "\n" << string(55,'=') << "\n";
}

// ─────────────────────────────────────────────────────────────────────────────
//  AUTH
// ─────────────────────────────────────────────────────────────────────────────
void seedAdmin() {
    ifstream f("users.dat"); if (f.good()) return;
    appendLine("users.dat", "admin|" + hashPwd("admin123") + "|admin");
    cout << "  [INFO] Default account -> username: admin  password: admin123\n";
}

string doLogin() {
    heading("LOGIN");
    string user = ask("  Username : ");
    string pwd  = ask("  Password : ");
    for (auto &rec : readFile("users.dat")) {
        auto f = split(rec);
        if (f.size() >= 2 && f[0] == user && f[1] == hashPwd(pwd))
            { cout << "\n  Welcome, " << user << "!\n"; return user; }
    }
    cout << "\n  [!] Wrong username or password.\n"; return "";
}

void doRegister() {
    heading("REGISTER");
    string user = ask("  Choose username : ");
    for (auto &rec : readFile("users.dat"))
        if (split(rec)[0] == user) { cout << "  [!] Username taken.\n"; return; }
    string pwd = ask("  Choose password : ");
    appendLine("users.dat", user + "|" + hashPwd(pwd) + "|counsellor");
    cout << "  [+] Registered! You can now login.\n";
}

// ─────────────────────────────────────────────────────────────────────────────
//  CASES
// ─────────────────────────────────────────────────────────────────────────────
void showCases() {
    auto recs = readFile("cases.dat");
    if (recs.empty()) { cout << "  No cases registered yet.\n"; return; }
    cout << "\n" << left << setw(7)<<"ID" << setw(12)<<"Patient"
         << setw(5)<<"Age" << setw(20)<<"Concern" << setw(8)<<"Severity" << "Status\n";
    cout << string(55,'-') << "\n";
    for (auto &r : recs) {
        auto f = split(r);
        if (f.size() < 7) continue;
        cout << left << setw(7)<<f[0] << setw(12)<<f[1] << setw(5)<<f[2]
             << setw(20)<<f[3] << setw(8)<<f[4] << f[6] << "\n";
    }
}

void addCase() {
    heading("REGISTER CASE");
    ostringstream id;
    id << "C" << setw(4) << setfill('0') << ((int)readFile("cases.dat").size() + 1);
    string name     = ask("  Patient initials        : ");
    string age      = ask("  Age                     : ");
    string concern  = ask("  Primary concern         : ");
    string severity = ask("  Severity (Low/Med/High) : ");
    appendLine("cases.dat", id.str()+"|"+name+"|"+age+"|"+concern+"|"+severity+"|"+now()+"|Active");
    cout << "\n  [+] Case registered with ID: " << id.str() << "\n";
}

void updateStatus() {
    heading("UPDATE CASE STATUS");
    showCases();
    string id = ask("\n  Case ID to update : ");
    transform(id.begin(), id.end(), id.begin(), ::toupper);
    auto recs = readFile("cases.dat"); bool found = false;
    for (auto &r : recs) {
        auto f = split(r);
        if (f.size() >= 7 && f[0] == id) {
            string ns = ask("  New status (Active/Closed/On-Hold) : ");
            r = f[0]+"|"+f[1]+"|"+f[2]+"|"+f[3]+"|"+f[4]+"|"+f[5]+"|"+ns;
            found = true; break;
        }
    }
    if (!found) { cout << "  [!] Case not found.\n"; return; }
    writeFile("cases.dat", recs); cout << "  [+] Status updated.\n";
}

// ─────────────────────────────────────────────────────────────────────────────
//  SESSIONS
// ─────────────────────────────────────────────────────────────────────────────
bool caseExists(const string &id) {
    for (auto &r : readFile("cases.dat")) if (!split(r).empty() && split(r)[0] == id) return true;
    return false;
}

void addSession() {
    heading("ADD SESSION");
    showCases();
    string id = ask("\n  Case ID : ");
    transform(id.begin(), id.end(), id.begin(), ::toupper);
    if (!caseExists(id)) { cout << "  [!] Case not found.\n"; return; }
    ostringstream sid;
    sid << "S" << setw(5) << setfill('0') << ((int)readFile("sessions.dat").size() + 1);
    string notes = ask("  Notes            : ");
    string dur   = ask("  Duration (mins)  : ");
    appendLine("sessions.dat", sid.str()+"|"+id+"|"+now()+"|"+dur+"|"+notes);
    cout << "\n  [+] Session saved: " << sid.str() << "\n";
}

void viewSessions() {
    heading("SESSION HISTORY");
    auto recs = readFile("sessions.dat");
    if (recs.empty()) { cout << "  No sessions yet.\n"; return; }
    string filter = ask("  Filter by Case ID (Enter = show all) : ");
    transform(filter.begin(), filter.end(), filter.begin(), ::toupper);
    cout << "\n" << left << setw(9)<<"Sess ID" << setw(9)<<"Case ID"
         << setw(18)<<"Date" << setw(7)<<"Mins" << "Notes\n";
    cout << string(55,'-') << "\n";
    for (auto &r : recs) {
        auto f = split(r);
        if (f.size() < 5) continue;
        if (!filter.empty() && f[1] != filter) continue;
        cout << left << setw(9)<<f[0] << setw(9)<<f[1]
             << setw(18)<<f[2] << setw(7)<<f[3] << f[4] << "\n";
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  MENUS
// ─────────────────────────────────────────────────────────────────────────────
void dashboard(const string &user) {
    while (true) {
        heading("DASHBOARD  [" + user + "]");
        cout << "  1. Register New Case\n"
             << "  2. View All Cases\n"
             << "  3. Update Case Status\n"
             << "  4. Add Session\n"
             << "  5. View Sessions\n"
             << "  6. Logout\n";
        string ch = ask("\n  Select: ");
        if      (ch == "1") addCase();
        else if (ch == "2") showCases();
        else if (ch == "3") updateStatus();
        else if (ch == "4") addSession();
        else if (ch == "5") viewSessions();
        else if (ch == "6") { cout << "  Goodbye, " << user << "!\n"; break; }
        else cout << "  [!] Invalid option.\n";
    }
}

int main() {
    seedAdmin();
    while (true) {
        heading("MENTAL HEALTH CASE TRACKING SYSTEM");
        cout << "  1. Login\n  2. Register\n  3. Exit\n";
        string ch = ask("\n  Select: ");
        if      (ch == "1") { string u = doLogin(); if (!u.empty()) dashboard(u); }
        else if (ch == "2") doRegister();
        else if (ch == "3") { cout << "  Bye!\n"; break; }
        else cout << "  [!] Invalid option.\n";
    }
    return 0;
}
