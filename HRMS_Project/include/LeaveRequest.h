#ifndef LEAVE_REQUEST_H
#define LEAVE_REQUEST_H

#include "Date.h"
#include <string>

enum class LeaveType   { Paid, Unpaid };
enum class LeaveStatus { Pending, Approved, Rejected };

class LeaveRequest {
private:
    std::string requestId;
    std::string employeeId;
    Date        startDate;
    Date        endDate;
    std::string reason;
    LeaveType   leaveType;
    LeaveStatus status;
    int         totalDays;

public:
    LeaveRequest();
    LeaveRequest(const std::string& reqId, const std::string& empId,
                 const Date& start, const Date& end,
                 const std::string& reason, LeaveType type);

    std::string getRequestId()  const { return requestId; }
    std::string getEmployeeId() const { return employeeId; }
    Date        getStartDate()  const { return startDate; }
    Date        getEndDate()    const { return endDate; }
    std::string getReason()     const { return reason; }
    LeaveType   getLeaveType()  const { return leaveType; }
    LeaveStatus getStatus()     const { return status; }
    int         getTotalDays()  const { return totalDays; }

    void approve() { status = LeaveStatus::Approved; }
    void reject()  { status = LeaveStatus::Rejected; }

    std::string leaveTypeToString()  const;
    std::string leaveStatusToString()const;
    static LeaveType   stringToLeaveType(const std::string& s);
    static LeaveStatus stringToLeaveStatus(const std::string& s);

    void displayInfo() const;
    std::string toCSV() const;
    static LeaveRequest fromCSV(const std::string& line);
};

#endif // LEAVE_REQUEST_H
