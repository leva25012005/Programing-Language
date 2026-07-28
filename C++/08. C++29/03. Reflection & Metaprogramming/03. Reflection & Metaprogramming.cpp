#include <iostream>
#include <meta> // Thư viện lõi của C++26 Reflection (Đề xuất P2996)
#include <string>
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY - C++26 & C++29)
// ==========================================
/*
 1. C++26 REFLECTION ("Đọc" cấu trúc code):
    - Dùng toán tử `^` (Reflection operator) để biến một Kiểu dữ liệu (Type),
      Biến (Variable), hoặc Hàm (Function) thành một đối tượng meta
 (std::meta::info).
    - std::meta::info tồn tại hoàn toàn ở Compile-time (lúc biên dịch).
    - Dùng toán tử `[: :]` (Splicing operator) để biến đối tượng meta ngược trở
 lại thành code thật.

 2. C++29 CODE INJECTION ("Viết/Sinh" code mới):
    - Đỉnh cao của Metaprogramming. Thay vì dùng C-Macro (#define) bẩn và nguy
 hiểm, ta có thể dùng C++ thuần để "bơm" (inject) thêm biến, thêm hàm vào một
 Class ngay trong lúc trình biên dịch đang chạy.
*/

// ==========================================
// CLASS MẪU DÙNG ĐỂ THỬ NGHIỆM
// ==========================================
struct NhanVien {
  std::string ten;
  int tuoi;
  double luong;
};

// ==========================================
// 1. MỨC CƠ BẢN (C++26): ĐỌC THÔNG TIN CLASS (INTROSPECTION)
// ==========================================
void cpp26_doc_cau_truc() {
  std::cout << "\n--- 1. BAZIC (C++26): DOC CAU TRUC CLASS ---\n";

  // Toán tử ^ lấy "Linh hồn" (meta info) của struct NhanVien
  constexpr std::meta::info struct_info = ^NhanVien;

  std::cout << "Ten cua struct la: " << std::meta::identifier_of(struct_info)
            << "\n";
  std::cout << "Danh sach cac bien ben trong:\n";

  // Vòng lặp template (Template for-loop): Lặp ngay lúc Compile-time!
  template for (constexpr auto member : std::meta::members_of(struct_info)) {
    // Lấy tên biến và tên kiểu dữ liệu của biến đó
    constexpr auto ten_bien = std::meta::identifier_of(member);
    constexpr auto kieu_du_lieu = std::meta::type_of(member);

    std::cout << "  - Bien: " << ten_bien
              << " | Kieu: " << std::meta::identifier_of(kieu_du_lieu) << "\n";
  }
}

// ==========================================
// 2. MỨC TRUNG CẤP (C++26): TỰ ĐỘNG HÓA HÀM IN RA MÀN HÌNH (SERIALIZATION)
// ==========================================
// Hãy tưởng tượng bạn không bao giờ phải cặm cụi viết hàm in dữ liệu cho từng
// Class nữa! Hàm template dưới đây có thể in BẤT KỲ struct nào.

template <typename T> void tu_dong_in_data(const T &object) {
  std::cout << "Data cua [" << std::meta::identifier_of(^T) << "]:\n";

  // Duyệt qua tất cả các thành viên của T ở Compile-time
  template for (constexpr auto member :
                std::meta::nonstatic_data_members_of(^T)) {

    // [: member :] biến meta_info thành con trỏ tới thành viên thật
    // object.[:member:] tương đương với object.ten, object.tuoi,...
    std::cout << "  " << std::meta::identifier_of(member) << " = "
              << object.[:member:]
        << "\n";
  }
}

void demo_tu_dong_in() {
  std::cout << "\n--- 2. INTERMEDIATE: AUTO SERIALIZATION ---\n";
  NhanVien nv = {"Le Van Nhan", 28, 1500.50};
  tu_dong_in_data(
      nv); // Gọi 1 phát ăn luôn, không cần viết hàm toString() cho NhanVien!
}

// ==========================================
// 3. MỨC NÂNG CAO (GIAO THOA 26-29): KIỂM TRA ĐIỀU KIỆN LÚC BIÊN DỊCH
// ==========================================
// Viết một Concept để cấm không cho biên dịch nếu Class không có biến tên là
// "id"
template <typename T> consteval bool bat_buoc_phai_co_id() {
  for (constexpr auto mem : std::meta::members_of(^T)) {
    if (std::meta::identifier_of(mem) == "id") {
      return true;
    }
  }
  return false;
}

struct SanPham HopLe {
  int id;
  std::string name;
};
struct SanPhamLoi {
  std::string name;
};

void cpp26_kiem_duyet_code() {
  std::cout << "\n--- 3. ADVANCED: COMPILE-TIME VALIDATION ---\n";

  static_assert(bat_buoc_phai_co_id<SanPhamHopLe>(), "Class nay hop le");

  // Dòng dưới nếu bỏ comment sẽ LỖI BIÊN DỊCH ngay lập tức, giúp diệt bug từ
  // trong trứng nước static_assert(bat_buoc_phai_co_id<SanPhamLoi>(), "Class
  // thieu bien 'id'!");

  std::cout << "Kiem duyet Compile-time hoat dong hoan hao.\n";
}

// ==========================================
// 4. MỨC CHUYÊN GIA (C++29 TẦM NHÌN): CODE INJECTION (BƠM CODE)
// ==========================================
/*
 Ở C++29, giấc mơ lớn nhất là ta có thể dùng từ khóa `consteval` block và cú
 pháp bơm `-> fragment` để sinh ra các hàm, các biến lồng thẳng vào bên trong
 một struct khác. Dưới đây là cú pháp mô phỏng cách ta tự động sinh ra toán tử
 so sánh bằng (operator==) cho BẤT KỲ struct nào mà không cần tự viết tay.
*/

template <typename T> consteval void bom_toan_tu_so_sanh() {
  // Bơm thẳng đoạn code sau vào bên trong định nghĩa của T
  ->fragment struct {
    bool operator==(const T &other) const {
      // Lặp qua từng biến để so sánh
      template for (constexpr auto mem :
                    std::meta::nonstatic_data_members_of(^T)) {
        if (this->[:mem:] != other.[:mem:]) {
          return false;
        }
      }
      return true; // Nếu tất cả các biến đều giống nhau
    }
  };
}

// Ứng dụng Bơm Code vào một Class mới tinh
struct QuaiVat {
  std::string he;
  int mau;

  // Kích hoạt tiêm code C++29 ngay tại đây!
  // Trình biên dịch sẽ tự động viết hàm operator== vào chỗ này.
  consteval { bom_toan_tu_so_sanh<QuaiVat>(); }
};

void cpp29_code_injection() {
  std::cout << "\n--- 4. EXPERT (C++29): MACRO-LESS CODE INJECTION ---\n";

  QuaiVat qv1 = {"Lua", 1000};
  QuaiVat qv2 = {"Lua", 1000};
  QuaiVat qv3 = {"Nuoc", 800};

  // Nhờ có Code Injection, qv1 và qv2 có thể dùng toán tử ==
  // mặc dù ta CHƯA TỪNG gõ định nghĩa hàm operator== bằng tay!
  if (qv1 == qv2) {
    std::cout << "Tuyet voi! qv1 va qv2 giong y het nhau.\n";
  }
  if (!(qv1 == qv3)) {
    std::cout << "qv1 va qv3 khac nhau (Chuan xac).\n";
  }
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== KY NGUYEN MOI CUA C++ (REFLECTION & INJECTION) =====\n";

  cpp26_doc_cau_truc();
  demo_tu_dong_in();
  cpp26_kiem_duyet_code();
  cpp29_code_injection();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}