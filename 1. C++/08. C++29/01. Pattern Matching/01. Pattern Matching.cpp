#include <iostream>
#include <string>
#include <variant> // Thư viện chứa std::variant và std::visit
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Pattern Matching (Khớp mẫu) là gì?
    - Là phiên bản "Siêu cấp" của lệnh switch-case.
    - switch-case thông thường CHỈ kiểm tra được SỐ NGUYÊN (int, char, enum).
    - Pattern Matching cho phép rẽ nhánh dựa trên: KIỂU DỮ LIỆU (Nó là int hay
 string?), CẤU TRÚC (Mảng này có 3 phần tử không?), và ĐIỀU KIỆN LOGIC (Chuỗi
 này có rỗng không?).

 2. Tình trạng ở C++ hiện tại:
    - Các ngôn ngữ như Rust (match) hay C# (switch) đã có tính năng này từ lâu.
    - C++ dự định đưa từ khóa `inspect` vào C++26, nhưng thiết kế quá phức tạp
 nên đã bị dời lại.
    - Hiện tại, C++17/20 dùng `std::variant` và `std::visit` để thay thế hoàn
 hảo.
*/

// ==========================================
// CÔNG CỤ MA THUẬT: KẺ THAY THẾ 'INSPECT' TRONG C++17/20
// ==========================================
// Cú pháp này có vẻ đáng sợ, nhưng nó là "Bùa chú" kinh điển của C++ hiện đại.
// Nhiệm vụ của nó: Gom nhiều hàm Lambda lại với nhau thành một cục (Overload
// set) để dùng làm các "case" (nhánh) cho Pattern Matching.
template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};

// Deduction guide cho C++17 (C++20 không cần dòng này nhưng viết cho an toàn)
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

// ==========================================
// 1. MỨC CƠ BẢN: KHỚP THEO KIỂU DỮ LIỆU (TYPE MATCHING)
// ==========================================
void pattern_matching_hien_tai() {
  std::cout << "\n--- 1. BAZIC: PATTERN MATCHING VOI STD::VISIT (C++17) ---\n";

  // std::variant giống như một chiếc hộp ma thuật, tại một thời điểm
  // nó chỉ chứa ĐÚNG 1 kiểu dữ liệu trong số những kiểu được khai báo.
  std::variant<int, double, std::string> hop_du_lieu;

  hop_du_lieu = "Xin chao C++"; // Hộp đang chứa chuỗi

  // Thực hiện Pattern Matching bằng std::visit và overloaded
  std::visit(
      overloaded{[](int i) { std::cout << "=> La so nguyen: " << i << "\n"; },
                 [](double d) { std::cout << "=> La so thuc: " << d << "\n"; },
                 [](const std::string &s) {
                   std::cout << "=> La chuoi: " << s << "\n";
                 }},
      hop_du_lieu);
}

// ==========================================
// 2. MỨC TRUNG CẤP: GẮN ĐIỀU KIỆN (GUARDS) & WILDCARD (BẮT CÁC TRƯỜNG HỢP CÒN
// LẠI)
// ==========================================
void pattern_matching_co_dieu_kien() {
  std::cout << "\n--- 2. INTERMEDIATE: GUARDS (DIEU KIEN) & DEFAULT ---\n";

  std::variant<int, std::string, bool> du_lieu = std::string("");

  std::visit(overloaded{[](int i) { std::cout << "So nguyen: " << i << "\n"; },

                        // Gắn điều kiện logic (Guard) ngay bên trong case
                        [](const std::string &s) {
                          if (s.empty()) {
                            std::cout << "=> Phat hien Chuoi rong!\n";
                          } else {
                            std::cout << "=> Chuoi co noi dung: " << s << "\n";
                          }
                        },

                        // WILDCARD (Trường hợp mặc định - Giống lệnh 'default'
                        // hoặc '_' trong Rust). auto sẽ đón bắt MỌI kiểu dữ
                        // liệu còn lại chưa được liệt kê (ở đây là bool).
                        [](auto t) {
                          std::cout << "=> Khong khop voi kieu nao quan trong "
                                       "(Default wildcard).\n";
                        }},
             du_lieu);
}

// ==========================================
// 3. MỨC NÂNG CAO: TƯƠNG LAI CỦA C++ (CÚ PHÁP 'INSPECT')
// ==========================================
/*
 KHỐI CODE DƯỚI ĐÂY ĐƯỢC VÔ HIỆU HÓA BẰNG #if 0 ĐỂ KHÔNG BỊ LỖI BIÊN DỊCH.
 Đây chính là Đề xuất chính thức (Proposal P2688) dự kiến sẽ xuất hiện trong
 tương lai.
*/
#if 0
void pattern_matching_tuong_lai() {
    std::cout << "\n--- 3. ADVANCED: CU PHAP INSPECT (FUTURE C++) ---\n";
    
    std::variant<int, std::string> myVariant = "Hello";

    // Cú pháp cực kỳ gọn gàng, vứt bỏ được toàn bộ std::visit và overloaded phức tạp.
    inspect (myVariant) {
        // Khớp kiểu: Dùng <Type> biến => biểu_thức
        <int> i => std::cout << "La so nguyen: " << i,
        
        // Khớp kiểu kèm Điều kiện (Guard 'if'):
        <std::string> s if (s.empty()) => std::cout << "Chuoi rong",
        
        // Khớp cụ thể một giá trị (Value matching)
        <std::string> "Hello" => std::cout << "Tim thay loi chao!",
        
        <std::string> s => std::cout << "Chuoi bat ky: " << s,
        
        // Wildcard (Bắt các trường hợp còn lại): Dùng dấu _
        _ => std::cout << "Khong khop voi bat ky mau nao"
    };

    // INSPECT TRÊN MẢNG / TUPLE (Tách cấu trúc - Destructuring)
    std::pair<int, int> toa_do = {0, 5};
    
    inspect (toa_do) {
        [0, 0] => std::cout << "Goc toa do",
        [0, y] => std::cout << "Nam tren truc Tung, y = " << y,
        [x, 0] => std::cout << "Nam tren truc Hoanh, x = " << x,
        [x, y] => std::cout << "Toa do bat ky"
    };
}
#endif

// ==========================================
// 4. MỨC CHUYÊN GIA: PATTERN MATCHING TRÊN CẤU TRÚC (HIỆN TẠI)
// ==========================================
// Hiện tại (C++17), mặc dù không có `inspect` trên Tuple/Pair,
// ta dùng "Structured Bindings" kết hợp `if constexpr` để làm điều tương tự.
void mo_phong_khop_cau_truc() {
  std::cout << "\n--- 4. EXPERT: STRUCTURED PATTERN (C++17) ---\n";

  std::pair<int, int> toa_do = {0, 10};

  // Tách cấu trúc ngay lập tức
  auto [x, y] = toa_do;

  // Rẽ nhánh logic (Mô phỏng lại khối inspect trên pair ở trên)
  if (x == 0 && y == 0) {
    std::cout << "=> Goc toa do\n";
  } else if (x == 0) {
    std::cout << "=> Nam tren truc Tung, y = " << y << "\n";
  } else if (y == 0) {
    std::cout << "=> Nam tren truc Hoanh, x = " << x << "\n";
  } else {
    std::cout << "=> Toa do: (" << x << ", " << y << ")\n";
  }
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== C++ PATTERN MATCHING =====\n";

  pattern_matching_hien_tai();
  pattern_matching_co_dieu_kien();
  mo_phong_khop_cau_truc();

  std::cout
      << "\n(Ban hay mo source code de doc phan C++ Tuong lai o muc 3 nhe!)\n";
  std::cout << "===== HOAN THANH =====\n";
  return 0;
}