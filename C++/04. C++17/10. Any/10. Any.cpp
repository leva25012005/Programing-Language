#include <any> // Bắt buộc: Thư viện chứa std::any (C++17)
#include <iostream>
#include <string>
#include <typeinfo> // Cho typeid
#include <utility>
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. std::any là gì?
    - Là một Class (chiếc hộp) có khả năng chứa đúng 1 giá trị thuộc BẤT KỲ kiểu
 dữ liệu nào.
    - Bạn có thể nhét `int` vào, sau đó vứt `int` đi và nhét `std::string` vào
 chính chiếc hộp đó.

 2. Tại sao không dùng void* ?
    - C cổ điển dùng `void*` để trỏ tới mọi thứ. Nhưng nó KHÔNG an toàn! Bạn ép
 kiểu sai, chương trình sẽ Crash (Segfault).
    - `void*` không biết cách gọi Hàm Hủy (Destructor) của Object, gây rò rỉ bộ
 nhớ (Memory Leak).
    - `std::any` NHỚ được kiểu dữ liệu nó đang chứa, và TỰ ĐỘNG gọi Destructor
 khi bị hủy. Cực kỳ an toàn!

 3. Hiệu năng (Bí mật dưới nền): Small Object Optimization (SOO)
    - Nếu dữ liệu nhỏ (thường <= 8 hoặc 16 bytes như int, double), std::any lưu
 trực tiếp trên Stack (Siêu nhanh).
    - Nếu dữ liệu lớn (như vector, struct to), std::any sẽ lén lút dùng lệnh
 `new` để cấp phát RAM trên Heap (Tốn thời gian).
*/

// ==========================================
// CLASS PHỤ TRỢ CHO PHẦN CHUYÊN GIA
// ==========================================
class VuKhiKhung {
public:
  std::string ten;
  int sat_thuong;

  VuKhiKhung(std::string t, int st) : ten(t), sat_thuong(st) {
    std::cout << "    [Khoi Tao] Vu khi: " << ten << "\n";
  }
  ~VuKhiKhung() { std::cout << "    [Huy] Vu khi: " << ten << "\n"; }
};

// ==========================================
// 1. MỨC CƠ BẢN: KHỞI TẠO, GÁN VÀ KIỂM TRA
// ==========================================
void any_co_ban() {
  std::cout << "\n--- 1. BAZIC: KHOI TAO & KIEM TRA ---\n";

  std::any hop_ma_thuat; // Hộp rỗng

  // Kiểm tra xem hộp có chứa dữ liệu không
  std::cout << "Hop co rông khong? "
            << (!hop_ma_thuat.has_value() ? "Co" : "Khong") << "\n";

  // Nhét số nguyên vào
  hop_ma_thuat = 42;
  std::cout << "Da nhet so. Co du lieu chua? "
            << (hop_ma_thuat.has_value() ? "Roi" : "Chua") << "\n";

  // Nhét Chuỗi vào (Tự động vứt bỏ số 42)
  hop_ma_thuat = std::string("Xin chao the gioi!");

  // Hỏi xem hộp ĐANG chứa kiểu dữ liệu gì (Dùng .type())
  std::cout << "Kieu du lieu hien tai: " << hop_ma_thuat.type().name() << "\n";
}

// ==========================================
// 2. MỨC TRUNG CẤP: LẤY DỮ LIỆU RA (ANY_CAST) & BẮT LỖI
// ==========================================
void any_trung_cap() {
  std::cout << "\n--- 2. INTERMEDIATE: ANY_CAST & BAD_ANY_CAST ---\n";

  std::any a = 3.14; // Chứa kiểu double

  // LẤY DỮ LIỆU ĐÚNG: Phải dùng std::any_cast<Kieu_Du_Lieu>(bien_any)
  double pi = std::any_cast<double>(a);
  std::cout << "Gia tri lay ra thanh cong: " << pi << "\n";

  // LẤY DỮ LIỆU SAI KIỂU (CẠM BẪY CHÍ MẠNG)
  // std::any cực kỳ khắt khe: Nếu nhét 'double', lấy 'int' hoặc 'float' CŨNG BỊ
  // LỖI! Phải chuẩn xác 100%.
  try {
    int sai_kieu = std::any_cast<int>(a); // Sẽ ném ra ngoại lệ!
  } catch (const std::bad_any_cast &e) {
    std::cout << "Da chan loi thanh cong: " << e.what()
              << " (Khong the ep double thanh int!)\n";
  }
}

// ==========================================
// 3. MỨC NÂNG CAO: CHỈNH SỬA TRỰC TIẾP & ÉP KIỂU CON TRỎ
// ==========================================
void any_nang_cao() {
  std::cout << "\n--- 3. ADVANCED: SUA TRUC TIEP & CON TRO ---\n";

  std::any ten_nhan_vat = std::string("Arthur");

  // a. CHỈNH SỬA TRỰC TIẾP (DÙNG THAM CHIẾU &)
  // Nếu lấy ra bằng any_cast<std::string>, nó sẽ COPY ra 1 biến mới.
  // Muốn sửa trực tiếp dữ liệu bên trong chiếc hộp, phải dùng tham chiếu:
  // any_cast<std::string&>
  std::any_cast<std::string &>(ten_nhan_vat) = "Lancelot";

  std::cout << "Ten sau khi sua truc tiep vao hop: "
            << std::any_cast<std::string>(ten_nhan_vat) << "\n";

  // b. ÉP KIỂU CON TRỎ (KHÔNG DÙNG TRY/CATCH)
  // Try/catch làm chương trình chậm. C++ cho phép bạn truyền CON TRỎ của chiếc
  // hộp vào any_cast. Nếu sai kiểu, nó sẽ không ném lỗi mà TỰ TRẢ VỀ NULLPTR!

  std::any diem = 100;

  // Lưu ý cú pháp: Truyền địa chỉ (&diem) vào
  int *ptr_diem = std::any_cast<int>(&diem);

  if (ptr_diem != nullptr) {
    std::cout << "Lay con tro thanh cong, diem la: " << *ptr_diem << "\n";
  }

  // Thử lấy sai kiểu (Lấy string từ hộp chứa int)
  std::string *ptr_chuoi = std::any_cast<std::string>(&diem);
  if (ptr_chuoi == nullptr) {
    std::cout
        << "Lay chuoi that bai! Tra ve nullptr rat an toan, khong bi Crash.\n";
  }
}

// ==========================================
// 4. MỨC CHUYÊN GIA: LƯU CLASS, MAKE_ANY & RESET
// ==========================================
void any_chuyen_gia() {
  std::cout << "\n--- 4. EXPERT: CUSTOM CLASS, MAKE_ANY & RESET ---\n";

  std::cout << "Dung std::any chua Object Tuy chinh:\n";

  // a. CÁCH LÀM SAI / CHẬM (Tốn 1 lần Copy/Move)
  // std::any kho_do = VuKhiKhung("Kiem go", 10);

  // b. CÁCH LÀM ĐÚNG: std::make_any (Giống make_unique)
  // Nó khởi tạo Object TRỰC TIẾP ngay bên trong vùng nhớ của chiếc hộp
  // std::any, miễn phí hoàn toàn chi phí Copy/Move!
  std::any kho_do = std::make_any<VuKhiKhung>("Kiem Anh Sang", 9999);

  // Lấy object ra đọc (Dùng tham chiếu để tránh copy)
  const auto &vu_khi = std::any_cast<const VuKhiKhung &>(kho_do);
  std::cout << "Vu khi trong kho: " << vu_khi.ten
            << " - Dame: " << vu_khi.sat_thuong << "\n";

  // c. HỦY DỮ LIỆU CHỦ ĐỘNG
  std::cout << "> Goi ham reset() de don sach hop:\n";
  kho_do.reset(); // Hàm này sẽ lập tức kích hoạt Destructor của VuKhiKhung!
  std::cout << "> Hop da rong. Ket thuc.\n";
}

// ==========================================
// TỔNG KẾT: KHI NÀO NÊN DÙNG STD::ANY? (Kinh nghiệm thực chiến)
// ==========================================
/*
 1. NÊN DÙNG:
    - Làm cầu nối giao tiếp với các ngôn ngữ kịch bản (như Python, Lua) nơi mà
 các biến thường mang tính "Động" (Dynamic Type).
    - Tạo các Hệ thống Gửi thông điệp (Message Passing System / Event Bus) trong
 Game Engine, nơi 1 cục Event phải chứa đủ loại dữ liệu tạp nham không biết
 trước.
    - Làm UI Framework (Data Binding) nơi một "Cell" có thể chứa Text, Image,
 hoặc Checkbox.

 2. KHÔNG NÊN DÙNG:
    - Đừng dùng nó chỉ vì lười suy nghĩ thiết kế Class (Code Smell).
    - Nếu bạn biết CHẮC CHẮN chiếc hộp chỉ có thể chứa TỐI ĐA 2 hoặc 3 kiểu dữ
 liệu (Ví dụ: Biến có thể là 'int' HOẶC 'string'), HÃY DÙNG `std::variant`.
    - `std::variant` cực kỳ tối ưu về bộ nhớ và hiệu năng, trong khi `std::any`
      sẽ lén dùng cấp phát động (Heap) nếu nhét dữ liệu lớn vào.
*/

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  any_co_ban();
  any_trung_cap();
  any_nang_cao();
  any_chuyen_gia();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}