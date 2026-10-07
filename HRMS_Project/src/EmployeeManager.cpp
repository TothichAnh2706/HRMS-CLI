#include "EmployeeManager.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <sstream>

EmployeeManager::EmployeeManager(AuthManager* auth)
    : authManager(auth),
      empFile("data/employees.csv"),
      deptFile("data/departments.csv"),
      posFile("data/positions.csv")
{}

// ===================== FILE I/O =====================
void EmployeeManager::loadEmployees() {
    employees.clear();
    std::ifstream f(empFile);
    if (!f.is_open()) return;
    std::string line;
    std::getline(f, line); // header
    while (std::getline(f, line))
        if (!line.empty()) employees.push_back(Employee::fromCSV(line));
}

void EmployeeManager::savEmployees() {
    std::ofstream f(empFile);
    f << "EmployeeID,FullName,Gender,BirthDate,Phone,Email,DepartmentID,PositionID,Status,BaseSalaryRate\n";
    for (const auto& e : employees) f << e.toCSV() << "\n";
}

void EmployeeManager::loadDepartments() {
    departments.clear();
    std::ifstream f(deptFile);
    if (!f.is_open()) return;
    std::string line;
    std::getline(f, line);
    while (std::getline(f, line))
        if (!line.empty()) departments.push_back(Department::fromCSV(line));
}

void EmployeeManager::saveDepartments() {
    std::ofstream f(deptFile);
    f << "DepartmentID,DepartmentName,ManagerID\n";
    for (const auto& d : departments) f << d.toCSV() << "\n";
}

void EmployeeManager::loadPositions() {
    positions.clear();
    std::ifstream f(posFile);
    if (!f.is_open()) return;
    std::string line;
    std::getline(f, line);
    while (std::getline(f, line))
        if (!line.empty()) positions.push_back(Position::fromCSV(line));
}

void EmployeeManager::savePositions() {
    std::ofstream f(posFile);
    f << "PositionID,PositionName,AllowanceCoeff\n";
    for (const auto& p : positions) f << p.toCSV() << "\n";
}

void EmployeeManager::loadFromFile() {
    loadEmployees();
    loadDepartments();
    loadPositions();
}

void EmployeeManager::saveToFile() {
    savEmployees();
    saveDepartments();
    savePositions();
}

// ===================== EMPLOYEE CRUD =====================
bool EmployeeManager::addEmployee(const Employee& emp) {
    if (isEmployeeExist(emp.getEmployeeId())) {
        std::cout << "  Ma nhan vien da ton tai!\n";
        return false;
    }
    employees.push_back(emp);
    savEmployees();
    std::cout << "  Them nhan vien thanh cong!\n";
    return true;
}

bool EmployeeManager::updateEmployee(const std::string& empId, const Employee& updated) {
    for (auto& e : employees) {
        if (e.getEmployeeId() == empId) {
            e = updated;
            savEmployees();
            std::cout << "  Cap nhat thanh cong!\n";
            return true;
        }
    }
    std::cout << "  Khong tim thay nhan vien!\n";
    return false;
}

bool EmployeeManager::removeEmployee(const std::string& empId) {
    for (auto& e : employees) {
        if (e.getEmployeeId() == empId) {
            e.setStatus("Resigned");
            savEmployees();
            std::cout << "  Da cap nhat trang thai 'Nghi viec'.\n";
            return true;
        }
    }
    return false;
}

Employee* EmployeeManager::getEmployeeById(const std::string& empId) {
    for (auto& e : employees)
        if (e.getEmployeeId() == empId) return &e;
    return nullptr;
}

bool EmployeeManager::isEmployeeExist(const std::string& empId) const {
    for (const auto& e : employees)
        if (e.getEmployeeId() == empId) return true;
    return false;
}

std::vector<Employee*> EmployeeManager::searchByName(const std::string& name) {
    std::vector<Employee*> result;
    std::string nameLower = name;
    std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
    for (auto& e : employees) {
        std::string n = e.getFullName();
        std::transform(n.begin(), n.end(), n.begin(), ::tolower);
        if (n.find(nameLower) != std::string::npos) result.push_back(&e);
    }
    return result;
}

std::vector<Employee*> EmployeeManager::searchByDepartment(const std::string& deptId) {
    std::vector<Employee*> result;
    for (auto& e : employees)
        if (e.getDepartmentId() == deptId) result.push_back(&e);
    return result;
}

// ===================== DEPARTMENT CRUD =====================
bool EmployeeManager::addDepartment(const Department& dept) {
    for (const auto& d : departments)
        if (d.getDepartmentId() == dept.getDepartmentId()) {
            std::cout << "  Ma phong ban da ton tai!\n";
            return false;
        }
    departments.push_back(dept);
    saveDepartments();
    return true;
}

bool EmployeeManager::removeDepartment(const std::string& deptId) {
    for (auto it = departments.begin(); it != departments.end(); ++it)
        if (it->getDepartmentId() == deptId) {
            departments.erase(it);
            saveDepartments();
            return true;
        }
    return false;
}

Department* EmployeeManager::getDepartmentById(const std::string& deptId) {
    for (auto& d : departments)
        if (d.getDepartmentId() == deptId) return &d;
    return nullptr;
}

// ===================== POSITION CRUD =====================
bool EmployeeManager::addPosition(const Position& pos) {
    for (const auto& p : positions)
        if (p.getPositionId() == pos.getPositionId()) {
            std::cout << "  Ma chuc vu da ton tai!\n";
            return false;
        }
    positions.push_back(pos);
    savePositions();
    return true;
}

bool EmployeeManager::removePosition(const std::string& posId) {
    for (auto it = positions.begin(); it != positions.end(); ++it)
        if (it->getPositionId() == posId) {
            positions.erase(it);
            savePositions();
            return true;
        }
    return false;
}

Position* EmployeeManager::getPositionById(const std::string& posId) {
    for (auto& p : positions)
        if (p.getPositionId() == posId) return &p;
    return nullptr;
}

// ===================== HELPERS =====================
std::string EmployeeManager::generateNextEmpId() const {
    int maxNum = 0;
    for (const auto& e : employees) {
        std::string id = e.getEmployeeId();
        if (id.substr(0, 3) == "EMP") {
            try { maxNum = std::max(maxNum, std::stoi(id.substr(3))); } catch (...) {}
        }
    }
    std::ostringstream oss;
    oss << "EMP" << std::setfill('0') << std::setw(3) << (maxNum + 1);
    return oss.str();
}

void EmployeeManager::printEmployeeList() const {
    std::cout << "\n" << std::string(90, '-') << "\n";
    std::cout << std::left
              << std::setw(8)  << "Ma NV"
              << std::setw(25) << "Ho va ten"
              << std::setw(8)  << "GT"
              << std::setw(12) << "Phong ban"
              << std::setw(10) << "Trang thai" << "\n";
    std::cout << std::string(90, '-') << "\n";
    for (const auto& e : employees) {
        std::cout << std::left
                  << std::setw(8)  << e.getEmployeeId()
                  << std::setw(25) << e.getFullName()
                  << std::setw(8)  << e.getGender()
                  << std::setw(12) << e.getDepartmentId()
                  << std::setw(10) << e.getStatus() << "\n";
    }
    std::cout << std::string(90, '-') << "\n";
}

// ===================== MENUS =====================
void EmployeeManager::menuManageEmployees() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n===== QUAN LY NHAN VIEN =====\n"
                  << "  1. Xem danh sach nhan vien\n"
                  << "  2. Them nhan vien moi\n"
                  << "  3. Cap nhat nhan vien\n"
                  << "  4. Nghi viec nhan vien\n"
                  << "  5. Tim kiem theo ten\n"
                  << "  6. Tim kiem theo phong ban\n"
                  << "  7. Xem chi tiet nhan vien\n"
                  << "  0. Quay lai\n"
                  << "Chon: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            printEmployeeList();
        } else if (choice == 2) {
            if (authManager && !authManager->hasPermission(Role::Manager)) {
                std::cout << "  Khong co quyen!\n"; continue;
            }
            std::string empId = generateNextEmpId();
            std::string name, gender, birth, phone, email, deptId, posId, rateStr;
            std::cout << "  Ma NV tu dong: " << empId << "\n";
            std::cout << "  Ho va ten: ";      std::getline(std::cin, name);
            std::cout << "  Gioi tinh (Male/Female): "; std::getline(std::cin, gender);
            std::cout << "  Ngay sinh (DD/MM/YYYY): "; std::getline(std::cin, birth);
            std::cout << "  So dien thoai: "; std::getline(std::cin, phone);
            std::cout << "  Email: ";         std::getline(std::cin, email);
            std::cout << "  Ma phong ban: "; std::getline(std::cin, deptId);
            std::cout << "  Ma chuc vu: ";   std::getline(std::cin, posId);
            std::cout << "  He so luong: ";  std::getline(std::cin, rateStr);
            double rate = rateStr.empty() ? 1.0 : std::stod(rateStr);
            Employee emp(empId, empId, name, gender, Date::fromString(birth),
                         phone, email, deptId, posId, "Active", rate);
            addEmployee(emp);
        } else if (choice == 3) {
            std::string id;
            std::cout << "  Ma NV can cap nhat: "; std::getline(std::cin, id);
            Employee* e = getEmployeeById(id);
            if (!e) { std::cout << "  Khong tim thay!\n"; continue; }
            std::string field, val;
            std::cout << "  Truong can sua (name/phone/email/dept/pos/rate): ";
            std::getline(std::cin, field);
            std::cout << "  Gia tri moi: "; std::getline(std::cin, val);
            if (field == "name")  e->setFullName(val);
            else if (field == "phone") e->setPhone(val);
            else if (field == "email") e->setEmail(val);
            else if (field == "dept")  e->setDepartmentId(val);
            else if (field == "pos")   e->setPositionId(val);
            else if (field == "rate")  e->setBaseSalaryRate(std::stod(val));
            savEmployees();
            std::cout << "  Da cap nhat!\n";
        } else if (choice == 4) {
            std::string id;
            std::cout << "  Ma NV nghi viec: "; std::getline(std::cin, id);
            removeEmployee(id);
        } else if (choice == 5) {
            std::string name;
            std::cout << "  Nhap ten tim kiem: "; std::getline(std::cin, name);
            auto res = searchByName(name);
            std::cout << "  Tim thay " << res.size() << " ket qua:\n";
            for (auto* e : res) e->displayInfo(), std::cout << std::string(40, '-') << "\n";
        } else if (choice == 6) {
            std::string deptId;
            std::cout << "  Ma phong ban: "; std::getline(std::cin, deptId);
            auto res = searchByDepartment(deptId);
            std::cout << "  Tim thay " << res.size() << " nhan vien:\n";
            for (auto* e : res) std::cout << "  - " << e->getEmployeeId() << " | " << e->getFullName() << "\n";
        } else if (choice == 7) {
            std::string id;
            std::cout << "  Ma NV: "; std::getline(std::cin, id);
            Employee* e = getEmployeeById(id);
            if (e) e->displayInfo();
            else std::cout << "  Khong tim thay!\n";
        }
    }
}

void EmployeeManager::menuManageDepartments() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n===== QUAN LY PHONG BAN =====\n"
                  << "  1. Xem danh sach phong ban\n"
                  << "  2. Them phong ban moi\n"
                  << "  3. Xoa phong ban\n"
                  << "  0. Quay lai\n"
                  << "Chon: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice == 1) {
            for (const auto& d : departments) d.displayInfo(), std::cout << std::string(40,'-') << "\n";
        } else if (choice == 2) {
            std::string id, name, mgr;
            std::cout << "  Ma PB: "; std::getline(std::cin, id);
            std::cout << "  Ten PB: "; std::getline(std::cin, name);
            std::cout << "  Ma truong phong: "; std::getline(std::cin, mgr);
            addDepartment(Department(id, name, mgr));
            std::cout << "  Da them!\n";
        } else if (choice == 3) {
            std::string id;
            std::cout << "  Ma PB can xoa: "; std::getline(std::cin, id);
            if (removeDepartment(id)) std::cout << "  Da xoa!\n";
            else std::cout << "  Khong tim thay!\n";
        }
    }
}

void EmployeeManager::menuManageAccounts() {
    if (!authManager) { std::cout << "  Loi: AuthManager chua khoi tao!\n"; return; }
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n===== TAO TAI KHOAN =====\n"
                  << "  1. Tao tai khoan moi\n"
                  << "  2. Xoa tai khoan\n"
                  << "  3. Xem danh sach tai khoan\n"
                  << "  0. Quay lai\n"
                  << "Chon: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice == 1) {
            std::string user, pwd, roleStr, empId;
            std::cout << "  Ten dang nhap: "; std::getline(std::cin, user);
            std::cout << "  Mat khau: ";      std::getline(std::cin, pwd);
            std::cout << "  Quyen (Admin/Manager/Employee): "; std::getline(std::cin, roleStr);
            std::cout << "  Ma nhan vien: "; std::getline(std::cin, empId);
            Account acc(user, Account::hashPassword(pwd), Account::stringToRole(roleStr), empId);
            if (authManager->addAccount(acc)) std::cout << "  Tao tai khoan thanh cong!\n";
        } else if (choice == 2) {
            std::string user;
            std::cout << "  Ten tai khoan can xoa: "; std::getline(std::cin, user);
            if (authManager->removeAccount(user)) std::cout << "  Da xoa!\n";
            else std::cout << "  Khong tim thay!\n";
        } else if (choice == 3) {
            for (const auto& acc : authManager->getAccounts())
                std::cout << "  " << acc.getUsername() << " | " << acc.roleToString()
                          << " | EmpID: " << acc.getEmployeeId() << "\n";
        }
    }
}

void EmployeeManager::displayMenu() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n========= QUAN LY NHAN SU =========\n"
                  << "  1. Quan ly nhan vien\n"
                  << "  2. Quan ly phong ban\n"
                  << "  3. Quan ly chuc vu\n"
                  << "  4. Quan ly tai khoan\n"
                  << "  0. Quay lai\n"
                  << "Chon: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice) {
        case 1: menuManageEmployees(); break;
        case 2: menuManageDepartments(); break;
        case 3: {
            int c2 = -1;
            while (c2 != 0) {
                std::cout << "\n===== QUAN LY CHUC VU =====\n"
                          << "  1. Xem danh sach\n  2. Them moi\n  3. Xoa\n  0. Quay lai\nChon: ";
                std::cin >> c2;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (c2 == 1) {
                    for (const auto& p : positions) p.displayInfo(), std::cout << std::string(40,'-') << "\n";
                } else if (c2 == 2) {
                    std::string id, name, coeff;
                    std::cout << "  Ma CV: "; std::getline(std::cin, id);
                    std::cout << "  Ten CV: "; std::getline(std::cin, name);
                    std::cout << "  He so phu cap: "; std::getline(std::cin, coeff);
                    addPosition(Position(id, name, std::stod(coeff)));
                    std::cout << "  Da them!\n";
                } else if (c2 == 3) {
                    std::string id;
                    std::cout << "  Ma CV: "; std::getline(std::cin, id);
                    if (removePosition(id)) std::cout << "  Da xoa!\n";
                    else std::cout << "  Khong tim thay!\n";
                }
            }
            break;
        }
        case 4: menuManageAccounts(); break;
        }
    }
}
