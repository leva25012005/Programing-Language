#include <iostream>
#include <string>

// ==========================================
// 📝 KHU VỰC KHAI BÁO HÀM (PROTOTYPES)
// ==========================================
void tangGiaTri(int x);                // Truyền tham trị
void tangGiaTriThat(int &x);           // Truyền tham chiếu
void inThongTin(const std::string &s); // Truyền tham chiếu hằng

// 🌟 MỞ RỘNG 1: Tham số mặc định (Default Argument)
void chaoHoi(std::string ten = "Khach hang");

// 🌟 MỞ RỘNG 2: Tự động suy luận kiểu trả về với 'auto' (C++14)
auto tinhTong(int a, int b) {
  return a + b; // Compiler tự biết kiểu trả về là int
}

// 🔀 KHU VỰC NẠP CHỒNG HÀM (FUNCTION OVERLOADING)
void bieuDien(int n);
void bieuDien(double d);

// ==========================================
// 🚀 HÀM CHÍNH (MAIN)
// ==========================================
int main() {
  // 1. Kiểm tra truyền tham trị vs Tham chiếu
  int so = 10;
  tangGiaTri(so);
  std::cout << "Sau tham tri: " << so << " (Khong doi)\n";

  tangGiaTriThat(so);
  std::cout << "Sau tham chieu: " << so << " (Da tang len 11)\n";

  // 2. Kiểm tra tham chiếu hằng
  std::string van_ban = "Hoc C++ cung Gemini";
  inThongTin(van_ban);

  // 3. Sử dụng tham số mặc định
  chaoHoi();     // In ra: Xin chao Khach hang
  chaoHoi("An"); // In ra: Xin chao An

  // 4. Sử dụng hàm nạp chồng
  bieuDien(42);     // Gọi phiên bản int
  bieuDien(3.1416); // Gọi phiên bản double

  return 0;
}

// ==========================================
// 🛠️ KHU VỰC ĐỊNH NGHĨA HÀM (DEFINITIONS)
// ==========================================
void tangGiaTri(int x) {
  x++; // Chỉ tăng trên bản sao
}

void tangGiaTriThat(int &x) {
  x++; // Tăng trực tiếp trên ô nhớ gốc
}

void inThongTin(const std::string &s) {
  // s = "Sua doi"; // ❌ LỖI nếu bỏ comment! const bảo vệ không cho sửa.
  std::cout << "Thong tin (Const Ref): " << s << "\n";
}

void chaoHoi(std::string ten) { std::cout << "Xin chao " << ten << "\n"; }

void bieuDien(int n) { std::cout << "In so nguyen: " << n << "\n"; }

void bieuDien(double d) { std::cout << "In so thuc: " << d << "\n"; }