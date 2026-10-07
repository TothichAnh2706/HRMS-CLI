#ifndef ATTENDANCE_MANAGER_H
#define ATTENDANCE_MANAGER_H

#include "BaseManager.h"
#include "TimekeepingRecord.h"
#include "LeaveRequest.h"
#include "WorkShift.h"
#include <vector>
#include <string>

class EmployeeManager;  // Forward declaration

class AttendanceManager : public BaseManager {
private:
    std::vector<TimekeepingRecord> records;
    std::vector<LeaveRequest>      leaveRequests;
    std::vector<WorkShift>         shifts;
    EmployeeManager*               employeeManager;

    std::string tkFile;
    std::string leaveFile;

    static const int MAX_PAID_LEAVE_DAYS = 12;

public:
    explicit AttendanceManager(EmployeeManager* empMgr = nullptr);
    ~AttendanceManager() override = default;

    void loadFromFile() override;
    void saveToFile() override;
    void displayMenu() override;

    // --- Timekeeping ---
    bool checkIn(const std::string& empId, const TimeHM& time);
    bool checkOut(const std::string& empId, const TimeHM& time, const Date& date);
    TimekeepingRecord* findTodayRecord(const std::string& empId, const Date& today);

    // --- Leave requests ---
    bool submitLeaveRequest(const LeaveRequest& req);
    bool approveLeave(const std::string& reqId);
    bool rejectLeave(const std::string& reqId);
    int  countPaidLeaveUsed(const std::string& empId, int year) const;

    // --- API for PayrollManager ---
    int getWorkingDays(const std::string& empId, int month, int year) const;

    // --- Queries ---
    std::vector<TimekeepingRecord> getRecordsByEmployee(const std::string& empId) const;
    std::vector<LeaveRequest>      getLeaveByEmployee(const std::string& empId) const;
    std::vector<LeaveRequest>      getPendingLeaves() const;

    // --- Sub-menus ---
    void menuEmployee(const std::string& empId);
    void menuManager();
    void printAttendanceReport(int month, int year) const;

    WorkShift getDefaultShift() const;
    std::string generateNextRecordId() const;
    std::string generateNextLeaveId() const;
};

#endif // ATTENDANCE_MANAGER_H
