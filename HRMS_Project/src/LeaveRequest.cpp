#include "LeaveRequest.h"
#include <iostream>
#include <sstream>

LeaveRequest::LeaveRequest()
    : leaveType(LeaveType::Paid), status(LeaveStatus::Pending), totalDays(0) {}

LeaveRequest::LeaveRequest(const std::string& reqId, const std::string& empId,
                           const Date& start, const Date& end,
                           const std::string& reason, LeaveType type)
    : requestId(reqId), employeeId(empId), startDate(start), endDate(end),
      reason(reason), leaveType(type), status(LeaveStatus::Pending)
{
    totalDays = start.daysBetween(end) + 1;
}

std::string LeaveRequest::leaveTypeToString() const {
    return leaveType == LeaveType::Paid ? "Paid" : "Unpaid";
}

std::string LeaveRequest::leaveStatusToString() const {
    switch (status) {
    case LeaveStatus::Pending:  return "Pending";
    case LeaveStatus::Approved: return "Approved";
    case LeaveStatus::Rejected: return "Rejected";
    default: return "Pending";
    }
}

LeaveType LeaveRequest::stringToLeaveType(const std::string& s) {
    return s == "Paid" ? LeaveType::Paid : LeaveType::Unpaid;
}

LeaveStatus LeaveRequest::stringToLeaveStatus(const std::string& s) {
    if (s == "Approved") return LeaveStatus::Approved;
    if (s == "Rejected") return LeaveStatus::Rejected;
    return LeaveStatus::Pending;
}

void LeaveRequest::displayInfo() const {
    std::cout << "  ID Don: " << requestId
              << "  NV: " << employeeId
              << "  Tu: " << startDate.toString()
              << " Den: " << endDate.toString()
              << "  So ngay: " << totalDays
              << "  Loai: " << leaveTypeToString()
              << "  TT: " << leaveStatusToString()
              << "  Ly do: " << reason << "\n";
}

std::string LeaveRequest::toCSV() const {
    return requestId + "," + employeeId + "," + startDate.toString() + "," +
           endDate.toString() + "," + reason + "," +
           leaveTypeToString() + "," + leaveStatusToString() + "," +
           std::to_string(totalDays);
}

LeaveRequest LeaveRequest::fromCSV(const std::string& line) {
    std::istringstream ss(line);
    std::string reqId, empId, start, end, reason, type, status, days;
    std::getline(ss, reqId,  ',');
    std::getline(ss, empId,  ',');
    std::getline(ss, start,  ',');
    std::getline(ss, end,    ',');
    std::getline(ss, reason, ',');
    std::getline(ss, type,   ',');
    std::getline(ss, status, ',');
    std::getline(ss, days,   ',');

    LeaveRequest req(reqId, empId, Date::fromString(start), Date::fromString(end),
                     reason, stringToLeaveType(type));
    req.status = stringToLeaveStatus(status);
    req.totalDays = days.empty() ? 0 : std::stoi(days);
    return req;
}
