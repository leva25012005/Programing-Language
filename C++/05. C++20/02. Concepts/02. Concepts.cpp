#include <concepts> // Thư viện BẮT BUỘC để dùng các Concepts chuẩn có sẵn
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Nỗi đau lịch sử (Duck Typing của Template Cũ):
    - Trước C++20, Template hoat động theo kiểu: "Cứ nhận bừa vào đi, chạy thử
 xem code có biên dịch được không. Nếu không, tao sẽ ném ra 500 dòng lỗi (Error
 Vomit) để mày tự mò".

 2. C++20 Concepts ra đời:
    - Nó là một "Bản Hợp Đồng". Nó kiểm tra kiểu dữ liệu TRƯỚC KHI cho phép
 Template hoạt động.
    - Trình biên dịch: "Ê hàm tính Tổng, mày chỉ nhận Số Nguyên thôi đúng không?
 Thằng Dev vừa truyền vào Chuỗi kìa. Tao báo lỗi đúng 1 dòng ở ngay chỗ gọi hàm
 luôn nhé!"

 3. Hai khái niệm cốt lõi phải phân biệt:
    - Khái niệm (Concept): Là tên của bản hợp đồng (Ví dụ: std::integral).
    - Từ khóa (Requires): Dùng để ép Template phải ký vào bản hợp đồng đó.
*/

// ==========================================
// 1. MỨC CƠ BẢN: 4 CÚ PHÁP SỬ DỤNG CONCEPTS
// ==========================================
// Đều mang một ý nghĩa: "Hàm này chỉ tính tổng cho Số nguyên (int, long,
// short...)"

// Cú pháp 1: Dùng từ khóa requires SAU danh sách Template (Kinh điển nhất)
template <typename T>
  requires std::integral<T>
T cong_cach_1(T a, T b) {
  return a + b;
}

// Cú pháp 2: Viết tắt trực tiếp vào danh sách Template (Gọn gàng nhất)
template <std::integral T> T cong_cach_2(T a, T b) { return a + b; }

// Cú pháp 3: Dùng requires SAU chữ ký hàm (Trailing requires - Thường dùng cho
// Class Methods)
template <typename T>
T cong_cach_3(T a, T b)
  requires std::integral<T>
{
  return a + b;
}

// Cú pháp 4: C++20 Abbreviated Function Template (Đỉnh cao của sự lười biếng,
// khuyên dùng)
auto cong_cach_4(std::integral auto a, std::integral auto b) { return a + b; }

void concept_co_ban() {
  std::cout << "\n--- 1. BAZIC: 4 CACH VIET CONCEPT ---\n";
  std::cout << "Ket qua: " << cong_cach_4(10, 20) << "\n";

  // Nếu bạn bỏ comment dòng dưới, Compiler C++20 sẽ báo 1 dòng lỗi cực kỳ thanh
  // lịch: "error: no matching function for call... constraints not satisfied...
  // the concept 'integral<double>' evaluated to false"

  // std::cout << cong_cach_4(3.14, 2.0); // LỖI BIÊN DỊCH BẢO VỆ!
}

// ==========================================
// 2. MỨC TRUNG CẤP: TỰ ĐỊNH NGHĨA CONCEPTS (CUSTOM CONCEPTS)
// ==========================================
// Bạn hoàn toàn có thể tự tạo ra những "Bản hợp đồng" mang tên riêng của mình
// bằng cách kết hợp các Concepts có sẵn bằng toán tử Logic (&&, ||)

// Tạo Concept: "Số Lớn" (Phải là số nguyên VÀ kích thước RAM phải từ 4 bytes
// trở lên)
template <typename T>
concept SoNguyenLon = std::integral<T> && (sizeof(T) >= 4);

// Tạo Concept: "Số Bất Kỳ" (Là số nguyên HOẶC số thực)
template <typename T>
concept LaConSo = std::integral<T> || std::floating_point<T>;

void xu_ly_so_lon(SoNguyenLon auto x) {
  std::cout << "  -> So nguyen lon hop le: " << x << "\n";
}

void concept_tu_dinh_nghia() {
  std::cout << "\n--- 2. INTERMEDIATE: CUSTOM CONCEPTS ---\n";

  long long so_bu = 999999999;
  xu_ly_so_lon(so_bu); // Hợp lệ (long long thường là 8 bytes)

  int so_int = 10;
  xu_ly_so_lon(so_int); // Hợp lệ (int là 4 bytes)

  short so_nho = 5;
  // xu_ly_so_lon(so_nho); // LỖI BIÊN DỊCH! Vì short chỉ có 2 bytes. (Vi phạm
  // sizeof >= 4)
}

// ==========================================
// 3. MỨC NÂNG CAO: REQUIRES EXPRESSION (MÔ TẢ CẤU TRÚC)
// ==========================================
// Đôi khi bạn muốn kiểm tra: "Kiểu dữ liệu này có hàm .size() không?", "Nó có
// hàm .begin() không?" Dùng `requires expression` (Khối requires) để kiểm tra
// HÀNH VI của một kiểu dữ liệu.

template <typename T>
concept LaVatChua = requires(T container) {
  // 1. Simple Requirement: Kiểm tra xem các hàm này CÓ TỒN TẠI VÀ BIÊN DỊCH
  // ĐƯỢC KHÔNG?
  container.begin();
  container.end();
  container.clear();

  // 2. Type Requirement: Kiểm tra xem nó CÓ CHỨA kiểu dữ liệu nội bộ
  // (value_type) không?
  typename T::value_type;

  // 3. Compound Requirement: Ép kiểu trả về của một biểu thức phải đúng chuẩn!
  // Ý nghĩa: Hàm .size() phải trả về một thứ gì đó có thể ép kiểu thành
  // std::size_t
  { container.size() } -> std::convertible_to<std::size_t>;
};

// Hàm này BẮT BUỘC chỉ nhận vào các Container (như vector, list, set...) có đủ
// begin, end, size, clear.
void in_vat_chua(const LaVatChua auto &v) {
  std::cout << "  Vat chua co kich thuoc: " << v.size() << "\n";
}

void concept_requires_expression() {
  std::cout << "\n--- 3. ADVANCED: REQUIRES EXPRESSION ---\n";

  std::vector<int> mang = {1, 2, 3};
  in_vat_chua(
      mang); // Hoạt động hoàn hảo vì std::vector đạt 100% chuẩn LaVatChua

  int so_le = 5;
  // in_vat_chua(so_le); // LỖI BIÊN DỊCH! 'int' không có hàm .begin() hay
  // .size().
}

// ==========================================
// 4. MỨC CHUYÊN GIA: PHÂN GIẢI NẠP CHỒNG (OVERLOAD RESOLUTION)
// ==========================================
// Concepts không chỉ dùng để BÁO LỖI, nó còn dùng để ĐIỀU HƯỚNG TỰ ĐỘNG!
// Khi có nhiều hàm trùng tên, Trình biên dịch sẽ ưu tiên chọn hàm có Concept
// KHẮT KHE NHẤT.

// Hàm 1: Ít khắt khe (Nhận mọi thứ)
template <typename T> void in_du_lieu(T x) {
  std::cout << "  [Mac Dinh] Khong the phan loai: " << x << "\n";
}

// Hàm 2: Khắt khe hơn (Chỉ nhận con số)
template <LaConSo T> void in_du_lieu(T x) {
  std::cout << "  [LaConSo] Day la mot con so: " << x << "\n";
}

// Hàm 3: Khắt khe NHẤT (Vừa là con số, vừa phải là số nguyên, vừa phải > 4
// bytes)
template <SoNguyenLon T> void in_du_lieu(T x) {
  std::cout << "  [SoNguyenLon] Day la mot so nguyen khong lo: " << x << "\n";
}

void concept_overload_resolution() {
  std::cout << "\n--- 4. EXPERT: OVERLOAD RESOLUTION (DAN DUONG) ---\n";

  // Truyền chuỗi -> Hàm 2 và 3 thất bại -> Rơi vào Hàm 1
  in_du_lieu("Xin Chao");

  // Truyền số thực (double) -> Hàm 3 thất bại -> Lọt vào Hàm 2 (LaConSo)
  in_du_lieu(3.14159);

  // Truyền int 4 bytes -> Đạt cả 3 hàm -> Nhưng Hàm 3 KHẮT KHE NHẤT -> Ưu tiên
  // chọn Hàm 3!
  in_du_lieu(10000);
}

// ==========================================
// 5. C++20 REQUIRES VỚI CONSTEVAL (KẾT HỢP LOGIC COMPILE-TIME)
// ==========================================
// Một khái niệm lắt léo: Từ khóa requires có thể được dùng 2 lần liên tiếp!
// requires thứ nhất: Mở đầu bản hợp đồng (Clause)
// requires thứ hai: Bắt đầu một khối kiểm tra hành vi (Expression)

template <typename T>
  requires requires(T a, T b) {
    a + b;
  } // Chỉ nhận những kiểu T nào có hỗ trợ dấu '+'
void cong_va_in(T a, T b) {
  std::cout << "  Tong la: " << (a + b) << "\n";
}

void concept_requires_requires() {
  std::cout << "\n--- 5. MASTER: REQUIRES REQUIRES ---\n";

  cong_va_in(5, 10); // Hợp lệ (int có dấu +)
  cong_va_in(std::string("Hello "),
             std::string("C++20")); // Hợp lệ (string có dấu +)

  // struct VoDung {};
  // cong_va_in(VoDung{}, VoDung{}); // LỖI BIÊN DỊCH! VoDung không hỗ trợ toán
  // tử +
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== KHOA HOC C++20 CONCEPTS =====\n";

  concept_co_ban();
  concept_tu_dinh_nghia();
  concept_requires_expression();
  concept_overload_resolution();
  concept_requires_requires();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}