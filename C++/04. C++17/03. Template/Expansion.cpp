#include <iostream>
#include <string>
#include <type_traits> // Bắt buộc để dùng các traits như std::is_pointer_v
#include <vector>


// =====================================================================
// PHẦN 1: IF CONSTEXPR - KẺ HỦY DIỆT SFINAE (COMPILE-TIME BRANCHING)
// =====================================================================
/*
 LÝ THUYẾT:
 - Lệnh 'if' bình thường: Trình biên dịch (Compiler) kiểm tra cú pháp và dịch CẢ
 HAI nhánh (true và false) ra mã máy. Lúc chạy (Runtime) mới quyết định đi vào
 nhánh nào.
 - Lệnh 'if constexpr': Trình biên dịch sẽ tính toán điều kiện NGAY LÚC BIÊN
 DỊCH. Nhánh nào sai (false) -> COMPILER XÓA SẠCH NÓ KHỎI MÃ MÁY.
   -> Nhờ vậy, nhánh sai chứa code vô lý (ví dụ: lấy .length() của số nguyên)
 cũng KHÔNG BỊ LỖI.
*/

// --- MỨC CƠ BẢN & TRUNG CẤP: Thay thế Nạp chồng hàm (Overloading) ---
template <typename T> void in_do_dai(const T &t) {
  std::cout << "\n[if constexpr] Xu ly kieu du lieu: " << typeid(T).name()
            << " -> ";

  // std::is_pointer_v<T> sẽ trả về true nếu T là con trỏ, false nếu không phải.
  if constexpr (std::is_pointer_v<T>) {
    std::cout << "Day la con tro, gia tri tro toi la: " << *t;
  } else if constexpr (std::is_integral_v<T>) {
    std::cout << "Day la so nguyen, khong co do dai: " << t;
  } else {
    // MA THUẬT NẰM Ở ĐÂY:
    // Nếu T là số nguyên (int), dòng code 't.length()' thực chất là sai cú
    // pháp. Nhưng nhờ 'if constexpr', compiler thấy T là 'int' (rơi vào nhánh
    // trên), nên nó VỨT BỎ HOÀN TOÀN nhánh 'else' này trước khi kiểm tra lỗi cú
    // pháp!
    std::cout << "Do dai (length) la: " << t.length();
  }
}

// --- MỨC CHUYÊN GIA: Thay thế C++11 SFINAE (enable_if) ---
// Vấn đề: Viết 1 hàm 'lay_gia_tri', nếu truyền con trỏ thì tự giải tham chiếu
// (*t), nếu truyền biến thường thì trả về biến đó. Nhờ if constexpr, ta có thể
// dùng từ khóa 'auto' trả về kiểu dữ liệu động cực kỳ thanh lịch.
template <typename T> auto lay_gia_tri(T t) {
  if constexpr (std::is_pointer_v<T>) {
    return *t; // Nếu là con trỏ, trả về giá trị nó trỏ tới
  } else {
    return t; // Nếu là biến thường, trả về chính nó
  }
}

// =====================================================================
// PHẦN 2: FOLD EXPRESSIONS - BIỂU THỨC GẬP (VARIADIC TEMPLATES)
// =====================================================================
/*
 LÝ THUYẾT:
 - Trươc C++17, để xử lý một hàm nhận VÔ SỐ tham số (Args... args), ta phải viết
   hàm Đệ quy (Recursive) cực kỳ rườm rà (1 hàm gốc + 1 hàm dừng đệ quy).
 - C++17 Fold Expressions cho phép áp dụng 1 toán tử (như +, -, *, <<, ,) lên
   TOÀN BỘ gói tham số chỉ bằng đúng 1 dòng code.
*/

// --- 1. MỨC CƠ BẢN: UNARY FOLD (Gập đơn phân) ---
// Cú pháp: (args op ...) -> (arg1 + (arg2 + arg3))
template <typename... Args> auto tinh_tong_tat_ca(Args... args) {
  return (args + ...); // Cộng tất cả lại với nhau
}

// --- 2. MỨC TRUNG CẤP: BINARY FOLD (Gập nhị phân - Có giá trị khởi tạo) ---
// Cú pháp: (init op ... op args) -> (100 + arg1 + arg2 + arg3)
template <typename... Args>
auto tinh_tong_co_khoi_tao(int khoi_tao, Args... args) {
  // Nếu gọi hàm không có args nào, nó sẽ trả về 'khoi_tao'
  return (khoi_tao + ... + args);
}

// --- 3. MỨC NÂNG CAO: FOLD VỚI TOÁN TỬ LOGIC (&&, ||) ---
// Kiểm tra xem TẤT CẢ các tham số truyền vào có phải số chẵn không
template <typename... Args> bool kiem_tra_toan_so_chan(Args... args) {
  // Sẽ gập thành: ((arg1 % 2 == 0) && (arg2 % 2 == 0) && ...)
  return ((args % 2 == 0) && ...);
}

// --- 4. MỨC CHUYÊN GIA: FOLD KẾT HỢP TOÁN TỬ PHẨY (COMMA OPERATOR) ---
// Vấn đề: Làm sao để cout (in ra) tất cả các tham số mà không dùng đệ quy?
// Giải pháp: Dùng toán tử phẩy ',' để thực thi liên tiếp các biểu thức.
template <typename... Args> void in_tat_ca(Args... args) {
  std::cout << "\n[Fold Comma] In danh sach: ";

  // Cú pháp: ( (Thực thi lệnh với args), ... )
  // Nó sẽ bung ra thành:
  // (cout << arg1 << " "), (cout << arg2 << " "), (cout << arg3 << " ");
  ((std::cout << args << " "), ...);

  std::cout << "\n";
}

// Gọi một hàm cụ thể lên toàn bộ các tham số (Push vào vector)
template <typename T, typename... Args>
void dua_vao_vector(std::vector<T> &vec, Args... args) {
  // Lặp qua từng arg, gọi vec.push_back(arg)
  (vec.push_back(args), ...);
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH (THỰC THI & CHỨNG MINH)
// ==========================================
int main() {
  std::cout << "====== TEST IF CONSTEXPR ======\n";

  std::string ten = "Le Van Nhan";
  int tuoi = 25;
  int *ptr_tuoi = &tuoi;

  // 1. Thử if constexpr với hàm in_do_dai
  in_do_dai(ten); // Biên dịch nhánh else (.length())
  in_do_dai(tuoi); // Biên dịch nhánh else if (Tự động bỏ qua lỗi .length())
  in_do_dai(ptr_tuoi); // Biên dịch nhánh if (*t)

  // 2. Thử if constexpr với hàm trả về linh hoạt
  std::cout << "\n\nLay gia tri tu bien thuong: " << lay_gia_tri(tuoi);
  std::cout << "\nLay gia tri tu con tro: " << lay_gia_tri(ptr_tuoi);

  std::cout << "\n\n====== TEST FOLD EXPRESSIONS ======\n";

  // 3. Thử Unary & Binary Fold
  std::cout << "Tong (1, 2.5, 3, 4.5): " << tinh_tong_tat_ca(1, 2.5, 3, 4.5)
            << "\n";
  std::cout << "Tong khoi tao 100 + (1, 2, 3): "
            << tinh_tong_co_khoi_tao(100, 1, 2, 3) << "\n";

  // 4. Thử Fold với Logic
  bool check1 = kiem_tra_toan_so_chan(2, 4, 6, 8); // true
  bool check2 = kiem_tra_toan_so_chan(2, 4, 5, 8); // false
  std::cout << "Kiem tra toan chan (2, 4, 6, 8): "
            << (check1 ? "True" : "False") << "\n";
  std::cout << "Kiem tra toan chan (2, 4, 5, 8): "
            << (check2 ? "True" : "False") << "\n";

  // 5. Thử Fold với Comma Operator (In tất cả)
  in_tat_ca("Hello", "C++17", 999, 3.14, 'A');

  // 6. Thử Push nhiều phần tử vào Vector bằng 1 dòng
  std::vector<int> ds;
  dua_vao_vector(ds, 10, 20, 30, 40, 50);

  std::cout << "[Fold Vector Push] Cac phan tu: ";
  for (int x : ds)
    std::cout << x << " ";
  std::cout << "\n";

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}