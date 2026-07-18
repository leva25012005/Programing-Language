#include <iostream>

// =========================================================================
// ⚙️ HÀM CONSTEXPR & CONSTEVAL (C++20)
// =========================================================================

// Hàm constexpr: Có thể chạy ở compile-time HOẶC run-time tùy vào tham số
// truyền vào
constexpr int binhPhuong(int n) { return n * n; }

// Hàm consteval (C++20): BẮT BUỘC phải tính toán tại thời điểm biên dịch
// (Immediate Function) Nếu truyền một biến thông thường vào đây, trình biên
// dịch sẽ báo lỗi lập tức.
consteval int lapPhuong(int n) { return n * n * n; }

// Biến toàn cục dùng để minh họa constinit
int bien_toan_cuc = 10;
// constinit (C++20): Ép buộc biến phải được khởi tạo hằng lúc biên dịch,
// nhưng sau đó VẪN có thể thay đổi giá trị khi chạy (khác với constexpr bị khóa
// luôn)
constinit int bien_khoi_tao_som = 100;

int main() {
  // =========================================================================
  // 🔒 ZONE 1: CONST VÀ CONSTEXPR CƠ BẢN
  // =========================================================================
  std::cout << "--- 1. CONST vs CONSTEXPR ---" << std::endl;

  int x = 10;
  const int n1 =
      x; // Hợp lệ: Giá trị của n1 được quyết định khi chạy (Run-time)
  // constexpr int n2 = x;   // LỖI! x không phải hằng số tại thời điểm biên
  // dịch

  constexpr int c1 =
      5; // Hợp lệ: 5 là hằng số rõ ràng khi biên dịch (Compile-time)
  constexpr int c2 =
      binhPhuong(c1); // Hợp lệ: Tính toán xong c2 = 25 ngay khi biên dịch

  int mang[c1] = {1, 2, 3, 4, 5};
  std::cout << "Kich thuoc mang tinh: " << c1 << " | c2 = " << c2 << std::endl;
  std::cout << "------------------------------------\n";

  // =========================================================================
  // 🎯 ZONE 2: CẠM BẪY CON TRỎ VÀ CONST (POINTER VÀ CONST)
  // =========================================================================
  std::cout << "--- 2. CAM BAY CON TRO VA CONST ---" << std::endl;

  int gia_tri_A = 100;
  int gia_tri_B = 200;

  // 2.1 Con trỏ trỏ tới hằng số (Pointer to Const) -> GIÁ TRỊ bị khóa, ĐỊA CHỈ
  // tự do
  const int *ptr_to_const = &gia_tri_A;
  // *ptr_to_const = 150;     // LỖI! Không được sửa giá trị vùng đích qua con
  // trỏ này
  ptr_to_const = &gia_tri_B; // Hợp lệ: Đổi hướng sang biến B thoải mái

  // 2.2 Con trỏ hằng (Const Pointer) -> ĐỊA CHỈ bị khóa, GIÁ TRỊ tự do
  int *const const_ptr = &gia_tri_A;
  *const_ptr = 150; // Hợp lệ: Thay đổi giá trị gốc của gia_tri_A thành 150
  // const_ptr = &gia_tri_B;  // LỖI! Địa chỉ đã bị khóa chặt, không đổi hướng
  // được

  std::cout << "Gia tri A sau khi sua qua const_ptr: " << gia_tri_A
            << std::endl;
  std::cout << "------------------------------------\n";

  // =========================================================================
  // 🚀 ZONE 3: C++20 NÂNG CAO - CONSTEVAL VÀ CONSTINIT
  // =========================================================================
  std::cout << "--- 3. C++20: CONSTEVAL VA CONSTINIT ---" << std::endl;

  // Sử dụng hàm consteval (Bắt buộc biên dịch phải tính xong)
  constexpr int ket_qua_cubed =
      lapPhuong(4); // Tính toán ngay lúc compile thành 64
  std::cout << "Ket qua tu ham consteval (4^3): " << ket_qua_cubed << std::endl;

  // Minh họa constinit: Ban đầu được khởi tạo sớm lúc biên dịch
  std::cout << "Gia tri constinit ban dau: " << bien_khoi_tao_som << std::endl;
  bien_khoi_tao_som = 500; // Hợp lệ: Vẫn có thể thay đổi giá trị khi chạy!
  std::cout << "Gia tri constinit sau khi thay doi: " << bien_khoi_tao_som
            << std::endl;
  std::cout << "------------------------------------\n";

  return 0;
}