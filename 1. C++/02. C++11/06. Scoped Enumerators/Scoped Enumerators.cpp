#include <cstdint> // Chứa uint8_t, uint16_t...
#include <iostream>
#include <type_traits> // Chứa std::underlying_type_t


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Vấn đề của enum Cổ điển (C-style enum):
    - Ô nhiễm không gian tên (Namespace Pollution): Các giá trị bên trong enum
 sẽ bị "tràn" ra ngoài phạm vi. Nếu bạn có 2 enum cùng chứa chữ "Red", chương
 trình sẽ báo lỗi.
    - Không an toàn kiểu (Type-unsafe): Nó ngầm định (implicit) ép kiểu sang
 'int'. Bạn có thể lấy quả Táo (enum) cộng với con Bò (enum) mà C++ vẫn cho
 phép!

 2. Giải pháp C++11: `enum class` (Scoped Enumerator)
    - Phạm vi nghiêm ngặt: Phải gọi thông qua TênEnum::GiaTri (VD: Color::Red).
 Không bao giờ đụng hàng.
    - An toàn kiểu tuyệt đối: KHÔNG THỂ tự động ép sang 'int'. Tránh được 99%
 lỗi logic ngớ ngẩn.
    - Kiểm soát bộ nhớ: Có thể định nghĩa chính xác nó chiếm bao nhiêu Byte trên
 RAM.
*/

// ==========================================
// 1. MỨC CƠ BẢN: ENUM CỔ ĐIỂN vs ENUM CLASS
// ==========================================

// Enum cổ điển (Unscoped)
enum MauSacCu { Red, Green, Blue };
// enum DenGiaoThong { Red, Yellow }; // LỖI BIÊN DỊCH NGAY LẬP TỨC! Chữ 'Red'
// đã bị MauSacCu chiếm dụng.

// Enum Class C++11 (Scoped - Có phạm vi)
enum class MauSacMoi { Red, Green, Blue };
enum class DenGiaoThong {
  Red,
  Yellow,
  Green
}; // Hoàn toàn hợp lệ! 'Red' này nằm trong vùng an toàn.

void enum_co_ban() {
  std::cout << "\n--- 1. BAZIC: ENUM CLASS VS C-ENUM ---\n";

  // a. Enum Cổ Điển: Tự động biến thành số nguyên (Cực kỳ nguy hiểm)
  MauSacCu mau_cu = Red;  // Tràn chữ Red ra ngoài
  int so_nguyen = mau_cu; // Tự động ép kiểu sang int
  if (mau_cu == 0)
    std::cout << "Mau cu la 0 (Red). Co the so sanh voi so nguyen.\n";

  // b. Enum Class: Phải gọi rõ ràng và Bị cấm ép kiểu ngầm định
  MauSacMoi mau_moi = MauSacMoi::Red;

  // int so_nguyen_2 = mau_moi; // LỖI BIÊN DỊCH! Không thể tự động biến thành
  // int. if (mau_moi == 0)          // LỖI BIÊN DỊCH! Không thể so sánh táo với
  // lê.

  // MUỐN LẤY SỐ NGUYÊN? Phải ép kiểu tường minh (Explicit cast)
  int so_nguyen_antoan = static_cast<int>(mau_moi);
  std::cout << "Gia tri cua MauSacMoi::Red la: " << so_nguyen_antoan << "\n";
}

// ==========================================
// 2. MỨC TRUNG CẤP: TỐI ƯU HÓA BỘ NHỚ (UNDERLYING TYPE)
// ==========================================
// Mặc định, enum chiếm 4 bytes (giống int).
// Nếu bạn chỉ có vài trạng thái, dùng 4 bytes là quá lãng phí.
// C++11 cho phép định nghĩa chính xác kiểu dữ liệu nền (Underlying type).

// Chỉ cần 1 byte (uint8_t) là đủ chứa 256 trạng thái! Rất quan trọng khi lập
// trình Game/Nhúng.
enum class TrangThaiMang : uint8_t { KetNoi = 0, MatKetNoi = 1, DangCho = 2 };

void enum_bo_nho() {
  std::cout << "\n--- 2. INTERMEDIATE: MEMORY OPTIMIZATION ---\n";

  std::cout << "Kich thuoc MauSacCu (mac din int): " << sizeof(MauSacCu)
            << " bytes\n";
  std::cout << "Kich thuoc TrangThaiMang (uint8_t): " << sizeof(TrangThaiMang)
            << " bytes\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: BIT-FLAGS (CỜ BIT) VỚI ENUM CLASS
// ==========================================
// Dùng enum để bật/tắt nhiều tùy chọn cùng lúc (dùng hệ nhị phân 1, 2, 4, 8...)
enum class QuyenHan : uint8_t {
  Khong = 0,    // 0000
  Doc = 1 << 0, // 0001 (1)
  Ghi = 1 << 1, // 0010 (2)
  Xoa = 1 << 2  // 0100 (4)
};

// VẤN ĐỀ: Enum Class cấm các phép toán bit (như | và &) vì nó là kiểu an toàn.
// GIẢI PHÁP: Ta phải "dạy" (Nạp chồng toán tử - Overload) cho C++ biết cách
// cộng 2 QuyenHan.

constexpr QuyenHan operator|(QuyenHan a, QuyenHan b) {
  // Ép sang số -> Cộng bit -> Ép ngược lại thành QuyenHan
  return static_cast<QuyenHan>(static_cast<uint8_t>(a) |
                               static_cast<uint8_t>(b));
}

constexpr QuyenHan operator&(QuyenHan a, QuyenHan b) {
  return static_cast<QuyenHan>(static_cast<uint8_t>(a) &
                               static_cast<uint8_t>(b));
}

void enum_bit_flags() {
  std::cout << "\n--- 3. ADVANCED: ENUM CLASS BIT FLAGS ---\n";

  // Gộp quyền Đọc và Ghi
  QuyenHan quyen_admin = QuyenHan::Doc | QuyenHan::Ghi;

  // Kiểm tra xem có quyền Ghi không?
  if ((quyen_admin & QuyenHan::Ghi) != QuyenHan::Khong) {
    std::cout << "Admin CO quyen Ghi (Write)!\n";
  }

  if ((quyen_admin & QuyenHan::Xoa) == QuyenHan::Khong) {
    std::cout << "Admin KHONG CO quyen Xoa (Delete)!\n";
  }
}

// ==========================================
// 4. MỨC CHUYÊN GIA: C++20 (USING ENUM) & CASTING GENERIC
// ==========================================

// 1. Hàm Template ma thuật: Ép mọi loại enum class ra số nguyên gốc của nó
template <typename E>
constexpr auto to_integral(E e) -> typename std::underlying_type_t<E> {
  return static_cast<typename std::underlying_type_t<E>>(e);
}

void enum_chuyen_gia_cxx20() {
  std::cout << "\n--- 4. EXPERT: C++20 'USING ENUM' & GENERIC CAST ---\n";

  DenGiaoThong den_hien_tai = DenGiaoThong::Green;

  // VẤN ĐỀ TRƯỚC C++20: Switch-case cực kỳ dài dòng vì phải lặp lại tên Enum
  // liên tục
  /*
  switch (den_hien_tai) {
      case DenGiaoThong::Red: ...
      case DenGiaoThong::Yellow: ...
      case DenGiaoThong::Green: ...
  }
  */

  // TÍNH NĂNG C++20: `using enum`
  // Cho phép "bung" các giá trị vào phạm vi hiện tại (chỉ dùng trong khối ngoặc
  // nhọn này)
  switch (den_hien_tai) {
    using enum DenGiaoThong; // Từ dòng này trở đi, không cần gõ chữ
                             // "DenGiaoThong::" nữa!
  case Red:
    std::cout << "Den Do! Dung lai!\n";
    break;
  case Yellow:
    std::cout << "Den Vang! Di cham!\n";
    break;
  case Green:
    std::cout << "Den Xanh! Di thoi! (Su dung C++20 using enum)\n";
    break;
  }

  // Demo hàm template lấy giá trị số nguyên generic
  std::cout << "Gia tri so cua Den Xanh la: " << to_integral(den_hien_tai)
            << "\n";
}

// ==========================================
// TỔNG KẾT LUẬT (Kinh nghiệm thực chiến)
// ==========================================
/*
 1. KHÔNG BAO GIỜ dùng `enum` cổ điển (C-style) nữa. Nó là tàn dư của ngôn ngữ
 C.
 2. LUÔN LUÔN dùng `enum class`. Nếu bạn thấy việc viết tên (Ví dụ Color::Red)
 quá dài, hãy dùng `using enum Color` (từ C++20) trong các phạm vi hàm cục bộ
 nhỏ hẹp để làm gọn code.
 3. Khi làm Game, Hệ thống Nhúng, hoặc Lập trình Mạng (Network Packets):
    Luôn khai báo rõ kích thước bằng `enum class Name : uint8_t` (hoặc 16_t) để
    tiết kiệm RAM, băng thông và kiểm soát byte-alignment chính xác.
*/

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  enum_co_ban();
  enum_bo_nho();
  enum_bit_flags();
  enum_chuyen_gia_cxx20();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}