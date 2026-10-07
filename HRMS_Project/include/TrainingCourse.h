#ifndef TRAINING_COURSE_H
#define TRAINING_COURSE_H

#include "Date.h"
#include <string>
#include <vector>

class TrainingCourse {
private:
    std::string              courseId;
    std::string              courseName;
    Date                     startDate;
    Date                     endDate;
    std::string              instructor;
    std::vector<std::string> listEmployeeIds;

public:
    TrainingCourse() = default;
    TrainingCourse(const std::string& cId, const std::string& name,
                   const Date& start, const Date& end, const std::string& instructor);

    std::string getCourseId()   const { return courseId; }
    std::string getCourseName() const { return courseName; }
    Date        getStartDate()  const { return startDate; }
    Date        getEndDate()    const { return endDate; }
    std::string getInstructor() const { return instructor; }
    const std::vector<std::string>& getEmployeeIds() const { return listEmployeeIds; }

    void enrollEmployee(const std::string& empId);
    void unenrollEmployee(const std::string& empId);
    bool isEnrolled(const std::string& empId) const;

    void displayInfo() const;
    std::string toCSV() const;
    static TrainingCourse fromCSV(const std::string& line);
};

#endif // TRAINING_COURSE_H
