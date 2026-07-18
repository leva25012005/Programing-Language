#include <iostream>

int main() {
  // ==========================================
  // 🔀 CHƯƠNG 7: CÁC TOÁN TỬ TRONG C++
  // ==========================================
  std::cout << "=== CHUONG 7: TOAN TU ===" << std::endl;

  // 7.1 Toán tử gán (=)
  int x = 123;
  int y = 456;
  x = y; // Giá trị của x bây giờ là 456

  // 7.2 Toán tử số học (+, -, *, /, %)
  int tong = 10 + 5;
  int hieu = 10 - 5;
  int tich = 10 * 5;

  // ⚠️ Lưu ý về phép chia số nguyên (Integer Division)
  int chia_nguyen = 9 / 2; // Kết quả là 4 (phần thập phân bị cắt bỏ)
  double chia_thuc =
      9.0 / 2; // Kết quả là 4.5 (ít nhất một toán hạng là double)
  int chia_du = 9 % 2; // Kết quả là 1 (phép chia lấy dư)

  std::cout << "9 / 2 (so nguyen) = " << chia_nguyen << std::endl;
  std::cout << "9.0 / 2 (so thuc) = " << chia_thuc << std::endl;
  std::cout << "9 % 2 (chia du)   = " << chia_du << std::endl;

  // 7.3 Toán tử gán phức hợp (+=, -=, *=, /=, %=)
  int a = 10;
  a += 5; // Tương đương: a = a + 5 -> a = 15
  a *= 2; // Tương đương: a = a * 2 -> a = 30
  std::cout << "Gia tri cua a sau gan phuc hop: " << a << std::endl;

  // 7.4 Toán tử tăng/giảm (++, --)
  int b = 5;
  int prefix =
      ++b; // Tiền tố: b tăng lên 6 trước, rồi gán vào prefix (prefix = 6)

  int c = 5;
  int postfix = c++; // Hậu tố: gán c vào postfix trước (postfix = 5), rồi c mới
                     // tăng lên 6

  std::cout << "Prefix (tien to): " << prefix
            << " | Postfix (hau to): " << postfix << "\n\n";

  // ==========================================
  // ⌨️ CHƯƠNG 8 & 9: NHẬP XUẤT CHUẨN (STD::CIN)
  // ==========================================
  std::cout << "=== CHUONG 8 & 9: NHAP XUAT CHUAN ===" << std::endl;

  // Nhập nhiều giá trị cùng lúc với các kiểu dữ liệu khác nhau
  char ky_tu;
  int so_nguyen;
  double so_thuc;

  std::cout << "Hay nhap vao mot ky tu, mot so nguyen va mot so thuc (cach "
               "nhau bang dau cach): ";
  std::cin >> ky_tu >> so_nguyen >> so_thuc;

  std::cout << "\nBan da nhap: " << std::endl;
  std::cout << "Ky tu: " << ky_tu << std::endl;
  std::cout << "So nguyen: " << so_nguyen << std::endl;
  std::cout << "So thuc: " << so_thuc << std::endl;

  return 0;
}