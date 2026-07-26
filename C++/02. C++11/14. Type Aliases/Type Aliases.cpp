#include <iostream>
#include <map>
#include <string>
#include <type_traits> // Chứa các công cụ biến đổi kiểu dữ liệu
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Type Alias là gì?
    - Là cách đặt một "Bí danh" (tên gọi khác ngắn gọn hơn, dễ hiểu hơn) cho một
      kiểu dữ liệu đã có sẵn. Nó KHÔNG tạo ra kiểu dữ liệu mới.

 2. typedef (C-Style) vs using (C++11 Modern Style):
    - 'typedef' là di sản từ ngôn ngữ C. Cú pháp của nó đôi khi rất lằng nhằng
 và khó đọc.
    - 'using' được giới thiệu từ C++11, cú pháp giống phép gán (TênMới = TênCũ)
 cực kỳ trực quan.
    - ĐẶC BIỆT: 'using' hỗ trợ Template (Alias Templates), điều mà 'typedef'
 KHÔNG THỂ LÀM ĐƯỢC!
*/

// ==========================================
// 1. MỨC CƠ BẢN: CÚ PHÁP TYPEDEF VÀ USING
// ==========================================
// Đặt tên ngắn gọn cho các kiểu dữ liệu dài dòng
typedef unsigned long long ULL_Cu; // Cách cũ (C-Style)
using ULL_Moi =
    unsigned long long; // Cách mới (C++11) - Đọc từ trái sang phải dễ hiểu hơn!

void type_alias_co_ban() {
  std::cout << "\n--- 1. BAZIC: TYPEDEF VS USING ---\n";

  ULL_Cu so_tien_lon = 999999999999;
  ULL_Moi khoang_cach = 15000000000;

  std::cout << "So tien: " << so_tien_lon << "\n";
  std::cout << "Khoang cach: " << khoang_cach << "\n";
}

// ==========================================
// 2. MỨC TRUNG CẤP: LÀM SẠCH CODE VỚI KIỂU PHỨC TẠP
// ==========================================
// a. Rút gọn STL Containers
using TuDien = std::map<std::string, std::vector<int>>;

// b. Con trỏ hàm (Function Pointers) - Đây là lúc 'using' tỏa sáng rực rỡ!
// Hãy xem cú pháp typedef kinh dị cỡ nào:
typedef void (*HamXuLy_Cu)(int, double);

// So sánh với using, rõ ràng rành mạch như một phép gán:
using HamXuLy_Moi = void (*)(int, double);

void in_so(int a, double b) {
  std::cout << "  Goi ham: " << a << " va " << b << "\n";
}

void type_alias_phuc_tap() {
  std::cout << "\n--- 2. INTERMEDIATE: LAM SACH CODE ---\n";

  TuDien bang_diem;
  bang_diem["Nhan"] = {9, 10, 8};

  std::cout << "Diem mon thu 2 cua Nhan: " << bang_diem["Nhan"][1] << "\n";

  // Sử dụng bí danh con trỏ hàm
  HamXuLy_Moi con_tro = in_so;
  con_tro(5, 3.14);
}

// ==========================================
// 3. MỨC NÂNG CAO: ALIAS TEMPLATES (ĐỘC QUYỀN CỦA 'USING')
// ==========================================
// VẤN ĐỀ: Typedef KHÔNG THỂ đi kèm với template!
// template <typename T> typedef std::vector<T> MyVector; // LỖI BIÊN DỊCH!

// GIẢI PHÁP: C++11 Alias Templates.
// Bạn có thể tạo ra một "Khuôn mẫu bí danh" chờ sẵn kiểu dữ liệu được nhét vào.

template <typename T>
using DanhSach =
    std::vector<T>; // Đổi tên std::vector thành DanhSach cho thân thiện

// Cố định 1 kiểu, để mở 1 kiểu. Ví dụ: Tạo Từ điển luôn có Key là string, Value
// tùy biến.
template <typename ValueType>
using TuDienChuoi = std::map<std::string, ValueType>;

void type_alias_templates() {
  std::cout << "\n--- 3. ADVANCED: ALIAS TEMPLATES (C++11) ---\n";

  // Sử dụng bí danh template
  DanhSach<int> day_so = {1, 2, 3};
  DanhSach<std::string> day_chu = {"Mot", "Hai"};

  TuDienChuoi<int> diem_thi = {{"Nhan", 10}, {"An", 9}};
  TuDienChuoi<std::string> bi_danh = {{"Nhan", "Boss"}, {"An", "Noob"}};

  std::cout << "Nhan duoc: " << diem_thi["Nhan"]
            << " diem, bi danh: " << bi_danh["Nhan"] << "\n";
}

// ==========================================
// 4. MỨC CHUYÊN GIA: LẬP TRÌNH SIÊU ĐỐI TƯỢNG (METAPROGRAMMING)
// ==========================================
// Trong thư viện chuẩn của C++, 'using' được dùng để chế tạo các "Hàm biến đổi
// kiểu dữ liệu". Bản chất của std::decay_t, std::remove_reference_t... chính là
// dùng 'using' bên dưới nền.

template <typename T> class HopChua {
public:
  // Định nghĩa bí danh BÊN TRONG Class.
  // Rất phổ biến trong STL (std::vector::value_type, std::vector::iterator).
  // Giúp các class/hàm bên ngoài biết được Class này đang chứa dữ liệu kiểu gì.
  using KieuDuLieu = T;
  using ConTroDuLieu = T *;

  KieuDuLieu gia_tri;
};

// Hàm Template trích xuất bí danh từ bên trong một Class khác.
// BẮT BUỘC phải dùng từ khóa 'typename' để nói với Compiler rằng KieuDuLieu là
// một Type, chứ không phải là một biến tĩnh (Static variable).
template <typename T_Hop> void in_kieu_hop(T_Hop hop) {
  using KieuThucSu = typename T_Hop::KieuDuLieu;

  KieuThucSu bien_ao =
      99.99; // Nếu T_Hop là HopChua<double>, bien_ao sẽ là double
  std::cout << "  Gia tri bien ao rut trich tu Class: " << bien_ao << "\n";
}

void type_alias_metaprogramming() {
  std::cout << "\n--- 4. EXPERT: METAPROGRAMMING & DEPENDENT TYPES ---\n";

  HopChua<double> hop_so_thuc;
  in_kieu_hop(hop_so_thuc);

  // Kỹ thuật gỡ bỏ Tham chiếu (Remove Reference) bằng thư viện <type_traits>
  // std::remove_reference_t<T> thực chất là bí danh cho
  // std::remove_reference<T>::type!
  int x = 10;
  int &ref_x = x;

  using KieuGoc =
      std::remove_reference_t<decltype(ref_x)>; // Lột xác ref_x từ 'int&' quay
                                                // về 'int'

  KieuGoc y = 50;
  y = 100; // y là bản copy, độc lập hoàn toàn với x

  std::cout << "KieuGoc doc lap: " << y << " (x van la " << x << ")\n";
}

// ==========================================
// TỔNG KẾT LUẬT LỰA CHỌN (BEST PRACTICES)
// ==========================================
/*
 1. QUÊN TYPEDEF ĐI: Trừ khi bạn đang bảo trì source code C cổ đại, hãy LUÔN
 LUÔN dùng `using` trong các dự án C++ hiện đại.
 2. Hữu ích nhất khi nào?
    - Rút gọn các Iterator dài như dải thiên hà.
    - Viết con trỏ hàm, con trỏ thành viên class.
    - Định nghĩa các "Đặc điểm kiểu" (Type traits) bên trong Class để tạo ra
 Generic Programming chuẩn mực.
*/

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  type_alias_co_ban();
  type_alias_phuc_tap();
  type_alias_templates();
  type_alias_metaprogramming();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}