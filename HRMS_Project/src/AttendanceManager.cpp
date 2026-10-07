#include "AttendanceManager.h"
#include "EmployeeManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <map>
#include <tuple>

AttendanceManager::AttendanceManager(EmployeeManager* empMgr)
    : employeeManager(empMgr), tkFile("data/timekeeping.csv"), leaveFile("data/leave_requests.csv")
{
    // Default shift: 08:00 - 17:00
    shifts.push_back(WorkShift("SH01", "Ca Hanh Chinh", {8, 0}, {17, 0}));
}

void AttendanceManager::loadFromFile() {
    records.clear();
    std::ifstream f(tkFile);
    if (f.is_open()) {
        std::string line;
        std::getline(f, line);
        while (std::getline(f, line))
            if (!line.empty()) records.push_back(TimekeepingRecord::fromCSV(line));
    }
    leaveRequests.clear();
    std::ifstream f2(leaveFile);
    if (f2.is_open()) {
        std::string line;
        std::getline(f2, line);
        while (std::getline(f2, line))
            if (!line.empty()) leaveRequests.push_back(LeaveRequest::fromCSV(line));
    }
}

void AttendanceManager::saveToFile() {
    std::ofstream f(tkFile);
    f << "RecordID,EmployeeID,Date,CheckIn,CheckOut,LateMinutes,EarlyMinutes\n";
    for (const auto& r : records) f << r.toCSV() << "\n";

    std::ofstream f2(leaveFile);
    f2 << "RequestID,EmployeeID,StartDate,EndDate,Reason,LeaveType,Status,TotalDays\n";
    for (const auto& r : leaveRequests) f2 << r.toCSV() << "\n";
}

WorkShift AttendanceManager::getDefaultShift() const {
    return shifts.empty() ? WorkShift("SH01","Ca HC",{8,0},{17,0}) : shifts[0];
}

TimekeepingRecord* AttendanceManager::findTodayRecord(const std::string& empId, const Date& today) {
    for (auto& r : records)
        if (r.getEmployeeId() == empId && r.getDate() == today) return &r;
    return nullptr;
}

bool AttendanceManager::checkIn(const std::string& empId, const TimeHM& time) {
    if (employeeManager && !employeeManager->isEmployeeExist(empId)) {
        std::cout << "  Nhan vien khong ton tai!\n"; return false;
    }
    // Get today's date (simple: use Date{1,1,2026} as placeholder, real system would use time())
    Date today{6, 10, 2026};
    TimekeepingRecord* existing = findTodayRecord(empId, today);
    if (existing) {
        std::cout << "  Da check-in hom nay roi!\n"; return false;
    }
    std::string recId = generateNextRecordId();
    TimekeepingRecord rec(recId, empId, today);
    rec.setCheckIn(time, getDefaultShift());
    records.push_back(rec);
    saveToFile();
    if (rec.getLateMinutes() > 0)
        std::cout << "  Check-in luc " << time.toString() << " - Di tre " << rec.getLateMinutes() << " phut!\n";
    else
        std::cout << "  Check-in thanh cong luc " << time.toString() << "\n";
    return true;
}

bool AttendanceManager::checkOut(const std::string& empId, const TimeHM& time, const Date& date) {
    TimekeepingRecord* rec = findTodayRecord(empId, date);
    if (!rec) { std::cout << "  Chua check-in hom nay!\n"; return false; }
    if (rec->isCheckedOut()) { std::cout << "  Da check-out roi!\n"; return false; }
    if (time.toMinutes() < rec->getCheckInTime().toMinutes()) {
        std::cout << "  Gio check-out khong the truoc gio check-in!\n"; return false;
    }
    rec->setCheckOut(time, getDefaultShift());
    saveToFile();
    if (rec->getEarlyMinutes() > 0)
        std::cout << "  Check-out luc " << time.toString() << " - Ve som " << rec->getEarlyMinutes() << " phut!\n";
    else
        std::cout << "  Check-out thanh cong luc " << time.toString() << "\n";
    return true;
}

bool AttendanceManager::submitLeaveRequest(const LeaveRequest& req) {
    if (!(req.getStartDate() <= req.getEndDate())) {
        std::cout << "  Ngay bat dau phai truoc ngay ket thuc!\n"; return false;
    }
    if (req.getLeaveType() == LeaveType::Paid) {
        int used = countPaidLeaveUsed(req.getEmployeeId(), req.getStartDate().year);
        if (used + req.getTotalDays() > MAX_PAID_LEAVE_DAYS) {
            std::cout << "  Canh bao: Vuot qua han muc nghi phep co luong ("
                      << MAX_PAID_LEAVE_DAYS << " ngay/nam)!\n";
        }
    }
    leaveRequests.push_back(req);
    saveToFile();
    std::cout << "  Da gui don nghi phep!\n";
    return true;
}

bool AttendanceManager::approveLeave(const std::string& reqId) {
    for (auto& r : leaveRequests)
        if (r.getRequestId() == reqId) { r.approve(); saveToFile(); return true; }
    return false;
}

bool AttendanceManager::rejectLeave(const std::string& reqId) {
    for (auto& r : leaveRequests)
        if (r.getRequestId() == reqId) { r.reject(); saveToFile(); return true; }
    return false;
}

int AttendanceManager::countPaidLeaveUsed(const std::string& empId, int year) const {
    int total = 0;
    for (const auto& r : leaveRequests)
        if (r.getEmployeeId() == empId && r.getLeaveType() == LeaveType::Paid &&
            r.getStatus() == LeaveStatus::Approved && r.getStartDate().year == year)
            total += r.getTotalDays();
    return total;
}

int AttendanceManager::getWorkingDays(const std::string& empId, int month, int year) const {
    int count = 0;
    for (const auto& r : records)
        if (r.getEmployeeId() == empId && r.getDate().month == month && r.getDate().year == year
            && r.isCheckedIn() && r.isCheckedOut())
            count++;
    return count;
}

std::vector<TimekeepingRecord> AttendanceManager::getRecordsByEmployee(const std::string& empId) const {
    std::vector<TimekeepingRecord> res;
    for (const auto& r : records)
        if (r.getEmployeeId() == empId) res.push_back(r);
    return res;
}

std::vector<LeaveRequest> AttendanceManager::getLeaveByEmployee(const std::string& empId) const {
    std::vector<LeaveRequest> res;
    for (const auto& r : leaveRequests)
        if (r.getEmployeeId() == empId) res.push_back(r);
    return res;
}

std::vector<LeaveRequest> AttendanceManager::getPendingLeaves() const {
    std::vector<LeaveRequest> res;
    for (const auto& r : leaveRequests)
        if (r.getStatus() == LeaveStatus::Pending) res.push_back(r);
    return res;
}

void AttendanceManager::printAttendanceReport(int month, int year) const {
    std::cout << "\n===== BAO CAO CHAM CONG " << month << "/" << year << " =====\n";
    std::cout << std::left << std::setw(10) << "Ma NV" << std::setw(8) << "Ngay cong"
              << std::setw(10) << "Di tre(p)" << std::setw(10) << "Ve som(p)" << "\n";
    std::cout << std::string(40, '-') << "\n";
    // Group by employee: map<empId, tuple<workDays, lateMin, earlyMin>>
    std::map<std::string, std::tuple<int,int,int>> empStats;
    for (const auto& r : records) {
        if (r.getDate().month == month && r.getDate().year == year
            && r.isCheckedIn() && r.isCheckedOut())
        {
            std::tuple<int,int,int>& t = empStats[r.getEmployeeId()];
            std::get<0>(t)++;
            std::get<1>(t) += r.getLateMinutes();
            std::get<2>(t) += r.getEarlyMinutes();
        }
    }
    for (auto it = empStats.begin(); it != empStats.end(); ++it) {
        int days  = std::get<0>(it->second);
        int late  = std::get<1>(it->second);
        int early = std::get<2>(it->second);
        std::cout << std::left << std::setw(10) << it->first
                  << std::setw(8)  << days
                  << std::setw(10) << late
                  << std::setw(10) << early << "\n";
    }
}

std::string AttendanceManager::generateNextRecordId() const {
    int maxNum = 0;
    for (const auto& r : records) {
        std::string id = r.getRecordId();
        if (id.size() > 2) try { maxNum = std::max(maxNum, std::stoi(id.substr(2))); } catch (...) {}
    }
    std::ostringstream oss;
    oss << "TK" << std::setfill('0') << std::setw(3) << (maxNum + 1);
    return oss.str();
}

std::string AttendanceManager::generateNextLeaveId() const {
    int maxNum = 0;
    for (const auto& r : leaveRequests) {
        std::string id = r.getRequestId();
        if (id.size() > 2) try { maxNum = std::max(maxNum, std::stoi(id.substr(2))); } catch (...) {}
    }
    std::ostringstream oss;
    oss << "LV" << std::setfill('0') << std::setw(3) << (maxNum + 1);
    return oss.str();
}

void AttendanceManager::menuEmployee(const std::string& empId) {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n===== CHAM CONG & NGHI PHEP =====\n"
                  << "  1. Check-in\n"
                  << "  2. Check-out\n"
                  << "  3. Gui don nghi phep\n"
                  << "  4. Xem lich su cham cong\n"
                  << "  5. Xem don nghi phep\n"
                  << "  0. Quay lai\n"
                  << "Chon: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice == 1) {
            std::string timeStr;
            std::cout << "  Gio check-in (HH:MM): "; std::getline(std::cin, timeStr);
            checkIn(empId, TimeHM::fromString(timeStr));
        } else if (choice == 2) {
            std::string timeStr;
            std::cout << "  Gio check-out (HH:MM): "; std::getline(std::cin, timeStr);
            Date today{6, 10, 2026};
            checkOut(empId, TimeHM::fromString(timeStr), today);
        } else if (choice == 3) {
            std::string start, end, reason, typeStr;
            std::cout << "  Ngay bat dau (DD/MM/YYYY): "; std::getline(std::cin, start);
            std::cout << "  Ngay ket thuc (DD/MM/YYYY): "; std::getline(std::cin, end);
            std::cout << "  Ly do: "; std::getline(std::cin, reason);
            std::cout << "  Loai nghi (Paid/Unpaid): "; std::getline(std::cin, typeStr);
            LeaveRequest req(generateNextLeaveId(), empId,
                             Date::fromString(start), Date::fromString(end),
                             reason, LeaveRequest::stringToLeaveType(typeStr));
            submitLeaveRequest(req);
        } else if (choice == 4) {
            auto recs = getRecordsByEmployee(empId);
            for (const auto& r : recs) r.displayInfo();
        } else if (choice == 5) {
            auto leaves = getLeaveByEmployee(empId);
            for (const auto& l : leaves) l.displayInfo();
        }
    }
}

void AttendanceManager::menuManager() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n===== QUAN LY CHAM CONG (MANAGER) =====\n"
                  << "  1. Duyet don nghi phep\n"
                  << "  2. Tu choi don nghi phep\n"
                  << "  3. Bao cao cham cong thang\n"
                  << "  4. Xem don nghi phep cho duyet\n"
                  << "  0. Quay lai\n"
                  << "Chon: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice == 1 || choice == 2) {
            auto pending = getPendingLeaves();
            if (pending.empty()) { std::cout << "  Khong co don nao cho duyet.\n"; continue; }
            std::cout << "  Danh sach don cho duyet:\n";
            for (const auto& l : pending) l.displayInfo();
            std::string reqId;
            std::cout << "  Nhap ID don: "; std::getline(std::cin, reqId);
            if (choice == 1) approveLeave(reqId), std::cout << "  Da duyet!\n";
            else             rejectLeave(reqId),  std::cout << "  Da tu choi!\n";
        } else if (choice == 3) {
            int m, y;
            std::cout << "  Thang (1-12): "; std::cin >> m;
            std::cout << "  Nam: "; std::cin >> y;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            printAttendanceReport(m, y);
        } else if (choice == 4) {
            auto pending = getPendingLeaves();
            std::cout << "  Co " << pending.size() << " don cho duyet:\n";
            for (const auto& l : pending) l.displayInfo();
        }
    }
}

void AttendanceManager::displayMenu() {
    menuManager();
}
