/*
 * ============================================================
 *  HRMS - Human Resource Management System
 *  C++ OOP - BTL Nhom 4 Nguoi
 * ============================================================
 *  Thanh vien 1: Core, Employee, Auth
 *  Thanh vien 2: Attendance & Leave
 *  Thanh vien 3: Payroll & Contracts
 *  Thanh vien 4: Recruitment, Training & Reports
 * ============================================================
 */

#include <iostream>
#include <string>
#include <limits>

#include "AuthManager.h"
#include "EmployeeManager.h"
#include "AttendanceManager.h"
#include "PayrollManager.h"
#include "ReportManager.h"

// ─── Banner ──────────────────────────────────────────────────
void printBanner() {
    std::cout << "\n";
    std::cout << "          HE THONG QUAN LY NHAN SU (HRMS)              \n";
    std::cout << "       Human Resource Management System v1.0           \n";
    std::cout << "           BTL OOP - C++ - Nhom 4 Nguoi                \n";
    
}

// ─── Login Screen ─────────────────────────────────────────────
bool loginScreen(AuthManager& auth) {
    int attempts = 0;
    while (attempts < 3) {
        std::string username, password;
        std::cout << "\n  ========== DANG NHAP HE THONG ==========\n";
        std::cout << "  Ten dang nhap: ";
        std::getline(std::cin, username);
        std::cout << "  Mat khau     : ";
        std::getline(std::cin, password);
        if (auth.login(username, password)) return true;
        attempts++;
    }
    std::cout << "  Qua so lan thu. Dang xuat!\n";
    return false;
}

// ─── Admin Menu ───────────────────────────────────────────────
void menuAdmin(AuthManager& auth, EmployeeManager& empMgr,
               AttendanceManager& attMgr, PayrollManager& payMgr,
               ReportManager& repMgr)
{
    int choice = -1;
    while (choice != 0) {
        
        std::cout << "         MENU ADMIN                   \n";
       
        std::cout << "  1. Quan ly Nhan su                  \n";
        std::cout << "  2. Quan ly Cham cong (Manager)      \n";
        std::cout << "  3. Quan ly Luong & Hop dong         \n";
        std::cout << "  4. Tuyen dung, Dao tao & Bao cao    \n";
        std::cout << "  5. Quan ly tai khoan cua toi        \n";
        std::cout << "  0. Dang xuat                        \n";
  
        std::cout << "  Chon: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice) {
        case 1: empMgr.displayMenu(); break;
        case 2: attMgr.menuManager(); break;
        case 3: payMgr.displayMenu(); break;
        case 4: repMgr.displayMenu(); break;
        case 5: auth.displayMenu(); choice = 0; break;
        }
    }
    auth.logout();
}

// ─── Manager Menu ─────────────────────────────────────────────
void menuManager(AuthManager& auth, EmployeeManager& empMgr,
                 AttendanceManager& attMgr, PayrollManager& payMgr,
                 ReportManager& repMgr)
{
    int choice = -1;
    while (choice != 0) {
       
        std::cout << "         MENU MANAGER                 \n";
        
        std::cout << "  1. Quan ly Nhan vien                \n";
        std::cout << "  2. Cham cong & Nghi phep            \n";
        std::cout << "  3. Luong & Hop dong                 \n";
        std::cout << "  4. Tuyen dung & Bao cao             \n";
        std::cout << "  5. Tai khoan cua toi                \n";
        std::cout << "  0. Dang xuat                        \n";
     
        std::cout << "  Chon: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice) {
        case 1: empMgr.menuManageEmployees(); break;
        case 2: attMgr.menuManager(); break;
        case 3: payMgr.displayMenu(); break;
        case 4: repMgr.displayMenu(); break;
        case 5: auth.displayMenu(); choice = 0; break;
        }
    }
    auth.logout();
}

// ─── Employee Menu ────────────────────────────────────────────
void menuEmployee(AuthManager& auth, AttendanceManager& attMgr,
                  PayrollManager& payMgr, ReportManager& repMgr)
{
    std::string empId = auth.getCurrentUser()
                        ? auth.getCurrentUser()->getEmployeeId()
                        : "";
    int choice = -1;
    while (choice != 0) {
        
        std::cout << "         MENU NHAN VIEN               \n";
        
        std::cout << "  1. Cham cong & Nghi phep            \n";
        std::cout << "  2. Xem phieu luong cua toi          \n";
        std::cout << "  3. Gui goi y / Khieu nai            \n";
        std::cout << "  4. Tai khoan cua toi                \n";
        std::cout << "  0. Dang xuat                        \n";
        
        std::cout << "  Chon: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice) {
        case 1: attMgr.menuEmployee(empId); break;
        case 2: {
            int m, y;
            std::cout << "  Thang: "; std::cin >> m;
            std::cout << "  Nam: ";   std::cin >> y;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            SalaryRecord* rec = payMgr.getSalaryRecord(empId, m, y);
            if (rec) rec->printPaySlip();
            else std::cout << "  Chua co phieu luong thang nay!\n";
            break;
        }
        case 3: repMgr.menuFeedback(empId); break;
        case 4: auth.displayMenu(); choice = 0; break;
        }
    }
    auth.logout();
}

// ─── Main ─────────────────────────────────────────────────────
int main() {
    printBanner();

    // ── Initialize all managers ──
    AuthManager      auth;
    EmployeeManager  empMgr(&auth);
    AttendanceManager attMgr(&empMgr);
    PayrollManager   payMgr(&empMgr, &attMgr);
    ReportManager    repMgr(&empMgr, &attMgr, &payMgr);

    // ── Load data from files ──
    std::cout << "\n  Dang tai du lieu...\n";
    auth.loadFromFile();
    empMgr.loadFromFile();
    attMgr.loadFromFile();
    payMgr.loadFromFile();
    repMgr.loadFromFile();
    std::cout << "  Tai du lieu hoan tat!\n";

    // ── Login loop ──
    bool running = true;
    while (running) {
        if (!loginScreen(auth)) break;

        Account* user = auth.getCurrentUser();
        if (!user) break;

        switch (user->getRole()) {
        case Role::Admin:
            menuAdmin(auth, empMgr, attMgr, payMgr, repMgr);
            break;
        case Role::Manager:
            menuManager(auth, empMgr, attMgr, payMgr, repMgr);
            break;
        case Role::Employee:
            menuEmployee(auth, attMgr, payMgr, repMgr);
            break;
        }

        // After logout, ask if want to login again
        if (!auth.isLoggedIn()) {
            std::cout << "\n  Tiep tuc dang nhap? (y/n): ";
            char c;
            std::cin >> c;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            running = (c == 'y' || c == 'Y');
        }
    }

    std::cout << "\n  Cam on da su dung HRMS. Tam biet!\n\n";
    return 0;
}
