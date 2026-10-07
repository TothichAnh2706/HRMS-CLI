#ifndef CANDIDATE_H
#define CANDIDATE_H

#include "Person.h"
#include <string>

enum class CandidateStatus { Applied, Interview, Passed, Rejected };

class Candidate : public Person {
private:
    std::string     candidateId;
    std::string     appliedPosition;   // positionId
    CandidateStatus status;
    double          interviewScore;    // 0-100

public:
    Candidate() : status(CandidateStatus::Applied), interviewScore(0.0) {}
    Candidate(const std::string& cId, const std::string& personId,
              const std::string& name, const std::string& gender,
              const Date& birth, const std::string& phone, const std::string& email,
              const std::string& appliedPos, double score = 0.0);

    std::string     getCandidateId()     const { return candidateId; }
    std::string     getAppliedPosition() const { return appliedPosition; }
    CandidateStatus getStatus()          const { return status; }
    double          getInterviewScore()  const { return interviewScore; }

    void setCandidateId(const std::string& v)    { candidateId = v; }
    void setAppliedPosition(const std::string& v){ appliedPosition = v; }
    void setStatus(CandidateStatus v)             { status = v; }
    void setInterviewScore(double v)              { interviewScore = v; }

    std::string statusToString() const;
    static CandidateStatus stringToStatus(const std::string& s);

    void displayInfo() const override;
    std::string toCSV() const;
    static Candidate fromCSV(const std::string& line);
};

#endif // CANDIDATE_H
