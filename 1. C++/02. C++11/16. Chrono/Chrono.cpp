#include <chrono>
#include <ctime> // Thư viện C-style time để hiển thị giờ hệ thống
#include <iomanip> // Dùng để định dạng in ngày tháng
#include <iostream>
#include <thread> // Dùng để giả lập hàm sleep (tạm dừng chương trình)

// Cho phép sử dụng các hậu tố viết tắt thời gian của C++14 (như 's', 'ms', 'h')
using namespace std::chrono_literals;

// ==========================================
// 1. MỨC CƠ BẢN: DURATION (KHOẢNG THỜI GIAN)
// ==========================================
void chrono_co_ban_duration() {
  std::cout << "\n--- 1. BAZIC: KHOANG THOI GIAN (DURATION) ---\n";

  // Khởi tạo các khoảng thời gian bằng C++14 Literals
  auto thoi_gian_hoc = 2h;     // 2 hours (Giờ)
  auto thoi_gian_nghi = 30min; // 30 minutes (Phút)
  auto thoi_gian_cho = 500ms;  // 500 milliseconds (Mili-giây)
  auto do_tre = 50us;          // 50 microseconds (Micro-giây)

  // Tính toán trực tiếp trên thời gian (Không cần quy đổi thủ công!)
  auto tong_thoi_gian = thoi_gian_hoc + thoi_gian_nghi; // 2h 30m

  // ĐỂ IN RA SỐ: Bắt buộc phải ép về một đơn vị cụ thể và gọi hàm .count()
  // Lưu ý: Ép từ đơn vị LỚN (giờ) sang đơn vị NHỎ (phút) thì C++ TỰ ĐỘNG LÀM.
  std::chrono::minutes tong_phut = tong_thoi_gian;

  std::cout << "Tong thoi gian hoc (Phut): " << tong_phut.count() << " phut\n";
  std::cout << "Thoi gian cho (Mili-giay): " << thoi_gian_cho.count()
            << " ms\n";
}

// ==========================================
// 2. MỨC TRUNG CẤP: ĐO LƯỜNG THỜI GIAN THỰC THI (STEADY_CLOCK)
// ==========================================
void chrono_do_luong_hieu_nang() {
  std::cout << "\n--- 2. INTERMEDIATE: DO LUONG THOI GIAN THUC THI ---\n";

  /* LÝ THUYẾT VỀ CLOCK:
     - system_clock: Đồng hồ hệ thống (Giống đồng hồ treo tường). Có thể bị sai
     lệch nếu người dùng hoặc Hệ điều hành chỉnh lại giờ (Sync Internet). KHÔNG
     DÙNG ĐỂ ĐO CODE!
     - steady_clock: Đồng hồ bấm giờ. Chỉ biết chạy tiến lên, cực kỳ ổn định.
       LUÔN DÙNG STEADY_CLOCK ĐỂ ĐO HIỆU NĂNG THUẬT TOÁN!
     - high_resolution_clock: Đồng hồ có độ chính xác cao nhất (Thường là bản
     sao của steady_clock).
  */

  // 1. Lấy mốc thời gian Bắt đầu
  auto start_time = std::chrono::steady_clock::now();

  // 2. Giả lập một thuật toán chạy mất 150 mili-giây
  std::cout << "  [Dang chay thuat toan he thong...]\n";
  std::this_thread::sleep_for(150ms);

  // 3. Lấy mốc thời gian Kết thúc
  auto end_time = std::chrono::steady_clock::now();

  // 4. Tính toán độ chênh lệch (Trả về một Duration)
  auto thoi_gian_chay = end_time - start_time;

  // 5. Ép kiểu thời gian (duration_cast)
  // Tại sao phải ép? Vì 'thoi_gian_chay' thường được tính bằng nanoseconds.
  // Dùng duration_cast để gọt bớt độ chính xác, ép nó về mili-giây (số nguyên).
  auto thoi_gian_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(thoi_gian_chay);

  std::cout << "Thuat toan chay mat: " << thoi_gian_ms.count() << " ms\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: CASTING SỐ THỰC VÀ SYSTEM CLOCK
// ==========================================
void chrono_nang_cao_float_va_system() {
  std::cout << "\n--- 3. ADVANCED: DOUBLE DURATION & SYSTEM CLOCK ---\n";

  // a. FLOATING-POINT DURATION (Tránh bị làm tròn mất số thập phân)
  // Mặc định C++ dùng số nguyên (int/long) để lưu thời gian.
  // Nếu bạn ép 150ms sang giây (Seconds), nó sẽ làm tròn thành 0 giây!
  auto khoang_thoi_gian = 1550ms;

  // Cách giải quyết: Định nghĩa một Duration sử dụng số thực (double)
  using DoubleSeconds = std::chrono::duration<double>; // 1.0 giây là chuẩn

  // Khi ép từ int sang double, C++ TỰ ĐỘNG CHUYỂN, không cần duration_cast
  DoubleSeconds giay_so_thuc = khoang_thoi_gian;

  std::cout << "1550 mili-giay = " << giay_so_thuc.count() << " giay.\n";

  // b. SYSTEM_CLOCK (Đồng hồ treo tường)
  // Dùng để lấy ngày tháng năm và giờ thực tế của thế giới.
  auto now = std::chrono::system_clock::now();

  // Chuyển đổi sang time_t (Cấu trúc thời gian ngôn ngữ C cổ điển) để in ra màn
  // hình
  std::time_t now_c = std::chrono::system_clock::to_time_t(now);

  // std::ctime sẽ tự động thêm dấu '\n' ở cuối
  std::cout << "Thoi gian he thong hien tai: " << std::ctime(&now_c);
}

// ==========================================
// 4. MỨC CHUYÊN GIA: C++20 CALENDAR & DATES (LỊCH HIỆN ĐẠI)
// ==========================================
void chrono_cxx20_calendar() {
  std::cout << "\n--- 4. EXPERT (C++20): CALENDAR & DATE ---\n";

  // Lưu ý: Tính năng này yêu cầu Trình biên dịch hỗ trợ chuẩn C++20 (GCC 10+,
  // MSVC 19.28+).
#if __cplusplus >= 202002L
  using namespace std::chrono;

  // C++20 cung cấp hậu tố 'y' cho Năm (Year) và cú pháp dấu '/' cực kỳ trực
  // quan
  year_month_day hom_nay = 2026y / July / 25;

  std::cout << "Ngay da tao: " << (int)hom_nay.year() << "-"
            << (unsigned)hom_nay.month() << "-" << (unsigned)hom_nay.day()
            << "\n";

  // Tính toán trên Lịch (Chưa bao giờ C++ làm việc với ngày tháng dễ như thế
  // này)
  auto thang_sau = hom_nay + months(1);

  // Kiểm tra xem có phải năm nhuận không
  bool la_nam_nhuan = hom_nay.year().is_leap();

  std::cout << "Thang sau se la thang: " << (unsigned)thang_sau.month() << "\n";
  std::cout << "Nam 2026 co phai nam nhuan khong? "
            << (la_nam_nhuan ? "Co" : "Khong") << "\n";

  // Lấy ngày cuối cùng của một tháng bất kỳ
  year_month_day cuoi_thang_2_nam_2024 = 2024y / February / last;
  std::cout << "Ngay cuoi cung Thang 2/2024: "
            << (unsigned)cuoi_thang_2_nam_2024.day() << "\n";
#else
  std::cout << "[Trinh bien dich chua ho tro hoan toan C++20 Calendar]\n";
#endif
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  chrono_co_ban_duration();
  chrono_do_luong_hieu_nang();
  chrono_nang_cao_float_va_system();
  chrono_cxx20_calendar();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}