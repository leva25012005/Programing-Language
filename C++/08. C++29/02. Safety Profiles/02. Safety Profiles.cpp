#include <iostream>
#include <memory>
#include <span>
#include <string>
#include <variant>
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG: SAFETY PROFILES (C++26/C++29)
// ==========================================
/*
 1. BỐI CẢNH LỊCH SỬ (TẠI SAO CÓ SAFETY PROFILES?):
    - Gần đây, NSA (Cơ quan An ninh Quốc gia Mỹ) và Nhà Trắng đã đưa ra khuyến
 cáo toàn cầu: "Ngừng sử dụng C/C++ cho các dự án mới vì rủi ro rò rỉ bộ nhớ,
 hãy chuyển sang Rust".
    - Để cứu vãn đế chế C++, cha đẻ Bjarne Stroustrup và Ủy ban chuẩn C++ đã đề
 xuất "Safety Profiles" (Dựa trên C++ Core Guidelines).

 2. SAFETY PROFILES LÀ GÌ?
    - Nó không phải là một ngôn ngữ C++ mới. Nó là một BỘ QUY TẮC KIỂM TRA
 NGHIÊM NGẶT được tích hợp thẳng vào Trình biên dịch (Compiler).
    - Khi bạn bật cờ (flag) Profile lên (ví dụ: gõ lệnh `g++ -Wprofile-lifetime
 code.cpp`), Trình biên dịch sẽ hóa thân thành một "Cảnh sát" khắt khe y hệt
 Rust (Borrow Checker).
    - Bất cứ dòng code nào có NGUY CƠ tràn mảng, con trỏ lơ lửng, ép kiểu bậy
 bạ... đều sẽ bị LỖI BIÊN DỊCH (Compile Error) ngay lập tức, không cho phép
 chạy!

 3. BA TRỤ CỘT CỦA SAFETY PROFILES:
    - Bounds Safety Profile (An toàn biên/mảng).
    - Type Safety Profile (An toàn kiểu dữ liệu).
    - Lifetime Safety Profile (An toàn vòng đời - Chống con trỏ lơ lửng).
*/

// ==========================================
// CÚ PHÁP TƯƠNG LAI (MÔ PHỎNG)
// ==========================================
// Ở C++26/29, bạn có thể bật/tắt Profile trực tiếp trong code bằng attribute
// hoặc pragma. Dưới đây là code mô phỏng. Trình biên dịch hiện tại sẽ phớt lờ
// nó, nhưng với compiler tương lai, nó sẽ kích hoạt chế độ "Rust-mode".

// [[feature::safety_profile(bounds, type, lifetime)]]  // (Cú pháp dự kiến)

// ==========================================
// 1. BOUNDS SAFETY (AN TOÀN BIÊN/MẢNG)
// ==========================================
void bounds_safety_profile() {
  std::cout << "\n--- 1. BOUNDS SAFETY (CHONG TRAN MANG) ---\n";

  int arr[5] = {1, 2, 3, 4, 5};
  int *ptr = arr;

  // [❌ LỖI BỊ PROFILE TỪ CHỐI BIÊN DỊCH]
  // ptr[10] = 99;       // Lỗi: Truy cập ngoài biên (Buffer Overflow).
  // *(ptr + 2) = 10;    // Lỗi: Arithmetic Pointer (Cộng trừ con trỏ) bị CẤM
  // HOÀN TOÀN!

  // [✅ CÁCH VIẾT CHUẨN ĐƯỢC PROFILE CHẤP NHẬN]
  // 1. Thay mảng C bằng std::array, std::vector.
  // 2. Dùng std::span (Ra mắt C++20) - Đại sứ an toàn của mảng:
  std::span<int> an_toan(arr);

  // Profile ép buộc bạn phải dùng vòng lặp range-based hoặc kiểm tra biên
  for (int x : an_toan) {
    std::cout << x << " ";
  }
  std::cout << "\n";
}

// ==========================================
// 2. TYPE SAFETY (AN TOÀN KIỂU DỮ LIỆU)
// ==========================================
void type_safety_profile() {
  std::cout << "\n--- 2. TYPE SAFETY (CHONG EP KIEU SAO RONG) ---\n";

  double so_thuc = 3.14;

  // [❌ LỖI BỊ PROFILE TỪ CHỐI BIÊN DỊCH]
  // int* p = (int*)&so_thuc;                  // Lỗi: Ép kiểu C-Style bị CẤM!
  // int* p2 = reinterpret_cast<int*>(&so_thuc); // Lỗi: reinterpret_cast bị CẤM
  // vì quá nguy hiểm! void* vp = &so_thuc;                      // Lỗi: Xóa mất
  // kiểu dữ liệu (Type Erasure) bằng void*.

  // Khai báo biến không khởi tạo cũng sẽ bị cấm:
  // int rac; // Lỗi: Bắt buộc phải là int rac = 0;

  // [✅ CÁCH VIẾT CHUẨN ĐƯỢC PROFILE CHẤP NHẬN]
  // Sử dụng static_cast cho những phép ép kiểu an toàn và rõ ràng.
  int so_nguyen = static_cast<int>(so_thuc);

  // Dùng std::variant (C++17) thay cho `union` cổ điển (union rất dễ đọc nhầm
  // bộ nhớ)
  std::variant<int, std::string> du_lieu_an_toan;
  du_lieu_an_toan = "Chuoi ki tu";

  std::cout << "Gia tri an toan: " << std::get<std::string>(du_lieu_an_toan)
            << "\n";
}

// ==========================================
// 3. LIFETIME SAFETY (AN TOÀN VÒNG ĐỜI - QUAN TRỌNG NHẤT)
// ==========================================
// ĐÂY LÀ TÍNH NĂNG ĐƯỢC MONG CHỜ NHẤT ĐỂ ĐÁNH BẠI RUST BORROW CHECKER.

// [❌ LỖI BỊ PROFILE TỪ CHỐI BIÊN DỊCH]
/*
int* tao_con_tro_nguy_hiem() {
    int local_var = 42;
    return &local_var; // LỖI CHÍ MẠNG: Trả về con trỏ trỏ tới biến đã chết
(Dangling Pointer)!
}
*/

// [❌ LỖI BỊ PROFILE TỪ CHỐI LÂY NHIỄM SANG LAMBDA]
/*
auto tao_lambda_nguy_hiem() {
    std::string text = "Tuyet mat";
    // LỖI: Bắt biến 'text' theo tham chiếu [&], nhưng khi hàm này kết thúc,
'text' bị hủy.
    // Nếu chạy lambda này ở ngoài, chương trình sẽ crash.
    return [&text]() { std::cout << text; };
}
*/

// [✅ CÁCH VIẾT CHUẨN ĐƯỢC PROFILE CHẤP NHẬN]
// Nếu muốn truyền dữ liệu ra ngoài, hãy dùng giá trị (Value) hoặc Smart
// Pointers.
std::unique_ptr<int> tao_con_tro_an_toan() {
  auto an_toan = std::make_unique<int>(42);
  return an_toan; // Hợp lệ, quyền sở hữu được chuyển giao ra ngoài.
}

void lifetime_safety_profile() {
  std::cout << "\n--- 3. LIFETIME SAFETY (CHONG CON TRO LO LUNG) ---\n";

  // Trình biên dịch theo dõi "Lifetime" (Tuổi thọ) của mọi biến:
  int *p = nullptr;
  {
    int x = 100;
    p = &x;
    // [❌ LỖI TẠI ĐÂY - NẾU BẬT PROFILE]
    // Compiler báo lỗi ngay lúc biên dịch: "p đang chứa địa chỉ của x.
    // x sẽ bị hủy ở cuối khối lệnh '}', khiến p thành Dangling Pointer".
  }
  // std::cout << *p; // Nếu không có Profile chặn ở trên, lệnh này gây Crash.

  std::cout << "Kich hoat std::unique_ptr de quan ly memory thanh cong.\n";
  auto ptr = tao_con_tro_an_toan();
  std::cout << "Du lieu an toan tu ham: " << *ptr << "\n";
}

// ==========================================
// 4. SUPPRESSION (LÁ BÙA MIỄN TRỪ)
// ==========================================
/*
 Sẽ có lúc bạn BẮT BUỘC phải làm những thao tác nguy hiểm (như viết Driver giao
 tiếp phần cứng, cần dùng ép kiểu reinterpret_cast). Giống như khối `unsafe {}`
 của Rust, C++ Safety Profile cho phép bạn "Xin miễn trừ" (Suppress) tại một số
 vị trí nhất định để compiler nhắm mắt làm ngơ.
*/

// Cú pháp dự kiến:
// [[gsl::suppress(type.1)]]
void thao_tac_nguy_hiem_co_chu_dich() {
  std::cout << "\n--- 4. SUPPRESS (KHỐI UNSAFE) ---\n";
  float f = 3.14f;

  // Báo cho cảnh sát Compiler: "Tôi biết tôi đang làm gì, xin đừng bắt lỗi dòng
  // này!"
  // [[suppress(type)]]
  int *bits = reinterpret_cast<int *>(&f);

  std::cout << "Da thuc hien thao tac nguy hiem thanh cong (Đã xin phép).\n";
}

// ==========================================
// TỔNG KẾT VÀ TẦM NHÌN (VISION)
// ==========================================
/*
 - Với C++26/29 Safety Profiles, C++ sẽ có cả 2 thế giới:
   1. Tốc độ tuyệt đối và tương thích ngược của C++.
   2. Sự an toàn bộ nhớ tuyệt đối của Rust.

 - Bạn không cần đập bỏ dự án 10 triệu dòng code C++ để viết lại bằng Rust.
   Bạn chỉ cần bật Profile lên, Trình biên dịch sẽ chỉ ra những chỗ nguy hiểm để
 bạn sửa dần (Gradual Adoption). Đây là đòn phản công mạnh mẽ nhất của thế giới
 C++.
*/

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== C++29 SAFETY PROFILES (RUST-MODE ENFORCEMENT) =====\n";

  bounds_safety_profile();
  type_safety_profile();
  lifetime_safety_profile();
  thao_tac_nguy_hiem_co_chu_dich();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}