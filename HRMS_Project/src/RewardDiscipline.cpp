#include "RewardDiscipline.h"
#include <iostream>
#include <sstream>

RewardDiscipline::RewardDiscipline()
    : rdType(RDType::Reward), amount(0.0) {}

RewardDiscipline::RewardDiscipline(const std::string& id, const std::string& empId,
                                   RDType type, double amount, const std::string& reason, const Date& d)
    : rdId(id), employeeId(empId), rdType(type), amount(amount), reason(reason), date(d) {}

std::string RewardDiscipline::rdTypeToString() const {
    return rdType == RDType::Reward ? "Reward" : "Discipline";
}

RDType RewardDiscipline::stringToRDType(const std::string& s) {
    return s == "Reward" ? RDType::Reward : RDType::Discipline;
}

void RewardDiscipline::displayInfo() const {
    std::cout << "  ID: " << rdId
              << "  NV: " << employeeId
              << "  Loai: " << rdTypeToString()
              << "  So tien: " << amount
              << "  Ngay: " << date.toString()
              << "  Ly do: " << reason << "\n";
}

std::string RewardDiscipline::toCSV() const {
    return rdId + "," + employeeId + "," + rdTypeToString() + "," +
           std::to_string(amount) + "," + reason + "," + date.toString();
}

RewardDiscipline RewardDiscipline::fromCSV(const std::string& line) {
    std::istringstream ss(line);
    std::string id, empId, type, amount, reason, date;
    std::getline(ss, id,     ',');
    std::getline(ss, empId,  ',');
    std::getline(ss, type,   ',');
    std::getline(ss, amount, ',');
    std::getline(ss, reason, ',');
    std::getline(ss, date,   ',');
    return RewardDiscipline(id, empId, stringToRDType(type),
                            amount.empty() ? 0.0 : std::stod(amount),
                            reason, Date::fromString(date));
}
