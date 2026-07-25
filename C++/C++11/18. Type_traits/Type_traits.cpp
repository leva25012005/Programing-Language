#include <iostream>
#include <string>
#include <type_traits> // BẮT BUỘC: Thư viện chứa mọi bùa chú Type Traits
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Type Traits là gì?
    - Là một tập hợp các Struct (Class) đặc biệt giúp Compiler có thể tự đặt câu
 hỏi: "Ê, biến T này có phải là số nguyên không?", "Biến này có phải con trỏ
 không?"
 2. Đặc tính cốt lõi:
    - MỌI THỨ ĐỀU CHẠY LÚC BIÊN DỊCH (Compile-time). Kết quả trả về luôn là hằng
 số.
    - C++11 dùng: std::is_integral<T>::value
    - C++17 rút gọn thành: std::is_integral_v<T> (Thêm chữ _v cho biến boolean,
 _t cho kiểu trả về).
 3. Sứ mệnh vĩ đại:
    - Viết các hàm Template "thông minh" (Hàm tự biết cách ứng xử khác nhau tùy
 theo kiểu dữ liệu truyền vào).
*/

// ==========================================
// CLASS PHỤ TRỢ CHO VÍ DỤ
// ==========================================
class VuKhi {};
class Kiem : public VuKhi {};

// ==========================================
// 1. MỨC CƠ BẢN: ĐẶT CÂU HỎI VỚI COMPILER (INTROSPECTION)
// ==========================================
void type_traits_co_ban() {
  std::cout << "\n--- 1. BAZIC: KIEM TRA KIEU DU LIEU ---\n";

  // a. Kiểm tra tính chất cơ bản
  // C++17 thêm hậu tố '_v' (value) để code ngắn hơn thay vì viết ::value
  std::cout << "int co phai la so nguyen? " << std::is_integral_v<int> << "\n";
  std::cout << "double co phai so nguyen? "
            << std::is_integral_v<double> << "\n";
  std::cout << "double co phai so thuc? "
            << std::is_floating_point_v<double> << "\n";

  // b. Kiểm tra con trỏ và Class
  std::cout << "int* co phai con tro? " << std::is_pointer_v<int *> << "\n";
  std::cout << "VuKhi co phai la Class? " << std::is_class_v<VuKhi> << "\n";

  // c. Kiểm tra quan hệ huyết thống (Kế thừa)
  std::cout << "Kiem co ke thua tu VuKhi? "
            << std::is_base_of_v<VuKhi, Kiem> << "\n";

  // d. So sánh 2 kiểu có giống hệt nhau không?
  std::cout << "int va const int giong nhau khong? "
            << std::is_same_v<int, const int> << " (Khac nhau vi co const)\n";
}

// ==========================================
// 2. MỨC TRUNG CẤP: PHẪU THUẬT KIỂU DỮ LIỆU (MODIFICATION)
// ==========================================
void type_traits_phau_thuat() {
  std::cout << "\n--- 2. INTERMEDIATE: BIEN DOI KIEU (MODIFICATION) ---\n";

  // C++17 dùng hậu tố '_t' (type) để trả về một KIỂU DỮ LIỆU MỚI sau khi đã
  // biến đổi.

  // 1. Lột bỏ tham chiếu (Remove Reference)
  // Biến kiểu 'int&' thành 'int'
  using KieuLoi = std::remove_reference_t<int &>;
  std::cout << "Sau khi xoa &: 'int&' thanh kieu 'int'? "
            << std::is_same_v<KieuLoi, int> << "\n";

  // 2. Lột bỏ Hằng số (Remove Const)
  // Biến kiểu 'const double' thành 'double'
  using KieuThuc = std::remove_const_t<const double>;
  std::cout << "Sau khi xoa const: Giong 'double'? "
            << std::is_same_v<KieuThuc, double> << "\n";

  // 3. Phẫu thuật toàn diện bằng std::decay (Ứng dụng cực nhiều)
  // std::decay_t sẽ lột bỏ mọi thứ rườm rà (const, &, &&) và biến Mảng thành
  // Con trỏ (Giống hệt cách 'auto' hoạt động).
  using MangSieuLoi = std::decay_t<const int(&)[10]>; // Đưa vào: Tham chiếu của
                                                      // Mảng const 10 phần tử
  std::cout << "Sau khi decay, no tro thanh: int* ? "
            << std::is_same_v<MangSieuLoi, int *> << "\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: BÍ THUẬT SFINAE VÀ ENABLE_IF
// ==========================================
/*
 SFINAE (Substitution Failure Is Not An Error) - "Thay thế thất bại KHÔNG phải
 là Lỗi". Đây là ma thuật của C++11: Khi Compiler cố gắng lắp kiểu dữ liệu vào
 Template, nếu nó thấy không khớp (thất bại), nó KHÔNG BÁO LỖI, mà nó lẳng lặng
 ĐI TÌM HÀM KHÁC phù hợp hơn. std::enable_if chính là công tắc để Tắt/Bật các
 hàm Template dựa trên kiểu dữ liệu.
*/

// Hàm 1: CHỈ CHO PHÉP số nguyên (int, long, char...) được chui vào hàm này.
template <typename T>
std::enable_if_t<std::is_integral_v<T>> in_du_lieu(T val) {
  std::cout << "  -> [SFINAE] Xu ly SO NGUYEN: " << val << "\n";
}

// Hàm 2: CHỈ CHO PHÉP số thực (float, double...) được chui vào hàm này.
template <typename T>
std::enable_if_t<std::is_floating_point_v<T>> in_du_lieu(T val) {
  std::cout << "  -> [SFINAE] Xu ly SO THUC: " << val << "\n";
}

void sfinae_enable_if() {
  std::cout << "\n--- 3. ADVANCED: SFINAE & ENABLE_IF (C++11/14) ---\n";

  // Khi gọi in_du_lieu(10), Compiler sẽ thử vào Hàm 2 -> Thất bại (vì int không
  // phải số thực). Nhờ SFINAE, Compiler không báo lỗi mà nhảy lên Hàm 1 ->
  // Khớp! Chạy Hàm 1.
  in_du_lieu(10);   // Gọi hàm 1
  in_du_lieu(3.14); // Gọi hàm 2
  // in_du_lieu("ABC"); // LỖI BIÊN DỊCH! Không có hàm nào kích hoạt cho kiểu
  // chuỗi.
}

// ==========================================
// 4. MỨC CHUYÊN GIA: IF CONSTEXPR (SÁT THỦ DIỆT SFINAE CỦA C++17)
// ==========================================
// std::enable_if rất mạnh, nhưng cú pháp quá xấu và khó đọc.
// Từ C++17, tính năng 'if constexpr' ra đời. Nó thực hiện kiểm tra IF ngay lúc
// BIÊN DỊCH! Nhờ đó, bạn chỉ cần viết MỘT hàm Template duy nhất.

template <typename T> void xu_ly_da_nang_cpp17(T val) {
  // Nếu điều kiện này Đúng, Compiler sẽ CHỈ GIỮ LẠI ĐOẠN CODE NÀY và XÓA SẠCH
  // đoạn else ở dưới.
  if constexpr (std::is_pointer_v<T>) {
    std::cout << "  -> [C++17] Toi biet day la CON TRO! Gia tri goc: " << *val
              << "\n";
  } else if constexpr (std::is_integral_v<T>) {
    std::cout << "  -> [C++17] Day la SO NGUYEN: " << val << "\n";
  } else {
    std::cout << "  -> [C++17] Kieu khac: " << val << "\n";
  }
}

void if_constexpr_cpp17() {
  std::cout << "\n--- 4. EXPERT: IF CONSTEXPR (MODERN C++17) ---\n";

  int x = 99;
  int *ptr = &x;

  xu_ly_da_nang_cpp17(5);       // Vào nhánh số nguyên
  xu_ly_da_nang_cpp17(ptr);     // Vào nhánh con trỏ
  xu_ly_da_nang_cpp17("Chuoi"); // Vào nhánh Else (Kiểu khác)
}

// ==========================================
// 5. TỐI THƯỢNG (C++20): CONCEPTS (TƯƠNG LAI CỦA TYPE TRAITS)
// ==========================================
// Tuy Type Traits rất tốt, C++20 tung ra "Concepts" để việc kiểm tra kiểu dữ
// liệu tự nhiên như ngôn ngữ nói.

#if __cplusplus >= 202002L // Chỉ compile phần này nếu dùng chuẩn C++20 trở lên
#include <concepts>

// Định nghĩa Concept: Chỉ chấp nhận kiểu dữ liệu Số (nguyên hoặc thực)
template <typename T>
concept SoHoc = std::is_arithmetic_v<T>;

// Cú pháp C++20: Quên enable_if đi, chỉ cần dùng chữ 'SoHoc' thay cho
// 'typename' là xong!
template <SoHoc T> void in_so_cpp20(T val) {
  std::cout << "  -> [C++20 Concept] In so: " << val << "\n";
}

void demo_concepts() {
  std::cout << "\n--- 5. MASTER: C++20 CONCEPTS ---\n";
  in_so_cpp20(42);
  in_so_cpp20(9.99);
  // in_so_cpp20("Hello"); // LỖI BIÊN DỊCH SIÊU DỄ ĐỌC: "const char* does not
  // satisfy concept 'SoHoc'"
}
#endif

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  type_traits_co_ban();
  type_traits_phau_thuat();
  sfinae_enable_if();
  if_constexpr_cpp17();

#if __cplusplus >= 202002L
  demo_concepts();
#else
  std::cout << "\n--- Bo qua phan 5 vi Compiler chua bat chuan C++20 ---\n";
#endif

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}