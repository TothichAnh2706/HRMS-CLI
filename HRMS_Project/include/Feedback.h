#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <string>

enum class FeedbackStatus { Open, InReview, Resolved };

class Feedback {
private:
    std::string    feedbackId;
    std::string    employeeId;
    std::string    content;
    std::string    response;
    FeedbackStatus status;

public:
    Feedback();
    Feedback(const std::string& fId, const std::string& empId, const std::string& content);

    std::string    getFeedbackId() const { return feedbackId; }
    std::string    getEmployeeId()const { return employeeId; }
    std::string    getContent()    const { return content; }
    std::string    getResponse()   const { return response; }
    FeedbackStatus getStatus()     const { return status; }

    void setResponse(const std::string& r){ response = r; status = FeedbackStatus::Resolved; }
    void setStatus(FeedbackStatus s)      { status = s; }

    std::string statusToString() const;
    static FeedbackStatus stringToStatus(const std::string& s);

    void displayInfo() const;
    std::string toCSV() const;
    static Feedback fromCSV(const std::string& line);
};

#endif // FEEDBACK_H
