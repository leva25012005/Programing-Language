#include <chrono>
#include <iostream>
#include <string>
#include <thread> // Dùng cho std::this_thread::sleep_for
#include <type_traits>
#include <typeinfo>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Literals (Tiền tố/Hậu tố hằng) là gì?
    - Khi bạn gõ số `10`, trình biên dịch hiểu đó là `int`.
    - Nếu bạn gõ `10.5`, nó hiểu là `double`. Gõ `10.5f`, nó hiểu là `float`.
    - Chữ 'f' chính là một Literal có sẵn của C++.

 2. Standard User-Defined Literals (Từ C++14):
    - C++14 mang đến các không gian tên (namespaces) đặc biệt chứa các Hậu tố
 được định nghĩa sẵn giúp code tự nhiên như tiếng Anh.
    - Cụ thể: `s` cho std::string, và `h`, `min`, `s`, `ms`, `us`, `ns` cho
 std::chrono.

 3. Rule of Thumb:
    - Để sử dụng, BẮT BUỘC phải mở khóa bằng lệnh: `using namespace ...`
*/

// ==========================================
// 1. MỨC CƠ BẢN: STD::STRING LITERALS (""s)
// ==========================================
void string_literals_co_ban() {
  std::cout << "\n--- 1. BAZIC: STRING LITERALS (\"\"s) ---\n";

  // BƯỚC BẮT BUỘC: Mở khóa quyền năng sử dụng hậu tố 's'
  using namespace std::string_literals;

  // a. SỰ KHÁC BIỆT VỚI CHUỖI CỔ ĐIỂN
  auto chuoi_c = "Hello"; // Kieu const char* (Chuỗi C cổ điển)
  auto chuoi_cpp =
      "Hello"s; // Kieu std::string (Chuỗi C++ hiện đại nhờ hậu tố 's')

  std::cout << "Kieu cua chuoi_c: " << typeid(chuoi_c).name() << "\n";
  std::cout << "Kieu cua chuoi_cpp: " << typeid(chuoi_cpp).name() << "\n";

  // b. SỨC MẠNH THỰC SỰ: Xử lý ký tự NULL (\0)
  auto c_str = "A\0B"; // Chuỗi C: Gặp \0 là nó tưởng chuỗi đã kết thúc!
  auto cpp_str = "A\0B"s; // Chuỗi C++: Thu nhận toàn bộ, kể cả \0 ở giữa!

  // std::string::length() của chuỗi_c sẽ bị nhầm là 1. Nhưng chuỗi_cpp chuẩn
  // xác là 3.
  std::cout << "Chieu dai chuoi C (\"A\\0B\"): " << std::string(c_str).length()
            << "\n"; // In ra 1
  std::cout << "Chieu dai chuoi C++ (\"A\\0B\"s): " << cpp_str.length()
            << "\n"; // In ra 3

  // c. ỨNG DỤNG RÚT GỌN KHI NỐI CHUỖI
  // Nếu không có "s": "Hello " + "World" -> Lỗi biên dịch vì không thể cộng 2
  // mảng char! Có "s": "Hello "s + "World" -> Hợp lệ, vì vế trái đã biến thành
  // std::string.
  auto loi_chao = "Xin chao "s + "C++14";
  std::cout << loi_chao << "\n";
}

// ==========================================
// 2. MỨC TRUNG CẤP: STD::CHRONO LITERALS
// ==========================================
void chrono_literals_thao_tac() {
  std::cout << "\n--- 2. INTERMEDIATE: CHRONO LITERALS (h, min, s, ms) ---\n";

  // BƯỚC BẮT BUỘC: Mở khóa quyền năng thời gian
  using namespace std::chrono_literals;

  // a. KHỞI TẠO SIÊU ĐẸP VÀ TỰ ĐỘNG TÍNH TOÁN
  auto thoi_gian_nau_an = 1h + 15min + 30s;

  // Lúc này, thoi_gian_nau_an là một cấu trúc std::chrono::duration (Rất phức
  // tạp nếu viết tay). Chúng ta có thể dễ dàng quy đổi toàn bộ cục thời gian
  // trên ra mili-giây, hoặc giây.

  auto tong_so_giay =
      std::chrono::duration_cast<std::chrono::seconds>(thoi_gian_nau_an);
  std::cout << "1h + 15min + 30s = " << tong_so_giay.count() << " giay.\n";

  // b. ỨNG DỤNG THỰC TẾ: Tạm dừng luồng (Thread Sleep)
  std::cout << "Dang dung chuong trinh 200 mili-giay...\n";

  // Thay vì viết dài dòng:
  // std::this_thread::sleep_for(std::chrono::milliseconds(200));
  std::this_thread::sleep_for(200ms); // Viết thế này sướng hơn nhiều!

  std::cout << "Da thuc day!\n";
}

// ==========================================
// 3. MỨC CHUYÊN GIA: TỰ TẠO HẬU TỐ CHO RIÊNG MÌNH (CUSTOM USER-DEFINED
// LITERALS)
// ==========================================
// C++ cho phép bạn tự chế ra hậu tố của riêng mình.
// LUẬT ÉP BUỘC: Hậu tố tự chế BẮT BUỘC phải bắt đầu bằng dấu gạch dưới ( _ ).
// Các hậu tố không có dấu gạch dưới (như 's', 'h') là "hàng độc quyền" của Ủy
// ban Tiêu chuẩn C++.

// Ví dụ 1: Xử lý Đơn vị đo lường (Quy đổi mọi thứ về Gram)
// Hàm này sẽ tự động chạy khi bạn gõ: 2.5_kg
constexpr long double operator"" _kg(long double val) {
  return val * 1000.0; // Đổi Kg sang Gram
}

// Hàm này sẽ tự động chạy khi bạn gõ: 500.0_g
constexpr long double operator"" _g(long double val) {
  return val; // Đã là Gram thì giữ nguyên
}

// Ví dụ 2: Định dạng tiền tệ
// Hàm này chạy khi bạn gõ: 150000_VND (Số nguyên không dấu unsigned long long)
std::string operator"" _VND(unsigned long long val) {
  std::string s = std::to_string(val);
  int n = s.length() - 3;
  // Thêm dấu phẩy ngăn cách hàng nghìn
  while (n > 0) {
    s.insert(n, ",");
    n -= 3;
  }
  return s + " VND";
}

void custom_user_defined_literals() {
  std::cout << "\n--- 3. EXPERT: CUSTOM USER-DEFINED LITERALS ---\n";

  // 1. Ứng dụng đo lường (Type-safety cho vật lý)
  auto khoi_luong_A = 2.5_kg;  // Tự động gọi hàm operator"" _kg
  auto khoi_luong_B = 500.0_g; // Tự động gọi hàm operator"" _g

  std::cout << "Khoi luong A (tinh ra gram): " << khoi_luong_A << " g\n";

  // Vì mọi thứ đã quy về Gram ở dưới nền, ta có thể cộng trừ rất an toàn
  auto tong_khoi_luong = khoi_luong_A + khoi_luong_B;
  std::cout << "2.5_kg + 500.0_g = " << tong_khoi_luong << " g\n";

  // 2. Ứng dụng tiền tệ
  auto gia_tien = 5000000_VND;
  std::cout << "Gia san pham: " << gia_tien << "\n";
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== KHOA HOC C++ STANDARD USER-DEFINED LITERALS =====\n";

  string_literals_co_ban();
  chrono_literals_thao_tac();
  custom_user_defined_literals();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}