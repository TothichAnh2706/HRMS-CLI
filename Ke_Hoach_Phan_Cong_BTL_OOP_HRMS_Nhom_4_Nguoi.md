# **TÀI LIỆU HƯỚNG DẪN VÀ PHÂN CÔNG THỰC HIỆN BÀI TẬP LỚN**

# **HỆ THỐNG QUẢN LÝ NHÂN SỰ (HRMS \- HUMAN RESOURCE MANAGEMENT SYSTEM)**

---

# **1\. TỔNG QUAN DỰ ÁN**

Dự án Xây dựng **Hệ thống Quản lý Nhân sự (HRMS)** theo phương pháp Lập trình Hướng đối tượng (OOP) bằng ngôn ngữ C++. Hệ thống được thiết kế dựa trên mô hình phân lớp nhằm đảm bảo tính mở rộng, bảo trì và phân chia công việc độc lập giữa 4 thành viên trong nhóm.

## **Kiến trúc phân lớp (Layered Architecture)**

* **Model Layer:** Định nghĩa các thực thể dữ liệu (`Person`, `Employee`, `Contract`, `WorkShift`, v.v.).  
* **Service/Manager Layer:** Chứa logic nghiệp vụ, tính toán, xử lý dữ liệu và đọc/ghi tệp (`EmployeeManager`, `PayrollManager`, v.v.).  
* **View/Console UI Layer:** Xử lý hiển thị menu, nhận input từ người dùng và kiểm tra dữ liệu đầu vào.

## **Các nguyên lý OOP cốt lõi áp dụng**

* **Đóng gói (Encapsulation):** Đảm bảo tính an toàn dữ liệu thông qua truy xuất `private`/`protected` và các phương thức Getter/Setter.  
* **Kế thừa (Inheritance):** Thiết lập quan hệ phân cấp giữa các lớp cơ sở (như `Person`, `BaseManager`) và lớp con (`Employee`, `Candidate`, `EmployeeManager`).  
* **Đa hình (Polymorphism):** Sử dụng hàm ảo (`virtual`), hàm thuần ảo (`pure virtual`) cho các phương thức hiển thị, xử lý lưu tệp và tính toán.  
* **Trừu tượng (Abstraction):** Trừu tượng hóa các lớp cơ sở để định nghĩa bộ khung chuẩn cho toàn hệ thống.

---

# **2\. QUY CHUẨN KỸ THUẬT & KIẾN TRÚC DÙNG CHUNG**

## **Quy chuẩn lập trình**

* **Tiêu chuẩn C++:** C++14 trở lên.  
* **Quy ước đặt tên (Naming Conventions):**  
  * **Class / Struct / Enum:** `PascalCase` (ví dụ: `EmployeeManager`, `ContractType`).  
  * **Hàm / Biến:** `camelCase` (ví dụ: `calculateSalary()`, `employeeId`).  
  * **Hằng số:** `UPPER_SNAKE_CASE` (ví dụ: `MAX_ALLOWANCE`, `TAX_BASE_LEVEL`).  
  * **File:** `snake_case` hoặc `PascalCase` đồng nhất (ví dụ: `employee_manager.cpp` / `EmployeeManager.cpp`).

## **Cấu trúc thư mục mã nguồn**

HRMS\_Project/

├── include/              \# Chứa toàn bộ các file header (.h / .hpp)

│   ├── Date.h

│   ├── Person.h

│   ├── Employee.h

│   ├── BaseManager.h

│   └── ...

├── src/                  \# Chứa toàn bộ các file hiện thực logic (.cpp)

│   ├── Date.cpp

│   ├── Person.cpp

│   ├── Employee.cpp

│   └── ...

├── data/                 \# Chứa dữ liệu lưu trữ (.txt / .csv)

│   ├── employees.csv

│   ├── timekeeping.csv

│   └── ...

└── main.cpp              \# Điểm khởi chạy chính của chương trình

## **Cấu trúc và Lớp nền tảng (Base Classes) dùng chung**

// Date.h \- Cấu trúc quản lý ngày tháng dùng chung

\#ifndef DATE\_H

\#define DATE\_H

\#include \<string\>

\#include \<iostream\>

struct Date {

    int day{1};

    int month{1};

    int year{2000};

    std::string toString() const;

    static DatefromString(const std::string& str);

    bool isValid() const;

};

\#endif

// BaseManager.h \- Lớp cơ sở trừu tượng cho tất cả các Manager

\#ifndef BASE\_MANAGER\_H

\#define BASE\_MANAGER\_H

class BaseManager {

public:

    virtual \~BaseManager() \= default;

    virtual void loadFromFile() \= 0; // Pure virtual function

    virtual void saveToFile() \= 0;   // Pure virtual function

    virtual void displayMenu() \= 0;  // Pure virtual function

};

\#endif

---

# **3\. PHÂN CÔNG CHI TIẾT DÀNH CHO TỪNG THÀNH VIÊN**

---

# **THÀNH VIÊN 1: Lõi Nhân sự, Cơ cấu Tổ chức & Quản lý Tài khoản**

**Phạm vi đảm nhiệm:** Lớp nền tảng `Person`, `Employee`, `Department`, `Position`, `Account`, lớp điều khiển `AuthManager`, `EmployeeManager`.

## **BƯỚC 1: Thiết kế Class & Khai báo Header (`.h`)**

* `Person.h`: Lớp cơ sở trừu tượng (`id`, `fullName`, `gender`, `birthDate`, `phone`, `email`). Có hàm thuần ảo `virtual void displayInfo() const = 0;`.  
* `Employee.h`: Kế thừa `Person`. Bổ sung `employeeId`, `departmentId`, `positionId`, `status` (Active/Resigned), `baseSalaryRate`. Ghè đè (`override`) `displayInfo()`.  
* `Department.h` & `Position.h`: Lưu thông tin phòng ban (mã, tên, mã trưởng phòng) và chức vụ (mã, tên, hệ số phụ cấp).  
* `Account.h`: Thông tin đăng nhập (`username`, `passwordHash`, `role`: Admin/Manager/Employee, `employeeId`).  
* `AuthManager.h`: Xử lý đăng nhập, phiên làm việc (`currentUser`), đăng xuất, đổi mật khẩu.  
* `EmployeeManager.h`: Kế thừa `BaseManager`. Quản lý danh sách `Employee`, `Department`, `Position`.

## **BƯỚC 2: Hiện thực Logic & Thuật toán (`.cpp`)**

* Thuật toán tìm kiếm nhân viên theo nhiều tiêu chí (ID, Tên, Phòng ban) bằng cách duyệt `std::vector<Employee>`.  
* Kiểm tra tính duy nhất của `employeeId` và `username` khi thêm mới.  
* Phân quyền hệ thống: Hàm `hasPermission(Role requiredRole)` kiểm tra quyền thực thi menu.

## **BƯỚC 3: Xử lý File I/O & Menu Console**

* Định dạng file: `data/employees.csv` (phân tách bằng dấu phẩy `,`).  
* Hàm `loadFromFile()` đọc từng dòng, tách chuỗi (string split) thành các thuộc tính để khởi tạo đối tượng `Employee`.  
* Hàm `saveToFile()` ghi đè lại file mỗi khi có thao tác CRUD.  
* Menu Console cung cấp chức năng: Quản lý nhân sự, Quản lý phòng ban, Tạo tài khoản hệ thống.

## **BƯỚC 4: Tích hợp & Kiểm thử (Unit Test)**

* Kiểm thử biên: Nhập ngày sinh không hợp lệ, trùng mã nhân viên, mật khẩu sai quá 3 lần.  
* Cung cấp các API công khai cho các thành viên khác: `Employee* getEmployeeById(string id)`, `bool isEmployeeExist(string id)`.

---

# **THÀNH VIÊN 2: Quản lý Ca làm việc, Chấm công & Nghỉ phép**

**Phạm vi đảm nhiệm:** Các lớp `WorkShift`, `TimekeepingRecord`, `LeaveRequest`, và lớp điều khiển `AttendanceManager`.

## **BƯỚC 1: Thiết kế Class & Khai báo Header (`.h`)**

* `WorkShift.h`: Khai báo ca làm việc (`shiftId`, `shiftName`, `startTime`, `endTime`).  
* `TimekeepingRecord.h`: Nhật ký chấm công (`recordId`, `employeeId`, `date`, `checkInTime`, `checkOutTime`, `lateMinutes`, `earlyMinutes`).  
* `LeaveRequest.h`: Đơn nghỉ phép (`requestId`, `employeeId`, `startDate`, `endDate`, `reason`, `leaveType`: Paid/Unpaid, `status`: Pending/Approved/Rejected).  
* `AttendanceManager.h`: Kế thừa `BaseManager`. Chứa `std::vector<TimekeepingRecord>` và `std::vector<LeaveRequest>`.

## **BƯỚC 2: Hiện thực Logic & Thuật toán (`.cpp`)**

* **Mô phỏng máy chấm công:**  
  * Hàm `checkIn(employeeId, time)` và `checkOut(employeeId, time)`.  
  * Thuật toán tự động tính `lateMinutes` (số phút đi trễ so với `startTime` của ca) và `earlyMinutes` (số phút về sớm).  
* **Luồng duyệt đơn xin nghỉ phép:**  
  * Thuật toán tự động tính tổng số ngày xin nghỉ giữa `startDate` và `endDate`.  
  * Đơn tự động chuyển trạng thái hoặc gửi cảnh báo nếu số ngày nghỉ phép có lương vượt quá hạn mức năm (12 ngày/năm).

## **BƯỚC 3: Xử lý File I/O & Menu Console**

* Định dạng file: `data/timekeeping.csv` và `data/leave_requests.csv`.  
* Hàm `loadFromFile()` và `saveToFile()` cho dữ liệu chấm công và đơn xin nghỉ.  
* Menu Console:  
  * Dành cho NV: Check-in/Check-out, Gửi đơn nghỉ phép, Xem lịch sử chấm công.  
  * Dành cho Manager: Duyệt đơn xin nghỉ, Báo cáo đi trễ/về sớm.

## **BƯỚC 4: Tích hợp & Kiểm thử (Unit Test)**

* Gọi API của Thành viên 1 để xác thực `employeeId` tồn tại trước khi cho phép chấm công.  
* Cung cấp API cho Thành viên 3: `int getWorkingDays(string employeeId, int month, int year)` để tính lương.  
* Kiểm thử biên: Check-out trước Check-in, đăng ký nghỉ phép có ngày kết thúc trước ngày bắt đầu.

---

# **THÀNH VIÊN 3: Hợp đồng Lao động, Khen thưởng/Kỷ luật & Quản lý Bảng lương**

**Phạm vi đảm nhiệm:** Các lớp `Contract`, `RewardDiscipline`, `SalaryRecord`, và lớp điều khiển `PayrollManager`.

## **BƯỚC 1: Thiết kế Class & Khai báo Header (`.h`)**

* `Contract.h`: Hợp đồng (`contractId`, `employeeId`, `contractType`, `signDate`, `expiredDate`, `baseSalary`).  
* `RewardDiscipline.h`: Khen thưởng / Kỷ luật (`id`, `employeeId`, `type`: Reward/Discipline, `amount`, `reason`, `date`).  
* `SalaryRecord.h`: Bảng lương tháng (`recordId`, `employeeId`, `month`, `year`, `workDays`, `baseSalary`, `allowance`, `bonus`, `deduction`, `tax`, `netSalary`, `isDisputed`).  
* `PayrollManager.h`: Kế thừa `BaseManager`. Điều khiển tính toán toàn bộ chi phí lương.

## **BƯỚC 2: Hiện thực Logic & Thuật toán (`.cpp`)**

* **Thuật toán tính lương Net chuẩn:**  
  \$\$\\text{Tổng thu nhập} \= (\\text{Lương cơ bản} \\times \\frac{\\text{Số ngày công}}{22}) \+ \\text{Phụ cấp} \+ \\text{Thưởng} \- \\text{Phạt}\$\$  
  \$\$\\text{BHXH bắt buộc} \= \\text{Lương cơ bản} \\times 10.5%\$\$  
  \$\$\\text{Thu nhập chịu thuế} \= \\text{Tổng thu nhập} \- \\text{BHXH} \- \\text{Giảm trừ bản thân (11 triệu)}\$\$  
  \$\$\\text{Thuế TNCN} \= \\text{Tính theo biểu thuế lũy tiến từng phần}\$\$  
  \$\$\\text{Lương Net} \= \\text{Tổng thu nhập} \- \\text{BHXH} \- \\text{Thuế TNCN}\$\$  
* **Quy trình khiếu nại:** Cập nhật biến `isDisputed = true`, cho phép HR sửa đổi thông số công/thưởng và tính toán lại (`recalculateSalary`).

## **BƯỚC 3: Xử lý File I/O & Menu Console**

* Định dạng file: `data/contracts.csv`, `data/payroll_MM_YYYY.csv`.  
* Hàm xuất phiếu lương cá nhân dạng văn bản (Pay Slip Generator).  
* Menu Console: Tính lương hàng tháng, Quản lý hợp đồng, Xử lý khiếu nại bảng lương.

## **BƯỚC 4: Tích hợp & Kiểm thử (Unit Test)**

* Kết nối dữ liệu: Lấy `baseSalary` từ `Contract`, lấy `workDays` từ `AttendanceManager` (Thành viên 2).  
* Cung cấp API cho Thành viên 4: `double getTotalPayrollByMonth(int month, int year)`.  
* Kiểm thử biên: Mức thu nhập không đến ngưỡng đóng thuế (Thuế \= 0), số ngày công bằng 0\.

---

# **THÀNH VIÊN 4: Tuyển dụng, Đào tạo, Phản hồi & Báo cáo Thống kê**

**Phạm vi đảm nhiệm:** Lớp `Candidate` (kế thừa `Person`), `RecruitmentPlan`, `TrainingCourse`, `Feedback`, và lớp điều khiển `ReportManager`.

## **BƯỚC 1: Thiết kế Class & Khai báo Header (`.h`)**

* `Candidate.h`: Kế thừa `Person`. Bổ sung `candidateId`, `appliedPosition`, `status` (Applied/Interview/Passed/Rejected), `interviewScore`.  
* `RecruitmentPlan.h`: Kế hoạch tuyển dụng (`planId`, `positionId`, `targetQuantity`, `deadline`).  
* `TrainingCourse.h`: Khóa đào tạo (`courseId`, `courseName`, `startDate`, `listEmployeeIds`).  
* `Feedback.h`: Đơn góp ý/khiếu nại (`feedbackId`, `employeeId`, `content`, `response`, `status`).  
* `ReportManager.h`: Kế thừa `BaseManager`. Tổng hợp dữ liệu toàn hệ thống.

## **BƯỚC 2: Hiện thực Logic & Thuật toán (`.cpp`)**

* **Chuyển đổi Ứng viên \-\> Nhân viên:** Khi `Candidate.status` chuyển thành `Passed` và xác nhận thử việc, hàm `convertToEmployee()` sẽ tự động đóng gói dữ liệu và gọi API từ Thành viên 1 để tạo một `Employee` mới vào hệ thống.  
* **Thuật toán Thống kê:**  
  * Thống kê cơ cấu nhân sự theo phòng ban (tính tỷ lệ %).  
  * Biểu đồ biến động nhân sự và tổng quỹ lương theo từng tháng.

## **BƯỚC 3: Xử lý File I/O & Menu Console**

* Định dạng file: `data/candidates.csv`, `data/feedbacks.csv`.  
* Báo cáo định dạng bảng sạch sẽ trên màn hình Console (Console Formatting with `std::setw`).  
* Menu Console: Quản lý tuyển dụng, Quản lý khóa đào tạo, Xem báo cáo thống kê dành cho Admin.

## **BƯỚC 4: Tích hợp & Kiểm thử (Unit Test)**

* Gọi API của Thành viên 1 (`EmployeeManager`) để chèn nhân viên mới khi trúng tuyển.  
* Gọi API của Thành viên 3 (`PayrollManager`) để lấy số liệu tổng chi phí lương xuất báo cáo.  
* Kiểm thử biên: Chuyển đổi ứng viên bị trúng tuyển trùng lặp, thống kê khi hệ thống chưa có dữ liệu chấm công/lương.

---

# **4\. LỘ TRÌNH THỰC HIỆN DỰ ÁN 4 TUẦN (TIMELINE & MILESTONES)**

| Tuần | Mục tiêu chính | Công việc cụ thể | Sản phẩm bàn giao (Deliverables) |
| :---- | :---- | :---- | :---- |
| **Tuần 1** | **Kiến trúc & Base Code** | \- Thống nhất Repository (Git/GitHub).- Định nghĩa file header dùng chung (`Date.h`, `BaseManager.h`, `Person.h`).- Phân chia chi tiết giao diện Header giữa các thành viên. | \- Git repository sẵn sàng.- Hoàn thiện toàn bộ file `.h` nền tảng.- Biên dịch thành công khung project. |
| **Tuần 2** | **Logic Độc lập & File I/O** | \- Cài đặt logic chi tiết trong các file `.cpp` độc lập.- Hiện thực các hàm đọc/ghi file CSV riêng cho từng module.- Viết menu Console độc lập thử nghiệm cho từng lớp. | \- Các file `.cpp` hoạt động độc lập.- File CSV mẫu cho từng module chứa dữ liệu giả định (mock data). |
| **Tuần 3** | **Tích hợp Hệ thống** | \- Kết nối `AuthManager` với toàn bộ hệ thống.- Ghép nối `AttendanceManager` \$\\rightarrow\$ `PayrollManager`.- Ghép nối `Candidate` \$\\rightarrow\$ `EmployeeManager`.- Ghép nối dữ liệu lên `ReportManager`. | \- File `main.cpp` hợp nhất hoàn chỉnh.- Luồng dữ liệu chạy xuyên suốt giữa 4 module. |
| **Tuần 4** | **Tối ưu, Test & Báo cáo** | \- Xử lý try-catch/ngoại lệ input.- Kiểm tra giải phóng bộ nhớ con trỏ (Memory Leak).- Viết Báo cáo Bài tập lớn (Word/PDF).- Làm Slide và Demo thuyết trình. | \- Mã nguồn hoàn chỉnh không lỗi.- Tài liệu Báo cáo Bài tập lớn.- Slide thuyết trình. |

---

# **5\. TỔNG HỢP GIAO THỨC TÍCH HỢP GIỮA CÁC THÀNH VIÊN**

\+-----------------------------------------------------------------------------------+

|                                 THÀNH VIÊN 1                                      |

|                     (Core, Person, Employee, AuthManager)                         |

\+-----------------------------------------------------------------------------------+

       │                                     │                                 │

       │ Cung cấp dữ liệu                    │ Cung cấp thông tin              │ Tiếp nhận

       │ Nhân viên                           │ Tài khoản & Phân quyền          │ Nhân viên mới

       ▼                                     ▼                                 │

\+-----------------------+           \+-----------------------+                  │

|     THÀNH VIÊN 2      |           |     THÀNH VIÊN 3      |                  │

| (Attendance & Leave)  |           | (Payroll & Contracts) |                  │

\+-----------------------+           \+-----------------------+                  │

       │                                     │                                 │

       │ Cung cấp số ngày làm                │ Cung cấp số liệu              │

       │ thực tế & nghỉ phép                 │ tổng quỹ lương                │

       └───────────────────┬─────────────────┘                                 │

                           ▼                                                   │

\+-------------------------------------------------------------------+          │

|                            THÀNH VIÊN 4                           |          │

|             (Recruitment, Training & ReportManager)               |──────────┘

\+-------------------------------------------------------------------+

---

# **6\. MẪU ĐỊNH DẠNG DỮ LIỆU LƯU TRỮ (FILE FORMATS)**

## **1\. `data/employees.csv` (Thành viên 1\)**

v  
EmployeeID,FullName,Gender,BirthDate,Phone,Email,DepartmentID,PositionID,Status,BaseSalaryRate  
EMP001,Nguyen Van A,Male,15/08/1990,0901234567,a.nguyen@company.com,DEP01,POS02,Active,2.5  
EMP002,Tran Thi B,Female,20/10/1995,0912345678,b.tran@company.com,DEP02,POS01,Active,1.8

\#\#\# 2\. \`data/timekeeping.csv\` (Thành viên 2\)

\`\`\`csv

RecordID,EmployeeID,Date,CheckIn,CheckOut,LateMinutes,EarlyMinutes

TK001,EMP001,01/10/2023,08:05,17:00,5,0

TK002,EMP002,01/10/2023,07:55,16:45,0,15

## **3\. `data/payroll.csv` (Thành viên 3\)**

v  
RecordID,EmployeeID,Month,Year,WorkDays,BaseSalary,Allowance,Bonus,Deduction,Tax,NetSalary  
PAY001,EMP001,10,2023,22,15000000,2000000,1000000,0,1200000,16800000

\#\#\# 4\. \`data/candidates.csv\` (Thành viên 4\)

\`\`\`csv

CandidateID,FullName,Gender,BirthDate,Phone,Email,AppliedPosition,InterviewScore,Status

CAN001,Le Van C,Male,12/05/1998,0987654321,c.le@gmail.com,POS01,85,Passed  
