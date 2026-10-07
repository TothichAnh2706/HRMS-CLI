#ifndef RECRUITMENT_PLAN_H
#define RECRUITMENT_PLAN_H

#include "Date.h"
#include <string>

class RecruitmentPlan {
private:
    std::string planId;
    std::string positionId;
    int         targetQuantity;
    Date        deadline;
    int         currentHired;

public:
    RecruitmentPlan();
    RecruitmentPlan(const std::string& pid, const std::string& posId,
                    int qty, const Date& dl);

    std::string getPlanId()        const { return planId; }
    std::string getPositionId()    const { return positionId; }
    int         getTargetQuantity()const { return targetQuantity; }
    Date        getDeadline()      const { return deadline; }
    int         getCurrentHired()  const { return currentHired; }

    void incrementHired() { currentHired++; }

    void displayInfo() const;
    std::string toCSV() const;
    static RecruitmentPlan fromCSV(const std::string& line);
};

#endif // RECRUITMENT_PLAN_H
