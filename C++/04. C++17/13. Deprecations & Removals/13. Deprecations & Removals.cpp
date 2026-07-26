#include <iostream>
#include <memory>    // Chứa std::unique_ptr
#include <stdexcept> // Chứa các chuẩn Exception
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 SỨ MỆNH CỦA C++17: "Dọn rác lịch sử"
 Trong suốt 20 năm, C++ phải gánh vác tính "tương thích ngược" (code cũ viết năm
 1998 vẫn phải chạy được trên C++14). Điều này khiến ngôn ngữ trở nên cồng kềnh
 và chứa nhiều tính năng lỗi thời, nguy hiểm. C++17 là cột mốc lịch sử khi Ủy
 ban C++ quyết định "MẠNH TAY XÓA BỎ" (Remove hoàn toàn, compiler sẽ báo lỗi chứ
 không chỉ cảnh báo nữa) những tàn dư độc hại này.
*/

// ==========================================
// CLASS PHỤ TRỢ ĐỂ DEMO BỘ NHỚ
// ==========================================
class TaiLieu Mat {
public:
  TaiLieu() { std::cout << "  [Tao] Tai lieu mat da duoc dua vao RAM.\n"; }
  ~TaiLieu() { std::cout << "  [Huy] Tai lieu mat da bi xoa khoi RAM.\n"; }
  void doc_du_lieu() { std::cout << "  -> Dang doc tai lieu an toan...\n"; }
};

// ==========================================
// 1. CÁI CHẾT CỦA `std::auto_ptr` (THAY BẰNG `std::unique_ptr`)
// ==========================================
void don_dep_auto_ptr() {
  std::cout << "\n--- 1. XOA BO STD::AUTO_PTR (KE GAY LOI THE KY) ---\n";

  /*
  LỊCH SỬ ĐEN TỐI:
  Ngày xưa ở C++98, std::auto_ptr được sinh ra để tự động gọi delete.
  Nhưng nó có một lỗi thiết kế CHÍ MẠNG trong phép Copy (Gán dấu =).

  Code C++98 (Đã bị Cấm biên dịch ở C++17):
      std::auto_ptr<TaiLieu> p1(new TaiLieu());
      std::auto_ptr<TaiLieu> p2 = p1;  // Tưởng là Copy, nhưng thực chất là ĂN
  CẮP!

      p1->doc_du_lieu(); // CRASH NGAY LẬP TỨC!!!
      // Vì p1 đã bị ngầm gán bằng NULL sau khi p2 ăn cắp vùng nhớ,
      // nhưng Compiler KHÔNG HỀ CẢNH BÁO.
  */

  // GIẢI PHÁP C++17 (Kế thừa từ C++11): Dùng std::unique_ptr
  std::cout << "Khoi tao std::unique_ptr:\n";
  std::unique_ptr<TaiLieu> p1 = std::make_unique<TaiLieu>();

  // std::unique_ptr<TaiLieu> p2 = p1;
  // ^ LỖI BIÊN DỊCH NGAY LẬP TỨC! C++17 cấm hoàn toàn phép Copy trên unique_ptr
  // để bảo vệ bạn.

  // Nếu thực sự muốn chuyển giao quyền sở hữu, phải dùng std::move() MỘT CÁCH
  // RÕ RÀNG:
  std::unique_ptr<TaiLieu> p2 = std::move(p1);

  std::cout << "Da chuyen giao (move) an toan tu p1 sang p2.\n";

  // Kiểm tra an toàn trước khi dùng
  if (!p1) {
    std::cout
        << "  -> p1 da tro thanh rong (nullptr), khong bi Crash ngam nua!\n";
  }
  p2->doc_du_lieu(); // Chạy bình thường
}

// ==========================================
// 2. KHAI TỬ TỪ KHÓA `register`
// ==========================================
void don_dep_register() {
  std::cout << "\n--- 2. XOA BO TU KHOA 'REGISTER' ---\n";

  /*
  LỊCH SỬ ĐEN TỐI:
  Vào thập niên 80-90, RAM rất chậm. Lập trình viên C/C++ có thói quen thêm chữ
  `register` vào trước biến vòng lặp để "van xin" CPU đưa biến này vào Thanh ghi
  (Register - nơi nhanh nhất của CPU) nhằm tăng tốc độ.

  Code C++98 (Lỗi biên dịch ở C++17):
      register int i = 0;
      for(register int i = 0; i < 1000; ++i) { ... }

  TẠI SAO BỊ XÓA?
  Từ những năm 2000, các Trình biên dịch (như GCC, Clang) với cờ tối ưu (-O2,
  -O3) đã thông minh hơn con người gấp vạn lần. NÓ BIẾT CHÍNH XÁC biến nào nên
  nằm ở RAM, biến nào nên nằm ở Thanh ghi. Việc lập trình viên tự gõ chữ
  `register` chỉ làm rối luồng tối ưu hóa của Trình biên dịch.
  */

  // GIẢI PHÁP C++17:
  // Cứ gõ code bình thường. Từ khóa `register` vẫn được ngôn ngữ C++ "giữ lại"
  // trong từ điển (dự trữ cho tương lai), nhưng bạn KHÔNG ĐƯỢC PHÉP dùng nó làm
  // biến nữa.

  int tong = 0;
  for (int i = 0; i < 1000000;
       ++i) { // Trình biên dịch sẽ TỰ ĐỘNG đưa 'i' vào thanh ghi CPU!
    tong += i;
  }
  std::cout << "Vong lap chay binh thuong. Compiler tu lo viec toi uu!\n";
}

// ==========================================
// 3. XÓA BỎ DYNAMIC EXCEPTION SPECIFICATION (throw(...))
// ==========================================

/*
 LỊCH SỬ ĐEN TỐI:
 Ngày xưa, người ta muốn kiểm soát chặt chẽ một hàm có thể ném ra những Lỗi
 (Exception) gì.

 Code C++98 (Đã bị cấm ở C++17):
    void ham_ket_noi_mang() throw(int, std::string) {
        // Lời thề: Hàm này CHỈ ĐƯỢC PHÉP ném ra số nguyên (int) hoặc chuỗi
 (string).
    }

 TẠI SAO LÀ THẢM HỌA?
 1. Nó không báo lỗi lúc biên dịch (Compile-time). Nếu hàm lỡ ném ra kiểu float,
    trình biên dịch vẫn cho qua.
 2. Thay vào đó, lúc chạy (Runtime), C++ sẽ phải chèn thêm hàng tấn code ngầm để
 kiểm tra xem cái lỗi ném ra có đúng là int hoặc string không. Nếu sai, nó gọi
 hàm std::terminate() và Crash ngay lập tức. Điều này làm chương trình CHẬM ĐI
 RẤT NHIỀU.
*/

// GIẢI PHÁP 1 TRONG C++17: HÀM CÓ THỂ NÉM LỖI (Để trống)
// Nếu hàm có khả năng văng lỗi, cứ khai báo bình thường. Đừng cố liệt kê kiểu
// lỗi ra.
void ham_xu_ly_file(bool bi_loi) {
  if (bi_loi) {
    throw std::runtime_error("Loi: Khong tim thay File!"); // Ném lỗi chuẩn
  }
}

// GIẢI PHÁP 2 TRONG C++17: HÀM TUYỆT ĐỐI KHÔNG NÉM LỖI (Dùng noexcept)
// `noexcept` (Không ngoại lệ) là từ khóa thời đại mới. Nó nói với Compiler:
// "Tao thề hàm này không bao giờ văng lỗi".
// Nhờ lời thề này, Compiler sẽ cắt bỏ toàn bộ code kiểm tra lỗi dưới nền, giúp
// hàm chạy NHANH HƠN.
void ham_tinh_toan_thuan_tuy(int a, int b) noexcept {
  int c = a + b;
  // std::cout << c;
  // Lưu ý: Ngay cả std::cout cũng có thể văng lỗi, nên viết code trong hàm
  // noexcept phải cực kỳ cẩn thận chỉ dùng các toán tử an toàn.
}

void don_dep_exception() {
  std::cout << "\n--- 3. XOA BO THROW TRÊN CHỮ KÝ HÀM ---\n";
  std::cout << "Khai bao 'throw(int)' da bi khai tu. Thay bang 'noexcept' neu "
               "ham an toan.\n";

  try {
    ham_xu_ly_file(true);
  } catch (const std::exception &e) {
    std::cout << "Da bat duoc loi an toan: " << e.what() << "\n";
  }

  ham_tinh_toan_thuan_tuy(5, 10);
  std::cout << "Ham noexcept thuc thi sieu toc do va khong the bi crash do "
               "exception.\n";
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== C++17 DEPRECATIONS & REMOVALS =====\n";

  don_dep_auto_ptr();
  don_dep_register();
  don_dep_exception();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}