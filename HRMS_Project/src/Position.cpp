#include "Position.h"
#include <iostream>
#include <sstream>

Position::Position(const std::string& id, const std::string& name, double coeff)
    : positionId(id), positionName(name), allowanceCoefficient(coeff) {}

void Position::displayInfo() const {
    std::cout << "  Chuc vu ID     : " << positionId            << "\n"
              << "  Ten chuc vu    : " << positionName           << "\n"
              << "  He so phu cap  : " << allowanceCoefficient   << "\n";
}

std::string Position::toCSV() const {
    return positionId + "," + positionName + "," + std::to_string(allowanceCoefficient);
}

Position Position::fromCSV(const std::string& line) {
    std::istringstream ss(line);
    std::string id, name, coeff;
    std::getline(ss, id,    ',');
    std::getline(ss, name,  ',');
    std::getline(ss, coeff, ',');
    double c = coeff.empty() ? 0.0 : std::stod(coeff);
    return Position(id, name, c);
}
