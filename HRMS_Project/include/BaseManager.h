#ifndef BASE_MANAGER_H
#define BASE_MANAGER_H

class BaseManager {
public:
    virtual ~BaseManager() = default;
    virtual void loadFromFile() = 0;   // Pure virtual function
    virtual void saveToFile() = 0;     // Pure virtual function
    virtual void displayMenu() = 0;    // Pure virtual function
};

#endif // BASE_MANAGER_H
