#include <algorithm> // Cho std::ranges
#include <array>
#include <iostream>
#include <numeric> // Cho std::accumulate
#include <span>    // Bắt buộc cho std::span
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. std::span là gì?
    - Nó là một "Góc nhìn" (View) nhẹ gọn, đại diện cho một dải bộ nhớ liên tục.
    - Bản chất dưới nền: std::span chỉ chứa đúng 2 thứ: 1 Con trỏ (trỏ tới phần
 tử đầu) và 1 Biến lưu kích thước (độ dài).

 2. Tại sao nên dùng std::span?
    - SIÊU NHẸ: Việc khởi tạo và copy span gần như miễn phí (chỉ copy 2 biến).
    - KHÔNG SỞ HỮU: Span KHÔNG TỰ QUẢN LÝ RAM. Nó chỉ "nhìn trộm" dữ liệu của
 Vector hoặc Mảng khác. (Khác với vector, nếu vector bị hủy, span sẽ thành con
 trỏ lơ lửng).
    - TIỆN LỢI & AN TOÀN: Thay thế hoàn toàn cách truyền tham số cổ điển (int*
 arr, size_t size) bằng một cú pháp gọn gàng, an toàn.
*/

// ==========================================
// HÀM PHỤ TRỢ ĐỂ DEMO (ỨNG DỤNG LỚN NHẤT CỦA SPAN)
// ==========================================
// TRƯỚC C++20: Bạn phải viết 3 hàm khác nhau cho Vector, Array, và Mảng C
// void in_mang_c(int* arr, size_t size);
// void in_vector(const std::vector<int>& v);
// void in_std_array(const std::array<int, 5>& a);

// TỪ C++20: BẠN CHỈ CẦN VIẾT ĐÚNG 1 HÀM NÀY BẰNG STD::SPAN!
// (Dùng std::span<const int> để đảm bảo hàm chỉ Đọc, không sửa dữ liệu gốc)
void in_du_lieu_tu_bat_ky_dau(std::span<const int> s, const std::string &msg) {
  std::cout << msg << ": ";
  for (int el : s) {
    std::cout << el << " ";
  }
  std::cout << "\n";
}

// ==========================================
// 1. MỨC CƠ BẢN: LIÊN KẾT VỚI VECTOR & CHỈNH SỬA DỮ LIỆU
// ==========================================
void span_co_ban() {
  std::cout << "\n--- 1. BAZIC: VECTOR & MODIFYING ---\n";

  std::vector<int> v = {1, 2, 3, 4, 5};

  // Tạo một span tham chiếu thẳng vào dữ liệu của Vector
  std::span<int> myintspan = v;

  // Vì span chỉ "nhìn" vào vector, nên sửa span LÀ SỬA TRỰC TIẾP VECTOR GỐC
  myintspan[2] = 256;

  std::cout << "Vector sau khi bi sua thong qua Span: \n";
  for (auto el : v) {
    std::cout << "  " << el << "\n";
  }
}

// ==========================================
// 2. MỨC TRUNG CẤP: DYNAMIC EXTENT vs STATIC EXTENT
// ==========================================
void span_extents() {
  std::cout << "\n--- 2. INTERMEDIATE: DYNAMIC vs STATIC EXTENT ---\n";

  // a. Dynamic Extent (Kích thước động - Không biết trước lúc biên dịch)
  // Mặc định std::span<T> là Dynamic Extent. Kích thước của nó được xác định
  // lúc chạy (Runtime).
  std::vector<int> vec = {10, 20, 30};
  std::span<int> span_dong = vec;
  std::cout << "Dynamic Span size: " << span_dong.size() << "\n";

  // b. Static Extent (Kích thước tĩnh - Chốt cứng ngay lúc biên dịch)
  // Cú pháp: std::span<T, N>. Giúp trình biên dịch tối ưu hóa cực mạnh, giảm
  // bớt biến lưu kích thước.
  int arr_c[] = {1, 2, 3, 4, 5};

  // Ràng buộc chặt chẽ: Span này CHỈ ĐƯỢC PHÉP liên kết với mảng có ĐÚNG 5 phần
  // tử.
  std::span<int, 5> span_tinh = arr_c;
  span_tinh[4] = 99; // Sửa dữ liệu mảng C gốc thành 99

  std::cout << "Mang C sau khi sua bang Static Span: ";
  for (auto el : arr_c) {
    std::cout << el << " ";
  }
  std::cout << "\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: CẮT MẢNG (SUBSPAN) O(1)
// ==========================================
void span_cung_cap_subspan() {
  std::cout << "\n--- 3. ADVANCED: SUBSPANS (CAT MANG SIEU NHANH) ---\n";

  std::vector<int> v = {10, 20, 30, 40, 50, 60, 70, 80};
  std::span<int> tong_span = v;

  // Ngày xưa nếu muốn lấy một phần mảng, bạn phải Copy chúng ra Vector mới.
  // Với Span, bạn dùng Subspan để tạo ra các "Góc nhìn" nhỏ hơn mà KHÔNG TỐN 1
  // BYTE COPY NÀO!

  // 1. Lấy 3 phần tử đầu tiên
  std::span<int> ba_phan_tu_dau = tong_span.first(3);
  in_du_lieu_tu_bat_ky_dau(ba_phan_tu_dau, "first(3)");

  // 2. Lấy 2 phần tử cuối cùng
  std::span<int> hai_phan_tu_cuoi = tong_span.last(2);
  in_du_lieu_tu_bat_ky_dau(hai_phan_tu_cuoi, "last(2)");

  // 3. Cắt ở giữa: Lấy từ index 2, độ dài 4 phần tử
  std::span<int> khuc_giua = tong_span.subspan(2, 4);
  in_du_lieu_tu_bat_ky_dau(khuc_giua, "subspan(2, 4)");
}

// ==========================================
// 4. MỨC CHUYÊN GIA: TRUYỀN THAM SỐ VÀ ÉP KIỂU BYTE
// ==========================================
void span_chuyen_gia_functions() {
  std::cout << "\n--- 4. EXPERT: FUNCTION PARAMETERS & BYTES ---\n";

  // 1. CHỨNG MINH TÍNH ĐA HÌNH CỦA SPAN KHI TRUYỀN HÀM
  std::vector<int> vec = {1, 2, 3};
  std::array<int, 3> arr_std = {4, 5, 6};
  int arr_c[] = {7, 8, 9};

  // Hàm in_du_lieu_tu_bat_ky_dau() chấp nhận MỌI THỨ! Không cần viết Overload.
  in_du_lieu_tu_bat_ky_dau(vec, "Tu std::vector");
  in_du_lieu_tu_bat_ky_dau(arr_std, "Tu std::array");
  in_du_lieu_tu_bat_ky_dau(arr_c, "Tu mang C (C-array)");

  // 2. STD::AS_BYTES - ĐỌC VÙNG NHỚ Ở MỨC ĐỘ THẤP NHẤT
  // Dùng để xem một biến số nguyên tốn bao nhiêu byte RAM, hoặc để đẩy dữ liệu
  // qua Mạng lưới (Network socket).
  float so_thuc = 3.14f;

  // Tạo 1 span có kích thước 1 phần tử trỏ vào biến so_thuc
  std::span<float, 1> span_so_thuc(&so_thuc, 1);

  // Ép sang hệ Bytes (Mỗi phần tử giờ là 1 byte, float sẽ biến thành dải 4
  // bytes)
  std::span<const std::byte> byte_span = std::as_bytes(span_so_thuc);

  std::cout << "\nDoc 3.14f duoi dang Hexa Bytes: ";
  for (std::byte b : byte_span) {
    // Ép kiểu về int để in dạng Hex
    std::cout << std::hex << (int)b << " ";
  }
  std::cout << std::dec << "\n"; // Trả lại chế độ in thập phân
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== KHOA HOC C++20 STD::SPAN =====\n";

  span_co_ban();
  span_extents();
  span_cung_cap_subspan();
  span_chuyen_gia_functions();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}