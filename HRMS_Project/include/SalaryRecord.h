#ifndef SALARY_RECORD_H
#define SALARY_RECORD_H

#include <string>

class SalaryRecord {
private:
    std::string recordId;
    std::string employeeId;
    int         month;
    int         year;
    int         workDays;
    double      baseSalary;
    double      allowance;
    double      bonus;
    double      deduction;
    double      bhxh;           // Social insurance (10.5%)
    double      tax;            // Personal income tax
    double      netSalary;
    bool        isDisputed;

public:
    SalaryRecord();
    SalaryRecord(const std::string& recId, const std::string& empId,
                 int month, int year, int workDays,
                 double base, double allow, double bonus, double deduction);

    std::string getRecordId()   const { return recordId; }
    std::string getEmployeeId() const { return employeeId; }
    int  getMonth()      const { return month; }
    int  getYear()       const { return year; }
    int  getWorkDays()   const { return workDays; }
    double getBaseSalary()const{ return baseSalary; }
    double getAllowance() const { return allowance; }
    double getBonus()    const { return bonus; }
    double getDeduction()const { return deduction; }
    double getBhxh()     const { return bhxh; }
    double getTax()      const { return tax; }
    double getNetSalary()const { return netSalary; }
    bool   getIsDisputed()const{ return isDisputed; }

    void setWorkDays(int v)    { workDays = v; }
    void setBonus(double v)    { bonus = v; }
    void setDeduction(double v){ deduction = v; }
    void setIsDisputed(bool v) { isDisputed = v; }

    void calculate();   // Compute bhxh, tax, netSalary

    void displayInfo() const;
    void printPaySlip() const;   // Formatted pay slip
    std::string toCSV() const;
    static SalaryRecord fromCSV(const std::string& line);
};

#endif // SALARY_RECORD_H
