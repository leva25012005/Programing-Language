#include <iostream>
#include <string>
#include <vector>

// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY - TỪ C++20)
// ==========================================
/*
 1. Vấn đề của C++ cũ:
    Khi khởi tạo Struct: User u = {"Nhan", 22};
    Nếu Struct có 10 trường, bạn rất dễ truyền nhầm thứ tự (ví dụ nhầm Tuổi và
 Cân nặng), gây ra bug logic cực kỳ khó phát hiện.

 2. Giải pháp C++20 (Designated Initializers):
    Cho phép gọi đích danh tên biến thành viên khi khởi tạo: User u = {.name =
 "Nhan", .age = 22}; Code trở nên TỰ TÀI LIỆU HÓA (Self-documenting), đọc vào là
 hiểu ngay biến nào mang giá trị gì.

 3. Điều kiện bắt buộc (Aggregate Type):
    Struct/Class của bạn KHÔNG ĐƯỢC có Constructor do người dùng tự viết.
    KHÔNG ĐƯỢC có các biến private/protected.
    KHÔNG ĐƯỢC có hàm ảo (Virtual function).
*/

// ==========================================
// CÁC STRUCT DÙNG CHO DEMO
// ==========================================
struct User {
  std::string name;
  int age;
  float gpa;
};

struct NhanVien {
  std::string ten = "Chua cap nhat"; // Giá trị mặc định
  int luong = 5000;
  bool dang_lam_viec = true;
};

struct DiaChi {
  std::string duong;
  std::string thanh_pho;
};

struct HoSo {
  std::string id;
  DiaChi noi_o; // Struct lồng Struct
  int mang_so[3];
};

// ==========================================
// 1. MỨC CƠ BẢN: KHỞI TẠO RÕ RÀNG & BỎ QUA BIẾN
// ==========================================
void designated_co_ban() {
  std::cout << "\n--- 1. BAZIC: KHOI TAO & BO QUA BIEN ---\n";

  // Khởi tạo chỉ định rõ ràng từng thành viên
  User u1 = {.name = "Le Van Nhan", .age = 22, .gpa = 3.8f};
  std::cout << "User 1: " << u1.name << " - Tuoi: " << u1.age << "\n";

  // TÍNH NĂNG HAY NHẤT: Bỏ qua biến không cần thiết
  // Các biến bị bỏ qua sẽ tự động nhận giá trị Default hoặc 0
  // (Zero-initialized).
  User u2 = {.name = "Khach Hang"};
  // .age tự động = 0, .gpa tự động = 0.0

  std::cout << "User 2: " << u2.name << " - Tuoi: " << u2.age
            << " - GPA: " << u2.gpa << "\n";
}

// ==========================================
// 2. MỨC TRUNG CẤP: LUẬT LỆ THÉP CỦA C++ (KHÁC VỚI C)
// ==========================================
void designated_luat_khac_nghiet() {
  std::cout << "\n--- 2. INTERMEDIATE: NHUNG DIEU C++ CAM (Ma C thi cho) ---\n";

  // a. CẤM ĐẢO LỘN THỨ TỰ (Trình biên dịch sẽ báo lỗi ngay lập tức)
  // Trong C, bạn có thể viết {.age = 22, .name = "Nhan"}. Nhưng C++ THÌ KHÔNG!
  // C++ bắt buộc thứ tự khởi tạo PHẢI Y HỆT như thứ tự khai báo trong Struct.
  /*
      User u_loi = { .age = 22, .name = "Loi" }; // LỖI BIÊN DỊCH Ở C++20!
  */

  // b. CẤM TRỘN LẪN (Mixing styles)
  // Bạn không thể vừa dùng cách cũ (không tên), vừa dùng cách mới (có tên).
  /*
      User u_loi_2 = { "Nhan", .age = 22 }; // LỖI BIÊN DỊCH!
  */

  std::cout
      << "-> Da hieu ro luat cam C++: Phai dung thu tu va Khong tron lan.\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: KẾT HỢP VỚI DEFAULT MEMBER INITIALIZERS
// ==========================================
void designated_mac_dinh() {
  std::cout << "\n--- 3. ADVANCED: DEFAULT INITIALIZERS ---\n";

  // Khởi tạo toàn bộ theo giá trị mặc định của Struct
  NhanVien nv1 = {};
  std::cout << "NV 1: " << nv1.ten << " | Luong: " << nv1.luong << "\n";

  // Chỉ ghi đè mức lương, các biến khác (Tên, Trạng thái) vẫn giữ nguyên giá
  // trị mặc định!
  NhanVien nv2 = {.luong = 9999};
  std::cout << "NV 2: " << nv2.ten << " | Luong moi: " << nv2.luong
            << " | Dang lam: " << (nv2.dang_lam_viec ? "Co" : "Khong") << "\n";
}

// ==========================================
// 4. MỨC CHUYÊN GIA: STRUCT LỒNG STRUCT VÀ MẢNG
// ==========================================
void designated_long_nhau() {
  std::cout << "\n--- 4. EXPERT: NESTED STRUCTS & ARRAYS ---\n";

  // 1. Struct lồng nhau (Phải dùng Ngoặc nhọn lồng nhau)
  // Lưu ý: C++ KHÔNG cho phép cú pháp chuỗi chấm kiểu C (.noi_o.duong = "A")
  HoSo hs = {
      .id = "HS_101",
      .noi_o = {.duong = "Quach Huu Nghiem", .thanh_pho = "Ho Chi Minh"},
      .mang_so = {10, 20, 30} // Khởi tạo mảng tĩnh
  };

  std::cout << "Ho So: " << hs.id << "\n";
  std::cout << "Dia chi: " << hs.noi_o.duong << ", " << hs.noi_o.thanh_pho
            << "\n";
  std::cout << "Mang: " << hs.mang_so[0] << ", " << hs.mang_so[1] << "\n";

  // 2. KHÔNG THỂ CHỈ ĐỊNH INDEX MẢNG NHƯ C
  /*
      Trong C, bạn có thể viết: int arr[3] = { [1] = 99 };
      Nhưng C++20 KHÔNG hỗ trợ cú pháp chỉ định Index mảng này!
  */
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== C++20 DESIGNATED INITIALIZERS =====\n";

  designated_co_ban();
  designated_luat_khac_nghiet();
  designated_mac_dinh();
  designated_long_nhau();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}