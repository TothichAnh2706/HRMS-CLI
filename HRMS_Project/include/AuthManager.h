#ifndef AUTH_MANAGER_H
#define AUTH_MANAGER_H

#include "Account.h"
#include "BaseManager.h"
#include <vector>
#include <string>

class AuthManager : public BaseManager {
private:
    std::vector<Account> accounts;
    Account* currentUser;       // Pointer to logged-in account (nullptr if logged out)
    std::string dataFile;

public:
    AuthManager();
    ~AuthManager() override = default;

    void loadFromFile() override;
    void saveToFile() override;
    void displayMenu() override;

    // Auth operations
    bool login(const std::string& username, const std::string& password);
    void logout();
    bool changePassword(const std::string& oldPwd, const std::string& newPwd);
    bool isLoggedIn() const { return currentUser != nullptr; }
    Account* getCurrentUser() const { return currentUser; }

    // Account management
    bool addAccount(const Account& acc);
    bool removeAccount(const std::string& username);
    Account* findAccount(const std::string& username);
    bool hasPermission(Role requiredRole) const;

    const std::vector<Account>& getAccounts() const { return accounts; }
};

#endif // AUTH_MANAGER_H
