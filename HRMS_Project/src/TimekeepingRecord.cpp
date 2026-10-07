#include "TimekeepingRecord.h"
#include <iostream>
#include <sstream>

TimekeepingRecord::TimekeepingRecord()
    : lateMinutes(0), earlyMinutes(0), checkedIn(false), checkedOut(false) {}

TimekeepingRecord::TimekeepingRecord(const std::string& recId, const std::string& empId, const Date& d)
    : recordId(recId), employeeId(empId), date(d),
      lateMinutes(0), earlyMinutes(0), checkedIn(false), checkedOut(false) {}

void TimekeepingRecord::setCheckIn(const TimeHM& t, const WorkShift& shift) {
    checkInTime = t;
    checkedIn = true;
    int diff = t.toMinutes() - shift.getStartTime().toMinutes();
    lateMinutes = diff > 0 ? diff : 0;
}

void TimekeepingRecord::setCheckOut(const TimeHM& t, const WorkShift& shift) {
    checkOutTime = t;
    checkedOut = true;
    int diff = shift.getEndTime().toMinutes() - t.toMinutes();
    earlyMinutes = diff > 0 ? diff : 0;
}

void TimekeepingRecord::displayInfo() const {
    std::cout << "  Ngay: " << date.toString()
              << "  Check-in: " << (checkedIn ? checkInTime.toString() : "---")
              << "  Check-out: " << (checkedOut ? checkOutTime.toString() : "---")
              << "  Di tre: " << lateMinutes << " phut"
              << "  Ve som: " << earlyMinutes << " phut\n";
}

std::string TimekeepingRecord::toCSV() const {
    return recordId + "," + employeeId + "," + date.toString() + "," +
           checkInTime.toString() + "," + checkOutTime.toString() + "," +
           std::to_string(lateMinutes) + "," + std::to_string(earlyMinutes);
}

TimekeepingRecord TimekeepingRecord::fromCSV(const std::string& line) {
    std::istringstream ss(line);
    std::string recId, empId, date, ci, co, late, early;
    std::getline(ss, recId, ',');
    std::getline(ss, empId, ',');
    std::getline(ss, date,  ',');
    std::getline(ss, ci,    ',');
    std::getline(ss, co,    ',');
    std::getline(ss, late,  ',');
    std::getline(ss, early, ',');

    TimekeepingRecord rec(recId, empId, Date::fromString(date));
    rec.checkInTime  = TimeHM::fromString(ci);
    rec.checkOutTime = TimeHM::fromString(co);
    rec.lateMinutes  = late.empty()  ? 0 : std::stoi(late);
    rec.earlyMinutes = early.empty() ? 0 : std::stoi(early);
    rec.checkedIn    = !ci.empty() && ci != "00:00";
    rec.checkedOut   = !co.empty() && co != "00:00";
    return rec;
}
