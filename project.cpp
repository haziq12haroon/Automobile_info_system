#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <stdexcept>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <ctime>
#include <map>
#include <unordered_map>
#include <sys/stat.h>


namespace Utils {

    std::string todayStr() {
        time_t now = time(nullptr);
        const struct tm* t = localtime(&now);
        char buf[12];
        strftime(buf, sizeof(buf), "%Y-%m-%d", t);
        return std::string(buf);
    }

    std::string tok(std::istringstream& ss) {
        std::string t;
        std::getline(ss, t, '|');
        return t;
    }

    int safeInt(const std::string& s) {
        try { return std::stoi(s); } catch (...) { return 0; }
    }

    double safeDbl(const std::string& s) {
        try { return std::stod(s); } catch (...) { return 0.0; }
    }

    void printLine(char c = '-', int len = 70) {
        std::cout << std::string(len, c) << "\n";
    }

    void printHeader(const std::string& title) {
        printLine('=');
        int pad = (70 - (int)title.size()) / 2;
        if (pad > 0) std::cout << std::string(pad, ' ');
        std::cout << title << "\n";
        printLine('=');
    }

    int readInt(const std::string& prompt) {
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

    double readDouble(const std::string& prompt) {
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

    std::string readStr(const std::string& prompt) {
        std::cout << prompt;
        std::string val;
        std::getline(std::cin, val);
        return val;
    }

    void pause() {
        std::cout << "\n  Press ENTER to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

} 

class Logger {
public:
    static Logger& instance() {
        static Logger log;
        return log;
    }
    void info(const std::string& msg)  { log("[INFO]  ", msg); }
    void warn(const std::string& msg)  { log("[WARN]  ", msg); }
    void error(const std::string& msg) { log("[ERROR] ", msg); }

private:
    Logger() : m_out("data/system.log", std::ios::app) {}
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void log(const std::string& level, const std::string& msg) {
        if (m_out.is_open())
            m_out << "[" << Utils::todayStr() << "] " << level << msg << "\n";
    }
    std::ofstream m_out;
};


class AppConfig {
public:
    static AppConfig& instance() {
        static AppConfig cfg;
        return cfg;
    }
    std::string dataDir() const { return "data/"; }
    int maxLoginAttempts()  const { return 3; }

private:
    AppConfig() = default;
    AppConfig(const AppConfig&) = delete;
};

class IEntity {
public:
    virtual ~IEntity() = default;
    virtual int         getId()       const = 0;
    virtual std::string serialize()   const = 0;
    virtual std::shared_ptr<IEntity> clone() const = 0;
};


// ── Staff ───────────────────────────────────────────────────────────────────
class Staff : public IEntity {
public:
    Staff() : m_id(0), m_workshopId(0) {}
    Staff(int id, const std::string& name, const std::string& role,
          const std::string& username, const std::string& password,
          const std::string& phone, const std::string& email,
          int workshopId = 0)
        : m_id(id), m_name(name), m_role(role),
          m_username(username), m_password(password),
          m_phone(phone), m_email(email), m_workshopId(workshopId) {}

    int         getId()         const override { return m_id; }
    std::string getName()       const { return m_name; }
    std::string getRole()       const { return m_role; }
    std::string getUsername()   const { return m_username; }
    std::string getPassword()   const { return m_password; }
    std::string getPhone()      const { return m_phone; }
    std::string getEmail()      const { return m_email; }
    int         getWorkshopId() const { return m_workshopId; }

    void setPassword(const std::string& pw) { m_password   = pw; }
    void setWorkshopId(int wid)             { m_workshopId = wid; }
    void setPhone(const std::string& ph)    { m_phone = ph; }
    void setEmail(const std::string& em)    { m_email = em; }

    std::string serialize() const override {
        std::ostringstream ss;
        ss << m_id << '|' << m_name << '|' << m_role << '|'
           << m_username << '|' << m_password << '|'
           << m_phone << '|' << m_email << '|' << m_workshopId;
        return ss.str();
    }
    
    std::shared_ptr<IEntity> clone() const override {
        return std::make_shared<Staff>(*this);
    }

    static Staff deserialize(const std::string& line) {
        std::istringstream ss(line);
        int id  = Utils::safeInt(Utils::tok(ss));
        auto n  = Utils::tok(ss), r = Utils::tok(ss),
             u  = Utils::tok(ss), p = Utils::tok(ss),
             ph = Utils::tok(ss), e = Utils::tok(ss);
        int wid = Utils::safeInt(Utils::tok(ss));
        return Staff(id, n, r, u, p, ph, e, wid);
    }

private:
    int         m_id;
    std::string m_name, m_role, m_username, m_password, m_phone, m_email;
    int         m_workshopId;
};

// ── Customer ─────────────────────────────────────────────────────────────────
class Customer : public IEntity {
public:
    Customer() : m_id(0) {}
    Customer(int id, const std::string& name, const std::string& cnic,
             const std::string& phone, const std::string& email,
             const std::string& address, const std::string& regDate)
        : m_id(id), m_name(name), m_cnic(cnic), m_phone(phone),
          m_email(email), m_address(address), m_regDate(regDate) {}

    int         getId()      const override { return m_id; }
    std::string getName()    const { return m_name; }
    std::string getCnic()    const { return m_cnic; }
    std::string getPhone()   const { return m_phone; }
    std::string getEmail()   const { return m_email; }
    std::string getAddress() const { return m_address; }
    std::string getRegDate() const { return m_regDate; }

    void setPhone(const std::string& p)   { m_phone   = p; }
    void setEmail(const std::string& e)   { m_email   = e; }
    void setAddress(const std::string& a) { m_address = a; }

    std::string serialize() const override {
        std::ostringstream ss;
        ss << m_id << '|' << m_name << '|' << m_cnic << '|'
           << m_phone << '|' << m_email << '|' << m_address << '|' << m_regDate;
        return ss.str();
    }
    std::shared_ptr<IEntity> clone() const override {
        return std::make_shared<Customer>(*this);
    }
    static Customer deserialize(const std::string& line) {
        std::istringstream ss(line);
        int id = Utils::safeInt(Utils::tok(ss));
        auto n = Utils::tok(ss), c = Utils::tok(ss),
             p = Utils::tok(ss), e = Utils::tok(ss),
             a = Utils::tok(ss), d = Utils::tok(ss);
        return Customer(id, n, c, p, e, a, d);
    }

private:
    int m_id;
    std::string m_name, m_cnic, m_phone, m_email, m_address, m_regDate;
};

// ── Vehicle ──────────────────────────────────────────────────────────────────
class Vehicle : public IEntity {
public:
    Vehicle() : m_id(0), m_customerId(0), m_year(0) {}
    Vehicle(int id, int customerId, const std::string& regNo,
            const std::string& make, const std::string& model,
            int year, const std::string& color,
            const std::string& chassisNo, const std::string& engineNo)
        : m_id(id), m_customerId(customerId), m_year(year),
          m_regNo(regNo), m_make(make), m_model(model),
          m_color(color), m_chassisNo(chassisNo), m_engineNo(engineNo) {}

    int         getId()         const override { return m_id; }
    int         getCustomerId() const { return m_customerId; }
    std::string getRegNo()      const { return m_regNo; }
    std::string getMake()       const { return m_make; }
    std::string getModel()      const { return m_model; }
    int         getYear()       const { return m_year; }
    std::string getColor()      const { return m_color; }
    std::string getChassisNo()  const { return m_chassisNo; }
    std::string getEngineNo()   const { return m_engineNo; }

    std::string serialize() const override {
        std::ostringstream ss;
        ss << m_id << '|' << m_customerId << '|' << m_regNo << '|'
           << m_make << '|' << m_model << '|' << m_year << '|'
           << m_color << '|' << m_chassisNo << '|' << m_engineNo;
        return ss.str();
    }
    std::shared_ptr<IEntity> clone() const override {
        return std::make_shared<Vehicle>(*this);
    }
    static Vehicle deserialize(const std::string& line) {
        std::istringstream ss(line);
        int id    = Utils::safeInt(Utils::tok(ss));
        int cid   = Utils::safeInt(Utils::tok(ss));
        auto rn   = Utils::tok(ss), mk = Utils::tok(ss), mo = Utils::tok(ss);
        int yr    = Utils::safeInt(Utils::tok(ss));
        auto col  = Utils::tok(ss), ch = Utils::tok(ss), en = Utils::tok(ss);
        return Vehicle(id, cid, rn, mk, mo, yr, col, ch, en);
    }

private:
    int m_id, m_customerId, m_year;
    std::string m_regNo, m_make, m_model, m_color, m_chassisNo, m_engineNo;
};

// ── Workshop ─────────────────────────────────────────────────────────────────
class Workshop : public IEntity {
public:
    Workshop() : m_id(0), m_managerId(0) {}
    Workshop(int id, const std::string& name, const std::string& address,
             const std::string& phone, const std::string& ownerName,
             int managerId = 0)
        : m_id(id), m_name(name), m_address(address),
          m_phone(phone), m_ownerName(ownerName), m_managerId(managerId) {}

    int         getId()        const override { return m_id; }
    std::string getName()      const { return m_name; }
    std::string getAddress()   const { return m_address; }
    std::string getPhone()     const { return m_phone; }
    std::string getOwnerName() const { return m_ownerName; }
    int         getManagerId() const { return m_managerId; }

    void setManagerId(int mid)             { m_managerId = mid; }
    void setPhone(const std::string& ph)   { m_phone   = ph; }
    void setAddress(const std::string& ad) { m_address = ad; }

    std::string serialize() const override {
        std::ostringstream ss;
        ss << m_id << '|' << m_name << '|' << m_address << '|'
           << m_phone << '|' << m_ownerName << '|' << m_managerId;
        return ss.str();
    }
    std::shared_ptr<IEntity> clone() const override {
        return std::make_shared<Workshop>(*this);
    }
    static Workshop deserialize(const std::string& line) {
        std::istringstream ss(line);
        int id  = Utils::safeInt(Utils::tok(ss));
        auto n  = Utils::tok(ss), a = Utils::tok(ss),
             ph = Utils::tok(ss), o = Utils::tok(ss);
        int mid = Utils::safeInt(Utils::tok(ss));
        return Workshop(id, n, a, ph, o, mid);
    }

private:
    int m_id;
    std::string m_name, m_address, m_phone, m_ownerName;
    int m_managerId;
};

// ── InsurancePolicy ──────────────────────────────────────────────────────────
class InsurancePolicy : public IEntity {
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

    int         getId()             const override { return m_id; }
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
    void        setStatus(const std::string& s) { m_status = s; }

    std::string serialize() const override {
        std::ostringstream ss;
        ss << m_id << '|' << m_customerId << '|' << m_vehicleId << '|'
           << m_salesmanId << '|' << m_policyNo << '|' << m_type << '|'
           << m_premiumAmount << '|' << m_coverageAmount << '|'
           << m_startDate << '|' << m_endDate << '|' << m_status;
        return ss.str();
    }
    std::shared_ptr<IEntity> clone() const override {
        return std::make_shared<InsurancePolicy>(*this);
    }
    static InsurancePolicy deserialize(const std::string& line) {
        std::istringstream ss(line);
        int id  = Utils::safeInt(Utils::tok(ss));
        int cid = Utils::safeInt(Utils::tok(ss));
        int vid = Utils::safeInt(Utils::tok(ss));
        int sid = Utils::safeInt(Utils::tok(ss));
        auto pno = Utils::tok(ss), type = Utils::tok(ss);
        double prem = Utils::safeDbl(Utils::tok(ss));
        double cov  = Utils::safeDbl(Utils::tok(ss));
        auto start = Utils::tok(ss), end = Utils::tok(ss), status = Utils::tok(ss);
        return InsurancePolicy(id, cid, vid, sid, pno, type, prem, cov, start, end, status);
    }

private:
    int         m_id, m_customerId, m_vehicleId, m_salesmanId;
    std::string m_policyNo, m_type;
    double      m_premiumAmount, m_coverageAmount;
    std::string m_startDate, m_endDate, m_status;
};

// ── Claim ────────────────────────────────────────────────────────────────────
class Claim : public IEntity {
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

    int         getId()            const override { return m_id; }
    int         getPolicyId()      const { return m_policyId; }
    int         getCustomerId()    const { return m_customerId; }
    int         getVehicleId()     const { return m_vehicleId; }
    std::string getClaimNo()       const { return m_claimNo; }
    std::string getIncidentDate()  const { return m_incidentDate; }
    std::string getFiledDate()     const { return m_filedDate; }
    std::string getDescription()   const { return m_description; }
    double      getEstimatedLoss() const { return m_estimatedLoss; }
    std::string getStatus()        const { return m_status; }
    void        setStatus(const std::string& s) { m_status = s; }

    std::string serialize() const override {
        std::ostringstream ss;
        ss << m_id << '|' << m_policyId << '|' << m_customerId << '|'
           << m_vehicleId << '|' << m_claimNo << '|' << m_incidentDate << '|'
           << m_filedDate << '|' << m_description << '|'
           << m_estimatedLoss << '|' << m_status;
        return ss.str();
    }
    std::shared_ptr<IEntity> clone() const override {
        return std::make_shared<Claim>(*this);
    }
    static Claim deserialize(const std::string& line) {
        std::istringstream ss(line);
        int id   = Utils::safeInt(Utils::tok(ss));
        int pol  = Utils::safeInt(Utils::tok(ss));
        int cid  = Utils::safeInt(Utils::tok(ss));
        int vid  = Utils::safeInt(Utils::tok(ss));
        auto cno = Utils::tok(ss), inc = Utils::tok(ss),
             fil = Utils::tok(ss), dsc = Utils::tok(ss);
        double loss   = Utils::safeDbl(Utils::tok(ss));
        auto status   = Utils::tok(ss);
        return Claim(id, pol, cid, vid, cno, inc, fil, dsc, loss, status);
    }

private:
    int         m_id, m_policyId, m_customerId, m_vehicleId;
    std::string m_claimNo, m_incidentDate, m_filedDate, m_description;
    double      m_estimatedLoss;
    std::string m_status;
};

// ── Inspection ───────────────────────────────────────────────────────────────
class Inspection : public IEntity {
public:
    Inspection() : m_id(0), m_claimId(0), m_surveyorId(0), m_workshopId(0), m_repairCost(0) {}
    Inspection(int id, int claimId, int surveyorId, int workshopId,
               const std::string& inspDate, const std::string& findings,
               double repairCost, const std::string& recommendation,
               const std::string& status)
        : m_id(id), m_claimId(claimId), m_surveyorId(surveyorId),
          m_workshopId(workshopId), m_inspDate(inspDate), m_findings(findings),
          m_repairCost(repairCost), m_recommendation(recommendation), m_status(status) {}

    int         getId()             const override { return m_id; }
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

    std::string serialize() const override {
        std::ostringstream ss;
        ss << m_id << '|' << m_claimId << '|' << m_surveyorId << '|'
           << m_workshopId << '|' << m_inspDate << '|' << m_findings << '|'
           << m_repairCost << '|' << m_recommendation << '|' << m_status;
        return ss.str();
    }
    std::shared_ptr<IEntity> clone() const override {
        return std::make_shared<Inspection>(*this);
    }
    static Inspection deserialize(const std::string& line) {
        std::istringstream ss(line);
        int id  = Utils::safeInt(Utils::tok(ss));
        int cid = Utils::safeInt(Utils::tok(ss));
        int sid = Utils::safeInt(Utils::tok(ss));
        int wid = Utils::safeInt(Utils::tok(ss));
        auto dt   = Utils::tok(ss), fi = Utils::tok(ss);
        double co = Utils::safeDbl(Utils::tok(ss));
        auto rc   = Utils::tok(ss), st = Utils::tok(ss);
        return Inspection(id, cid, sid, wid, dt, fi, co, rc, st);
    }

private:
    int         m_id, m_claimId, m_surveyorId, m_workshopId;
    std::string m_inspDate, m_findings;
    double      m_repairCost;
    std::string m_recommendation, m_status;
};

// ── Repair ───────────────────────────────────────────────────────────────────
class Repair : public IEntity {
public:
    Repair() : m_id(0), m_claimId(0), m_workshopId(0),
               m_inspectionId(0), m_workshopMgrId(0), m_repairCost(0) {}
    Repair(int id, int claimId, int workshopId, int inspectionId,
           int workshopMgrId,
           const std::string& startDate, const std::string& endDate,
           double repairCost, const std::string& status, const std::string& notes)
        : m_id(id), m_claimId(claimId), m_workshopId(workshopId),
          m_inspectionId(inspectionId), m_workshopMgrId(workshopMgrId),
          m_startDate(startDate), m_endDate(endDate),
          m_repairCost(repairCost), m_status(status), m_notes(notes) {}

    int         getId()            const override { return m_id; }
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

    std::string serialize() const override {
        std::ostringstream ss;
        ss << m_id << '|' << m_claimId << '|' << m_workshopId << '|'
           << m_inspectionId << '|' << m_workshopMgrId << '|'
           << m_startDate << '|' << m_endDate << '|'
           << m_repairCost << '|' << m_status << '|' << m_notes;
        return ss.str();
    }
    std::shared_ptr<IEntity> clone() const override {
        return std::make_shared<Repair>(*this);
    }
    static Repair deserialize(const std::string& line) {
        std::istringstream ss(line);
        int id  = Utils::safeInt(Utils::tok(ss));
        int ci  = Utils::safeInt(Utils::tok(ss));
        int wi  = Utils::safeInt(Utils::tok(ss));
        int ii  = Utils::safeInt(Utils::tok(ss));
        int mi  = Utils::safeInt(Utils::tok(ss));
        auto st = Utils::tok(ss), en = Utils::tok(ss);
        double co = Utils::safeDbl(Utils::tok(ss));
        auto sta  = Utils::tok(ss), no = Utils::tok(ss);
        return Repair(id, ci, wi, ii, mi, st, en, co, sta, no);
    }

private:
    int         m_id, m_claimId, m_workshopId, m_inspectionId, m_workshopMgrId;
    std::string m_startDate, m_endDate;
    double      m_repairCost;
    std::string m_status, m_notes;
};

template<typename T>
class IRepository {
public:
    virtual ~IRepository() = default;
    virtual std::vector<T> loadAll()             const = 0;
    virtual void           saveAll(const std::vector<T>&) = 0;
    virtual bool           add(const T& item)          = 0;
    virtual bool           update(const T& item)       = 0;
    virtual bool           remove(int id)              = 0;
    virtual T              findById(int id)      const = 0;
    virtual int            nextId()              const = 0;
};

template<typename T>
class FileRepo : public IRepository<T> {
public:
    explicit FileRepo(const std::string& path) : m_path(path) {}

    std::vector<T> loadAll() const override {
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

    void saveAll(const std::vector<T>& recs) override {
        std::ofstream f(m_path, std::ios::trunc);
        for (const auto& r : recs) f << r.serialize() << '\n';
    }

    bool add(const T& item) override {
        auto recs = loadAll();
        recs.push_back(item);
        saveAll(recs);
        return true;
    }

    bool update(const T& item) override {
        auto recs = loadAll();
        for (auto& r : recs)
            if (r.getId() == item.getId()) { r = item; saveAll(recs); return true; }
        return false;
    }

    bool remove(int id) override {
        auto recs = loadAll();
        auto it = std::remove_if(recs.begin(), recs.end(),
                                 [id](const T& r){ return r.getId() == id; });
        if (it == recs.end()) return false;
        recs.erase(it, recs.end());
        saveAll(recs);
        return true;
    }

    T findById(int id) const override {
        for (const auto& r : loadAll())
            if (r.getId() == id) return r;
        return T();
    }

    int nextId() const override {
        int mx = 0;
        for (const auto& r : loadAll()) if (r.getId() > mx) mx = r.getId();
        return mx + 1;
    }

private:
    std::string m_path;
};


template<typename T>
class LoggingRepository : public IRepository<T> {
public:
    explicit LoggingRepository(std::shared_ptr<IRepository<T>> inner,
                                const std::string& entityName)
        : m_inner(inner), m_entityName(entityName) {}

    std::vector<T> loadAll()  const override { return m_inner->loadAll(); }
    T findById(int id)        const override { return m_inner->findById(id); }
    int nextId()              const override { return m_inner->nextId(); }
    void saveAll(const std::vector<T>& v)   override { m_inner->saveAll(v); }

    bool add(const T& item) override {
        bool ok = m_inner->add(item);
        Logger::instance().info("ADD " + m_entityName + " id=" + std::to_string(item.getId()));
        return ok;
    }
    bool update(const T& item) override {
        bool ok = m_inner->update(item);
        Logger::instance().info("UPDATE " + m_entityName + " id=" + std::to_string(item.getId()));
        return ok;
    }
    bool remove(int id) override {
        bool ok = m_inner->remove(id);
        Logger::instance().info("REMOVE " + m_entityName + " id=" + std::to_string(id));
        return ok;
    }

private:
    std::shared_ptr<IRepository<T>> m_inner;
    std::string m_entityName;
};

class IRepositoryFactory {
public:
    virtual ~IRepositoryFactory() = default;
    virtual std::shared_ptr<IRepository<Staff>>          makeStaff()    = 0;
    virtual std::shared_ptr<IRepository<Customer>>       makeCustomer() = 0;
    virtual std::shared_ptr<IRepository<Vehicle>>        makeVehicle()  = 0;
    virtual std::shared_ptr<IRepository<Workshop>>       makeWorkshop() = 0;
    virtual std::shared_ptr<IRepository<InsurancePolicy>>makePolicy()   = 0;
    virtual std::shared_ptr<IRepository<Claim>>          makeClaim()    = 0;
    virtual std::shared_ptr<IRepository<Inspection>>     makeInspection()= 0;
    virtual std::shared_ptr<IRepository<Repair>>         makeRepair()   = 0;
};

class FileRepositoryFactory : public IRepositoryFactory {
    std::string dir_;
    template<typename T>
    std::shared_ptr<IRepository<T>> make(const std::string& file, const std::string& name) {
        return std::make_shared<LoggingRepository<T>>(
            std::make_shared<FileRepo<T>>(dir_ + file), name);
    }
public:
    explicit FileRepositoryFactory(const std::string& dir) : dir_(dir) {}

    std::shared_ptr<IRepository<Staff>>          makeStaff()     override { return make<Staff>         ("staff.dat",       "Staff"); }
    std::shared_ptr<IRepository<Customer>>       makeCustomer()  override { return make<Customer>      ("customers.dat",   "Customer"); }
    std::shared_ptr<IRepository<Vehicle>>        makeVehicle()   override { return make<Vehicle>       ("vehicles.dat",    "Vehicle"); }
    std::shared_ptr<IRepository<Workshop>>       makeWorkshop()  override { return make<Workshop>      ("workshops.dat",   "Workshop"); }
    std::shared_ptr<IRepository<InsurancePolicy>>makePolicy()    override { return make<InsurancePolicy>("policies.dat",   "Policy"); }
    std::shared_ptr<IRepository<Claim>>          makeClaim()     override { return make<Claim>         ("claims.dat",      "Claim"); }
    std::shared_ptr<IRepository<Inspection>>     makeInspection()override { return make<Inspection>    ("inspections.dat", "Inspection"); }
    std::shared_ptr<IRepository<Repair>>         makeRepair()    override { return make<Repair>        ("repairs.dat",     "Repair"); }
};


struct DomainEvent {
    std::string type;      
    std::string details;
    DomainEvent(const std::string& t, const std::string& d) : type(t), details(d) {}
};

class IEventListener {
public:
    virtual ~IEventListener() = default;
    virtual void onEvent(const DomainEvent& e) = 0;
};

class EventBus {
public:
    static EventBus& instance() { static EventBus bus; return bus; }
    void subscribe(const std::string& type, IEventListener* listener) {
        m_listeners[type].push_back(listener);
    }
    void publish(const DomainEvent& event) {
        auto it = m_listeners.find(event.type);
        if (it != m_listeners.end())
            for (auto* l : it->second) l->onEvent(event);
        // also log every event
        Logger::instance().info("EVENT " + event.type + ": " + event.details);
    }
private:
    EventBus() = default;
    std::unordered_map<std::string, std::vector<IEventListener*>> m_listeners;
};

// Concrete listener – audit trail
class AuditListener : public IEventListener {
public:
    void onEvent(const DomainEvent& e) override {
        
    }
};

class ClaimState {
public:
    virtual ~ClaimState() = default;
    virtual std::string name() const = 0;
    virtual bool canTransitionTo(const std::string& next) const = 0;
};

class PendingState : public ClaimState {
public:
    std::string name() const override { return "PENDING"; }
    bool canTransitionTo(const std::string& n) const override {
        return n == "UNDER_REVIEW";
    }
};
class UnderReviewState : public ClaimState {
public:
    std::string name() const override { return "UNDER_REVIEW"; }
    bool canTransitionTo(const std::string& n) const override {
        return n == "APPROVED" || n == "REJECTED";
    }
};
class ApprovedState : public ClaimState {
public:
    std::string name() const override { return "APPROVED"; }
    bool canTransitionTo(const std::string&) const override { return false; }
};
class RejectedState : public ClaimState {
public:
    std::string name() const override { return "REJECTED"; }
    bool canTransitionTo(const std::string&) const override { return false; }
};

class ClaimStateMachine {
public:
    static bool isValidTransition(const std::string& from, const std::string& to) {
        auto state = makeState(from);
        return state && state->canTransitionTo(to);
    }
private:
    static std::unique_ptr<ClaimState> makeState(const std::string& s) {
        if (s == "PENDING")      return std::make_unique<PendingState>();
        if (s == "UNDER_REVIEW") return std::make_unique<UnderReviewState>();
        if (s == "APPROVED")     return std::make_unique<ApprovedState>();
        if (s == "REJECTED")     return std::make_unique<RejectedState>();
        return nullptr;
    }
};


class IValidator {
public:
    virtual ~IValidator() = default;
    virtual bool validate(const std::string& input, std::string& errMsg) const = 0;
    void setNext(std::shared_ptr<IValidator> next) { m_next = next; }
protected:
    bool passToNext(const std::string& input, std::string& errMsg) const {
        if (m_next) return m_next->validate(input, errMsg);
        return true;
    }
    std::shared_ptr<IValidator> m_next;
};

class NotEmptyValidator : public IValidator {
public:
    bool validate(const std::string& input, std::string& errMsg) const override {
        if (input.empty()) { errMsg = "Field cannot be empty."; return false; }
        return passToNext(input, errMsg);
    }
};

class MinLengthValidator : public IValidator {
    int m_min;
public:
    explicit MinLengthValidator(int min) : m_min(min) {}
    bool validate(const std::string& input, std::string& errMsg) const override {
        if ((int)input.size() < m_min) {
            errMsg = "Must be at least " + std::to_string(m_min) + " characters.";
            return false;
        }
        return passToNext(input, errMsg);
    }
};


class PolicyBuilder {
    int m_customerId = 0, m_vehicleId = 0, m_salesmanId = 0;
    std::string m_type, m_startDate, m_endDate;
    double m_premium = 0, m_coverage = 0;
public:
    PolicyBuilder& customer(int id)              { m_customerId = id;  return *this; }
    PolicyBuilder& vehicle(int id)               { m_vehicleId  = id;  return *this; }
    PolicyBuilder& salesman(int id)              { m_salesmanId = id;  return *this; }
    PolicyBuilder& type(const std::string& t)    { m_type       = t;   return *this; }
    PolicyBuilder& premium(double p)             { m_premium    = p;   return *this; }
    PolicyBuilder& coverage(double c)            { m_coverage   = c;   return *this; }
    PolicyBuilder& startDate(const std::string& d){ m_startDate = d;   return *this; }
    PolicyBuilder& endDate(const std::string& d) { m_endDate    = d;   return *this; }

    InsurancePolicy build(int id, const std::string& policyNo) const {
        return InsurancePolicy(id, m_customerId, m_vehicleId, m_salesmanId,
                               policyNo, m_type, m_premium, m_coverage,
                               m_startDate, m_endDate, "ACTIVE");
    }
};

class ClaimBuilder {
    int m_policyId = 0, m_customerId = 0, m_vehicleId = 0;
    std::string m_incidentDate, m_description;
    double m_estimatedLoss = 0;
public:
    ClaimBuilder& policy(int id)                    { m_policyId     = id; return *this; }
    ClaimBuilder& customer(int id)                  { m_customerId   = id; return *this; }
    ClaimBuilder& vehicle(int id)                   { m_vehicleId    = id; return *this; }
    ClaimBuilder& incidentDate(const std::string& d){ m_incidentDate = d;  return *this; }
    ClaimBuilder& description(const std::string& d) { m_description  = d;  return *this; }
    ClaimBuilder& estimatedLoss(double l)           { m_estimatedLoss = l; return *this; }

    Claim build(int id, const std::string& claimNo) const {
        return Claim(id, m_policyId, m_customerId, m_vehicleId, claimNo,
                     m_incidentDate, Utils::todayStr(), m_description,
                     m_estimatedLoss, "PENDING");
    }
};


class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
    virtual void undo()    = 0; 
    virtual std::string description() const = 0;
};

class CommandHistory {
public:
    void push(std::shared_ptr<ICommand> cmd) {
        m_history.push_back(cmd);
        if (m_history.size() > 50) m_history.erase(m_history.begin());
    }
    bool undoLast() {
        if (m_history.empty()) return false;
        m_history.back()->undo();
        m_history.pop_back();
        return true;
    }
private:
    std::vector<std::shared_ptr<ICommand>> m_history;
};


template<typename T>
class ISortStrategy {
public:
    virtual ~ISortStrategy() = default;
    virtual void sort(std::vector<T>& items) const = 0;
};

class ClaimSortByDate : public ISortStrategy<Claim> {
public:
    void sort(std::vector<Claim>& items) const override {
        std::sort(items.begin(), items.end(), [](const Claim& a, const Claim& b){
            return a.getFiledDate() < b.getFiledDate();
        });
    }
};

class ClaimSortByLoss : public ISortStrategy<Claim> {
public:
    void sort(std::vector<Claim>& items) const override {
        std::sort(items.begin(), items.end(), [](const Claim& a, const Claim& b){
            return a.getEstimatedLoss() > b.getEstimatedLoss();
        });
    }
};


//   DATABASE 

struct DB {
    std::shared_ptr<IRepository<Staff>>          staff;
    std::shared_ptr<IRepository<Customer>>       customer;
    std::shared_ptr<IRepository<Vehicle>>        vehicle;
    std::shared_ptr<IRepository<Workshop>>       workshop;
    std::shared_ptr<IRepository<InsurancePolicy>>policy;
    std::shared_ptr<IRepository<Claim>>          claim;
    std::shared_ptr<IRepository<Inspection>>     insp;
    std::shared_ptr<IRepository<Repair>>         repair;

    explicit DB(IRepositoryFactory& factory) {
        staff    = factory.makeStaff();
        customer = factory.makeCustomer();
        vehicle  = factory.makeVehicle();
        workshop = factory.makeWorkshop();
        policy   = factory.makePolicy();
        claim    = factory.makeClaim();
        insp     = factory.makeInspection();
        repair   = factory.makeRepair();
    }

    //  query methods 
    Staff findStaffByUsername(const std::string& u) const {
        for (const auto& s : staff->loadAll())
            if (s.getUsername() == u) return s;
        return Staff();
    }
    std::vector<Staff> staffByRole(const std::string& role) const {
        std::vector<Staff> out;
        for (const auto& s : staff->loadAll())
            if (s.getRole() == role) out.push_back(s);
        return out;
    }
    Customer findCustomerByCnic(const std::string& cnic) const {
        for (const auto& c : customer->loadAll())
            if (c.getCnic() == cnic) return c;
        return Customer();
    }
    std::vector<Customer> customersByMonth(const std::string& ym) const {
        std::vector<Customer> out;
        for (const auto& c : customer->loadAll())
            if (c.getRegDate().substr(0,7) == ym) out.push_back(c);
        return out;
    }
    Vehicle findVehicleByRegNo(const std::string& rn) const {
        for (const auto& v : vehicle->loadAll())
            if (v.getRegNo() == rn) return v;
        return Vehicle();
    }
    std::vector<Vehicle> vehiclesByCustomer(int cid) const {
        std::vector<Vehicle> out;
        for (const auto& v : vehicle->loadAll())
            if (v.getCustomerId() == cid) out.push_back(v);
        return out;
    }
    Workshop workshopByManager(int staffId) const {
        for (const auto& w : workshop->loadAll())
            if (w.getManagerId() == staffId) return w;
        return Workshop();
    }
    std::vector<InsurancePolicy> policiesByCustomer(int cid) const {
        std::vector<InsurancePolicy> out;
        for (const auto& p : policy->loadAll())
            if (p.getCustomerId() == cid) out.push_back(p);
        return out;
    }
    Claim findClaimByNo(const std::string& no) const {
        for (const auto& c : claim->loadAll())
            if (c.getClaimNo() == no) return c;
        return Claim();
    }
    std::vector<Claim> claimsByCustomer(int cid) const {
        std::vector<Claim> out;
        for (const auto& c : claim->loadAll())
            if (c.getCustomerId() == cid) out.push_back(c);
        return out;
    }
    std::vector<Claim> claimsByStatus(const std::string& status) const {
        std::vector<Claim> out;
        for (const auto& c : claim->loadAll())
            if (c.getStatus() == status) out.push_back(c);
        return out;
    }
    std::vector<Inspection> inspectionsByClaim(int claimId) const {
        std::vector<Inspection> out;
        for (const auto& i : insp->loadAll())
            if (i.getClaimId() == claimId) out.push_back(i);
        return out;
    }
    std::vector<Inspection> inspectionsBySurveyor(int surveyorId) const {
        std::vector<Inspection> out;
        for (const auto& i : insp->loadAll())
            if (i.getSurveyorId() == surveyorId) out.push_back(i);
        return out;
    }
    std::vector<Repair> repairsByClaim(int claimId) const {
        std::vector<Repair> out;
        for (const auto& r : repair->loadAll())
            if (r.getClaimId() == claimId) out.push_back(r);
        return out;
    }
    std::vector<Repair> repairsByWorkshop(int workshopId) const {
        std::vector<Repair> out;
        for (const auto& r : repair->loadAll())
            if (r.getWorkshopId() == workshopId) out.push_back(r);
        return out;
    }
    bool hasCompletedInspection(int claimId) const {
        for (const auto& i : inspectionsByClaim(claimId))
            if (i.getStatus() == "COMPLETED") return true;
        return false;
    }
};


class IInsuranceService {
public:
    virtual ~IInsuranceService() = default;

    virtual Staff          login(const std::string& username, const std::string& password) = 0;
    virtual Customer       registerCustomer(const std::string& name, const std::string& cnic,
                               const std::string& phone, const std::string& email,
                               const std::string& address) = 0;
    virtual Vehicle        registerVehicle(int customerId, const std::string& regNo,
                               const std::string& make, const std::string& model, int year,
                               const std::string& color, const std::string& chassis,
                               const std::string& engine) = 0;
    virtual InsurancePolicy issuePolicy(const PolicyBuilder& builder) = 0;
    virtual Claim           fileClaim(const ClaimBuilder& builder) = 0;
    virtual Inspection      scheduleInspection(int claimId, int surveyorId,
                               int workshopId, const std::string& inspDate) = 0;
    virtual bool            submitInspectionReport(int inspId, const std::string& findings,
                               double cost, const std::string& recommendation) = 0;
    virtual Repair          logRepair(int claimId, int workshopId, int inspId,
                               int mgrId, const std::string& startDate, const std::string& notes) = 0;
    virtual bool            completeRepair(int repairId, const std::string& endDate, double cost) = 0;
    virtual bool            approveClaim(int claimId) = 0;
    virtual bool            rejectClaim(int claimId) = 0;
    virtual bool            assignWorkshopManager(int workshopId, int staffId) = 0;
    virtual bool            unassignWorkshopManager(int workshopId) = 0;

    virtual const DB& db() const = 0;
};

// ── Concrete Facade ──────────────────────────────────────────────────────────
class InsuranceService : public IInsuranceService {
public:
    explicit InsuranceService(std::shared_ptr<DB> db) : m_db(db) {}

    const DB& db() const override { return *m_db; }

    Staff login(const std::string& username, const std::string& password) override {
        Staff s = m_db->findStaffByUsername(username);
        if (s.getId() == 0 || s.getPassword() != password) return Staff();
        return s;
    }

    Customer registerCustomer(const std::string& name, const std::string& cnic,
                               const std::string& phone, const std::string& email,
                               const std::string& address) override {
        if (m_db->findCustomerByCnic(cnic).getId() != 0)
            throw std::runtime_error("A customer with this CNIC already exists.");
        int id = m_db->customer->nextId();
        Customer c(id, name, cnic, phone, email, address, Utils::todayStr());
        m_db->customer->add(c);
        EventBus::instance().publish({"CUSTOMER_REGISTERED", "id=" + std::to_string(id)});
        return c;
    }

    Vehicle registerVehicle(int customerId, const std::string& regNo,
                             const std::string& make, const std::string& model, int year,
                             const std::string& color, const std::string& chassis,
                             const std::string& engine) override {
        if (m_db->customer->findById(customerId).getId() == 0)
            throw std::runtime_error("Customer not found.");
        if (m_db->findVehicleByRegNo(regNo).getId() != 0)
            throw std::runtime_error("Vehicle registration already exists.");
        int id = m_db->vehicle->nextId();
        Vehicle v(id, customerId, regNo, make, model, year, color, chassis, engine);
        m_db->vehicle->add(v);
        EventBus::instance().publish({"VEHICLE_REGISTERED", "id=" + std::to_string(id)});
        return v;
    }

    InsurancePolicy issuePolicy(const PolicyBuilder& builder) override {
        int id = m_db->policy->nextId();
        std::string pno = "POL-" + std::to_string(id) + "-" + Utils::todayStr().substr(0,4);
        InsurancePolicy p = builder.build(id, pno);
        if (m_db->customer->findById(p.getCustomerId()).getId() == 0)
            throw std::runtime_error("Customer not found.");
        if (m_db->vehicle->findById(p.getVehicleId()).getId() == 0)
            throw std::runtime_error("Vehicle not found.");
        m_db->policy->add(p);
        EventBus::instance().publish({"POLICY_ISSUED", "id=" + std::to_string(id)});
        return p;
    }

    Claim fileClaim(const ClaimBuilder& builder) override {
        InsurancePolicy pol = m_db->policy->findById(builder.build(0,"").getPolicyId());
        
        int id = m_db->claim->nextId();
        std::string cno = "CLM-" + std::to_string(id) + "-" + Utils::todayStr().substr(0,4);
        Claim c = builder.build(id, cno);
        InsurancePolicy p = m_db->policy->findById(c.getPolicyId());
        if (p.getId() == 0)          throw std::runtime_error("Policy not found.");
        if (p.getStatus() != "ACTIVE") throw std::runtime_error("Policy is not active.");
        m_db->claim->add(c);
        EventBus::instance().publish({"CLAIM_FILED", "id=" + std::to_string(id)});
        return c;
    }

    Inspection scheduleInspection(int claimId, int surveyorId,
                                   int workshopId, const std::string& inspDate) override {
        if (m_db->claim->findById(claimId).getId() == 0)
            throw std::runtime_error("Claim not found.");
        if (m_db->workshop->findById(workshopId).getId() == 0)
            throw std::runtime_error("Workshop not found.");
        int id = m_db->insp->nextId();
        Inspection insp(id, claimId, surveyorId, workshopId, inspDate, "", 0.0, "", "SCHEDULED");
        m_db->insp->add(insp);
        
        Claim cl = m_db->claim->findById(claimId);
        if (ClaimStateMachine::isValidTransition(cl.getStatus(), "UNDER_REVIEW")) {
            cl.setStatus("UNDER_REVIEW");
            m_db->claim->update(cl);
        }
        EventBus::instance().publish({"INSPECTION_SCHEDULED", "claimId=" + std::to_string(claimId)});
        return insp;
    }

    bool submitInspectionReport(int inspId, const std::string& findings,
                                 double cost, const std::string& recommendation) override {
        Inspection insp = m_db->insp->findById(inspId);
        if (insp.getId() == 0) return false;
        insp.setFindings(findings);
        insp.setRepairCost(cost);
        insp.setRecommendation(recommendation);
        insp.setStatus("COMPLETED");
        EventBus::instance().publish({"INSPECTION_COMPLETED", "id=" + std::to_string(inspId)});
        return m_db->insp->update(insp);
    }

    Repair logRepair(int claimId, int workshopId, int inspId,
                     int mgrId, const std::string& startDate, const std::string& notes) override {
        Workshop w = m_db->workshop->findById(workshopId);
        if (w.getId() == 0) throw std::runtime_error("Workshop not found.");
        if (w.getManagerId() != mgrId)
            throw std::runtime_error("You are not the assigned manager of this workshop.");
        int id = m_db->repair->nextId();
        Repair r(id, claimId, workshopId, inspId, mgrId, startDate, "", 0.0, "IN_PROGRESS", notes);
        m_db->repair->add(r);
        return r;
    }

    bool completeRepair(int repairId, const std::string& endDate, double finalCost) override {
        Repair r = m_db->repair->findById(repairId);
        if (r.getId() == 0) return false;
        r.setStatus("COMPLETED");
        r.setEndDate(endDate);
        r.setRepairCost(finalCost);
        EventBus::instance().publish({"REPAIR_COMPLETED", "id=" + std::to_string(repairId)});
        return m_db->repair->update(r);
    }

    bool approveClaim(int claimId) override {
        if (!m_db->hasCompletedInspection(claimId))
            throw std::runtime_error("Cannot approve: no completed inspection found.");
        Claim c = m_db->claim->findById(claimId);
        if (c.getId() == 0) return false;
        if (!ClaimStateMachine::isValidTransition(c.getStatus(), "APPROVED"))
            throw std::runtime_error("Invalid state transition to APPROVED.");
        c.setStatus("APPROVED");
        EventBus::instance().publish({"CLAIM_APPROVED", "id=" + std::to_string(claimId)});
        return m_db->claim->update(c);
    }

    bool rejectClaim(int claimId) override {
        Claim c = m_db->claim->findById(claimId);
        if (c.getId() == 0) return false;
        if (!ClaimStateMachine::isValidTransition(c.getStatus(), "REJECTED"))
            throw std::runtime_error("Invalid state transition to REJECTED.");
        c.setStatus("REJECTED");
        EventBus::instance().publish({"CLAIM_REJECTED", "id=" + std::to_string(claimId)});
        return m_db->claim->update(c);
    }

    bool assignWorkshopManager(int workshopId, int staffId) override {
        Workshop w = m_db->workshop->findById(workshopId);
        if (w.getId() == 0) throw std::runtime_error("Workshop not found.");
        Staff s = m_db->staff->findById(staffId);
        if (s.getId() == 0) throw std::runtime_error("Staff not found.");
        if (s.getRole() != "WORKSHOP_MGR")
            throw std::runtime_error("Staff must have role WORKSHOP_MGR.");
        if (w.getManagerId() != 0 && w.getManagerId() != staffId)
            throw std::runtime_error("Workshop already has a different manager.");
        Workshop existing = m_db->workshopByManager(staffId);
        if (existing.getId() != 0 && existing.getId() != workshopId)
            throw std::runtime_error("Staff is already managing: " + existing.getName());
        w.setManagerId(staffId);
        m_db->workshop->update(w);
        s.setWorkshopId(workshopId);
        m_db->staff->update(s);
        return true;
    }

    bool unassignWorkshopManager(int workshopId) override {
        Workshop w = m_db->workshop->findById(workshopId);
        if (w.getId() == 0) return false;
        int oldMgrId = w.getManagerId();
        w.setManagerId(0);
        m_db->workshop->update(w);
        if (oldMgrId != 0) {
            Staff s = m_db->staff->findById(oldMgrId);
            if (s.getId() != 0) { s.setWorkshopId(0); m_db->staff->update(s); }
        }
        return true;
    }

private:
    std::shared_ptr<DB> m_db;
};

class AuthProxy : public IInsuranceService {
public:
    AuthProxy(std::shared_ptr<IInsuranceService> real, const Staff& currentUser)
        : m_real(real), m_user(currentUser) {}

    const DB& db() const override { return m_real->db(); }
    Staff login(const std::string& u, const std::string& p) override { return m_real->login(u, p); }

    Customer registerCustomer(const std::string& name, const std::string& cnic,
                               const std::string& phone, const std::string& email,
                               const std::string& address) override {
        requireRole({"ADMIN","SALESMAN"});
        return m_real->registerCustomer(name, cnic, phone, email, address);
    }
    Vehicle registerVehicle(int cid, const std::string& rn, const std::string& mk,
                             const std::string& mo, int yr, const std::string& col,
                             const std::string& ch, const std::string& en) override {
        requireRole({"ADMIN","SALESMAN"});
        return m_real->registerVehicle(cid, rn, mk, mo, yr, col, ch, en);
    }
    InsurancePolicy issuePolicy(const PolicyBuilder& b) override {
        requireRole({"ADMIN","SALESMAN"}); return m_real->issuePolicy(b);
    }
    Claim fileClaim(const ClaimBuilder& b) override {
        requireRole({"ADMIN","SALESMAN"}); return m_real->fileClaim(b);
    }
    Inspection scheduleInspection(int cid, int sid, int wid, const std::string& d) override {
        requireRole({"ADMIN","SURVEYOR"}); return m_real->scheduleInspection(cid, sid, wid, d);
    }
    bool submitInspectionReport(int id, const std::string& f, double c, const std::string& r) override {
        requireRole({"SURVEYOR"}); return m_real->submitInspectionReport(id, f, c, r);
    }
    Repair logRepair(int ci, int wi, int ii, int mi, const std::string& sd, const std::string& n) override {
        requireRole({"WORKSHOP_MGR"}); return m_real->logRepair(ci, wi, ii, mi, sd, n);
    }
    bool completeRepair(int rid, const std::string& ed, double cost) override {
        requireRole({"WORKSHOP_MGR"}); return m_real->completeRepair(rid, ed, cost);
    }
    bool approveClaim(int id) override {
        requireRole({"ADMIN"}); return m_real->approveClaim(id);
    }
    bool rejectClaim(int id) override {
        requireRole({"ADMIN"}); return m_real->rejectClaim(id);
    }
    bool assignWorkshopManager(int wid, int sid) override {
        requireRole({"ADMIN"}); return m_real->assignWorkshopManager(wid, sid);
    }
    bool unassignWorkshopManager(int wid) override {
        requireRole({"ADMIN"}); return m_real->unassignWorkshopManager(wid);
    }

private:
    void requireRole(const std::vector<std::string>& roles) const {
        for (const auto& r : roles)
            if (m_user.getRole() == r) return;
        throw std::runtime_error("Access denied: role '" + m_user.getRole() + "' not permitted.");
    }
    std::shared_ptr<IInsuranceService> m_real;
    Staff m_user;
};


class ReportTemplate {
public:
    virtual ~ReportTemplate() = default;
    
    void generate(const DB& db) {
        printTitle();
        printColumnHeaders();
        Utils::printLine('-');
        printRows(db);
        printSummary(db);
    }
protected:
    virtual void printTitle()          const = 0;
    virtual void printColumnHeaders()  const = 0;
    virtual void printRows(const DB&)  const = 0;
    virtual void printSummary(const DB&) const {}
};

class AllCustomersReport : public ReportTemplate {
    void printTitle() const override { Utils::printHeader("ALL CUSTOMERS"); }
    void printColumnHeaders() const override {
        std::cout << std::left << std::setw(5) << "ID"
                  << std::setw(22) << "Name"
                  << std::setw(18) << "CNIC"
                  << std::setw(14) << "Phone"
                  << "Reg Date\n";
    }
    void printRows(const DB& db) const override {
        for (const auto& c : db.customer->loadAll())
            std::cout << std::setw(5)  << c.getId()
                      << std::setw(22) << c.getName()
                      << std::setw(18) << c.getCnic()
                      << std::setw(14) << c.getPhone()
                      << c.getRegDate() << "\n";
    }
    void printSummary(const DB& db) const override {
        std::cout << "  Total: " << db.customer->loadAll().size() << "\n";
    }
};

class AllVehiclesReport : public ReportTemplate {
    void printTitle() const override { Utils::printHeader("ALL VEHICLES"); }
    void printColumnHeaders() const override {
        std::cout << std::left << std::setw(5)  << "ID"
                  << std::setw(11) << "Reg No"
                  << std::setw(11) << "Make"
                  << std::setw(11) << "Model"
                  << std::setw(6)  << "Year"
                  << "Cust ID\n";
    }
    void printRows(const DB& db) const override {
        for (const auto& v : db.vehicle->loadAll())
            std::cout << std::setw(5)  << v.getId()
                      << std::setw(11) << v.getRegNo()
                      << std::setw(11) << v.getMake()
                      << std::setw(11) << v.getModel()
                      << std::setw(6)  << v.getYear()
                      << v.getCustomerId() << "\n";
    }
};

class AllPoliciesReport : public ReportTemplate {
    void printTitle() const override { Utils::printHeader("ALL POLICIES"); }
    void printColumnHeaders() const override {
        std::cout << std::left << std::setw(5)  << "ID"
                  << std::setw(16) << "Policy No"
                  << std::setw(15) << "Type"
                  << std::setw(12) << "Status"
                  << std::setw(12) << "Start"
                  << "End\n";
    }
    void printRows(const DB& db) const override {
        for (const auto& p : db.policy->loadAll())
            std::cout << std::setw(5)  << p.getId()
                      << std::setw(16) << p.getPolicyNo()
                      << std::setw(15) << p.getType()
                      << std::setw(12) << p.getStatus()
                      << std::setw(12) << p.getStartDate()
                      << p.getEndDate() << "\n";
    }
    void printSummary(const DB& db) const override {
        std::cout << "  Total: " << db.policy->loadAll().size() << "\n";
    }
};

class PendingClaimsReport : public ReportTemplate {
    void printTitle() const override { Utils::printHeader("PENDING / UNDER-REVIEW CLAIMS"); }
    void printColumnHeaders() const override {
        std::cout << std::left << std::setw(6)  << "ID"
                  << std::setw(14) << "Claim No"
                  << std::setw(13) << "Filed Date"
                  << std::setw(16) << "Status"
                  << "Est. Loss (PKR)\n";
    }
    void printRows(const DB& db) const override {
        auto p1 = db.claimsByStatus("PENDING");
        auto p2 = db.claimsByStatus("UNDER_REVIEW");
        p1.insert(p1.end(), p2.begin(), p2.end());
        
        ClaimSortByLoss sorter;
        sorter.sort(p1);
        for (const auto& c : p1)
            std::cout << std::setw(6)  << c.getId()
                      << std::setw(14) << c.getClaimNo()
                      << std::setw(13) << c.getFiledDate()
                      << std::setw(16) << c.getStatus()
                      << c.getEstimatedLoss() << "\n";
    }
};

class WorkshopRepairsReport : public ReportTemplate {
    int m_workshopId;
public:
    explicit WorkshopRepairsReport(int wid) : m_workshopId(wid) {}
    void printTitle() const override { Utils::printHeader("WORKSHOP REPAIRS REPORT"); }
    void printColumnHeaders() const override {
        std::cout << std::left << std::setw(5)  << "ID"
                  << std::setw(9)  << "ClaimID"
                  << std::setw(13) << "Start"
                  << std::setw(13) << "End"
                  << std::setw(14) << "Status"
                  << "Cost (PKR)\n";
    }
    void printRows(const DB& db) const override {
        for (const auto& r : db.repairsByWorkshop(m_workshopId))
            std::cout << std::setw(5)  << r.getId()
                      << std::setw(9)  << r.getClaimId()
                      << std::setw(13) << r.getStartDate()
                      << std::setw(13) << r.getEndDate()
                      << std::setw(14) << r.getStatus()
                      << r.getRepairCost() << "\n";
    }
};

//   UI MENU 



class IRenderer {
public:
    virtual ~IRenderer() = default;
    virtual void renderTitle(const std::string& title) const = 0;
    virtual void renderOption(int key, const std::string& label) const = 0;
    virtual void renderError(const std::string& msg) const = 0;
};

class ConsoleRenderer : public IRenderer {
public:
    void renderTitle(const std::string& t) const override { Utils::printHeader(t); }
    void renderOption(int key, const std::string& label) const override {
        std::cout << "  " << key << ". " << label << "\n";
    }
    void renderError(const std::string& msg) const override {
        std::cout << "  [!] " << msg << "\n";
    }
};

    
class MenuNode {
public:
    virtual ~MenuNode() = default;
    virtual void run(IInsuranceService& svc) = 0;
    virtual int  key()   const = 0;
    virtual std::string label() const = 0;
};


class MenuLeaf : public MenuNode {
public:
    MenuLeaf(int key, std::string label, std::function<void(IInsuranceService&)> action)
        : m_key(key), m_label(std::move(label)), m_action(std::move(action)) {}
    void run(IInsuranceService& svc) override { m_action(svc); }
    int         key()   const override { return m_key; }
    std::string label() const override { return m_label; }
private:
    int    m_key;
    std::string m_label;
    std::function<void(IInsuranceService&)> m_action;
};

class MenuGroup : public MenuNode {
public:
    MenuGroup(int key, std::string title, std::shared_ptr<IRenderer> renderer)
        : m_key(key), m_title(std::move(title)), m_renderer(std::move(renderer)) {}

    void add(std::shared_ptr<MenuNode> child) { m_children.push_back(child); }

    void run(IInsuranceService& svc) override {
        int ch;
        do {
            m_renderer->renderTitle(m_title);
            for (const auto& c : m_children)
                m_renderer->renderOption(c->key(), c->label());
            m_renderer->renderOption(0, "Back / Logout");
            ch = Utils::readInt("  Choice: ");
            bool found = false;
            for (auto& c : m_children) {
                if (c->key() == ch) {
                    try { c->run(svc); }
                    catch (const std::exception& e) {
                        m_renderer->renderError(e.what());
                        Utils::pause();
                    }
                    found = true; break;
                }
            }
            if (!found && ch != 0)
                m_renderer->renderError("Invalid option.");
        } while (ch != 0);
    }

    int         key()   const override { return m_key; }
    std::string label() const override { return m_title; }

private:
    int m_key;
    std::string m_title;
    std::shared_ptr<IRenderer> m_renderer;
    std::vector<std::shared_ptr<MenuNode>> m_children;
};


static std::shared_ptr<MenuGroup> buildAdminMenu(const Staff& user,
                                                  std::shared_ptr<IRenderer> r) {
    auto menu = std::make_shared<MenuGroup>(0, "ADMIN MENU  –  " + user.getName(), r);

    // ── Staff Management ───────────────────────────────────────────────────
    menu->add(std::make_shared<MenuLeaf>(1, "Add New Staff", [](IInsuranceService& svc) {
        std::string n  = Utils::readStr("  Name     : ");
        std::string ro = Utils::readStr("  Role (ADMIN/SALESMAN/SURVEYOR/WORKSHOP_MGR): ");
        std::string u  = Utils::readStr("  Username : ");
        std::string p  = Utils::readStr("  Password : ");
        std::string ph = Utils::readStr("  Phone    : ");
        std::string em = Utils::readStr("  Email    : ");
        int id = svc.db().staff->nextId();
        Staff s(id, n, ro, u, p, ph, em, 0);
        svc.db().staff->add(s);
        std::cout << "  Staff added. ID=" << id << "\n";
        Utils::pause();
    }));

    menu->add(std::make_shared<MenuLeaf>(2, "List All Staff", [](IInsuranceService& svc) {
        Utils::printHeader("ALL STAFF");
        std::cout << std::left << std::setw(5) << "ID"
                  << std::setw(20) << "Name"
                  << std::setw(15) << "Role"
                  << std::setw(14) << "Phone"
                  << "Email\n";
        Utils::printLine();
        for (const auto& s : svc.db().staff->loadAll())
            std::cout << std::setw(5)  << s.getId()
                      << std::setw(20) << s.getName()
                      << std::setw(15) << s.getRole()
                      << std::setw(14) << s.getPhone()
                      << s.getEmail() << "\n";
        Utils::pause();
    }));

    menu->add(std::make_shared<MenuLeaf>(3, "Register Customer", [](IInsuranceService& svc) {
        auto n  = Utils::readStr("  Name    : ");
        auto cn = Utils::readStr("  CNIC    : ");
        auto ph = Utils::readStr("  Phone   : ");
        auto em = Utils::readStr("  Email   : ");
        auto ad = Utils::readStr("  Address : ");
        Customer c = svc.registerCustomer(n, cn, ph, em, ad);
        std::cout << "  Customer registered. ID=" << c.getId() << "\n";
        Utils::pause();
    }));

    menu->add(std::make_shared<MenuLeaf>(4, "Register Vehicle", [](IInsuranceService& svc) {
        int cid     = Utils::readInt("  Customer ID   : ");
        auto rn     = Utils::readStr("  Reg No        : ");
        auto mk     = Utils::readStr("  Make          : ");
        auto mo     = Utils::readStr("  Model         : ");
        int yr      = Utils::readInt("  Year          : ");
        auto col    = Utils::readStr("  Color         : ");
        auto ch     = Utils::readStr("  Chassis No    : ");
        auto en     = Utils::readStr("  Engine No     : ");
        Vehicle v   = svc.registerVehicle(cid, rn, mk, mo, yr, col, ch, en);
        std::cout << "  Vehicle registered. ID=" << v.getId() << "\n";
        Utils::pause();
    }));

    menu->add(std::make_shared<MenuLeaf>(5, "Manage Workshops", [r](IInsuranceService& svc) {
        auto sub = std::make_shared<MenuGroup>(0, "WORKSHOP MANAGEMENT", r);
        sub->add(std::make_shared<MenuLeaf>(1, "Add Workshop", [](IInsuranceService& svc) {
            auto n  = Utils::readStr("  Name       : ");
            auto a  = Utils::readStr("  Address    : ");
            auto ph = Utils::readStr("  Phone      : ");
            auto ow = Utils::readStr("  Owner Name : ");
            int id  = svc.db().workshop->nextId();
            Workshop w(id, n, a, ph, ow, 0);
            svc.db().workshop->add(w);
            std::cout << "  Workshop added. ID=" << id << "\n";
            Utils::pause();
        }));
        sub->add(std::make_shared<MenuLeaf>(2, "List Workshops", [](IInsuranceService& svc) {
            Utils::printHeader("ALL WORKSHOPS");
            for (const auto& w : svc.db().workshop->loadAll())
                std::cout << "  [" << w.getId() << "] " << w.getName()
                          << " | " << w.getAddress()
                          << " | Manager ID: " << w.getManagerId() << "\n";
            Utils::pause();
        }));
        sub->add(std::make_shared<MenuLeaf>(3, "Assign Manager", [](IInsuranceService& svc) {
            int wid = Utils::readInt("  Workshop ID : ");
            int sid = Utils::readInt("  Staff ID    : ");
            svc.assignWorkshopManager(wid, sid);
            std::cout << "  Manager assigned.\n";
            Utils::pause();
        }));
        sub->add(std::make_shared<MenuLeaf>(4, "Unassign Manager", [](IInsuranceService& svc) {
            int wid = Utils::readInt("  Workshop ID : ");
            svc.unassignWorkshopManager(wid);
            std::cout << "  Manager unassigned.\n";
            Utils::pause();
        }));
        sub->run(svc);
    }));

    menu->add(std::make_shared<MenuLeaf>(6, "Approve / Reject Claim", [](IInsuranceService& svc) {
        PendingClaimsReport{}.generate(svc.db());
        int cid = Utils::readInt("  Claim ID   : ");
        auto choice = Utils::readStr("  Action (A=Approve / R=Reject): ");
        if (choice == "A" || choice == "a") {
            svc.approveClaim(cid);
            std::cout << "  Claim APPROVED.\n";
        } else {
            svc.rejectClaim(cid);
            std::cout << "  Claim REJECTED.\n";
        }
        Utils::pause();
    }));

    menu->add(std::make_shared<MenuLeaf>(7, "Reports", [r](IInsuranceService& svc) {
        auto sub = std::make_shared<MenuGroup>(0, "REPORTS", r);
        sub->add(std::make_shared<MenuLeaf>(1, "All Customers",  [](IInsuranceService& svc){ AllCustomersReport{}.generate(svc.db()); Utils::pause(); }));
        sub->add(std::make_shared<MenuLeaf>(2, "All Vehicles",   [](IInsuranceService& svc){ AllVehiclesReport{}.generate(svc.db());  Utils::pause(); }));
        sub->add(std::make_shared<MenuLeaf>(3, "All Policies",   [](IInsuranceService& svc){ AllPoliciesReport{}.generate(svc.db());  Utils::pause(); }));
        sub->add(std::make_shared<MenuLeaf>(4, "Pending Claims", [](IInsuranceService& svc){ PendingClaimsReport{}.generate(svc.db()); Utils::pause(); }));
        sub->add(std::make_shared<MenuLeaf>(5, "New Customers by Month", [](IInsuranceService& svc) {
            auto ym = Utils::readStr("  Year-Month (YYYY-MM): ");
            auto list = svc.db().customersByMonth(ym);
            Utils::printHeader("NEW CUSTOMERS [" + ym + "]");
            std::cout << "  Total: " << list.size() << "\n";
            for (const auto& c : list)
                std::cout << "  [" << c.getId() << "] " << c.getName() << "  " << c.getCnic() << "\n";
            Utils::pause();
        }));
        sub->run(svc);
    }));

    return menu;
}

static std::shared_ptr<MenuGroup> buildSalesmanMenu(const Staff& user,
                                                      std::shared_ptr<IRenderer> r) {
    auto menu = std::make_shared<MenuGroup>(0, "SALESMAN MENU  –  " + user.getName(), r);

    menu->add(std::make_shared<MenuLeaf>(1, "Register Customer", [](IInsuranceService& svc) {
        auto n  = Utils::readStr("  Name    : ");
        auto cn = Utils::readStr("  CNIC    : ");
        auto ph = Utils::readStr("  Phone   : ");
        auto em = Utils::readStr("  Email   : ");
        auto ad = Utils::readStr("  Address : ");
        Customer c = svc.registerCustomer(n, cn, ph, em, ad);
        std::cout << "  Customer registered. ID=" << c.getId() << "\n";
        Utils::pause();
    }));

    menu->add(std::make_shared<MenuLeaf>(2, "Register Vehicle", [](IInsuranceService& svc) {
        int cid  = Utils::readInt("  Customer ID: ");
        auto rn  = Utils::readStr("  Reg No     : ");
        auto mk  = Utils::readStr("  Make       : ");
        auto mo  = Utils::readStr("  Model      : ");
        int yr   = Utils::readInt("  Year       : ");
        auto col = Utils::readStr("  Color      : ");
        auto ch  = Utils::readStr("  Chassis    : ");
        auto en  = Utils::readStr("  Engine     : ");
        Vehicle v = svc.registerVehicle(cid, rn, mk, mo, yr, col, ch, en);
        std::cout << "  Vehicle registered. ID=" << v.getId() << "\n";
        Utils::pause();
    }));

    menu->add(std::make_shared<MenuLeaf>(3, "Issue Policy", [&user](IInsuranceService& svc) {
        int cid   = Utils::readInt("  Customer ID : ");
        int vid   = Utils::readInt("  Vehicle ID  : ");
        auto type = Utils::readStr("  Type (COMPREHENSIVE/THIRD_PARTY): ");
        double prem = Utils::readDouble("  Premium (PKR) : ");
        double cov  = Utils::readDouble("  Coverage (PKR): ");
        auto st   = Utils::readStr("  Start Date (YYYY-MM-DD): ");
        auto en   = Utils::readStr("  End Date   (YYYY-MM-DD): ");
        PolicyBuilder pb;
        pb.customer(cid).vehicle(vid).salesman(user.getId())
          .type(type).premium(prem).coverage(cov).startDate(st).endDate(en);
        InsurancePolicy p = svc.issuePolicy(pb);
        std::cout << "  Policy issued. No=" << p.getPolicyNo() << "\n";
        Utils::pause();
    }));

    menu->add(std::make_shared<MenuLeaf>(4, "File a Claim", [](IInsuranceService& svc) {
        int pid   = Utils::readInt("  Policy ID      : ");
        auto inc  = Utils::readStr("  Incident Date  : ");
        auto desc = Utils::readStr("  Description    : ");
        double loss = Utils::readDouble("  Estimated Loss : ");
        InsurancePolicy pol = svc.db().policy->findById(pid);
        if (pol.getId() == 0) { std::cout << "  Policy not found.\n"; Utils::pause(); return; }
        ClaimBuilder cb;
        cb.policy(pid).customer(pol.getCustomerId()).vehicle(pol.getVehicleId())
          .incidentDate(inc).description(desc).estimatedLoss(loss);
        Claim c = svc.fileClaim(cb);
        std::cout << "  Claim filed. No=" << c.getClaimNo() << "\n";
        Utils::pause();
    }));

    menu->add(std::make_shared<MenuLeaf>(5, "View Customer Claims", [](IInsuranceService& svc) {
        int cid = Utils::readInt("  Customer ID: ");
        Customer cust = svc.db().customer->findById(cid);
        Utils::printHeader("CLAIMS – " + cust.getName());
        for (const auto& c : svc.db().claimsByCustomer(cid))
            std::cout << "  [" << c.getId() << "] " << c.getClaimNo()
                      << "  " << c.getStatus()
                      << "  PKR " << c.getEstimatedLoss() << "\n";
        Utils::pause();
    }));

    menu->add(std::make_shared<MenuLeaf>(6, "View All Policies", [](IInsuranceService& svc) {
        AllPoliciesReport{}.generate(svc.db());
        Utils::pause();
    }));

    return menu;
}

static std::shared_ptr<MenuGroup> buildSurveyorMenu(const Staff& user,
                                                      std::shared_ptr<IRenderer> r) {
    auto menu = std::make_shared<MenuGroup>(0, "SURVEYOR MENU  –  " + user.getName(), r);

    menu->add(std::make_shared<MenuLeaf>(1, "Schedule Inspection", [&user](IInsuranceService& svc) {
        PendingClaimsReport{}.generate(svc.db());
        int cid    = Utils::readInt("  Claim ID    : ");
        // Show workshops
        Utils::printHeader("WORKSHOPS");
        for (const auto& w : svc.db().workshop->loadAll())
            std::cout << "  [" << w.getId() << "] " << w.getName() << "\n";
        int wid    = Utils::readInt("  Workshop ID : ");
        auto date  = Utils::readStr("  Insp Date (YYYY-MM-DD): ");
        Inspection i = svc.scheduleInspection(cid, user.getId(), wid, date);
        std::cout << "  Inspection scheduled. ID=" << i.getId() << "\n";
        Utils::pause();
    }));

    menu->add(std::make_shared<MenuLeaf>(2, "Submit Inspection Report", [&user](IInsuranceService& svc) {
        auto insps = svc.db().inspectionsBySurveyor(user.getId());
        Utils::printHeader("YOUR SCHEDULED INSPECTIONS");
        for (const auto& i : insps)
            if (i.getStatus() == "SCHEDULED")
                std::cout << "  [" << i.getId() << "] Claim:" << i.getClaimId()
                          << "  Date:" << i.getInspDate() << "\n";
        int iid    = Utils::readInt("  Inspection ID : ");
        auto find  = Utils::readStr("  Findings      : ");
        double cost = Utils::readDouble("  Repair Cost   : ");
        auto rec   = Utils::readStr("  Recommendation: ");
        if (svc.submitInspectionReport(iid, find, cost, rec))
            std::cout << "  Report submitted.\n";
        else
            std::cout << "  Not found.\n";
        Utils::pause();
    }));

    menu->add(std::make_shared<MenuLeaf>(3, "View My Inspections", [&user](IInsuranceService& svc) {
        Utils::printHeader("MY INSPECTIONS");
        for (const auto& i : svc.db().inspectionsBySurveyor(user.getId())) {
            Staff sv = svc.db().staff->findById(i.getSurveyorId());
            Workshop w = svc.db().workshop->findById(i.getWorkshopId());
            std::cout << "  [" << i.getId() << "] Claim:" << i.getClaimId()
                      << " Date:" << i.getInspDate()
                      << " Workshop:" << w.getName()
                      << " Status:" << i.getStatus()
                      << " Cost:" << i.getRepairCost() << "\n";
        }
        Utils::pause();
    }));

    menu->add(std::make_shared<MenuLeaf>(4, "View Inspection Detail", [](IInsuranceService& svc) {
        int cid = Utils::readInt("  Claim ID: ");
        Claim cl = svc.db().claim->findById(cid);
        if (cl.getId() == 0) { std::cout << "  Not found.\n"; Utils::pause(); return; }
        Utils::printHeader("INSPECTION REPORT – " + cl.getClaimNo());
        std::cout << "  Filed   : " << cl.getFiledDate()   << "\n"
                  << "  Status  : " << cl.getStatus()      << "\n"
                  << "  Est Loss: PKR " << cl.getEstimatedLoss() << "\n\n";
        for (const auto& i : svc.db().inspectionsByClaim(cid)) {
            Workshop w = svc.db().workshop->findById(i.getWorkshopId());
            std::cout << "  Insp[" << i.getId() << "] " << i.getInspDate()
                      << " | Workshop: " << w.getName()
                      << " | Findings: " << i.getFindings()
                      << " | Cost: PKR " << i.getRepairCost()
                      << " | Status: " << i.getStatus() << "\n";
        }
        Utils::pause();
    }));

    return menu;
}

static std::shared_ptr<MenuGroup> buildWorkshopMgrMenu(const Staff& user,
                                                         std::shared_ptr<IRenderer> r) {
    Workshop myWorkshop;
    auto menu = std::make_shared<MenuGroup>(0, "WORKSHOP MGR MENU  –  " + user.getName(), r);

    menu->add(std::make_shared<MenuLeaf>(1, "View My Workshop's Repairs", [&user](IInsuranceService& svc) {
        Workshop w = svc.db().workshopByManager(user.getId());
        if (w.getId() == 0) { std::cout << "  Not assigned to any workshop.\n"; Utils::pause(); return; }
        WorkshopRepairsReport{w.getId()}.generate(svc.db());
        Utils::pause();
    }));

    menu->add(std::make_shared<MenuLeaf>(2, "Log New Repair Job", [&user](IInsuranceService& svc) {
        Workshop w = svc.db().workshopByManager(user.getId());
        if (w.getId() == 0) { std::cout << "  Not assigned.\n"; Utils::pause(); return; }
        std::cout << "  Approved claims with completed inspections at " << w.getName() << ":\n";
        bool any = false;
        for (const auto& i : svc.db().insp->loadAll()) {
            if (i.getWorkshopId() == w.getId() && i.getStatus() == "COMPLETED") {
                Claim cl = svc.db().claim->findById(i.getClaimId());
                if (cl.getId() != 0 && cl.getStatus() == "APPROVED") {
                    std::cout << "    Claim[" << cl.getId() << "] " << cl.getClaimNo()
                              << "  InspID:" << i.getId()
                              << "  Cost: PKR " << i.getRepairCost() << "\n";
                    any = true;
                }
            }
        }
        if (!any) { std::cout << "  No approved claims ready.\n"; Utils::pause(); return; }
        int cid    = Utils::readInt("  Claim ID       : ");
        int iid    = Utils::readInt("  Inspection ID  : ");
        Inspection insp = svc.db().insp->findById(iid);
        if (insp.getId() == 0 || insp.getWorkshopId() != w.getId()) {
            std::cout << "  Inspection not found or not your workshop.\n"; Utils::pause(); return;
        }
        auto start = Utils::readStr("  Start Date (YYYY-MM-DD): ");
        auto notes = Utils::readStr("  Notes          : ");
        Repair rep = svc.logRepair(cid, w.getId(), iid, user.getId(), start, notes);
        std::cout << "  Repair logged. ID=" << rep.getId() << "\n";
        Utils::pause();
    }));

    menu->add(std::make_shared<MenuLeaf>(3, "Mark Repair as Complete", [&user](IInsuranceService& svc) {
        Workshop w = svc.db().workshopByManager(user.getId());
        if (w.getId() == 0) { std::cout << "  Not assigned.\n"; Utils::pause(); return; }
        bool any = false;
        for (const auto& rep : svc.db().repairsByWorkshop(w.getId()))
            if (rep.getStatus() == "IN_PROGRESS") {
                std::cout << "  [" << rep.getId() << "] Claim:" << rep.getClaimId()
                          << " Started:" << rep.getStartDate() << "\n";
                any = true;
            }
        if (!any) { std::cout << "  None in progress.\n"; Utils::pause(); return; }
        int rid = Utils::readInt("  Repair ID     : ");
        Repair rep = svc.db().repair->findById(rid);
        if (rep.getId() == 0 || rep.getWorkshopId() != w.getId()) {
            std::cout << "  Not found or not your workshop.\n"; Utils::pause(); return;
        }
        auto ed    = Utils::readStr("  End Date (YYYY-MM-DD): ");
        double cost = Utils::readDouble("  Final Cost (PKR)     : ");
        if (svc.completeRepair(rid, ed, cost))
            std::cout << "  Repair marked COMPLETE.\n";
        Utils::pause();
    }));

    menu->add(std::make_shared<MenuLeaf>(4, "View Inspections at My Workshop", [&user](IInsuranceService& svc) {
        Workshop w = svc.db().workshopByManager(user.getId());
        if (w.getId() == 0) { std::cout << "  Not assigned.\n"; Utils::pause(); return; }
        Utils::printHeader("INSPECTIONS AT " + w.getName());
        for (const auto& i : svc.db().insp->loadAll()) {
            if (i.getWorkshopId() != w.getId()) continue;
            Staff sv = svc.db().staff->findById(i.getSurveyorId());
            std::cout << "  [" << i.getId() << "] Claim:" << i.getClaimId()
                      << " Date:" << i.getInspDate()
                      << " Surveyor:" << sv.getName()
                      << " Status:" << i.getStatus()
                      << " Cost:" << i.getRepairCost() << "\n";
        }
        Utils::pause();
    }));

    menu->add(std::make_shared<MenuLeaf>(5, "View Claim Details", [](IInsuranceService& svc) {
        int cid = Utils::readInt("  Claim ID: ");
        Claim c = svc.db().claim->findById(cid);
        if (c.getId() == 0) { std::cout << "  Not found.\n"; Utils::pause(); return; }
        Utils::printLine();
        std::cout << "  Claim No    : " << c.getClaimNo()      << "\n"
                  << "  Incident    : " << c.getIncidentDate() << "\n"
                  << "  Description : " << c.getDescription()  << "\n"
                  << "  Est. Loss   : PKR " << c.getEstimatedLoss() << "\n"
                  << "  Status      : " << c.getStatus()       << "\n";
        Utils::pause();
    }));

    return menu;
}


//   APPLICATION CONTROLLER 


class Application {
public:
    Application() {
        
#ifdef _WIN32
        _mkdir("data");
#else
        mkdir("data", 0755);
#endif
        
        FileRepositoryFactory factory(AppConfig::instance().dataDir());
        m_db = std::make_shared<DB>(factory);

        
        if (m_db->staff->loadAll().empty()) {
            Staff admin(1, "System Admin", "ADMIN", "admin", "admin123",
                        "0300-0000000", "admin@autoshield.com", 0);
            m_db->staff->add(admin);
            Utils::printLine('*');
            std::cout << "  First-run setup: admin account created.\n"
                      << "  Username: admin   Password: admin123\n";
            Utils::printLine('*');
        }

        
        EventBus::instance().subscribe("CLAIM_FILED",    &m_auditor);
        EventBus::instance().subscribe("CLAIM_APPROVED", &m_auditor);
        EventBus::instance().subscribe("CLAIM_REJECTED", &m_auditor);

        m_renderer = std::make_shared<ConsoleRenderer>();
    }

    void run() {
        
        auto coreService = std::make_shared<InsuranceService>(m_db);

        Utils::printHeader("AutoShield Insurance Management System");
        

        while (true) {
            Utils::printHeader("LOGIN");
            Staff user = loginScreen(*coreService);
            if (user.getId() == 0) {
                std::cout << "  Exiting. Goodbye!\n";
                break;
            }

            
            auto guardedService = std::make_shared<AuthProxy>(coreService, user);

            std::shared_ptr<MenuGroup> menu;
            const std::string& role = user.getRole();
            if      (role == "ADMIN")        menu = buildAdminMenu(user, m_renderer);
            else if (role == "SALESMAN")     menu = buildSalesmanMenu(user, m_renderer);
            else if (role == "SURVEYOR")     menu = buildSurveyorMenu(user, m_renderer);
            else if (role == "WORKSHOP_MGR") menu = buildWorkshopMgrMenu(user, m_renderer);
            else {
                std::cout << "  [!] Unknown role '" << role << "'. Contact Admin.\n";
                continue;
            }

            menu->run(*guardedService);
        }
    }

private:
    Staff loginScreen(IInsuranceService& svc) {
        int maxAttempts = AppConfig::instance().maxLoginAttempts();
        for (int attempt = 0; attempt < maxAttempts; ++attempt) {
            std::string u = Utils::readStr("  Username (or 'quit'): ");
            if (u == "quit") return Staff();
            std::string p = Utils::readStr("  Password            : ");
            Staff s = svc.login(u, p);
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

    std::shared_ptr<DB>        m_db;
    std::shared_ptr<IRenderer> m_renderer;
    AuditListener              m_auditor;
};

//  MAIN

int main() {
    Application app;
    app.run();
    return 0;
}
