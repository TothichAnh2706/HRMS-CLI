#include "Feedback.h"
#include <iostream>
#include <sstream>

Feedback::Feedback() : status(FeedbackStatus::Open) {}

Feedback::Feedback(const std::string& fId, const std::string& empId, const std::string& content)
    : feedbackId(fId), employeeId(empId), content(content), status(FeedbackStatus::Open) {}

std::string Feedback::statusToString() const {
    switch (status) {
    case FeedbackStatus::Open:     return "Open";
    case FeedbackStatus::InReview: return "InReview";
    case FeedbackStatus::Resolved: return "Resolved";
    default: return "Open";
    }
}

FeedbackStatus Feedback::stringToStatus(const std::string& s) {
    if (s == "InReview") return FeedbackStatus::InReview;
    if (s == "Resolved") return FeedbackStatus::Resolved;
    return FeedbackStatus::Open;
}

void Feedback::displayInfo() const {
    std::cout << "  ID GY     : " << feedbackId << "\n"
              << "  NV        : " << employeeId  << "\n"
              << "  Noi dung  : " << content     << "\n"
              << "  Trang thai: " << statusToString() << "\n"
              << "  Phan hoi  : " << (response.empty() ? "(Chua co)" : response) << "\n";
}

std::string Feedback::toCSV() const {
    // Replace commas in content/response with semicolons to avoid CSV issues
    auto sanitize = [](const std::string& s) {
        std::string r = s;
        for (auto& c : r) if (c == ',') c = ';';
        return r;
    };
    return feedbackId + "," + employeeId + "," + sanitize(content) + "," +
           sanitize(response) + "," + statusToString();
}

Feedback Feedback::fromCSV(const std::string& line) {
    std::istringstream ss(line);
    std::string fId, empId, content, resp, status;
    std::getline(ss, fId,     ',');
    std::getline(ss, empId,   ',');
    std::getline(ss, content, ',');
    std::getline(ss, resp,    ',');
    std::getline(ss, status,  ',');
    Feedback fb(fId, empId, content);
    fb.response = resp;
    fb.status = stringToStatus(status);
    return fb;
}
