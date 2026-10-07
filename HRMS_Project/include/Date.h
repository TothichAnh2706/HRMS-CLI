#ifndef DATE_H
#define DATE_H

#include <string>
#include <iostream>

struct Date {
    int day{1};
    int month{1};
    int year{2000};

    Date() = default;
    Date(int d, int m, int y) : day(d), month(m), year(y) {}

    std::string toString() const;
    static Date fromString(const std::string& str);
    bool isValid() const;
    bool operator<(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator==(const Date& other) const;
    int daysBetween(const Date& other) const;  // Returns other - this in days
};

#endif // DATE_H
