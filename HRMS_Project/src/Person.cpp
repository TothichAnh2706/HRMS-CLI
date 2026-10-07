#include "Person.h"

Person::Person(const std::string& id, const std::string& name, const std::string& gender,
               const Date& birth, const std::string& phone, const std::string& email)
    : id(id), fullName(name), gender(gender), birthDate(birth), phone(phone), email(email)
{}
