#include "AuthManager.h"
#include <fstream>
#include <iostream>
#include <limits>

AuthManager::AuthManager() : currentUser(nullptr), dataFile("data/accounts.csv") {}

void AuthManager::loadFromFile() {
    accounts.clear();
    std::ifstream file(dataFile);
    if (!file.is_open()) return;
    std::string line;
    std::getline(file, line); // skip header
    while (std::getline(file, line)) {
        if (!line.empty())
            accounts.push_back(Account::fromCSV(line));
    }
}

void AuthManager::saveToFile() {
    std::ofstream file(dataFile);
    file << "Username,PasswordHash,Role,EmployeeID\n";
    for (const auto& acc : accounts)
        file << acc.toCSV() << "\n";
}

bool AuthManager::login(const std::string& username, const std::string& password) {
    Account* acc = findAccount(username);
    if (!acc) {
        std::cout << "  Tai khoan khong ton tai!\n";
        return false;
    }
    if (acc->getFailedAttempts() >= 3) {
        std::cout << "  Tai khoan bi khoa do nhap sai qua 3 lan!\n";
        return false;
    }
    std::string hashed = Account::hashPassword(password);
    if (acc->getPasswordHash() != hashed) {
        acc->incrementFailedAttempts();
        std::cout << "  Sai mat khau! Con " << (3 - acc->getFailedAttempts()) << " lan thu.\n";
        return false;
    }
    acc->resetFailedAttempts();
    currentUser = acc;
    std::cout << "  Dang nhap thanh cong! Xin chao, " << username
              << " [" << acc->roleToString() << "]\n";
    return true;
}

void AuthManager::logout() {
    currentUser = nullptr;
    std::cout << "  Da dang xuat.\n";
}

bool AuthManager::changePassword(const std::string& oldPwd, const std::string& newPwd) {
    if (!currentUser) return false;
    if (currentUser->getPasswordHash() != Account::hashPassword(oldPwd)) {
        std::cout << "  Mat khau cu khong dung!\n";
        return false;
    }
    currentUser->setPasswordHash(Account::hashPassword(newPwd));
    saveToFile();
    std::cout << "  Doi mat khau thanh cong!\n";
    return true;
}

bool AuthManager::addAccount(const Account& acc) {
    if (findAccount(acc.getUsername())) {
        std::cout << "  Ten dang nhap da ton tai!\n";
        return false;
    }
    accounts.push_back(acc);
    saveToFile();
    return true;
}

bool AuthManager::removeAccount(const std::string& username) {
    for (auto it = accounts.begin(); it != accounts.end(); ++it) {
        if (it->getUsername() == username) {
            accounts.erase(it);
            saveToFile();
            return true;
        }
    }
    return false;
}

Account* AuthManager::findAccount(const std::string& username) {
    for (auto& acc : accounts)
        if (acc.getUsername() == username) return &acc;
    return nullptr;
}

bool AuthManager::hasPermission(Role requiredRole) const {
    if (!currentUser) return false;
    return static_cast<int>(currentUser->getRole()) <= static_cast<int>(requiredRole);
}

void AuthManager::displayMenu() {
    int choice = 0;
    do {
        std::cout << "\n========= QUAN LY TAI KHOAN =========\n"
                  << "  1. Doi mat khau\n"
                  << "  2. Dang xuat\n"
                  << "  0. Quay lai\n"
                  << "Chon: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice) {
        case 1: {
            std::string oldPwd, newPwd;
            std::cout << "  Mat khau cu: "; std::getline(std::cin, oldPwd);
            std::cout << "  Mat khau moi: "; std::getline(std::cin, newPwd);
            changePassword(oldPwd, newPwd);
            break;
        }
        case 2:
            logout();
            choice = 0;
            break;
        }
    } while (choice != 0);
}
