#ifndef REWARD_DISCIPLINE_H
#define REWARD_DISCIPLINE_H

#include "Date.h"
#include <string>

enum class RDType { Reward, Discipline };

class RewardDiscipline {
private:
    std::string rdId;
    std::string employeeId;
    RDType      rdType;
    double      amount;     // Positive = bonus, negative = penalty
    std::string reason;
    Date        date;

public:
    RewardDiscipline();
    RewardDiscipline(const std::string& id, const std::string& empId,
                     RDType type, double amount, const std::string& reason, const Date& d);

    std::string getRdId()       const { return rdId; }
    std::string getEmployeeId() const { return employeeId; }
    RDType      getRdType()     const { return rdType; }
    double      getAmount()     const { return amount; }
    std::string getReason()     const { return reason; }
    Date        getDate()       const { return date; }

    std::string rdTypeToString() const;
    static RDType stringToRDType(const std::string& s);

    void displayInfo() const;
    std::string toCSV() const;
    static RewardDiscipline fromCSV(const std::string& line);
};

#endif // REWARD_DISCIPLINE_H
