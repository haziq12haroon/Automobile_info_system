#include "Services.h"
#include <ctime>
#include <sstream>
#include <iomanip>
#include <stdexcept>

// ─── date helper ────────────────────────────
static std::string todayStr() {
    time_t now = time(nullptr);
    struct tm* t = localtime(&now);
    char buf[12];
    strftime(buf, sizeof(buf), "%Y-%m-%d", t);
    return std::string(buf);
}

// ═══════════════════════════════════════════
//  AuthService
// ═══════════════════════════════════════════
AuthService::AuthService(StaffRepository& staffRepo) : m_staffRepo(staffRepo) {}

Staff AuthService::login(const std::string& username, const std::string& password) const {
    Staff s = m_staffRepo.findByUsername(username);
    if (s.getId() == 0) return Staff();           // not found
    if (s.getPassword() != password) return Staff(); // wrong password
    return s;
}

bool AuthService::isValidRole(const Staff& s, const std::string& role) const {
    return s.getRole() == role;
}

// ═══════════════════════════════════════════
//  CustomerService
// ═══════════════════════════════════════════
CustomerService::CustomerService(CustomerRepository& repo) : m_customerRepo(repo) {}

Customer CustomerService::registerCustomer(const std::string& name, const std::string& cnic,
                                            const std::string& phone, const std::string& email,
                                            const std::string& address) {
    // Prevent duplicate CNIC
    Customer existing = m_customerRepo.findByCnic(cnic);
    if (existing.getId() != 0) throw std::runtime_error("Customer with this CNIC already exists.");
    int id = m_customerRepo.nextId();
    Customer c(id, name, cnic, phone, email, address, todayStr());
    m_customerRepo.save(c);
    return c;
}

bool CustomerService::updateCustomer(const Customer& c) {
    return m_customerRepo.update(c);
}

Customer CustomerService::getById(int id) const            { return m_customerRepo.findById(id); }
Customer CustomerService::getByCnic(const std::string& c) const { return m_customerRepo.findByCnic(c); }
std::vector<Customer> CustomerService::getAllCustomers() const { return m_customerRepo.findAll(); }

std::vector<Customer> CustomerService::getNewCustomersInMonth(const std::string& ym) const {
    return m_customerRepo.findByMonth(ym);
}

// ═══════════════════════════════════════════
//  VehicleService
// ═══════════════════════════════════════════
VehicleService::VehicleService(VehicleRepository& repo) : m_vehicleRepo(repo) {}

Vehicle VehicleService::addVehicle(int customerId, const std::string& regNo,
                                    const std::string& make, const std::string& model,
                                    int year, const std::string& color,
                                    const std::string& chassisNo, const std::string& engineNo) {
    // Check duplicate reg number
    Vehicle existing = m_vehicleRepo.findByRegNo(regNo);
    if (existing.getId() != 0) throw std::runtime_error("Vehicle with this registration number already exists.");
    int id = m_vehicleRepo.nextId();
    Vehicle v(id, customerId, regNo, make, model, year, color, chassisNo, engineNo);
    m_vehicleRepo.save(v);
    return v;
}

Vehicle VehicleService::getById(int id) const                    { return m_vehicleRepo.findById(id); }
Vehicle VehicleService::getByRegNo(const std::string& r) const   { return m_vehicleRepo.findByRegNo(r); }
std::vector<Vehicle> VehicleService::getByCustomer(int cid) const { return m_vehicleRepo.findByCustomer(cid); }
std::vector<Vehicle> VehicleService::getAllVehicles() const       { return m_vehicleRepo.findAll(); }

// ═══════════════════════════════════════════
//  PolicyService
// ═══════════════════════════════════════════
PolicyService::PolicyService(PolicyRepository& policyRepo, CustomerRepository& customerRepo,
                              VehicleRepository& vehicleRepo)
    : m_policyRepo(policyRepo), m_customerRepo(customerRepo), m_vehicleRepo(vehicleRepo) {}

std::string PolicyService::generatePolicyNo() const {
    return "POL-" + std::to_string(m_policyRepo.nextId()) + "-" + todayStr().substr(0,4);
}

InsurancePolicy PolicyService::issuePolicy(int customerId, int vehicleId, int salesmanId,
                                            const std::string& type, double premium,
                                            double coverage, const std::string& startDate,
                                            const std::string& endDate) {
    Customer c = m_customerRepo.findById(customerId);
    if (c.getId() == 0) throw std::runtime_error("Customer not found.");
    Vehicle v = m_vehicleRepo.findById(vehicleId);
    if (v.getId() == 0) throw std::runtime_error("Vehicle not found.");
    int id = m_policyRepo.nextId();
    std::string polNo = generatePolicyNo();
    InsurancePolicy p(id, customerId, vehicleId, salesmanId, polNo, type, premium, coverage,
                      startDate, endDate, "ACTIVE");
    m_policyRepo.save(p);
    return p;
}

bool PolicyService::cancelPolicy(int policyId) {
    InsurancePolicy p = m_policyRepo.findById(policyId);
    if (p.getId() == 0) return false;
    p.setStatus("CANCELLED");
    return m_policyRepo.update(p);
}

InsurancePolicy PolicyService::getById(int id) const               { return m_policyRepo.findById(id); }
InsurancePolicy PolicyService::getByPolicyNo(const std::string& n) const { return m_policyRepo.findByPolicyNo(n); }
std::vector<InsurancePolicy> PolicyService::getByCustomer(int cid) const { return m_policyRepo.findByCustomer(cid); }
std::vector<InsurancePolicy> PolicyService::getAllPolicies() const        { return m_policyRepo.findAll(); }

bool PolicyService::isPolicyActive(int policyId) const {
    InsurancePolicy p = m_policyRepo.findById(policyId);
    return p.getId() != 0 && p.getStatus() == "ACTIVE";
}

// ═══════════════════════════════════════════
//  ClaimService
// ═══════════════════════════════════════════
ClaimService::ClaimService(ClaimRepository& claimRepo, PolicyRepository& policyRepo)
    : m_claimRepo(claimRepo), m_policyRepo(policyRepo) {}

std::string ClaimService::generateClaimNo() const {
    return "CLM-" + std::to_string(m_claimRepo.nextId()) + "-" + todayStr().substr(0,4);
}

Claim ClaimService::fileClaim(int policyId, int customerId, int vehicleId,
                               const std::string& incidentDate, const std::string& description,
                               double estimatedLoss) {
    InsurancePolicy p = m_policyRepo.findById(policyId);
    if (p.getId() == 0) throw std::runtime_error("Policy not found.");
    if (p.getStatus() != "ACTIVE") throw std::runtime_error("Policy is not active.");
    int id = m_claimRepo.nextId();
    std::string claimNo = generateClaimNo();
    Claim c(id, policyId, customerId, vehicleId, claimNo, incidentDate,
            todayStr(), description, estimatedLoss, "PENDING");
    m_claimRepo.save(c);
    return c;
}

bool ClaimService::updateClaimStatus(int claimId, const std::string& status) {
    Claim c = m_claimRepo.findById(claimId);
    if (c.getId() == 0) return false;
    c.setStatus(status);
    return m_claimRepo.update(c);
}

Claim ClaimService::getById(int id) const                    { return m_claimRepo.findById(id); }
Claim ClaimService::getByClaimNo(const std::string& n) const { return m_claimRepo.findByClaimNo(n); }
std::vector<Claim> ClaimService::getByCustomer(int cid) const { return m_claimRepo.findByCustomer(cid); }
std::vector<Claim> ClaimService::getPendingClaims() const     { return m_claimRepo.findByStatus("PENDING"); }
std::vector<Claim> ClaimService::getAllClaims() const          { return m_claimRepo.findAll(); }

bool ClaimService::approveClaim(int claimId) { return updateClaimStatus(claimId, "APPROVED"); }
bool ClaimService::rejectClaim(int claimId)  { return updateClaimStatus(claimId, "REJECTED"); }

// ═══════════════════════════════════════════
//  InspectionService
// ═══════════════════════════════════════════
InspectionService::InspectionService(InspectionRepository& inspRepo, ClaimRepository& claimRepo,
                                      WorkshopRepository& workshopRepo)
    : m_inspRepo(inspRepo), m_claimRepo(claimRepo), m_workshopRepo(workshopRepo) {}

Inspection InspectionService::scheduleInspection(int claimId, int surveyorId, int workshopId,
                                                   const std::string& inspectionDate) {
    Claim c = m_claimRepo.findById(claimId);
    if (c.getId() == 0) throw std::runtime_error("Claim not found.");
    Workshop w = m_workshopRepo.findById(workshopId);
    if (w.getId() == 0) throw std::runtime_error("Workshop not found. Vehicle must be repaired at a registered workshop.");
    int id = m_inspRepo.nextId();
    Inspection insp(id, claimId, surveyorId, workshopId, inspectionDate, "", 0.0, "", "SCHEDULED");
    m_inspRepo.save(insp);
    // Move claim to UNDER_REVIEW
    c.setStatus("UNDER_REVIEW");
    m_claimRepo.update(c);
    return insp;
}

bool InspectionService::submitReport(int inspectionId, const std::string& findings,
                                      double repairCost, const std::string& recommendation) {
    Inspection insp = m_inspRepo.findById(inspectionId);
    if (insp.getId() == 0) return false;
    insp.setFindings(findings);
    insp.setRepairCost(repairCost);
    insp.setRecommendation(recommendation);
    insp.setStatus("COMPLETED");
    return m_inspRepo.update(insp);
}

Inspection InspectionService::getById(int id) const               { return m_inspRepo.findById(id); }
std::vector<Inspection> InspectionService::getByClaim(int cid) const { return m_inspRepo.findByClaim(cid); }
std::vector<Inspection> InspectionService::getBySurveyor(int sid) const { return m_inspRepo.findBySurveyor(sid); }
std::vector<Inspection> InspectionService::getAllInspections() const    { return m_inspRepo.findAll(); }

// ═══════════════════════════════════════════
//  RepairService
// ═══════════════════════════════════════════
RepairService::RepairService(RepairRepository& repairRepo, WorkshopRepository& workshopRepo,
                              ClaimRepository& claimRepo)
    : m_repairRepo(repairRepo), m_workshopRepo(workshopRepo), m_claimRepo(claimRepo) {}

Repair RepairService::assignRepair(int claimId, int workshopId, int inspectionId,
                                    const std::string& startDate, const std::string& notes) {
    Workshop w = m_workshopRepo.findById(workshopId);
    if (w.getId() == 0) throw std::runtime_error("Workshop not found. Must use a registered workshop.");
    int id = m_repairRepo.nextId();
    Repair r(id, claimId, workshopId, inspectionId, startDate, "", 0.0, "IN_PROGRESS", notes);
    m_repairRepo.save(r);
    return r;
}

bool RepairService::completeRepair(int repairId, const std::string& endDate, double finalCost) {
    Repair r = m_repairRepo.findById(repairId);
    if (r.getId() == 0) return false;
    r.setStatus("COMPLETED");
    r.setEndDate(endDate);
    r.setRepairCost(finalCost);
    return m_repairRepo.update(r);
}

Repair RepairService::getById(int id) const                { return m_repairRepo.findById(id); }
std::vector<Repair> RepairService::getByClaim(int cid) const { return m_repairRepo.findByClaim(cid); }
std::vector<Repair> RepairService::getAllRepairs() const      { return m_repairRepo.findAll(); }

// ═══════════════════════════════════════════
//  StaffService
// ═══════════════════════════════════════════
StaffService::StaffService(StaffRepository& repo) : m_staffRepo(repo) {}

Staff StaffService::addStaff(const std::string& name, const std::string& role,
                               const std::string& username, const std::string& password,
                               const std::string& phone, const std::string& email) {
    Staff existing = m_staffRepo.findByUsername(username);
    if (existing.getId() != 0) throw std::runtime_error("Username already taken.");
    int id = m_staffRepo.nextId();
    Staff s(id, name, role, username, password, phone, email);
    m_staffRepo.save(s);
    return s;
}

bool StaffService::updateStaff(const Staff& s) { return m_staffRepo.update(s); }
Staff StaffService::getById(int id) const       { return m_staffRepo.findById(id); }
std::vector<Staff> StaffService::getAllStaff()  const { return m_staffRepo.findAll(); }
std::vector<Staff> StaffService::getSalesmen()  const { return m_staffRepo.findByRole("SALESMAN"); }
std::vector<Staff> StaffService::getSurveyors() const { return m_staffRepo.findByRole("SURVEYOR"); }

// ═══════════════════════════════════════════
//  WorkshopService
// ═══════════════════════════════════════════
WorkshopService::WorkshopService(WorkshopRepository& repo) : m_workshopRepo(repo) {}

Workshop WorkshopService::addWorkshop(const std::string& name, const std::string& address,
                                       const std::string& phone, const std::string& ownerName) {
    int id = m_workshopRepo.nextId();
    Workshop w(id, name, address, phone, ownerName);
    m_workshopRepo.save(w);
    return w;
}

bool WorkshopService::updateWorkshop(const Workshop& w) { return m_workshopRepo.update(w); }
bool WorkshopService::removeWorkshop(int id)            { return m_workshopRepo.remove(id); }
Workshop WorkshopService::getById(int id) const         { return m_workshopRepo.findById(id); }
std::vector<Workshop> WorkshopService::getAllWorkshops() const { return m_workshopRepo.findAll(); }

// ═══════════════════════════════════════════
//  ReportService
// ═══════════════════════════════════════════
ReportService::ReportService(CustomerRepository& custRepo, PolicyRepository& polRepo,
                              ClaimRepository& claimRepo, InspectionRepository& inspRepo,
                              VehicleRepository& vehRepo, StaffRepository& staffRepo,
                              RepairRepository& repairRepo)
    : m_custRepo(custRepo), m_polRepo(polRepo), m_claimRepo(claimRepo),
      m_inspRepo(inspRepo), m_vehRepo(vehRepo), m_staffRepo(staffRepo),
      m_repairRepo(repairRepo) {}

std::string ReportService::newCustomersReport(const std::string& yearMonth) const {
    std::ostringstream ss;
    ss << "\n====== NEW CUSTOMERS REPORT [" << yearMonth << "] ======\n";
    auto customers = m_custRepo.findByMonth(yearMonth);
    ss << "Total New Customers: " << customers.size() << "\n";
    ss << std::left << std::setw(5) << "ID" << std::setw(20) << "Name"
       << std::setw(16) << "CNIC" << std::setw(14) << "Phone" << "Date\n";
    ss << std::string(60, '-') << "\n";
    for (const auto& c : customers)
        ss << std::setw(5) << c.getId() << std::setw(20) << c.getName()
           << std::setw(16) << c.getCnic() << std::setw(14) << c.getPhone()
           << c.getRegDate() << "\n";
    return ss.str();
}

std::string ReportService::pendingClaimsReport() const {
    std::ostringstream ss;
    ss << "\n====== PENDING CLAIMS REPORT ======\n";
    auto claims = m_claimRepo.findByStatus("PENDING");
    auto uclaims = m_claimRepo.findByStatus("UNDER_REVIEW");
    claims.insert(claims.end(), uclaims.begin(), uclaims.end());
    ss << "Total Pending/Under-Review: " << claims.size() << "\n";
    ss << std::left << std::setw(6) << "ID" << std::setw(14) << "ClaimNo"
       << std::setw(12) << "Filed" << std::setw(14) << "Status" << "EstLoss\n";
    ss << std::string(60, '-') << "\n";
    for (const auto& c : claims)
        ss << std::setw(6) << c.getId() << std::setw(14) << c.getClaimNo()
           << std::setw(12) << c.getFiledDate() << std::setw(14) << c.getStatus()
           << c.getEstimatedLoss() << "\n";
    return ss.str();
}

std::string ReportService::inspectionReport(int claimId) const {
    std::ostringstream ss;
    ss << "\n====== INSPECTION REPORT [Claim ID: " << claimId << "] ======\n";
    Claim cl = m_claimRepo.findById(claimId);
    if (cl.getId() == 0) { ss << "Claim not found.\n"; return ss.str(); }
    ss << "Claim No    : " << cl.getClaimNo() << "\n"
       << "Filed Date  : " << cl.getFiledDate() << "\n"
       << "Description : " << cl.getDescription() << "\n"
       << "Status      : " << cl.getStatus() << "\n\n";
    auto insps = m_inspRepo.findByClaim(claimId);
    for (const auto& i : insps) {
        Staff surveyor = m_staffRepo.findById(i.getSurveyorId());
        ss << "--- Inspection ID: " << i.getId() << " ---\n"
           << "Date        : " << i.getInspectionDate() << "\n"
           << "Surveyor    : " << surveyor.getName() << "\n"
           << "Findings    : " << i.getFindings() << "\n"
           << "Repair Cost : PKR " << i.getRepairCost() << "\n"
           << "Recommend.  : " << i.getRecommendation() << "\n"
           << "Status      : " << i.getStatus() << "\n\n";
    }
    return ss.str();
}

std::string ReportService::customerClaimsReport(int customerId) const {
    std::ostringstream ss;
    Customer cust = m_custRepo.findById(customerId);
    ss << "\n====== CLAIMS REPORT [Customer: " << cust.getName() << "] ======\n";
    auto claims = m_claimRepo.findByCustomer(customerId);
    ss << "Total Claims: " << claims.size() << "\n";
    ss << std::left << std::setw(6) << "ID" << std::setw(14) << "ClaimNo"
       << std::setw(12) << "Incident" << std::setw(14) << "Status" << "EstLoss\n";
    ss << std::string(60, '-') << "\n";
    for (const auto& c : claims)
        ss << std::setw(6) << c.getId() << std::setw(14) << c.getClaimNo()
           << std::setw(12) << c.getIncidentDate() << std::setw(14) << c.getStatus()
           << c.getEstimatedLoss() << "\n";
    return ss.str();
}

std::string ReportService::allPoliciesReport() const {
    std::ostringstream ss;
    ss << "\n====== ALL POLICIES REPORT ======\n";
    auto policies = m_polRepo.findAll();
    ss << "Total: " << policies.size() << "\n";
    ss << std::left << std::setw(6) << "ID" << std::setw(16) << "PolicyNo"
       << std::setw(14) << "Type" << std::setw(12) << "Status"
       << std::setw(12) << "Start" << "End\n";
    ss << std::string(70, '-') << "\n";
    for (const auto& p : policies)
        ss << std::setw(6) << p.getId() << std::setw(16) << p.getPolicyNo()
           << std::setw(14) << p.getType() << std::setw(12) << p.getStatus()
           << std::setw(12) << p.getStartDate() << p.getEndDate() << "\n";
    return ss.str();
}

std::string ReportService::workshopRepairsReport(int workshopId, WorkshopRepository& wRepo) const {
    std::ostringstream ss;
    Workshop w = wRepo.findById(workshopId);
    ss << "\n====== REPAIRS REPORT [Workshop: " << w.getName() << "] ======\n";
    auto repairs = m_repairRepo.findByWorkshop(workshopId);
    ss << "Total Repairs: " << repairs.size() << "\n";
    ss << std::left << std::setw(6) << "ID" << std::setw(10) << "ClaimID"
       << std::setw(14) << "Start" << std::setw(14) << "End"
       << std::setw(12) << "Status" << "Cost\n";
    ss << std::string(65, '-') << "\n";
    for (const auto& r : repairs)
        ss << std::setw(6) << r.getId() << std::setw(10) << r.getClaimId()
           << std::setw(14) << r.getStartDate() << std::setw(14) << r.getEndDate()
           << std::setw(12) << r.getStatus() << r.getRepairCost() << "\n";
    return ss.str();
}
