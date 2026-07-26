#include <array>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <utility>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Structured Bindings (Ràng buộc có cấu trúc) là gì?
    - Ra mắt trong C++17, tính năng này giúp "bung gói" (unpack) các cấu trúc dữ
 liệu chứa nhiều phần tử (Tuple, Pair, Array, Struct) thành các biến độc lập chỉ
 với 1 dòng code.
    - Lấy cảm hứng trực tiếp từ Python (Tuple unpacking).

 2. Cú pháp cốt lõi:
    auto [bien_1, bien_2, ...] = bieu_thuc;

 3. Ba lợi ích khổng lồ:
    - Code vô cùng ngắn gọn, sạch sẽ (clean code).
    - Không cần khai báo biến trước (C++11 std::tie bắt buộc phải khai báo biến
 trước).
    - Kết hợp hoàn hảo với toán tử Tham chiếu (&) và Hằng (const) để tối ưu hiệu
 năng.
*/

// ==========================================
// CẤU TRÚC PHỤ TRỢ
// ==========================================
struct ToaDo3D {
  double x, y, z;
};

// ==========================================
// 1. MỨC CƠ BẢN: BUNG GÓI ARRAY, PAIR VÀ TUPLE
// ==========================================
void sb_co_ban() {
  std::cout << "\n--- 1. BAZIC: UNPACK ARRAY, PAIR, TUPLE ---\n";

  // a. C-Style Array & std::array
  int mang_thuong[] = {10, 20};
  auto [a, b] = mang_thuong;
  std::cout << "Array unpack: a=" << a << ", b=" << b << "\n";

  std::array<std::string, 3> mau_sac = {"Do", "Xanh", "Vang"};
  auto [m1, m2, m3] = mau_sac;
  std::cout << "std::array unpack: " << m1 << ", " << m2 << ", " << m3 << "\n";

  // b. std::pair
  std::pair<int, std::string> id_ten = {101, "Le Nhan"};
  auto [id, ten] = id_ten;
  std::cout << "Pair unpack: ID " << id << " la " << ten << "\n";

  // c. std::tuple (Nhiều hơn 2 phần tử)
  std::tuple<int, double, char> du_lieu_hon_hop = {1, 3.14, 'A'};
  auto [so_nguyen, so_thuc, ky_tu] = du_lieu_hon_hop;
  std::cout << "Tuple unpack: " << so_nguyen << " - " << so_thuc << " - "
            << ky_tu << "\n";
}

// ==========================================
// 2. MỨC TRUNG CẤP: RANGE-BASED FOR VÀ THAM CHIẾU (&)
// ==========================================
void sb_vong_lap_va_tham_chieu() {
  std::cout << "\n--- 2. INTERMEDIATE: MAP ITERATION & REFERENCE ---\n";

  std::map<int, std::string> kho_hang = {{1, "Kiem"}, {2, "Khien"}};

  // A. BUNG GÓI TRONG VÒNG LẶP (Giống hệt ví dụ bạn đưa ra)
  // Thay vì dùng it->first và it->second rườm rà, ta bung gói trực tiếp.
  // LƯU Ý KINH ĐIỂN: LUÔN DÙNG 'const auto&' khi chỉ đọc để tránh Copy toàn bộ
  // string!
  std::cout << "Duyet Map bang Structured Bindings:\n";
  for (const auto &[ma_so, vat_pham] : kho_hang) {
    std::cout << "  - Ma: " << ma_so << " -> " << vat_pham << "\n";
  }

  // B. SỰ KHÁC BIỆT KHI DÙNG THAM CHIẾU ĐỂ SỬA DỮ LIỆU
  ToaDo3D vi_tri = {10.0, 20.0, 30.0};

  // Nếu chỉ dùng 'auto', C++ sẽ COPY dữ liệu ra vùng nhớ mới.
  auto [x1, y1, z1] = vi_tri;
  x1 = 999.0; // Không ảnh hưởng tới struct gốc!
  std::cout << "Gia tri goc sau khi sua ban COPY: x=" << vi_tri.x
            << "\n"; // Vẫn là 10.0

  // Nếu dùng 'auto&', x2, y2, z2 chính là các TÊN GỌI KHÁC của các biến trong
  // struct.
  auto &[x2, y2, z2] = vi_tri;
  x2 = 999.0; // Tác động thẳng vào struct gốc!
  std::cout << "Gia tri goc sau khi sua bang THAM CHIEU: x=" << vi_tri.x
            << " (Da doi thanh 999)\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: UNPACK CUSTOM STRUCT
// ==========================================
// Hàm trả về thẳng một Struct thay vì dùng std::tuple rườm rà
ToaDo3D lay_toa_do_nguoi_choi() { return {5.5, 6.6, 7.7}; }

void sb_nang_cao_struct() {
  std::cout << "\n--- 3. ADVANCED: STRUCT UNPACKING ---\n";

  // Khi một Struct chỉ chứa các biến public (Plain Old Data),
  // bạn có thể bung gói nó tự động hệt như Tuple mà KHÔNG CẦN CẤU HÌNH GÌ THÊM.
  auto [x, y, z] = lay_toa_do_nguoi_choi();
  std::cout << "Toa do nguoi choi: x=" << x << ", y=" << y << ", z=" << z
            << "\n";
}

// ==========================================
// 4. MỨC CHUYÊN GIA: BUNG GÓI CLASS CÓ THUỘC TÍNH PRIVATE
// ==========================================
// Nếu Class có các biến private (Tính đóng gói OOP), Structured Bindings sẽ BÁO
// LỖI. Để vượt qua, ta phải "dạy" cho C++ biết làm sao để bung gói Class này
// thông qua 3 bước đặc biệt dưới đây.

class NhanVienBaoMat {
private:
  int id;
  std::string ten;

public:
  NhanVienBaoMat(int i, std::string t) : id(i), ten(t) {}

  // Bước 1: Viết hàm get<Index>() dạng template để trả về biến private
  template <std::size_t Index> decltype(auto) get() const {
    if constexpr (Index == 0)
      return id;
    else if constexpr (Index == 1)
      return ten;
  }
};

// Bước 2 & 3: Phải tiêm (inject) các khuôn mẫu vào không gian tên (namespace)
// của thư viện chuẩn std.
namespace std {
// Bước 2: Nói cho C++ biết Class này khi bung ra sẽ có bao nhiêu biến (Ở đây là
// 2 biến)
template <>
struct tuple_size<NhanVienBaoMat> : std::integral_constant<std::size_t, 2> {};

// Bước 3: Nói cho C++ biết kiểu dữ liệu của TỪNG biến khi bung ra là gì
template <> struct tuple_element<0, NhanVienBaoMat> {
  using type = int;
};
template <> struct tuple_element<1, NhanVienBaoMat> {
  using type = std::string;
};
} // namespace std

void sb_chuyen_gia_private_class() {
  std::cout << "\n--- 4. EXPERT: UNPACKING ENCAPSULATED PRIVATE CLASS ---\n";

  NhanVienBaoMat sếp(999, "Giam Doc Nhan");

  // Ma thuật C++17: Dù biến là Private, ta vẫn bung gói gọn gàng
  // vì C++ tự động đi tìm hàm get<Index> và các cấu hình tuple_size ta đã viết
  // ở trên!
  auto [ma_nhan_vien, ho_ten] = sếp;

  std::cout << "Ho so bi mat duoc mo: ID [" << ma_nhan_vien << "] - Ten ["
            << ho_ten << "]\n";
}

// ==========================================
// TỔNG KẾT & CẠM BẪY
// ==========================================
/*
 CẠM BẪY SỐ 1: Bỏ sót tham chiếu.
 - auto [k, v] = myMap; -> Mỗi vòng lặp bạn đang tạo ra một BẢN SAO của
 String/Object, game sẽ lag tung chảo.
 - SỬA LẠI: const auto& [k, v] = myMap;

 CẠM BẪY SỐ 2: Không được bỏ bớt biến.
 - Nếu Tuple trả về 3 biến, bạn PHẢI HỨNG ĐỦ 3 BIẾN: auto [a, b, c] = ...
 - Nếu bạn viết: auto [a, b] = ... -> LỖI BIÊN DỊCH.
 - (Nếu không muốn xài biến thứ 3, C++26 mới cho phép dùng _ để phớt lờ: auto
 [a, b, _]). Trong C++17, hãy đặt tên giả (vd: auto [a, b, bo_qua]).
*/

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  sb_co_ban();
  sb_vong_lap_va_tham_chieu();
  sb_nang_cao_struct();
  sb_chuyen_gia_private_class();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}