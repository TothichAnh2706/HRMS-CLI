#include "SalaryRecord.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

SalaryRecord::SalaryRecord()
    : month(1), year(2026), workDays(0), baseSalary(0), allowance(0),
      bonus(0), deduction(0), bhxh(0), tax(0), netSalary(0), isDisputed(false) {}

SalaryRecord::SalaryRecord(const std::string& recId, const std::string& empId,
                           int m, int y, int wd,
                           double base, double allow, double bon, double deduct)
    : recordId(recId), employeeId(empId), month(m), year(y), workDays(wd),
      baseSalary(base), allowance(allow), bonus(bon), deduction(deduct),
      bhxh(0), tax(0), netSalary(0), isDisputed(false)
{
    calculate();
}

// ===================== THUẬT TOÁN TÍNH LƯƠNG =====================
void SalaryRecord::calculate() {
    // Step 1: Gross income
    double grossIncome = (baseSalary * workDays / 22.0) + allowance + bonus - deduction;

    // Step 2: BHXH = baseSalary * 10.5%
    bhxh = baseSalary * 0.105;

    // Step 3: Taxable income = grossIncome - bhxh - personal deduction (11 million)
    const double PERSONAL_DEDUCTION = 11000000.0;
    double taxableIncome = grossIncome - bhxh - PERSONAL_DEDUCTION;
    taxableIncome = std::max(0.0, taxableIncome);

    // Step 4: Progressive personal income tax (PIT)
    // Vietnamese PIT brackets:
    // <= 5M:  5%
    // 5-10M:  10%
    // 10-18M: 15%
    // 18-32M: 20%
    // 32-52M: 25%
    // 52-80M: 30%
    // > 80M:  35%
    tax = 0.0;
    double remaining = taxableIncome;
    if (remaining > 0) {
        double brackets[][2] = {{5000000, 0.05}, {5000000, 0.10}, {8000000, 0.15},
                                {14000000, 0.20}, {20000000, 0.25}, {28000000, 0.30}};
        for (auto& b : brackets) {
            double chunk = std::min(remaining, b[0]);
            tax += chunk * b[1];
            remaining -= chunk;
            if (remaining <= 0) break;
        }
        if (remaining > 0) tax += remaining * 0.35;
    }

    // Step 5: Net salary
    netSalary = grossIncome - bhxh - tax;
}

void SalaryRecord::displayInfo() const {
    std::cout << "  ID: " << recordId << "  NV: " << employeeId
              << "  Thang: " << month << "/" << year
              << "  Ngay cong: " << workDays
              << "  Luong net: " << std::fixed << std::setprecision(0) << netSalary << " VND\n";
}

void SalaryRecord::printPaySlip() const {
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "          PHIEU LUONG THANG " << month << "/" << year << "\n";
    std::cout << std::string(50, '=') << "\n";
    std::cout << std::left;
    std::cout << std::setw(30) << "Ma nhan vien:"         << employeeId   << "\n";
    std::cout << std::setw(30) << "Ngay cong thuc te:"    << workDays << " ngay / 22 ngay\n";
    std::cout << std::string(50, '-') << "\n";
    std::cout << std::setw(30) << "Luong co ban (x ngay cong):"
              << std::fixed << std::setprecision(0) << (baseSalary * workDays / 22.0) << "\n";
    std::cout << std::setw(30) << "+ Phu cap:" << allowance << "\n";
    std::cout << std::setw(30) << "+ Thuong:"  << bonus     << "\n";
    std::cout << std::setw(30) << "- Phat:"    << deduction << "\n";
    std::cout << std::string(50, '-') << "\n";
    double gross = (baseSalary * workDays / 22.0) + allowance + bonus - deduction;
    std::cout << std::setw(30) << "Tong thu nhap:"        << gross    << "\n";
    std::cout << std::setw(30) << "- BHXH (10.5%):"       << bhxh     << "\n";
    std::cout << std::setw(30) << "- Thue TNCN:"          << tax      << "\n";
    std::cout << std::string(50, '=') << "\n";
    std::cout << std::setw(30) << "LUONG NET:" << netSalary << " VND\n";
    std::cout << std::string(50, '=') << "\n";
    if (isDisputed) std::cout << "  [!] Phieu luong dang khieu nai!\n";
}

std::string SalaryRecord::toCSV() const {
    return recordId + "," + employeeId + "," + std::to_string(month) + "," +
           std::to_string(year) + "," + std::to_string(workDays) + "," +
           std::to_string(baseSalary) + "," + std::to_string(allowance) + "," +
           std::to_string(bonus) + "," + std::to_string(deduction) + "," +
           std::to_string(tax) + "," + std::to_string(netSalary) + "," +
           (isDisputed ? "1" : "0");
}

SalaryRecord SalaryRecord::fromCSV(const std::string& line) {
    std::istringstream ss(line);
    std::string recId, empId, m, y, wd, base, allow, bonus, deduct, tax, net, disputed;
    std::getline(ss, recId,    ',');
    std::getline(ss, empId,    ',');
    std::getline(ss, m,        ',');
    std::getline(ss, y,        ',');
    std::getline(ss, wd,       ',');
    std::getline(ss, base,     ',');
    std::getline(ss, allow,    ',');
    std::getline(ss, bonus,    ',');
    std::getline(ss, deduct,   ',');
    std::getline(ss, tax,      ',');
    std::getline(ss, net,      ',');
    std::getline(ss, disputed, ',');

    SalaryRecord rec;
    rec.recordId   = recId;
    rec.employeeId = empId;
    rec.month      = m.empty()    ? 1 : std::stoi(m);
    rec.year       = y.empty()    ? 2026 : std::stoi(y);
    rec.workDays   = wd.empty()   ? 0 : std::stoi(wd);
    rec.baseSalary = base.empty() ? 0 : std::stod(base);
    rec.allowance  = allow.empty()? 0 : std::stod(allow);
    rec.bonus      = bonus.empty()? 0 : std::stod(bonus);
    rec.deduction  = deduct.empty()? 0 : std::stod(deduct);
    rec.tax        = tax.empty()  ? 0 : std::stod(tax);
    rec.netSalary  = net.empty()  ? 0 : std::stod(net);
    rec.isDisputed = (disputed == "1");
    return rec;
}
