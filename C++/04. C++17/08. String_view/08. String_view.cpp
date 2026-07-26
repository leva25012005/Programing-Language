#include <iostream>
#include <string>
#include <string_view>
#include <vector>

// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Sứ mệnh: std::string_view (C++17) sinh ra để giải quyết vấn đề hiệu năng của
 std::string.
 2. Bản chất: Là một Non-owning View (Góc nhìn không sở hữu). Nó KHÔNG TỰ QUẢN
 LÝ BỘ NHỚ. Nó chỉ là 1 con trỏ + 1 biến size. Chi phí tạo ra nó gần như BẰNG 0.
 3. Tác dụng:
    - Thay thế hoàn toàn việc truyền tham số `const std::string&` trong các hàm
 chỉ có nhu cầu ĐỌC chuỗi.
    - Cho phép cắt chuỗi (substring) với tốc độ hằng số O(1) mà không tốn thêm
 RAM.
 4. Hạn chế (Read-only): Bạn không thể thay đổi dữ liệu gốc thông qua
 string_view (Không thể thêm/sửa ký tự).
*/

// ==========================================
// 1. MỨC CƠ BẢN: KHỞI TẠO VÀ TRUYỀN THAM SỐ
// ==========================================
// XƯA: Truyền const std::string&.
// Nhược điểm: Nếu người dùng truyền vào chuỗi "char*" (ví dụ: "Hello"), C++ sẽ
// lén lút cấp phát động một object std::string tạm thời, copy "Hello" vào đó,
// rồi mới truyền tham chiếu vào hàm. RẤT CHẬM!
void in_chuoi_cu(const std::string &s) { std::cout << " [Cu] " << s << "\n"; }

// NAY: Truyền std::string_view BẰNG GIÁ TRỊ (Pass by value).
// Tốc độ copy 1 string_view (8 byte con trỏ + 8 byte size) còn nhanh hơn cả
// truyền tham chiếu (1 con trỏ ẩn). Không bao giờ có sự cấp phát động (Dynamic
// Allocation) nào xảy ra ở đây.
void in_chuoi_moi(std::string_view sv) { std::cout << " [Moi] " << sv << "\n"; }

void string_view_co_ban() {
  std::cout << "\n--- 1. BAZIC: KHOI TAO & TRUYEN THAM SO ---\n";

  std::string chuoi_cpp = "Chuoi chuan cua C++";
  const char *chuoi_c = "Chuoi kieu C";

  // Khởi tạo string_view từ đủ mọi nguồn mà KHÔNG HỀ TỐN BỘ NHỚ COPY
  std::string_view sv1 = chuoi_cpp; // Nhìn vào std::string
  std::string_view sv2 = chuoi_c;   // Nhìn vào mảng char*
  std::string_view sv3 =
      "Chuoi literal"; // Nhìn thẳng vào vùng nhớ tĩnh của chương trình

  // Truyền tham số siêu việt
  in_chuoi_cu(chuoi_cpp);
  // in_chuoi_cu("Hello"); // CẢNH BÁO: Ở đây có cấp phát động ẩn (Hidden
  // Allocation)!

  in_chuoi_moi(chuoi_cpp); // Không cấp phát
  in_chuoi_moi("Hello"); // CỰC NHANH: Chỉ đơn giản là trỏ kính lúp vào chữ
                         // "Hello". Không cấp phát!
}

// ==========================================
// 2. MỨC TRUNG CẤP: CẮT CHUỖI SIÊU TỐC O(1)
// ==========================================
void string_view_cat_chuoi() {
  std::cout << "\n--- 2. INTERMEDIATE: THAO TAC O(1) ---\n";

  std::string log_data = "[ERROR] Ket noi mang bi mat vao luc 10:00";
  std::string_view sv_log = log_data;

  // 1. Cắt tiếp đầu ngữ (remove_prefix) - O(1)
  // Thay vì copy tạo chuỗi mới, string_view chỉ đơn giản dời con trỏ lên 8 bước
  // và giảm size đi 8.
  sv_log.remove_prefix(8);
  std::cout << "Sau remove_prefix(8): " << sv_log
            << "\n"; // "Ket noi mang bi mat vao luc 10:00"

  // 2. Cắt tiếp vĩ ngữ (remove_suffix) - O(1)
  // Kéo lùi giới hạn size lại 15 bước.
  sv_log.remove_suffix(15);
  std::cout << "Sau remove_suffix(15): " << sv_log
            << "\n"; // "Ket noi mang bi mat"

  // 3. Substr siêu tốc - O(1)
  // Hàm substr của std::string tạo ra 1 chuỗi C++ mới (O(N) và cấp phát RAM).
  // Hàm substr của std::string_view trả ra một string_view khác nhìn vào đoạn
  // đó (O(1)).
  std::string_view sv_loi_chinh = sv_log.substr(0, 7);
  std::cout << "Substr O(1): " << sv_loi_chinh << "\n";

  // LƯU Ý KỲ DIỆU: Dù cắt xén nát bét cái string_view, dữ liệu gốc của
  // std::string vẫn còn nguyên vẹn!
  std::cout << "Du lieu goc (van an toan): " << log_data << "\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: PHÂN TÍCH CHUỖI (PARSING)
// ==========================================
// Ứng dụng thực tế: Tách các từ cách nhau bởi dấu phẩy (Split)
void split_string_hien_dai(std::string_view van_ban) {
  std::cout << "\n--- 3. ADVANCED: STRING SPLIT (Zero Allocation) ---\n";

  std::cout << "Tach chuoi: [" << van_ban << "]\n";

  size_t pos = 0;
  while ((pos = van_ban.find(',')) != std::string_view::npos) {
    // Lấy từ đầu tiên ra (O(1))
    std::string_view tu = van_ban.substr(0, pos);
    std::cout << "  -> Tim thay: " << tu << "\n";

    // Cắt bỏ phần đã xử lý cộng thêm dấu phẩy (O(1))
    van_ban.remove_prefix(pos + 1);
  }
  // In từ cuối cùng
  if (!van_ban.empty()) {
    std::cout << "  -> Tim thay: " << van_ban << "\n";
  }
  // KẾT LUẬN: Toàn bộ quá trình tách chuỗi trên tốn ĐÚNG 0 BYTES BỘ NHỚ CẤP
  // PHÁT ĐỘNG!
}

// ==========================================
// 4. MỨC CHUYÊN GIA: 2 CẠM BẪY CHÍ MẠNG
// ==========================================

// CẠM BẪY 1: DANGLING POINTER (Con trỏ lơ lửng / Nhìn vào cõi hư vô)
std::string_view tao_cam_bay_dangling() {
  std::string ten_tam_thoi = "Le Van Nhan";

  std::string_view kinh_lup = ten_tam_thoi;

  return kinh_lup; // NGUY HIỂM TỘT ĐỘ!
  // Khi thoát hàm, biến 'ten_tam_thoi' bị HỦY khỏi RAM.
  // Trả về 'kinh_lup' lúc này là trả về một cái kính lúp đang nhìn vào một bãi
  // rác bộ nhớ!
}

void string_view_cam_bay() {
  std::cout << "\n--- 4. EXPERT: CAM BAY NGUY HIEM ---\n";

  // a. Đừng bao giờ làm thế này:
  // std::string_view loi = tao_cam_bay_dangling();
  // std::cout << loi; // CRASH CHƯƠNG TRÌNH (Undefined Behavior)

  // b. CẠM BẪY 2: THIẾU KÝ TỰ KẾT THÚC CHUỖI '\0' (Null-terminator)
  std::string goc = "Hello World";
  std::string_view sv = goc;

  sv.remove_suffix(6);
  std::cout << "Chuoi tren C++: " << sv << " (In ra hoan toan dung)\n";

  // NGUY HIỂM: Hàm printf của ngôn ngữ C đòi hỏi mảng char* phải có số 0 ở cuối
  // ('\0') để biết điểm dừng. Kính lúp (string_view) KHÔNG hề chèn số 0 vào dữ
  // liệu gốc!
  std::cout << "Loi khi dung C-API (.data()): ";
  printf("%s\n",
         sv.data()); // NÓ SẼ IN RA "Hello World", CHỨ KHÔNG PHẢI "Hello"!

  // GIẢI PHÁP: C++ cung cấp sẵn cách bọc dữ liệu an toàn nếu bắt buộc gọi C-API
  // Phải chấp nhận mất hiệu năng để copy sang std::string có chứa '\0'.
  std::string an_toan(sv);
  std::cout << "Sua loi C-API: ";
  printf("%s\n", an_toan.c_str());
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  string_view_co_ban();
  string_view_cat_chuoi();

  split_string_hien_dai("Apple,Banana,Orange,Mango");

  string_view_cam_bay();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}