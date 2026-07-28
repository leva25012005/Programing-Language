#include <algorithm>
#include <functional> // Cho std::function
#include <iostream>
#include <memory> // Cho std::unique_ptr
#include <string>
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Lambda Expression là gì?
    - Là một "Hàm ẩn danh" (Hàm không cần đặt tên) có thể được viết ngay bên
 trong hàm khác.
    - Được thiết kế để viết code ngắn gọn, dùng 1 lần rồi bỏ, đặc biệt là khi
 truyền vào các thuật toán STL (như std::sort, std::for_each).

 2. Cú pháp kinh điển:
    [capture](parameters) -> return_type { body }

    - [capture]: Danh sách bắt biến (Đem các biến bên ngoài vào trong Lambda).
    - (parameters): Tham số truyền vào giống hệt hàm bình thường.
    - -> return_type: Kiểu trả về (Thường bỏ qua để auto tự đoán).
    - { body }: Thân hàm.

 3. Bản chất dưới nền:
    - Khi bạn viết 1 Lambda, C++ compiler sẽ NGẦM tạo ra một Class (Functor)
      có chứa toán tử operator() và các biến thành viên (nếu có capture).
*/

// ==========================================
// 1. MỨC CƠ BẢN: CÚ PHÁP VÀ TRUYỀN THAM SỐ
// ==========================================
void lambda_co_ban() {
  std::cout << "\n--- 1. BAZIC: CU PHAP CO BAN ---\n";

  // a. Lambda đơn giản nhất (Không tham số, không bắt biến)
  auto chao_hoi = []() { std::cout << "Xin chao tu Lambda!\n"; };
  chao_hoi(); // Gọi hàm

  // b. Lambda có tham số và tự suy luận kiểu trả về
  auto cong_hai_so = [](int a, int b) {
    return a + b; // C++ tự hiểu hàm này trả về int
  };
  std::cout << "5 + 3 = " << cong_hai_so(5, 3) << "\n";

  // c. Ứng dụng thực tế: Dùng thay cho Custom Comparator
  std::vector<int> v = {4, 1, 8, 3, 9};

  // Sắp xếp giảm dần ngay tại chỗ, không cần viết hàm bên ngoài
  std::sort(v.begin(), v.end(), [](int a, int b) { return a > b; });

  std::cout << "Mang giam dan: ";
  for (int x : v)
    std::cout << x << " ";
  std::cout << "\n";
}

// ==========================================
// 2. MỨC TRUNG CẤP: BẮT BIẾN (CAPTURE LIST) VÀ MUTABLE
// ==========================================
void lambda_capture_list() {
  std::cout << "\n--- 2. INTERMEDIATE: CAPTURE LIST & MUTABLE ---\n";

  int he_so = 10;
  int cong_them = 5;

  // a. [=] Bắt TOÀN BỘ biến bên ngoài bằng CÁCH COPY (READ-ONLY)
  auto nhan_he_so = [=](int x) {
    // he_so = 20; // LỖI BIÊN DỊCH! Biến copy vào Lambda mặc định là hằng số
    // (const).
    return x * he_so + cong_them;
  };
  std::cout << "Gia tri (2 * 10 + 5) = " << nhan_he_so(2) << "\n";

  // b. [&] Bắt TOÀN BỘ biến bên ngoài bằng THAM CHIẾU (Có thể sửa đổi gốc)
  auto sua_bien_goc = [&](int x) {
    he_so = x; // Hợp lệ, và sẽ làm thay đổi biến 'he_so' ở bên ngoài
  };
  sua_bien_goc(99);
  std::cout << "Bien he_so sau khi sua bang Tham chieu: " << he_so << "\n";

  // c. Bắt hỗn hợp (Mixed Capture)
  // [=, &cong_them]: Copy tất cả, NHƯNG bắt 'cong_them' bằng tham chiếu.
  // [&, he_so]: Tham chiếu tất cả, NHƯNG copy 'he_so'.

  // d. TỪ KHÓA 'mutable': Phá vỡ giới hạn Read-only của phép Copy
  int dem_so = 0;
  // Bắt bằng Copy, nhưng cho phép sửa biến Copy đó ở bên trong Lambda (biến gốc
  // ở ngoài KHÔNG đổi)
  auto bo_dem = [dem_so]() mutable {
    dem_so++;
    return dem_so;
  };

  std::cout << "Goi bo dem lan 1: " << bo_dem() << "\n"; // In 1
  std::cout << "Goi bo dem lan 2: " << bo_dem() << "\n"; // In 2
  std::cout << "Bien dem_so GOC o ben ngoai: " << dem_so << " (Van la 0)\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: GENERIC LAMBDA & INIT CAPTURE (C++14)
// ==========================================
void lambda_cxx14_modern() {
  std::cout << "\n--- 3. ADVANCED: C++14 GENERIC & INIT CAPTURE ---\n";

  // a. C++14 Generic Lambda: Dùng 'auto' làm tham số
  // Biến Lambda này thành một Template ẩn (Có thể nhận bất kỳ kiểu dữ liệu nào)
  auto in_hai_gia_tri = [](auto a, auto b) {
    std::cout << "  - " << a << " va " << b << "\n";
  };

  in_hai_gia_tri(10, 20);           // Chạy với int
  in_hai_gia_tri("Xin chao", 3.14); // Chạy với const char* và double

  // b. C++14 Init-Capture (Bắt biến có khởi tạo)
  // VẤN ĐỀ: unique_ptr KHÔNG THỂ COPY. Nếu dùng [=] sẽ lỗi!
  // GIẢI PHÁP: Dùng cú pháp [ten_moi = bieu_thuc] để MOVE thẳng nó vào Lambda.
  std::unique_ptr<int> ptr = std::make_unique<int>(999);

  auto doc_con_tro = [p = std::move(ptr)]() {
    std::cout << "  - Gia tri trong unique_ptr: " << *p << "\n";
  };

  doc_con_tro();
  if (ptr == nullptr)
    std::cout << "  - ptr goc da bi hut can (nullptr)!\n";
}

// ==========================================
// 4. MỨC CHUYÊN GIA: TEMPLATE LAMBDA (C++20) & THỦ THUẬT IIFE
// ==========================================
void lambda_cxx20_expert() {
  std::cout << "\n--- 4. EXPERT: C++20 TEMPLATE LAMBDA & IIFE ---\n";

  // a. C++20 Explicit Template Lambda
  // Generic Lambda (auto) rất mạnh, nhưng nếu bạn muốn ÉP BUỘC cả 2 tham số
  // phải CÙNG MỘT KIỂU thì 'auto' không làm được. C++20 giải quyết bằng cách
  // thêm <T>
  auto cong_hai_kieu_giong_nhau = []<typename T>(T a, T b) { return a + b; };

  std::cout << "Cong int: " << cong_hai_kieu_giong_nhau(5, 10) << "\n";
  // cong_hai_kieu_giong_nhau(5, 3.14); // LỖI BIÊN DỊCH: int và double không
  // khớp T!

  // b. IIFE (Immediately Invoked Function Expression)
  // Viết Lambda xong, GỌI CHẠY NGAY LẬP TỨC bằng ().
  // Thường dùng để khởi tạo một biến const mang logic phức tạp.
  const int ket_qua = []() {
    int tong = 0;
    for (int i = 1; i <= 10; ++i)
      tong += i;
    return tong;
  }(); // Cặp ngoặc () này để kích hoạt chạy hàm ngay

  std::cout << "Ket qua tu IIFE: " << ket_qua << "\n";
}

// ==========================================
// 5. CẠM BẪY NGUY HIỂM NHẤT CỦA OOP: BẮT [this]
// ==========================================
class NhanVien {
private:
  std::string ten = "Nhan Le";

public:
  // Trả về một hàm std::function
  std::function<void()> taoLambdaLoi() {
    // [=] ở trong Class không bắt COPY các biến như 'ten',
    // mà nó ngầm bắt CON TRỎ [this].
    // Điều này dẫn đến Lỗi Dangling Pointer nếu Object NhanVien bị hủy!
    return [=]() { std::cout << "Ten toi la: " << ten << "\n"; };
  }

  std::function<void()> taoLambdaAnToan_Cxx17() {
    // C++17 cho phép bắt [*this]: Bắt COPY TOÀN BỘ OBJECT hiện tại vào trong
    // Lambda. Dù Object gốc bên ngoài có bị Hủy, Lambda vẫn sống khỏe mạnh!
    return [*this]() { std::cout << "Ten toi la (An toan): " << ten << "\n"; };
  }
};

void lambda_trap_this() {
  std::cout << "\n--- 5. OOP TRAP: CAPTURING [*this] (C++17) ---\n";

  std::function<void()> ham_chay_sau;
  {
    NhanVien nv;
    ham_chay_sau = nv.taoLambdaAnToan_Cxx17();
  }
  // Đến đây, Object 'nv' đã CHẾT và bị xóa khỏi RAM.

  // Nếu dùng taoLambdaLoi(), dòng dưới đây sẽ gây CRASH hoặc in ra rác
  // (Dangling Pointer). Nhưng vì dùng [*this] của C++17, nó đã copy nguyên 1
  // Object NhanVien vào trong Lambda.
  ham_chay_sau(); // Vẫn in ra "Nhan Le" bình thường!
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== KHOA HOC C++ LAMBDA EXPRESSION =====\n";

  lambda_co_ban();
  lambda_capture_list();
  lambda_cxx14_modern();
  lambda_cxx20_expert();
  lambda_trap_this();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}