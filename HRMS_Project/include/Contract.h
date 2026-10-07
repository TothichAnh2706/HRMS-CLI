#ifndef CONTRACT_H
#define CONTRACT_H

#include "Date.h"
#include <string>

enum class ContractType { Probation, Official, PartTime, Internship };

class Contract {
private:
    std::string  contractId;
    std::string  employeeId;
    ContractType contractType;
    Date         signDate;
    Date         expiredDate;
    double       baseSalary;    // VND per month
    bool         isActive;

public:
    Contract();
    Contract(const std::string& cId, const std::string& empId,
             ContractType type, const Date& sign, const Date& exp, double salary);

    std::string  getContractId()   const { return contractId; }
    std::string  getEmployeeId()   const { return employeeId; }
    ContractType getContractType() const { return contractType; }
    Date         getSignDate()     const { return signDate; }
    Date         getExpiredDate()  const { return expiredDate; }
    double       getBaseSalary()   const { return baseSalary; }
    bool         getIsActive()     const { return isActive; }

    void setBaseSalary(double v) { baseSalary = v; }
    void setIsActive(bool v)     { isActive = v; }

    std::string contractTypeToString() const;
    static ContractType stringToContractType(const std::string& s);

    void displayInfo() const;
    std::string toCSV() const;
    static Contract fromCSV(const std::string& line);
};

#endif // CONTRACT_H
