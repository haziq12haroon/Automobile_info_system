#include "InsuranceEntities.h"
#include <sstream>

static std::string tok(std::istringstream& ss) {
    std::string t;
    std::getline(ss, t, '|');
    return t;
}

// ─────────────────────────────────────────────
//  InsurancePolicy
// ─────────────────────────────────────────────
InsurancePolicy::InsurancePolicy()
    : m_id(0), m_customerId(0), m_vehicleId(0), m_salesmanId(0),
      m_premiumAmount(0.0), m_coverageAmount(0.0) {}

InsurancePolicy::InsurancePolicy(int id, int customerId, int vehicleId, int salesmanId,
                                 const std::string& policyNo, const std::string& type,
                                 double premiumAmount, double coverageAmount,
                                 const std::string& startDate, const std::string& endDate,
                                 const std::string& status)
    : m_id(id), m_customerId(customerId), m_vehicleId(vehicleId), m_salesmanId(salesmanId),
      m_policyNo(policyNo), m_type(type), m_premiumAmount(premiumAmount),
      m_coverageAmount(coverageAmount), m_startDate(startDate), m_endDate(endDate),
      m_status(status) {}

int         InsurancePolicy::getId()             const { return m_id; }
int         InsurancePolicy::getCustomerId()     const { return m_customerId; }
int         InsurancePolicy::getVehicleId()      const { return m_vehicleId; }
int         InsurancePolicy::getSalesmanId()     const { return m_salesmanId; }
std::string InsurancePolicy::getPolicyNo()       const { return m_policyNo; }
std::string InsurancePolicy::getType()           const { return m_type; }
double      InsurancePolicy::getPremiumAmount()  const { return m_premiumAmount; }
double      InsurancePolicy::getCoverageAmount() const { return m_coverageAmount; }
std::string InsurancePolicy::getStartDate()      const { return m_startDate; }
std::string InsurancePolicy::getEndDate()        const { return m_endDate; }
std::string InsurancePolicy::getStatus()         const { return m_status; }

void InsurancePolicy::setStatus(const std::string& s) { m_status = s; }

std::string InsurancePolicy::serialize() const {
    std::ostringstream ss;
    ss << m_id << '|' << m_customerId << '|' << m_vehicleId << '|' << m_salesmanId << '|'
       << m_policyNo << '|' << m_type << '|' << m_premiumAmount << '|' << m_coverageAmount
       << '|' << m_startDate << '|' << m_endDate << '|' << m_status;
    return ss.str();
}

InsurancePolicy InsurancePolicy::deserialize(const std::string& line) {
    std::istringstream ss(line);
    int id             = std::stoi(tok(ss));
    int custId         = std::stoi(tok(ss));
    int vehId          = std::stoi(tok(ss));
    int salId          = std::stoi(tok(ss));
    std::string polNo  = tok(ss);
    std::string type   = tok(ss);
    double premium     = std::stod(tok(ss));
    double coverage    = std::stod(tok(ss));
    std::string start  = tok(ss);
    std::string end    = tok(ss);
    std::string status = tok(ss);
    return InsurancePolicy(id, custId, vehId, salId, polNo, type, premium, coverage, start, end, status);
}

// ─────────────────────────────────────────────
//  Claim
// ─────────────────────────────────────────────
Claim::Claim()
    : m_id(0), m_policyId(0), m_customerId(0), m_vehicleId(0), m_estimatedLoss(0.0) {}

Claim::Claim(int id, int policyId, int customerId, int vehicleId,
             const std::string& claimNo, const std::string& incidentDate,
             const std::string& filedDate, const std::string& description,
             double estimatedLoss, const std::string& status)
    : m_id(id), m_policyId(policyId), m_customerId(customerId), m_vehicleId(vehicleId),
      m_claimNo(claimNo), m_incidentDate(incidentDate), m_filedDate(filedDate),
      m_description(description), m_estimatedLoss(estimatedLoss), m_status(status) {}

int         Claim::getId()            const { return m_id; }
int         Claim::getPolicyId()      const { return m_policyId; }
int         Claim::getCustomerId()    const { return m_customerId; }
int         Claim::getVehicleId()     const { return m_vehicleId; }
std::string Claim::getClaimNo()       const { return m_claimNo; }
std::string Claim::getIncidentDate()  const { return m_incidentDate; }
std::string Claim::getFiledDate()     const { return m_filedDate; }
std::string Claim::getDescription()   const { return m_description; }
double      Claim::getEstimatedLoss() const { return m_estimatedLoss; }
std::string Claim::getStatus()        const { return m_status; }

void Claim::setStatus(const std::string& s)  { m_status = s; }
void Claim::setEstimatedLoss(double loss)     { m_estimatedLoss = loss; }

std::string Claim::serialize() const {
    std::ostringstream ss;
    ss << m_id << '|' << m_policyId << '|' << m_customerId << '|' << m_vehicleId << '|'
       << m_claimNo << '|' << m_incidentDate << '|' << m_filedDate << '|'
       << m_description << '|' << m_estimatedLoss << '|' << m_status;
    return ss.str();
}

Claim Claim::deserialize(const std::string& line) {
    std::istringstream ss(line);
    int id              = std::stoi(tok(ss));
    int polId           = std::stoi(tok(ss));
    int custId          = std::stoi(tok(ss));
    int vehId           = std::stoi(tok(ss));
    std::string claimNo = tok(ss);
    std::string incDate = tok(ss);
    std::string filDate = tok(ss);
    std::string desc    = tok(ss);
    double loss         = std::stod(tok(ss));
    std::string status  = tok(ss);
    return Claim(id, polId, custId, vehId, claimNo, incDate, filDate, desc, loss, status);
}

// ─────────────────────────────────────────────
//  Inspection
// ─────────────────────────────────────────────
Inspection::Inspection()
    : m_id(0), m_claimId(0), m_surveyorId(0), m_workshopId(0), m_repairCost(0.0) {}

Inspection::Inspection(int id, int claimId, int surveyorId, int workshopId,
                        const std::string& inspectionDate, const std::string& findings,
                        double repairCost, const std::string& recommendation,
                        const std::string& status)
    : m_id(id), m_claimId(claimId), m_surveyorId(surveyorId), m_workshopId(workshopId),
      m_inspectionDate(inspectionDate), m_findings(findings),
      m_repairCost(repairCost), m_recommendation(recommendation), m_status(status) {}

int         Inspection::getId()             const { return m_id; }
int         Inspection::getClaimId()        const { return m_claimId; }
int         Inspection::getSurveyorId()     const { return m_surveyorId; }
int         Inspection::getWorkshopId()     const { return m_workshopId; }
std::string Inspection::getInspectionDate() const { return m_inspectionDate; }
std::string Inspection::getFindings()       const { return m_findings; }
double      Inspection::getRepairCost()     const { return m_repairCost; }
std::string Inspection::getRecommendation() const { return m_recommendation; }
std::string Inspection::getStatus()         const { return m_status; }

void Inspection::setFindings(const std::string& f)     { m_findings = f; }
void Inspection::setRepairCost(double c)               { m_repairCost = c; }
void Inspection::setRecommendation(const std::string& r) { m_recommendation = r; }
void Inspection::setStatus(const std::string& s)       { m_status = s; }

std::string Inspection::serialize() const {
    std::ostringstream ss;
    ss << m_id << '|' << m_claimId << '|' << m_surveyorId << '|' << m_workshopId << '|'
       << m_inspectionDate << '|' << m_findings << '|' << m_repairCost << '|'
       << m_recommendation << '|' << m_status;
    return ss.str();
}

Inspection Inspection::deserialize(const std::string& line) {
    std::istringstream ss(line);
    int id               = std::stoi(tok(ss));
    int claimId          = std::stoi(tok(ss));
    int surveyorId       = std::stoi(tok(ss));
    int workshopId       = std::stoi(tok(ss));
    std::string iDate    = tok(ss);
    std::string findings = tok(ss);
    double cost          = std::stod(tok(ss));
    std::string rec      = tok(ss);
    std::string status   = tok(ss);
    return Inspection(id, claimId, surveyorId, workshopId, iDate, findings, cost, rec, status);
}

// ─────────────────────────────────────────────
//  Repair
// ─────────────────────────────────────────────
Repair::Repair()
    : m_id(0), m_claimId(0), m_workshopId(0), m_inspectionId(0), m_repairCost(0.0) {}

Repair::Repair(int id, int claimId, int workshopId, int inspectionId,
               const std::string& startDate, const std::string& endDate,
               double repairCost, const std::string& status, const std::string& notes)
    : m_id(id), m_claimId(claimId), m_workshopId(workshopId), m_inspectionId(inspectionId),
      m_startDate(startDate), m_endDate(endDate), m_repairCost(repairCost),
      m_status(status), m_notes(notes) {}

int         Repair::getId()           const { return m_id; }
int         Repair::getClaimId()      const { return m_claimId; }
int         Repair::getWorkshopId()   const { return m_workshopId; }
int         Repair::getInspectionId() const { return m_inspectionId; }
std::string Repair::getStartDate()    const { return m_startDate; }
std::string Repair::getEndDate()      const { return m_endDate; }
double      Repair::getRepairCost()   const { return m_repairCost; }
std::string Repair::getStatus()       const { return m_status; }
std::string Repair::getNotes()        const { return m_notes; }

void Repair::setStatus(const std::string& s)   { m_status = s; }
void Repair::setEndDate(const std::string& d)  { m_endDate = d; }
void Repair::setRepairCost(double c)           { m_repairCost = c; }

std::string Repair::serialize() const {
    std::ostringstream ss;
    ss << m_id << '|' << m_claimId << '|' << m_workshopId << '|' << m_inspectionId << '|'
       << m_startDate << '|' << m_endDate << '|' << m_repairCost << '|'
       << m_status << '|' << m_notes;
    return ss.str();
}

Repair Repair::deserialize(const std::string& line) {
    std::istringstream ss(line);
    int id             = std::stoi(tok(ss));
    int claimId        = std::stoi(tok(ss));
    int workshopId     = std::stoi(tok(ss));
    int inspId         = std::stoi(tok(ss));
    std::string start  = tok(ss);
    std::string end    = tok(ss);
    double cost        = std::stod(tok(ss));
    std::string status = tok(ss);
    std::string notes  = tok(ss);
    return Repair(id, claimId, workshopId, inspId, start, end, cost, status, notes);
}
