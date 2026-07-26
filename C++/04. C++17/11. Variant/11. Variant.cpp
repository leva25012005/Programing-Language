#include <iostream>
#include <string>
#include <variant> // Bắt buộc cho std::variant, std::get, std::visit
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. std::variant là gì?
    - Là một "Type-safe Union" (Union an toàn kiểu dữ liệu).
    - Ví dụ: std::variant<int, double, string> v;
      Nghĩa là 'v' có thể là int, HOẶC double, HOẶC string tại một thời điểm.

 2. Tại sao không dùng 'union' cổ điển của C?
    - union không gọi Constructor/Destructor của các object phức tạp (như
 std::string).
    - union không nhớ nó đang chứa kiểu gì, bạn đọc nhầm kiểu nó sẽ in ra rác
 (Undefined Behavior).
    - std::variant giải quyết toàn bộ vấn đề trên!

 3. Kích thước (Memory Overhead):
    - Kích thước của variant = Kích thước của kiểu dữ liệu LỚN NHẤT bên trong nó
 + 1 byte (để lưu Index đánh dấu kiểu hiện tại).
*/

// ==========================================
// CLASS PHỤ TRỢ CHO PHẦN NÂNG CAO
// ==========================================
// Struct không có Constructor mặc định (Default Constructor)
struct KhongCoDefault {
  int id;
  KhongCoDefault(int x) : id(x) {} // Cố tình giấu default constructor
};

// ==========================================
// 1. MỨC CƠ BẢN: KHỞI TẠO, GÁN VÀ ĐỌC GIÁ TRỊ
// ==========================================
void variant_co_ban() {
  std::cout << "\n--- 1. BAZIC: KHOI TAO & TRUY XUAT ---\n";

  // Khai báo: Biến 'du_lieu' có thể chứa 1 trong 3 kiểu này.
  // MẶC ĐỊNH: Nó sẽ khởi tạo bằng kiểu ĐẦU TIÊN trong danh sách (ở đây là int =
  // 0).
  std::variant<int, double, std::string> du_lieu;

  // Gán dữ liệu (Variant tự động chuyển đổi kiểu hiện hành)
  du_lieu = 42;           // Lúc này nó là int
  du_lieu = 3.14;         // Giờ nó biến thành double
  du_lieu = "Chao C++17"; // Cuối cùng nó thành chuỗi

  // ĐỌC DỮ LIỆU: Dùng std::get<Type>
  std::cout << "Kieu hien tai la chuoi: " << std::get<std::string>(du_lieu)
            << "\n";

  // ĐỌC BẰNG INDEX:
  std::cout << "Doc bang index 2: " << std::get<2>(du_lieu) << "\n";

  // KIỂM TRA KIỂU HIỆN TẠI (TRÁNH CRASH)
  if (std::holds_alternative<std::string>(du_lieu)) {
    std::cout << " -> Toi chac chan no dang la std::string!\n";
  }

  // LẤY INDEX HIỆN TẠI (0: int, 1: double, 2: string)
  std::cout << "Index hien tai cua Variant la: " << du_lieu.index() << "\n";
}

// ==========================================
// 2. MỨC TRUNG CẤP: XỬ LÝ LỖI (BAD_VARIANT_ACCESS & GET_IF)
// ==========================================
void variant_xu_ly_loi() {
  std::cout << "\n--- 2. INTERMEDIATE: AN TOAN & XU LY LOI ---\n";

  std::variant<int, std::string> v = 100;

  // a. CẠM BẪY std::get
  // Nếu bạn cố tình lấy sai kiểu, chương trình sẽ NÉM NGOẠI LỆ (Crash nếu không
  // bắt).
  try {
    std::string s =
        std::get<std::string>(v); // v đang là int, lấy string sẽ nổ!
  } catch (const std::bad_variant_access &e) {
    std::cout << "Da bat duoc loi std::get sai kieu: " << e.what() << "\n";
  }

  // b. GIẢI PHÁP TỐI ƯU HƠN: std::get_if
  // Nó nhận vào CON TRỎ của variant.
  // Nếu ĐÚNG kiểu -> Trả về con trỏ tới dữ liệu.
  // Nếu SAI kiểu -> Trả về nullptr (Không hề ném Exception, chạy cực nhanh).
  if (int *pVal = std::get_if<int>(&v)) {
    std::cout << "Doc an toan bang get_if: " << *pVal << "\n";
  } else {
    std::cout << "Khong phai int!\n";
  }
}

// ==========================================
// 3. MỨC NÂNG CAO: VẤN ĐỀ KHỞI TẠO VÀ STD::MONOSTATE
// ==========================================
void variant_monostate() {
  std::cout << "\n--- 3. ADVANCED: STD::MONOSTATE ---\n";

  // LỖI KINH ĐIỂN: std::variant luôn ép kiểu ĐẦU TIÊN phải có Default
  // Constructor. std::variant<KhongCoDefault, int> v_loi; // -> SẼ BỊ LỖI BIÊN
  // DỊCH!

  // GIẢI PHÁP: Dùng std::monostate (Một kiểu dữ liệu rỗng, đóng vai trò như
  // 'null' hoặc 'empty') Đặt nó ở đầu danh sách để cho phép variant được khởi
  // tạo rỗng.
  std::variant<std::monostate, KhongCoDefault, int> v_chuan;

  if (std::holds_alternative<std::monostate>(v_chuan)) {
    std::cout << "Variant hien dang 'rong' (monostate).\n";
  }

  // Gán dữ liệu thật vào
  v_chuan = KhongCoDefault(99);
  std::cout << "Da gan KhongCoDefault, ID = "
            << std::get<KhongCoDefault>(v_chuan).id << "\n";
}

// ==========================================
// 4. MỨC CHUYÊN GIA (EXPERT): STD::VISIT VÀ PATTERN MATCHING
// ==========================================

// MA THUẬT CỦA C++17: THE OVERLOADED PATTERN
// Một Struct kế thừa từ nhiều Lambda functions để xử lý gọn gàng std::visit
template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};
// Deduction guide cho C++17 (C++20 không cần dòng này nữa)
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

void variant_visit_va_pattern_matching() {
  std::cout << "\n--- 4. EXPERT: STD::VISIT (MA THUAT TOI THUONG) ---\n";

  std::variant<int, double, std::string> v1 = 42;
  std::variant<int, double, std::string> v2 = "Lap trinh C++";

  // VẤN ĐỀ: Khi có 1 danh sách (Vector) chứa các Variant, làm sao để duyệt và
  // in mà không cần phải dùng 1 đống lệnh if (holds_alternative) rối rắm?

  std::vector<std::variant<int, double, std::string>> danh_sach = {10, 3.14,
                                                                   "Hello"};

  std::cout << "> Cach 1: Dung std::visit voi Generic Lambda (auto)\n";
  for (auto &item : danh_sach) {
    // std::visit sẽ TỰ ĐỘNG CHUI VÀO Variant, xem nó là kiểu gì,
    // và tự pass dữ liệu đó vào biến 'val' của Lambda.
    std::visit(
        [](auto &&val) { std::cout << "  Du lieu chung: " << val << "\n"; },
        item);
  }

  std::cout << "\n> Cach 2: Dung Overloaded Pattern (Giong PATTERN MATCHING "
               "cua Rust/Swift)\n";
  for (auto &item : danh_sach) {
    std::visit(overloaded{[](int arg) {
                            std::cout << "  Xu ly INT: Toi nhan len 2 -> "
                                      << arg * 2 << "\n";
                          },
                          [](double arg) {
                            std::cout << "  Xu ly DOUBLE: Toi chia 2 -> "
                                      << arg / 2.0 << "\n";
                          },
                          [](std::string arg) {
                            std::cout << "  Xu ly STRING: Do dai chuoi la -> "
                                      << arg.length() << "\n";
                          }},
               item);
  }
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== KHOA HOC STD::VARIANT (C++17) =====\n";

  variant_co_ban();
  variant_xu_ly_loi();
  variant_monostate();
  variant_visit_va_pattern_matching();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}