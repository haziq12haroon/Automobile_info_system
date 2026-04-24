#include "FileRepository.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

// ═══════════════════════════════════════════════
//  CustomerRepository
// ═══════════════════════════════════════════════
CustomerRepository::CustomerRepository(const std::string& filePath)
    : m_filePath(filePath) {}

std::vector<Customer> CustomerRepository::loadAll() const {
    std::vector<Customer> records;
    std::ifstream file(m_filePath);
    if (!file.is_open()) return records;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            try { records.push_back(Customer::deserialize(line)); }
            catch (...) {}
        }
    }
    return records;
}

void CustomerRepository::saveAll(const std::vector<Customer>& records) const {
    std::ofstream file(m_filePath, std::ios::trunc);
    for (const auto& r : records)
        file << r.serialize() << '\n';
}

bool CustomerRepository::save(const Customer& c) {
    auto records = loadAll();
    records.push_back(c);
    saveAll(records);
    return true;
}

bool CustomerRepository::update(const Customer& c) {
    auto records = loadAll();
    for (auto& r : records) {
        if (r.getId() == c.getId()) { r = c; saveAll(records); return true; }
    }
    return false;
}

bool CustomerRepository::remove(int id) {
    auto records = loadAll();
    auto it = std::remove_if(records.begin(), records.end(),
        [id](const Customer& c){ return c.getId() == id; });
    if (it == records.end()) return false;
    records.erase(it, records.end());
    saveAll(records);
    return true;
}

Customer CustomerRepository::findById(int id) const {
    for (const auto& c : loadAll())
        if (c.getId() == id) return c;
    return Customer();
}

Customer CustomerRepository::findByCnic(const std::string& cnic) const {
    for (const auto& c : loadAll())
        if (c.getCnic() == cnic) return c;
    return Customer();
}

std::vector<Customer> CustomerRepository::findAll() const { return loadAll(); }

std::vector<Customer> CustomerRepository::findByMonth(const std::string& ym) const {
    std::vector<Customer> result;
    for (const auto& c : loadAll())
        if (c.getRegDate().substr(0,7) == ym) result.push_back(c);
    return result;
}

int CustomerRepository::nextId() const {
    auto records = loadAll();
    int maxId = 0;
    for (const auto& r : records) if (r.getId() > maxId) maxId = r.getId();
    return maxId + 1;
}

// ═══════════════════════════════════════════════
//  VehicleRepository
// ═══════════════════════════════════════════════
VehicleRepository::VehicleRepository(const std::string& fp) : m_filePath(fp) {}

std::vector<Vehicle> VehicleRepository::loadAll() const {
    std::vector<Vehicle> records;
    std::ifstream file(m_filePath);
    if (!file.is_open()) return records;
    std::string line;
    while (std::getline(file, line))
        if (!line.empty()) { try { records.push_back(Vehicle::deserialize(line)); } catch(...){} }
    return records;
}

void VehicleRepository::saveAll(const std::vector<Vehicle>& records) const {
    std::ofstream file(m_filePath, std::ios::trunc);
    for (const auto& r : records) file << r.serialize() << '\n';
}

bool VehicleRepository::save(const Vehicle& v) {
    auto records = loadAll(); records.push_back(v); saveAll(records); return true;
}

bool VehicleRepository::update(const Vehicle& v) {
    auto records = loadAll();
    for (auto& r : records) { if (r.getId() == v.getId()) { r = v; saveAll(records); return true; } }
    return false;
}

Vehicle VehicleRepository::findById(int id) const {
    for (const auto& v : loadAll()) if (v.getId() == id) return v;
    return Vehicle();
}

Vehicle VehicleRepository::findByRegNo(const std::string& regNo) const {
    for (const auto& v : loadAll()) if (v.getRegNo() == regNo) return v;
    return Vehicle();
}

std::vector<Vehicle> VehicleRepository::findAll() const { return loadAll(); }

std::vector<Vehicle> VehicleRepository::findByCustomer(int cid) const {
    std::vector<Vehicle> result;
    for (const auto& v : loadAll()) if (v.getCustomerId() == cid) result.push_back(v);
    return result;
}

int VehicleRepository::nextId() const {
    auto records = loadAll(); int mx = 0;
    for (const auto& r : records) if (r.getId() > mx) mx = r.getId();
    return mx + 1;
}

// ═══════════════════════════════════════════════
//  StaffRepository
// ═══════════════════════════════════════════════
StaffRepository::StaffRepository(const std::string& fp) : m_filePath(fp) {}

std::vector<Staff> StaffRepository::loadAll() const {
    std::vector<Staff> records;
    std::ifstream file(m_filePath);
    if (!file.is_open()) return records;
    std::string line;
    while (std::getline(file, line))
        if (!line.empty()) { try { records.push_back(Staff::deserialize(line)); } catch(...){} }
    return records;
}

void StaffRepository::saveAll(const std::vector<Staff>& records) const {
    std::ofstream file(m_filePath, std::ios::trunc);
    for (const auto& r : records) file << r.serialize() << '\n';
}

bool StaffRepository::save(const Staff& s) {
    auto records = loadAll(); records.push_back(s); saveAll(records); return true;
}

bool StaffRepository::update(const Staff& s) {
    auto records = loadAll();
    for (auto& r : records) { if (r.getId() == s.getId()) { r = s; saveAll(records); return true; } }
    return false;
}

Staff StaffRepository::findById(int id) const {
    for (const auto& s : loadAll()) if (s.getId() == id) return s;
    return Staff();
}

Staff StaffRepository::findByUsername(const std::string& username) const {
    for (const auto& s : loadAll()) if (s.getUsername() == username) return s;
    return Staff();
}

std::vector<Staff> StaffRepository::findAll() const { return loadAll(); }

std::vector<Staff> StaffRepository::findByRole(const std::string& role) const {
    std::vector<Staff> result;
    for (const auto& s : loadAll()) if (s.getRole() == role) result.push_back(s);
    return result;
}

int StaffRepository::nextId() const {
    auto records = loadAll(); int mx = 0;
    for (const auto& r : records) if (r.getId() > mx) mx = r.getId();
    return mx + 1;
}

// ═══════════════════════════════════════════════
//  WorkshopRepository
// ═══════════════════════════════════════════════
WorkshopRepository::WorkshopRepository(const std::string& fp) : m_filePath(fp) {}

std::vector<Workshop> WorkshopRepository::loadAll() const {
    std::vector<Workshop> records;
    std::ifstream file(m_filePath);
    if (!file.is_open()) return records;
    std::string line;
    while (std::getline(file, line))
        if (!line.empty()) { try { records.push_back(Workshop::deserialize(line)); } catch(...){} }
    return records;
}

void WorkshopRepository::saveAll(const std::vector<Workshop>& records) const {
    std::ofstream file(m_filePath, std::ios::trunc);
    for (const auto& r : records) file << r.serialize() << '\n';
}

bool WorkshopRepository::save(const Workshop& w) {
    auto records = loadAll(); records.push_back(w); saveAll(records); return true;
}

bool WorkshopRepository::update(const Workshop& w) {
    auto records = loadAll();
    for (auto& r : records) { if (r.getId() == w.getId()) { r = w; saveAll(records); return true; } }
    return false;
}

bool WorkshopRepository::remove(int id) {
    auto records = loadAll();
    auto it = std::remove_if(records.begin(), records.end(),
        [id](const Workshop& w){ return w.getId() == id; });
    if (it == records.end()) return false;
    records.erase(it, records.end()); saveAll(records); return true;
}

Workshop WorkshopRepository::findById(int id) const {
    for (const auto& w : loadAll()) if (w.getId() == id) return w;
    return Workshop();
}

std::vector<Workshop> WorkshopRepository::findAll() const { return loadAll(); }

int WorkshopRepository::nextId() const {
    auto records = loadAll(); int mx = 0;
    for (const auto& r : records) if (r.getId() > mx) mx = r.getId();
    return mx + 1;
}

// ═══════════════════════════════════════════════
//  PolicyRepository
// ═══════════════════════════════════════════════
PolicyRepository::PolicyRepository(const std::string& fp) : m_filePath(fp) {}

std::vector<InsurancePolicy> PolicyRepository::loadAll() const {
    std::vector<InsurancePolicy> records;
    std::ifstream file(m_filePath);
    if (!file.is_open()) return records;
    std::string line;
    while (std::getline(file, line))
        if (!line.empty()) { try { records.push_back(InsurancePolicy::deserialize(line)); } catch(...){} }
    return records;
}

void PolicyRepository::saveAll(const std::vector<InsurancePolicy>& records) const {
    std::ofstream file(m_filePath, std::ios::trunc);
    for (const auto& r : records) file << r.serialize() << '\n';
}

bool PolicyRepository::save(const InsurancePolicy& p) {
    auto records = loadAll(); records.push_back(p); saveAll(records); return true;
}

bool PolicyRepository::update(const InsurancePolicy& p) {
    auto records = loadAll();
    for (auto& r : records) { if (r.getId() == p.getId()) { r = p; saveAll(records); return true; } }
    return false;
}

InsurancePolicy PolicyRepository::findById(int id) const {
    for (const auto& p : loadAll()) if (p.getId() == id) return p;
    return InsurancePolicy();
}

InsurancePolicy PolicyRepository::findByPolicyNo(const std::string& no) const {
    for (const auto& p : loadAll()) if (p.getPolicyNo() == no) return p;
    return InsurancePolicy();
}

std::vector<InsurancePolicy> PolicyRepository::findAll() const { return loadAll(); }

std::vector<InsurancePolicy> PolicyRepository::findByCustomer(int cid) const {
    std::vector<InsurancePolicy> result;
    for (const auto& p : loadAll()) if (p.getCustomerId() == cid) result.push_back(p);
    return result;
}

std::vector<InsurancePolicy> PolicyRepository::findByVehicle(int vid) const {
    std::vector<InsurancePolicy> result;
    for (const auto& p : loadAll()) if (p.getVehicleId() == vid) result.push_back(p);
    return result;
}

int PolicyRepository::nextId() const {
    auto records = loadAll(); int mx = 0;
    for (const auto& r : records) if (r.getId() > mx) mx = r.getId();
    return mx + 1;
}

// ═══════════════════════════════════════════════
//  ClaimRepository
// ═══════════════════════════════════════════════
ClaimRepository::ClaimRepository(const std::string& fp) : m_filePath(fp) {}

std::vector<Claim> ClaimRepository::loadAll() const {
    std::vector<Claim> records;
    std::ifstream file(m_filePath);
    if (!file.is_open()) return records;
    std::string line;
    while (std::getline(file, line))
        if (!line.empty()) { try { records.push_back(Claim::deserialize(line)); } catch(...){} }
    return records;
}

void ClaimRepository::saveAll(const std::vector<Claim>& records) const {
    std::ofstream file(m_filePath, std::ios::trunc);
    for (const auto& r : records) file << r.serialize() << '\n';
}

bool ClaimRepository::save(const Claim& c) {
    auto records = loadAll(); records.push_back(c); saveAll(records); return true;
}

bool ClaimRepository::update(const Claim& c) {
    auto records = loadAll();
    for (auto& r : records) { if (r.getId() == c.getId()) { r = c; saveAll(records); return true; } }
    return false;
}

Claim ClaimRepository::findById(int id) const {
    for (const auto& c : loadAll()) if (c.getId() == id) return c;
    return Claim();
}

Claim ClaimRepository::findByClaimNo(const std::string& no) const {
    for (const auto& c : loadAll()) if (c.getClaimNo() == no) return c;
    return Claim();
}

std::vector<Claim> ClaimRepository::findAll() const { return loadAll(); }

std::vector<Claim> ClaimRepository::findByCustomer(int cid) const {
    std::vector<Claim> result;
    for (const auto& c : loadAll()) if (c.getCustomerId() == cid) result.push_back(c);
    return result;
}

std::vector<Claim> ClaimRepository::findByPolicy(int pid) const {
    std::vector<Claim> result;
    for (const auto& c : loadAll()) if (c.getPolicyId() == pid) result.push_back(c);
    return result;
}

std::vector<Claim> ClaimRepository::findByStatus(const std::string& status) const {
    std::vector<Claim> result;
    for (const auto& c : loadAll()) if (c.getStatus() == status) result.push_back(c);
    return result;
}

int ClaimRepository::nextId() const {
    auto records = loadAll(); int mx = 0;
    for (const auto& r : records) if (r.getId() > mx) mx = r.getId();
    return mx + 1;
}

// ═══════════════════════════════════════════════
//  InspectionRepository
// ═══════════════════════════════════════════════
InspectionRepository::InspectionRepository(const std::string& fp) : m_filePath(fp) {}

std::vector<Inspection> InspectionRepository::loadAll() const {
    std::vector<Inspection> records;
    std::ifstream file(m_filePath);
    if (!file.is_open()) return records;
    std::string line;
    while (std::getline(file, line))
        if (!line.empty()) { try { records.push_back(Inspection::deserialize(line)); } catch(...){} }
    return records;
}

void InspectionRepository::saveAll(const std::vector<Inspection>& records) const {
    std::ofstream file(m_filePath, std::ios::trunc);
    for (const auto& r : records) file << r.serialize() << '\n';
}

bool InspectionRepository::save(const Inspection& i) {
    auto records = loadAll(); records.push_back(i); saveAll(records); return true;
}

bool InspectionRepository::update(const Inspection& i) {
    auto records = loadAll();
    for (auto& r : records) { if (r.getId() == i.getId()) { r = i; saveAll(records); return true; } }
    return false;
}

Inspection InspectionRepository::findById(int id) const {
    for (const auto& i : loadAll()) if (i.getId() == id) return i;
    return Inspection();
}

std::vector<Inspection> InspectionRepository::findAll() const { return loadAll(); }

std::vector<Inspection> InspectionRepository::findByClaim(int cid) const {
    std::vector<Inspection> result;
    for (const auto& i : loadAll()) if (i.getClaimId() == cid) result.push_back(i);
    return result;
}

std::vector<Inspection> InspectionRepository::findBySurveyor(int sid) const {
    std::vector<Inspection> result;
    for (const auto& i : loadAll()) if (i.getSurveyorId() == sid) result.push_back(i);
    return result;
}

int InspectionRepository::nextId() const {
    auto records = loadAll(); int mx = 0;
    for (const auto& r : records) if (r.getId() > mx) mx = r.getId();
    return mx + 1;
}

// ═══════════════════════════════════════════════
//  RepairRepository
// ═══════════════════════════════════════════════
RepairRepository::RepairRepository(const std::string& fp) : m_filePath(fp) {}

std::vector<Repair> RepairRepository::loadAll() const {
    std::vector<Repair> records;
    std::ifstream file(m_filePath);
    if (!file.is_open()) return records;
    std::string line;
    while (std::getline(file, line))
        if (!line.empty()) { try { records.push_back(Repair::deserialize(line)); } catch(...){} }
    return records;
}

void RepairRepository::saveAll(const std::vector<Repair>& records) const {
    std::ofstream file(m_filePath, std::ios::trunc);
    for (const auto& r : records) file << r.serialize() << '\n';
}

bool RepairRepository::save(const Repair& r) {
    auto records = loadAll(); records.push_back(r); saveAll(records); return true;
}

bool RepairRepository::update(const Repair& r) {
    auto records = loadAll();
    for (auto& rec : records) { if (rec.getId() == r.getId()) { rec = r; saveAll(records); return true; } }
    return false;
}

Repair RepairRepository::findById(int id) const {
    for (const auto& r : loadAll()) if (r.getId() == id) return r;
    return Repair();
}

std::vector<Repair> RepairRepository::findAll() const { return loadAll(); }

std::vector<Repair> RepairRepository::findByClaim(int cid) const {
    std::vector<Repair> result;
    for (const auto& r : loadAll()) if (r.getClaimId() == cid) result.push_back(r);
    return result;
}

std::vector<Repair> RepairRepository::findByWorkshop(int wid) const {
    std::vector<Repair> result;
    for (const auto& r : loadAll()) if (r.getWorkshopId() == wid) result.push_back(r);
    return result;
}

int RepairRepository::nextId() const {
    auto records = loadAll(); int mx = 0;
    for (const auto& r : records) if (r.getId() > mx) mx = r.getId();
    return mx + 1;
}
