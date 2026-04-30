#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <ctime>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>   // for _mkdir
#else
#include <sys/stat.h> // for mkdir
#endif
// ════════════════════════════════════════════════════════════
//  SECTION 1 – UTILITY HELPERS
// ════════════════════════════════════════════════════════════

static std::string todayStr() {
    time_t now = time(nullptr);
    const struct tm* t = localtime(&now);
    char buf[12];
    strftime(buf, sizeof(buf), "%Y-%m-%d", t);
    return std::string(buf);
}

// Extract one pipe-delimited token from an istringstream
static std::string tok(std::istringstream& ss) {
    std::string t;
    std::getline(ss, t, '|');
    return t;
}

// Safe integer parse with a fallback of 0
static int safeInt(const std::string& s) {
    try { return std::stoi(s); } catch (...) { return 0; }
}

// Safe double parse with a fallback of 0.0
static double safeDbl(const std::string& s) {
    try { return std::stod(s); } catch (...) { return 0.0; }
}

// Console helpers
static void printLine(char c = '-', int len = 62) {
    std::cout << std::string(len, c) << "\n";
}

static void printHeader(const std::string& title) {
    printLine('=');
    int pad = (62 - (int)title.size()) / 2;
    if (pad > 0) std::cout << std::string(pad, ' ');
    std::cout << title << "\n";
    printLine('=');
}

static int readInt(const std::string& prompt) {
    int val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  [!] Enter a whole number.\n";
    }
}

static double readDouble(const std::string& prompt) {
    double val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  [!] Enter a numeric value.\n";
    }
}

static std::string readStr(const std::string& prompt) {
    std::cout << prompt;
    std::string val;
    std::getline(std::cin, val);
    return val;
}

static void pause() {
    std::cout << "\n  Press ENTER to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// ════════════════════════════════════════════════════════════
//  SECTION 2 – ENTITY CLASSES
// ════════════════════════════════════════════════════════════

// ─── Staff ──────────────────────────────────────────────────
// Role values: ADMIN | SALESMAN | SURVEYOR | WORKSHOP_MGR
class Staff {
public:
    Staff() : m_id(0), m_workshopId(0) {}
    Staff(int id, const std::string& name, const std::string& role,
          const std::string& username, const std::string& password,
          const std::string& phone, const std::string& email,
          int workshopId = 0)
        : m_id(id), m_name(name), m_role(role),
          m_username(username), m_password(password),
          m_phone(phone), m_email(email), m_workshopId(workshopId) {}

    int         getId()        const { return m_id; }
    std::string getName()      const { return m_name; }
    std::string getRole()      const { return m_role; }
    std::string getUsername()  const { return m_username; }
    std::string getPassword()  const { return m_password; }
    std::string getPhone()     const { return m_phone; }
    std::string getEmail()     const { return m_email; }
    int         getWorkshopId()const { return m_workshopId; }

    void setPassword(const std::string& pw)  { m_password  = pw; }
    void setWorkshopId(int wid)              { m_workshopId = wid; }
    void setPhone(const std::string& ph)     { m_phone = ph; }
    void setEmail(const std::string& em)     { m_email = em; }

    std::string serialize() const {
        std::ostringstream ss;
        ss << m_id << '|' << m_name << '|' << m_role << '|'
           << m_username << '|' << m_password << '|'
           << m_phone << '|' << m_email << '|' << m_workshopId;
        return ss.str();
    }

    static Staff deserialize(const std::string& line) {
        std::istringstream ss(line);
        int id               = safeInt(tok(ss));
        std::string name     = tok(ss);
        std::string role     = tok(ss);
        std::string username = tok(ss);
        std::string password = tok(ss);
        std::string phone    = tok(ss);
        std::string email    = tok(ss);
        int wid              = safeInt(tok(ss));
        return Staff(id, name, role, username, password, phone, email, wid);
    }

private:
    int         m_id;
    std::string m_name;
    std::string m_role;
    std::string m_username;
    std::string m_password;
    std::string m_phone;
    std::string m_email;
    int         m_workshopId;  // 0 = not assigned 
};

// ─── Customer ───────────────────────────────────────────────
class Customer {
public:
    Customer() : m_id(0) {}
    Customer(int id, const std::string& name, const std::string& cnic,
             const std::string& phone, const std::string& email,
             const std::string& address, const std::string& regDate)
        : m_id(id), m_name(name), m_cnic(cnic), m_phone(phone),
          m_email(email), m_address(address), m_regDate(regDate) {}

    int         getId()      const { return m_id; }
    std::string getName()    const { return m_name; }
    std::string getCnic()    const { return m_cnic; }
    std::string getPhone()   const { return m_phone; }
    std::string getEmail()   const { return m_email; }
    std::string getAddress() const { return m_address; }
    std::string getRegDate() const { return m_regDate; }

    void setPhone(const std::string& p)   { m_phone   = p; }
    void setEmail(const std::string& e)   { m_email   = e; }
    void setAddress(const std::string& a) { m_address = a; }

    std::string serialize() const {
        std::ostringstream ss;
        ss << m_id << '|' << m_name << '|' << m_cnic << '|'
           << m_phone << '|' << m_email << '|' << m_address << '|' << m_regDate;
        return ss.str();
    }

    static Customer deserialize(const std::string& line) {
        std::istringstream ss(line);
        int id              = safeInt(tok(ss));
        std::string name    = tok(ss);
        std::string cnic    = tok(ss);
        std::string phone   = tok(ss);
        std::string email   = tok(ss);
        std::string address = tok(ss);
        std::string regDate = tok(ss);
        return Customer(id, name, cnic, phone, email, address, regDate);
    }

private:
    int         m_id;
    std::string m_name, m_cnic, m_phone, m_email, m_address, m_regDate;
};

// ─── Vehicle ────────────────────────────────────────────────
class Vehicle {
public:
    Vehicle() : m_id(0), m_customerId(0), m_year(0) {}
    Vehicle(int id, int customerId, const std::string& regNo,
            const std::string& make, const std::string& model,
            int year, const std::string& color,
            const std::string& chassisNo, const std::string& engineNo)
        : m_id(id), m_customerId(customerId), m_year(year),
          m_regNo(regNo), m_make(make), m_model(model),
          m_color(color), m_chassisNo(chassisNo), m_engineNo(engineNo) {}

    int         getId()         const { return m_id; }
    int         getCustomerId() const { return m_customerId; }
    std::string getRegNo()      const { return m_regNo; }
    std::string getMake()       const { return m_make; }
    std::string getModel()      const { return m_model; }
    int         getYear()       const { return m_year; }
    std::string getColor()      const { return m_color; }
    std::string getChassisNo()  const { return m_chassisNo; }
    std::string getEngineNo()   const { return m_engineNo; }

    std::string serialize() const {
        std::ostringstream ss;
        ss << m_id << '|' << m_customerId << '|' << m_regNo << '|'
           << m_make << '|' << m_model << '|' << m_year << '|'
           << m_color << '|' << m_chassisNo << '|' << m_engineNo;
        return ss.str();
    }

    static Vehicle deserialize(const std::string& line) {
        std::istringstream ss(line);
        int id          = safeInt(tok(ss));
        int custId      = safeInt(tok(ss));
        std::string rn  = tok(ss);
        std::string mk  = tok(ss);
        std::string mo  = tok(ss);
        int yr          = safeInt(tok(ss));
        std::string col = tok(ss);
        std::string ch  = tok(ss);
        std::string en  = tok(ss);
        return Vehicle(id, custId, rn, mk, mo, yr, col, ch, en);
    }

private:
    int m_id, m_customerId, m_year;
    std::string m_regNo, m_make, m_model, m_color, m_chassisNo, m_engineNo;
};

// ─── Workshop ───────────────────────────────────────────────
class Workshop {
public:
    Workshop() : m_id(0), m_managerId(0) {}
    Workshop(int id, const std::string& name, const std::string& address,
             const std::string& phone, const std::string& ownerName,
             int managerId = 0)
        : m_id(id), m_name(name), m_address(address),
          m_phone(phone), m_ownerName(ownerName), m_managerId(managerId) {}

    int         getId()        const { return m_id; }
    std::string getName()      const { return m_name; }
    std::string getAddress()   const { return m_address; }
    std::string getPhone()     const { return m_phone; }
    std::string getOwnerName() const { return m_ownerName; }
    int         getManagerId() const { return m_managerId; }

    void setManagerId(int mid)              { m_managerId = mid; }
    void setPhone(const std::string& ph)    { m_phone = ph; }
    void setAddress(const std::string& ad)  { m_address = ad; }

    std::string serialize() const {
        std::ostringstream ss;
        ss << m_id << '|' << m_name << '|' << m_address << '|'
           << m_phone << '|' << m_ownerName << '|' << m_managerId;
        return ss.str();
    }

    static Workshop deserialize(const std::string& line) {
        std::istringstream ss(line);
        int id               = safeInt(tok(ss));
        std::string name     = tok(ss);
        std::string address  = tok(ss);
        std::string phone    = tok(ss);
        std::string owner    = tok(ss);
        int managerId        = safeInt(tok(ss));
        return Workshop(id, name, address, phone, owner, managerId);
    }

private:
    int         m_id;
    std::string m_name, m_address, m_phone, m_ownerName;
    int         m_managerId;
};

// ─── InsurancePolicy ────────────────────────────────────────
class InsurancePolicy {
public:
    InsurancePolicy() : m_id(0), m_customerId(0), m_vehicleId(0),
                        m_salesmanId(0), m_premiumAmount(0), m_coverageAmount(0) {}
    InsurancePolicy(int id, int customerId, int vehicleId, int salesmanId,
                    const std::string& policyNo, const std::string& type,
                    double premium, double coverage,
                    const std::string& startDate, const std::string& endDate,
                    const std::string& status)
        : m_id(id), m_customerId(customerId), m_vehicleId(vehicleId),
          m_salesmanId(salesmanId), m_policyNo(policyNo), m_type(type),
          m_premiumAmount(premium), m_coverageAmount(coverage),
          m_startDate(startDate), m_endDate(endDate), m_status(status) {}

    int         getId()             const { return m_id; }
    int         getCustomerId()     const { return m_customerId; }
    int         getVehicleId()      const { return m_vehicleId; }
    int         getSalesmanId()     const { return m_salesmanId; }
    std::string getPolicyNo()       const { return m_policyNo; }
    std::string getType()           const { return m_type; }
    double      getPremiumAmount()  const { return m_premiumAmount; }
    double      getCoverageAmount() const { return m_coverageAmount; }
    std::string getStartDate()      const { return m_startDate; }
    std::string getEndDate()        const { return m_endDate; }
    std::string getStatus()         const { return m_status; }

    void setStatus(const std::string& s) { m_status = s; }

    std::string serialize() const {
        std::ostringstream ss;
        ss << m_id << '|' << m_customerId << '|' << m_vehicleId << '|'
           << m_salesmanId << '|' << m_policyNo << '|' << m_type << '|'
           << m_premiumAmount << '|' << m_coverageAmount << '|'
           << m_startDate << '|' << m_endDate << '|' << m_status;
        return ss.str();
    }

    static InsurancePolicy deserialize(const std::string& line) {
        std::istringstream ss(line);
        int id              = safeInt(tok(ss));
        int cid             = safeInt(tok(ss));
        int vid             = safeInt(tok(ss));
        int sid             = safeInt(tok(ss));
        std::string pno     = tok(ss);
        std::string type    = tok(ss);
        double prem         = safeDbl(tok(ss));
        double cov          = safeDbl(tok(ss));
        std::string start   = tok(ss);
        std::string end     = tok(ss);
        std::string status  = tok(ss);
        return InsurancePolicy(id, cid, vid, sid, pno, type, prem, cov, start, end, status);
    }

private:
    int         m_id, m_customerId, m_vehicleId, m_salesmanId;
    std::string m_policyNo, m_type;
    double      m_premiumAmount, m_coverageAmount;
    std::string m_startDate, m_endDate, m_status;
};

// ─── Claim ──────────────────────────────────────────────────
class Claim {
public:
    Claim() : m_id(0), m_policyId(0), m_customerId(0), m_vehicleId(0), m_estimatedLoss(0) {}
    Claim(int id, int policyId, int customerId, int vehicleId,
          const std::string& claimNo, const std::string& incidentDate,
          const std::string& filedDate, const std::string& description,
          double estimatedLoss, const std::string& status)
        : m_id(id), m_policyId(policyId), m_customerId(customerId),
          m_vehicleId(vehicleId), m_claimNo(claimNo),
          m_incidentDate(incidentDate), m_filedDate(filedDate),
          m_description(description), m_estimatedLoss(estimatedLoss),
          m_status(status) {}

    int         getId()            const { return m_id; }
    int         getPolicyId()      const { return m_policyId; }
    int         getCustomerId()    const { return m_customerId; }
    int         getVehicleId()     const { return m_vehicleId; }
    std::string getClaimNo()       const { return m_claimNo; }
    std::string getIncidentDate()  const { return m_incidentDate; }
    std::string getFiledDate()     const { return m_filedDate; }
    std::string getDescription()   const { return m_description; }
    double      getEstimatedLoss() const { return m_estimatedLoss; }
    std::string getStatus()        const { return m_status; }

    void setStatus(const std::string& s) { m_status = s; }

    std::string serialize() const {
        std::ostringstream ss;
        ss << m_id << '|' << m_policyId << '|' << m_customerId << '|'
           << m_vehicleId << '|' << m_claimNo << '|' << m_incidentDate << '|'
           << m_filedDate << '|' << m_description << '|'
           << m_estimatedLoss << '|' << m_status;
        return ss.str();
    }

    static Claim deserialize(const std::string& line) {
        std::istringstream ss(line);
        int id              = safeInt(tok(ss));
        int polId           = safeInt(tok(ss));
        int cid             = safeInt(tok(ss));
        int vid             = safeInt(tok(ss));
        std::string cno     = tok(ss);
        std::string inc     = tok(ss);
        std::string fil     = tok(ss);
        std::string desc    = tok(ss);
        double loss         = safeDbl(tok(ss));
        std::string status  = tok(ss);
        return Claim(id, polId, cid, vid, cno, inc, fil, desc, loss, status);
    }

private:
    int         m_id, m_policyId, m_customerId, m_vehicleId;
    std::string m_claimNo, m_incidentDate, m_filedDate, m_description;
    double      m_estimatedLoss;
    std::string m_status;
};

// ─── Inspection ─────────────────────────────────────────────
class Inspection {
public:
    Inspection() : m_id(0), m_claimId(0), m_surveyorId(0), m_workshopId(0), m_repairCost(0) {}
    Inspection(int id, int claimId, int surveyorId, int workshopId,
               const std::string& inspDate, const std::string& findings,
               double repairCost, const std::string& recommendation,
               const std::string& status)
        : m_id(id), m_claimId(claimId), m_surveyorId(surveyorId),
          m_workshopId(workshopId), m_inspDate(inspDate), m_findings(findings),
          m_repairCost(repairCost), m_recommendation(recommendation), m_status(status) {}

    int         getId()             const { return m_id; }
    int         getClaimId()        const { return m_claimId; }
    int         getSurveyorId()     const { return m_surveyorId; }
    int         getWorkshopId()     const { return m_workshopId; }
    std::string getInspDate()       const { return m_inspDate; }
    std::string getFindings()       const { return m_findings; }
    double      getRepairCost()     const { return m_repairCost; }
    std::string getRecommendation() const { return m_recommendation; }
    std::string getStatus()         const { return m_status; }

    void setFindings(const std::string& f)       { m_findings = f; }
    void setRepairCost(double c)                 { m_repairCost = c; }
    void setRecommendation(const std::string& r) { m_recommendation = r; }
    void setStatus(const std::string& s)         { m_status = s; }

    std::string serialize() const {
        std::ostringstream ss;
        ss << m_id << '|' << m_claimId << '|' << m_surveyorId << '|'
           << m_workshopId << '|' << m_inspDate << '|' << m_findings << '|'
           << m_repairCost << '|' << m_recommendation << '|' << m_status;
        return ss.str();
    }

    static Inspection deserialize(const std::string& line) {
        std::istringstream ss(line);
        int id              = safeInt(tok(ss));
        int cid             = safeInt(tok(ss));
        int sid             = safeInt(tok(ss));
        int wid             = safeInt(tok(ss));
        std::string dt      = tok(ss);
        std::string find    = tok(ss);
        double cost         = safeDbl(tok(ss));
        std::string rec     = tok(ss);
        std::string status  = tok(ss);
        return Inspection(id, cid, sid, wid, dt, find, cost, rec, status);
    }

private:
    int         m_id, m_claimId, m_surveyorId, m_workshopId;
    std::string m_inspDate, m_findings;
    double      m_repairCost;
    std::string m_recommendation, m_status;
};

// ─── Repair ─────────────────────────────────────────────────
class Repair {
public:
    Repair() : m_id(0), m_claimId(0), m_workshopId(0), m_inspectionId(0),
               m_workshopMgrId(0), m_repairCost(0) {}
    Repair(int id, int claimId, int workshopId, int inspectionId,
           int workshopMgrId,
           const std::string& startDate, const std::string& endDate,
           double repairCost, const std::string& status,
           const std::string& notes)
        : m_id(id), m_claimId(claimId), m_workshopId(workshopId),
          m_inspectionId(inspectionId), m_workshopMgrId(workshopMgrId),
          m_startDate(startDate), m_endDate(endDate),
          m_repairCost(repairCost), m_status(status), m_notes(notes) {}

    int         getId()            const { return m_id; }
    int         getClaimId()       const { return m_claimId; }
    int         getWorkshopId()    const { return m_workshopId; }
    int         getInspectionId()  const { return m_inspectionId; }
    int         getWorkshopMgrId() const { return m_workshopMgrId; }
    std::string getStartDate()     const { return m_startDate; }
    std::string getEndDate()       const { return m_endDate; }
    double      getRepairCost()    const { return m_repairCost; }
    std::string getStatus()        const { return m_status; }
    std::string getNotes()         const { return m_notes; }

    void setStatus(const std::string& s)  { m_status = s; }
    void setEndDate(const std::string& d) { m_endDate = d; }
    void setRepairCost(double c)          { m_repairCost = c; }

    std::string serialize() const {
        std::ostringstream ss;
        ss << m_id << '|' << m_claimId << '|' << m_workshopId << '|'
           << m_inspectionId << '|' << m_workshopMgrId << '|'
           << m_startDate << '|' << m_endDate << '|'
           << m_repairCost << '|' << m_status << '|' << m_notes;
        return ss.str();
    }

    static Repair deserialize(const std::string& line) {
        std::istringstream ss(line);
        int id          = safeInt(tok(ss));
        int cid         = safeInt(tok(ss));
        int wid         = safeInt(tok(ss));
        int iid         = safeInt(tok(ss));
        int mid         = safeInt(tok(ss));
        std::string st  = tok(ss);
        std::string en  = tok(ss);
        double cost     = safeDbl(tok(ss));
        std::string sta = tok(ss);
        std::string no  = tok(ss);
        return Repair(id, cid, wid, iid, mid, st, en, cost, sta, no);
    }

private:
    int         m_id, m_claimId, m_workshopId, m_inspectionId, m_workshopMgrId;
    std::string m_startDate, m_endDate;
    double      m_repairCost;
    std::string m_status, m_notes;
};

// ════════════════════════════════════════════════════════════
//  SECTION 3 – GENERIC FILE REPOSITORY
// ════════════════════════════════════════════════════════════

template<typename T>
class FileRepo {
public:
    explicit FileRepo(const std::string& path) : m_path(path) {}

    std::vector<T> loadAll() const {
        std::vector<T> recs;
        std::ifstream f(m_path);
        if (!f.is_open()) return recs;
        std::string line;
        while (std::getline(f, line))
            if (!line.empty()) {
                try { recs.push_back(T::deserialize(line)); } catch (...) {}
            }
        return recs;
    }

    void saveAll(const std::vector<T>& recs) const {
        std::ofstream f(m_path, std::ios::trunc);
        for (const auto& r : recs) f << r.serialize() << '\n';
    }

    bool add(const T& item) {
        auto recs = loadAll();
        recs.push_back(item);
        saveAll(recs);
        return true;
    }

    bool update(const T& item) {
        auto recs = loadAll();
        for (auto& r : recs)
            if (r.getId() == item.getId()) { r = item; saveAll(recs); return true; }
        return false;
    }

    bool remove(int id) {
        auto recs = loadAll();
        auto it = std::remove_if(recs.begin(), recs.end(),
                                  [id](const T& r){ return r.getId() == id; });
        if (it == recs.end()) return false;
        recs.erase(it, recs.end());
        saveAll(recs);
        return true;
    }

    T findById(int id) const {
        for (const auto& r : loadAll())
            if (r.getId() == id) return r;
        return T();
    }

    int nextId() const {
        int mx = 0;
        for (const auto& r : loadAll()) if (r.getId() > mx) mx = r.getId();
        return mx + 1;
    }

private:
    std::string m_path;
};

// ════════════════════════════════════════════════════════════
//  SECTION 4 – DATABASE
// ════════════════════════════════════════════════════════════

struct DB {
    FileRepo<Staff>          staff    {"data/staff.dat"};
    FileRepo<Customer>       customer {"data/customers.dat"};
    FileRepo<Vehicle>        vehicle  {"data/vehicles.dat"};
    FileRepo<Workshop>       workshop {"data/workshops.dat"};
    FileRepo<InsurancePolicy>policy   {"data/policies.dat"};
    FileRepo<Claim>          claim    {"data/claims.dat"};
    FileRepo<Inspection>     insp     {"data/inspections.dat"};
    FileRepo<Repair>         repair   {"data/repairs.dat"};

    // ── Staff queries ──────────────────────────────
    Staff findStaffByUsername(const std::string& u) const {
        for (const auto& s : staff.loadAll())
            if (s.getUsername() == u) return s;
        return Staff();
    }

    std::vector<Staff> staffByRole(const std::string& role) const {
        std::vector<Staff> out;
        for (const auto& s : staff.loadAll())
            if (s.getRole() == role) out.push_back(s);
        return out;
    }

    // ── Customer queries ───────────────────────────
    Customer findCustomerByCnic(const std::string& cnic) const {
        for (const auto& c : customer.loadAll())
            if (c.getCnic() == cnic) return c;
        return Customer();
    }

    std::vector<Customer> customersByMonth(const std::string& ym) const {
        std::vector<Customer> out;
        for (const auto& c : customer.loadAll())
            if (c.getRegDate().substr(0,7) == ym) out.push_back(c);
        return out;
    }

    // ── Vehicle queries ────────────────────────────
    Vehicle findVehicleByRegNo(const std::string& rn) const {
        for (const auto& v : vehicle.loadAll())
            if (v.getRegNo() == rn) return v;
        return Vehicle();
    }

    std::vector<Vehicle> vehiclesByCustomer(int cid) const {
        std::vector<Vehicle> out;
        for (const auto& v : vehicle.loadAll())
            if (v.getCustomerId() == cid) out.push_back(v);
        return out;
    }

    // ── Workshop queries ───────────────────────────
    Workshop workshopByManager(int staffId) const {
        for (const auto& w : workshop.loadAll())
            if (w.getManagerId() == staffId) return w;
        return Workshop();
    }

    bool workshopHasManager(int workshopId) const {
        Workshop w = workshop.findById(workshopId);
        return w.getId() != 0 && w.getManagerId() != 0;
    }

    // ── Policy queries ─────────────────────────────
    std::vector<InsurancePolicy> policiesByCustomer(int cid) const {
        std::vector<InsurancePolicy> out;
        for (const auto& p : policy.loadAll())
            if (p.getCustomerId() == cid) out.push_back(p);
        return out;
    }

    // ── Claim queries ──────────────────────────────
    Claim findClaimByNo(const std::string& no) const {
        for (const auto& c : claim.loadAll())
            if (c.getClaimNo() == no) return c;
        return Claim();
    }

    std::vector<Claim> claimsByCustomer(int cid) const {
        std::vector<Claim> out;
        for (const auto& c : claim.loadAll())
            if (c.getCustomerId() == cid) out.push_back(c);
        return out;
    }

    std::vector<Claim> claimsByStatus(const std::string& status) const {
        std::vector<Claim> out;
        for (const auto& c : claim.loadAll())
            if (c.getStatus() == status) out.push_back(c);
        return out;
    }

    // ── Inspection queries ─────────────────────────
    std::vector<Inspection> inspectionsByClaim(int claimId) const {
        std::vector<Inspection> out;
        for (const auto& i : insp.loadAll())
            if (i.getClaimId() == claimId) out.push_back(i);
        return out;
    }

    std::vector<Inspection> inspectionsBySurveyor(int surveyorId) const {
        std::vector<Inspection> out;
        for (const auto& i : insp.loadAll())
            if (i.getSurveyorId() == surveyorId) out.push_back(i);
        return out;
    }

    // ── Repair queries ─────────────────────────────
    std::vector<Repair> repairsByClaim(int claimId) const {
        std::vector<Repair> out;
        for (const auto& r : repair.loadAll())
            if (r.getClaimId() == claimId) out.push_back(r);
        return out;
    }

    std::vector<Repair> repairsByWorkshop(int workshopId) const {
        std::vector<Repair> out;
        for (const auto& r : repair.loadAll())
            if (r.getWorkshopId() == workshopId) out.push_back(r);
        return out;
    }

    bool hasCompletedInspection(int claimId) const {
        for (const auto& i : inspectionsByClaim(claimId))
            if (i.getStatus() == "COMPLETED") return true;
        return false;
    }
};

// ════════════════════════════════════════════════════════════
//  SECTION 5 – BUSINESS SERVICES
// ════════════════════════════════════════════════════════════

// ─── Auth ────────────────────────────────────────────────────
static Staff login(DB& db, const std::string& username, const std::string& password) {
    Staff s = db.findStaffByUsername(username);
    if (s.getId() == 0 || s.getPassword() != password) return Staff();
    return s;
}

// ─── Customer Service ────────────────────────────────────────
static Customer registerCustomer(DB& db,
    const std::string& name, const std::string& cnic,
    const std::string& phone, const std::string& email,
    const std::string& address)
{
    if (db.findCustomerByCnic(cnic).getId() != 0)
        throw std::runtime_error("A customer with this CNIC already exists.");
    int id = db.customer.nextId();
    Customer c(id, name, cnic, phone, email, address, todayStr());
    db.customer.add(c);
    return c;
}

// ─── Vehicle Service ─────────────────────────────────────────
static Vehicle registerVehicle(DB& db,
    int customerId, const std::string& regNo,
    const std::string& make, const std::string& model, int year,
    const std::string& color, const std::string& chassis,
    const std::string& engine)
{
    if (db.customer.findById(customerId).getId() == 0)
        throw std::runtime_error("Customer not found.");
    if (db.findVehicleByRegNo(regNo).getId() != 0)
        throw std::runtime_error("A vehicle with this registration number already exists.");
    int id = db.vehicle.nextId();
    Vehicle v(id, customerId, regNo, make, model, year, color, chassis, engine);
    db.vehicle.add(v);
    return v;
}

// ─── Policy Service ──────────────────────────────────────────
static std::string genPolicyNo(DB& db) {
    return "POL-" + std::to_string(db.policy.nextId()) + "-" + todayStr().substr(0,4);
}

static InsurancePolicy issuePolicy(DB& db,
    int customerId, int vehicleId, int salesmanId,
    const std::string& type, double premium, double coverage,
    const std::string& startDate, const std::string& endDate)
{
    if (db.customer.findById(customerId).getId() == 0)
        throw std::runtime_error("Customer not found.");
    if (db.vehicle.findById(vehicleId).getId() == 0)
        throw std::runtime_error("Vehicle not found.");
    int id = db.policy.nextId();
    InsurancePolicy p(id, customerId, vehicleId, salesmanId,
                      genPolicyNo(db), type, premium, coverage,
                      startDate, endDate, "ACTIVE");
    db.policy.add(p);
    return p;
}

// ─── Claim Service ───────────────────────────────────────────
static std::string genClaimNo(DB& db) {
    return "CLM-" + std::to_string(db.claim.nextId()) + "-" + todayStr().substr(0,4);
}

static Claim fileClaim(DB& db,
    int policyId, const std::string& incidentDate,
    const std::string& description, double estimatedLoss)
{
    InsurancePolicy p = db.policy.findById(policyId);
    if (p.getId() == 0)       throw std::runtime_error("Policy not found.");
    if (p.getStatus() != "ACTIVE") throw std::runtime_error("Policy is not active.");
    int id = db.claim.nextId();
    Claim c(id, policyId, p.getCustomerId(), p.getVehicleId(),
            genClaimNo(db), incidentDate, todayStr(),
            description, estimatedLoss, "PENDING");
    db.claim.add(c);
    return c;
}

// ─── Inspection Service ──────────────────────────────────────
static Inspection scheduleInspection(DB& db,
    int claimId, int surveyorId, int workshopId,
    const std::string& inspDate)
{
    if (db.claim.findById(claimId).getId() == 0)
        throw std::runtime_error("Claim not found.");
    if (db.workshop.findById(workshopId).getId() == 0)
        throw std::runtime_error("Workshop not found. Must use a registered workshop.");
    int id = db.insp.nextId();
    Inspection insp(id, claimId, surveyorId, workshopId, inspDate, "", 0.0, "", "SCHEDULED");
    db.insp.add(insp);
    // Advance claim status
    Claim cl = db.claim.findById(claimId);
    cl.setStatus("UNDER_REVIEW");
    db.claim.update(cl);
    return insp;
}

static bool submitInspectionReport(DB& db,
    int inspectionId, const std::string& findings,
    double repairCost, const std::string& recommendation)
{
    Inspection insp = db.insp.findById(inspectionId);
    if (insp.getId() == 0) return false;
    insp.setFindings(findings);
    insp.setRepairCost(repairCost);
    insp.setRecommendation(recommendation);
    insp.setStatus("COMPLETED");
    return db.insp.update(insp);
}

// ─── Repair Service (Workshop Manager driven) ────────────────
static Repair logRepair(DB& db,
    int claimId, int workshopId, int inspectionId,
    int workshopMgrId, const std::string& startDate,
    const std::string& notes)
{
    // Enforce: only the assigned manager of THIS workshop may log repairs for it
    Workshop w = db.workshop.findById(workshopId);
    if (w.getId() == 0)
        throw std::runtime_error("Workshop not found.");
    if (w.getManagerId() != workshopMgrId)
        throw std::runtime_error("You are not the assigned manager of this workshop.");

    int id = db.repair.nextId();
    Repair r(id, claimId, workshopId, inspectionId, workshopMgrId,
             startDate, "", 0.0, "IN_PROGRESS", notes);
    db.repair.add(r);
    return r;
}

static bool completeRepair(DB& db,
    int repairId, const std::string& endDate, double finalCost)
{
    Repair r = db.repair.findById(repairId);
    if (r.getId() == 0) return false;
    r.setStatus("COMPLETED");
    r.setEndDate(endDate);
    r.setRepairCost(finalCost);
    return db.repair.update(r);
}

// ─── Workshop Manager Assignment ────────────────────────────
static bool assignWorkshopManager(DB& db, int workshopId, int staffId) {
    Workshop w = db.workshop.findById(workshopId);
    if (w.getId() == 0) throw std::runtime_error("Workshop not found.");
    Staff s = db.staff.findById(staffId);
    if (s.getId() == 0) throw std::runtime_error("Staff not found.");
    if (s.getRole() != "WORKSHOP_MGR")
        throw std::runtime_error("Staff member must have role WORKSHOP_MGR.");
    // Ensure workshop doesn't already have another manager
    if (w.getManagerId() != 0 && w.getManagerId() != staffId)
        throw std::runtime_error("This workshop already has a different manager assigned.");
    // Ensure this staff member isn't already managing another workshop
    Workshop existing = db.workshopByManager(staffId);
    if (existing.getId() != 0 && existing.getId() != workshopId)
        throw std::runtime_error("This staff member is already managing workshop: " + existing.getName());

    w.setManagerId(staffId);
    db.workshop.update(w);
    s.setWorkshopId(workshopId);
    db.staff.update(s);
    return true;
}

static bool unassignWorkshopManager(DB& db, int workshopId) {
    Workshop w = db.workshop.findById(workshopId);
    if (w.getId() == 0) return false;
    int oldMgrId = w.getManagerId();
    w.setManagerId(0);
    db.workshop.update(w);
    if (oldMgrId != 0) {
        Staff s = db.staff.findById(oldMgrId);
        if (s.getId() != 0) { s.setWorkshopId(0); db.staff.update(s); }
    }
    return true;
}

// ─── Claim Approval (Admin only) ─────────────────────────────
static bool approveClaim(DB& db, int claimId) {
    if (!db.hasCompletedInspection(claimId))
        throw std::runtime_error("Cannot approve: no completed inspection report found.");
    Claim c = db.claim.findById(claimId);
    if (c.getId() == 0) return false;
    c.setStatus("APPROVED");
    return db.claim.update(c);
}

static bool rejectClaim(DB& db, int claimId) {
    Claim c = db.claim.findById(claimId);
    if (c.getId() == 0) return false;
    c.setStatus("REJECTED");
    return db.claim.update(c);
}

// ════════════════════════════════════════════════════════════
//  SECTION 6 – REPORT GENERATORS
// ════════════════════════════════════════════════════════════

static void rptNewCustomers(DB& db, const std::string& ym) {
    auto list = db.customersByMonth(ym);
    printHeader("NEW CUSTOMERS REPORT [" + ym + "]");
    std::cout << "  Total: " << list.size() << "\n\n";
    std::cout << std::left << std::setw(5) << "ID"
              << std::setw(22) << "Name"
              << std::setw(18) << "CNIC"
              << std::setw(14) << "Phone"
              << "Reg Date\n";
    printLine();
    for (const auto& c : list)
        std::cout << std::setw(5)  << c.getId()
                  << std::setw(22) << c.getName()
                  << std::setw(18) << c.getCnic()
                  << std::setw(14) << c.getPhone()
                  << c.getRegDate() << "\n";
}

static void rptPendingClaims(DB& db) {
    printHeader("PENDING / UNDER-REVIEW CLAIMS");
    auto p1 = db.claimsByStatus("PENDING");
    auto p2 = db.claimsByStatus("UNDER_REVIEW");
    p1.insert(p1.end(), p2.begin(), p2.end());
    std::cout << "  Total: " << p1.size() << "\n\n";
    std::cout << std::left << std::setw(6)  << "ID"
              << std::setw(14) << "Claim No"
              << std::setw(13) << "Filed Date"
              << std::setw(16) << "Status"
              << "Est. Loss (PKR)\n";
    printLine();
    for (const auto& c : p1)
        std::cout << std::setw(6)  << c.getId()
                  << std::setw(14) << c.getClaimNo()
                  << std::setw(13) << c.getFiledDate()
                  << std::setw(16) << c.getStatus()
                  << c.getEstimatedLoss() << "\n";
}

static void rptInspection(DB& db, int claimId) {
    printHeader("INSPECTION REPORT");
    Claim cl = db.claim.findById(claimId);
    if (cl.getId() == 0) { std::cout << "  Claim not found.\n"; return; }
    std::cout << "  Claim No    : " << cl.getClaimNo()      << "\n"
              << "  Filed Date  : " << cl.getFiledDate()    << "\n"
              << "  Incident    : " << cl.getIncidentDate() << "\n"
              << "  Description : " << cl.getDescription()  << "\n"
              << "  Est. Loss   : PKR " << cl.getEstimatedLoss() << "\n"
              << "  Status      : " << cl.getStatus()       << "\n\n";
    auto insps = db.inspectionsByClaim(claimId);
    if (insps.empty()) { std::cout << "  No inspections recorded.\n"; return; }
    for (const auto& i : insps) {
        Staff sv = db.staff.findById(i.getSurveyorId());
        Workshop w = db.workshop.findById(i.getWorkshopId());
        printLine('-', 50);
        std::cout << "  Inspection ID  : " << i.getId()          << "\n"
                  << "  Date           : " << i.getInspDate()    << "\n"
                  << "  Surveyor       : " << sv.getName()       << "\n"
                  << "  Workshop       : " << w.getName()        << "\n"
                  << "  Findings       : " << i.getFindings()    << "\n"
                  << "  Repair Cost    : PKR " << i.getRepairCost() << "\n"
                  << "  Recommendation : " << i.getRecommendation() << "\n"
                  << "  Status         : " << i.getStatus()      << "\n";
    }
}

static void rptCustomerClaims(DB& db, int customerId) {
    Customer cust = db.customer.findById(customerId);
    printHeader("CLAIMS HISTORY – " + cust.getName());
    auto list = db.claimsByCustomer(customerId);
    std::cout << "  Total Claims: " << list.size() << "\n\n";
    std::cout << std::left << std::setw(6)  << "ID"
              << std::setw(14) << "Claim No"
              << std::setw(13) << "Incident"
              << std::setw(16) << "Status"
              << "Est. Loss\n";
    printLine();
    for (const auto& c : list)
        std::cout << std::setw(6)  << c.getId()
                  << std::setw(14) << c.getClaimNo()
                  << std::setw(13) << c.getIncidentDate()
                  << std::setw(16) << c.getStatus()
                  << c.getEstimatedLoss() << "\n";
}

static void rptAllPolicies(DB& db) {
    printHeader("ALL POLICIES");
    auto list = db.policy.loadAll();
    std::cout << "  Total: " << list.size() << "\n\n";
    std::cout << std::left << std::setw(5)  << "ID"
              << std::setw(16) << "Policy No"
              << std::setw(15) << "Type"
              << std::setw(12) << "Status"
              << std::setw(12) << "Start"
              << "End\n";
    printLine();
    for (const auto& p : list)
        std::cout << std::setw(5)  << p.getId()
                  << std::setw(16) << p.getPolicyNo()
                  << std::setw(15) << p.getType()
                  << std::setw(12) << p.getStatus()
                  << std::setw(12) << p.getStartDate()
                  << p.getEndDate() << "\n";
}

static void rptWorkshopRepairs(DB& db, int workshopId) {
    Workshop w = db.workshop.findById(workshopId);
    Staff mgr = db.staff.findById(w.getManagerId());
    printHeader("REPAIRS REPORT – " + w.getName());
    std::cout << "  Workshop Manager: "
              << (mgr.getId() ? mgr.getName() : "UNASSIGNED") << "\n\n";
    auto list = db.repairsByWorkshop(workshopId);
    std::cout << "  Total Repairs: " << list.size() << "\n\n";
    std::cout << std::left << std::setw(5)  << "ID"
              << std::setw(9)  << "ClaimID"
              << std::setw(13) << "Start"
              << std::setw(13) << "End"
              << std::setw(14) << "Status"
              << "Cost (PKR)\n";
    printLine();
    for (const auto& r : list)
        std::cout << std::setw(5)  << r.getId()
                  << std::setw(9)  << r.getClaimId()
                  << std::setw(13) << r.getStartDate()
                  << std::setw(13) << r.getEndDate()
                  << std::setw(14) << r.getStatus()
                  << r.getRepairCost() << "\n";
}

// ════════════════════════════════════════════════════════════
//  SECTION 7 – SHARED DISPLAY HELPERS
// ════════════════════════════════════════════════════════════

static void showAllCustomers(DB& db) {
    printHeader("ALL CUSTOMERS");
    auto list = db.customer.loadAll();
    std::cout << std::left << std::setw(5)  << "ID"
              << std::setw(22) << "Name"
              << std::setw(18) << "CNIC"
              << std::setw(14) << "Phone"
              << "Reg Date\n";
    printLine();
    for (const auto& c : list)
        std::cout << std::setw(5)  << c.getId()
                  << std::setw(22) << c.getName()
                  << std::setw(18) << c.getCnic()
                  << std::setw(14) << c.getPhone()
                  << c.getRegDate() << "\n";
    std::cout << "  Total: " << list.size() << "\n";
    pause();
}

static void showAllVehicles(DB& db) {
    printHeader("ALL VEHICLES");
    auto list = db.vehicle.loadAll();
    std::cout << std::left << std::setw(5)  << "ID"
              << std::setw(11) << "Reg No"
              << std::setw(11) << "Make"
              << std::setw(11) << "Model"
              << std::setw(6)  << "Year"
              << "Cust ID\n";
    printLine();
    for (const auto& v : list)
        std::cout << std::setw(5)  << v.getId()
                  << std::setw(11) << v.getRegNo()
                  << std::setw(11) << v.getMake()
                  << std::setw(11) << v.getModel()
                  << std::setw(6)  << v.getYear()
                  << v.getCustomerId() << "\n";
    std::cout << "  Total: " << list.size() << "\n";
    pause();
}

static void showAllWorkshops(DB& db) {
    printHeader("REGISTERED WORKSHOPS");
    auto list = db.workshop.loadAll();
    std::cout << std::left << std::setw(5)  << "ID"
              << std::setw(24) << "Name"
              << std::setw(14) << "Phone"
              << std::setw(20) << "Owner"
              << "Manager\n";
    printLine();
    for (const auto& w : list) {
        Staff mgr = db.staff.findById(w.getManagerId());
        std::string mgrName = (mgr.getId() != 0) ? mgr.getName() : "** UNASSIGNED **";
        std::cout << std::setw(5)  << w.getId()
                  << std::setw(24) << w.getName()
                  << std::setw(14) << w.getPhone()
                  << std::setw(20) << w.getOwnerName()
                  << mgrName << "\n";
    }
    std::cout << "  Total: " << list.size() << "\n";
}

static void showAllStaff(DB& db) {
    printHeader("ALL STAFF");
    auto list = db.staff.loadAll();
    std::cout << std::left << std::setw(5)  << "ID"
              << std::setw(22) << "Name"
              << std::setw(15) << "Role"
              << std::setw(14) << "Username"
              << "Workshop\n";
    printLine();
    for (const auto& s : list) {
        std::string wName = "";
        if (s.getRole() == "WORKSHOP_MGR" && s.getWorkshopId() != 0) {
            Workshop w = db.workshop.findById(s.getWorkshopId());
            wName = w.getName();
        }
        std::cout << std::setw(5)  << s.getId()
                  << std::setw(22) << s.getName()
                  << std::setw(15) << s.getRole()
                  << std::setw(14) << s.getUsername()
                  << wName << "\n";
    }
    std::cout << "  Total: " << list.size() << "\n";
    pause();
}

static void showAllPolicies(DB& db) {
    rptAllPolicies(db);
    pause();
}

static void showAllClaims(DB& db) {
    printHeader("ALL CLAIMS");
    auto list = db.claim.loadAll();
    std::cout << std::left << std::setw(5)  << "ID"
              << std::setw(14) << "Claim No"
              << std::setw(13) << "Filed"
              << std::setw(16) << "Status"
              << "Est. Loss\n";
    printLine();
    for (const auto& c : list)
        std::cout << std::setw(5)  << c.getId()
                  << std::setw(14) << c.getClaimNo()
                  << std::setw(13) << c.getFiledDate()
                  << std::setw(16) << c.getStatus()
                  << c.getEstimatedLoss() << "\n";
    std::cout << "  Total: " << list.size() << "\n";
    pause();
}

// ════════════════════════════════════════════════════════════
//  SECTION 8 – MENU FUNCTIONS
// ════════════════════════════════════════════════════════════

// ─── ADMIN MENUS ─────────────────────────────────────────────
static void adminCustomerMenu(DB& db) {
    int ch;
    do {
        printHeader("CUSTOMER MANAGEMENT");
        std::cout << "  1. View All Customers\n"
                  << "  2. Search by CNIC\n"
                  << "  3. View Customer Details + Vehicles\n"
                  << "  0. Back\n";
        ch = readInt("  Choice: ");
        if (ch == 1) {
            showAllCustomers(db);
        } else if (ch == 2) {
            std::string cnic = readStr("  CNIC: ");
            Customer c = db.findCustomerByCnic(cnic);
            if (c.getId() == 0) std::cout << "  Not found.\n";
            else std::cout << "  [" << c.getId() << "] " << c.getName()
                           << " | " << c.getPhone() << " | " << c.getRegDate() << "\n";
            pause();
        } else if (ch == 3) {
            int id = readInt("  Customer ID: ");
            Customer c = db.customer.findById(id);
            if (c.getId() == 0) { std::cout << "  Not found.\n"; pause(); continue; }
            printLine();
            std::cout << "  ID      : " << c.getId()      << "\n"
                      << "  Name    : " << c.getName()    << "\n"
                      << "  CNIC    : " << c.getCnic()    << "\n"
                      << "  Phone   : " << c.getPhone()   << "\n"
                      << "  Email   : " << c.getEmail()   << "\n"
                      << "  Address : " << c.getAddress() << "\n"
                      << "  Reg Date: " << c.getRegDate() << "\n";
            auto vehs = db.vehiclesByCustomer(id);
            std::cout << "\n  Vehicles (" << vehs.size() << "):\n";
            for (const auto& v : vehs)
                std::cout << "    [" << v.getId() << "] " << v.getRegNo()
                          << "  " << v.getMake() << " " << v.getModel()
                          << " " << v.getYear() << "\n";
            pause();
        }
    } while (ch != 0);
}

static void adminStaffMenu(DB& db) {
    int ch;
    do {
        printHeader("STAFF MANAGEMENT");
        std::cout << "  1. View All Staff\n"
                  << "  2. Add Staff Member\n"
                  << "  3. View by Role\n"
                  << "  0. Back\n";
        ch = readInt("  Choice: ");
        if (ch == 1) {
            showAllStaff(db);
        } else if (ch == 2) {
            std::cout << "\n  Roles: ADMIN | SALESMAN | SURVEYOR | WORKSHOP_MGR\n";
            std::string name  = readStr("  Full Name    : ");
            std::string role  = readStr("  Role         : ");
            std::string uname = readStr("  Username     : ");
            std::string pass  = readStr("  Password     : ");
            std::string phone = readStr("  Phone        : ");
            std::string email = readStr("  Email        : ");
            // Duplicate username check
            if (db.findStaffByUsername(uname).getId() != 0) {
                std::cout << "  [!] Username already taken.\n";
            } else {
                int id = db.staff.nextId();
                Staff s(id, name, role, uname, pass, phone, email, 0);
                db.staff.add(s);
                std::cout << "  Staff added. ID=" << id << "\n";
            }
            pause();
        } else if (ch == 3) {
            std::string role = readStr("  Role to filter (e.g. SURVEYOR): ");
            auto list = db.staffByRole(role);
            for (const auto& s : list)
                std::cout << "  [" << s.getId() << "] " << s.getName()
                          << " | " << s.getUsername() << " | " << s.getPhone() << "\n";
            if (list.empty()) std::cout << "  None found.\n";
            pause();
        }
    } while (ch != 0);
}

static void adminWorkshopMenu(DB& db) {
    int ch;
    do {
        printHeader("WORKSHOP MANAGEMENT");
        std::cout << "  1. View All Workshops (with managers)\n"
                  << "  2. Add Workshop\n"
                  << "  3. Assign Manager to Workshop\n"
                  << "  4. Unassign Manager from Workshop\n"
                  << "  5. Remove Workshop\n"
                  << "  0. Back\n";
        ch = readInt("  Choice: ");

        if (ch == 1) {
            showAllWorkshops(db);
            pause();
        } else if (ch == 2) {
            std::string name  = readStr("  Workshop Name : ");
            std::string addr  = readStr("  Address       : ");
            std::string phone = readStr("  Phone         : ");
            std::string owner = readStr("  Owner Name    : ");
            int id = db.workshop.nextId();
            Workshop w(id, name, addr, phone, owner, 0);
            db.workshop.add(w);
            std::cout << "  Workshop added. ID=" << id
                      << "  (No manager assigned yet)\n";
            pause();
        } else if (ch == 3) {
            showAllWorkshops(db);
            int wid = readInt("\n  Workshop ID to assign manager to: ");
            // Show available WORKSHOP_MGR staff
            std::cout << "\n  Available Workshop Managers (WORKSHOP_MGR role):\n";
            auto mgrs = db.staffByRole("WORKSHOP_MGR");
            if (mgrs.empty()) {
                std::cout << "  No WORKSHOP_MGR staff found. Add one first.\n";
                pause(); continue;
            }
            for (const auto& s : mgrs) {
                Workshop assigned = db.workshopByManager(s.getId());
                std::string info = (assigned.getId() != 0)
                    ? " [currently manages: " + assigned.getName() + "]" : " [unassigned]";
                std::cout << "    [" << s.getId() << "] " << s.getName() << info << "\n";
            }
            int sid = readInt("  Staff ID of manager to assign: ");
            try {
                assignWorkshopManager(db, wid, sid);
                Staff s = db.staff.findById(sid);
                Workshop w = db.workshop.findById(wid);
                std::cout << "  ✓ " << s.getName() << " assigned as manager of "
                          << w.getName() << ".\n";
            } catch (const std::exception& e) {
                std::cout << "  [!] " << e.what() << "\n";
            }
            pause();
        } else if (ch == 4) {
            showAllWorkshops(db);
            int wid = readInt("\n  Workshop ID to unassign manager from: ");
            if (unassignWorkshopManager(db, wid))
                std::cout << "  Manager unassigned.\n";
            else
                std::cout << "  Workshop not found.\n";
            pause();
        } else if (ch == 5) {
            int wid = readInt("  Workshop ID to remove: ");
            // Unassign manager first
            unassignWorkshopManager(db, wid);
            if (db.workshop.remove(wid)) std::cout << "  Workshop removed.\n";
            else std::cout << "  Not found.\n";
            pause();
        }
    } while (ch != 0);
}

static void adminClaimMenu(DB& db) {
    int ch;
    do {
        printHeader("CLAIM MANAGEMENT");
        std::cout << "  1. View All Claims\n"
                  << "  2. View Pending / Under-Review Claims\n"
                  << "  3. Approve Claim  (requires completed inspection)\n"
                  << "  4. Reject Claim\n"
                  << "  5. View Claims by Customer\n"
                  << "  6. Schedule Inspection\n"
                  << "  0. Back\n";
        ch = readInt("  Choice: ");

        if (ch == 1) {
            showAllClaims(db);
        } else if (ch == 2) {
            rptPendingClaims(db);
            pause();
        } else if (ch == 3) {
            int cid = readInt("  Claim ID to APPROVE: ");
            try {
                approveClaim(db, cid);
                std::cout << "  Claim APPROVED.\n";
            } catch (const std::exception& e) {
                std::cout << "  [!] " << e.what() << "\n";
            }
            pause();
        } else if (ch == 4) {
            int cid = readInt("  Claim ID to REJECT: ");
            if (rejectClaim(db, cid)) std::cout << "  Claim REJECTED.\n";
            else std::cout << "  Claim not found.\n";
            pause();
        } else if (ch == 5) {
            int custId = readInt("  Customer ID: ");
            auto list = db.claimsByCustomer(custId);
            if (list.empty()) std::cout << "  No claims found.\n";
            for (const auto& c : list)
                std::cout << "  [" << c.getId() << "] " << c.getClaimNo()
                          << " | " << c.getStatus()
                          << " | Loss: " << c.getEstimatedLoss() << "\n";
            pause();
        } else if (ch == 6) {
            showAllClaims(db);
            int claimId = readInt("  Claim ID: ");
            // Show surveyors
            auto survs = db.staffByRole("SURVEYOR");
            std::cout << "\n  Surveyors:\n";
            for (const auto& s : survs)
                std::cout << "    [" << s.getId() << "] " << s.getName() << "\n";
            int survId = readInt("  Surveyor ID: ");
            showAllWorkshops(db);
            int wid = readInt("\n  Workshop ID: ");
            std::string date = readStr("  Inspection Date (YYYY-MM-DD): ");
            try {
                Inspection insp = scheduleInspection(db, claimId, survId, wid, date);
                std::cout << "  Inspection scheduled. ID=" << insp.getId() << "\n";
            } catch (const std::exception& e) {
                std::cout << "  [!] " << e.what() << "\n";
            }
            pause();
        }
    } while (ch != 0);
}

static void adminRepairMenu(DB& db) {
    int ch;
    do {
        printHeader("REPAIR MANAGEMENT");
        std::cout << "  1. View All Repairs\n"
                  << "  2. View Repairs by Workshop\n"
                  << "  0. Back\n";
        ch = readInt("  Choice: ");
        if (ch == 1) {
            auto list = db.repair.loadAll();
            printHeader("ALL REPAIRS");
            std::cout << std::left << std::setw(5)  << "ID"
                      << std::setw(9) << "ClaimID"
                      << std::setw(11) << "Workshop"
                      << std::setw(13) << "Start"
                      << std::setw(14) << "Status"
                      << "Cost\n";
            printLine();
            for (const auto& r : list)
                std::cout << std::setw(5)  << r.getId()
                          << std::setw(9)  << r.getClaimId()
                          << std::setw(11) << r.getWorkshopId()
                          << std::setw(13) << r.getStartDate()
                          << std::setw(14) << r.getStatus()
                          << r.getRepairCost() << "\n";
            pause();
        } else if (ch == 2) {
            showAllWorkshops(db);
            int wid = readInt("\n  Workshop ID: ");
            rptWorkshopRepairs(db, wid);
            pause();
        }
    } while (ch != 0);
}

static void adminReportMenu(DB& db) {
    int ch;
    do {
        printHeader("REPORTS");
        std::cout << "  1. New Customers in a Month\n"
                  << "  2. Pending Claims\n"
                  << "  3. Inspection Report (by Claim ID)\n"
                  << "  4. Customer Claims History\n"
                  << "  5. All Policies\n"
                  << "  6. Workshop Repairs Report\n"
                  << "  0. Back\n";
        ch = readInt("  Choice: ");
        if (ch == 1) {
            std::string ym = readStr("  Year-Month (YYYY-MM): ");
            rptNewCustomers(db, ym);  pause();
        } else if (ch == 2) {
            rptPendingClaims(db);     pause();
        } else if (ch == 3) {
            int cid = readInt("  Claim ID: ");
            rptInspection(db, cid);   pause();
        } else if (ch == 4) {
            int cid = readInt("  Customer ID: ");
            rptCustomerClaims(db, cid); pause();
        } else if (ch == 5) {
            showAllPolicies(db);
        } else if (ch == 6) {
            showAllWorkshops(db);
            int wid = readInt("\n  Workshop ID: ");
            rptWorkshopRepairs(db, wid); pause();
        }
    } while (ch != 0);
}

static void adminMenu(DB& db, const Staff& /*user*/) {
    int ch;
    do {
        printHeader("ADMIN MENU");
        std::cout << "  1. Customer Management\n"
                  << "  2. Policy Management\n"
                  << "  3. Claim Management\n"
                  << "  4. Reports\n"
                  << "  5. Staff Management\n"
                  << "  6. Workshop Management\n"
                  << "  7. Repair Overview\n"
                  << "  0. Logout\n";
        ch = readInt("  Choice: ");
        switch (ch) {
            case 1: adminCustomerMenu(db); break;
            case 2: showAllPolicies(db);   break;
            case 3: adminClaimMenu(db);    break;
            case 4: adminReportMenu(db);   break;
            case 5: adminStaffMenu(db);    break;
            case 6: adminWorkshopMenu(db); break;
            case 7: adminRepairMenu(db);   break;
            case 0: break;
            default: std::cout << "  Invalid.\n";
        }
    } while (ch != 0);
}

// ─── SALESMAN MENUS ───────────────────────────────────────────
static void salesmanMenu(DB& db, const Staff& user) {
    int ch;
    do {
        printHeader("SALESMAN MENU  –  " + user.getName());
        std::cout << "  1. Register New Customer\n"
                  << "  2. Update Customer Info\n"
                  << "  3. Register Vehicle\n"
                  << "  4. View Customer Vehicles\n"
                  << "  5. Issue Policy\n"
                  << "  6. View Customer Policies\n"
                  << "  7. File a Claim\n"
                  << "  8. View All Customers\n"
                  << "  9. View All Vehicles\n"
                  << "  0. Logout\n";
        ch = readInt("  Choice: ");

        if (ch == 1) {
            std::string name  = readStr("  Full Name    : ");
            std::string cnic  = readStr("  CNIC         : ");
            std::string phone = readStr("  Phone        : ");
            std::string email = readStr("  Email        : ");
            std::string addr  = readStr("  Address      : ");
            try {
                Customer c = registerCustomer(db, name, cnic, phone, email, addr);
                std::cout << "  Customer registered. ID=" << c.getId() << "\n";
            } catch (const std::exception& e) {
                std::cout << "  [!] " << e.what() << "\n";
            }
            pause();
        } else if (ch == 2) {
            int id = readInt("  Customer ID: ");
            Customer c = db.customer.findById(id);
            if (c.getId() == 0) { std::cout << "  Not found.\n"; pause(); continue; }
            std::string phone = readStr("  New Phone (blank=keep): ");
            std::string email = readStr("  New Email (blank=keep): ");
            std::string addr  = readStr("  New Address (blank=keep): ");
            if (!phone.empty()) c.setPhone(phone);
            if (!email.empty()) c.setEmail(email);
            if (!addr.empty())  c.setAddress(addr);
            db.customer.update(c);
            std::cout << "  Updated.\n";
            pause();
        } else if (ch == 3) {
            int custId = readInt("  Customer ID  : ");
            std::string regNo = readStr("  Reg Number   : ");
            std::string make  = readStr("  Make         : ");
            std::string model = readStr("  Model        : ");
            int year          = readInt("  Year         : ");
            std::string color = readStr("  Color        : ");
            std::string ch_no = readStr("  Chassis No   : ");
            std::string eng   = readStr("  Engine No    : ");
            try {
                Vehicle v = registerVehicle(db, custId, regNo, make, model,
                                            year, color, ch_no, eng);
                std::cout << "  Vehicle registered. ID=" << v.getId() << "\n";
            } catch (const std::exception& e) {
                std::cout << "  [!] " << e.what() << "\n";
            }
            pause();
        } else if (ch == 4) {
            int cid = readInt("  Customer ID: ");
            auto vehs = db.vehiclesByCustomer(cid);
            for (const auto& v : vehs)
                std::cout << "  [" << v.getId() << "] " << v.getRegNo()
                          << "  " << v.getMake() << " " << v.getModel()
                          << " (" << v.getYear() << ")\n";
            if (vehs.empty()) std::cout << "  None found.\n";
            pause();
        } else if (ch == 5) {
            int custId = readInt("  Customer ID: ");
            auto vehs = db.vehiclesByCustomer(custId);
            if (vehs.empty()) {
                std::cout << "  No vehicles for this customer.\n"; pause(); continue;
            }
            std::cout << "  Vehicles:\n";
            for (const auto& v : vehs)
                std::cout << "    [" << v.getId() << "] " << v.getRegNo()
                          << "  " << v.getMake() << " " << v.getModel() << "\n";
            int vehId = readInt("  Vehicle ID  : ");
            std::cout << "  Types: COMPREHENSIVE | THIRD_PARTY\n";
            std::string type   = readStr("  Type         : ");
            double premium     = readDouble("  Premium/yr (PKR): ");
            double coverage    = readDouble("  Coverage (PKR)  : ");
            std::string start  = readStr("  Start Date (YYYY-MM-DD): ");
            std::string end    = readStr("  End Date   (YYYY-MM-DD): ");
            try {
                InsurancePolicy p = issuePolicy(db, custId, vehId,
                                                user.getId(), type,
                                                premium, coverage, start, end);
                std::cout << "  Policy issued. No=" << p.getPolicyNo() << "\n";
            } catch (const std::exception& e) {
                std::cout << "  [!] " << e.what() << "\n";
            }
            pause();
        } else if (ch == 6) {
            int custId = readInt("  Customer ID: ");
            auto pols = db.policiesByCustomer(custId);
            for (const auto& p : pols)
                std::cout << "  [" << p.getId() << "] " << p.getPolicyNo()
                          << " | " << p.getType() << " | " << p.getStatus()
                          << " | " << p.getStartDate() << "→" << p.getEndDate() << "\n";
            if (pols.empty()) std::cout << "  None found.\n";
            pause();
        } else if (ch == 7) {
            int custId = readInt("  Customer ID: ");
            auto pols = db.policiesByCustomer(custId);
            if (pols.empty()) {
                std::cout << "  No policies found.\n"; pause(); continue;
            }
            std::cout << "  Policies:\n";
            for (const auto& p : pols)
                std::cout << "    [" << p.getId() << "] " << p.getPolicyNo()
                          << " | " << p.getType() << " | " << p.getStatus() << "\n";
            int polId          = readInt("  Policy ID        : ");
            std::string incDate = readStr("  Incident Date    : ");
            std::string desc    = readStr("  Incident Desc    : ");
            double loss         = readDouble("  Estimated Loss   : ");
            try {
                Claim c = fileClaim(db, polId, incDate, desc, loss);
                std::cout << "  Claim filed. No=" << c.getClaimNo()
                          << "  ID=" << c.getId() << "\n";
            } catch (const std::exception& e) {
                std::cout << "  [!] " << e.what() << "\n";
            }
            pause();
        } else if (ch == 8) {
            showAllCustomers(db);
        } else if (ch == 9) {
            showAllVehicles(db);
        }
    } while (ch != 0);
}

// ─── SURVEYOR MENU ────────────────────────────────────────────
static void surveyorMenu(DB& db, const Staff& user) {
    int ch;
    do {
        printHeader("SURVEYOR MENU  –  " + user.getName());
        std::cout << "  1. View My Assigned Inspections\n"
                  << "  2. Submit Inspection Report\n"
                  << "  3. View Claim Details\n"
                  << "  0. Logout\n";
        ch = readInt("  Choice: ");

        if (ch == 1) {
            auto list = db.inspectionsBySurveyor(user.getId());
            printHeader("MY INSPECTIONS");
            if (list.empty()) std::cout << "  No inspections assigned to you.\n";
            for (const auto& i : list) {
                Workshop w = db.workshop.findById(i.getWorkshopId());
                std::cout << "  [" << i.getId() << "]"
                          << "  Claim:" << i.getClaimId()
                          << "  Date:" << i.getInspDate()
                          << "  Workshop:" << w.getName()
                          << "  Status:" << i.getStatus() << "\n";
            }
            pause();
        } else if (ch == 2) {
            // Only show SCHEDULED inspections assigned to this surveyor
            auto list = db.inspectionsBySurveyor(user.getId());
            std::cout << "\n  Your SCHEDULED inspections:\n";
            bool any = false;
            for (const auto& i : list)
                if (i.getStatus() == "SCHEDULED") {
                    std::cout << "    [" << i.getId() << "] Claim:" << i.getClaimId()
                              << "  Date:" << i.getInspDate() << "\n";
                    any = true;
                }
            if (!any) { std::cout << "  None.\n"; pause(); continue; }
            int iid = readInt("  Inspection ID   : ");
            // Verify it belongs to this surveyor
            Inspection insp = db.insp.findById(iid);
            if (insp.getId() == 0 || insp.getSurveyorId() != user.getId()) {
                std::cout << "  [!] Inspection not found or not assigned to you.\n";
                pause(); continue;
            }
            std::string findings = readStr("  Findings        : ");
            double cost          = readDouble("  Repair Cost (PKR): ");
            std::string rec      = readStr("  Recommendation  : ");
            if (submitInspectionReport(db, iid, findings, cost, rec))
                std::cout << "  Report submitted.\n";
            else
                std::cout << "  Failed.\n";
            pause();
        } else if (ch == 3) {
            int cid = readInt("  Claim ID: ");
            Claim c = db.claim.findById(cid);
            if (c.getId() == 0) { std::cout << "  Not found.\n"; pause(); continue; }
            printLine();
            std::cout << "  Claim No    : " << c.getClaimNo()      << "\n"
                      << "  Incident    : " << c.getIncidentDate() << "\n"
                      << "  Filed       : " << c.getFiledDate()    << "\n"
                      << "  Description : " << c.getDescription()  << "\n"
                      << "  Est. Loss   : PKR " << c.getEstimatedLoss() << "\n"
                      << "  Status      : " << c.getStatus()       << "\n";
            pause();
        }
    } while (ch != 0);
}

// ─── WORKSHOP MANAGER MENU ───────────────────────────────────
static void workshopMgrMenu(DB& db, const Staff& user) {
    // Find this manager's workshop
    Workshop myWorkshop = db.workshopByManager(user.getId());

    int ch;
    do {
        printHeader("WORKSHOP MANAGER MENU  –  " + user.getName());
        if (myWorkshop.getId() == 0) {
            std::cout << "  *** You are not currently assigned to any workshop. ***\n"
                      << "  *** Please ask the Admin to assign you to a workshop. ***\n\n";
        } else {
            std::cout << "  Your Workshop : " << myWorkshop.getName()
                      << "  [ID=" << myWorkshop.getId() << "]\n"
                      << "  Address       : " << myWorkshop.getAddress() << "\n\n";
        }
        std::cout << "  1. View My Workshop's Repairs\n"
                  << "  2. Log New Repair Job\n"
                  << "  3. Mark Repair as Complete\n"
                  << "  4. View Inspection Reports (for my workshop)\n"
                  << "  5. View Claim Details\n"
                  << "  0. Logout\n";
        ch = readInt("  Choice: ");

        if (myWorkshop.getId() == 0 && ch != 0) {
            std::cout << "  [!] No workshop assigned. Contact Admin.\n";
            pause(); continue;
        }

        if (ch == 1) {
            rptWorkshopRepairs(db, myWorkshop.getId());
            pause();
        } else if (ch == 2) {
            // Show claims that have a COMPLETED inspection at THIS workshop
            std::cout << "\n  Approved claims with completed inspections at "
                      << myWorkshop.getName() << ":\n";
            bool any = false;
            auto insps = db.insp.loadAll();
            for (const auto& i : insps) {
                if (i.getWorkshopId() == myWorkshop.getId() && i.getStatus() == "COMPLETED") {
                    Claim cl = db.claim.findById(i.getClaimId());
                    if (cl.getId() != 0 && cl.getStatus() == "APPROVED") {
                        std::cout << "    Claim [" << cl.getId() << "] "
                                  << cl.getClaimNo()
                                  << "  Insp ID:" << i.getId()
                                  << "  Cost: PKR " << i.getRepairCost() << "\n";
                        any = true;
                    }
                }
            }
            if (!any) {
                std::cout << "  No approved claims ready for repair.\n";
                pause(); continue;
            }
            int claimId  = readInt("  Claim ID       : ");
            int inspId   = readInt("  Inspection ID  : ");
            // Validate the inspection belongs to this workshop
            Inspection insp = db.insp.findById(inspId);
            if (insp.getId() == 0 || insp.getWorkshopId() != myWorkshop.getId()) {
                std::cout << "  [!] Inspection not found or not assigned to your workshop.\n";
                pause(); continue;
            }
            std::string start = readStr("  Start Date (YYYY-MM-DD): ");
            std::string notes = readStr("  Notes          : ");
            try {
                Repair r = logRepair(db, claimId, myWorkshop.getId(),
                                     inspId, user.getId(), start, notes);
                std::cout << "  Repair job logged. ID=" << r.getId() << "\n";
            } catch (const std::exception& e) {
                std::cout << "  [!] " << e.what() << "\n";
            }
            pause();
        } else if (ch == 3) {
            auto repairs = db.repairsByWorkshop(myWorkshop.getId());
            std::cout << "\n  IN_PROGRESS repairs at your workshop:\n";
            bool any = false;
            for (const auto& r : repairs)
                if (r.getStatus() == "IN_PROGRESS") {
                    std::cout << "    [" << r.getId() << "] Claim:" << r.getClaimId()
                              << "  Started:" << r.getStartDate() << "\n";
                    any = true;
                }
            if (!any) { std::cout << "  None.\n"; pause(); continue; }
            int rid = readInt("  Repair ID   : ");
            // Verify ownership
            Repair rep = db.repair.findById(rid);
            if (rep.getId() == 0 || rep.getWorkshopId() != myWorkshop.getId()) {
                std::cout << "  [!] Not found or not your workshop's repair.\n";
                pause(); continue;
            }
            std::string endDate = readStr("  End Date (YYYY-MM-DD): ");
            double finalCost    = readDouble("  Final Cost (PKR)      : ");
            if (completeRepair(db, rid, endDate, finalCost))
                std::cout << "  Repair marked COMPLETE.\n";
            else
                std::cout << "  Update failed.\n";
            pause();
        } else if (ch == 4) {
            // Show inspections assigned to this workshop
            std::cout << "\n  Inspections assigned to " << myWorkshop.getName() << ":\n";
            auto insps = db.insp.loadAll();
            bool any = false;
            for (const auto& i : insps)
                if (i.getWorkshopId() == myWorkshop.getId()) {
                    Staff sv = db.staff.findById(i.getSurveyorId());
                    std::cout << "  [" << i.getId() << "]"
                              << "  Claim:" << i.getClaimId()
                              << "  Date:" << i.getInspDate()
                              << "  Surveyor:" << sv.getName()
                              << "  Status:" << i.getStatus()
                              << "  Cost: PKR " << i.getRepairCost() << "\n";
                    any = true;
                }
            if (!any) std::cout << "  None.\n";
            pause();
        } else if (ch == 5) {
            int cid = readInt("  Claim ID: ");
            Claim c = db.claim.findById(cid);
            if (c.getId() == 0) { std::cout << "  Not found.\n"; pause(); continue; }
            printLine();
            std::cout << "  Claim No    : " << c.getClaimNo()      << "\n"
                      << "  Incident    : " << c.getIncidentDate() << "\n"
                      << "  Description : " << c.getDescription()  << "\n"
                      << "  Est. Loss   : PKR " << c.getEstimatedLoss() << "\n"
                      << "  Status      : " << c.getStatus()       << "\n";
            pause();
        }

        // Reload workshop info in case admin changed it
        myWorkshop = db.workshopByManager(user.getId());
    } while (ch != 0);
}

// ════════════════════════════════════════════════════════════
//  SECTION 9 – LOGIN SCREEN
// ════════════════════════════════════════════════════════════

static Staff loginScreen(DB& db) {
    for (int attempt = 0; attempt < 3; ++attempt) {
        std::string user = readStr("  Username (or 'quit'): ");
        if (user == "quit") return Staff();
        std::string pass = readStr("  Password            : ");
        Staff s = login(db, user, pass);
        if (s.getId() != 0) {
            std::cout << "\n  Welcome, " << s.getName()
                      << "  [" << s.getRole() << "]\n\n";
            return s;
        }
        std::cout << "  [!] Invalid credentials.\n\n";
    }
    std::cout << "  Too many failed attempts.\n";
    return Staff();
}

// ════════════════════════════════════════════════════════════
//  SECTION 10 – MAIN ENTRY POINT
// ════════════════════════════════════════════════════════════

int main() {
    // Create data directory if missing
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0755);
#endif

    DB db;

    // Seed default admin if no staff exists yet
    if (db.staff.loadAll().empty()) {
        Staff admin(1, "System Admin", "ADMIN", "admin", "admin123",
                    "0300-0000000", "admin@insurance.com", 0);
        db.staff.add(admin);
        printLine('*');
        std::cout << "  First-run setup: default admin account created.\n"
                  << "  Username: admin   Password: admin123\n";
        printLine('*');
        std::cout << "\n";
    }

    printHeader("AutoShield Insurance Management System");
    std::cout << "  File-backed  |  No DBMS  |  C++11\n\n";

    while (true) {
        printHeader("LOGIN");
        Staff user = loginScreen(db);

        if (user.getId() == 0) {
            std::cout << "Exiting. Goodbye!\n";
            break;
        }

        std::string role = user.getRole();
        if      (role == "ADMIN")        adminMenu(db, user);
        else if (role == "SALESMAN")     salesmanMenu(db, user);
        else if (role == "SURVEYOR")     surveyorMenu(db, user);
        else if (role == "WORKSHOP_MGR") workshopMgrMenu(db, user);
        else {
            std::cout << "  [!] Unknown role '" << role << "'. Contact Admin.\n";
        }
    }

    return 0;
}
