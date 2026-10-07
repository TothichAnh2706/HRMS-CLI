#include "Employee.h"
#include <iostream>
#include <sstream>
#include <iomanip>

Employee::Employee(const std::string& empId,
                   const std::string& personId,
                   const std::string& name,
                   const std::string& gender,
                   const Date& birth,
                   const std::string& phone,
                   const std::string& email,
                   const std::string& deptId,
                   const std::string& posId,
                   const std::string& st,
                   double salaryRate)
    : Person(personId, name, gender, birth, phone, email),
      employeeId(empId), departmentId(deptId), positionId(posId),
      status(st), baseSalaryRate(salaryRate)
{}

void Employee::displayInfo() const {
    std::cout << std::left
              << "  Ma NV          : " << employeeId       << "\n"
              << "  Ho va ten      : " << fullName          << "\n"
              << "  Gioi tinh      : " << gender            << "\n"
              << "  Ngay sinh      : " << birthDate.toString() << "\n"
              << "  So dien thoai  : " << phone             << "\n"
              << "  Email          : " << email             << "\n"
              << "  Phong ban      : " << departmentId      << "\n"
              << "  Chuc vu        : " << positionId        << "\n"
              << "  Trang thai     : " << status            << "\n"
              << "  He so luong    : " << baseSalaryRate     << "\n";
}

std::string Employee::toCSV() const {
    return employeeId + "," + fullName + "," + gender + "," +
           birthDate.toString() + "," + phone + "," + email + "," +
           departmentId + "," + positionId + "," + status + "," +
           std::to_string(baseSalaryRate);
}

Employee Employee::fromCSV(const std::string& line) {
    std::istringstream ss(line);
    std::string empId, name, gender, birth, phone, email, dept, pos, status, rate;
    std::getline(ss, empId,  ',');
    std::getline(ss, name,   ',');
    std::getline(ss, gender, ',');
    std::getline(ss, birth,  ',');
    std::getline(ss, phone,  ',');
    std::getline(ss, email,  ',');
    std::getline(ss, dept,   ',');
    std::getline(ss, pos,    ',');
    std::getline(ss, status, ',');
    std::getline(ss, rate,   ',');

    double r = rate.empty() ? 1.0 : std::stod(rate);
    return Employee(empId, empId, name, gender, Date::fromString(birth),
                    phone, email, dept, pos, status, r);
}
