#ifndef REPORT_MANAGER_H
#define REPORT_MANAGER_H

#include "BaseManager.h"
#include "Candidate.h"
#include "RecruitmentPlan.h"
#include "TrainingCourse.h"
#include "Feedback.h"
#include <vector>
#include <string>
#include <map>

class EmployeeManager;
class AttendanceManager;
class PayrollManager;

class ReportManager : public BaseManager {
private:
    std::vector<Candidate>       candidates;
    std::vector<RecruitmentPlan> recruitmentPlans;
    std::vector<TrainingCourse>  trainingCourses;
    std::vector<Feedback>        feedbacks;

    EmployeeManager*   employeeManager;
    AttendanceManager* attendanceManager;
    PayrollManager*    payrollManager;

    std::string candidateFile;
    std::string feedbackFile;

    void loadCandidates();
    void saveCandidates();
    void loadFeedbacks();
    void saveFeedbacks();

public:
    ReportManager(EmployeeManager* empMgr = nullptr,
                  AttendanceManager* attMgr = nullptr,
                  PayrollManager* payMgr = nullptr);
    ~ReportManager() override = default;

    void loadFromFile() override;
    void saveToFile() override;
    void displayMenu() override;

    // --- Candidate management ---
    bool addCandidate(const Candidate& c);
    bool updateCandidateStatus(const std::string& cId, CandidateStatus status, double score = -1.0);
    bool convertToEmployee(const std::string& cId);  // Candidate -> Employee
    Candidate* getCandidateById(const std::string& cId);
    std::vector<Candidate>& getAllCandidates() { return candidates; }

    // --- Recruitment plan ---
    bool addRecruitmentPlan(const RecruitmentPlan& plan);
    std::vector<RecruitmentPlan>& getRecruitmentPlans() { return recruitmentPlans; }

    // --- Training ---
    bool addTrainingCourse(const TrainingCourse& course);
    bool enrollEmployeeInCourse(const std::string& courseId, const std::string& empId);
    std::vector<TrainingCourse>& getTrainingCourses() { return trainingCourses; }

    // --- Feedback ---
    bool submitFeedback(const Feedback& fb);
    bool respondToFeedback(const std::string& fbId, const std::string& response);
    std::vector<Feedback>& getAllFeedbacks() { return feedbacks; }

    // --- Reports ---
    void reportHeadcountByDepartment() const;
    void reportPayrollTrend(int months) const;
    void reportLateAttendance(int month, int year) const;
    void reportCandidatePipeline() const;

    // --- Sub-menus ---
    void menuRecruitment();
    void menuTraining();
    void menuReports();
    void menuFeedback(const std::string& empId);
    void menuAdminFeedback();

    std::string generateNextCandidateId() const;
    std::string generateNextCourseId() const;
    std::string generateNextPlanId() const;
    std::string generateNextFeedbackId() const;
};

#endif // REPORT_MANAGER_H
