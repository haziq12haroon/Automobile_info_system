#!/usr/bin/env python3

SVG = '''<?xml version="1.0" encoding="UTF-8"?>
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 1400 1020" width="1400" height="1020"
     font-family="Arial, sans-serif">

  <defs>
    <marker id="arr" markerWidth="10" markerHeight="7" refX="9" refY="3.5" orient="auto">
      <polygon points="0 0,10 3.5,0 7" fill="#555"/>
    </marker>
    <marker id="diamond" markerWidth="12" markerHeight="8" refX="0" refY="4" orient="auto">
      <polygon points="0 4,6 0,12 4,6 8" fill="none" stroke="#555" stroke-width="1.2"/>
    </marker>
  </defs>

  <!-- Background -->
  <rect width="1400" height="1020" fill="#f4f6f9"/>
  <text x="700" y="32" text-anchor="middle" font-size="20" font-weight="bold" fill="#1a3c5e">
    AutoShield Insurance System – Class Diagram
  </text>

  <!-- ══ ENTITY LAYER label ══ -->
  <rect x="10" y="44" width="130" height="22" rx="4" fill="#1a3c5e" opacity=".85"/>
  <text x="75" y="59" text-anchor="middle" font-size="11" fill="#fff" font-weight="bold">Entity Layer</text>

  <!-- ══ Customer ══ -->
  <g id="Customer">
    <rect x="20" y="72" width="190" height="168" rx="5" fill="#fff" stroke="#1a3c5e" stroke-width="1.8"/>
    <rect x="20" y="72" width="190" height="24"  rx="5" fill="#1a3c5e"/>
    <text x="115" y="88" text-anchor="middle" font-size="12" font-weight="bold" fill="#fff">Customer</text>
    <line x1="20" y1="96" x2="210" y2="96" stroke="#ddd"/>
    <text x="28" y="110" font-size="10" fill="#555">- id: int</text>
    <text x="28" y="123" font-size="10" fill="#555">- name: string</text>
    <text x="28" y="136" font-size="10" fill="#555">- cnic: string</text>
    <text x="28" y="149" font-size="10" fill="#555">- phone: string</text>
    <text x="28" y="162" font-size="10" fill="#555">- email: string</text>
    <text x="28" y="175" font-size="10" fill="#555">- address: string</text>
    <text x="28" y="188" font-size="10" fill="#555">- regDate: string</text>
    <line x1="20" y1="194" x2="210" y2="194" stroke="#ddd"/>
    <text x="28" y="207" font-size="10" fill="#2471a3">+ serialize(): string</text>
    <text x="28" y="220" font-size="10" fill="#2471a3">+ deserialize(line): Customer</text>
    <text x="28" y="233" font-size="10" fill="#2471a3">+ getId(), getName(), ...</text>
  </g>

  <!-- ══ Vehicle ══ -->
  <g id="Vehicle">
    <rect x="230" y="72" width="190" height="180" rx="5" fill="#fff" stroke="#1a3c5e" stroke-width="1.8"/>
    <rect x="230" y="72" width="190" height="24"  rx="5" fill="#1a3c5e"/>
    <text x="325" y="88" text-anchor="middle" font-size="12" font-weight="bold" fill="#fff">Vehicle</text>
    <line x1="230" y1="96" x2="420" y2="96" stroke="#ddd"/>
    <text x="238" y="110" font-size="10" fill="#555">- id: int</text>
    <text x="238" y="123" font-size="10" fill="#555">- customerId: int</text>
    <text x="238" y="136" font-size="10" fill="#555">- regNo: string</text>
    <text x="238" y="149" font-size="10" fill="#555">- make: string</text>
    <text x="238" y="162" font-size="10" fill="#555">- model: string</text>
    <text x="238" y="175" font-size="10" fill="#555">- year: int</text>
    <text x="238" y="188" font-size="10" fill="#555">- color: string</text>
    <text x="238" y="201" font-size="10" fill="#555">- chassisNo: string</text>
    <line x1="230" y1="207" x2="420" y2="207" stroke="#ddd"/>
    <text x="238" y="220" font-size="10" fill="#2471a3">+ serialize(): string</text>
    <text x="238" y="233" font-size="10" fill="#2471a3">+ getId(), getRegNo(), ...</text>
  </g>

  <!-- ══ Staff ══ -->
  <g id="Staff">
    <rect x="440" y="72" width="195" height="180" rx="5" fill="#fff" stroke="#1a3c5e" stroke-width="1.8"/>
    <rect x="440" y="72" width="195" height="24"  rx="5" fill="#1a3c5e"/>
    <text x="537" y="88" text-anchor="middle" font-size="12" font-weight="bold" fill="#fff">Staff</text>
    <text x="537" y="100" text-anchor="middle" font-size="9" fill="#ddd">(MANAGER|SALESMAN|SURVEYOR)</text>
    <line x1="440" y1="104" x2="635" y2="104" stroke="#ddd"/>
    <text x="448" y="118" font-size="10" fill="#555">- id: int</text>
    <text x="448" y="131" font-size="10" fill="#555">- name: string</text>
    <text x="448" y="144" font-size="10" fill="#555">- role: string</text>
    <text x="448" y="157" font-size="10" fill="#555">- username: string</text>
    <text x="448" y="170" font-size="10" fill="#555">- password: string</text>
    <text x="448" y="183" font-size="10" fill="#555">- phone: string</text>
    <text x="448" y="196" font-size="10" fill="#555">- email: string</text>
    <line x1="440" y1="202" x2="635" y2="202" stroke="#ddd"/>
    <text x="448" y="215" font-size="10" fill="#2471a3">+ serialize(): string</text>
    <text x="448" y="228" font-size="10" fill="#2471a3">+ getId(), getRole(), ...</text>
    <text x="448" y="241" font-size="10" fill="#2471a3">+ setPassword(pw)</text>
  </g>

  <!-- ══ Workshop ══ -->
  <g id="Workshop">
    <rect x="655" y="72" width="185" height="155" rx="5" fill="#fff" stroke="#1a3c5e" stroke-width="1.8"/>
    <rect x="655" y="72" width="185" height="24"  rx="5" fill="#1a3c5e"/>
    <text x="747" y="88" text-anchor="middle" font-size="12" font-weight="bold" fill="#fff">Workshop</text>
    <line x1="655" y1="96" x2="840" y2="96" stroke="#ddd"/>
    <text x="663" y="110" font-size="10" fill="#555">- id: int</text>
    <text x="663" y="123" font-size="10" fill="#555">- name: string</text>
    <text x="663" y="136" font-size="10" fill="#555">- address: string</text>
    <text x="663" y="149" font-size="10" fill="#555">- phone: string</text>
    <text x="663" y="162" font-size="10" fill="#555">- ownerName: string</text>
    <line x1="655" y1="168" x2="840" y2="168" stroke="#ddd"/>
    <text x="663" y="181" font-size="10" fill="#2471a3">+ serialize(): string</text>
    <text x="663" y="194" font-size="10" fill="#2471a3">+ getId(), getName(), ...</text>
    <text x="663" y="207" font-size="10" fill="#2471a3">+ deserialize(line): Workshop</text>
  </g>

  <!-- ══ InsurancePolicy ══ -->
  <g id="InsurancePolicy">
    <rect x="860" y="72" width="210" height="195" rx="5" fill="#fff" stroke="#1a3c5e" stroke-width="1.8"/>
    <rect x="860" y="72" width="210" height="24"  rx="5" fill="#1a3c5e"/>
    <text x="965" y="88" text-anchor="middle" font-size="12" font-weight="bold" fill="#fff">InsurancePolicy</text>
    <line x1="860" y1="96" x2="1070" y2="96" stroke="#ddd"/>
    <text x="868" y="110" font-size="10" fill="#555">- id: int</text>
    <text x="868" y="123" font-size="10" fill="#555">- customerId: int</text>
    <text x="868" y="136" font-size="10" fill="#555">- vehicleId: int</text>
    <text x="868" y="149" font-size="10" fill="#555">- salesmanId: int</text>
    <text x="868" y="162" font-size="10" fill="#555">- policyNo: string</text>
    <text x="868" y="175" font-size="10" fill="#555">- type: string</text>
    <text x="868" y="188" font-size="10" fill="#555">- premiumAmount: double</text>
    <text x="868" y="201" font-size="10" fill="#555">- coverageAmount: double</text>
    <text x="868" y="214" font-size="10" fill="#555">- startDate, endDate, status</text>
    <line x1="860" y1="220" x2="1070" y2="220" stroke="#ddd"/>
    <text x="868" y="233" font-size="10" fill="#2471a3">+ serialize(): string</text>
    <text x="868" y="246" font-size="10" fill="#2471a3">+ getId(), getPolicyNo(), ...</text>
    <text x="868" y="259" font-size="10" fill="#2471a3">+ setStatus(s)</text>
  </g>

  <!-- ══ Claim ══ -->
  <g id="Claim">
    <rect x="20" y="310" width="200" height="195" rx="5" fill="#fff" stroke="#27ae60" stroke-width="1.8"/>
    <rect x="20" y="310" width="200" height="24"  rx="5" fill="#27ae60"/>
    <text x="120" y="326" text-anchor="middle" font-size="12" font-weight="bold" fill="#fff">Claim</text>
    <line x1="20" y1="334" x2="220" y2="334" stroke="#ddd"/>
    <text x="28" y="348" font-size="10" fill="#555">- id: int</text>
    <text x="28" y="361" font-size="10" fill="#555">- policyId: int</text>
    <text x="28" y="374" font-size="10" fill="#555">- customerId: int</text>
    <text x="28" y="387" font-size="10" fill="#555">- vehicleId: int</text>
    <text x="28" y="400" font-size="10" fill="#555">- claimNo: string</text>
    <text x="28" y="413" font-size="10" fill="#555">- incidentDate: string</text>
    <text x="28" y="426" font-size="10" fill="#555">- estimatedLoss: double</text>
    <text x="28" y="439" font-size="10" fill="#555">- status: string</text>
    <line x1="20" y1="445" x2="220" y2="445" stroke="#ddd"/>
    <text x="28" y="458" font-size="10" fill="#2471a3">+ serialize(): string</text>
    <text x="28" y="471" font-size="10" fill="#2471a3">+ setStatus(s), getClaimNo()</text>
    <text x="28" y="484" font-size="10" fill="#2471a3">+ getId(), getStatus(), ...</text>
  </g>

  <!-- ══ Inspection ══ -->
  <g id="Inspection">
    <rect x="240" y="310" width="205" height="195" rx="5" fill="#fff" stroke="#27ae60" stroke-width="1.8"/>
    <rect x="240" y="310" width="205" height="24"  rx="5" fill="#27ae60"/>
    <text x="342" y="326" text-anchor="middle" font-size="12" font-weight="bold" fill="#fff">Inspection</text>
    <line x1="240" y1="334" x2="445" y2="334" stroke="#ddd"/>
    <text x="248" y="348" font-size="10" fill="#555">- id: int</text>
    <text x="248" y="361" font-size="10" fill="#555">- claimId: int</text>
    <text x="248" y="374" font-size="10" fill="#555">- surveyorId: int</text>
    <text x="248" y="387" font-size="10" fill="#555">- workshopId: int</text>
    <text x="248" y="400" font-size="10" fill="#555">- inspectionDate: string</text>
    <text x="248" y="413" font-size="10" fill="#555">- findings: string</text>
    <text x="248" y="426" font-size="10" fill="#555">- repairCost: double</text>
    <text x="248" y="439" font-size="10" fill="#555">- recommendation: string</text>
    <line x1="240" y1="445" x2="445" y2="445" stroke="#ddd"/>
    <text x="248" y="458" font-size="10" fill="#2471a3">+ serialize(): string</text>
    <text x="248" y="471" font-size="10" fill="#2471a3">+ setFindings(), setRepairCost()</text>
    <text x="248" y="484" font-size="10" fill="#2471a3">+ submitReport(...)</text>
  </g>

  <!-- ══ Repair ══ -->
  <g id="Repair">
    <rect x="465" y="310" width="195" height="195" rx="5" fill="#fff" stroke="#27ae60" stroke-width="1.8"/>
    <rect x="465" y="310" width="195" height="24"  rx="5" fill="#27ae60"/>
    <text x="562" y="326" text-anchor="middle" font-size="12" font-weight="bold" fill="#fff">Repair</text>
    <line x1="465" y1="334" x2="660" y2="334" stroke="#ddd"/>
    <text x="473" y="348" font-size="10" fill="#555">- id: int</text>
    <text x="473" y="361" font-size="10" fill="#555">- claimId: int</text>
    <text x="473" y="374" font-size="10" fill="#555">- workshopId: int</text>
    <text x="473" y="387" font-size="10" fill="#555">- inspectionId: int</text>
    <text x="473" y="400" font-size="10" fill="#555">- startDate: string</text>
    <text x="473" y="413" font-size="10" fill="#555">- endDate: string</text>
    <text x="473" y="426" font-size="10" fill="#555">- repairCost: double</text>
    <text x="473" y="439" font-size="10" fill="#555">- status: string</text>
    <line x1="465" y1="445" x2="660" y2="445" stroke="#ddd"/>
    <text x="473" y="458" font-size="10" fill="#2471a3">+ serialize(): string</text>
    <text x="473" y="471" font-size="10" fill="#2471a3">+ setStatus(), setEndDate()</text>
    <text x="473" y="484" font-size="10" fill="#2471a3">+ getId(), getStatus(), ...</text>
  </g>

  <!-- ══ SERVICE LAYER label ══ -->
  <rect x="10" y="530" width="130" height="22" rx="4" fill="#8e44ad" opacity=".85"/>
  <text x="75" y="545" text-anchor="middle" font-size="11" fill="#fff" font-weight="bold">Service Layer</text>

  <!-- Services row -->
  <!-- AuthService -->
  <rect x="20"  y="558" width="150" height="80" rx="5" fill="#fff" stroke="#8e44ad" stroke-width="1.8"/>
  <rect x="20"  y="558" width="150" height="22" rx="5" fill="#8e44ad"/>
  <text x="95"  y="573" text-anchor="middle" font-size="11" font-weight="bold" fill="#fff">AuthService</text>
  <line x1="20" y1="580" x2="170" y2="580" stroke="#ddd"/>
  <text x="28"  y="594" font-size="10" fill="#555">- staffRepo: StaffRepository</text>
  <line x1="20" y1="600" x2="170" y2="600" stroke="#ddd"/>
  <text x="28"  y="613" font-size="10" fill="#2471a3">+ login(u,p): Staff</text>
  <text x="28"  y="625" font-size="10" fill="#2471a3">+ isValidRole(s,r): bool</text>

  <!-- CustomerService -->
  <rect x="185" y="558" width="165" height="90" rx="5" fill="#fff" stroke="#8e44ad" stroke-width="1.8"/>
  <rect x="185" y="558" width="165" height="22" rx="5" fill="#8e44ad"/>
  <text x="267" y="573" text-anchor="middle" font-size="11" font-weight="bold" fill="#fff">CustomerService</text>
  <line x1="185" y1="580" x2="350" y2="580" stroke="#ddd"/>
  <text x="193" y="594" font-size="10" fill="#555">- customerRepo</text>
  <line x1="185" y1="600" x2="350" y2="600" stroke="#ddd"/>
  <text x="193" y="614" font-size="10" fill="#2471a3">+ registerCustomer(...): Customer</text>
  <text x="193" y="627" font-size="10" fill="#2471a3">+ updateCustomer(c): bool</text>
  <text x="193" y="640" font-size="10" fill="#2471a3">+ getNewCustomersInMonth(...)</text>

  <!-- PolicyService -->
  <rect x="365" y="558" width="165" height="90" rx="5" fill="#fff" stroke="#8e44ad" stroke-width="1.8"/>
  <rect x="365" y="558" width="165" height="22" rx="5" fill="#8e44ad"/>
  <text x="447" y="573" text-anchor="middle" font-size="11" font-weight="bold" fill="#fff">PolicyService</text>
  <line x1="365" y1="580" x2="530" y2="580" stroke="#ddd"/>
  <text x="373" y="594" font-size="10" fill="#555">- policyRepo, customerRepo, vehRepo</text>
  <line x1="365" y1="600" x2="530" y2="600" stroke="#ddd"/>
  <text x="373" y="614" font-size="10" fill="#2471a3">+ issuePolicy(...): InsurancePolicy</text>
  <text x="373" y="627" font-size="10" fill="#2471a3">+ cancelPolicy(id): bool</text>
  <text x="373" y="640" font-size="10" fill="#2471a3">+ isPolicyActive(id): bool</text>

  <!-- ClaimService -->
  <rect x="545" y="558" width="155" height="90" rx="5" fill="#fff" stroke="#8e44ad" stroke-width="1.8"/>
  <rect x="545" y="558" width="155" height="22" rx="5" fill="#8e44ad"/>
  <text x="622" y="573" text-anchor="middle" font-size="11" font-weight="bold" fill="#fff">ClaimService</text>
  <line x1="545" y1="580" x2="700" y2="580" stroke="#ddd"/>
  <text x="553" y="594" font-size="10" fill="#555">- claimRepo, policyRepo</text>
  <line x1="545" y1="600" x2="700" y2="600" stroke="#ddd"/>
  <text x="553" y="614" font-size="10" fill="#2471a3">+ fileClaim(...): Claim</text>
  <text x="553" y="627" font-size="10" fill="#2471a3">+ approveClaim(id): bool</text>
  <text x="553" y="640" font-size="10" fill="#2471a3">+ getPendingClaims(): list</text>

  <!-- InspectionService -->
  <rect x="714" y="558" width="165" height="90" rx="5" fill="#fff" stroke="#8e44ad" stroke-width="1.8"/>
  <rect x="714" y="558" width="165" height="22" rx="5" fill="#8e44ad"/>
  <text x="796" y="573" text-anchor="middle" font-size="11" font-weight="bold" fill="#fff">InspectionService</text>
  <line x1="714" y1="580" x2="879" y2="580" stroke="#ddd"/>
  <text x="722" y="594" font-size="10" fill="#555">- inspRepo, claimRepo, wRepo</text>
  <line x1="714" y1="600" x2="879" y2="600" stroke="#ddd"/>
  <text x="722" y="614" font-size="10" fill="#2471a3">+ scheduleInspection(...):</text>
  <text x="722" y="627" font-size="10" fill="#2471a3">+ submitReport(...): bool</text>
  <text x="722" y="640" font-size="10" fill="#2471a3">+ getBySurveyor(id): list</text>

  <!-- ReportService -->
  <rect x="893" y="558" width="175" height="90" rx="5" fill="#fff" stroke="#8e44ad" stroke-width="1.8"/>
  <rect x="893" y="558" width="175" height="22" rx="5" fill="#8e44ad"/>
  <text x="980" y="573" text-anchor="middle" font-size="11" font-weight="bold" fill="#fff">ReportService</text>
  <line x1="893" y1="580" x2="1068" y2="580" stroke="#ddd"/>
  <text x="901" y="594" font-size="10" fill="#555">- (all repositories)</text>
  <line x1="893" y1="600" x2="1068" y2="600" stroke="#ddd"/>
  <text x="901" y="614" font-size="10" fill="#2471a3">+ newCustomersReport(ym)</text>
  <text x="901" y="627" font-size="10" fill="#2471a3">+ pendingClaimsReport()</text>
  <text x="901" y="640" font-size="10" fill="#2471a3">+ inspectionReport(claimId)</text>

  <!-- ══ REPOSITORY LAYER label ══ -->
  <rect x="10" y="680" width="150" height="22" rx="4" fill="#e67e22" opacity=".85"/>
  <text x="85" y="695" text-anchor="middle" font-size="11" fill="#fff" font-weight="bold">Repository Layer</text>

  <!-- Repos row -->
  <rect x="20"  y="708" width="148" height="52" rx="5" fill="#fff" stroke="#e67e22" stroke-width="1.8"/>
  <rect x="20"  y="708" width="148" height="20" rx="5" fill="#e67e22"/>
  <text x="94"  y="721" text-anchor="middle" font-size="10" font-weight="bold" fill="#fff">CustomerRepository</text>
  <text x="28"  y="736" font-size="9"  fill="#555">- filePath: string</text>
  <text x="28"  y="748" font-size="9"  fill="#2471a3">+ save/update/findAll/findById</text>

  <rect x="180" y="708" width="145" height="52" rx="5" fill="#fff" stroke="#e67e22" stroke-width="1.8"/>
  <rect x="180" y="708" width="145" height="20" rx="5" fill="#e67e22"/>
  <text x="252" y="721" text-anchor="middle" font-size="10" font-weight="bold" fill="#fff">VehicleRepository</text>
  <text x="188" y="736" font-size="9"  fill="#555">- filePath: string</text>
  <text x="188" y="748" font-size="9"  fill="#2471a3">+ save/update/findByCustomer</text>

  <rect x="338" y="708" width="145" height="52" rx="5" fill="#fff" stroke="#e67e22" stroke-width="1.8"/>
  <rect x="338" y="708" width="145" height="20" rx="5" fill="#e67e22"/>
  <text x="410" y="721" text-anchor="middle" font-size="10" font-weight="bold" fill="#fff">PolicyRepository</text>
  <text x="346" y="736" font-size="9"  fill="#555">- filePath: string</text>
  <text x="346" y="748" font-size="9"  fill="#2471a3">+ save/update/findByCustomer</text>

  <rect x="496" y="708" width="140" height="52" rx="5" fill="#fff" stroke="#e67e22" stroke-width="1.8"/>
  <rect x="496" y="708" width="140" height="20" rx="5" fill="#e67e22"/>
  <text x="566" y="721" text-anchor="middle" font-size="10" font-weight="bold" fill="#fff">ClaimRepository</text>
  <text x="504" y="736" font-size="9"  fill="#555">- filePath: string</text>
  <text x="504" y="748" font-size="9"  fill="#2471a3">+ save/update/findByStatus</text>

  <rect x="649" y="708" width="155" height="52" rx="5" fill="#fff" stroke="#e67e22" stroke-width="1.8"/>
  <rect x="649" y="708" width="155" height="20" rx="5" fill="#e67e22"/>
  <text x="726" y="721" text-anchor="middle" font-size="10" font-weight="bold" fill="#fff">InspectionRepository</text>
  <text x="657" y="736" font-size="9"  fill="#555">- filePath: string</text>
  <text x="657" y="748" font-size="9"  fill="#2471a3">+ save/update/findByClaim</text>

  <rect x="817" y="708" width="140" height="52" rx="5" fill="#fff" stroke="#e67e22" stroke-width="1.8"/>
  <rect x="817" y="708" width="140" height="20" rx="5" fill="#e67e22"/>
  <text x="887" y="721" text-anchor="middle" font-size="10" font-weight="bold" fill="#fff">RepairRepository</text>
  <text x="825" y="736" font-size="9"  fill="#555">- filePath: string</text>
  <text x="825" y="748" font-size="9"  fill="#2471a3">+ save/update/findByClaim</text>

  <rect x="969" y="708" width="145" height="52" rx="5" fill="#fff" stroke="#e67e22" stroke-width="1.8"/>
  <rect x="969" y="708" width="145" height="20" rx="5" fill="#e67e22"/>
  <text x="1041" y="721" text-anchor="middle" font-size="10" font-weight="bold" fill="#fff">StaffRepository</text>
  <text x="977" y="736" font-size="9"  fill="#555">- filePath: string</text>
  <text x="977" y="748" font-size="9"  fill="#2471a3">+ findByUsername/findByRole</text>

  <!-- ══ UI LAYER ══ -->
  <rect x="10" y="795" width="110" height="22" rx="4" fill="#c0392b" opacity=".85"/>
  <text x="65" y="810" text-anchor="middle" font-size="11" fill="#fff" font-weight="bold">UI Layer</text>

  <rect x="20"  y="822" width="120" height="62" rx="5" fill="#fff" stroke="#c0392b" stroke-width="1.8"/>
  <rect x="20"  y="822" width="120" height="20" rx="5" fill="#c0392b"/>
  <text x="80"  y="836" text-anchor="middle" font-size="10" font-weight="bold" fill="#fff">ConsoleUI</text>
  <text x="28"  y="852" font-size="9"  fill="#2471a3">+ printHeader(title)</text>
  <text x="28"  y="863" font-size="9"  fill="#2471a3">+ readInt/readString()</text>
  <text x="28"  y="874" font-size="9"  fill="#2471a3">+ pause(), clearScreen()</text>

  <rect x="158" y="822" width="200" height="75" rx="5" fill="#fff" stroke="#c0392b" stroke-width="1.8"/>
  <rect x="158" y="822" width="200" height="20" rx="5" fill="#c0392b"/>
  <text x="258" y="836" text-anchor="middle" font-size="10" font-weight="bold" fill="#fff">Application</text>
  <text x="166" y="852" font-size="9"  fill="#555">- (all repos, services, currentUser)</text>
  <line x1="158" y1="856" x2="358" y2="856" stroke="#ddd"/>
  <text x="166" y="869" font-size="9"  fill="#2471a3">+ run()</text>
  <text x="166" y="880" font-size="9"  fill="#2471a3">+ loginScreen(): bool</text>
  <text x="166" y="891" font-size="9"  fill="#2471a3">+ managerMenu/salesmanMenu/...</text>

  <!-- ══ DATA FILES box ══ -->
  <rect x="1080" y="700" width="290" height="145" rx="8" fill="#ecf0f1" stroke="#bdc3c7" stroke-width="1.5" stroke-dasharray="6,3"/>
  <text x="1225" y="718" text-anchor="middle" font-size="11" font-weight="bold" fill="#555">Flat File Storage (data/)</text>
  <text x="1088" y="736" font-size="10" fill="#777">📄 customers.dat</text>
  <text x="1088" y="750" font-size="10" fill="#777">📄 vehicles.dat</text>
  <text x="1088" y="764" font-size="10" fill="#777">📄 staff.dat</text>
  <text x="1088" y="778" font-size="10" fill="#777">📄 workshops.dat</text>
  <text x="1200" y="736" font-size="10" fill="#777">📄 policies.dat</text>
  <text x="1200" y="750" font-size="10" fill="#777">📄 claims.dat</text>
  <text x="1200" y="764" font-size="10" fill="#777">📄 inspections.dat</text>
  <text x="1200" y="778" font-size="10" fill="#777">📄 repairs.dat</text>
  <text x="1088" y="800" font-size="9"  fill="#999">Format: pipe-delimited (|) text</text>
  <text x="1088" y="814" font-size="9"  fill="#999">Each line = one serialized record</text>
  <text x="1088" y="828" font-size="9"  fill="#999">No DBMS – file I/O only</text>

  <!-- Arrows: Entity to Repo -->
  <line x1="94"  y1="760" x2="94"  y2="794" stroke="#e67e22" stroke-width="1.2" marker-end="url(#arr)" stroke-dasharray="4,2"/>
  <line x1="252" y1="760" x2="252" y2="794" stroke="#e67e22" stroke-width="1.2" marker-end="url(#arr)" stroke-dasharray="4,2"/>
  <line x1="410" y1="760" x2="410" y2="794" stroke="#e67e22" stroke-width="1.2" marker-end="url(#arr)" stroke-dasharray="4,2"/>
  <line x1="566" y1="760" x2="566" y2="794" stroke="#e67e22" stroke-width="1.2" marker-end="url(#arr)" stroke-dasharray="4,2"/>

  <!-- Arrow: Repo to Files -->
  <line x1="1041" y1="760" x2="1078" y2="760" stroke="#bdc3c7" stroke-width="1.5" marker-end="url(#arr)" stroke-dasharray="5,3"/>

  <!-- Arrow: App uses Services -->
  <line x1="258" y1="897" x2="258" y2="650" stroke="#c0392b" stroke-width="1" stroke-dasharray="4,3" marker-end="url(#arr)"/>

  <!-- Legend -->
  <rect x="1090" y="868" width="280" height="90" rx="6" fill="#fff" stroke="#ddd"/>
  <text x="1098" y="884" font-size="10" font-weight="bold" fill="#333">Legend</text>
  <rect x="1098" y="890" width="12" height="12" fill="#1a3c5e"/>
  <text x="1115" y="900" font-size="9" fill="#333">Entity (domain object)</text>
  <rect x="1098" y="906" width="12" height="12" fill="#8e44ad"/>
  <text x="1115" y="916" font-size="9" fill="#333">Service (business logic)</text>
  <rect x="1098" y="922" width="12" height="12" fill="#e67e22"/>
  <text x="1115" y="932" font-size="9" fill="#333">Repository (persistence)</text>
  <rect x="1098" y="938" width="12" height="12" fill="#c0392b"/>
  <text x="1115" y="948" font-size="9" fill="#333">UI Layer</text>
  <rect x="1200" y="890" width="12" height="12" fill="#27ae60"/>
  <text x="1217" y="900" font-size="9" fill="#333">Transaction Entity</text>
</svg>
'''

with open("docs/class_diagram.svg", "w") as f:
    f.write(SVG)
print("Class diagram written: docs/class_diagram.svg")
