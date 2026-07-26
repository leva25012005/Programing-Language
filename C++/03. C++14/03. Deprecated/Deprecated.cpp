#include <iostream>
#include <string>

// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. [[deprecated]] là gì?
    - Là một thuộc tính (Attribute) chuẩn hóa từ C++14.
    - Sứ mệnh: Báo hiệu cho các Lập trình viên khác biết rằng đoạn code này ĐÃ
 LỖI THỜI, không an toàn, hoặc sắp bị xóa sổ trong phiên bản tương lai.
    - Kết quả: Khi ai đó cố tình gọi hàm/lớp này, Trình biên dịch (Compiler) sẽ
 văng ra một CẢNH BÁO (Warning), nhưng chương trình VẪN ĐƯỢC BIÊN DỊCH VÀ CHẠY
 BÌNH THƯỜNG.

 2. Nguyên tắc "Tâm Huyết":
    - Đừng bao giờ dùng [[deprecated]] trống không. HÃY LUÔN ĐỂ LẠI TIN NHẮN!
    - Tin nhắn phải trả lời được câu hỏi: "Tại sao nó lỗi thời? Và tôi phải dùng
 cái gì để thay thế?"
*/

// ==========================================
// 1. MỨC CƠ BẢN: ĐÁNH DẤU HÀM VÀ BIẾN
// ==========================================

// Cấp độ 1: Lỗi thời không có tin nhắn (KHÔNG KHUYÊN DÙNG vì rất vô tâm)
[[deprecated]]
void tinh_toan_cu() {
  std::cout << "Dang chay thuat toan cu...\n";
}

// Cấp độ 2: Lỗi thời CÓ tin nhắn (CHUẨN BEST PRACTICE)
[[deprecated("Ham nay rat cham. Hay dung tinh_toan_v2() de thay the!")]]
void tinh_toan_v1() {
  std::cout << "Dang chay thuat toan v1...\n";
}

void tinh_toan_v2() { std::cout << "Dang chay thuat toan v2 sieu toc!\n"; }

// Đánh dấu lỗi thời cho một Biến toàn cục
[[deprecated(
    "Bien 'so_pi_cu' thieu chinh xac, hay dung 'PI_MOI' tu thu vien toan hoc")]]
double so_pi_cu = 3.14;

// ==========================================
// 2. MỨC TRUNG CẤP: LỚP (CLASS), CẤU TRÚC (STRUCT) VÀ ENUM
// ==========================================

// Đánh dấu toàn bộ một Lớp (Class)
class [[deprecated(
    "Class HocSinh da cu, hay chuyen sang dung SinhVien")]] HocSinh {
public:
  void chao() { std::cout << "Em la hoc sinh.\n"; }
};

// Hoặc chỉ đánh dấu MỘT PHƯƠNG THỨC BÊN TRONG Lớp
class SinhVien {
public:
  void ma_so_moi() { std::cout << "MSSV: 2026...\n"; }

  [[deprecated("Phuong thuc in_ma_so_cu() da bi xoa bo, dung ma_so_moi() nhe")]]
  void in_ma_so_cu() {}
};

// Đánh dấu lỗi thời cho MỘT GIÁ TRỊ CỤ THỂ TRONG ENUM (Rất hay dùng khi update
// logic)
enum class TrangThaiHeThong {
  DangChay,
  DungHoatDong,
  // Đánh dấu giá trị 'Loi' là lỗi thời, khuyến khích tách ra mã lỗi riêng
  Loi [[deprecated(
      "Khong dung TrangThaiHeThong::Loi nua, hay dung Enum MaLoi rieng")]]
};

// ==========================================
// 3. MỨC NÂNG CAO: TÊN MIỀN (NAMESPACE) VÀ TYPEDEF
// ==========================================

// Tái cấu trúc (Refactor) toàn bộ thư viện: Khai tử một Namespace cũ
namespace [[deprecated("Toan bo namespace api_v1 se bi xoa vao nam 2027. Hay "
                       "migrate sang api_v2")]] api_v1 {
void ket_noi_database() {}
} // namespace api_v1

namespace api_v2 {
void ket_noi_database_nhanh_hon() {}
} // namespace api_v2

// Đánh dấu lỗi thời cho một Bí danh (Alias / Typedef)
using ChuoiCu
    [[deprecated("Dung std::string truc tiep thay vi dung ChuoiCu")]] =
        std::string;

// ==========================================
// 4. MỨC CHUYÊN GIA: ÉP TRÌNH BIÊN DỊCH IM LẶNG (SUPPRESS WARNINGS)
// ==========================================
/*
 Tình huống thực tế: Bạn biết hàm tinh_toan_v1() đã lỗi thời,
 nhưng dự án của bạn quá lớn, sếp chưa cho phép sửa code cũ ngay lúc này.
 Bạn không muốn mỗi lần compile lại bị văng ra 1000 cái Warnings rác màu vàng
 làm trôi mất log quan trọng.

 Giải pháp: Dùng #pragma để RA LỆNH cho Trình biên dịch "nhắm mắt làm ngơ" ở
 đúng khu vực này.
*/

void chay_code_cu_ma_khong_bi_nhac_nho() {
  std::cout << "\n--- 4. EXPERT: EP COMPILER IM LANG ---\n";

  // TẮT CẢNH BÁO (Bắt đầu khu vực vô cảm)
#if defined(__GNUC__) || defined(__clang__) // Dành cho GCC và Clang
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER) // Dành cho Visual Studio (MSVC)
#pragma warning(push)
#pragma warning(disable : 4996)
#endif

  // BẠN CÓ THỂ GỌI HÀM LỖI THỜI Ở ĐÂY MÀ KHÔNG BỊ COMPILER CHỬI (0 WARNINGS)
  tinh_toan_v1();
  so_pi_cu = 3.1415;
  api_v1::ket_noi_database();

  // BẬT LẠI CẢNH BÁO CHO CÁC ĐOẠN CODE BÊN DƯỚI (Trả lại sự bình thường)
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif

  std::cout << "Da chay xong code cu mot cach im lang!\n";
}

// ==========================================
// 5. MỨC BẬC THẦY: KIỂM TRA PHIÊN BẢN C++ TRƯỚC KHI DEPRECATE
// ==========================================
// Vì [[deprecated]] chỉ có từ C++14, nếu bạn viết thư viện cho khách hàng dùng
// C++11, nó sẽ báo lỗi đỏ. Ta dùng Macro (Tiền xử lý) để bọc nó lại một cách
// linh hoạt.

#if __cplusplus >= 201402L // Nếu là C++14 trở lên
#define MY_LIB_DEPRECATED(msg) [[deprecated(msg)]]
#elif defined(__GNUC__) || defined(__clang__) // Nếu là C++11 trên GCC/Clang
#define MY_LIB_DEPRECATED(msg) __attribute__((deprecated(msg)))
#elif defined(_MSC_VER) // Nếu là C++11 trên Visual Studio
#define MY_LIB_DEPRECATED(msg) __declspec(deprecated(msg))
#else
#define MY_LIB_DEPRECATED(                                                     \
    msg) // Xấu nhất: Không hỗ trợ thì làm cho nó tàng hình
#endif

// Sử dụng Macro siêu tương thích vừa viết:
MY_LIB_DEPRECATED("Ham nay an toan tren moi nen tang tu C++98 den C++20")
void ham_thu_vien_tu_viet() {}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== DEMO THUOC TINH [[deprecated]] =====\n\n";

  // 1. Chạy bình thường (Compiler sẽ văng ra Warnings màu vàng lúc biên dịch)
  /*
     BỎ COMMENT 2 DÒNG DƯỚI NẾU BẠN MUỐN THẤY WARNINGS CỦA COMPILER:
     tinh_toan_v1();
     HocSinh hs;
  */

  // 2. Chạy hàm chuyên gia (Im lặng tuyệt đối)
  chay_code_cu_ma_khong_bi_nhac_nho();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}