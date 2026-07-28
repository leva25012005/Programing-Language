#include <iostream>
#include <string>
#include <tuple> // Thư viện cốt lõi cho Tuple
#include <type_traits>
#include <utility>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Tuple là gì?
    - Nếu std::pair chỉ cho phép gộp ĐÚNG 2 biến (first và second),
      thì std::tuple là phiên bản nâng cấp, cho phép gộp N biến (0, 1, 2, 3...
 biến) thuộc các kiểu dữ liệu KHÁC NHAU vào làm một.

 2. Đặc điểm cốt lõi:
    - Kích thước và các kiểu dữ liệu của Tuple phải được xác định ngay lúc biên
 dịch (Compile-time).
    - KHÔNG thể dùng vòng lặp for (int i = 0...) để duyệt qua Tuple như mảng,
      vì mỗi phần tử của Tuple có thể mang một kiểu dữ liệu khác nhau.
      Phải truy cập bằng std::get<Index> với Index là hằng số.
*/

// ==========================================
// CLASS PHỤ TRỢ CHO PHẦN CHUYÊN GIA
// ==========================================
class NhanVien {
public:
  std::string ten;
  int tuoi;
  double luong;

  NhanVien(std::string t, int tu, double l) : ten(t), tuoi(tu), luong(l) {
    std::cout << "  [Khoi tao] Nhan vien: " << ten << "\n";
  }
};

void ham_xu_ly_thong_tin(std::string ten, int tuoi, double luong) {
  std::cout << "  -> Ham dang xu ly: " << ten << " (" << tuoi
            << " tuoi), Luong: " << luong << "\n";
}

// ==========================================
// 1. MỨC CƠ BẢN: KHỞI TẠO VÀ TRUY XUẤT
// ==========================================
void tuple_co_ban() {
  std::cout << "\n--- 1. BAZIC: KHOI TAO & TRUY XUAT ---\n";

  // Cách 1: Khởi tạo truyền thống
  std::tuple<int, double, std::string> t1(10, 3.14, "C++");

  // Cách 2: std::make_tuple (Trình biên dịch tự đoán kiểu)
  auto t2 = std::make_tuple(99, 5.5f, std::string("Hello"));

  // TRUY XUẤT THEO VỊ TRÍ (Index phải là hằng số)
  // t1[0] là LỖI BIÊN DỊCH. Phải dùng std::get<0>
  std::cout << "Phan tu thu 0: " << std::get<0>(t1) << "\n";
  std::cout << "Phan tu thu 2: " << std::get<2>(t1) << "\n";

  // Thay đổi giá trị trực tiếp
  std::get<0>(t1) = 999;

  // C++14: TRUY XUẤT THEO KIỂU DỮ LIỆU (Chỉ hoạt động nếu kiểu đó là duy nhất
  // trong Tuple) Vì t1 chỉ có duy nhất 1 biến std::string, ta có thể lấy thẳng
  // nó ra mà không cần nhớ Index!
  std::cout << "Phan tu kieu chuoi (C++14): " << std::get<std::string>(t1)
            << "\n";
}

// ==========================================
// 2. MỨC TRUNG CẤP: BUNG GÓI (UNPACKING)
// ==========================================
std::tuple<std::string, int, bool> get_user_info() {
  return {"Le Van Nhan", 25, true};
}

void tuple_unpacking() {
  std::cout << "\n--- 2. INTERMEDIATE: BUNG GOI (UNPACKING) ---\n";

  // a. Cách C++11: Dùng std::tie
  std::string ten;
  int tuoi;
  bool dang_hoat_dong;

  // Bung giá trị từ tuple đập thẳng vào 3 biến đã khai báo
  std::tie(ten, tuoi, dang_hoat_dong) = get_user_info();
  std::cout << "C++11: " << ten << ", " << tuoi << " tuoi\n";

  // MẸO: Dùng std::ignore nếu bạn chỉ muốn lấy Tuổi, bỏ qua Tên và Trạng thái
  // để tiết kiệm RAM
  int chi_lay_tuoi;
  std::tie(std::ignore, chi_lay_tuoi, std::ignore) = get_user_info();

  // b. Cách C++17: Structured Bindings (Thanh lịch tối thượng)
  // Không cần khai báo biến trước, code cực kỳ ngắn gọn!
  auto [name_17, age_17, is_active_17] = get_user_info();
  std::cout << "C++17: " << name_17 << ", " << age_17 << " tuoi\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: GỘP (CONCAT) VÀ SO SÁNH
// ==========================================
void tuple_nang_cao() {
  std::cout << "\n--- 3. ADVANCED: TUPLE_CAT & SO SANH ---\n";

  auto t1 = std::make_tuple(1, "Mot");
  auto t2 = std::make_tuple(3.14, 'A');
  auto t3 = std::make_tuple(true);

  // Gộp nhiều Tuple lại thành 1 siêu Tuple khổng lồ (std::tuple_cat)
  auto sieu_tuple = std::tuple_cat(t1, t2, t3);

  // sieu_tuple mang kiểu: std::tuple<int, const char*, double, char, bool>
  std::cout << "Phan tu cuoi cua Sieu Tuple la: " << std::get<4>(sieu_tuple)
            << "\n";

  // Tuple hỗ trợ sẵn phép toán so sánh từ điển (Lexicographical) giống hệt
  // std::pair
  auto tA = std::make_tuple(1, "A");
  auto tB = std::make_tuple(1, "B");

  if (tA < tB) {
    std::cout << "tA < tB (Vi 'A' < 'B')\n";
  }
}

// ==========================================
// 4. MỨC CHUYÊN GIA: C++17 TƯƠNG TÁC VỚI HÀM (STD::APPLY)
// ==========================================
void tuple_chuyen_gia_cpp17() {
  std::cout << "\n--- 4. EXPERT: C++17 STD::APPLY & MAKE_FROM_TUPLE ---\n";

  auto data_goi = std::make_tuple("Nguyen Van B", 30, 15.5);

  // VẤN ĐỀ: Ta có 1 Tuple chứa (Tên, Tuổi, Lương).
  // Ta có 1 hàm nhận vào 3 tham số riêng biệt: Tên, Tuổi, Lương.
  // Làm sao để "nhét" cái Tuple này vào hàm một cách tự động?

  // CÁCH CŨ: Quá cực khổ!
  // ham_xu_ly_thong_tin(std::get<0>(data_goi), std::get<1>(data_goi),
  // std::get<2>(data_goi));

  // CÁCH MỚI (C++17): std::apply
  // Nó sẽ tự động "đập vỡ" Tuple ra thành các tham số riêng biệt và đẩy vào
  // hàm.
  std::cout << "Dung std::apply:\n";
  std::apply(ham_xu_ly_thong_tin, data_goi);

  // Tương tự, dùng std::make_from_tuple (C++17) để KHỞI TẠO OBJECT trực tiếp từ
  // 1 Tuple! Nó lấy 3 phần tử trong Tuple nhét thẳng vào Constructor của class
  // NhanVien.
  std::cout << "Dung std::make_from_tuple:\n";
  NhanVien nv = std::make_from_tuple<NhanVien>(data_goi);
}

// ==========================================
// 5. CẢNH GIỚI CAO NHẤT: SIÊU LẬP TRÌNH (METAPROGRAMMING)
// ==========================================
void tuple_metaprogramming() {
  std::cout << "\n--- 5. MASTER: COMPILE-TIME METAPROGRAMMING ---\n";

  using KieuTuple = std::tuple<int, double, std::string>;

  // a. std::tuple_size: Hỏi xem Tuple này có bao nhiêu phần tử NGAY LÚC BIÊN
  // DỊCH
  constexpr size_t so_luong = std::tuple_size_v<KieuTuple>;
  std::cout << "Kich thuoc Tuple (Compile-time): " << so_luong << "\n";

  // b. std::tuple_element: Lấy ra kiểu dữ liệu của phần tử thứ N NGAY LÚC BIÊN
  // DỊCH Khai báo một biến 'bien_ao' mang kiểu dữ liệu giống hệt phần tử số 1
  // (double)
  std::tuple_element_t<1, KieuTuple> bien_ao = 9.99;

  std::cout << "bien_ao mang kieu double co gia tri: " << bien_ao << "\n";

  // Chững minh bằng type_traits
  if (std::is_same_v<decltype(bien_ao), double>) {
    std::cout
        << "-> Da xac nhan kieu du lieu duoc boc tach chinh xac la double!\n";
  }
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  tuple_co_ban();
  tuple_unpacking();
  tuple_nang_cao();
  tuple_chuyen_gia_cpp17();
  tuple_metaprogramming();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}