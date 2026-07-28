#include <chrono>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY: BRANCH PREDICTION)
// ==========================================
/*
 1. CPU hoạt động như thế nào?
    - CPU hiện đại không đọc từng lệnh rồi mới chạy lệnh tiếp theo. Nó dùng
 "Đường ống" (Pipeline) để đọc trước (prefetch) hàng chục lệnh tiếp theo vào bộ
 nhớ đệm (Cache) để thực thi song song.

 2. Vấn đề của lệnh IF/ELSE:
    - Khi gặp một ngã rẽ IF/ELSE, CPU buộc phải "đoán" xem đoạn code nào sẽ được
 chạy để tải trước vào Pipeline.
    - Nếu đoán ĐÚNG: Chương trình chạy với tốc độ ánh sáng (Zero-cost).
    - Nếu đoán SAI: CPU phải xóa toàn bộ Đường ống (Pipeline Flush), vứt bỏ công
 sức đọc trước đó và quay lại đọc nhánh đúng. Quá trình này RẤT CHẬM (tốn hàng
 chục chu kỳ CPU).

 3. Sứ mệnh của [[likely]] và [[unlikely]]:
    - Trình biên dịch (GCC, Clang, MSVC) sẽ sắp xếp lại mã máy (Assembly).
    - Nhánh được đánh dấu [[likely]] sẽ được đặt nằm LIỀN KỀ ngay dưới lệnh kiểm
 tra điều kiện.
    - Nhánh [[unlikely]] sẽ bị ném ra một vùng nhớ xa xôi khác.
    -> CPU sẽ luôn ưu tiên "rơi tự do" vào khối code liền kề mà không cần phải
 thực hiện bước nhảy (JMP).
*/

// ==========================================
// 1. MỨC CƠ BẢN: SỬ DỤNG TRONG IF / ELSE
// ==========================================
void check_tuoi_hop_le(int tuoi) {
  std::cout << "\n--- 1. BAZIC: IF / ELSE ---\n";

  // Đặt thuộc tính ngay sau điều kiện if
  // Hầu hết người dùng nhập tuổi hợp lệ, ta "thì thầm" cho CPU biết điều này.
  if (tuoi >= 18 && tuoi <= 100) [[likely]] {
    std::cout << "  -> Truong hop pho bien: Nguoi dung hop le.\n";
    // Khối lệnh này sẽ được ưu tiên đưa vào Instruction Cache của CPU
  } else [[unlikely]] {
    std::cout << "  -> Truong hop hiem: Tuoi tre em hoac nhap sai.\n";
    // Khối lệnh này bị tống ra xa, CPU sẽ lười nạp nó vào đệm.
  }
}

// ==========================================
// 2. MỨC TRUNG CẤP: SỬ DỤNG TRONG SWITCH - CASE
// ==========================================
void xu_ly_http_status(int status_code) {
  std::cout << "\n--- 2. INTERMEDIATE: SWITCH CASE ---\n";

  switch (status_code) {
  // Trong một API thực tế, 99% các request trả về 200 OK.
  case 200:
    [[likely]] std::cout << "  -> 200 OK: Xu ly payload (Chay cuc nhanh)\n";
    break;

  case 404:
    std::cout << "  -> 404 Not Found: Khong tim thay\n";
    break;

  // Server sập là chuyện cực kỳ hiếm khi xảy ra
  case 500:
    [[unlikely]] std::cout
        << "  -> 500 Internal Error: Ghi log canh bao he thong!\n";
    break;

  default:
    std::cout << "  -> Other Status\n";
    break;
  }
}

// ==========================================
// 3. MỨC NÂNG CAO: XỬ LÝ LỖI TRONG VÒNG LẶP NÓNG (HOT LOOP)
// ==========================================
void toi_uu_hot_loop() {
  std::cout << "\n--- 3. ADVANCED: HOT LOOP OPTIMIZATION ---\n";

  std::vector<int> data(1000000, 1); // 1 triệu phần tử mang giá trị 1
  data[999999] = 0; // Chỉ có duy nhất phần tử cuối là số 0

  long long sum = 0;

  // "Vòng lặp nóng" là nơi các lệnh được chạy đi chạy lại hàng triệu lần.
  // Tối ưu được 1 chu kỳ CPU ở đây nghĩa là bạn tiết kiệm được 1 triệu chu kỳ
  // tổng thể.
  auto start = std::chrono::high_resolution_clock::now();

  for (int val : data) {
    // Phép chia cho 0 là lỗi chí mạng, nhưng nó CỰC KỲ HIẾM.
    // Bằng cách đánh dấu [[unlikely]], ta dọn đường cho phép chia diễn ra không
    // ma sát.
    if (val == 0) [[unlikely]] {
      std::cout << "  -> Phat hien so 0, bo qua phep chia.\n";
      continue;
    }

    // Đoạn code này được đặt sát rạt lệnh if trong Assembly
    sum += 100 / val;
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "  -> Hoan thanh tinh toan: Sum = " << sum << "\n";
  // Thường thì sự khác biệt về tốc độ chỉ rõ rệt nhất khi bật cờ tối ưu -O3 của
  // compiler.
}

// ==========================================
// 4. MỨC CHUYÊN GIA: KHI NÀO KHÔNG NÊN DÙNG?
// ==========================================
void cam_bay_chi_mang() {
  std::cout << "\n--- 4. EXPERT: CAM BAY TU TIN THAI QUA ---\n";

  /*
   CẠM BẪY SỐ 1: Bệnh "Lạm dụng tối ưu" (Premature Optimization)
   Trình biên dịch và CPU thời nay ĐÃ CỰC KỲ THÔNG MINH.
   CPU có khả năng "Học" (Dynamic Branch Prediction) dựa trên lịch sử chạy thực
   tế. Nếu bạn đoán sai và đặt [[likely]] bừa bãi, bạn sẽ LÀM CHẬM chương trình
   một cách thảm họa!

   LỜI KHUYÊN TỪ CÁC KỸ SƯ GOOGLE/MICROSOFT:
   1. Chỉ dùng [[unlikely]] cho các luồng BÁO LỖI (Exceptions, Cảnh báo, Null
   pointer).
   2. Chỉ dùng [[likely]] khi bạn ĐÃ CHẠY PROFILER (Đo lường) và CÓ SỐ LIỆU
   chứng minh rằng luồng đó chiếm > 95% thời gian chạy.
   3. ĐỪNG bao giờ "đoán mò" thay cho CPU.
  */

  int random_val = rand() % 100;

  // VÍ DỤ SAI LẦM:
  // Tỉ lệ chẵn lẻ là 50/50. CPU tự học được mô hình này rất nhanh.
  // Nếu bạn ép nó ưu tiên số Chẵn bằng [[likely]], bạn đang làm khó Trình biên
  // dịch.
  if (random_val % 2 == 0) /* DỪNG DÙNG LIKELY Ở ĐÂY */ {
    // ...
  }

  std::cout << "  -> Ghi nho: Chi thi tham khi ban THUC SU chac chan 99% ve "
               "tuong lai!\n";
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== KHOA HOC C++20: BRANCH PREDICTION ATTRIBUTES =====\n";

  check_tuoi_hop_le(25);
  check_tuoi_hop_le(5);

  xu_ly_http_status(200);
  xu_ly_http_status(500);

  toi_uu_hot_loop();
  cam_bay_chi_mang();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}

// Cách dùng Profiler trong C++
// Hiểu về CPU Cache Line (L1, L2, L3)