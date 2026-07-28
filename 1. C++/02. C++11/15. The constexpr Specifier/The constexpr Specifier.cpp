#include <iostream>
#include <numeric>
#include <string>
#include <type_traits>
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. constexpr (Constant Expression) là gì?
    - Ra mắt từ C++11, nó là một "mệnh lệnh" gửi đến Trình biên dịch (Compiler):
      "Hãy tính toán giá trị này/chạy hàm này NGAY LÚC BIÊN DỊCH (Compile-time)
       để lúc phần mềm chạy (Runtime) không phải tốn CPU tính toán nữa."

 2. Sự khác biệt sống còn giữa const và constexpr:
    - const: "Biến này chỉ đọc (read-only), không được phép sửa." (Nhưng giá trị
 của nó có thể được nạp vào lúc chạy).
    - constexpr: "Biến này bắt buộc phải biết giá trị TRƯỚC KHI CHƯƠNG TRÌNH
 CHẠY, và không được phép sửa." Mọi constexpr đều ngầm định là const, nhưng
 const thì chưa chắc là constexpr.

 3. Triết lý thiết kế (Zero-Cost Abstraction):
    - Dịch chuyển gánh nặng tính toán từ Máy của người dùng (Lúc chạy) sang Máy
 của lập trình viên (Lúc biên dịch).
*/

// ==========================================
// 1. MỨC CƠ BẢN: BIẾN CONSTEXPR VÀ SỰ KHÁC BIỆT VỚI CONST
// ==========================================
void constexpr_co_ban() {
  std::cout << "\n--- 1. BAZIC: BIEN CONSTEXPR VS CONST ---\n";

  int x = 5; // Giá trị x chỉ có khi chương trình chạy

  const int c1 = 10; // C++ biết c1 = 10 lúc biên dịch
  const int c2 =
      x; // Hợp lệ! c2 nhận giá trị 5 lúc chạy, và khóa không cho sửa nữa.

  constexpr int ce1 = 10; // Hợp lệ! Trình biên dịch biết số 10.
  // constexpr int ce2 = x; // LỖI BIÊN DỊCH! Trình biên dịch không biết 'x' là
  // số mấy trước khi chạy.

  // Ứng dụng: Cần Compile-time constant cho mảng tĩnh tĩnh
  int mang_1[c1];  // Hợp lệ
  int mang_2[ce1]; // Hợp lệ và chuẩn mực nhất cho C++
  // int mang_3[x];  // Lỗi trên C++ chuẩn (dù một số compiler như GCC lén lút
  // hỗ trợ VLA).
}

// ==========================================
// 2. MỨC TRUNG CẤP: HÀM CONSTEXPR (C++11 vs C++14)
// ==========================================
/*
 C++11: Hàm constexpr cực kỳ khắt khe. Nó chỉ được phép có ĐÚNG 1 câu lệnh
 'return'. C++14: Thả lỏng toàn bộ. Hàm constexpr được phép dùng vòng lặp (for,
 while), if-else, và khai báo biến cục bộ (phải khởi tạo ngay).
*/

// Hàm C++14: Tính giai thừa dùng vòng lặp bình thường
constexpr long long tinh_giai_thua(int n) {
  long long ket_qua = 1;
  for (int i = 1; i <= n; ++i) {
    ket_qua *= i;
  }
  return ket_qua;
}

void constexpr_ham() {
  std::cout << "\n--- 2. INTERMEDIATE: HAM CONSTEXPR ---\n";

  // TRƯỜNG HỢP 1: Cung cấp tham số tĩnh -> Hàm chạy NGAY LÚC BIÊN DỊCH.
  // Máy tính của người dùng sẽ không phải chạy vòng lặp, nó thấy thẳng số
  // 3628800 trong RAM.
  constexpr long long gt_10 = tinh_giai_thua(10);
  std::cout << "Giai thua (tinh luc bien dich): " << gt_10 << "\n";

  // TRƯỜNG HỢP 2: Cung cấp tham số động -> Trình biên dịch sẽ "tha" cho hàm này
  // và âm thầm biến nó thành hàm bình thường, CHẠY LÚC RUNTIME!
  int so_nhap_vao = 5; // Tưởng tượng đây là std::cin >>
  long long gt_dong = tinh_giai_thua(so_nhap_vao);
  std::cout << "Giai thua (tinh luc runtime vi tham so la bien so): " << gt_dong
            << "\n";

  // -> Đây là sức mạnh của constexpr: 1 hàm dùng được cho cả Compile-time và
  // Runtime!
}

// ==========================================
// 3. MỨC NÂNG CAO: CLASS VÀ OBJECT CONSTEXPR
// ==========================================
// Bạn có thể tạo ra các Object Phức tạp sẵn từ lúc Biên dịch (Nạp thẳng vào
// file .exe)
class ToaDo {
private:
  double x, y;

public:
  // Constructor constexpr: Chỉ được khởi tạo các thành viên, không dùng bộ nhớ
  // động
  constexpr ToaDo(double x_val, double y_val) : x(x_val), y(y_val) {}

  // Các hàm getter cũng phải là constexpr
  constexpr double getX() const { return x; }
  constexpr double getY() const { return y; }

  // Hàm xử lý logic tính toán
  constexpr double khoang_cach_binh_phuong() const { return x * x + y * y; }
};

void constexpr_class() {
  std::cout << "\n--- 3. ADVANCED: LOP VA DOI TUONG CONSTEXPR ---\n";

  // Object này ĐÃ TỒN TẠI ở trạng thái hoàn chỉnh ngay trong file thực thi.
  constexpr ToaDo diem_A(3.0, 4.0);

  // Hàm này được tính toán ngay lúc biên dịch, gán sẵn số 25.0
  constexpr double kc = diem_A.khoang_cach_binh_phuong();

  std::cout << "Khoang cach binh phuong cua diem_A la: " << kc
            << " (Da tinh xong truoc khi chay)\n";
}

// ==========================================
// 4. MỨC CHUYÊN GIA: IF CONSTEXPR (C++17)
// ==========================================
// 'if constexpr' kiểm tra điều kiện ngay lúc biên dịch.
// Nhánh nào (if hoặc else) sai, trình biên dịch SẼ XÓA SẠCH nhánh đó khỏi file
// .exe. Ứng dụng cực mạnh trong Generic Programming (Template).

template <typename T> void in_thong_tin_kieu(T gia_tri) {
  // Dùng Type Traits để hỏi thông tin kiểu dữ liệu
  if constexpr (std::is_integral_v<T>) {
    std::cout << "Day la so nguyen: " << gia_tri << "\n";
  } else if constexpr (std::is_floating_point_v<T>) {
    std::cout << "Day la so thuc: " << gia_tri << "\n";
    // Nhờ if constexpr, nếu T là chuỗi, đoạn code %f ở dưới (nếu có) sẽ không
    // bị báo lỗi vì compiler đã ném nhánh này vào sọt rác trước khi kiểm tra cú
    // pháp!
  } else {
    std::cout << "Day la kieu du lieu khac: " << gia_tri << "\n";
  }
}

void if_constexpr_cxx17() {
  std::cout << "\n--- 4. EXPERT: IF CONSTEXPR (C++17) ---\n";

  in_thong_tin_kieu(100);    // Compiler chỉ sinh ra code của nhánh 1
  in_thong_tin_kieu(3.1415); // Compiler chỉ sinh ra code của nhánh 2
  in_thong_tin_kieu("C++");  // Compiler chỉ sinh ra code của nhánh 3
}

// ==========================================
// 5. HIỆN ĐẠI TỐI THƯỢNG: CONSTEXPR DYNAMIC ALLOCATION (C++20) & CONSTEVAL
// ==========================================

/*
 a. C++20 cho phép dùng cấp phát động (new/delete), std::vector, std::string
 bên trong hàm constexpr!
 Điều kiện duy nhất: Vùng nhớ đó phải được "dọn dẹp" (hủy) TRƯỚC KHI hàm kết
 thúc. Bạn không thể trả về một std::vector ra ngoài Compile-time được, nhưng
 bạn có thể dùng nó để tính toán một con số rồi trả về con số đó.
*/
constexpr int tinh_tong_bang_vector() {
  std::vector<int> mang_ao; // C++20: Cấp phát mảng động NGAY LÚC BIÊN DỊCH!
  for (int i = 1; i <= 5; ++i) {
    mang_ao.push_back(i * 10);
  }

  int tong = 0;
  for (int x : mang_ao)
    tong += x;

  return tong;
  // Khi return, mang_ao bị hủy (thu hồi RAM). Hợp lệ 100%!
}

/*
 b. CONSTEVAL (C++20 - Hàm Tức thời):
 Như đã học ở phần 2, hàm `constexpr` có thể bị biến thành hàm Runtime nếu
 truyền tham số động. Nếu bạn BẮT BUỘC muốn hàm đó PHẢI CHẠY LÚC BIÊN DỊCH, nếu
 không thì báo lỗi, hãy dùng `consteval`.
*/
consteval int phep_toan_bat_buoc_bien_dich(int a, int b) {
  return a * a + b * b;
}

void constexpr_cxx20() {
  std::cout << "\n--- 5. MODERN C++20: DYNAMIC MEMORY & CONSTEVAL ---\n";

  // Mảng động std::vector được tạo, tính toán và hủy đi ngay trong đầu Trình
  // biên dịch.
  constexpr int tong_so = tinh_tong_bang_vector();
  std::cout << "Tong so tu std::vector luc compile-time: " << tong_so << "\n";

  // consteval bắt buộc chạy lúc compile-time
  constexpr int ket_qua = phep_toan_bat_buoc_bien_dich(3, 4);
  std::cout << "Consteval (3^2 + 4^2): " << ket_qua << "\n";

  int x = 5;
  // int loi = phep_toan_bat_buoc_bien_dich(x, 4);
  // LỖI BIÊN DỊCH! 'x' là biến runtime, consteval cấm ngặt việc này (trong khi
  // constexpr thì tha).
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  constexpr_co_ban();
  constexpr_ham();
  constexpr_class();
  if_constexpr_cxx17();
  constexpr_cxx20();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}