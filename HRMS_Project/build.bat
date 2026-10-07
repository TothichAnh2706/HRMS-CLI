@echo off
echo ================================================
echo  Build HRMS Project - C++ OOP
echo ================================================

set SRC_DIR=src
set INC_DIR=include
set OUT=hrms.exe

echo Dang bien dich...

g++ -std=c++14 -Wall -Wextra -o %OUT% ^
    main.cpp ^
    %SRC_DIR%\Date.cpp ^
    %SRC_DIR%\Person.cpp ^
    %SRC_DIR%\Employee.cpp ^
    %SRC_DIR%\Department.cpp ^
    %SRC_DIR%\Position.cpp ^
    %SRC_DIR%\Account.cpp ^
    %SRC_DIR%\AuthManager.cpp ^
    %SRC_DIR%\EmployeeManager.cpp ^
    %SRC_DIR%\WorkShift.cpp ^
    %SRC_DIR%\TimekeepingRecord.cpp ^
    %SRC_DIR%\LeaveRequest.cpp ^
    %SRC_DIR%\AttendanceManager.cpp ^
    %SRC_DIR%\Contract.cpp ^
    %SRC_DIR%\RewardDiscipline.cpp ^
    %SRC_DIR%\SalaryRecord.cpp ^
    %SRC_DIR%\PayrollManager.cpp ^
    %SRC_DIR%\Candidate.cpp ^
    %SRC_DIR%\RecruitmentPlan.cpp ^
    %SRC_DIR%\TrainingCourse.cpp ^
    %SRC_DIR%\Feedback.cpp ^
    %SRC_DIR%\ReportManager.cpp ^
    -I%INC_DIR%

if %ERRORLEVEL% == 0 (
    echo.
    echo ================================================
    echo  Build THANH CONG! File: %OUT%
    echo  Chay chuong trinh: hrms.exe
    echo ================================================
) else (
    echo.
    echo ================================================
    echo  Build THAT BAI! Kiem tra loi tren.
    echo ================================================
)
pause
