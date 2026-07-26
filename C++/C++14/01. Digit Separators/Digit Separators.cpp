#include <iomanip>
#include <iostream>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Digit Separators (Dấu phân cách chữ số) là gì?
    - Ra mắt chính thức từ phiên bản C++14.
    - Sử dụng dấu nháy đơn (') để phân tách các chữ số bên trong một số.

 2. Sứ mệnh cốt lõi:
    - CHỈ DÀNH CHO CON NGƯỜI ĐỌC: Giúp mắt người dễ dàng phân biệt hàng nghìn,
 hàng triệu, hoặc nhóm các bit/byte mà không bị hoa mắt đếm số lượng số 0.
    - ĐỐI VỚI TRÌNH BIÊN DỊCH: Nó hoàn toàn bị "ngó lơ" và xóa sạch trong quá
 trình biên dịch.

 3. Hiệu năng:
    - Zero-overhead (Hoàn toàn không tốn thêm bất kỳ 1 byte RAM hay chu kỳ CPU
 nào lúc chương trình chạy).
*/

// ==========================================
// 1. MỨC CƠ BẢN: CÁC HỆ SỐ THÔNG THƯỜNG (CƠ SỐ 10 & SỐ THỰC)
// ==========================================
void separator_co_ban() {
  std::cout << "\n--- 1. BAZIC: HE CO SO 10 VA SO THUC ---\n";

  // Trước C++14: Phải ngồi đếm xem có bao nhiêu số 0
  long long tien_ty_cu = 1000000000;

  // Từ C++14: Rõ ràng, rành mạch
  long long tien_ty_moi = 1'000'000'000;

  // Đối với số thực (Floating-point)
  double pi = 3.141'592'653'589;
  double khoang_cach =
      1'495.97'870'700; // Bạn có thể ngắt ở bất kỳ đâu bạn muốn

  std::cout << "1 Ty: " << tien_ty_moi << "\n";
  std::cout << std::setprecision(14) << "So Pi: " << pi << "\n";
}

// ==========================================
// 2. MỨC TRUNG CẤP: LẬP TRÌNH BẬC THẤP (HEX, BINARY, OCTAL)
// ==========================================
void separator_he_co_so_khac() {
  std::cout << "\n--- 2. INTERMEDIATE: NHOM BIT VA BYTE ---\n";

  // a. HỆ NHỊ PHÂN (Binary - Tiền tố 0b hoặc 0B)
  // Thường dùng separator để gom nhóm mỗi 4 bit (1 Nibble) hoặc 8 bit (1 Byte)
  int mask_nhi_phan = 0b1101'0010'1010'1111;

  // b. HỆ THẬP LỤC PHÂN (Hexadecimal - Tiền tố 0x hoặc 0X)
  // Thường gom nhóm 2 ký tự = 1 Byte. Rất hữu ích khi biểu diễn màu HEX (RGBA)
  // hoặc địa chỉ RAM
  long mau_sac_rgba = 0xFF'80'00'AA;

  // c. HỆ BÁT PHÂN (Octal - Tiền tố 0)
  int phan_quyen_linux = 0'777;

  std::cout << "Mau RGBA (In ra he 10): " << mau_sac_rgba << "\n";
  std::cout << "Mask nhi phan (In ra he 10): " << mask_nhi_phan << "\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: NHỮNG LUẬT LỆ KHẮT KHE (CÚ PHÁP)
// ==========================================
void separator_luat_le() {
  std::cout << "\n--- 3. ADVANCED: CAC QUY TAC CAM KY ---\n";

  // Quy tắc 1: Không được đặt ở ĐẦU hoặc CUỐI con số
  // int a = '1000; // LỖI BIÊN DỊCH! Trình biên dịch nghĩ đây là ký tự (char)
  // bị thiếu dấu nháy đóng int b = 1000'; // LỖI BIÊN DỊCH!

  // Quy tắc 2: Không được đứng kề bên tiền tố hệ cơ số
  // int hex_loi = 0x'FF; // LỖI BIÊN DỊCH! Dấu nháy dính liền với 'x'
  int hex_chuan = 0x00'FF; // ĐÚNG. Dấu nháy nằm giữa các chữ số

  // Quy tắc 3: Không được đứng kề dấu chấm thập phân (.)
  // double pi_loi_1 = 3.'14; // LỖI BIÊN DỊCH!
  // double pi_loi_2 = 3'.14; // LỖI BIÊN DỊCH!
  double pi_chuan = 3.1'4; // ĐÚNG. Nằm giữa số 1 và số 4

  // Quy tắc 4: Không được dùng 2 dấu nháy liên tiếp
  // int trieu_loi = 1''000''000; // LỖI BIÊN DỊCH!

  // Quy tắc 5: ĐƯỢC PHÉP kết hợp với các Hậu tố (Suffix: U, L, LL, f)
  // Nhưng cũng không được dính liền với chữ cái của hậu tố.
  unsigned long long dung_luong_ram = 16'000'000'000ULL;
  float chieu_cao = 1'75.5f;

  std::cout << "Dung luong RAM: " << dung_luong_ram << " bytes\n";
}

// ==========================================
// 4. MỨC CHUYÊN GIA: KẾT HỢP USER-DEFINED LITERALS (C++11)
// ==========================================
// C++ cho phép ta tự định nghĩa các hậu tố của riêng mình.
// Khi kết hợp với Digit Separators, code C++ sẽ đọc hệt như tiếng Anh chuẩn!

// Viết hậu tố '_km' để đổi từ Kilomet sang Met
constexpr long double operator"" _km(long double km) { return km * 1000.0; }

// Viết hậu tố '_VND' (Mô phỏng trả về chuỗi)
std::string operator"" _VND(unsigned long long tien) {
  return std::to_string(tien) + " Viet Nam Dong";
}

void separator_chuyen_gia_literals() {
  std::cout
      << "\n--- 4. EXPERT: KET HOP TIEP VI NGU TU DINH NGHIA (LITERALS) ---\n";

  // Vừa dùng Separator cho dễ đọc, vừa dùng Literal để tự động quy đổi đơn vị!
  long double khoang_cach = 1'200.50_km;

  auto tai_khoan = 500'000'000_VND;

  std::cout << "1'200.50_km chuyen thanh met: " << khoang_cach << " m\n";
  std::cout << "Tai khoan cua ban co: " << tai_khoan << "\n";
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  separator_co_ban();
  separator_he_co_so_khac();
  separator_luat_le();
  separator_chuyen_gia_literals();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}