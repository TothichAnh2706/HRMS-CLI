#include "PayrollManager.h"
#include "EmployeeManager.h"
#include "AttendanceManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <algorithm>

PayrollManager::PayrollManager(EmployeeManager* empMgr, AttendanceManager* attMgr)
    : employeeManager(empMgr), attendanceManager(attMgr),
      contractFile("data/contracts.csv"), rdFile("data/reward_discipline.csv") {}

// ===================== FILE I/O =====================
void PayrollManager::loadContracts() {
    contracts.clear();
    std::ifstream f(contractFile);
    if (!f.is_open()) return;
    std::string line;
    std::getline(f, line);
    while (std::getline(f, line))
        if (!line.empty()) contracts.push_back(Contract::fromCSV(line));
}

void PayrollManager::saveContracts() {
    std::ofstream f(contractFile);
    f << "ContractID,EmployeeID,ContractType,SignDate,ExpiredDate,BaseSalary,IsActive\n";
    for (const auto& c : contracts) f << c.toCSV() << "\n";
}

void PayrollManager::loadRDRecords() {
    rdRecords.clear();
    std::ifstream f(rdFile);
    if (!f.is_open()) return;
    std::string line;
    std::getline(f, line);
    while (std::getline(f, line))
        if (!line.empty()) rdRecords.push_back(RewardDiscipline::fromCSV(line));
}

void PayrollManager::saveRDRecords() {
    std::ofstream f(rdFile);
    f << "ID,EmployeeID,Type,Amount,Reason,Date\n";
    for (const auto& r : rdRecords) f << r.toCSV() << "\n";
}

void PayrollManager::loadPayroll() {
    salaryRecords.clear();
    std::ifstream f("data/payroll.csv");
    if (!f.is_open()) return;
    std::string line;
    std::getline(f, line);
    while (std::getline(f, line))
        if (!line.empty()) salaryRecords.push_back(SalaryRecord::fromCSV(line));
}

void PayrollManager::savePayroll(int month, int year) {
    std::ostringstream fname;
    fname << "data/payroll_" << std::setfill('0') << std::setw(2) << month << "_" << year << ".csv";
    std::ofstream f(fname.str());
    f << "RecordID,EmployeeID,Month,Year,WorkDays,BaseSalary,Allowance,Bonus,Deduction,Tax,NetSalary,IsDisputed\n";
    for (const auto& r : salaryRecords)
        if (r.getMonth() == month && r.getYear() == year) f << r.toCSV() << "\n";
    // Also update master payroll file
    std::ofstream f2("data/payroll.csv");
    f2 << "RecordID,EmployeeID,Month,Year,WorkDays,BaseSalary,Allowance,Bonus,Deduction,Tax,NetSalary,IsDisputed\n";
    for (const auto& r : salaryRecords) f2 << r.toCSV() << "\n";
}

void PayrollManager::loadFromFile() {
    loadContracts();
    loadRDRecords();
    loadPayroll();
}

void PayrollManager::saveToFile() {
    saveContracts();
    saveRDRecords();
}

// ===================== SALARY CALCULATION =====================
SalaryRecord PayrollManager::calculateSalary(const std::string& empId, int month, int year) {
    // Get base salary from active contract
    Contract* contract = getActiveContractByEmployee(empId);
    double baseSalary = contract ? contract->getBaseSalary() : 0.0;

    // Get work days from attendance
    int workDays = attendanceManager ? attendanceManager->getWorkingDays(empId, month, year) : 22;

    // Get allowance from position (simplified: 500000 per unit coefficient)
    double allowance = 500000.0;
    if (employeeManager) {
        Employee* emp = employeeManager->getEmployeeById(empId);
        if (emp) {
            Position* pos = employeeManager->getPositionById(emp->getPositionId());
            if (pos) allowance = pos->getAllowanceCoeff() * 500000.0;
        }
    }

    // Get bonus/penalty for the month
    double bonus   = getTotalBonusForMonth(empId, month, year);
    double penalty = getTotalPenaltyForMonth(empId, month, year);

    // Generate record ID
    std::ostringstream oss;
    oss << "PAY" << std::setfill('0') << std::setw(3) << (salaryRecords.size() + 1);

    SalaryRecord rec(oss.str(), empId, month, year, workDays,
                     baseSalary, allowance, bonus, penalty);
    return rec;
}

void PayrollManager::runPayroll(int month, int year) {
    if (!employeeManager) { std::cout << "  Loi: EmployeeManager chua khoi tao!\n"; return; }
    // Remove existing records for this month/year
    salaryRecords.erase(
        std::remove_if(salaryRecords.begin(), salaryRecords.end(),
            [month, year](const SalaryRecord& r){ return r.getMonth()==month && r.getYear()==year; }),
        salaryRecords.end());

    int count = 0;
    for (const auto& emp : employeeManager->getAllEmployees()) {
        if (emp.getStatus() != "Active") continue;
        SalaryRecord rec = calculateSalary(emp.getEmployeeId(), month, year);
        salaryRecords.push_back(rec);
        count++;
    }
    savePayroll(month, year);
    std::cout << "  Da tinh luong cho " << count << " nhan vien thang " << month << "/" << year << "\n";
}

bool PayrollManager::recalculateSalary(const std::string& recordId) {
    for (auto& r : salaryRecords) {
        if (r.getRecordId() == recordId) {
            r.setIsDisputed(false);
            r.calculate();
            saveToFile();
            return true;
        }
    }
    return false;
}

// ===================== CONTRACT MANAGEMENT =====================
bool PayrollManager::addContract(const Contract& c) {
    // Deactivate old contracts for the employee
    for (auto& existing : contracts)
        if (existing.getEmployeeId() == c.getEmployeeId()) existing.setIsActive(false);
    contracts.push_back(c);
    saveContracts();
    return true;
}

bool PayrollManager::updateContract(const std::string& cId, const Contract& updated) {
    for (auto& c : contracts) {
        if (c.getContractId() == cId) { c = updated; saveContracts(); return true; }
    }
    return false;
}

Contract* PayrollManager::getActiveContractByEmployee(const std::string& empId) {
    for (auto& c : contracts)
        if (c.getEmployeeId() == empId && c.getIsActive()) return &c;
    return nullptr;
}

std::vector<Contract> PayrollManager::getContractsByEmployee(const std::string& empId) const {
    std::vector<Contract> res;
    for (const auto& c : contracts)
        if (c.getEmployeeId() == empId) res.push_back(c);
    return res;
}

// ===================== REWARD/DISCIPLINE =====================
bool PayrollManager::addRDRecord(const RewardDiscipline& rd) {
    rdRecords.push_back(rd);
    saveRDRecords();
    return true;
}

double PayrollManager::getTotalBonusForMonth(const std::string& empId, int month, int year) const {
    double total = 0;
    for (const auto& r : rdRecords)
        if (r.getEmployeeId() == empId && r.getRdType() == RDType::Reward
            && r.getDate().month == month && r.getDate().year == year)
            total += r.getAmount();
    return total;
}

double PayrollManager::getTotalPenaltyForMonth(const std::string& empId, int month, int year) const {
    double total = 0;
    for (const auto& r : rdRecords)
        if (r.getEmployeeId() == empId && r.getRdType() == RDType::Discipline
            && r.getDate().month == month && r.getDate().year == year)
            total += r.getAmount();
    return total;
}

// ===================== DISPUTE =====================
bool PayrollManager::flagDispute(const std::string& recordId) {
    for (auto& r : salaryRecords)
        if (r.getRecordId() == recordId) { r.setIsDisputed(true); saveToFile(); return true; }
    return false;
}

std::vector<SalaryRecord> PayrollManager::getDisputedRecords() const {
    std::vector<SalaryRecord> res;
    for (const auto& r : salaryRecords)
        if (r.getIsDisputed()) res.push_back(r);
    return res;
}

// ===================== API =====================
double PayrollManager::getTotalPayrollByMonth(int month, int year) const {
    double total = 0;
    for (const auto& r : salaryRecords)
        if (r.getMonth() == month && r.getYear() == year) total += r.getNetSalary();
    return total;
}

SalaryRecord* PayrollManager::getSalaryRecord(const std::string& empId, int month, int year) {
    for (auto& r : salaryRecords)
        if (r.getEmployeeId() == empId && r.getMonth() == month && r.getYear() == year)
            return &r;
    return nullptr;
}

// ===================== MENUS =====================
void PayrollManager::menuManageContracts() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n===== QUAN LY HOP DONG =====\n"
                  << "  1. Xem hop dong cua NV\n"
                  << "  2. Them hop dong moi\n"
                  << "  3. Them khen thuong/ky luat\n"
                  << "  4. Xem danh sach KT/KL\n"
                  << "  0. Quay lai\n"
                  << "Chon: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice == 1) {
            std::string empId;
            std::cout << "  Ma NV: "; std::getline(std::cin, empId);
            auto cs = getContractsByEmployee(empId);
            if (cs.empty()) { std::cout << "  Khong co hop dong!\n"; continue; }
            for (const auto& c : cs) c.displayInfo(), std::cout << std::string(40,'-') << "\n";
        } else if (choice == 2) {
            std::string cId, empId, type, sign, exp, salary;
            std::cout << "  Ma HĐ: "; std::getline(std::cin, cId);
            std::cout << "  Ma NV: "; std::getline(std::cin, empId);
            std::cout << "  Loai (Official/Probation/PartTime/Internship): "; std::getline(std::cin, type);
            std::cout << "  Ngay ky (DD/MM/YYYY): "; std::getline(std::cin, sign);
            std::cout << "  Ngay het han (DD/MM/YYYY): "; std::getline(std::cin, exp);
            std::cout << "  Luong co ban (VND): "; std::getline(std::cin, salary);
            Contract c(cId, empId, Contract::stringToContractType(type),
                       Date::fromString(sign), Date::fromString(exp),
                       salary.empty() ? 0 : std::stod(salary));
            addContract(c);
            std::cout << "  Da them hop dong!\n";
        } else if (choice == 3) {
            std::string rdId, empId, type, amount, reason, date;
            std::cout << "  Ma KT/KL: "; std::getline(std::cin, rdId);
            std::cout << "  Ma NV: "; std::getline(std::cin, empId);
            std::cout << "  Loai (Reward/Discipline): "; std::getline(std::cin, type);
            std::cout << "  So tien: "; std::getline(std::cin, amount);
            std::cout << "  Ly do: "; std::getline(std::cin, reason);
            std::cout << "  Ngay (DD/MM/YYYY): "; std::getline(std::cin, date);
            addRDRecord(RewardDiscipline(rdId, empId, RewardDiscipline::stringToRDType(type),
                                        amount.empty() ? 0 : std::stod(amount),
                                        reason, Date::fromString(date)));
            std::cout << "  Da them!\n";
        } else if (choice == 4) {
            for (const auto& r : rdRecords) r.displayInfo();
        }
    }
}

void PayrollManager::menuManagePayroll() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n===== QUAN LY BANG LUONG =====\n"
                  << "  1. Tinh luong thang\n"
                  << "  2. Xem phieu luong ca nhan\n"
                  << "  3. Khieu nai bang luong\n"
                  << "  4. Tong quy luong thang\n"
                  << "  0. Quay lai\n"
                  << "Chon: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice == 1) {
            int m, y;
            std::cout << "  Thang: "; std::cin >> m;
            std::cout << "  Nam: ";   std::cin >> y;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            runPayroll(m, y);
        } else if (choice == 2) {
            std::string empId; int m, y;
            std::cout << "  Ma NV: "; std::getline(std::cin, empId);
            std::cout << "  Thang: "; std::cin >> m;
            std::cout << "  Nam: ";   std::cin >> y;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            SalaryRecord* rec = getSalaryRecord(empId, m, y);
            if (rec) rec->printPaySlip();
            else std::cout << "  Chua co phieu luong!\n";
        } else if (choice == 3) {
            std::string recId;
            std::cout << "  ID phieu luong khieu nai: "; std::getline(std::cin, recId);
            if (flagDispute(recId)) std::cout << "  Da danh dau khieu nai!\n";
            else std::cout << "  Khong tim thay phieu luong!\n";
        } else if (choice == 4) {
            int m, y;
            std::cout << "  Thang: "; std::cin >> m;
            std::cout << "  Nam: ";   std::cin >> y;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            double total = getTotalPayrollByMonth(m, y);
            std::cout << std::fixed << std::setprecision(0)
                      << "  Tong quy luong thang " << m << "/" << y
                      << ": " << total << " VND\n";
        }
    }
}

void PayrollManager::menuHandleDisputes() {
    auto disputed = getDisputedRecords();
    if (disputed.empty()) { std::cout << "  Khong co phieu luong khieu nai.\n"; return; }
    std::cout << "  Danh sach phieu luong khieu nai:\n";
    for (const auto& r : disputed) r.displayInfo();
    std::string recId;
    std::cout << "  Nhap ID de tinh lai (hoac Enter bo qua): ";
    std::getline(std::cin, recId);
    if (!recId.empty()) {
        if (recalculateSalary(recId)) std::cout << "  Da tinh lai phieu luong!\n";
        else std::cout << "  Khong tim thay!\n";
    }
}

void PayrollManager::displayMenu() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n===== LUONG & HOP DONG =====\n"
                  << "  1. Quan ly bang luong\n"
                  << "  2. Quan ly hop dong & KT/KL\n"
                  << "  3. Xu ly khieu nai bang luong\n"
                  << "  0. Quay lai\n"
                  << "Chon: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice) {
        case 1: menuManagePayroll(); break;
        case 2: menuManageContracts(); break;
        case 3: menuHandleDisputes(); break;
        }
    }
}
