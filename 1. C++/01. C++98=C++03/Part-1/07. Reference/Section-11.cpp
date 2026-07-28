#include <cctype> // Thư viện cần thiết để dùng toupper, tolower
#include <iostream>
#include <string>
#include <string_view> // Mở rộng C++17 giúp tối ưu hiệu năng đọc chuỗi


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
  // PHẦN BỔ SUNG: CÁC HÀM PHỔ BIẾN HAY DÙNG VỚI STRING
  // =========================================================================
  std::cout << "--- PHAN BO SUNG: CAC HAM PHO BIEN HAY DUNG ---" << std::endl;

  // 1. Kiểm tra rỗng và Xóa chuỗi
  std::string test_str = "Data";
  std::cout << "Chuoi co rong khong? (0: Sai, 1: Dung): " << test_str.empty()
            << std::endl;
  test_str.clear(); // Xóa sạch sành sanh
  std::cout << "Sau khi .clear(), chuoi co rong khong?: " << test_str.empty()
            << std::endl;

  // 2. Chuyển đổi giữa Chuỗi và Số (Rất hay dùng khi nhập xuất dữ liệu)
  int so_nguyen = 2026;
  std::string chuoi_so = std::to_string(so_nguyen); // Số -> Chuỗi
  std::cout << "Chuyen so thanh chuoi: " << chuoi_so + " vaf them chuoi"
            << std::endl;

  std::string text_so = "456";
  int so_da_chuyen = std::stoi(text_so); // Chuỗi -> Số (String to Integer)
  std::cout << "Chuyen chuoi thanh so (roi + 4): " << so_da_chuyen + 4
            << std::endl;

  // 3. Chuyển đổi Chữ hoa / Chữ thường
  std::string userName = "cPp LeArNeR";
  std::cout << "Chuoi viet hoa toan bo: ";
  for (char &c : userName) {
    c = std::toupper(c); // Duyet qua tung ky tu va viet hoa
  }
  std::cout << userName << std::endl;
  std::cout << "------------------------------------------------\n";

  // =========================================================================
  // PHẦN 4: CHUYỂN ĐỔI SANG CON TRỎ KIỂU C CỔ ĐIỂN (.c_str())
  // =========================================================================
  std::cout << "--- 4. CON TRO C-STYLE STRING ---" << std::endl;
  std::string main_str = "Lap trinh C++ cung ban hoc.";
  const char *c_str = main_str.c_str();
  std::cout << "Chuoi in tu con tro kieu C: " << c_str << std::endl;
  std::cout << "------------------------------------------------\n";

  // =========================================================================
  // PHẦN 5: MỞ RỘNG: TỐI ƯU HIỆU NĂNG VỚI std::string_view (C++17)
  // =========================================================================
  std::cout << "--- 5. STRING_VIEW (C++17 OPTIMIZATION) ---" << std::endl;
  std::string long_string = "Day la mot chuoi sieu dai luu tren bo nho Heap...";
  std::string_view view(long_string);
  std::string_view sub_view = view.substr(10, 8);
  std::cout << "Cat chuoi sieu toc dung view: " << sub_view << std::endl;
  std::cout << "------------------------------------------\n";

  // =========================================================================
  // PHẦN 6: THAM CHIẾU NÂNG CAO - RVALUE REFERENCE (&&)
  // =========================================================================
  std::cout << "--- 6. RVALUE REFERENCE (&&) ---" << std::endl;
  int &&ref_tam = 50;
  std::cout << "Gia tri cua ref_tam: " << ref_tam << std::endl;
  std::string chuoi_co_ten = "Hello";
  xuLyChuoi(chuoi_co_ten);
  xuLyChuoi("Text tam thoi");
  std::cout << "\nTruoc khi move: chuoi_co_ten = \"" << chuoi_co_ten << "\""
            << std::endl;
  xuLyChuoi(std::move(chuoi_co_ten));
  std::cout << "------------------------------------------------\n";

  return 0;
}