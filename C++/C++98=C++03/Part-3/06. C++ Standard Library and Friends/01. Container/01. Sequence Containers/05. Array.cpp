#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <string>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Sứ mệnh: std::array sinh ra để tiêu diệt mảng tĩnh kiểu C cổ điển (VD: int
 arr[10];).
 2. Bản chất: Nó là một lớp bọc (thin wrapper) bọc quanh mảng C.
    Chi phí (Overhead) bằng 0. Nó KHÔNG lưu thêm bất kỳ thông tin nào ngoài
 chính dữ liệu của nó.
 3. Bộ nhớ: Lưu trực tiếp trên STACK (siêu tốc độ), KHÔNG dùng HEAP (cấp phát
 động) như Vector.
 4. Hạn chế: Kích thước phải là HẰNG SỐ được biết ngay lúc biên dịch
 (Compile-time). Không thể thay đổi kích thước sau khi đã tạo.
*/

// ==========================================
// 1. MỨC CƠ BẢN: KHỞI TẠO VÀ TRUY XUẤT AN TOÀN
// ==========================================
void array_co_ban() {
  std::cout << "\n--- 1. BAZIC: KHOI TAO & TRUY XUAT ---\n";

  // Khởi tạo: Cần 2 tham số Template <Kiểu dữ liệu, Số lượng>
  std::array<int, 5> arr1 = {10, 20, 30, 40, 50};

  // Nếu không khởi tạo giá trị, nó sẽ chứa "rác" trên RAM (giống mảng C)
  std::array<int, 5> arr_rac;

  // Khởi tạo tất cả bằng 0
  std::array<int, 5> arr_khong = {};

  // Điểm mạnh số 1: Nó BIẾT kích thước của chính nó (Mảng C bị mất thông tin
  // này khi truyền vào hàm)
  std::cout << "Kich thuoc mang: " << arr1.size()
            << "\n"; // Luôn là O(1), thậm chí là Compile-time

  // Truy xuất
  std::cout << "Phan tu [0] (Khong check tran vien): " << arr1[0] << "\n";

  // Điểm mạnh số 2: .at() giúp chống lỗi tràn viền (Buffer Overflow - Lỗi bảo
  // mật kinh điển)
  try {
    std::cout << "Phan tu thu 100: " << arr1.at(100) << "\n";
  } catch (const std::out_of_range &e) {
    std::cout << "Da chan dung loi tran vien: " << e.what() << "\n";
  }

  std::cout << "Duyet mang: ";
  for (int val : arr1)
    std::cout << val << " ";
  std::cout << "\n";
}

// ==========================================
// 2. MỨC TRUNG CẤP: BỘ NHỚ STACK & TƯƠNG THÍCH C-API
// ==========================================
// Hàm API kiểu C cổ điển chỉ nhận con trỏ và kích thước
void ham_c_cu(int *mang, size_t kich_thuoc) {
  std::cout << "  [C-API] Da nhan mang " << kich_thuoc << " phan tu.\n";
}

void array_bo_nho() {
  std::cout << "\n--- 2. INTERMEDIATE: MEMORY & C-API ---\n";

  std::array<int, 3> arr = {1, 2, 3};

  // Chứng minh không có Overhead: Kích thước của std::array<int, 3> bằng đúng 3
  // * 4 bytes = 12 bytes
  std::cout << "Kich thuoc RAM (sizeof): " << sizeof(arr) << " bytes\n";

  // Tương thích ngược: Làm sao đưa std::array vào các hàm thư viện C cũ?
  // Dùng .data() để lấy ra con trỏ nguyên thủy trỏ tới phần tử đầu tiên.
  ham_c_cu(arr.data(), arr.size());

  // Điểm mạnh số 3: Hỗ trợ phép GÁN (Mảng C không cho phép arr1 = arr2)
  std::array<int, 3> arr_copy;
  arr_copy = arr; // Copy toàn bộ dữ liệu cái vèo!
  std::cout << "Phan tu [0] cua ban copy: " << arr_copy[0] << "\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: THUẬT TOÁN (ALGORITHMS)
// ==========================================
void array_thuat_toan() {
  std::cout << "\n--- 3. ADVANCED: ALGORITHMS ---\n";

  std::array<int, 6> arr = {5, 2, 8, 1, 9, 3};

  // Nhờ có bộ Iterators (begin, end), std::array xài được MỌI thuật toán STL
  std::sort(arr.begin(), arr.end());

  std::cout << "Sau khi sort: ";
  for (int x : arr)
    std::cout << x << " ";
  std::cout << "\n";

  // Phủ toàn bộ mảng bằng 1 giá trị cực nhanh
  arr.fill(99);

  // Tính tổng các phần tử (Dùng <numeric>)
  int tong = std::accumulate(arr.begin(), arr.end(), 0);
  std::cout << "Tong 6 so 99 la: " << tong << "\n";
}

// ==========================================
// 4. MỨC CHUYÊN GIA: MODERN C++ (C++17 & C++20)
// ==========================================
// Khởi tạo tính toán ngay lúc biên dịch (Compile-time)
// Toàn bộ mảng này được tạo ra trước cả khi file .exe được chạy!
constexpr std::array<int, 5> tao_mang_compile_time() {
  std::array<int, 5> a = {1, 2, 3, 4, 5};
  for (int i = 0; i < 5; ++i)
    a[i] *= 10;
  return a;
}

void array_chuyen_gia_modern_cpp() {
  std::cout << "\n--- 4. EXPERT: MODERN C++ (CTAD & STRUCTURED BINDING) ---\n";

  // 1. C++17 CTAD (Class Template Argument Deduction):
  // Không cần viết <int, 3> mệt mỏi nữa, Trình biên dịch tự đoán!
  std::array arr_tu_doan = {1.1, 2.2, 3.3}; // Tự hiểu là std::array<double, 3>

  // 2. C++17 Structured Bindings (Trói buộc cấu trúc):
  // Bung mảng ra thành các biến riêng lẻ một cách thanh lịch
  std::array diem_toa_do = {10, 20, 30};
  auto [x, y, z] = diem_toa_do;
  std::cout << "Toa do tháo ròi: x=" << x << ", y=" << y << ", z=" << z << "\n";

  // 3. constexpr (Tối ưu hóa sức mạnh phần cứng)
  constexpr auto mang_sieutoc = tao_mang_compile_time();
  std::cout << "Mang Compile-time index 2: " << mang_sieutoc[2]
            << "\n"; // In ra 30
}

// ==========================================
// 5. MẢNG ĐA CHIỀU (MATRIX) BẰNG STD::ARRAY
// ==========================================
void array_2d() {
  std::cout << "\n--- 5. MATRIX: STD::ARRAY 2D ---\n";

  // Cú pháp hơi lồng ghép: Mảng 3 phần tử, mỗi phần tử là 1 mảng 4 phần tử int
  std::array<std::array<int, 4>, 3> ma_tran = {
      {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}}};

  std::cout << "Phan tu Hang 2, Cot 3: " << ma_tran[1][2] << "\n"; // In ra 7
}

// ==========================================
// TỔNG KẾT LUẬT LỰA CHỌN (Kinh nghiệm thực chiến)
// ==========================================
/*
 1. BAO GIỜ THÌ DÙNG std::array?
    - Khi bạn BIẾT TRƯỚC VÀ CHẮC CHẮN số lượng phần tử không bao giờ thay đổi.
    - Cần tốc độ tối đa (Stack luôn nhanh hơn Heap).
    - Ví dụ: Vector toán học (Toạ độ XYZ), Ma trận biến đổi trong Game 3D,
      Mảng chứa 12 tháng trong năm, Bảng băm cố định.

 2. CẠM BẪY CHẾT NGƯỜI (STACK OVERFLOW):
    - Vì nó lưu trên Stack (dung lượng rất nhỏ, thường chỉ 1MB - 8MB tùy HĐH).
    - Nếu bạn khai báo: std::array<int, 1000000> arr; -> CHƯƠNG TRÌNH SẼ CRASH
 NGAY LẬP TỨC!
    - Khi dữ liệu lớn hoặc không biết trước kích thước -> BẮT BUỘC quay về dùng
 std::vector.
*/

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  array_co_ban();
  array_bo_nho();
  array_thuat_toan();
  array_chuyen_gia_modern_cpp();
  array_2d();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}