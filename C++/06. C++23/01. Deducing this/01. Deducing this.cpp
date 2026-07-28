#include <iostream>
#include <string>
#include <type_traits> // Cho std::is_same_v
#include <utility>     // Cho std::forward


// ==========================================
// CẢNH BÁO QUAN TRỌNG
// ==========================================
/*
 TÍNH NĂNG NÀY CHỈ CÓ TỪ C++23 TRỞ ĐI!
 Nếu trình biên dịch báo lỗi ở chữ 'this', hãy đảm bảo bạn đã bật cờ -std=c++2b
 hoặc -std=c++23.
*/

// ==========================================
// 1. MỨC CƠ BẢN: XÓA BỎ SỰ TRÙNG LẶP CODE (DEDUPLICATION)
// ==========================================

/*
 VẤN ĐỀ TRƯỚC C++23:
 Để viết một hàm Getter hoàn hảo, trả về đúng kiểu (Tham chiếu, Hằng, hoặc
 Move), bạn phải copy/paste ra 4 hàm y hệt nhau chỉ khác mỗi chữ ký cuối cùng
 (const, &, &&).
*/
class VanBanCu {
  std::string data = "C++98 rat met moi";

public:
  std::string &get() & { return data; }              // Cho lvalue thường
  const std::string &get() const & { return data; }  // Cho const lvalue
  std::string &&get() && { return std::move(data); } // Cho rvalue (move)
};

/*
 GIẢI PHÁP C++23: Deducing 'this'
 Bạn được phép truyền đối tượng hiện tại vào làm tham số ĐẦU TIÊN của hàm,
 bắt đầu bằng chữ khóa `this`.
 -> C++ sẽ TỰ ĐỘNG nhận diện nó là const, &, hay &&. 1 hàm cân tất cả!
*/
class VanBanMoi {
  std::string data = "C++23 sieu ngan gon!";

public:
  // 'this auto&& self' chính là C++23. 'self' có thể đặt tên tùy ý (như 'me',
  // 'obj'). Hàm này ngầm định là một Template, 'self' sẽ khớp với mọi trạng
  // thái của object.
  template <typename Self> auto &&get(this Self &&self) {
    // Phải dùng std::forward để giữ nguyên trạng thái (Lvalue hay Rvalue) của
    // self
    return std::forward<Self>(self).data;
  }
};

void demo_deduplication() {
  std::cout << "\n--- 1. BAZIC: XOA BO TRUNG LAP (DEDUPLICATION) ---\n";

  VanBanMoi vb;
  const VanBanMoi vb_const;

  // Tự động trả về std::string&
  std::cout << "Goi tu object thuong: " << vb.get() << "\n";

  // Tự động trả về const std::string&
  std::cout << "Goi tu object const: " << vb_const.get() << "\n";

  // Tự động trả về std::string&& (Move semantics)
  std::string an_cap_data = std::move(vb).get();
  std::cout << "Goi tu object an danh (Rvalue): " << an_cap_data << "\n";
}

// ==========================================
// 2. MỨC TRUNG CẤP: PASS BY VALUE (TRUYỀN THEO GIÁ TRỊ)
// ==========================================
class HanhLy {
  int can_nang;

public:
  HanhLy(int kg) : can_nang(kg) {}

  // TRƯỚC C++23: Bạn không bao giờ có thể tạo ra một bản COPY của chính class
  // trong lúc gọi hàm thành viên (Bởi vì 'this' luôn là con trỏ).
  // TỪ C++23: Bạn có thể yêu cầu truyền 'this' BẰNG GIÁ TRỊ (Pass by value)!

  void in_sau_khi_tang(this HanhLy self) {
    // 'self' ở đây là một BẢN SAO ĐỘC LẬP của object gọi hàm!
    self.can_nang += 10;
    std::cout << "Trong ham (Da copy va tang 10): " << self.can_nang << " kg\n";
  }

  int get_nang() const { return can_nang; }
};

void demo_pass_by_value() {
  std::cout << "\n--- 2. INTERMEDIATE: PASS BY VALUE ---\n";
  HanhLy tui(20);
  tui.in_sau_khi_tang(); // In ra 30
  std::cout << "Thuc te hien tai: " << tui.get_nang()
            << " kg (Khong bi anh huong)\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: ĐỆ QUY LAMBDA (RECURSIVE LAMBDA)
// ==========================================
void demo_recursive_lambda() {
  std::cout << "\n--- 3. ADVANCED: DE QUY LAMBDA (RECURSIVE LAMBDA) ---\n";

  /*
   VẤN ĐỀ TRƯỚC C++23:
   Lambda ẩn danh không có tên, nên nó KHÔNG THỂ gọi lại chính nó (Đệ quy).
   Bạn phải dùng std::function (rất chậm) hoặc dùng mẹo auto& (rườm rà).
  */

  /*
   GIẢI PHÁP C++23:
   Truyền 'this auto const& self' vào Lambda! Nó sẽ tự tham chiếu đến chính cái
   Lambda đó.
  */
  auto fibonacci = [](this auto const &self, int n) -> int {
    if (n <= 1)
      return n;
    return self(n - 1) +
           self(n - 2); // Tự gọi lại chính mình vô cùng thanh lịch!
  };

  std::cout << "Fibonacci(6) = " << fibonacci(6) << "\n";
}

// ==========================================
// 4. MỨC CHUYÊN GIA: GIẾT CHẾT CRTP (CURIOUSLY RECURRING TEMPLATE PATTERN)
// ==========================================

/*
 LỊCH SỬ ĐEN TỐI:
 CRTP là một mẫu thiết kế C++ nổi tiếng dùng để thực hiện "Đa hình lúc biên
 dịch" (Tránh dùng virtual rất chậm). Cách cũ siêu lằng nhằng: template<typename
 Derived> struct Base { ... static_cast<Derived*>(this) ... };
*/

// C++23: Deducing 'this' GIẾT CHẾT CÁCH VIẾT CRTP CŨ!
// Giờ đây, Base class không cần phải là một Template lằng nhằng nữa!

struct BaseMoi {
  // Chỉ cần dùng 'this auto&& self', C++ sẽ tự động ép kiểu 'self' thành class
  // Con (Derived) lúc biên dịch!
  void interface(this auto &&self) {
    std::cout << "Base dang dieu phoi... Chuyen qua cho Derived: ";
    // Tự động gọi hàm implementation() của class Con, không cần Virtual, tốc độ
    // bàn thờ!
    self.implementation();
  }
};

struct DerivedMoi_A : BaseMoi {
  void implementation() const { std::cout << "[Day la class Con A]\n"; }
};

struct DerivedMoi_B : BaseMoi {
  void implementation() const { std::cout << "[Day la class Con B]\n"; }
};

void demo_crtp_killer() {
  std::cout
      << "\n--- 4. EXPERT: KHAI TU CRTP CU (COMPILE-TIME POLYMORPHISM) ---\n";

  DerivedMoi_A objA;
  DerivedMoi_B objB;

  // Mặc dù gọi hàm từ Base, nhưng 'this auto&&' tự suy luận ra kiểu Con!
  objA.interface();
  objB.interface();
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== C++23: DEDUCING THIS (EXPLICIT OBJECT PARAMETER) =====\n";

  demo_deduplication();
  demo_pass_by_value();
  demo_recursive_lambda();
  demo_crtp_killer();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}