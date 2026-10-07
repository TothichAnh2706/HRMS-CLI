#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "Person.h"
#include <string>

class Employee : public Person {
private:
    std::string employeeId;
    std::string departmentId;
    std::string positionId;
    std::string status;        // "Active" / "Resigned"
    double      baseSalaryRate;// Hệ số lương cơ bản

public:
    Employee() : status("Active"), baseSalaryRate(1.0) {}
    Employee(const std::string& empId,
             const std::string& personId,
             const std::string& name,
             const std::string& gender,
             const Date& birth,
             const std::string& phone,
             const std::string& email,
             const std::string& deptId,
             const std::string& posId,
             const std::string& status,
             double salaryRate);

    // Getters
    std::string getEmployeeId()   const { return employeeId; }
    std::string getDepartmentId() const { return departmentId; }
    std::string getPositionId()   const { return positionId; }
    std::string getStatus()       const { return status; }
    double      getBaseSalaryRate()const{ return baseSalaryRate; }

    // Setters
    void setEmployeeId(const std::string& v)   { employeeId = v; }
    void setDepartmentId(const std::string& v) { departmentId = v; }
    void setPositionId(const std::string& v)   { positionId = v; }
    void setStatus(const std::string& v)       { status = v; }
    void setBaseSalaryRate(double v)           { baseSalaryRate = v; }

    // Override pure virtual
    void displayInfo() const override;

    // CSV serialization
    std::string toCSV() const;
    static Employee fromCSV(const std::string& line);
};

#endif // EMPLOYEE_H
