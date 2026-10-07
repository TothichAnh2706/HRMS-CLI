#include "Department.h"
#include <iostream>
#include <sstream>

Department::Department(const std::string& id, const std::string& name, const std::string& mgr)
    : departmentId(id), departmentName(name), managerId(mgr) {}

void Department::displayInfo() const {
    std::cout << "  Phong ban ID   : " << departmentId   << "\n"
              << "  Ten phong ban  : " << departmentName  << "\n"
              << "  Truong phong   : " << (managerId.empty() ? "(Chua co)" : managerId) << "\n";
}

std::string Department::toCSV() const {
    return departmentId + "," + departmentName + "," + managerId;
}

Department Department::fromCSV(const std::string& line) {
    std::istringstream ss(line);
    std::string id, name, mgr;
    std::getline(ss, id,   ',');
    std::getline(ss, name, ',');
    std::getline(ss, mgr,  ',');
    return Department(id, name, mgr);
}
