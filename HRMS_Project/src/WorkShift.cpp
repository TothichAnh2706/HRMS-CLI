#include "WorkShift.h"
#include <iostream>
#include <sstream>
#include <iomanip>

std::string TimeHM::toString() const {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << hour << ":"
        << std::setw(2) << minute;
    return oss.str();
}

TimeHM TimeHM::fromString(const std::string& s) {
    TimeHM t;
    if (s.size() >= 5) {
        try {
            t.hour   = std::stoi(s.substr(0, 2));
            t.minute = std::stoi(s.substr(3, 2));
        } catch (...) {}
    }
    return t;
}

WorkShift::WorkShift(const std::string& id, const std::string& name,
                     const TimeHM& start, const TimeHM& end)
    : shiftId(id), shiftName(name), startTime(start), endTime(end) {}

void WorkShift::displayInfo() const {
    std::cout << "  Ca lam viec: " << shiftName
              << " (" << startTime.toString() << " - " << endTime.toString() << ")\n";
}

std::string WorkShift::toCSV() const {
    return shiftId + "," + shiftName + "," + startTime.toString() + "," + endTime.toString();
}

WorkShift WorkShift::fromCSV(const std::string& line) {
    std::istringstream ss(line);
    std::string id, name, start, end;
    std::getline(ss, id,    ',');
    std::getline(ss, name,  ',');
    std::getline(ss, start, ',');
    std::getline(ss, end,   ',');
    return WorkShift(id, name, TimeHM::fromString(start), TimeHM::fromString(end));
}
