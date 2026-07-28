#include <array>
#include <iostream>
#include <string_view>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY) - C++20 CHUẨN MỚI
// ==========================================
/*
 1. consteval (Hàm tức thời - Immediate Functions):
    - VẤN ĐỀ CỦA C++11/14: Từ khóa `constexpr` yêu cầu hàm "có thể" chạy lúc
 biên dịch. Nhưng nếu bạn truyền vào một biến bình thường, nó sẽ LÉN LÚT hạ cấp
 xuống chạy lúc Runtime!
    - GIẢI PHÁP C++20: `consteval` ra đời. Nó mang tính ĐỘC TÀI: "Bắt buộc 100%
 phải tính toán xong xuôi trước khi chương trình .exe được tạo ra. Nếu không tao
 báo lỗi biên dịch!".

 2. constinit (Khởi tạo hằng số cho biến toàn cục):
    - VẤN ĐỀ (Static Initialization Order Fiasco): Nếu bạn có 2 file .cpp, file
 A chứa biến toàn cục X, file B chứa biến toàn cục Y = X + 1. C++ KHÔNG ĐẢM BẢO
 X hay Y được khởi tạo trước lúc chương trình chạy (Runtime). Nếu Y khởi tạo
 trước khi X ra đời -> Giá trị rác -> Bug "Nhân phẩm" cực kỳ khó fix.
    - GIẢI PHÁP C++20: `constinit` ép biến toàn cục/static phải được khởi tạo
 bằng hằng số (Compile-time). Nhờ đó, khi Runtime bắt đầu, dữ liệu đã nằm sẵn ở
 đó một cách hoàn hảo, không có Fiasco nào cả! Lưu ý: Nó chỉ quản lý lúc KHỞI
 TẠO, sau đó bạn VẪN CÓ THỂ THAY ĐỔI giá trị biến đó (Khác với constexpr).
*/

// ==========================================
// 1. MỨC CƠ BẢN: SỰ KHÁC BIỆT GIỮA CONSTEXPR VÀ CONSTEVAL
// ==========================================

// Hàm constexpr (C++11): "Ba phải"
constexpr int binh_phuong_constexpr(int n) { return n * n; }

// Hàm consteval (C++20): "Độc tài"
consteval int binh_phuong_consteval(int n) { return n * n; }

void demo_consteval_co_ban() {
  std::cout << "\n--- 1. BAZIC: CONSTEVAL VS CONSTEXPR ---\n";

  int x = 5; // x là biến Runtime

  // constexpr chấp nhận biến Runtime -> Nó sẽ tính toán lúc CHẠY CHƯƠNG TRÌNH
  // (Bị chậm đi).
  int kq1 = binh_phuong_constexpr(x);
  std::cout << "Kq1 (Chay luc Runtime): " << kq1 << "\n";

  // consteval TUYỆT ĐỐI KHÔNG CHẤP NHẬN biến Runtime!
  // int kq2 = binh_phuong_consteval(x); // LỖI BIÊN DỊCH (TỪ CHỐI COMPILING)!!!

  // ĐỂ DÙNG CONSTEVAL: Tham số truyền vào phải là hằng số đã biết trước
  // (Compile-time)
  int kq3 = binh_phuong_consteval(
      10); // Hợp lệ. Con số 100 đã được tính xong từ trước khi bấm nút Run!
  std::cout << "Kq3 (Da tinh xong truoc khi chay): " << kq3 << "\n";
}

// ==========================================
// 2. MỨC TRUNG CẤP: CONSTINIT CHO BIẾN TOÀN CỤC/STATIC
// ==========================================

consteval int tinh_toan_phuc_tap() { return 100 * 2 + 50; }

// KHAI BÁO BIẾN GLOBAL VỚI CONSTINIT
// Đảm bảo con số 250 đã được nạp thẳng vào file .exe. Không cần đợi hàm main()
// chạy.
constinit int global_thuong = tinh_toan_phuc_tap();

// constinit int global_loi = x; // LỖI BIÊN DỊCH! Không thể khởi tạo bằng một
// biến Runtime.

void demo_constinit_co_ban() {
  std::cout
      << "\n--- 2. INTERMEDIATE: CONSTINIT (GLOBAL/STATIC VARIABLES) ---\n";

  // ĐIỂM ĂN TIỀN NHẤT CỦA CONSTINIT:
  // Khác với `constexpr` (khởi tạo xong là bị đóng băng thành const).
  // Biến `constinit` KHÔNG PHẢI LÀ CONST! Khởi tạo Compile-time an toàn xong,
  // xuống Runtime vẫn sửa thoải mái!

  std::cout << "Gia tri global ban dau: " << global_thuong << "\n";

  global_thuong = 999; // Hợp lệ!

  std::cout << "Gia tri global sau khi sua (Runtime): " << global_thuong
            << "\n";

  // Có thể dùng cho biến static bên trong hàm
  constinit static int static_dem = tinh_toan_phuc_tap();
  static_dem++;
  std::cout << "Bien static constinit: " << static_dem << "\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: DÙNG CONSTEVAL ĐỂ VALIDATE STRING LÚC BIÊN DỊCH
// ==========================================
// Ứng dụng thực tế: Báo lỗi ngay lúc Code thay vì chờ chương trình chạy mới
// văng lỗi.

consteval bool kiem_tra_mat_khau(std::string_view pass) {
  if (pass.length() < 6) {
    // Trong consteval, gọi throw sẽ ép Trình biên dịch dừng quá trình Build và
    // in ra lỗi này!
    throw "Loi Compile-time: Mat khau phai >= 6 ky tu!";
  }
  return true;
}

void demo_consteval_validate() {
  std::cout
      << "\n--- 3. ADVANCED: COMPILE-TIME VALIDATION BANG CONSTEVAL ---\n";

  // Trình biên dịch sẽ quét qua dòng này. Chiều dài = 8 (OK). Biên dịch thành
  // công.
  constexpr bool check1 = kiem_tra_mat_khau("Abc12345");

  // constexpr bool check2 = kiem_tra_mat_khau("123");
  // ^ BỎ COMMENT DÒNG TRÊN SẼ LÀM NỔ TRÌNH BIÊN DỊCH (LỖI FATAL ERROR)!
  // Giúp chặn đứng các thiết lập sai trái từ trong trứng nước.

  if (check1)
    std::cout << "Mat khau 'Abc12345' hop le (Da check luc Build).\n";
}

// ==========================================
// 4. MỨC CHUYÊN GIA: KẾT HỢP CONSTEVAL, CONSTINIT VÀ CONSTEXPR
// ==========================================

// Struct tạo mảng dữ liệu ngay lúc biên dịch
struct BangDuLieu {
  std::array<int, 5> data;

  // Constructor dùng consteval để ép buộc bảng này chỉ được tạo lúc biên dịch
  consteval BangDuLieu() : data{0} {
    for (int i = 0; i < 5; ++i) {
      data[i] = i * 10;
    }
  }
};

// 1. Dùng constinit: Muốn giữ mảng trong RAM an toàn, và có thể sửa đổi sau này
constinit BangDuLieu bang_sua_duoc;

// 2. Dùng constexpr: Muốn mảng này là HẰNG SỐ vĩnh viễn (Read-only)
constexpr BangDuLieu bang_co_dinh;

void demo_chuyen_gia_ket_hop() {
  std::cout << "\n--- 4. EXPERT: KET HOP 3 TU KHOA QUYEN LUC NHAT ---\n";

  std::cout << "> In bang constinit (Truoc khi sua): " << bang_sua_duoc.data[3]
            << "\n";
  bang_sua_duoc.data[3] = 999; // HỢP LỆ! constinit cho phép sửa.
  std::cout << "> In bang constinit (Sau khi sua): " << bang_sua_duoc.data[3]
            << "\n";

  std::cout << "> In bang constexpr: " << bang_co_dinh.data[3] << "\n";
  // bang_co_dinh.data[3] = 999; // LỖI BIÊN DỊCH! constexpr là hằng số đóng
  // băng.
}

// ==========================================
// TỔNG KẾT LUẬT DÙNG (C++20 BEST PRACTICES)
// ==========================================
/*
 1. CÓ NÊN TỪ BỎ CONSTEXPR?
    - Không! Hãy coi `consteval` như một chức năng KIỂM TRA ĐẦU VÀO NGHIÊM NGẶT.
    - Đại đa số hàm tiện ích (như toán học, chuỗi) vẫn nên dùng `constexpr` để
 nó vừa phục vụ được Compile-time, vừa tái sử dụng được lúc Runtime nếu người
 dùng vô tình truyền biến động vào.

 2. KHI NÀO BẮT BUỘC DÙNG CONSTEVAL?
    - Khi bạn thao tác trực tiếp với Macro thay thế, hoặc hàm đó xử lý Cấu trúc
 dữ liệu tĩnh (như Metaprogramming) mà đem xuống Runtime chạy thì sẽ gây Crash.

 3. KHI NÀO DÙNG CONSTINIT?
    - BẤT CỨ KHI NÀO BẠN VIẾT TỪ KHÓA `static` HOẶC KHAI BÁO BIẾN TOÀN CỤC
 (ngoài main).
    - Việc kẹp thêm `constinit` vào biến tĩnh/toàn cục gần như là BẮT BUỘC trong
 các công ty lớn hiện nay (như Google) để triệt tiêu vĩnh viễn lỗi Static
 Initialization Order Fiasco.
*/

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== C++20 CONSTEVAL & CONSTINIT =====\n";

  demo_consteval_co_ban();
  demo_constinit_co_ban();
  demo_consteval_validate();
  demo_chuyen_gia_ket_hop();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}