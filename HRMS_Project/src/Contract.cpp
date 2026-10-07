#include "Contract.h"
#include <iostream>
#include <sstream>

Contract::Contract()
    : contractType(ContractType::Official), baseSalary(0.0), isActive(true) {}

Contract::Contract(const std::string& cId, const std::string& empId,
                   ContractType type, const Date& sign, const Date& exp, double salary)
    : contractId(cId), employeeId(empId), contractType(type),
      signDate(sign), expiredDate(exp), baseSalary(salary), isActive(true) {}

std::string Contract::contractTypeToString() const {
    switch (contractType) {
    case ContractType::Probation: return "Probation";
    case ContractType::Official:  return "Official";
    case ContractType::PartTime:  return "PartTime";
    case ContractType::Internship:return "Internship";
    default: return "Official";
    }
}

ContractType Contract::stringToContractType(const std::string& s) {
    if (s == "Probation")  return ContractType::Probation;
    if (s == "PartTime")   return ContractType::PartTime;
    if (s == "Internship") return ContractType::Internship;
    return ContractType::Official;
}

void Contract::displayInfo() const {
    std::cout << "  Ma HĐ       : " << contractId                     << "\n"
              << "  Ma NV       : " << employeeId                     << "\n"
              << "  Loai HĐ     : " << contractTypeToString()         << "\n"
              << "  Ngay ky     : " << signDate.toString()            << "\n"
              << "  Het han     : " << expiredDate.toString()         << "\n"
              << "  Luong co ban: " << baseSalary                     << " VND\n"
              << "  Trang thai  : " << (isActive ? "Hieu luc" : "Het hieu luc") << "\n";
}

std::string Contract::toCSV() const {
    return contractId + "," + employeeId + "," + contractTypeToString() + "," +
           signDate.toString() + "," + expiredDate.toString() + "," +
           std::to_string(baseSalary) + "," + (isActive ? "1" : "0");
}

Contract Contract::fromCSV(const std::string& line) {
    std::istringstream ss(line);
    std::string cId, empId, type, sign, exp, salary, active;
    std::getline(ss, cId,    ',');
    std::getline(ss, empId,  ',');
    std::getline(ss, type,   ',');
    std::getline(ss, sign,   ',');
    std::getline(ss, exp,    ',');
    std::getline(ss, salary, ',');
    std::getline(ss, active, ',');
    Contract c(cId, empId, stringToContractType(type),
               Date::fromString(sign), Date::fromString(exp),
               salary.empty() ? 0.0 : std::stod(salary));
    c.isActive = (active == "1");
    return c;
}
