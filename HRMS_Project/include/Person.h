#ifndef PERSON_H
#define PERSON_H

#include <string>
#include "Date.h"

class Person {
protected:
    std::string id;
    std::string fullName;
    std::string gender;   // "Male" / "Female"
    Date birthDate;
    std::string phone;
    std::string email;

public:
    Person() = default;
    Person(const std::string& id, const std::string& name, const std::string& gender,
           const Date& birth, const std::string& phone, const std::string& email);
    virtual ~Person() = default;

    // Getters
    std::string getId()       const { return id; }
    std::string getFullName() const { return fullName; }
    std::string getGender()   const { return gender; }
    Date        getBirthDate()const { return birthDate; }
    std::string getPhone()    const { return phone; }
    std::string getEmail()    const { return email; }

    // Setters
    void setId(const std::string& v)       { id = v; }
    void setFullName(const std::string& v) { fullName = v; }
    void setGender(const std::string& v)   { gender = v; }
    void setBirthDate(const Date& v)       { birthDate = v; }
    void setPhone(const std::string& v)    { phone = v; }
    void setEmail(const std::string& v)    { email = v; }

    // Pure virtual – every derived class must implement
    virtual void displayInfo() const = 0;
};

#endif // PERSON_H
