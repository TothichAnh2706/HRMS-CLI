#include "Date.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>

std::string Date::toString() const {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << day << "/"
        << std::setw(2) << month << "/"
        << year;
    return oss.str();
}

Date Date::fromString(const std::string& str) {
    // Expected format: DD/MM/YYYY
    Date d;
    if (str.size() >= 10) {
        try {
            d.day   = std::stoi(str.substr(0, 2));
            d.month = std::stoi(str.substr(3, 2));
            d.year  = std::stoi(str.substr(6, 4));
        } catch (...) {
            d = Date{};
        }
    }
    return d;
}

bool Date::isValid() const {
    if (month < 1 || month > 12) return false;
    if (day < 1) return false;
    int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    // Leap year
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        daysInMonth[1] = 29;
    return day <= daysInMonth[month - 1];
}

bool Date::operator<(const Date& o) const {
    if (year != o.year)  return year < o.year;
    if (month != o.month)return month < o.month;
    return day < o.day;
}

bool Date::operator<=(const Date& o) const {
    return !(o < *this);
}

bool Date::operator==(const Date& o) const {
    return day == o.day && month == o.month && year == o.year;
}

// Approximate: counts calendar days from *this to other (other - this)
int Date::daysBetween(const Date& other) const {
    auto toJulian = [](int y, int m, int d) -> long {
        int a = (14 - m) / 12;
        int yy = y + 4800 - a;
        int mm = m + 12 * a - 3;
        return d + (153 * mm + 2) / 5 + 365L * yy + yy / 4 - yy / 100 + yy / 400 - 32045;
    };
    long jd1 = toJulian(year, month, day);
    long jd2 = toJulian(other.year, other.month, other.day);
    return static_cast<int>(jd2 - jd1);
}
