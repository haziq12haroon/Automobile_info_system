#include "Menus.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <stdexcept>

// ═══════════════════════════════════════════
//  ConsoleUI helpers
// ═══════════════════════════════════════════
void ConsoleUI::printLine(char c, int len) {
    std::cout << std::string(len, c) << "\n";
}

void ConsoleUI::printHeader(const std::string& title) {
    printLine('=');
    int pad = (60 - (int)title.size()) / 2;
    if (pad > 0) std::cout << std::string(pad, ' ');
    std::cout << title << "\n";
    printLine('=');
}

int ConsoleUI::readInt(const std::string& prompt) {
    int val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) { std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return val; }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  Invalid input. Enter a number.\n";
    }
}

double ConsoleUI::readDouble(const std::string& prompt) {
    double val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) { std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return val; }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  Invalid input. Enter a number.\n";
    }
}

std::string ConsoleUI::readString(const std::string& prompt) {
    std::cout << prompt;
    std::string val;
    std::getline(std::cin, val);
    return val;
}

void ConsoleUI::pause() {
    std::cout << "\nPress ENTER to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void ConsoleUI::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// ═══════════════════════════════════════════
//  Application constructor
// ═══════════════════════════════════════════
Application::Application()
    : m_customerRepo("data/customers.dat"),
      m_vehicleRepo("data/vehicles.dat"),
      m_staffRepo("data/staff.dat"),
      m_workshopRepo("data/workshops.dat"),
      m_policyRepo("data/policies.dat"),
      m_claimRepo("data/claims.dat"),
      m_inspRepo("data/inspections.dat"),
      m_repairRepo("data/repairs.dat"),
      m_authService(m_staffRepo),
      m_customerService(m_customerRepo),
      m_vehicleService(m_vehicleRepo),
      m_policyService(m_policyRepo, m_customerRepo, m_vehicleRepo),
      m_claimService(m_claimRepo, m_policyRepo),
      m_inspService(m_inspRepo, m_claimRepo, m_workshopRepo),
      m_repairService(m_repairRepo, m_workshopRepo, m_claimRepo),
      m_staffService(m_staffRepo),
      m_workshopService(m_workshopRepo),
      m_reportService(m_customerRepo, m_policyRepo, m_claimRepo,
                      m_inspRepo, m_vehicleRepo, m_staffRepo, m_repairRepo)
{
    // Seed default admin if no staff exists
    if (m_staffRepo.findAll().empty()) {
        m_staffService.addStaff("System Manager", "MANAGER", "admin", "admin123",
                                "0300-0000000", "admin@insurance.com");
        std::cout << "[SYSTEM] Default manager created: admin / admin123\n";
    }
}

// ═══════════════════════════════════════════
//  run
// ═══════════════════════════════════════════
void Application::run() {
    ConsoleUI::printHeader("AUTO INSURANCE MANAGEMENT SYSTEM");
    std::cout << "  Welcome to AutoShield Insurance Co.\n\n";

    while (true) {
        if (!loginScreen()) {
            std::cout << "Exiting system. Goodbye!\n";
            return;
        }
        std::string role = m_currentUser.getRole();
        if (role == "MANAGER")  managerMenu();
        else if (role == "SALESMAN")  salesmanMenu();
        else if (role == "SURVEYOR")  surveyorMenu();
        else std::cout << "Unknown role. Contact administrator.\n";
    }
}

// ─── Login ─────────────────────────────────
bool Application::loginScreen() {
    ConsoleUI::printHeader("LOGIN");
    for (int attempt = 0; attempt < 3; ++attempt) {
        std::string user = ConsoleUI::readString("  Username (or 'quit' to exit): ");
        if (user == "quit") return false;
        std::string pass = ConsoleUI::readString("  Password: ");
        Staff s = m_authService.login(user, pass);
        if (s.getId() != 0) {
            m_currentUser = s;
            std::cout << "\n  Login successful! Welcome, " << s.getName()
                      << " (" << s.getRole() << ")\n\n";
            return true;
        }
        std::cout << "  Invalid credentials. Try again.\n\n";
    }
    std::cout << "  Too many failed attempts.\n";
    return false;
}

// ═══════════════════════════════════════════
//  MANAGER MENU
// ═══════════════════════════════════════════
void Application::managerMenu() {
    int choice;
    do {
        ConsoleUI::printHeader("MANAGER MENU");
        std::cout << "  1. Customer Management\n"
                  << "  2. Policy Management\n"
                  << "  3. Claim Management\n"
                  << "  4. Reports\n"
                  << "  5. Staff Management\n"
                  << "  6. Workshop Management\n"
                  << "  7. Repair Management\n"
                  << "  0. Logout\n";
        choice = ConsoleUI::readInt("  Choice: ");
        switch (choice) {
            case 1: managerCustomerMenu(); break;
            case 2: managerPolicyMenu();   break;
            case 3: managerClaimMenu();    break;
            case 4: managerReportMenu();   break;
            case 5: managerStaffMenu();    break;
            case 6: managerWorkshopMenu(); break;
            case 7: managerRepairMenu();   break;
            case 0: break;
            default: std::cout << "  Invalid choice.\n";
        }
    } while (choice != 0);
}

// ─── Manager: Customer ─────────────────────
void Application::managerCustomerMenu() {
    int choice;
    do {
        ConsoleUI::printHeader("CUSTOMER MANAGEMENT");
        std::cout << "  1. View All Customers\n"
                  << "  2. View Customer Details\n"
                  << "  3. Search by CNIC\n"
                  << "  0. Back\n";
        choice = ConsoleUI::readInt("  Choice: ");
        if (choice == 1) viewAllCustomers();
        else if (choice == 2) viewCustomerDetails();
        else if (choice == 3) {
            std::string cnic = ConsoleUI::readString("  Enter CNIC: ");
            Customer c = m_customerService.getByCnic(cnic);
            if (c.getId() == 0) std::cout << "  Customer not found.\n";
            else {
                ConsoleUI::printLine();
                std::cout << "  ID      : " << c.getId() << "\n"
                          << "  Name    : " << c.getName() << "\n"
                          << "  CNIC    : " << c.getCnic() << "\n"
                          << "  Phone   : " << c.getPhone() << "\n"
                          << "  Email   : " << c.getEmail() << "\n"
                          << "  Address : " << c.getAddress() << "\n"
                          << "  Reg Date: " << c.getRegDate() << "\n";
            }
            ConsoleUI::pause();
        }
    } while (choice != 0);
}

// ─── Manager: Policy ───────────────────────
void Application::managerPolicyMenu() {
    int choice;
    do {
        ConsoleUI::printHeader("POLICY MANAGEMENT");
        std::cout << "  1. View All Policies\n"
                  << "  2. View Policies by Customer\n"
                  << "  3. Cancel Policy\n"
                  << "  0. Back\n";
        choice = ConsoleUI::readInt("  Choice: ");
        if (choice == 1) viewAllPolicies();
        else if (choice == 2) {
            int cid = ConsoleUI::readInt("  Customer ID: ");
            auto policies = m_policyService.getByCustomer(cid);
            if (policies.empty()) std::cout << "  No policies found.\n";
            else {
                for (const auto& p : policies)
                    std::cout << "  [" << p.getId() << "] " << p.getPolicyNo()
                              << " | " << p.getType() << " | " << p.getStatus()
                              << " | Premium: " << p.getPremiumAmount()
                              << " | Coverage: " << p.getCoverageAmount() << "\n";
            }
            ConsoleUI::pause();
        }
        else if (choice == 3) {
            int pid = ConsoleUI::readInt("  Policy ID to cancel: ");
            if (m_policyService.cancelPolicy(pid)) std::cout << "  Policy cancelled.\n";
            else std::cout << "  Policy not found.\n";
            ConsoleUI::pause();
        }
    } while (choice != 0);
}

// ─── Manager: Claims ───────────────────────
void Application::managerClaimMenu() {
    int choice;
    do {
        ConsoleUI::printHeader("CLAIM MANAGEMENT");
        std::cout << "  1. View All Claims\n"
                  << "  2. View Pending Claims\n"
                  << "  3. Approve Claim\n"
                  << "  4. Reject Claim\n"
                  << "  5. View Claims by Customer\n"
                  << "  6. Schedule Inspection\n"
                  << "  0. Back\n";
        choice = ConsoleUI::readInt("  Choice: ");

        if (choice == 1) viewAllClaims();
        else if (choice == 2) {
            auto claims = m_claimService.getPendingClaims();
            std::cout << "\n  Pending Claims: " << claims.size() << "\n";
            for (const auto& c : claims)
                std::cout << "  [" << c.getId() << "] " << c.getClaimNo()
                          << " | Filed: " << c.getFiledDate()
                          << " | Est Loss: PKR " << c.getEstimatedLoss() << "\n";
            ConsoleUI::pause();
        }
        else if (choice == 3) {
            int cid = ConsoleUI::readInt("  Claim ID to APPROVE: ");
            // Check inspection report exists
            auto insps = m_inspService.getByClaim(cid);
            bool hasCompleted = false;
            for (const auto& i : insps) if (i.getStatus() == "COMPLETED") hasCompleted = true;
            if (!hasCompleted) {
                std::cout << "  No completed inspection report found. Cannot approve yet.\n";
            } else if (m_claimService.approveClaim(cid)) {
                std::cout << "  Claim APPROVED successfully.\n";
            } else std::cout << "  Claim not found.\n";
            ConsoleUI::pause();
        }
        else if (choice == 4) {
            int cid = ConsoleUI::readInt("  Claim ID to REJECT: ");
            if (m_claimService.rejectClaim(cid)) std::cout << "  Claim REJECTED.\n";
            else std::cout << "  Claim not found.\n";
            ConsoleUI::pause();
        }
        else if (choice == 5) {
            int custId = ConsoleUI::readInt("  Customer ID: ");
            auto claims = m_claimService.getByCustomer(custId);
            if (claims.empty()) std::cout << "  No claims found.\n";
            else {
                for (const auto& c : claims)
                    std::cout << "  [" << c.getId() << "] " << c.getClaimNo()
                              << " | " << c.getStatus()
                              << " | Loss: " << c.getEstimatedLoss() << "\n";
            }
            ConsoleUI::pause();
        }
        else if (choice == 6) {
            int claimId   = ConsoleUI::readInt("  Claim ID: ");
            // Show surveyors
            auto surveyors = m_staffService.getSurveyors();
            std::cout << "  Available Surveyors:\n";
            for (const auto& s : surveyors)
                std::cout << "    [" << s.getId() << "] " << s.getName() << "\n";
            int survId    = ConsoleUI::readInt("  Surveyor ID: ");
            // Show workshops
            viewAllWorkshops();
            int wId       = ConsoleUI::readInt("  Workshop ID: ");
            std::string date = ConsoleUI::readString("  Inspection Date (YYYY-MM-DD): ");
            try {
                Inspection insp = m_inspService.scheduleInspection(claimId, survId, wId, date);
                std::cout << "  Inspection scheduled. ID=" << insp.getId() << "\n";
            } catch (const std::exception& e) {
                std::cout << "  Error: " << e.what() << "\n";
            }
            ConsoleUI::pause();
        }
    } while (choice != 0);
}

// ─── Manager: Reports ──────────────────────
void Application::managerReportMenu() {
    int choice;
    do {
        ConsoleUI::printHeader("REPORTS");
        std::cout << "  1. New Customers in a Month\n"
                  << "  2. Pending Claims Report\n"
                  << "  3. Inspection Report (by Claim)\n"
                  << "  4. Customer Claims History\n"
                  << "  5. All Policies Report\n"
                  << "  6. Workshop Repairs Report\n"
                  << "  0. Back\n";
        choice = ConsoleUI::readInt("  Choice: ");

        if (choice == 1) {
            std::string ym = ConsoleUI::readString("  Enter Year-Month (YYYY-MM): ");
            std::cout << m_reportService.newCustomersReport(ym);
            ConsoleUI::pause();
        } else if (choice == 2) {
            std::cout << m_reportService.pendingClaimsReport();
            ConsoleUI::pause();
        } else if (choice == 3) {
            int claimId = ConsoleUI::readInt("  Claim ID: ");
            std::cout << m_reportService.inspectionReport(claimId);
            ConsoleUI::pause();
        } else if (choice == 4) {
            int custId = ConsoleUI::readInt("  Customer ID: ");
            std::cout << m_reportService.customerClaimsReport(custId);
            ConsoleUI::pause();
        } else if (choice == 5) {
            std::cout << m_reportService.allPoliciesReport();
            ConsoleUI::pause();
        } else if (choice == 6) {
            viewAllWorkshops();
            int wId = ConsoleUI::readInt("  Workshop ID: ");
            std::cout << m_reportService.workshopRepairsReport(wId, m_workshopRepo);
            ConsoleUI::pause();
        }
    } while (choice != 0);
}

// ─── Manager: Staff ────────────────────────
void Application::managerStaffMenu() {
    int choice;
    do {
        ConsoleUI::printHeader("STAFF MANAGEMENT");
        std::cout << "  1. View All Staff\n"
                  << "  2. Add Staff\n"
                  << "  3. View Salesmen\n"
                  << "  4. View Surveyors\n"
                  << "  0. Back\n";
        choice = ConsoleUI::readInt("  Choice: ");

        if (choice == 1) viewAllStaff();
        else if (choice == 2) {
            std::cout << "\n  Roles: MANAGER | SALESMAN | SURVEYOR\n";
            std::string name  = ConsoleUI::readString("  Name: ");
            std::string role  = ConsoleUI::readString("  Role: ");
            std::string uname = ConsoleUI::readString("  Username: ");
            std::string pass  = ConsoleUI::readString("  Password: ");
            std::string phone = ConsoleUI::readString("  Phone: ");
            std::string email = ConsoleUI::readString("  Email: ");
            try {
                Staff s = m_staffService.addStaff(name, role, uname, pass, phone, email);
                std::cout << "  Staff added. ID=" << s.getId() << "\n";
            } catch (const std::exception& e) {
                std::cout << "  Error: " << e.what() << "\n";
            }
            ConsoleUI::pause();
        }
        else if (choice == 3) {
            auto salesmen = m_staffService.getSalesmen();
            std::cout << "\n  Salesmen:\n";
            for (const auto& s : salesmen)
                std::cout << "  [" << s.getId() << "] " << s.getName() << " | " << s.getPhone() << "\n";
            ConsoleUI::pause();
        }
        else if (choice == 4) {
            auto surveyors = m_staffService.getSurveyors();
            std::cout << "\n  Surveyors:\n";
            for (const auto& s : surveyors)
                std::cout << "  [" << s.getId() << "] " << s.getName() << " | " << s.getPhone() << "\n";
            ConsoleUI::pause();
        }
    } while (choice != 0);
}

// ─── Manager: Workshops ────────────────────
void Application::managerWorkshopMenu() {
    int choice;
    do {
        ConsoleUI::printHeader("WORKSHOP MANAGEMENT");
        std::cout << "  1. View All Workshops\n"
                  << "  2. Add Workshop\n"
                  << "  3. Remove Workshop\n"
                  << "  0. Back\n";
        choice = ConsoleUI::readInt("  Choice: ");

        if (choice == 1) viewAllWorkshops();
        else if (choice == 2) {
            std::string name  = ConsoleUI::readString("  Workshop Name: ");
            std::string addr  = ConsoleUI::readString("  Address: ");
            std::string phone = ConsoleUI::readString("  Phone: ");
            std::string owner = ConsoleUI::readString("  Owner Name: ");
            Workshop w = m_workshopService.addWorkshop(name, addr, phone, owner);
            std::cout << "  Workshop added. ID=" << w.getId() << "\n";
            ConsoleUI::pause();
        }
        else if (choice == 3) {
            int wId = ConsoleUI::readInt("  Workshop ID to remove: ");
            if (m_workshopService.removeWorkshop(wId)) std::cout << "  Workshop removed.\n";
            else std::cout << "  Not found.\n";
            ConsoleUI::pause();
        }
    } while (choice != 0);
}

// ─── Manager: Repairs ──────────────────────
void Application::managerRepairMenu() {
    int choice;
    do {
        ConsoleUI::printHeader("REPAIR MANAGEMENT");
        std::cout << "  1. View All Repairs\n"
                  << "  2. Assign Repair to Workshop\n"
                  << "  3. Mark Repair as Complete\n"
                  << "  0. Back\n";
        choice = ConsoleUI::readInt("  Choice: ");

        if (choice == 1) {
            auto repairs = m_repairService.getAllRepairs();
            std::cout << "\n  Total Repairs: " << repairs.size() << "\n";
            for (const auto& r : repairs)
                std::cout << "  [" << r.getId() << "] Claim:" << r.getClaimId()
                          << " | Workshop:" << r.getWorkshopId()
                          << " | " << r.getStatus()
                          << " | Cost: " << r.getRepairCost() << "\n";
            ConsoleUI::pause();
        }
        else if (choice == 2) {
            int claimId   = ConsoleUI::readInt("  Claim ID: ");
            int inspId    = ConsoleUI::readInt("  Inspection ID: ");
            viewAllWorkshops();
            int wId       = ConsoleUI::readInt("  Workshop ID: ");
            std::string date = ConsoleUI::readString("  Start Date (YYYY-MM-DD): ");
            std::string notes = ConsoleUI::readString("  Notes: ");
            try {
                Repair r = m_repairService.assignRepair(claimId, wId, inspId, date, notes);
                std::cout << "  Repair assigned. ID=" << r.getId() << "\n";
            } catch (const std::exception& e) {
                std::cout << "  Error: " << e.what() << "\n";
            }
            ConsoleUI::pause();
        }
        else if (choice == 3) {
            int rId = ConsoleUI::readInt("  Repair ID: ");
            std::string endDate = ConsoleUI::readString("  End Date (YYYY-MM-DD): ");
            double cost = ConsoleUI::readDouble("  Final Cost (PKR): ");
            if (m_repairService.completeRepair(rId, endDate, cost))
                std::cout << "  Repair marked complete.\n";
            else std::cout << "  Repair not found.\n";
            ConsoleUI::pause();
        }
    } while (choice != 0);
}

// ═══════════════════════════════════════════
//  SALESMAN MENU
// ═══════════════════════════════════════════
void Application::salesmanMenu() {
    int choice;
    do {
        ConsoleUI::printHeader("SALESMAN MENU");
        std::cout << "  Logged in as: " << m_currentUser.getName() << "\n\n"
                  << "  1. Customer Management\n"
                  << "  2. Vehicle Management\n"
                  << "  3. Policy Management\n"
                  << "  4. File a Claim\n"
                  << "  0. Logout\n";
        choice = ConsoleUI::readInt("  Choice: ");
        switch (choice) {
            case 1: salesmanCustomerMenu(); break;
            case 2: salesmanVehicleMenu();  break;
            case 3: salesmanPolicyMenu();   break;
            case 4: {
                ConsoleUI::printHeader("FILE A CLAIM");
                int custId = ConsoleUI::readInt("  Customer ID: ");
                auto policies = m_policyService.getByCustomer(custId);
                if (policies.empty()) {
                    std::cout << "  No policies found for this customer.\n";
                    ConsoleUI::pause(); break;
                }
                std::cout << "  Policies:\n";
                for (const auto& p : policies)
                    std::cout << "    [" << p.getId() << "] " << p.getPolicyNo()
                              << " | " << p.getType() << " | " << p.getStatus() << "\n";
                int polId = ConsoleUI::readInt("  Policy ID: ");
                InsurancePolicy pol = m_policyService.getById(polId);
                if (pol.getId() == 0) { std::cout << "  Policy not found.\n"; ConsoleUI::pause(); break; }
                std::string incDate = ConsoleUI::readString("  Incident Date (YYYY-MM-DD): ");
                std::string desc    = ConsoleUI::readString("  Incident Description: ");
                double loss = ConsoleUI::readDouble("  Estimated Loss (PKR): ");
                try {
                    Claim c = m_claimService.fileClaim(polId, pol.getCustomerId(),
                                                        pol.getVehicleId(), incDate, desc, loss);
                    std::cout << "  Claim filed. No=" << c.getClaimNo() << "  ID=" << c.getId() << "\n";
                } catch (const std::exception& e) {
                    std::cout << "  Error: " << e.what() << "\n";
                }
                ConsoleUI::pause(); break;
            }
            case 0: break;
            default: std::cout << "  Invalid choice.\n";
        }
    } while (choice != 0);
}

void Application::salesmanCustomerMenu() {
    int choice;
    do {
        ConsoleUI::printHeader("CUSTOMER MANAGEMENT");
        std::cout << "  1. Register New Customer\n"
                  << "  2. View All Customers\n"
                  << "  3. Search Customer by CNIC\n"
                  << "  4. Update Customer Info\n"
                  << "  0. Back\n";
        choice = ConsoleUI::readInt("  Choice: ");

        if (choice == 1) {
            std::string name  = ConsoleUI::readString("  Full Name: ");
            std::string cnic  = ConsoleUI::readString("  CNIC (XXXXX-XXXXXXX-X): ");
            std::string phone = ConsoleUI::readString("  Phone: ");
            std::string email = ConsoleUI::readString("  Email: ");
            std::string addr  = ConsoleUI::readString("  Address: ");
            try {
                Customer c = m_customerService.registerCustomer(name, cnic, phone, email, addr);
                std::cout << "  Customer registered. ID=" << c.getId() << "\n";
            } catch (const std::exception& e) {
                std::cout << "  Error: " << e.what() << "\n";
            }
            ConsoleUI::pause();
        }
        else if (choice == 2) viewAllCustomers();
        else if (choice == 3) {
            std::string cnic = ConsoleUI::readString("  CNIC: ");
            Customer c = m_customerService.getByCnic(cnic);
            if (c.getId() == 0) std::cout << "  Not found.\n";
            else std::cout << "  [" << c.getId() << "] " << c.getName()
                           << " | " << c.getPhone() << " | " << c.getEmail() << "\n";
            ConsoleUI::pause();
        }
        else if (choice == 4) {
            int id = ConsoleUI::readInt("  Customer ID: ");
            Customer c = m_customerService.getById(id);
            if (c.getId() == 0) { std::cout << "  Not found.\n"; ConsoleUI::pause(); continue; }
            std::string phone = ConsoleUI::readString("  New Phone (blank=keep): ");
            std::string email = ConsoleUI::readString("  New Email (blank=keep): ");
            std::string addr  = ConsoleUI::readString("  New Address (blank=keep): ");
            if (!phone.empty()) c.setPhone(phone);
            if (!email.empty()) c.setEmail(email);
            if (!addr.empty())  c.setAddress(addr);
            m_customerService.updateCustomer(c);
            std::cout << "  Customer updated.\n";
            ConsoleUI::pause();
        }
    } while (choice != 0);
}

void Application::salesmanVehicleMenu() {
    int choice;
    do {
        ConsoleUI::printHeader("VEHICLE MANAGEMENT");
        std::cout << "  1. Register Vehicle\n"
                  << "  2. View Vehicles by Customer\n"
                  << "  3. View All Vehicles\n"
                  << "  0. Back\n";
        choice = ConsoleUI::readInt("  Choice: ");

        if (choice == 1) {
            int custId    = ConsoleUI::readInt("  Customer ID: ");
            std::string regNo    = ConsoleUI::readString("  Registration Number: ");
            std::string make     = ConsoleUI::readString("  Make (e.g. Toyota): ");
            std::string model    = ConsoleUI::readString("  Model (e.g. Corolla): ");
            int year      = ConsoleUI::readInt("  Year: ");
            std::string color    = ConsoleUI::readString("  Color: ");
            std::string chassis  = ConsoleUI::readString("  Chassis No: ");
            std::string engine   = ConsoleUI::readString("  Engine No: ");
            try {
                Vehicle v = m_vehicleService.addVehicle(custId, regNo, make, model,
                                                         year, color, chassis, engine);
                std::cout << "  Vehicle registered. ID=" << v.getId() << "\n";
            } catch (const std::exception& e) {
                std::cout << "  Error: " << e.what() << "\n";
            }
            ConsoleUI::pause();
        }
        else if (choice == 2) {
            int custId = ConsoleUI::readInt("  Customer ID: ");
            auto vehicles = m_vehicleService.getByCustomer(custId);
            for (const auto& v : vehicles)
                std::cout << "  [" << v.getId() << "] " << v.getRegNo()
                          << " | " << v.getMake() << " " << v.getModel()
                          << " " << v.getYear() << "\n";
            ConsoleUI::pause();
        }
        else if (choice == 3) viewAllVehicles();
    } while (choice != 0);
}

void Application::salesmanPolicyMenu() {
    int choice;
    do {
        ConsoleUI::printHeader("POLICY MANAGEMENT");
        std::cout << "  1. Issue New Policy\n"
                  << "  2. View Customer Policies\n"
                  << "  3. View All Policies\n"
                  << "  0. Back\n";
        choice = ConsoleUI::readInt("  Choice: ");

        if (choice == 1) {
            int custId  = ConsoleUI::readInt("  Customer ID: ");
            // Show customer vehicles
            auto vehicles = m_vehicleService.getByCustomer(custId);
            if (vehicles.empty()) {
                std::cout << "  No vehicles registered for this customer.\n";
                ConsoleUI::pause(); continue;
            }
            std::cout << "  Vehicles:\n";
            for (const auto& v : vehicles)
                std::cout << "    [" << v.getId() << "] " << v.getRegNo()
                          << " " << v.getMake() << " " << v.getModel() << "\n";
            int vehId   = ConsoleUI::readInt("  Vehicle ID: ");
            std::cout << "  Policy Types: COMPREHENSIVE | THIRD_PARTY\n";
            std::string type  = ConsoleUI::readString("  Type: ");
            double premium    = ConsoleUI::readDouble("  Premium Amount (PKR/year): ");
            double coverage   = ConsoleUI::readDouble("  Coverage Amount (PKR): ");
            std::string start = ConsoleUI::readString("  Start Date (YYYY-MM-DD): ");
            std::string end   = ConsoleUI::readString("  End Date   (YYYY-MM-DD): ");
            try {
                InsurancePolicy p = m_policyService.issuePolicy(custId, vehId,
                                        m_currentUser.getId(), type, premium, coverage, start, end);
                std::cout << "  Policy issued. No=" << p.getPolicyNo() << "\n";
            } catch (const std::exception& e) {
                std::cout << "  Error: " << e.what() << "\n";
            }
            ConsoleUI::pause();
        }
        else if (choice == 2) {
            int custId = ConsoleUI::readInt("  Customer ID: ");
            auto policies = m_policyService.getByCustomer(custId);
            for (const auto& p : policies)
                std::cout << "  [" << p.getId() << "] " << p.getPolicyNo()
                          << " | " << p.getType() << " | " << p.getStatus()
                          << " | " << p.getStartDate() << " to " << p.getEndDate() << "\n";
            ConsoleUI::pause();
        }
        else if (choice == 3) viewAllPolicies();
    } while (choice != 0);
}

// ═══════════════════════════════════════════
//  SURVEYOR MENU
// ═══════════════════════════════════════════
void Application::surveyorMenu() {
    int choice;
    do {
        ConsoleUI::printHeader("SURVEYOR MENU");
        std::cout << "  Logged in as: " << m_currentUser.getName() << "\n\n"
                  << "  1. My Inspections\n"
                  << "  2. Submit Inspection Report\n"
                  << "  3. View Claim Details\n"
                  << "  0. Logout\n";
        choice = ConsoleUI::readInt("  Choice: ");
        switch (choice) {
            case 1: surveyorInspectionMenu(); break;
            case 2: {
                int inspId = ConsoleUI::readInt("  Inspection ID: ");
                std::string findings = ConsoleUI::readString("  Findings: ");
                double cost = ConsoleUI::readDouble("  Estimated Repair Cost (PKR): ");
                std::string rec = ConsoleUI::readString("  Recommendation (APPROVE/REJECT/PARTIAL): ");
                if (m_inspService.submitReport(inspId, findings, cost, rec))
                    std::cout << "  Report submitted successfully.\n";
                else
                    std::cout << "  Inspection not found.\n";
                ConsoleUI::pause();
                break;
            }
            case 3: surveyorClaimMenu(); break;
            case 0: break;
            default: std::cout << "  Invalid choice.\n";
        }
    } while (choice != 0);
}

void Application::surveyorInspectionMenu() {
    auto insps = m_inspService.getBySurveyor(m_currentUser.getId());
    ConsoleUI::printHeader("MY INSPECTIONS");
    if (insps.empty()) { std::cout << "  No inspections assigned.\n"; ConsoleUI::pause(); return; }
    for (const auto& i : insps)
        std::cout << "  [" << i.getId() << "] Claim:" << i.getClaimId()
                  << " | Date:" << i.getInspectionDate()
                  << " | Workshop:" << i.getWorkshopId()
                  << " | Status:" << i.getStatus() << "\n";
    ConsoleUI::pause();
}

void Application::surveyorClaimMenu() {
    int claimId = ConsoleUI::readInt("  Claim ID: ");
    Claim c = m_claimService.getById(claimId);
    if (c.getId() == 0) { std::cout << "  Claim not found.\n"; ConsoleUI::pause(); return; }
    ConsoleUI::printHeader("CLAIM DETAILS");
    std::cout << "  Claim No      : " << c.getClaimNo() << "\n"
              << "  Incident Date : " << c.getIncidentDate() << "\n"
              << "  Filed Date    : " << c.getFiledDate() << "\n"
              << "  Description   : " << c.getDescription() << "\n"
              << "  Est. Loss     : PKR " << c.getEstimatedLoss() << "\n"
              << "  Status        : " << c.getStatus() << "\n";
    ConsoleUI::pause();
}

// ─── Salesman also files claims ────────────
// ═══════════════════════════════════════════
//  Shared helpers
// ═══════════════════════════════════════════
void Application::viewAllCustomers() {
    auto customers = m_customerService.getAllCustomers();
    ConsoleUI::printHeader("ALL CUSTOMERS");
    std::cout << std::left << std::setw(6) << "ID" << std::setw(22) << "Name"
              << std::setw(18) << "CNIC" << std::setw(15) << "Phone" << "Reg.Date\n";
    ConsoleUI::printLine();
    for (const auto& c : customers)
        std::cout << std::setw(6) << c.getId() << std::setw(22) << c.getName()
                  << std::setw(18) << c.getCnic() << std::setw(15) << c.getPhone()
                  << c.getRegDate() << "\n";
    std::cout << "  Total: " << customers.size() << "\n";
    ConsoleUI::pause();
}

void Application::viewCustomerDetails() {
    int id = ConsoleUI::readInt("  Customer ID: ");
    Customer c = m_customerService.getById(id);
    if (c.getId() == 0) { std::cout << "  Not found.\n"; ConsoleUI::pause(); return; }
    ConsoleUI::printLine();
    std::cout << "  ID      : " << c.getId()      << "\n"
              << "  Name    : " << c.getName()    << "\n"
              << "  CNIC    : " << c.getCnic()    << "\n"
              << "  Phone   : " << c.getPhone()   << "\n"
              << "  Email   : " << c.getEmail()   << "\n"
              << "  Address : " << c.getAddress() << "\n"
              << "  Reg Date: " << c.getRegDate() << "\n";
    // Show their vehicles
    auto vehicles = m_vehicleService.getByCustomer(id);
    std::cout << "\n  Vehicles (" << vehicles.size() << "):\n";
    for (const auto& v : vehicles)
        std::cout << "    [" << v.getId() << "] " << v.getRegNo()
                  << " " << v.getMake() << " " << v.getModel() << "\n";
    ConsoleUI::pause();
}

void Application::viewAllVehicles() {
    auto vehicles = m_vehicleService.getAllVehicles();
    ConsoleUI::printHeader("ALL VEHICLES");
    std::cout << std::left << std::setw(6) << "ID" << std::setw(12) << "RegNo"
              << std::setw(12) << "Make" << std::setw(12) << "Model"
              << std::setw(6) << "Year" << "CustID\n";
    ConsoleUI::printLine();
    for (const auto& v : vehicles)
        std::cout << std::setw(6) << v.getId() << std::setw(12) << v.getRegNo()
                  << std::setw(12) << v.getMake() << std::setw(12) << v.getModel()
                  << std::setw(6) << v.getYear() << v.getCustomerId() << "\n";
    ConsoleUI::pause();
}

void Application::viewAllPolicies() {
    auto policies = m_policyService.getAllPolicies();
    ConsoleUI::printHeader("ALL POLICIES");
    std::cout << std::left << std::setw(6) << "ID" << std::setw(16) << "PolicyNo"
              << std::setw(14) << "Type" << std::setw(12) << "Status" << "CustID\n";
    ConsoleUI::printLine();
    for (const auto& p : policies)
        std::cout << std::setw(6) << p.getId() << std::setw(16) << p.getPolicyNo()
                  << std::setw(14) << p.getType() << std::setw(12) << p.getStatus()
                  << p.getCustomerId() << "\n";
    std::cout << "  Total: " << policies.size() << "\n";
    ConsoleUI::pause();
}

void Application::viewAllClaims() {
    auto claims = m_claimService.getAllClaims();
    ConsoleUI::printHeader("ALL CLAIMS");
    std::cout << std::left << std::setw(6) << "ID" << std::setw(14) << "ClaimNo"
              << std::setw(12) << "Filed" << std::setw(16) << "Status" << "EstLoss\n";
    ConsoleUI::printLine();
    for (const auto& c : claims)
        std::cout << std::setw(6) << c.getId() << std::setw(14) << c.getClaimNo()
                  << std::setw(12) << c.getFiledDate() << std::setw(16) << c.getStatus()
                  << c.getEstimatedLoss() << "\n";
    std::cout << "  Total: " << claims.size() << "\n";
    ConsoleUI::pause();
}

void Application::viewAllWorkshops() {
    auto workshops = m_workshopService.getAllWorkshops();
    ConsoleUI::printHeader("REGISTERED WORKSHOPS");
    std::cout << std::left << std::setw(6) << "ID" << std::setw(22) << "Name"
              << std::setw(14) << "Phone" << "Owner\n";
    ConsoleUI::printLine();
    for (const auto& w : workshops)
        std::cout << std::setw(6) << w.getId() << std::setw(22) << w.getName()
                  << std::setw(14) << w.getPhone() << w.getOwnerName() << "\n";
}

void Application::viewAllStaff() {
    auto staff = m_staffService.getAllStaff();
    ConsoleUI::printHeader("ALL STAFF");
    std::cout << std::left << std::setw(6) << "ID" << std::setw(20) << "Name"
              << std::setw(12) << "Role" << std::setw(16) << "Username" << "Phone\n";
    ConsoleUI::printLine();
    for (const auto& s : staff)
        std::cout << std::setw(6) << s.getId() << std::setw(20) << s.getName()
                  << std::setw(12) << s.getRole() << std::setw(16) << s.getUsername()
                  << s.getPhone() << "\n";
    ConsoleUI::pause();
}
