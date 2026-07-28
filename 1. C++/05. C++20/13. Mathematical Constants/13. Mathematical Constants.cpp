#include <concepts> // C++20 Concepts (Ràng buộc kiểu dữ liệu)
#include <iomanip>  // Thư viện định dạng đầu ra (setprecision)
#include <iostream>
#include <numbers> // Thư viện C++20 chứa các hằng số toán học


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Lịch sử đen tối trước C++20:
    - Trước đây, để lấy số Pi, lập trình viên phải dùng macro của ngôn ngữ C:
      #define _USE_MATH_DEFINES
      #include <cmath>
      // Sau đó dùng M_PI.
    - Nhược điểm: Macro không an toàn về kiểu dữ liệu (Type-safety), dễ gây lỗi
 ngầm.

 2. Giải pháp C++20 (<numbers>):
    - Các hằng số được đặt trong namespace `std::numbers`.
    - An toàn tuyệt đối, hỗ trợ chuẩn xác các kiểu số thực (float, double, long
 double).
    - Có thể tính toán ngay lúc biên dịch (constexpr).
*/

// ==========================================
// 1. MỨC CƠ BẢN: SỬ DỤNG HẰNG SỐ MẶC ĐỊNH
// ==========================================
void numbers_co_ban() {
  std::cout << "\n--- 1. BAZIC: HANG SO TOAN HOC (Kieu double) ---\n";

  // Theo mặc định, nếu viết thẳng tên, chúng mang kiểu dữ liệu 'double'
  std::cout << "Pi (Ty so chu vi / duong kinh)   : " << std::numbers::pi
            << '\n';
  std::cout << "e (Co so logarit tu nhien)      : " << std::numbers::e << '\n';
  std::cout << "log2(e) (Logarit co so 2 cua e) : " << std::numbers::log2e
            << '\n';
  std::cout << "log10(e) (Log co so 10 cua e)   : " << std::numbers::log10e
            << '\n';
  std::cout << "ln(2) (Log tu nhien cua 2)      : " << std::numbers::ln2
            << '\n';
  std::cout << "ln(10) (Log tu nhien cua 10)    : " << std::numbers::ln10
            << '\n';
  std::cout << "sqrt(2) (Can bac 2 cua 2)       : " << std::numbers::sqrt2
            << '\n';
  std::cout << "sqrt(3) (Can bac 2 cua 3)       : " << std::numbers::sqrt3
            << '\n';

  // Ngoài ra C++20 còn cung cấp:
  std::cout << "1 / Pi                          : " << std::numbers::inv_pi
            << '\n';
  std::cout << "1 / sqrt(Pi)                    : " << std::numbers::inv_sqrtpi
            << '\n';
  std::cout << "Phi (Ty le vang - Golden Ratio) : " << std::numbers::phi
            << '\n';
}

// ==========================================
// 2. MỨC TRUNG CẤP: ĐỘ CHÍNH XÁC & TEMPLATE VARIABLE (_v)
// ==========================================
void numbers_do_chinh_xac() {
  std::cout << "\n--- 2. INTERMEDIATE: TEMPLATE VARIABLE (_v) ---\n";

  // VẤN ĐỀ: std::numbers::pi chỉ là kiểu 'double' (Khoảng 15 chữ số thập phân).
  // Nếu lập trình Game 3D cần tốc độ (chỉ cần float) hoặc Tính toán Khoa học
  // (cần long double)?

  // GIẢI PHÁP: C++20 cung cấp Template Variable với hậu tố '_v<T>'

  float pi_float = std::numbers::pi_v<float>;
  double pi_double = std::numbers::pi_v<double>; // Tuong duong std::numbers::pi
  long double pi_ldouble = std::numbers::pi_v<long double>;

  // In ra với 20 chữ số thập phân để thấy sự khác biệt về độ chính xác
  std::cout << std::setprecision(20);
  std::cout << "Pi (float)       : " << pi_float << " (Sai so o cuoi)\n";
  std::cout << "Pi (double)      : " << pi_double << "\n";
  std::cout << "Pi (long double) : " << pi_ldouble << " (Chinh xac nhat)\n";

  // Trả lại định dạng mặc định cho console
  std::cout << std::setprecision(6);
}

// ==========================================
// 3. MỨC NÂNG CAO: TÍNH TOÁN COMPILE-TIME & CONCEPTS
// ==========================================

// a. Ứng dụng C++20 Concepts:
// Ràng buộc hàm này CHỈ NHẬN kiểu dữ liệu là Số thực (floating_point),
// không cho phép truyền int hay chuỗi vào để tránh sai số toán học.
template <std::floating_point T>
constexpr T tinh_dien_tich_hinh_tron(T ban_kinh) {
  // Tự động sử dụng đúng độ chính xác Pi của kiểu dữ liệu T
  return std::numbers::pi_v<T> * ban_kinh * ban_kinh;
}

// b. Tính toán Compile-time (constexpr):
// Nhờ hằng số trong <numbers> là constexpr, biểu thức này được tính toán
// XONG XUÔI ngay khi đang biên dịch (Compile-time), tốc độ khi chạy (Runtime)
// tốn 0 mili-giây.
constexpr double dien_tich_mau = tinh_dien_tich_hinh_tron(5.0);

void numbers_chuyen_gia() {
  std::cout << "\n--- 3. ADVANCED: COMPILE-TIME & CONCEPTS ---\n";

  std::cout << "Dien tich hinh tron (R=5.0, tinh tu luc Compile): "
            << dien_tich_mau << "\n";

  // Tính toán Runtime với độ chính xác float
  float r_float = 3.0f;
  std::cout << "Dien tich (float): " << tinh_dien_tich_hinh_tron(r_float)
            << "\n";

  // Tính toán Runtime với độ chính xác long double
  long double r_ldouble = 3.0L;
  std::cout << "Dien tich (long double): "
            << tinh_dien_tich_hinh_tron(r_ldouble) << "\n";

  // Tinh_dien_tich_hinh_tron(5); // LỖI BIÊN DỊCH! Vì 5 là 'int', vi phạm
  // concept std::floating_point
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== C++20 MATHEMATICAL CONSTANTS (<numbers>) =====\n";

  numbers_co_ban();
  numbers_do_chinh_xac();
  numbers_chuyen_gia();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}