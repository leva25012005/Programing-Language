#include <cstdint>
#include <iostream>
#include <string>
#include <type_traits> // Thư viện cực kỳ quan trọng đi kèm với static_assert

// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. static_assert là gì?
    - Ra mắt từ C++11, nó là một "Chốt chặn bảo vệ" hoạt động NGAY LÚC BIÊN DỊCH
 (Compile-time).
    - Cú pháp: static_assert(dieu_kien_boolean, "Thong bao loi");
      (Từ C++17, thông báo lỗi là tùy chọn).

 2. Khác biệt với assert() thông thường:
    - assert(): Chỉ kiểm tra khi chương trình ĐANG CHẠY (Runtime). Nếu lỗi,
 chương trình bị Crash.
    - static_assert: Kiểm tra trước cả khi file .exe được tạo ra. Nếu lỗi, BÁO
 LỖI BIÊN DỊCH (Build failed) kèm theo dòng "Thong bao loi" hiển thị đỏ chót
 trên IDE. Hoàn toàn KHÔNG tốn hiệu năng khi chạy!

 3. Sứ mệnh:
    - Đảm bảo môi trường biên dịch đúng chuẩn (Ví dụ: Chắc chắn RAM hệ thống là
 64-bit).
    - Giới hạn kiểu dữ liệu truyền vào Template (Template Metaprogramming).
    - Đảm bảo các hàm constexpr tính toán đúng logic.
*/

// ==========================================
// 1. MỨC CƠ BẢN: KIỂM TRA MÔI TRƯỜNG VÀ KÍCH THƯỚC
// ==========================================
// Các lệnh static_assert có thể đặt trơ trọi ở cấp độ toàn cục (Global scope),
// không cần nằm trong bất kỳ hàm nào vì nó chạy lúc Compiler đang dịch code.

// Đảm bảo kiểu 'int' trên máy tính đang dịch code phải có ít nhất 4 bytes (32
// bit)
static_assert(
    sizeof(int) >= 4,
    "Loi: He thong nay co kieu 'int' nho hon 4 bytes, code se bi tran so!");

// Đảm bảo con trỏ có kích thước 8 bytes (Tức là đang build ở chế độ 64-bit x64,
// không phải x86) (Thử chuyển IDE sang build x86, dòng này sẽ báo lỗi ngay lập
// tức)
static_assert(sizeof(void *) == 8,
              "Vui long Build du an o che do 64-bit (x64)!");

// C++17: Không cần ghi thông báo lỗi nếu bạn thấy nó quá rõ ràng
static_assert(sizeof(char) == 1);

// ==========================================
// 2. MỨC TRUNG CẤP: KIỂM TRA ĐẶC TÍNH CLASS (TYPE TRAITS)
// ==========================================
// Thư viện <type_traits> cung cấp hàng loạt công cụ soi mói kiểu dữ liệu. Kết
// hợp với static_assert sẽ tạo ra lớp bảo mật vững chắc cho kiến trúc phần mềm.

struct ToaDo {
  int x;
  int y;
};

class QuaiVat {
public:
  virtual void tan_cong() {
  } // Có virtual -> Có vtable -> Không phải kiểu dữ liệu thô (POD)
};

// Đảm bảo ToaDo là kiểu dữ liệu thô (Standard Layout), có thể copy an toàn bằng
// hàm memcpy của C
static_assert(std::is_standard_layout_v<ToaDo>,
              "ToaDo phai la kieu layout chuan de copy nhanh!");

// Nếu mở comment dòng dưới, code sẽ LỖI BIÊN DỊCH vì QuaiVat có chứa hàm ảo
// (virtual) static_assert(std::is_standard_layout_v<QuaiVat>, "Loi: QuaiVat
// chua ham ao, memory memory khong lien tuc!");

// Kiểm tra tính Đa hình (Polymorphic)
static_assert(std::is_polymorphic_v<QuaiVat>,
              "QuaiVat phai ho tro da hinh (Co virtual func)!");

// ==========================================
// 3. MỨC NÂNG CAO: BẢO VỆ TEMPLATE TRƯỚC SỰ NGU NGỐC
// ==========================================
// Khi viết Hàm Template, bạn mở cửa cho mọi kiểu dữ liệu đi vào.
// static_assert giúp bạn "đóng cửa bớt" những kiểu dữ liệu gây nguy hiểm.

template <typename T> T tinh_chia(T a, T b) {
  // 1. NGĂN CHẶN KIỂU SỐ NGUYÊN (Vì phép chia số nguyên bị mất phần thập phân)
  // Tức là: Nếu người dùng lỡ truyền int, Compiler sẽ chửi ngay thay vì ra kết
  // quả sai ngầm.
  static_assert(std::is_floating_point_v<T>,
                "LOI NGHIEM TRONG: Ham chia nay chi ho tro kieu So Thuc "
                "(float, double)!");

  return a / b;
}

template <typename T> void in_do_dai_chuoi(T chuoi) {
  // 2. ÉP BUỘC PHẢI LÀ STRING HOẶC KẾ THỪA TỪ STRING
  static_assert(std::is_same_v<T, std::string>,
                "LOI: Tham so truyen vao bat buoc phai la std::string!");
  std::cout << "Do dai chuoi: " << chuoi.length() << "\n";
}

// ==========================================
// 4. MỨC CHUYÊN GIA: KIỂM TOÁN LOGIC CỦA CONSTEXPR
// ==========================================
// Từ C++14/C++17, hàm 'constexpr' cho phép tính toán logic ngay lúc biên dịch.
// Bạn có thể dùng static_assert để viết Unit Test cho chính các hàm đó ngay
// trong lúc Build!

constexpr int tinh_giai_thua(int n) {
  return n <= 1 ? 1 : n * tinh_giai_thua(n - 1);
}

// VIẾT UNIT TEST NGAY LÚC COMPILER ĐANG CHẠY:
// Trình biên dịch sẽ tự tính giai thừa của 5. Nếu không bằng 120, nó TỪ CHỐI
// BIÊN DỊCH. Điều này chứng minh hàm tinh_giai_thua() được viết đúng logic 100%
// trước khi tới tay khách hàng.
static_assert(tinh_giai_thua(5) == 120,
              "Logic ham giai thua bi sai! 5! phai bang 120.");
static_assert(tinh_giai_thua(0) == 1,
              "Logic ham giai thua bi sai o Base case (n=0)!");

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== KHOA HOC STATIC_ASSERT =====\n";
  std::cout << "Neu chuong trinh nay In ra duoc dong nay, \n";
  std::cout << "co nghia la TAT CA cac bai Test cua static_assert \n";
  std::cout << "da duoc Trinh bien dich thong qua thanh cong!\n\n";

  // --- TEST TEMPLATE (MỨC NÂNG CAO) ---
  double kq = tinh_chia(10.5, 2.0);
  std::cout << "Ket qua chia: " << kq << "\n";

  // Thử bỏ comment dòng dưới, bạn sẽ thấy IDE báo lỗi đỏ chót ngay lập tức:
  // int kq_loi = tinh_chia(10, 2);

  std::string text = "Xin chao!";
  in_do_dai_chuoi(text);

  // Thử bỏ comment dòng dưới, IDE cũng sẽ báo lỗi vì "Hello" là const char*,
  // không phải std::string in_do_dai_chuoi("Hello");

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}