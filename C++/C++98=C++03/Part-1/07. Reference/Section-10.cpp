#include <iostream>
#include <string>
#include <utility> // Thư viện cần thiết để dùng std::move

// --- CÁC HÀM MẪU ĐỂ MINH HỌA TRUYỀN THAM CHIẾU ---

void tangDiemSovan(int diem) { diem += 10; }
void tangDiemThat(int &diem) { diem += 10; }
void inThongTinLon(const std::string &text) { std::cout << text << std::endl; }

// Hàm nạp chồng (Overloading) để minh họa Rvalue Reference
void xuLyChuoi(const std::string &lref) {
  std::cout << "[Lvalue Ref] Copy hoac doc thong tin: " << lref << std::endl;
}

void xuLyChuoi(std::string &&rref) {
  std::cout << "[Rvalue Ref] Chiom dung tai nguyen cua chuoi tam thoi: " << rref
            << std::endl;
  // Trong thuc te, tai day nguoi ta se dung co che dich chuyen (Move Semantics)
}

int main() {
  // =========================================================================
  // PHẦN 1: THAM CHIẾU CƠ BẢN (ALIAS) VÀ CẢNH BÁO PHÂN BIỆT KÝ TỰ &
  // =========================================================================
  std::cout << "--- 1. THAM CHIEU CO BAN ---" << std::endl;
  int x = 123;
  int &y = x;
  std::cout << "Gia tri x: " << x << " | Gia tri y: " << y << std::endl;
  y = 789;
  std::cout << "Sau khi doi y = 789 -> Gia tri x: " << x << std::endl;
  int *p = &x;
  std::cout << "Dia chi cua x (&x): " << &x << std::endl;
  std::cout << "Gia tri con tro p dang giu (p): " << p << std::endl;
  std::cout << "------------------------------------------------\n";

  // =========================================================================
  // PHẦN 2: THAM CHIẾU HẰNG (CONST REFERENCE)
  // =========================================================================
  std::cout << "--- 2. THAM CHIEU HANG (CONST INT&) ---" << std::endl;
  int a = 500;
  const int &b = a;
  std::cout << "Gia tri a: " << a << " | Gia tri b: " << b << std::endl;
  a = 600;
  std::cout << "Sau khi doi a = 600 -> Gia tri b tu dong cap nhat: " << b
            << std::endl;
  std::cout << "------------------------------------------------\n";

  // =========================================================================
  // PHẦN 3: MỞ RỘNG - ỨNG DỤNG TRONG HÀM (THỰC TẾ DÙNG NHIỀU NHẤT)
  // =========================================================================
  std::cout << "--- 3. UNG DUNG TRONG HAM ---" << std::endl;
  int diem_so = 80;
  tangDiemSovan(diem_so);
  std::cout << "Sau khi goi tangDiemSovan: " << diem_so
            << " (Khong doi vi chi truyen ban sao)" << std::endl;
  tangDiemThat(diem_so);
  std::cout << "Sau khi goi tangDiemThat : " << diem_so
            << " (Da tang len 90 vi truyen tham chieu)" << std::endl;

  std::string thong_bao =
      "Day la mot chuoi van ban rat dai va ton bo nho neu sao chep...";
  std::cout << "Goi ham in dung const reference: ";
  inThongTinLon(thong_bao);
  std::cout << "------------------------------------------------\n";

  // =========================================================================
  // PHẦN 4: THAM CHIẾU NÂNG CAO - RVALUE REFERENCE (&&)
  // =========================================================================
  std::cout << "--- 4. RVALUE REFERENCE (&&) ---" << std::endl;

  // 1. Khai báo co ban
  int &&ref_tam = 50; // Hop le! 50 la mot Rvalue (gia tri tam thoi)
  // int&& error_ref = x; // LỖI TRÌNH BIÊN DỊCH! Vi x la Lvalue (bien co ten)

  std::cout << "Gia tri cua ref_tam: " << ref_tam << std::endl;

  // 2. Minh hoa nap chong ham de toi uu hoa tai nguyen
  std::string chuoi_co_ten = "Hello";

  // Truyen vao bien co ten -> Trinh bien dich se chon ham Lvalue Ref
  xuLyChuoi(chuoi_co_ten);

  // Truyen vao mot gia tri tam thoi (Rvalue literal) -> Trinh bien dich chon
  // Rvalue Ref
  xuLyChuoi("Text tam thoi");

  // 3. Sử dụng std::move để bien Lvalue thanh Rvalue (Chu dong chuyen giao tai
  // nguyen)
  std::cout << "\nTruoc khi move: chuoi_co_ten = \"" << chuoi_co_ten << "\""
            << std::endl;

  // Ép 'chuoi_co_ten' thanh Rvalue va truyen vao ham
  xuLyChuoi(std::move(chuoi_co_ten));

  std::cout << "------------------------------------------------\n";
  return 0;
}