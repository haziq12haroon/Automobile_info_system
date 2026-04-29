 AutoShield Insurance Management System

A console-based vehicle insurance management system written in C++11. It handles the full insurance lifecycle — from customer onboarding and policy issuance, through claim filing, surveyor inspections, and workshop repairs — with no external database required. All data is persisted to plain text files.

---

 Features

- Role-based access control with four distinct user roles
- Full insurance workflow: customers → vehicles → policies → claims → inspections → repairs
- File-backed persistence using a generic templated repository — no DBMS needed
- Input validation throughout, with safe integer and double parsing
- Auto-generated IDs, policy numbers (`POL-{id}-{year}`), and claim numbers (`CLM-{id}-{year}`)
- First-run setup: default admin account is seeded automatically on first launch

---

 Roles

| Role | Description |
|------|-------------|
| `ADMIN` | Manages staff, customers, vehicles, workshops, policies, claims, and generates reports |
| `SALESMAN` | Registers customers and vehicles, issues policies, and files claims |
| `SURVEYOR` | Schedules inspections, submits findings, and sets repair cost estimates |
| `WORKSHOP_MGR` | Logs repair jobs and marks them complete for their assigned workshop |

---

 Workflow

```
Customer registered → Vehicle registered → Policy issued
        ↓
   Claim filed (PENDING)
        ↓
   Inspection scheduled (UNDER_REVIEW)
        ↓
   Inspection report submitted (COMPLETED)
        ↓
   Admin approves claim (APPROVED)
        ↓
   Workshop manager logs repair (IN_PROGRESS)
        ↓
   Repair completed (COMPLETED)
```

---

 Project Structure

```
project.cpp          # Entire application — single-file C++11
data/                # Created automatically on first run
  staff.dat
  customers.dat
  vehicles.dat
  workshops.dat
  policies.dat
  claims.dat
  inspections.dat
  repairs.dat
```

All `.dat` files use a pipe-delimited (`|`) format and are managed by the generic `FileRepo<T>` template class.

---

 Getting Started

 Prerequisites

- A C++11 (or later) compiler: `g++`, `clang++`, or MSVC

 Build

Linux / macOS**
```bash
g++ -std=c++11 -o autoshield project.cpp
./autoshield
```

Windows (MinGW)**
```bash
g++ -std=c++11 -o autoshield.exe project.cpp
autoshield.exe
```

Windows (MSVC)**
```bash
cl /EHsc /std:c++11 project.cpp /Fe:autoshield.exe
autoshield.exe
```

 First Login

On the very first run, a default admin account is created automatically:

```
Username: admin
Password: admin123
```

> Change this password after first login.**

---

 Data Model

| Entity | Key Fields |
|--------|-----------|
| `Staff` | id, name, role, username, password, phone, email, workshopId |
| `Customer` | id, name, CNIC, phone, email, address, registrationDate |
| `Vehicle` | id, customerId, regNo, make, model, year, color, chassisNo, engineNo |
| `Workshop` | id, name, address, phone, ownerName, managerId |
| `InsurancePolicy` | id, customerId, vehicleId, salesmanId, policyNo, type, premium, coverage, start/end dates, status |
| `Claim` | id, policyId, customerId, vehicleId, claimNo, incidentDate, filedDate, description, estimatedLoss, status |
| `Inspection` | id, claimId, surveyorId, workshopId, date, findings, repairCost, recommendation, status |
| `Repair` | id, claimId, workshopId, inspectionId, workshopMgrId, startDate, endDate, repairCost, status, notes |

---

 Architecture

The codebase is organized into logical sections within a single file:

| Section | Contents |
|---------|----------|
| 1 | Utility helpers (date, I/O, string parsing) |
| 2 | Entity classes (`Staff`, `Customer`, `Vehicle`, `Workshop`, `InsurancePolicy`, `Claim`, `Inspection`, `Repair`) |
| 3 | Generic `FileRepo<T>` template (add, update, remove, findById, loadAll) |
| 4 | `DB` struct aggregating all repositories and cross-entity queries |
| 5 | Business service functions (register, issue, file, schedule, submit, log, complete) |
| 6 | Report functions |
| 7–8 | Role-specific menu handlers (`adminMenu`, `salesmanMenu`, `surveyorMenu`, `workshopMgrMenu`) |
| 9 | Login screen with 3-attempt lockout |
| 10 | `main()` entry point |

---

 Status Values

Policy: `ACTIVE` · `EXPIRED` · `CANCELLED`

Claim: `PENDING` → `UNDER_REVIEW` → `APPROVED` / `REJECTED`

Inspection: `SCHEDULED` → `COMPLETED`

Repair: `IN_PROGRESS` → `COMPLETED`

---

Notes

- Passwords are stored in "plain text" in the `.dat` files. This is suitable for academic/demo use only — do not use in production without adding proper password hashing.
- The `data/` directory and all `.dat` files are created automatically. Do not edit them manually while the program is running.
- The `FileRepo<T>` template does a full file read/write on every operation, which is fine for small datasets typical of a course project.

---

License

This project is released for educational purposes. Feel free to fork and extend it.
