#include <charconv>
#include <expected> // C++23: std::expected
#include <mdspan>   // C++23: std::mdspan
#include <optional> // C++23: Monadic optional
#include <print>    // C++23: std::print, std::println
#include <stdfloat> // C++23: Standard floating-point types
#include <string>
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 C++23 mang đến một cuộc cách mạng về tính thực dụng, vay mượn rất nhiều triết
 lý an toàn từ Rust và sức mạnh tính toán từ Data Science.

 ĐỂ CHẠY ĐƯỢC CODE NÀY:
 Bạn cần Trình biên dịch hỗ trợ C++23 (GCC 14+, Clang 18+, MSVC 19.38+)
 và bật cờ biên dịch: -std=c++23
*/

// ==========================================
// 1. STD::PRINT VÀ STD::PRINTLN (Kẻ Hủy Diệt iostream)
// ==========================================
void cpp23_print_va_println() {
  // Ngày xưa: std::cout << "Ten: " << name << ", Tuoi: " << age << "\n";
  // Nhược điểm: Chậm, cồng kềnh, biên dịch lâu, dễ lỗi định dạng, khó căn lề.

  std::println("\n--- 1. STD::PRINTLN ---");

  std::string ten = "C++23";
  int rank = 1;

  // Cơ bản: Thay thế placeholder {}
  std::println("Hello {}, ban dung thu {}", ten, rank);

  // Nâng cao: Căn lề và định dạng (Giống hệt Python f-string hoặc std::format)
  double pi = 3.14159265;
  std::println("Pi lam tron 2 chu so: {:.2f}", pi);

  // Căn giữa, căn phải, thêm ký tự đệm (Padding)
  std::println("ID: {:0>5}", 42);             // In ra: ID: 00042
  std::println("Lề trái:  |{:<10}|", "Trai"); // In ra: |Trai      |
  std::println("Căn giữa: |{:^10}|", "Giua"); // In ra: |   Giua   |
}

// ==========================================
// 2. STD::EXPECTED (Xử Lý Lỗi Kiểu Functional / Rust)
// ==========================================
/*
 std::expected<T, E> chứa:
 - Một KẾT QUẢ kiểu T (nếu thành công)
 - HOẶC một LỖI kiểu E (nếu thất bại).
 Tuyệt đối không ném Exception (throw), giúp chương trình chạy nhanh hơn rất
 nhiều trong các hệ thống nhúng, game, và real-time.
*/

// Hàm chia hai số, trả về Kết quả (double) hoặc Lỗi (std::string)
std::expected<double, std::string> chia_hai_so(double a, double b) {
  if (b == 0.0) {
    // Trả về lỗi bằng cách bọc trong std::unexpected
    return std::unexpected("Loi: Khong the chia cho 0!");
  }
  return a / b; // Trả về kết quả bình thường
}

void cpp23_expected() {
  std::println("\n--- 2. STD::EXPECTED ---");

  auto ket_qua_dung = chia_hai_so(10.0, 2.0);
  auto ket_qua_sai = chia_hai_so(10.0, 0.0);

  // BẮT BUỘC người lập trình phải kiểm tra (has_value) trước khi lấy dữ liệu
  if (ket_qua_dung.has_value()) {
    std::println("Phep tinh dung: {}", ket_qua_dung.value());
  }

  if (!ket_qua_sai.has_value()) {
    std::println("Phep tinh sai da bi bat: {}", ket_qua_sai.error());
  }

  // NÂNG CAO: Dùng Monadic Operations với expected (and_then, transform)
  // Cực kỳ mạnh khi bạn muốn "Chain" (Nối) nhiều hàm dễ sinh lỗi lại với nhau.
  auto chuoi_ket_qua = chia_hai_so(20.0, 2.0).transform(
      [](double res) { return res * 10; }); // Nếu phép chia đúng, nhân 10 lên

  std::println("Gia tri sau khi transform: {}", chuoi_ket_qua.value_or(-1));
}

// ==========================================
// 3. STD::MDSPAN (Kính Lúp Đa Chiều Không Tốn RAM)
// ==========================================
void cpp23_mdspan() {
  std::println("\n--- 3. STD::MDSPAN ---");

  // Vấn đề: Trong Data Science/AI, dữ liệu thường được lưu ở mảng 1 chiều (1D)
  // để tối ưu CPU Cache. Nhưng thuật toán lại cần tư duy theo Ma trận (2D) hoặc
  // Tensor (3D).
  std::vector<int> du_lieu_1D = {1, 2, 3, 4, 5, 6}; // Mảng phẳng 6 phần tử

  // Giải pháp mdspan: "Đeo kính lúp" để nhìn mảng 1D này như một Ma trận 2x3
  // std::extents chỉ định kích thước các chiều (2 hàng, 3 cột)
  std::mdspan ma_tran_2d(du_lieu_1D.data(), std::extents<int, 2, 3>{});

  // Truy cập theo cú pháp đa chiều: ma_tran_2d[row, col] (Tính năng đa tham số
  // toán tử [] từ C++23)
  std::println("Phan tu tai hang 1, cot 2 la: {}",
               ma_tran_2d[1, 2]); // Tương đương du_lieu_1D[5] = 6

  std::println("Duyet ma tran 2x3 bang mdspan:");
  for (size_t r = 0; r < ma_tran_2d.extent(0); ++r) {
    for (size_t c = 0; c < ma_tran_2d.extent(1); ++c) {
      std::print("{} ", ma_tran_2d[r, c]);
    }
    std::println("");
  }
  // Lợi ích tối thượng: Không hề copy dữ liệu. mdspan chỉ là một lớp tính toán
  // Index ảo siêu nhẹ!
}

// ==========================================
// 4. HÀM MONADIC CHO STD::OPTIONAL
// ==========================================
// Đọc chuỗi ra số, có thể thất bại nên trả về optional
std::optional<int> phan_tich_so(std::string_view str) {
  int gia_tri;
  auto [ptr, ec] =
      std::from_chars(str.data(), str.data() + str.size(), gia_tri);
  if (ec == std::errc())
    return gia_tri;
  return std::nullopt;
}

// Tính căn bậc 2, số âm sẽ thất bại
std::optional<double> can_bac_hai(int x) {
  if (x < 0)
    return std::nullopt;
  return std::sqrt(x);
}

void cpp23_monadic_optional() {
  std::println("\n--- 4. MONADIC OPTIONAL ---");

  // Ngày xưa (if-else lồng nhau "Deep nesting"):
  /*
  auto opt1 = phan_tich_so("16");
  if (opt1.has_value()) {
      auto opt2 = can_bac_hai(opt1.value());
      if (opt2.has_value()) {
          std::println("Ket qua: {}", opt2.value());
      }
  }
  */

  // C++23 Monadic Operations (Pipeline thanh lịch không cần rẽ nhánh)
  auto ket_qua = phan_tich_so("16")
                     // and_then: Nếu bước trước thành công, truyền giá trị vào
                     // hàm trả về optional tiếp theo
                     .and_then(can_bac_hai)
                     // transform: Nếu bước trước thành công, biến đổi giá trị
                     // (hàm trả về giá trị thường)
                     .transform([](double x) { return x * 10; })
                     // or_else: Nếu BẤT KỲ bước nào ở trên thất bại (ra
                     // nullopt), thì cung cấp đường lui
                     .or_else([]() -> std::optional<double> { return -1.0; });

  std::println("Ket qua cua Pipeline: {}",
               ket_qua.value()); // In ra 40 (Căn của 16 là 4, x 10)

  // Thử nghiệm với chuỗi gây lỗi
  auto ket_qua_loi =
      phan_tich_so("-9")         // Số âm
          .and_then(can_bac_hai) // Thất bại ở đây, nhảy vọt qua transform
          .transform([](double x) { return x * 10; })
          .or_else([]() -> std::optional<double> {
            return -999.0;
          }); // Cứu vãn tại đây

  std::println("Ket qua cua Pipeline loi: {}", ket_qua_loi.value());
}

// ==========================================
// 5. STD::FLOAT (Các Kiểu Dấu Phẩy Động Chuẩn Xác)
// ==========================================
void cpp23_stdfloat() {
  std::println("\n--- 5. THU VIEN <stdfloat> ---");

  // Ngày xưa: C++ chỉ có float, double. Bạn không rõ float là bao nhiêu bit
  // (thường là 32, nhưng không bắt buộc bởi ngôn ngữ). Trong Machine Learning /
  // AI / Đồ họa, số lượng BIT quy định trực tiếp tốc độ và dung lượng VRAM.

#ifdef __STDCPP_FLOAT16_T__
  // 16-bit float (Bán độ chính xác - Half precision)
  // Tiết kiệm một nửa RAM so với float truyền thống, siêu quan trọng cho việc
  // huấn luyện AI.
  std::float16_t f16 = 3.14f16;
  std::println("Ho tro float16_t!");
#endif

#ifdef __STDCPP_BFLOAT16_T__
  // Brain Floating Point (Thiết kế riêng bởi Google cho AI và Deep Learning)
  // Dải mũ lớn như float32 nhưng cắt bớt độ chính xác thập phân.
  std::bfloat16_t bf16 = 2.71bf16;
  std::println("Ho tro bfloat16_t!");
#endif

#ifdef __STDCPP_FLOAT32_T__
  // Float 32-bit CHUẨN XÁC, không phụ thuộc kiến trúc CPU
  std::float32_t f32 = 1.0f32;
  std::println("Ho tro float32_t (32-bit float chuan xac).");
#endif

#ifdef __STDCPP_FLOAT64_T__
  // Float 64-bit (Tương đương double tiêu chuẩn)
  std::float64_t f64 = 1.0f64;
#endif

  std::println("(Luu y: Các kieu stdfloat phu thuoc vao viec phan cung CPU/GPU "
               "co ho tro hay khong)");
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::println("======== C++23 BREAKTHROUGH FEATURES ========");

  cpp23_print_va_println();
  cpp23_expected();
  cpp23_mdspan();
  cpp23_monadic_optional();
  cpp23_stdfloat();

  std::println("\n======== HOAN THANH ========");
  return 0;
}