#include "Candidate.h"
#include <iostream>
#include <sstream>

Candidate::Candidate(const std::string& cId, const std::string& personId,
                     const std::string& name, const std::string& gender,
                     const Date& birth, const std::string& phone, const std::string& email,
                     const std::string& appliedPos, double score)
    : Person(personId, name, gender, birth, phone, email),
      candidateId(cId), appliedPosition(appliedPos),
      status(CandidateStatus::Applied), interviewScore(score) {}

std::string Candidate::statusToString() const {
    switch (status) {
    case CandidateStatus::Applied:   return "Applied";
    case CandidateStatus::Interview: return "Interview";
    case CandidateStatus::Passed:    return "Passed";
    case CandidateStatus::Rejected:  return "Rejected";
    default: return "Applied";
    }
}

CandidateStatus Candidate::stringToStatus(const std::string& s) {
    if (s == "Interview") return CandidateStatus::Interview;
    if (s == "Passed")    return CandidateStatus::Passed;
    if (s == "Rejected")  return CandidateStatus::Rejected;
    return CandidateStatus::Applied;
}

void Candidate::displayInfo() const {
    std::cout << "  Ma UV       : " << candidateId      << "\n"
              << "  Ho va ten   : " << fullName          << "\n"
              << "  Gioi tinh   : " << gender            << "\n"
              << "  Ngay sinh   : " << birthDate.toString() << "\n"
              << "  Dien thoai  : " << phone             << "\n"
              << "  Email       : " << email             << "\n"
              << "  Vi tri DU   : " << appliedPosition   << "\n"
              << "  Diem PV     : " << interviewScore    << "\n"
              << "  Trang thai  : " << statusToString()  << "\n";
}

std::string Candidate::toCSV() const {
    return candidateId + "," + fullName + "," + gender + "," +
           birthDate.toString() + "," + phone + "," + email + "," +
           appliedPosition + "," + std::to_string(interviewScore) + "," + statusToString();
}

Candidate Candidate::fromCSV(const std::string& line) {
    std::istringstream ss(line);
    std::string cId, name, gender, birth, phone, email, pos, score, status;
    std::getline(ss, cId,    ',');
    std::getline(ss, name,   ',');
    std::getline(ss, gender, ',');
    std::getline(ss, birth,  ',');
    std::getline(ss, phone,  ',');
    std::getline(ss, email,  ',');
    std::getline(ss, pos,    ',');
    std::getline(ss, score,  ',');
    std::getline(ss, status, ',');
    double sc = score.empty() ? 0.0 : std::stod(score);
    Candidate c(cId, cId, name, gender, Date::fromString(birth), phone, email, pos, sc);
    c.status = stringToStatus(status);
    return c;
}
