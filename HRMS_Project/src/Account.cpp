#include "Account.h"
#include <sstream>
#include <functional>
#include <string>

Account::Account(const std::string& user, const std::string& pwdHash,
                 Role r, const std::string& empId)
    : username(user), passwordHash(pwdHash), role(r), employeeId(empId), failedAttempts(0)
{}

std::string Account::roleToString() const {
    switch (role) {
        case Role::Admin:    return "Admin";
        case Role::Manager:  return "Manager";
        case Role::Employee: return "Employee";
        default:             return "Employee";
    }
}

Role Account::stringToRole(const std::string& s) {
    if (s == "Admin")   return Role::Admin;
    if (s == "Manager") return Role::Manager;
    return Role::Employee;
}

// Simple hash using std::hash<string> (demo purposes only)
std::string Account::hashPassword(const std::string& password) {
    std::hash<std::string> hasher;
    return std::to_string(hasher(password));
}

std::string Account::toCSV() const {
    return username + "," + passwordHash + "," + roleToString() + "," + employeeId;
}

Account Account::fromCSV(const std::string& line) {
    std::istringstream ss(line);
    std::string user, pwd, roleStr, empId;
    std::getline(ss, user,    ',');
    std::getline(ss, pwd,     ',');
    std::getline(ss, roleStr, ',');
    std::getline(ss, empId,   ',');
    return Account(user, pwd, stringToRole(roleStr), empId);
}
