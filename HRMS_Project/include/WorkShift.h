#ifndef WORK_SHIFT_H
#define WORK_SHIFT_H

#include <string>

struct TimeHM {
    int hour{8};
    int minute{0};
    std::string toString() const;
    static TimeHM fromString(const std::string& s);
    int toMinutes() const { return hour * 60 + minute; }
};

class WorkShift {
private:
    std::string shiftId;
    std::string shiftName;
    TimeHM      startTime;
    TimeHM      endTime;

public:
    WorkShift() = default;
    WorkShift(const std::string& id, const std::string& name,
              const TimeHM& start, const TimeHM& end);

    std::string getShiftId()   const { return shiftId; }
    std::string getShiftName() const { return shiftName; }
    TimeHM      getStartTime() const { return startTime; }
    TimeHM      getEndTime()   const { return endTime; }

    void setShiftId(const std::string& v)   { shiftId = v; }
    void setShiftName(const std::string& v) { shiftName = v; }
    void setStartTime(const TimeHM& v)      { startTime = v; }
    void setEndTime(const TimeHM& v)        { endTime = v; }

    void displayInfo() const;
    std::string toCSV() const;
    static WorkShift fromCSV(const std::string& line);
};

#endif // WORK_SHIFT_H
