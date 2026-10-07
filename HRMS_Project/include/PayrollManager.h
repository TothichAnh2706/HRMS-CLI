#ifndef PAYROLL_MANAGER_H
#define PAYROLL_MANAGER_H

#include "BaseManager.h"
#include "SalaryRecord.h"
#include "Contract.h"
#include "RewardDiscipline.h"
#include <vector>
#include <string>

class EmployeeManager;
class AttendanceManager;

class PayrollManager : public BaseManager {
private:
    std::vector<SalaryRecord>    salaryRecords;
    std::vector<Contract>        contracts;
    std::vector<RewardDiscipline>rdRecords;

    EmployeeManager*   employeeManager;
    AttendanceManager* attendanceManager;

    std::string contractFile;
    std::string rdFile;

    double calculateBHXH(double baseSalary) const;
    double calculateTax(double taxableIncome) const;  // Progressive tax brackets

    void loadContracts();
    void saveContracts();
    void loadRDRecords();
    void saveRDRecords();
    void loadPayroll();
    void savePayroll(int month, int year);

public:
    PayrollManager(EmployeeManager* empMgr = nullptr,
                   AttendanceManager* attMgr = nullptr);
    ~PayrollManager() override = default;

    void loadFromFile() override;
    void saveToFile() override;
    void displayMenu() override;

    // --- Payroll calculation ---
    SalaryRecord calculateSalary(const std::string& empId, int month, int year);
    void runPayroll(int month, int year);   // Calculate all employees
    bool recalculateSalary(const std::string& recordId);

    // --- Contract management ---
    bool addContract(const Contract& c);
    bool updateContract(const std::string& cId, const Contract& updated);
    Contract* getActiveContractByEmployee(const std::string& empId);
    std::vector<Contract> getContractsByEmployee(const std::string& empId) const;

    // --- RewardDiscipline management ---
    bool addRDRecord(const RewardDiscipline& rd);
    double getTotalBonusForMonth(const std::string& empId, int month, int year) const;
    double getTotalPenaltyForMonth(const std::string& empId, int month, int year) const;

    // --- Dispute ---
    bool flagDispute(const std::string& recordId);
    std::vector<SalaryRecord> getDisputedRecords() const;

    // --- API for ReportManager ---
    double getTotalPayrollByMonth(int month, int year) const;

    SalaryRecord* getSalaryRecord(const std::string& empId, int month, int year);
    std::vector<SalaryRecord>& getAllSalaryRecords() { return salaryRecords; }

    void menuManageContracts();
    void menuManagePayroll();
    void menuHandleDisputes();
};

#endif // PAYROLL_MANAGER_H
