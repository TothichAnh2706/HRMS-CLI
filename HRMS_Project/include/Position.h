#ifndef POSITION_H
#define POSITION_H

#include <string>

class Position {
private:
    std::string positionId;
    std::string positionName;
    double      allowanceCoefficient;  // Hệ số phụ cấp

public:
    Position() : allowanceCoefficient(0.0) {}
    Position(const std::string& id, const std::string& name, double coeff);

    std::string getPositionId()   const { return positionId; }
    std::string getPositionName() const { return positionName; }
    double getAllowanceCoeff()    const { return allowanceCoefficient; }

    void setPositionId(const std::string& v)   { positionId = v; }
    void setPositionName(const std::string& v) { positionName = v; }
    void setAllowanceCoeff(double v)           { allowanceCoefficient = v; }

    void displayInfo() const;
    std::string toCSV() const;
    static Position fromCSV(const std::string& line);
};

#endif // POSITION_H
