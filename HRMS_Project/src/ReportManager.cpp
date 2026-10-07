#include "ReportManager.h"
#include "EmployeeManager.h"
#include "AttendanceManager.h"
#include "PayrollManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <map>
#include <algorithm>

ReportManager::ReportManager(EmployeeManager* empMgr, AttendanceManager* attMgr, PayrollManager* payMgr)
    : employeeManager(empMgr), attendanceManager(attMgr), payrollManager(payMgr),
      candidateFile("data/candidates.csv"), feedbackFile("data/feedbacks.csv") {}

void ReportManager::loadCandidates() {
    candidates.clear();
    std::ifstream f(candidateFile);
    if (!f.is_open()) return;
    std::string line;
    std::getline(f, line);
    while (std::getline(f, line))
        if (!line.empty()) candidates.push_back(Candidate::fromCSV(line));
}

void ReportManager::saveCandidates() {
    std::ofstream f(candidateFile);
    f << "CandidateID,FullName,Gender,BirthDate,Phone,Email,AppliedPosition,InterviewScore,Status\n";
    for (const auto& c : candidates) f << c.toCSV() << "\n";
}

void ReportManager::loadFeedbacks() {
    feedbacks.clear();
    std::ifstream f(feedbackFile);
    if (!f.is_open()) return;
    std::string line;
    std::getline(f, line);
    while (std::getline(f, line))
        if (!line.empty()) feedbacks.push_back(Feedback::fromCSV(line));
}

void ReportManager::saveFeedbacks() {
    std::ofstream f(feedbackFile);
    f << "FeedbackID,EmployeeID,Content,Response,Status\n";
    for (const auto& fb : feedbacks) f << fb.toCSV() << "\n";
}

void ReportManager::loadFromFile() {
    loadCandidates();
    loadFeedbacks();
}

void ReportManager::saveToFile() {
    saveCandidates();
    saveFeedbacks();
}

// ===================== CANDIDATE MANAGEMENT =====================
bool ReportManager::addCandidate(const Candidate& c) {
    candidates.push_back(c);
    saveCandidates();
    return true;
}

bool ReportManager::updateCandidateStatus(const std::string& cId, CandidateStatus status, double score) {
    for (auto& c : candidates) {
        if (c.getCandidateId() == cId) {
            c.setStatus(status);
            if (score >= 0) c.setInterviewScore(score);
            saveCandidates();
            return true;
        }
    }
    return false;
}

bool ReportManager::convertToEmployee(const std::string& cId) {
    Candidate* candidate = getCandidateById(cId);
    if (!candidate) { std::cout << "  Khong tim thay ung vien!\n"; return false; }
    if (candidate->getStatus() != CandidateStatus::Passed) {
        std::cout << "  Ung vien chua co trang thai 'Passed'!\n"; return false;
    }
    if (!employeeManager) { std::cout << "  Loi: EmployeeManager chua khoi tao!\n"; return false; }

    std::string newEmpId = employeeManager->generateNextEmpId();
    if (employeeManager->isEmployeeExist(newEmpId)) {
        std::cout << "  Ma NV bi trung lap!\n"; return false;
    }

    Employee newEmp(newEmpId, newEmpId,
                    candidate->getFullName(),
                    candidate->getGender(),
                    candidate->getBirthDate(),
                    candidate->getPhone(),
                    candidate->getEmail(),
                    "",
                    candidate->getAppliedPosition(),
                    "Active",
                    1.0);
    employeeManager->addEmployee(newEmp);
    std::cout << "  Da chuyen ung vien thanh nhan vien: " << newEmpId << "\n";
    return true;
}

Candidate* ReportManager::getCandidateById(const std::string& cId) {
    for (auto& c : candidates)
        if (c.getCandidateId() == cId) return &c;
    return nullptr;
}

bool ReportManager::addRecruitmentPlan(const RecruitmentPlan& plan) {
    recruitmentPlans.push_back(plan);
    return true;
}

bool ReportManager::addTrainingCourse(const TrainingCourse& course) {
    trainingCourses.push_back(course);
    return true;
}

bool ReportManager::enrollEmployeeInCourse(const std::string& courseId, const std::string& empId) {
    for (auto& c : trainingCourses)
        if (c.getCourseId() == courseId) { c.enrollEmployee(empId); return true; }
    return false;
}

bool ReportManager::submitFeedback(const Feedback& fb) {
    feedbacks.push_back(fb);
    saveFeedbacks();
    return true;
}

bool ReportManager::respondToFeedback(const std::string& fbId, const std::string& response) {
    for (auto& fb : feedbacks)
        if (fb.getFeedbackId() == fbId) { fb.setResponse(response); saveFeedbacks(); return true; }
    return false;
}

// ===================== REPORTS =====================
void ReportManager::reportHeadcountByDepartment() const {
    if (!employeeManager) return;
    std::map<std::string, int> deptCount;
    int total = 0;
    for (const auto& emp : employeeManager->getAllEmployees()) {
        if (emp.getStatus() == "Active") {
            deptCount[emp.getDepartmentId()]++;
            total++;
        }
    }
    std::cout << "\n===== BAO CAO CO CAU NHAN SU THEO PHONG BAN =====\n";
    std::cout << std::left << std::setw(15) << "Phong ban"
              << std::setw(12) << "So NV"
              << std::setw(12) << "Ty le %" << "\n";
    std::cout << std::string(40, '-') << "\n";
    for (auto it = deptCount.begin(); it != deptCount.end(); ++it) {
        const std::string& deptId = it->first;
        int count = it->second;
        double pct = total > 0 ? (100.0 * count / total) : 0;
        std::string deptName = deptId;
        Department* dept = employeeManager->getDepartmentById(deptId);
        if (dept) deptName = dept->getDepartmentName();
        std::cout << std::left
                  << std::setw(15) << deptName
                  << std::setw(12) << count
                  << std::fixed << std::setprecision(1) << pct << "%\n";
    }
    std::cout << std::string(40, '-') << "\n";
    std::cout << "  Tong cong: " << total << " nhan vien\n";
}

void ReportManager::reportPayrollTrend(int months) const {
    if (!payrollManager) return;
    std::cout << "\n===== XU HUONG QUY LUONG " << months << " THANG =====\n";
    Date now{6, 10, 2026};
    for (int i = months - 1; i >= 0; i--) {
        int m = now.month - i;
        int y = now.year;
        while (m <= 0) { m += 12; y--; }
        double total = payrollManager->getTotalPayrollByMonth(m, y);
        std::cout << "  " << std::setfill('0') << std::setw(2) << m << "/" << y
                  << " : " << std::fixed << std::setprecision(0) << total << " VND\n";
    }
}

void ReportManager::reportLateAttendance(int month, int year) const {
    if (!attendanceManager) return;
    attendanceManager->printAttendanceReport(month, year);
}

void ReportManager::reportCandidatePipeline() const {
    std::map<std::string, int> statusCount;
    for (const auto& c : candidates)
        statusCount[c.statusToString()]++;
    std::cout << "\n===== BAO CAO PIPELINE TUYEN DUNG =====\n";
    for (auto it = statusCount.begin(); it != statusCount.end(); ++it)
        std::cout << "  " << std::left << std::setw(12) << it->first << ": " << it->second << " ung vien\n";
    std::cout << "  Tong: " << candidates.size() << " ung vien\n";
}

// ===================== HELPERS =====================
std::string ReportManager::generateNextCandidateId() const {
    int maxNum = 0;
    for (const auto& c : candidates) {
        std::string id = c.getCandidateId();
        if (id.size() > 3) try { maxNum = std::max(maxNum, std::stoi(id.substr(3))); } catch (...) {}
    }
    std::ostringstream oss;
    oss << "CAN" << std::setfill('0') << std::setw(3) << (maxNum + 1);
    return oss.str();
}

std::string ReportManager::generateNextCourseId() const {
    int maxNum = 0;
    for (const auto& c : trainingCourses) {
        std::string id = c.getCourseId();
        if (id.size() > 3) try { maxNum = std::max(maxNum, std::stoi(id.substr(3))); } catch (...) {}
    }
    std::ostringstream oss;
    oss << "CRS" << std::setfill('0') << std::setw(3) << (maxNum + 1);
    return oss.str();
}

std::string ReportManager::generateNextPlanId() const {
    int maxNum = recruitmentPlans.size();
    std::ostringstream oss;
    oss << "PLN" << std::setfill('0') << std::setw(3) << (maxNum + 1);
    return oss.str();
}

std::string ReportManager::generateNextFeedbackId() const {
    int maxNum = 0;
    for (const auto& f : feedbacks) {
        std::string id = f.getFeedbackId();
        if (id.size() > 2) try { maxNum = std::max(maxNum, std::stoi(id.substr(2))); } catch (...) {}
    }
    std::ostringstream oss;
    oss << "FB" << std::setfill('0') << std::setw(3) << (maxNum + 1);
    return oss.str();
}

// ===================== MENUS =====================
void ReportManager::menuRecruitment() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n===== QUAN LY TUYEN DUNG =====\n"
                  << "  1. Them ung vien moi\n"
                  << "  2. Xem danh sach ung vien\n"
                  << "  3. Cap nhat trang thai ung vien\n"
                  << "  4. Chuyen ung vien thanh nhan vien\n"
                  << "  5. Bao cao pipeline tuyen dung\n"
                  << "  6. Quan ly ke hoach tuyen dung\n"
                  << "  0. Quay lai\n"
                  << "Chon: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice == 1) {
            std::string name, gender, birth, phone, email, pos;
            std::string cId = generateNextCandidateId();
            std::cout << "  Ma UV tu dong: " << cId << "\n";
            std::cout << "  Ho va ten: ";   std::getline(std::cin, name);
            std::cout << "  Gioi tinh: ";   std::getline(std::cin, gender);
            std::cout << "  Ngay sinh (DD/MM/YYYY): "; std::getline(std::cin, birth);
            std::cout << "  Dien thoai: ";  std::getline(std::cin, phone);
            std::cout << "  Email: ";       std::getline(std::cin, email);
            std::cout << "  Vi tri ung tuyen: "; std::getline(std::cin, pos);
            Candidate c(cId, cId, name, gender, Date::fromString(birth), phone, email, pos);
            addCandidate(c);
            std::cout << "  Da them ung vien!\n";
        } else if (choice == 2) {
            for (const auto& c : candidates) c.displayInfo(), std::cout << std::string(40,'-') << "\n";
        } else if (choice == 3) {
            std::string cId, statusStr, scoreStr;
            std::cout << "  Ma UV: "; std::getline(std::cin, cId);
            std::cout << "  Trang thai (Applied/Interview/Passed/Rejected): "; std::getline(std::cin, statusStr);
            std::cout << "  Diem phong van (0-100, -1 de bo qua): "; std::getline(std::cin, scoreStr);
            double sc = scoreStr.empty() ? -1 : std::stod(scoreStr);
            if (updateCandidateStatus(cId, Candidate::stringToStatus(statusStr), sc))
                std::cout << "  Da cap nhat!\n";
            else std::cout << "  Khong tim thay UV!\n";
        } else if (choice == 4) {
            std::string cId;
            std::cout << "  Ma UV: "; std::getline(std::cin, cId);
            convertToEmployee(cId);
        } else if (choice == 5) {
            reportCandidatePipeline();
        } else if (choice == 6) {
            std::string pid, posId, qty, dl;
            std::string planId = generateNextPlanId();
            std::cout << "  Ma KH tuyen dung: " << planId << "\n";
            std::cout << "  Vi tri can tuyen (posId): "; std::getline(std::cin, posId);
            std::cout << "  Chi tieu: "; std::getline(std::cin, qty);
            std::cout << "  Deadline (DD/MM/YYYY): "; std::getline(std::cin, dl);
            addRecruitmentPlan(RecruitmentPlan(planId, posId,
                qty.empty() ? 0 : std::stoi(qty), Date::fromString(dl)));
            std::cout << "  Da tao ke hoach tuyen dung!\n";
        }
    }
}

void ReportManager::menuTraining() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n===== QUAN LY DAO TAO =====\n"
                  << "  1. Them khoa dao tao\n"
                  << "  2. Dang ky NV vao khoa hoc\n"
                  << "  3. Xem danh sach khoa hoc\n"
                  << "  0. Quay lai\n"
                  << "Chon: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice == 1) {
            std::string name, start, end, instr;
            std::string cId = generateNextCourseId();
            std::cout << "  Ma khoa hoc: " << cId << "\n";
            std::cout << "  Ten khoa: "; std::getline(std::cin, name);
            std::cout << "  Bat dau (DD/MM/YYYY): "; std::getline(std::cin, start);
            std::cout << "  Ket thuc (DD/MM/YYYY): "; std::getline(std::cin, end);
            std::cout << "  Giang vien: "; std::getline(std::cin, instr);
            addTrainingCourse(TrainingCourse(cId, name, Date::fromString(start), Date::fromString(end), instr));
            std::cout << "  Da them khoa hoc!\n";
        } else if (choice == 2) {
            std::string courseId, empId;
            std::cout << "  Ma khoa hoc: "; std::getline(std::cin, courseId);
            std::cout << "  Ma NV: "; std::getline(std::cin, empId);
            if (enrollEmployeeInCourse(courseId, empId))
                std::cout << "  Da dang ky!\n";
            else std::cout << "  Khong tim thay khoa hoc!\n";
        } else if (choice == 3) {
            for (const auto& tc : trainingCourses)
                tc.displayInfo(), std::cout << std::string(40,'-') << "\n";
        }
    }
}

void ReportManager::menuReports() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n===== BAO CAO THONG KE =====\n"
                  << "  1. Co cau nhan su theo phong ban\n"
                  << "  2. Xu huong quy luong\n"
                  << "  3. Bao cao di tre/ve som\n"
                  << "  4. Pipeline tuyen dung\n"
                  << "  0. Quay lai\n"
                  << "Chon: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice) {
        case 1: reportHeadcountByDepartment(); break;
        case 2: reportPayrollTrend(3); break;
        case 3: {
            int m, y;
            std::cout << "  Thang: "; std::cin >> m;
            std::cout << "  Nam: ";   std::cin >> y;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            reportLateAttendance(m, y);
            break;
        }
        case 4: reportCandidatePipeline(); break;
        }
    }
}

void ReportManager::menuFeedback(const std::string& empId) {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n===== GOI Y / KHIEU NAI =====\n"
                  << "  1. Gui goi y/khieu nai\n"
                  << "  2. Xem phan hoi cua toi\n"
                  << "  0. Quay lai\n"
                  << "Chon: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice == 1) {
            std::string content;
            std::cout << "  Noi dung: "; std::getline(std::cin, content);
            Feedback fb(generateNextFeedbackId(), empId, content);
            submitFeedback(fb);
            std::cout << "  Da gui!\n";
        } else if (choice == 2) {
            for (const auto& fb : feedbacks)
                if (fb.getEmployeeId() == empId) fb.displayInfo();
        }
    }
}

void ReportManager::menuAdminFeedback() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n===== QUAN LY GOI Y (ADMIN) =====\n"
                  << "  1. Xem tat ca goi y\n"
                  << "  2. Phan hoi goi y\n"
                  << "  0. Quay lai\n"
                  << "Chon: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice == 1) {
            for (const auto& fb : feedbacks) fb.displayInfo(), std::cout << std::string(40,'-') << "\n";
        } else if (choice == 2) {
            std::string fbId, resp;
            std::cout << "  ID goi y: "; std::getline(std::cin, fbId);
            std::cout << "  Phan hoi: "; std::getline(std::cin, resp);
            if (respondToFeedback(fbId, resp)) std::cout << "  Da phan hoi!\n";
            else std::cout << "  Khong tim thay!\n";
        }
    }
}

void ReportManager::displayMenu() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n===== TUYEN DUNG, DAO TAO & BAO CAO =====\n"
                  << "  1. Quan ly tuyen dung\n"
                  << "  2. Quan ly dao tao\n"
                  << "  3. Bao cao thong ke\n"
                  << "  4. Quan ly goi y/khieu nai (Admin)\n"
                  << "  0. Quay lai\n"
                  << "Chon: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice) {
        case 1: menuRecruitment(); break;
        case 2: menuTraining(); break;
        case 3: menuReports(); break;
        case 4: menuAdminFeedback(); break;
        }
    }
}
