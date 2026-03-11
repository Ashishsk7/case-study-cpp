# 🧠 Mental Health Case Tracking System
**ITM Skills University — Case Study 31**
**Student:** Ashish SK Gupta | **Roll No:** 150096725084

---

## 📌 About
A command-line application built in **C++** to securely manage mental health patient records. Supports user registration/login, case management, and session tracking — all saved locally with no external dependencies.

---

## ⚙️ How to Run

```bash
g++ mental_health_tracker.cpp -o tracker
./tracker
```
> On Windows: `tracker.exe`

---

## 🔐 Features
- **Register & Login** — Create your own username and password. Passwords are hashed (FNV-1a) before saving — never stored as plain text.
- **Case Registration** — Add patient cases with unique IDs (C0001, C0002...), severity level, and status tracking.
- **Session Tracking** — Log therapy sessions linked to a case, with date, duration, and notes.
- **Persistent Storage** — All data saved in `.dat` files. Your data stays between runs.
- **Update Case Status** — Change any case to Active / Closed / On-Hold anytime.

---

## 🗂️ File Structure
```
mental_health_tracker.cpp   ← main source code
users.dat                   ← created on first run (stores accounts)
cases.dat                   ← created when you add a case
sessions.dat                ← created when you add a session
```

---

## 🖥️ Program Flow
```
Launch → Register (pick any username + password)
       → Login with those credentials
       → Dashboard:
           1. Register New Case
           2. View All Cases
           3. Update Case Status
           4. Add Session
           5. View Sessions
           6. Logout
```

---

## 🛠️ Built With
- C++ (Standard Library only — no installs needed)
- Headers: `iostream` `fstream` `sstream` `iomanip` `ctime` `algorithm`

---

## 📄 Docs
- [`Mental_Health_Tracker_Report.pdf`](./Mental_Health_Tracker_Report.pdf) — Full case study report
- [`Mental_Health_Tracker_PPT.pdf`](./Mental_Health_Tracker_PPT.pdf) — Presentation slides
