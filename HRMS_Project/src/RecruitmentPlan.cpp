#include "RecruitmentPlan.h"
#include <iostream>
#include <sstream>

RecruitmentPlan::RecruitmentPlan() : targetQuantity(0), currentHired(0) {}

RecruitmentPlan::RecruitmentPlan(const std::string& pid, const std::string& posId,
                                 int qty, const Date& dl)
    : planId(pid), positionId(posId), targetQuantity(qty), deadline(dl), currentHired(0) {}

void RecruitmentPlan::displayInfo() const {
    std::cout << "  Ma KH       : " << planId           << "\n"
              << "  Vi tri      : " << positionId        << "\n"
              << "  Chi tieu    : " << targetQuantity    << "\n"
              << "  Da tuyen    : " << currentHired      << "\n"
              << "  Deadline    : " << deadline.toString() << "\n";
}

std::string RecruitmentPlan::toCSV() const {
    return planId + "," + positionId + "," + std::to_string(targetQuantity) + "," +
           deadline.toString() + "," + std::to_string(currentHired);
}

RecruitmentPlan RecruitmentPlan::fromCSV(const std::string& line) {
    std::istringstream ss(line);
    std::string pid, posId, qty, dl, hired;
    std::getline(ss, pid,   ',');
    std::getline(ss, posId, ',');
    std::getline(ss, qty,   ',');
    std::getline(ss, dl,    ',');
    std::getline(ss, hired, ',');
    RecruitmentPlan rp(pid, posId, qty.empty() ? 0 : std::stoi(qty), Date::fromString(dl));
    rp.currentHired = hired.empty() ? 0 : std::stoi(hired);
    return rp;
}
