#include <array> // std::array
#include <iostream>
#include <string>
#include <tuple>       // std::tuple
#include <type_traits> // std::is_same_v
#include <typeinfo>
#include <utility> // std::pair
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY - C++17 TRỞ LÊN)
// ==========================================
/*
 1. CTAD là gì?
    - Trước C++17: Khởi tạo Template Class BẮT BUỘC phải viết rõ kiểu dữ liệu
 trong ngoặc nhọn <>. Ví dụ: std::pair<int, double> p(1, 2.5);
    - Từ C++17 (CTAD): Trình biên dịch TỰ ĐỘNG nhìn vào tham số truyền vào (1
 và 2.5) để suy luận ra kiểu <int, double>. Bạn chỉ cần viết: std::pair
 p(1, 2.5);

 2. Sự khác biệt giữa 'auto' và CTAD:
    - 'auto' dùng cho SUY LUẬN BIẾN (Variable).
    - 'CTAD' dùng cho SUY LUẬN LỚP (Class Template).

 3. Cơ chế hoạt động:
    - Trình biên dịch sẽ xem xét tất cả các Constructor của Class đó.
    - So khớp kiểu của các tham số bạn truyền vào với các Constructor.
    - Nếu khớp, nó chốt kiểu dữ liệu. Nếu mơ hồ, nó báo lỗi.
*/

// ==========================================
// CLASS VÀ STRUCT PHỤ TRỢ CHO PHẦN NÂNG CAO
// ==========================================
// Một Class Template Tùy chỉnh (Do bạn tự viết)
template <typename T> class HopSua {
public:
  T the_tich;
  HopSua(T v) : the_tich(v) {
    std::cout << "  [Tao Hop Sua] Kieu du lieu: " << typeid(T).name() << "\n";
  }
};

// Một Class Template phức tạp hơn cần Deduction Guide
template <typename T> class ThongBao {
public:
  T tin_nhan;
  ThongBao(T msg) : tin_nhan(msg) {}
};

// ==========================================
// 1. MỨC CƠ BẢN: CTAD VỚI THƯ VIỆN CHUẨN (STL)
// ==========================================
void ctad_co_ban() {
  std::cout << "\n--- 1. BAZIC: CTAD VOI THU VIEN STL ---\n";

  // a. std::pair
  // C++98: std::pair<int, double> p(1, 2.5);
  // C++11: auto p = std::make_pair(1, 2.5);
  // C++17 CTAD: Tuyệt đẹp và thanh lịch!
  std::pair p(1, 2.5);

  // b. std::tuple
  std::tuple t(10, "Hello", 3.14f); // Tự hiểu: tuple<int, const char*, float>

  // c. std::vector
  std::vector v = {1, 2, 3}; // Tự hiểu: std::vector<int>

  // d. std::array (TÍNH NĂNG ĐỈNH NHẤT CỦA CTAD)
  // Trước đây khai báo array bắt buộc phải ghi rõ Số Lượng. Bây giờ Compiler tự
  // đếm!
  std::array arr = {10, 20, 30, 40}; // Tự hiểu: std::array<int, 4>

  std::cout << "Da khoi tao vector " << v.size() << " phan tu va array "
            << arr.size() << " phan tu bang CTAD.\n";
}

// ==========================================
// 2. MỨC TRUNG CẤP: CTAD CHO CLASS TỰ VIẾT (USER-DEFINED)
// ==========================================
void ctad_cho_class_tu_viet() {
  std::cout << "\n--- 2. INTERMEDIATE: CTAD CHO CLASS CUSTOM ---\n";

  // Bạn KHÔNG CẦN viết thêm bất kỳ dòng code nào.
  // CTAD tự động hoạt động trên các Class do chính bạn viết!

  HopSua hop1(
      500); // Compiler nhìn vào số 500 (int) -> Tự nội suy ra HopSua<int>
  HopSua hop2(
      1.5); // Compiler nhìn vào số 1.5 (double) -> Tự nội suy ra HopSua<double>

  // Cạm bẫy chuỗi ký tự thô:
  HopSua hop3("1 Lit"); // Sẽ bị suy luận thành HopSua<const char*>
}

// ==========================================
// 3. MỨC NÂNG CAO: USER-DEFINED DEDUCTION GUIDES
// ==========================================
/*
 VẤN ĐỀ: Khi ta truyền "Xin Chao", C++ hiểu nó là (const char*).
 Trình biên dịch sẽ tạo ra ThongBao<const char*>.
 Điều này rất nguy hiểm vì con trỏ có thể bị lỗi bộ nhớ lơ lửng.
 Ta muốn C++ phải TỰ ĐỘNG chuyển nó thành std::string (ThongBao<std::string>).

 GIẢI PHÁP: Viết Deduction Guide (Hướng dẫn suy luận).
 Cú pháp: TenClass(Kieu_Tham_So) -> TenClass<Kieu_Thuc_Su_Muon_Dung>;
 Lệnh này phải đặt ở phạm vi toàn cục (bên ngoài các hàm), ngay dưới định nghĩa
 Class.
*/

// VIẾT HƯỚNG DẪN Ở ĐÂY:
// "Này Trình biên dịch, nếu thấy ai khởi tạo ThongBao bằng chuỗi thô (const
// char*), hãy tự động ép kiểu Template T thành std::string cho tôi!"
ThongBao(const char *) -> ThongBao<std::string>;

void ctad_deduction_guide() {
  std::cout << "\n--- 3. ADVANCED: DEDUCTION GUIDES ---\n";

  ThongBao tb("He thong bi loi!");

  // Kiểm tra xem CTAD có nghe lời Deduction Guide của ta không?
  if (std::is_same_v<decltype(tb.tin_nhan), std::string>) {
    std::cout << "Thanh cong! CTAD da tu dong ep kieu sang std::string, khong "
                 "phai const char*.\n";
  }
}

// ==========================================
// 4. MỨC CHUYÊN GIA: DEDUCTION GUIDE CHO ITERATOR
// ==========================================
// Giả sử ta viết một Class Vecto Custom, khởi tạo từ 2 Iterator (begin, end)
// của một mảng khác.
template <typename T> class VectoTuyChinh {
public:
  VectoTuyChinh(T a, T b) { std::cout << "  [Tao Vecto] Bang kieu thuong.\n"; }

  template <typename Iter> VectoTuyChinh(Iter dau, Iter cuoi) {
    std::cout << "  [Tao Vecto] Bang hai Iterator.\n";
  }
};

// VẤN ĐỀ CHÍ MẠNG: Nếu ta gọi VectoTuyChinh(v.begin(), v.end())
// CTAD sẽ tự lấy kiểu của Iterator ném thẳng vào T -> Tạo ra
// VectoTuyChinh<std::vector<int>::iterator>. Rất vô nghĩa! Ta muốn nó lấy KIỂU
// DỮ LIỆU BÊN TRONG ITERATOR đó (tức là 'int').

// DEDUCTION GUIDE BẬC THẦY:
// Dùng std::iterator_traits để móc ruột Iterator, lôi cái 'value_type' (int) ra
// ngoài!
template <typename Iter>
VectoTuyChinh(Iter, Iter)
    -> VectoTuyChinh<typename std::iterator_traits<Iter>::value_type>;

void ctad_iterator_expert() {
  std::cout << "\n--- 4. EXPERT: ITERATOR DEDUCTION GUIDE ---\n";

  std::vector<double> v_goc = {1.1, 2.2, 3.3};

  // Nhờ Deduction Guide bên trên, C++ nhìn vào (v.begin, v.end)
  // Tự động móc ra kiểu 'double', và suy luận thành VectoTuyChinh<double>.
  VectoTuyChinh vec_moi(v_goc.begin(), v_goc.end());
  std::cout << "Da suy luan thanh cong kieu ben trong Iterator!\n";
}

// ==========================================
// 5. CẠM BẪY COPY CONSTRUCTOR TRONG CTAD
// ==========================================
void ctad_cam_bay() {
  std::cout << "\n--- 5. TRAPS: COPY vs WRAPPING ---\n";

  std::vector v1 = {1, 2, 3}; // std::vector<int>

  // Khi bạn đưa v1 vào v2. Có 2 luồng tư duy:
  // Tùy chọn A: Copy Constructor -> v2 là std::vector<int> giống hệt v1.
  // Tùy chọn B: Khởi tạo Wrap -> v2 là std::vector<std::vector<int>> (Một mảng
  // chứa mảng v1).

  std::vector v2(v1); // CTAD ƯU TIÊN COPY CONSTRUCTOR TỐI ĐA!
  std::cout << "Kieu cua v2: " << typeid(v2).name()
            << " (Cung la std::vector<int>)\n";

  // Nếu bạn THỰC SỰ muốn Wrap (Tùy chọn B), bạn phải ép nó bằng ngoặc nhọn:
  std::vector v3 = {v1};
  std::cout << "So phan tu cua v3: " << v3.size()
            << " (Day la vector<vector<int>>. Chứa 1 phan tu duy nhat la v1)\n";
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  ctad_co_ban();
  ctad_cho_class_tu_viet();
  ctad_deduction_guide();
  ctad_iterator_expert();
  ctad_cam_bay();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}