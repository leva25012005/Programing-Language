#include <iostream>

int main() {
  // -------------------------------------------------------------------------
  // 1. CÁI BẪY 1: PHÉP CHIA SỐ NGUYÊN (INTEGER DIVISION TRUNCATION)
  // -------------------------------------------------------------------------
  int a = 5;
  int b = 2;

  int ket_qua_nguyen =
      a / b; // Cả 2 đều là int -> Kết quả bị CẮT CỤT phần thập phân
  double ket_qua_thuc =
      a /
      b; // Vẫn bằng 2! Vì vế phải (a / b) tính xong ra 2 rồi mới gán vào double

  // CÁCH CỐ ĐỊNH: Ép kiểu (Casting) hoặc đảm bảo ít nhất 1 toán hạng là số thực
  double ket_qua_dung = (double)a / b; // Ép 'a' thành double trước khi chia
  double ket_qua_literal = 5 / 2.0;    // Viết 2.0 thay vì 2

  std::cout << "--- 1. PHEP CHIA SO NGUYEN vs SO THUC ---" << std::endl;
  std::cout << "5 / 2 (kieu int)    = " << ket_qua_nguyen << std::endl;
  std::cout << "5 / 2 (gan double)  = " << ket_qua_thuc << " (Van bi sai!)"
            << std::endl;
  std::cout << "5 / 2.0 (chuan)     = " << ket_qua_literal << std::endl;
  std::cout << "5 % 2 (chia lay du) = " << (a % b)
            << std::endl; // Lấy phần dư: 5 chia 2 dư 1
  std::cout << "------------------------------------------------" << std::endl;

  // -------------------------------------------------------------------------
  // 2. TOÁN TỬ TOÁN HỢP (COMPOUND ASSIGNMENT)
  // -------------------------------------------------------------------------
  int x = 10;
  x += 5; // Tương đương: x = x + 5 -> x bây giờ là 15
  x *= 2; // Tương đương: x = x * 2 -> x bây giờ là 30

  std::cout << "--- 2. TOAN TU TOAN HOP ---" << std::endl;
  std::cout << "Gia tri x sau khi += 5 va *= 2 la: " << x << std::endl;
  std::cout << "------------------------------------------------" << std::endl;

  // -------------------------------------------------------------------------
  // 3. CÁI BẪY 2: SỰ KHÁC BIỆT CỦA ++X VÀ X++ (TIMING IN EXPRESSION)
  // -------------------------------------------------------------------------
  int y = 10;

  std::cout << "--- 3. TIEN TO (PRE) vs HAU TO (POST) ---" << std::endl;

  // Trường hợp 1: Post-increment (Hậu tố y++)
  // Luật: "Dùng trước, tăng sau" -> In giá trị hiện tại của y ra trước, rồi mới
  // cộng y lên 1.
  std::cout << "In ra y++: " << y++
            << " (Gia tri in ra van la 10, nhung y da tang len 11 sau dong nay)"
            << std::endl;
  std::cout << "Gia tri y hien tai: " << y << std::endl;

  // Trả lại y = 10 để test tiếp
  y = 10;

  // Trường hợp 2: Pre-increment (Tiền tố ++y)
  // Luật: "Tăng trước, dùng sau" -> Cộng y lên 1 trước (thành 11), rồi mới in
  // giá trị mới ra.
  std::cout << "In ra ++y: " << ++y << " (Gia tri tang ngay lap tuc thanh 11)"
            << std::endl;
  std::cout << "Gia tri y hien tai: " << y << std::endl;

  std::cout << "------------------------------------------------" << std::endl;

  return 0;
}