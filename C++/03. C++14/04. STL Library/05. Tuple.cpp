#include <iostream>
#include <string>
#include <tuple>
#include <utility> // Chứa std::pair


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY) - TỪ C++14
// ==========================================
/*
 1. Tính năng std::get<Type> là gì?
    - Ra mắt từ C++14, nó cho phép bạn lấy dữ liệu ra khỏi Tuple/Pair dựa trên
      KIỂU DỮ LIỆU thay vì VỊ TRÍ INDEX (0, 1, 2...).
    - Cú pháp: std::get<Kieu_Du_Lieu>(Ten_Tuple)

 2. Tại sao nó tuyệt vời?
    - Đọc code dễ hiểu hơn (VD: std::get<string> dễ hiểu hơn std::get<2>).
    - An toàn khi tái cấu trúc (Refactoring): Nếu bạn đổi vị trí các phần tử
      trong khai báo Tuple, code dùng kiểu dữ liệu vẫn chạy đúng,
      còn code dùng Index sẽ bị lệch và lỗi.

 3. LUẬT SẮT BẮT BUỘC (DUY NHẤT):
    - Kiểu dữ liệu bạn muốn lấy phải là DUY NHẤT trong Tuple đó.
    - Nếu có 2 kiểu giống nhau (VD: tuple<int, int, string>), gọi std::get<int>
      sẽ gây LỖI BIÊN DỊCH vì C++ không biết bạn muốn lấy số int nào!
*/

// ==========================================
// CLASS & STRUCT PHỤ TRỢ CHO PHẦN CHUYÊN GIA
// ==========================================
// "Strong Types" (Kiểu dữ liệu mạnh) dùng để giải quyết vấn đề trùng lặp kiểu
struct Tuoi {
  int gia_tri;
};
struct Diem {
  int gia_tri;
};

// ==========================================
// 1. MỨC CƠ BẢN: TRUY XUẤT VÀ THAY ĐỔI
// ==========================================
void tuple_get_by_type_co_ban() {
  std::cout << "\n--- 1. BAZIC: TRUY XUAT TUPLE BANG KIEU DU LIEU ---\n";

  // Khởi tạo Tuple với 3 kiểu HOÀN TOÀN KHÁC NHAU
  std::tuple<int, double, std::string> thong_tin = {101, 8.5, "Le Van Nhan"};

  // TRUY XUẤT (ĐỌC)
  // Thay vì nhớ tên "Le Van Nhan" nằm ở vị trí số 2, ta chỉ cần gọi kiểu
  // std::string
  std::cout << "Ten: " << std::get<std::string>(thong_tin) << "\n";
  std::cout << "ID : " << std::get<int>(thong_tin) << "\n";
  std::cout << "GPA: " << std::get<double>(thong_tin) << "\n";

  // THAY ĐỔI (GHI)
  // Hàm std::get trả về Tham chiếu (&), nên ta có thể gán giá trị mới trực tiếp
  std::get<double>(thong_tin) = 9.8;
  std::get<std::string>(thong_tin) = "Le Van Nhan (Da update)";

  std::cout << "GPA moi: " << std::get<double>(thong_tin) << "\n";
  std::cout << "Ten moi: " << std::get<std::string>(thong_tin) << "\n";
}

// ==========================================
// 2. MỨC TRUNG CẤP: ÁP DỤNG CHO STD::PAIR
// ==========================================
void pair_get_by_type() {
  std::cout << "\n--- 2. INTERMEDIATE: DUNG CHO CA STD::PAIR ---\n";

  // std::pair thực chất là một dạng Tuple đặc biệt chỉ có 2 phần tử
  std::pair<int, std::string> san_pham = {500, "Dien Thoai"};

  // Ta hoàn toàn có thể dùng std::get<Type> cho Pair thay vì dùng .first và
  // .second
  std::cout << "San pham: " << std::get<std::string>(san_pham) << "\n";
  std::cout << "Gia ban : " << std::get<int>(san_pham) << "\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: CẠM BẪY TRÙNG KIỂU DỮ LIỆU
// ==========================================
void tuple_cam_bay_trung_kieu() {
  std::cout << "\n--- 3. ADVANCED: CAM BAY KHI TRUNG KIEU DU LIEU ---\n";

  // Khai báo Tuple có 2 kiểu "int"
  std::tuple<int, int, std::string> toa_do = {10, 20, "Diem A"};

  std::cout << "Ten toa do: " << std::get<std::string>(toa_do)
            << "\n"; // Hợp lệ vì string là duy nhất

  // NẾU BỎ COMMENT DÒNG DƯỚI, CHƯƠNG TRÌNH SẼ BÁO LỖI BIÊN DỊCH LẬP TỨC!
  // std::cout << std::get<int>(toa_do);
  // -> Lỗi: static_assert failed "type can only occur once in type list"

  // Cách duy nhất để lấy 10 hoặc 20 lúc này là quay về dùng Index truyền thống:
  std::cout << "Toa do X: " << std::get<0>(toa_do) << "\n";
  std::cout << "Toa do Y: " << std::get<1>(toa_do) << "\n";
}

// ==========================================
// 4. MỨC CHUYÊN GIA: KỸ THUẬT STRONG TYPING
// ==========================================
void tuple_strong_typing() {
  std::cout << "\n--- 4. EXPERT: STRONG TYPING (BOC KIEU) ---\n";

  // VẤN ĐỀ: Ta có Tuổi (int) và Điểm (int). Nếu để chung vào Tuple sẽ bị trùng
  // kiểu (Lỗi ở phần 3). Xấu: std::tuple<int, int, std::string> -> Phải dùng
  // std::get<0>, std::get<1>.

  // GIẢI PHÁP: Bọc int vào các Struct trống (Strong Types) tên là Tuoi và Diem.
  // Lúc này, các kiểu dữ liệu trở nên ĐỘC NHẤT.

  std::tuple<Tuoi, Diem, std::string> ho_so = {{25}, {9}, "Christopher"};

  // Bây giờ, ta có thể truy xuất rạch ròi bằng std::get<Type> mà không lo lỗi
  // biên dịch! Trình biên dịch C++ sẽ tối ưu hóa (Optimize) các struct này
  // khiến chi phí RAM/CPU bằng 0.

  std::cout << "Ho ten: " << std::get<std::string>(ho_so) << "\n";
  std::cout << "Tuoi  : " << std::get<Tuoi>(ho_so).gia_tri << "\n";
  std::cout << "Diem  : " << std::get<Diem>(ho_so).gia_tri << "\n";
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  tuple_get_by_type_co_ban();
  pair_get_by_type();
  tuple_cam_bay_trung_kieu();
  tuple_strong_typing();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}