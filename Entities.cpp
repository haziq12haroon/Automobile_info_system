#include "Entities.h"
#include <sstream>
#include <stdexcept>

// ───── helpers ─────
static std::string tok(std::istringstream& ss) {
    std::string t;
    std::getline(ss, t, '|');
    return t;
}

// ─────────────────────────────────────────────
//  Customer
// ─────────────────────────────────────────────
Customer::Customer() : m_id(0) {}

Customer::Customer(int id, const std::string& name, const std::string& cnic,
                   const std::string& phone, const std::string& email,
                   const std::string& address, const std::string& regDate)
    : m_id(id), m_name(name), m_cnic(cnic), m_phone(phone),
      m_email(email), m_address(address), m_regDate(regDate) {}

int         Customer::getId()       const { return m_id; }
std::string Customer::getName()     const { return m_name; }
std::string Customer::getCnic()     const { return m_cnic; }
std::string Customer::getPhone()    const { return m_phone; }
std::string Customer::getEmail()    const { return m_email; }
std::string Customer::getAddress()  const { return m_address; }
std::string Customer::getRegDate()  const { return m_regDate; }

void Customer::setName(const std::string& n)    { m_name = n; }
void Customer::setPhone(const std::string& p)   { m_phone = p; }
void Customer::setEmail(const std::string& e)   { m_email = e; }
void Customer::setAddress(const std::string& a) { m_address = a; }

std::string Customer::serialize() const {
    std::ostringstream ss;
    ss << m_id << '|' << m_name << '|' << m_cnic << '|'
       << m_phone << '|' << m_email << '|' << m_address << '|' << m_regDate;
    return ss.str();
}

Customer Customer::deserialize(const std::string& line) {
    std::istringstream ss(line);
    int id = std::stoi(tok(ss));
    std::string name    = tok(ss);
    std::string cnic    = tok(ss);
    std::string phone   = tok(ss);
    std::string email   = tok(ss);
    std::string address = tok(ss);
    std::string regDate = tok(ss);
    return Customer(id, name, cnic, phone, email, address, regDate);
}

// ─────────────────────────────────────────────
//  Vehicle
// ─────────────────────────────────────────────
Vehicle::Vehicle() : m_id(0), m_customerId(0), m_year(0) {}

Vehicle::Vehicle(int id, int customerId, const std::string& regNo,
                 const std::string& make, const std::string& model,
                 int year, const std::string& color,
                 const std::string& chassisNo, const std::string& engineNo)
    : m_id(id), m_customerId(customerId), m_regNo(regNo),
      m_make(make), m_model(model), m_year(year),
      m_color(color), m_chassisNo(chassisNo), m_engineNo(engineNo) {}

int         Vehicle::getId()         const { return m_id; }
int         Vehicle::getCustomerId() const { return m_customerId; }
std::string Vehicle::getRegNo()      const { return m_regNo; }
std::string Vehicle::getMake()       const { return m_make; }
std::string Vehicle::getModel()      const { return m_model; }
int         Vehicle::getYear()       const { return m_year; }
std::string Vehicle::getColor()      const { return m_color; }
std::string Vehicle::getChassisNo()  const { return m_chassisNo; }
std::string Vehicle::getEngineNo()   const { return m_engineNo; }

std::string Vehicle::serialize() const {
    std::ostringstream ss;
    ss << m_id << '|' << m_customerId << '|' << m_regNo << '|'
       << m_make << '|' << m_model << '|' << m_year << '|'
       << m_color << '|' << m_chassisNo << '|' << m_engineNo;
    return ss.str();
}

Vehicle Vehicle::deserialize(const std::string& line) {
    std::istringstream ss(line);
    int id         = std::stoi(tok(ss));
    int custId     = std::stoi(tok(ss));
    std::string regNo     = tok(ss);
    std::string make      = tok(ss);
    std::string model     = tok(ss);
    int year       = std::stoi(tok(ss));
    std::string color     = tok(ss);
    std::string chassisNo = tok(ss);
    std::string engineNo  = tok(ss);
    return Vehicle(id, custId, regNo, make, model, year, color, chassisNo, engineNo);
}

// ─────────────────────────────────────────────
//  Staff
// ─────────────────────────────────────────────
Staff::Staff() : m_id(0) {}

Staff::Staff(int id, const std::string& name, const std::string& role,
             const std::string& username, const std::string& password,
             const std::string& phone, const std::string& email)
    : m_id(id), m_name(name), m_role(role),
      m_username(username), m_password(password),
      m_phone(phone), m_email(email) {}

int         Staff::getId()       const { return m_id; }
std::string Staff::getName()     const { return m_name; }
std::string Staff::getRole()     const { return m_role; }
std::string Staff::getUsername() const { return m_username; }
std::string Staff::getPassword() const { return m_password; }
std::string Staff::getPhone()    const { return m_phone; }
std::string Staff::getEmail()    const { return m_email; }

void Staff::setPassword(const std::string& pw) { m_password = pw; }

std::string Staff::serialize() const {
    std::ostringstream ss;
    ss << m_id << '|' << m_name << '|' << m_role << '|'
       << m_username << '|' << m_password << '|' << m_phone << '|' << m_email;
    return ss.str();
}

Staff Staff::deserialize(const std::string& line) {
    std::istringstream ss(line);
    int id               = std::stoi(tok(ss));
    std::string name     = tok(ss);
    std::string role     = tok(ss);
    std::string username = tok(ss);
    std::string password = tok(ss);
    std::string phone    = tok(ss);
    std::string email    = tok(ss);
    return Staff(id, name, role, username, password, phone, email);
}

// ─────────────────────────────────────────────
//  Workshop
// ─────────────────────────────────────────────
Workshop::Workshop() : m_id(0) {}

Workshop::Workshop(int id, const std::string& name, const std::string& address,
                   const std::string& phone, const std::string& ownerName)
    : m_id(id), m_name(name), m_address(address),
      m_phone(phone), m_ownerName(ownerName) {}

int         Workshop::getId()        const { return m_id; }
std::string Workshop::getName()      const { return m_name; }
std::string Workshop::getAddress()   const { return m_address; }
std::string Workshop::getPhone()     const { return m_phone; }
std::string Workshop::getOwnerName() const { return m_ownerName; }

std::string Workshop::serialize() const {
    std::ostringstream ss;
    ss << m_id << '|' << m_name << '|' << m_address << '|' << m_phone << '|' << m_ownerName;
    return ss.str();
}

Workshop Workshop::deserialize(const std::string& line) {
    std::istringstream ss(line);
    int id                  = std::stoi(tok(ss));
    std::string name        = tok(ss);
    std::string address     = tok(ss);
    std::string phone       = tok(ss);
    std::string ownerName   = tok(ss);
    return Workshop(id, name, address, phone, ownerName);
}
