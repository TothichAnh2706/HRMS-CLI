#ifndef EMPLOYEE_MANAGER_H
#define EMPLOYEE_MANAGER_H

#include "BaseManager.h"
#include "Employee.h"
#include "Department.h"
#include "Position.h"
#include "AuthManager.h"
#include <vector>
#include <string>

class EmployeeManager : public BaseManager {
private:
    std::vector<Employee>   employees;
    std::vector<Department> departments;
    std::vector<Position>   positions;
    AuthManager*            authManager;

    std::string empFile;
    std::string deptFile;
    std::string posFile;

    void loadEmployees();
    void savEmployees();
    void loadDepartments();
    void saveDepartments();
    void loadPositions();
    void savePositions();

public:
    explicit EmployeeManager(AuthManager* auth = nullptr);
    ~EmployeeManager() override = default;

    void loadFromFile() override;
    void saveToFile() override;
    void displayMenu() override;

    // --- Employee CRUD ---
    bool addEmployee(const Employee& emp);
    bool updateEmployee(const std::string& empId, const Employee& updated);
    bool removeEmployee(const std::string& empId);   // Sets status to Resigned
    Employee* getEmployeeById(const std::string& empId);
    bool      isEmployeeExist(const std::string& empId) const;

    std::vector<Employee*> searchByName(const std::string& name);
    std::vector<Employee*> searchByDepartment(const std::string& deptId);
    std::vector<Employee>& getAllEmployees() { return employees; }

    // --- Department CRUD ---
    bool addDepartment(const Department& dept);
    bool removeDepartment(const std::string& deptId);
    Department* getDepartmentById(const std::string& deptId);
    std::vector<Department>& getAllDepartments() { return departments; }

    // --- Position CRUD ---
    bool addPosition(const Position& pos);
    bool removePosition(const std::string& posId);
    Position* getPositionById(const std::string& posId);
    std::vector<Position>& getAllPositions() { return positions; }

    // --- Sub-menus ---
    void menuManageEmployees();
    void menuManageDepartments();
    void menuManageAccounts();
    void printEmployeeList() const;

    std::string generateNextEmpId() const;
};

#endif // EMPLOYEE_MANAGER_H
