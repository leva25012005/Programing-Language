#include <array>
#include <bit>    // Cho thao tác Bit
#include <chrono> // Cho Calendar & Timezone
#include <format> // Cho std::format
#include <iostream>
#include <map>
#include <span> // Cho std::span
#include <string>
#include <thread> // Cho std::jthread
#include <vector>


// ==========================================
// 1. STD::FORMAT (Chấm dứt nỗi đau printf và cout)
// ==========================================
void demo_format() {
  std::cout << "\n--- 1. STD::FORMAT ---\n";

  // a. Cơ bản: Điền vào chỗ trống (Nhanh và An toàn tuyệt đối)
  std::string s1 = std::format("Xin chao {}, ban duoc {} diem.", "Nhan", 100);
  std::cout << s1 << "\n";

  // b. Vị trí tham số (Positional arguments)
  // Rất hữu ích khi cần đảo thứ tự từ vựng khi dịch đa ngôn ngữ (i18n)
  std::string s2 = std::format("Toi ten la {1}, nam nay {0} tuoi.", 25, "Nhan");
  std::cout << s2 << "\n";

  // c. Nâng cao: Căn lề, Định dạng số (Pad & Format)
  // {:<10} Căn trái 10 ký tự, {:>10} Căn phải 10 ký tự
  // {:.2f} Lấy 2 số thập phân, {:04} Thêm số 0 ở đầu
  std::cout << std::format("Can trai : |{:<10}|\n", "C++20");
  std::cout << std::format("Can phai : |{:>10}|\n", "C++20");
  std::cout << std::format("So Pi    : {:.2f}\n", 3.14159);
  std::cout << std::format("Ma Hex   : 0x{:04X}\n", 255); // In ra 0x00FF
}

// ==========================================
// 2. STD::SPAN (Kính lúp cho mảng)
// ==========================================
// SỨ MỆNH: Thay vì truyền (int* arr, int size) cực kỳ nguy hiểm và dễ tràn
// viền, hay truyền const std::vector<int>& (bị bó buộc vào std::vector), ta
// truyền std::span. Nó siêu nhẹ (chỉ tốn 2 con trỏ), và nhận MỌI LOẠI MẢNG!

void in_mang_bang_span(std::span<const int> mang) {
  std::cout << "Kich thuoc span: " << mang.size() << " -> ";
  for (int x : mang)
    std::cout << x << " ";
  std::cout << "\n";
}

void demo_span() {
  std::cout << "\n--- 2. STD::SPAN ---\n";

  std::vector<int> v = {1, 2, 3, 4, 5};
  int mang_C[] = {10, 20, 30};
  std::array<int, 4> mang_arr = {100, 200, 300, 400};

  // a. Đỉnh cao: Hàm in_mang_bang_span nhận ĐƯỢC TẤT CẢ mà không cần viết
  // Template!
  in_mang_bang_span(v);
  in_mang_bang_span(mang_C);
  in_mang_bang_span(mang_arr);

  // b. Thao tác Subspan (Cắt nhỏ mảng ra mà không tốn byte Copy nào)
  std::span<int> span_v = v;
  std::span<int> ba_phan_tu_dau = span_v.first(3);
  std::span<int> hai_phan_tu_cuoi = span_v.last(2);
  std::span<int> doan_giua =
      span_v.subspan(1, 3); // Cắt từ index 1, lấy 3 phần tử

  // Vì span trỏ trực tiếp vào dữ liệu gốc, sửa span là sửa dữ liệu gốc!
  ba_phan_tu_dau[0] = 999;
  std::cout << "Vector sau khi sua qua span: " << v[0] << "\n"; // In ra 999
}

// ==========================================
// 3. CHRONO CALENDARS (Lịch và Ngày Tháng)
// ==========================================
void demo_chrono_calendar() {
  std::cout << "\n--- 3. CHRONO CALENDARS ---\n";

  // Mở khóa cú pháp hậu tố (literals) siêu việt như 'y' (year), 'd' (day)
  using namespace std::chrono;

  // a. Cú pháp khởi tạo trực quan (Year / Month / Day)
  auto hom_nay = 2026y / July / 28d;

  // Ép kiểu về year_month_day để lấy số liệu dễ dàng
  year_month_day ymd = hom_nay;

  std::cout << "Ngay: " << (unsigned)ymd.day()
            << ", Thang: " << (unsigned)ymd.month()
            << ", Nam: " << (int)ymd.year() << "\n";

  // b. Nâng cao: Toán học trên Lịch (Chỉ C++20 mới làm được trơn tru)
  // Cộng thêm 3 tháng
  auto ba_thang_sau = ymd + months{3};
  std::cout << "3 thang sau se la thang: " << (unsigned)ba_thang_sau.month()
            << "\n";

  // Tìm ngày cuối cùng của tháng 2 năm 2024 (Năm nhuận)
  auto ngay_cuoi_thang_2 = 2024y / February / last;
  year_month_day kq_nhuan = ngay_cuoi_thang_2;
  std::cout << "Thang 2 nam 2024 co: " << (unsigned)kq_nhuan.day()
            << " ngay.\n";
}

// ==========================================
// 4. STD::JTHREAD (Luồng thông minh)
// ==========================================
void nhiem_vu_cong_nhan(std::stop_token stoken) {
  std::cout << "  [JThread] Cong nhan bat dau lam viec...\n";

  // stoken.stop_requested() sẽ trả về True nếu có ai đó gọi .request_stop()
  while (!stoken.stop_requested()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  std::cout
      << "  [JThread] Nhan duoc lenh dung! Dang don dep roi thoat an toan...\n";
}

void demo_jthread() {
  std::cout << "\n--- 4. STD::JTHREAD ---\n";

  /*
   Khác biệt với std::thread cũ:
   1. KHÔNG BAO GIỜ bị Crash nếu bạn quên gọi .join() trước khi thoát hàm.
      jthread sẽ TỰ ĐỘNG gọi .join() trong Hàm Hủy (Destructor) của nó (Chuẩn
   RAII).
   2. Hỗ trợ std::stop_token để "ra lệnh" dừng luồng từ bên ngoài một cách văn
   minh, thay vì kill luồng đột ngột gây tràn bộ nhớ.
  */

  {
    std::jthread worker(nhiem_vu_cong_nhan); // Bắt đầu luồng mới
    std::this_thread::sleep_for(std::chrono::milliseconds(250)); // Chờ 1 chút

    std::cout << "[Main] Phat lenh yeu cau dung luong!\n";
    worker.request_stop(); // Truyền tín hiệu dừng an toàn vào stop_token

  } // Vừa chạm ngoặc '}' này, jthread tự động gọi .join() chặn chờ luồng con
    // chết hẳn. Tuyệt đối an toàn!
}

// ==========================================
// 5. TOÁN HỌC BIT (<bit>)
// ==========================================
void demo_bit_math() {
  std::cout << "\n--- 5. THAO TAC BIT (<bit>) ---\n";

  unsigned int n = 0b10110010; // Số nhị phân

  // a. popcount: Đếm số lượng bit 1 (Cực kỳ nhanh, gọi thẳng tập lệnh phần
  // cứng)
  std::cout << "So luong bit 1 trong n la: " << std::popcount(n) << "\n";

  // b. has_single_bit: Kiểm tra xem số này có phải là Bội số của 2 không (Chỉ
  // có duy nhất 1 bit 1)
  unsigned int x = 32; // 0b100000
  std::cout << "32 co phai la luy thua cua 2 khong? "
            << (std::has_single_bit(x) ? "Co" : "Khong") << "\n";

  // c. bit_cast: Kẻ hủy diệt reinterpret_cast
  // Ngày xưa để ép kiểu dữ liệu từ float sang bit nguyên thủy int,
  // ta hay dùng reinterpret_cast (rất nguy hiểm và sinh rác Undefined
  // Behavior). bit_cast thực hiện việc copy từng bit một cách an toàn tuyệt
  // đối.
  float f = 3.14f;
  auto bit_cua_f = std::bit_cast<uint32_t>(f);
  std::cout << std::format("Bieu dien hexa cua float 3.14: 0x{:X}\n",
                           bit_cua_f);
}

// ==========================================
// 6. XÓA PHẦN TỬ THẦN TỐC (std::erase & std::erase_if)
// ==========================================
void demo_erase() {
  std::cout << "\n--- 6. UNIFIED ERASE (Cham dut Erase-Remove Idiom) ---\n";

  std::vector<int> v = {10, 20, 20, 30, 20, 40};

  /*
   Code C++11 (Xấu xí và dễ quên):
   v.erase(std::remove(v.begin(), v.end(), 20), v.end());
  */

  // C++20: GỌN GÀNG!
  // 1. std::erase (Xóa theo giá trị)
  std::erase(v, 20); // Xóa tất cả các số 20 ra khỏi vector

  std::cout << "Sau khi std::erase(20): ";
  for (int x : v)
    std::cout << x << " "; // 10, 30, 40
  std::cout << "\n";

  // 2. std::erase_if (Xóa theo điều kiện Lambda)
  std::map<int, std::string> tu_dien = {{1, "A"}, {2, "B"}, {3, "C"}};

  // Xóa tất cả phần tử có Key là số chẵn
  std::erase_if(tu_dien, [](const auto &pair) { return pair.first % 2 == 0; });

  std::cout << "Map sau khi std::erase_if (xoa key chan): ";
  for (const auto &[k, val] : tu_dien)
    std::cout << "[" << k << ":" << val << "] ";
  std::cout << "\n";
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== C++20 STANDARD LIBRARY MEGA UPDATE =====\n";

  demo_format();
  demo_span();
  demo_chrono_calendar();
  demo_jthread();
  demo_bit_math();
  demo_erase();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}