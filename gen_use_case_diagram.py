#!/usr/bin/env python3

SVG = '''<?xml version="1.0" encoding="UTF-8"?>
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 1280 860" width="1280" height="860"
     font-family="Arial, sans-serif">

  <!-- Background -->
  <rect width="1280" height="860" fill="#f4f6f9"/>
  <text x="640" y="32" text-anchor="middle" font-size="20" font-weight="bold" fill="#1a3c5e">
    AutoShield Insurance System – Use Case Diagram
  </text>

  <!-- ════ SYSTEM BOUNDARY ════ -->
  <rect x="260" y="50" width="760" height="780" rx="12" fill="none" stroke="#1a3c5e" stroke-width="2.5" stroke-dasharray="10,4"/>
  <text x="640" y="70" text-anchor="middle" font-size="13" font-weight="bold" fill="#1a3c5e">
    Auto Insurance Management System
  </text>

  <!-- ─── Actor helper: ellipse head + body ─── -->
  <!-- Manager Actor (left side) -->
  <g id="actor_mgr" transform="translate(60,330)">
    <circle cx="30" cy="20" r="16" fill="#1a3c5e" stroke="#1a3c5e" stroke-width="1.5"/>
    <text x="30" y="25" text-anchor="middle" font-size="11" fill="#fff" font-weight="bold">M</text>
    <line x1="30" y1="36" x2="30" y2="72" stroke="#1a3c5e" stroke-width="2"/>
    <line x1="8"  y1="50" x2="52" y2="50" stroke="#1a3c5e" stroke-width="2"/>
    <line x1="30" y1="72" x2="10" y2="96" stroke="#1a3c5e" stroke-width="2"/>
    <line x1="30" y1="72" x2="50" y2="96" stroke="#1a3c5e" stroke-width="2"/>
    <text x="30" y="112" text-anchor="middle" font-size="12" font-weight="bold" fill="#1a3c5e">Manager</text>
  </g>

  <!-- Salesman Actor (left side, lower) -->
  <g id="actor_sal" transform="translate(60,510)">
    <circle cx="30" cy="20" r="16" fill="#27ae60" stroke="#27ae60" stroke-width="1.5"/>
    <text x="30" y="25" text-anchor="middle" font-size="11" fill="#fff" font-weight="bold">S</text>
    <line x1="30" y1="36" x2="30" y2="72" stroke="#27ae60" stroke-width="2"/>
    <line x1="8"  y1="50" x2="52" y2="50" stroke="#27ae60" stroke-width="2"/>
    <line x1="30" y1="72" x2="10" y2="96" stroke="#27ae60" stroke-width="2"/>
    <line x1="30" y1="72" x2="50" y2="96" stroke="#27ae60" stroke-width="2"/>
    <text x="30" y="112" text-anchor="middle" font-size="12" font-weight="bold" fill="#27ae60">Salesman</text>
  </g>

  <!-- Surveyor Actor (left side, bottom) -->
  <g id="actor_surv" transform="translate(60,680)">
    <circle cx="30" cy="20" r="16" fill="#8e44ad" stroke="#8e44ad" stroke-width="1.5"/>
    <text x="30" y="25" text-anchor="middle" font-size="11" fill="#fff" font-weight="bold">V</text>
    <line x1="30" y1="36" x2="30" y2="72" stroke="#8e44ad" stroke-width="2"/>
    <line x1="8"  y1="50" x2="52" y2="50" stroke="#8e44ad" stroke-width="2"/>
    <line x1="30" y1="72" x2="10" y2="96" stroke="#8e44ad" stroke-width="2"/>
    <line x1="30" y1="72" x2="50" y2="96" stroke="#8e44ad" stroke-width="2"/>
    <text x="30" y="112" text-anchor="middle" font-size="12" font-weight="bold" fill="#8e44ad">Surveyor</text>
  </g>

  <!-- Customer Actor (right side) -->
  <g id="actor_cust" transform="translate(1160,400)">
    <circle cx="30" cy="20" r="16" fill="#e67e22" stroke="#e67e22" stroke-width="1.5"/>
    <text x="30" y="25" text-anchor="middle" font-size="11" fill="#fff" font-weight="bold">C</text>
    <line x1="30" y1="36" x2="30" y2="72" stroke="#e67e22" stroke-width="2"/>
    <line x1="8"  y1="50" x2="52" y2="50" stroke="#e67e22" stroke-width="2"/>
    <line x1="30" y1="72" x2="10" y2="96" stroke="#e67e22" stroke-width="2"/>
    <line x1="30" y1="72" x2="50" y2="96" stroke="#e67e22" stroke-width="2"/>
    <text x="30" y="112" text-anchor="middle" font-size="12" font-weight="bold" fill="#e67e22">Customer</text>
  </g>

  <!-- ─── USE CASE ellipses ─── -->
  <!-- helper macro: ellipses with labels -->
  <!-- col 1: ~350, col 2: ~560, col 3: ~760 -->

  <!-- UC01 Login -->
  <ellipse cx="640" cy="110" rx="90" ry="22" fill="#fff" stroke="#1a3c5e" stroke-width="1.5"/>
  <text x="640" y="115" text-anchor="middle" font-size="11" fill="#1a3c5e" font-weight="bold">UC01: Login</text>

  <!-- UC02 Register Customer -->
  <ellipse cx="420" cy="175" rx="110" ry="22" fill="#d5f5e3" stroke="#27ae60" stroke-width="1.5"/>
  <text x="420" y="180" text-anchor="middle" font-size="10" fill="#1a5e2a">UC02: Register Customer</text>

  <!-- UC03 Update Customer -->
  <ellipse cx="420" cy="230" rx="110" ry="22" fill="#d5f5e3" stroke="#27ae60" stroke-width="1.5"/>
  <text x="420" y="235" text-anchor="middle" font-size="10" fill="#1a5e2a">UC03: Update Customer</text>

  <!-- UC04 Register Vehicle -->
  <ellipse cx="420" cy="290" rx="110" ry="22" fill="#d5f5e3" stroke="#27ae60" stroke-width="1.5"/>
  <text x="420" y="295" text-anchor="middle" font-size="10" fill="#1a5e2a">UC04: Register Vehicle</text>

  <!-- UC05 Issue Policy -->
  <ellipse cx="420" cy="350" rx="110" ry="22" fill="#d5f5e3" stroke="#27ae60" stroke-width="1.5"/>
  <text x="420" y="355" text-anchor="middle" font-size="10" fill="#1a5e2a">UC05: Issue Policy</text>

  <!-- UC06 File Claim -->
  <ellipse cx="420" cy="415" rx="110" ry="22" fill="#fde8d8" stroke="#e67e22" stroke-width="1.5"/>
  <text x="420" y="420" text-anchor="middle" font-size="10" fill="#7e3500">UC06: File Claim</text>

  <!-- UC07 View Own Policies -->
  <ellipse cx="860" cy="200" rx="115" ry="22" fill="#fde8d8" stroke="#e67e22" stroke-width="1.5"/>
  <text x="860" y="205" text-anchor="middle" font-size="10" fill="#7e3500">UC07: View Own Policies</text>

  <!-- UC08 View Claim Status -->
  <ellipse cx="860" cy="260" rx="115" ry="22" fill="#fde8d8" stroke="#e67e22" stroke-width="1.5"/>
  <text x="860" y="265" text-anchor="middle" font-size="10" fill="#7e3500">UC08: View Claim Status</text>

  <!-- UC09 Schedule Inspection -->
  <ellipse cx="640" cy="490" rx="125" ry="22" fill="#f9ebff" stroke="#8e44ad" stroke-width="1.5"/>
  <text x="640" y="495" text-anchor="middle" font-size="10" fill="#5b2c6f">UC09: Schedule Inspection</text>

  <!-- UC10 Submit Inspection Report -->
  <ellipse cx="640" cy="550" rx="140" ry="22" fill="#f9ebff" stroke="#8e44ad" stroke-width="1.5"/>
  <text x="640" y="555" text-anchor="middle" font-size="10" fill="#5b2c6f">UC10: Submit Inspection Report</text>

  <!-- UC11 Approve/Reject Claim -->
  <ellipse cx="640" cy="335" rx="130" ry="22" fill="#eaf4fb" stroke="#1a3c5e" stroke-width="1.5"/>
  <text x="640" y="340" text-anchor="middle" font-size="10" fill="#1a3c5e">UC11: Approve / Reject Claim</text>

  <!-- UC12 Assign Repair -->
  <ellipse cx="640" cy="395" rx="130" ry="22" fill="#eaf4fb" stroke="#1a3c5e" stroke-width="1.5"/>
  <text x="640" y="400" text-anchor="middle" font-size="10" fill="#1a3c5e">UC12: Assign Repair to Workshop</text>

  <!-- UC13 Complete Repair -->
  <ellipse cx="640" cy="455" rx="130" ry="22" fill="#eaf4fb" stroke="#1a3c5e" stroke-width="1.5"/>
  <text x="640" y="460" text-anchor="middle" font-size="10" fill="#1a3c5e">UC13: Mark Repair Complete</text>

  <!-- UC14 Cancel Policy -->
  <ellipse cx="640" cy="175" rx="125" ry="22" fill="#eaf4fb" stroke="#1a3c5e" stroke-width="1.5"/>
  <text x="640" y="180" text-anchor="middle" font-size="10" fill="#1a3c5e">UC14: Cancel Policy</text>

  <!-- UC15 Add/Remove Workshop -->
  <ellipse cx="640" cy="230" rx="130" ry="22" fill="#eaf4fb" stroke="#1a3c5e" stroke-width="1.5"/>
  <text x="640" y="235" text-anchor="middle" font-size="10" fill="#1a3c5e">UC15: Manage Workshops</text>

  <!-- UC16 Manage Staff -->
  <ellipse cx="640" cy="285" rx="130" ry="22" fill="#eaf4fb" stroke="#1a3c5e" stroke-width="1.5"/>
  <text x="640" y="290" text-anchor="middle" font-size="10" fill="#1a3c5e">UC16: Manage Staff</text>

  <!-- UC17 Generate Reports -->
  <ellipse cx="640" cy="615" rx="130" ry="22" fill="#eaf4fb" stroke="#1a3c5e" stroke-width="1.5"/>
  <text x="640" y="620" text-anchor="middle" font-size="10" fill="#1a3c5e">UC17: Generate Reports</text>

  <!-- UC18 View Inspection Report -->
  <ellipse cx="860" cy="330" rx="125" ry="22" fill="#eaf4fb" stroke="#1a3c5e" stroke-width="1.5"/>
  <text x="860" y="335" text-anchor="middle" font-size="10" fill="#1a3c5e">UC18: View Inspection Report</text>

  <!-- ─── ASSOCIATIONS (actor → use case lines) ─── -->

  <!-- Manager → UC01 Login -->
  <line x1="120" y1="350" x2="550" y2="120" stroke="#1a3c5e" stroke-width="1.2"/>
  <!-- Manager → UC14 Cancel Policy -->
  <line x1="120" y1="380" x2="515" y2="200" stroke="#1a3c5e" stroke-width="1.2"/>
  <!-- Manager → UC15 Manage Workshops -->
  <line x1="120" y1="390" x2="510" y2="246" stroke="#1a3c5e" stroke-width="1.2"/>
  <!-- Manager → UC16 Manage Staff -->
  <line x1="120" y1="400" x2="510" y2="285" stroke="#1a3c5e" stroke-width="1.2"/>
  <!-- Manager → UC11 Approve/Reject Claim -->
  <line x1="120" y1="410" x2="510" y2="340" stroke="#1a3c5e" stroke-width="1.2"/>
  <!-- Manager → UC12 Assign Repair -->
  <line x1="120" y1="420" x2="510" y2="400" stroke="#1a3c5e" stroke-width="1.2"/>
  <!-- Manager → UC13 Complete Repair -->
  <line x1="120" y1="430" x2="510" y2="460" stroke="#1a3c5e" stroke-width="1.2"/>
  <!-- Manager → UC17 Reports -->
  <line x1="120" y1="445" x2="510" y2="615" stroke="#1a3c5e" stroke-width="1.2"/>
  <!-- Manager → UC18 View Inspection -->
  <line x1="120" y1="435" x2="735" y2="340" stroke="#1a3c5e" stroke-width="1.2"/>

  <!-- Salesman → UC01 Login -->
  <line x1="120" y1="530" x2="550" y2="125" stroke="#27ae60" stroke-width="1.2"/>
  <!-- Salesman → UC02 Register Customer -->
  <line x1="120" y1="552" x2="310" y2="188" stroke="#27ae60" stroke-width="1.2"/>
  <!-- Salesman → UC03 Update Customer -->
  <line x1="120" y1="556" x2="310" y2="242" stroke="#27ae60" stroke-width="1.2"/>
  <!-- Salesman → UC04 Register Vehicle -->
  <line x1="120" y1="562" x2="310" y2="297" stroke="#27ae60" stroke-width="1.2"/>
  <!-- Salesman → UC05 Issue Policy -->
  <line x1="120" y1="568" x2="310" y2="358" stroke="#27ae60" stroke-width="1.2"/>
  <!-- Salesman → UC06 File Claim -->
  <line x1="120" y1="575" x2="310" y2="420" stroke="#27ae60" stroke-width="1.2"/>

  <!-- Surveyor → UC01 Login -->
  <line x1="120" y1="700" x2="553" y2="130" stroke="#8e44ad" stroke-width="1.2"/>
  <!-- Surveyor → UC09 Schedule Inspection -->
  <line x1="120" y1="722" x2="515" y2="497" stroke="#8e44ad" stroke-width="1.2"/>
  <!-- Surveyor → UC10 Submit Report -->
  <line x1="120" y1="728" x2="500" y2="555" stroke="#8e44ad" stroke-width="1.2"/>

  <!-- Customer → UC07 View Policies -->
  <line x1="1165" y1="440" x2="975" y2="215" stroke="#e67e22" stroke-width="1.2"/>
  <!-- Customer → UC08 View Claim Status -->
  <line x1="1165" y1="450" x2="975" y2="268" stroke="#e67e22" stroke-width="1.2"/>

  <!-- «include» / «extend» relations -->
  <!-- UC06 File Claim <<include>> UC05 Issue Policy check -->
  <line x1="420" y1="393" x2="420" y2="372" stroke="#777" stroke-width="1" stroke-dasharray="5,3" marker-end="url(#arrd)"/>
  <text x="435" y="385" font-size="8" fill="#777">«include»</text>

  <!-- UC11 <<include>> UC10 Inspect Report -->
  <line x1="640" y1="313" x2="640" y2="533" stroke="#777" stroke-width="1" stroke-dasharray="5,3" marker-end="url(#arrd)"/>
  <text x="646" y="430" font-size="8" fill="#777">«include»</text>

  <defs>
    <marker id="arrd" markerWidth="10" markerHeight="7" refX="9" refY="3.5" orient="auto">
      <polygon points="0 0,10 3.5,0 7" fill="#777"/>
    </marker>
  </defs>

  <!-- ─── LEGEND ─── -->
  <rect x="268" y="668" width="680" height="130" rx="8" fill="#fff" stroke="#ddd" stroke-width="1"/>
  <text x="608" y="686" text-anchor="middle" font-size="11" font-weight="bold" fill="#333">Use Case Legend</text>
  <!-- row 1 -->
  <ellipse cx="310" cy="705" rx="40" ry="12" fill="#d5f5e3" stroke="#27ae60" stroke-width="1.2"/>
  <text x="310" y="709" text-anchor="middle" font-size="8" fill="#1a5e2a">Salesman UC</text>
  <ellipse cx="430" cy="705" rx="40" ry="12" fill="#eaf4fb" stroke="#1a3c5e" stroke-width="1.2"/>
  <text x="430" y="709" text-anchor="middle" font-size="8" fill="#1a3c5e">Manager UC</text>
  <ellipse cx="550" cy="705" rx="40" ry="12" fill="#f9ebff" stroke="#8e44ad" stroke-width="1.2"/>
  <text x="550" y="709" text-anchor="middle" font-size="8" fill="#5b2c6f">Surveyor UC</text>
  <ellipse cx="670" cy="705" rx="40" ry="12" fill="#fde8d8" stroke="#e67e22" stroke-width="1.2"/>
  <text x="670" y="709" text-anchor="middle" font-size="8" fill="#7e3500">Customer UC</text>
  <!-- row 2: key rules -->
  <text x="285" y="730" font-size="10" fill="#555">🔑 All actors must Login (UC01) before any action.</text>
  <text x="285" y="748" font-size="10" fill="#555">🔒 Claim approval (UC11) requires a completed Inspection Report (UC10) — «include».</text>
  <text x="285" y="766" font-size="10" fill="#555">🏭 Inspections and Repairs are only assigned to Registered Workshops (enforced at service layer).</text>
  <text x="285" y="784" font-size="10" fill="#555">📋 Manager views: new customers/month, pending claims, inspection reports, customer claim history.</text>

</svg>
'''

with open("docs/use_case_diagram.svg", "w") as f:
    f.write(SVG)
print("Use case diagram written: docs/use_case_diagram.svg")
