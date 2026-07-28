#include <iomanip> // Thư viện để định dạng số thực đầu ra
#include <iostream>

int main() {
  // -------------------------------------------------------------------------
  // 1. MINH HỌA CÁC CÁCH KHỞI TẠO (Bao gồm dấu ngoặc đơn "()")
  // -------------------------------------------------------------------------
  bool b1 = true; // Copy initialization
  bool b2(true);  // Direct initialization
  bool b3{true};  // List initialization (Khuyên dùng trong C++ hiện đại)
  bool b4 = {true};
  // -------------------------------------------------------------------------
  // 2. KIỂU CHAR VÀ MÃ ASCII
  // -------------------------------------------------------------------------
  char c1 = 'a';
  char c2 = 97; // 97 là mã ASCII của 'a'

  std::cout << "--- KIEU DU LIEU CO BAN ---" << std::endl;
  std::cout << "Char c1: " << c1 << ", Char c2 (khoi tao bang so 97): " << c2
            << std::endl;
  std::cout << "Kich thuoc cua char: " << sizeof(char) << " byte" << std::endl;
  std::cout << "------------------------------------------" << std::endl;

  // -------------------------------------------------------------------------
  // 3. SO NGUYEN: DECIMAL, OCTAL, HEXADECIMAL & MODIFIERS
  // -------------------------------------------------------------------------
  int dec = 10;  // Hệ 10
  int oct = 012; // Hệ 8 (Bắt đầu bằng số 0)
  int hex = 0xA; // Hệ 16 (Bắt đầu bằng 0x)

  unsigned int u_int =
      4000000000U; // Chỉ chứa số dương, chứa được số lớn hơn int thường

  std::cout << "Hex 0xA in ra man hinh: " << hex << std::endl;
  std::cout << "Unsigned int: " << u_int << std::endl;
  std::cout << "------------------------------------------" << std::endl;

  // -------------------------------------------------------------------------
  // 4. MỞ RỘNG: SO SÁNH FLOAT VÀ DOUBLE (Độ chính xác)
  // -------------------------------------------------------------------------
  // Khi viết số thực, mặc định C++ hiểu là double. Để chỉ định float, ta thêm
  // hậu tố 'f' hoặc 'F'.
  float f_num = 3.141592653589793f; // Float (4 bytes) - Độ chính xác thấp hơn
  double d_num = 3.141592653589793; // Double (8 bytes) - Độ chính xác cao hơn

  std::cout << "--- SO SANH FLOAT VA DOUBLE ---" << std::endl;
  // Ép hiển thị 16 chữ số phần thập phân để thấy sự khác biệt
  std::cout << std::setprecision(16);
  std::cout << "Gia tri Float  f_num: " << f_num << " (Bi sai so o cuoi!)"
            << std::endl;
  std::cout << "Gia tri Double d_num: " << d_num << " (Chinh xac hon)"
            << std::endl;
  std::cout << "Kich thuoc float: " << sizeof(float) << " bytes" << std::endl;
  std::cout << "Kich thuoc double: " << sizeof(double) << " bytes" << std::endl;
  std::cout << "------------------------------------------" << std::endl;

  // -------------------------------------------------------------------------
  // 5. CẢNH BÁO: UNDEFINED BEHAVIOR (UB) VỚI BIẾN CHƯA KHỞI TẠO
  // -------------------------------------------------------------------------
  // Dưới đây là ví dụ về nguy cơ nghiêm trọng khi KHÔNG khởi tạo biến cục bộ
  int uninitialized_var = 1;

  std::cout << "--- CANH BAO: UNDEFINED BEHAVIOR ---" << std::endl;
  std::cout << "Nut canh bao! Bien chua khoi tao co gia tri rac: "
            << uninitialized_var << std::endl;
  // Chú ý: Giá trị in ra là ngẫu nhiên tùy thuộc vào ô nhớ cũ của RAM.
  std::cout << "------------------------------------------" << std::endl;

  return 0;
}