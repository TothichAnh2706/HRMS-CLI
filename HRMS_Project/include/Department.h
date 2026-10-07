#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <string>

class Department {
private:
    std::string departmentId;
    std::string departmentName;
    std::string managerId;   // employeeId of department manager

public:
    Department() = default;
    Department(const std::string& id, const std::string& name, const std::string& mgr = "");

    std::string getDepartmentId()   const { return departmentId; }
    std::string getDepartmentName() const { return departmentName; }
    std::string getManagerId()      const { return managerId; }

    void setDepartmentId(const std::string& v)   { departmentId = v; }
    void setDepartmentName(const std::string& v) { departmentName = v; }
    void setManagerId(const std::string& v)      { managerId = v; }

    void displayInfo() const;
    std::string toCSV() const;
    static Department fromCSV(const std::string& line);
};

#endif // DEPARTMENT_H
