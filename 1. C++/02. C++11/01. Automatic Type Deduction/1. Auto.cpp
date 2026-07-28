#include <iostream>
#include <map>
#include <string>
#include <typeinfo>
#include <vector>

// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. auto là gì?
    - Ra mắt từ C++11, `auto` yêu cầu Trình biên dịch (Compiler) TỰ ĐỘNG SUY
 LUẬN kiểu dữ liệu của biến dựa trên giá trị khởi tạo của nó.
    - Việc suy luận diễn ra NGAY LÚC BIÊN DỊCH (Compile-time). Do đó, `auto`
 KHÔNG HỀ làm chậm chương trình so với việc viết rõ kiểu dữ liệu.

 2. Luật vàng bắt buộc:
    - Đã dùng `auto` thì PHẢI khởi tạo giá trị ngay lập tức (ví dụ: `auto x =
 5;`).
    - Không thể viết: `auto x; x = 5;` (Lỗi biên dịch).

 3. Sứ mệnh lớn nhất:
    - Viết code ngắn gọn, sạch sẽ hơn (đặc biệt với các kiểu dữ liệu dài ngoằng
 như Iterator).
    - Dễ dàng refactor code (thay đổi kiểu dữ liệu ở 1 nơi, các chỗ dùng `auto`
 sẽ tự thích ứng).
*/

// ==========================================
// CLASS PHỤ TRỢ
// ==========================================
class VuKhi {
public:
  std::string ten = "Kiem";
  std::string &getTen() { return ten; }
};

// ==========================================
// 1. MỨC CƠ BẢN: TỰ ĐỘNG SUY LUẬN VÀ RÚT GỌN
// ==========================================
void auto_co_ban() {
  std::cout << "\n--- 1. BAZIC: SUY LUAN KIEU DU LIEU ---\n";

  auto a = 10;                   // Suy luận: int
  auto b = 3.14;                 // Suy luận: double
  auto c = 3.14f;                // Suy luận: float (nhờ hậu tố 'f')
  auto d = "C++11";              // Suy luận: const char*
  auto e = std::string("Hello"); // Suy luận: std::string

  // Chứng minh kiểu dữ liệu bằng typeid
  std::cout << "Kieu cua a la: " << typeid(a).name() << " (i = int)\n";
  std::cout << "Kieu cua b la: " << typeid(b).name() << " (d = double)\n";

  // ỨNG DỤNG LỚN NHẤT: Rút gọn Iterator
  std::map<std::string, int> danh_ba = {{"Nhan", 1}, {"An", 2}};

  // Thay vì phải viết dài kinh dị:
  // for (std::map<std::string, int>::const_iterator it = danh_ba.begin(); ...)
  std::cout << "Duyet Map bang auto: \n";
  for (auto it = danh_ba.begin(); it != danh_ba.end(); ++it) {
    std::cout << "  - " << it->first << ": " << it->second << "\n";
  }
}

// ==========================================
// 2. MỨC TRUNG CẤP: CẠM BẪY ĐÁNH MẤT REFERENCE VÀ CONST
// ==========================================
void auto_tham_chieu_va_const() {
  std::cout << "\n--- 2. INTERMEDIATE: CONST & REFERENCE RULES ---\n";

  int x = 10;
  int &ref_x = x;
  const int const_x = 20;

  // LUẬT CỦA AUTO: Khi suy luận, nó sẽ TỰ ĐỘNG VỨT BỎ thuộc tính Tham chiếu (&)
  // và Hằng (const)!
  auto a =
      ref_x; // a chỉ là kiểu 'int' (Copy giá trị). KHÔNG PHẢI LÀ THAM CHIẾU!
  a = 99; // Đổi a không làm thay đổi x
  std::cout << "x sau khi a=99 la: " << x
            << " (Van la 10. auto da Copy, khong tham chieu)\n";

  auto b = const_x; // b chỉ là 'int' thông thường. Đã bị lột bỏ 'const'.
  b = 99;           // Hợp lệ, không lỗi!

  // GIẢI PHÁP: Nếu muốn giữ Tham chiếu và Const, bạn phải TỰ VIẾT TAY VÀO:
  auto &c = x; // c mang kiểu 'int&' (Tham chiếu thật sự)
  c = 99;
  std::cout << "x sau khi c=99 la: " << x << " (Da doi thanh 99!)\n";

  const auto &d = x; // d mang kiểu 'const int&' (Tham chiếu chỉ đọc)
  // d = 100;             // LỖI BIÊN DỊCH! d là const.

  // ỨNG DỤNG RANGE-BASED FOR (Cạm bẫy cực kỳ phổ biến):
  std::vector<std::string> ds_chuoi = {"Mot", "Hai", "Ba"};

  // for (auto str : ds_chuoi)       // SAI LẦM! C++ sẽ Copy từng chuỗi (Rất tốn
  // RAM và CPU) for (auto& str : ds_chuoi)      // CHUẨN ĐỂ SỬA! Tham chiếu
  // trực tiếp, có thể sửa đổi dữ liệu gốc. for (const auto& str: ds_chuoi) //
  // CHUẨN ĐỂ ĐỌC! Tránh Copy và an toàn tuyệt đối.
}

// ==========================================
// 3. MỨC NÂNG CAO: AUTO TRONG HÀM VÀ LAMBDA
// ==========================================
// a. Trailing Return Type (C++11):
// Hàm trả về kiểu gì thì xem biểu thức ở mũi tên ->. Rất hữu ích trong
// Template.
auto cong_hai_so(int a, double b) -> double { return a + b; }

// b. Auto Return Type Deduction (C++14):
// Không cần -> nữa, Compiler tự chạy vào xem chữ 'return' trả về gì thì nó lấy
// kiểu đó.
auto nhan_hai_so(double a, double b) {
  return a * b; // Tự hiểu là double
}

void auto_ham_va_lambda() {
  std::cout << "\n--- 3. ADVANCED: FUNCTION & GENERIC LAMBDA ---\n";

  // c. C++14 Generic Lambda (Siêu mạnh):
  // Cho phép dùng auto làm tham số của Lambda, biến Lambda thành một Template
  // ngầm.
  auto InThongTin = [](auto x, auto y) {
    std::cout << "  Lambda nhan: " << x << " va " << y << "\n";
  };

  InThongTin(10, 20);           // Chạy kiểu int
  InThongTin("Xin chao", 3.14); // Chạy kiểu const char* và double (Thần kỳ!)
}

// ==========================================
// 4. MỨC CHUYÊN GIA: DECLTYPE(AUTO) VÀ MODERN C++
// ==========================================
// Hàm trả về tham chiếu. Dùng C++14 auto sẽ bị LỖI lột bỏ '&' như đã học ở
// mục 2. auto getTenVuKhi(VuKhi& vk) { return vk.getTen(); } -> Bị ép thành trả
// về Copy (std::string)!

// GIẢI PHÁP TỐI THƯỢNG C++14: decltype(auto)
// Lệnh này nói với Compiler: "Hàm kia trả về chính xác chữ gì (kể cả & hay
// const), xin hãy bê nguyên xi chữ đó làm kiểu trả về cho tôi, đừng lột bỏ gì
// cả!"
decltype(auto) getTenVuKhi(VuKhi &vk) {
  return vk.getTen(); // Trả về chính xác std::string&
}

void auto_chuyen_gia_modern_cpp() {
  std::cout << "\n--- 4. EXPERT: DECLTYPE(AUTO) & STRUCTURED BINDING ---\n";

  VuKhi k;
  // Hứng kết quả cũng phải dùng decltype(auto) hoặc auto& để không bị Copy.
  decltype(auto) ten_vk = getTenVuKhi(k);
  ten_vk = "Kiem Than"; // Sửa trực tiếp vào Object
  std::cout << "Ten vu khi sau khi sua: " << k.ten << "\n";

  // KỸ THUẬT C++17: Structured Bindings
  // Bung gói Tuple, Pair, Struct cực kỳ thanh lịch bằng auto
  std::pair<int, std::string> sinh_vien = {101, "Le Van Nhan"};

  // Tách 1 Pair thành 2 biến riêng biệt.
  // Dùng auto& nếu muốn sửa, dùng const auto& nếu chỉ muốn đọc.
  auto &[id, ten] = sinh_vien;
  ten = "Vo Danh";

  std::cout << "Sau khi bung goi va sua: " << sinh_vien.second << "\n";
}

// ==========================================
// 5. C++20: AUTO TRONG THAM SỐ HÀM THÔNG THƯỜNG
// ==========================================
// Từ C++20, không chỉ Lambda, hàm thông thường cũng được phép dùng auto ở tham
// số. Việc này sẽ biến hàm in_bat_ky thành một Template ngầm (Abbreviated
// Function Template).
void in_bat_ky(auto gia_tri) {
  std::cout << "  C++20 Abbreviated Template in: " << gia_tri << "\n";
}

void auto_cxx20() {
  std::cout << "\n--- 5. C++20: ABBREVIATED TEMPLATE ---\n";
  in_bat_ky(999);
  in_bat_ky("Hoan toan dung auto thay the template!");
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  auto_co_ban();
  auto_tham_chieu_va_const();
  auto_ham_va_lambda();
  auto_chuyen_gia_modern_cpp();
  auto_cxx20();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}