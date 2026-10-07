#ifndef TIMEKEEPING_RECORD_H
#define TIMEKEEPING_RECORD_H

#include "Date.h"
#include "WorkShift.h"
#include <string>

class TimekeepingRecord {
private:
    std::string recordId;
    std::string employeeId;
    Date        date;
    TimeHM      checkInTime;
    TimeHM      checkOutTime;
    int         lateMinutes;
    int         earlyMinutes;
    bool        checkedIn;
    bool        checkedOut;

public:
    TimekeepingRecord();
    TimekeepingRecord(const std::string& recId, const std::string& empId, const Date& d);

    std::string getRecordId()    const { return recordId; }
    std::string getEmployeeId()  const { return employeeId; }
    Date        getDate()        const { return date; }
    TimeHM      getCheckInTime() const { return checkInTime; }
    TimeHM      getCheckOutTime()const { return checkOutTime; }
    int         getLateMinutes() const { return lateMinutes; }
    int         getEarlyMinutes()const { return earlyMinutes; }
    bool        isCheckedIn()    const { return checkedIn; }
    bool        isCheckedOut()   const { return checkedOut; }

    void setCheckIn(const TimeHM& t, const WorkShift& shift);
    void setCheckOut(const TimeHM& t, const WorkShift& shift);

    void displayInfo() const;
    std::string toCSV() const;
    static TimekeepingRecord fromCSV(const std::string& line);
};

#endif // TIMEKEEPING_RECORD_H
