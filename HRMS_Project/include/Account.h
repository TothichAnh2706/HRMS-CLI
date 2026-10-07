#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>

enum class Role { Admin, Manager, Employee };

class Account {
private:
    std::string username;
    std::string passwordHash;  // SHA256 or simple hash
    Role        role;
    std::string employeeId;
    int         failedAttempts;

public:
    Account() : role(Role::Employee), failedAttempts(0) {}
    Account(const std::string& user, const std::string& pwdHash,
            Role r, const std::string& empId);

    std::string getUsername()    const { return username; }
    std::string getPasswordHash()const { return passwordHash; }
    Role        getRole()        const { return role; }
    std::string getEmployeeId()  const { return employeeId; }
    int         getFailedAttempts()const{ return failedAttempts; }

    void setUsername(const std::string& v)    { username = v; }
    void setPasswordHash(const std::string& v){ passwordHash = v; }
    void setRole(Role v)                      { role = v; }
    void setEmployeeId(const std::string& v)  { employeeId = v; }
    void incrementFailedAttempts()            { failedAttempts++; }
    void resetFailedAttempts()                { failedAttempts = 0; }

    std::string roleToString() const;
    static Role stringToRole(const std::string& s);

    std::string toCSV() const;
    static Account fromCSV(const std::string& line);

    // Simple hash (for demo – not production-grade)
    static std::string hashPassword(const std::string& password);
};

#endif // ACCOUNT_H
