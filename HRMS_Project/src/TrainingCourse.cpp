#include "TrainingCourse.h"
#include <iostream>
#include <sstream>
#include <algorithm>

TrainingCourse::TrainingCourse(const std::string& cId, const std::string& name,
                               const Date& start, const Date& end, const std::string& instr)
    : courseId(cId), courseName(name), startDate(start), endDate(end), instructor(instr) {}

void TrainingCourse::enrollEmployee(const std::string& empId) {
    if (!isEnrolled(empId)) listEmployeeIds.push_back(empId);
}

void TrainingCourse::unenrollEmployee(const std::string& empId) {
    listEmployeeIds.erase(
        std::remove(listEmployeeIds.begin(), listEmployeeIds.end(), empId),
        listEmployeeIds.end());
}

bool TrainingCourse::isEnrolled(const std::string& empId) const {
    return std::find(listEmployeeIds.begin(), listEmployeeIds.end(), empId) != listEmployeeIds.end();
}

void TrainingCourse::displayInfo() const {
    std::cout << "  Ma KH  : " << courseId   << "\n"
              << "  Ten KH : " << courseName  << "\n"
              << "  GV     : " << instructor  << "\n"
              << "  Tu     : " << startDate.toString() << " - " << endDate.toString() << "\n"
              << "  So hoc vien: " << listEmployeeIds.size() << "\n";
}

std::string TrainingCourse::toCSV() const {
    std::string empIds;
    for (size_t i = 0; i < listEmployeeIds.size(); i++) {
        if (i > 0) empIds += ";";
        empIds += listEmployeeIds[i];
    }
    return courseId + "," + courseName + "," + startDate.toString() + "," +
           endDate.toString() + "," + instructor + "," + empIds;
}

TrainingCourse TrainingCourse::fromCSV(const std::string& line) {
    std::istringstream ss(line);
    std::string cId, name, start, end, instr, empIds;
    std::getline(ss, cId,    ',');
    std::getline(ss, name,   ',');
    std::getline(ss, start,  ',');
    std::getline(ss, end,    ',');
    std::getline(ss, instr,  ',');
    std::getline(ss, empIds, ',');

    TrainingCourse tc(cId, name, Date::fromString(start), Date::fromString(end), instr);
    // Parse semicolon-separated employee IDs
    std::istringstream idStream(empIds);
    std::string empId;
    while (std::getline(idStream, empId, ';'))
        if (!empId.empty()) tc.listEmployeeIds.push_back(empId);
    return tc;
}
