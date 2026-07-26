#include <iostream>
#include <memory>    // Bắt buộc chứa std::unique_ptr và std::make_unique
#include <stdexcept> // Chứa std::runtime_error
#include <string>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Lịch sử:
    - C++11 ra mắt std::unique_ptr và std::shared_ptr, kèm theo hàm
 std::make_shared. Nhưng Hội đồng C++ lại... "quên" thêm std::make_unique.
    - Hậu quả: Lập trình viên vẫn phải dùng từ khóa 'new' khi tạo unique_ptr:
      std::unique_ptr<int> ptr(new int(10));
    - C++14 chính thức bổ sung std::make_unique để kết liễu hoàn toàn từ khóa
 'new'.

 2. 3 Lý do TỐI THƯỢNG bắt buộc phải dùng make_unique:
    - Code ngắn gọn hơn (Không cần lặp lại tên Class 2 lần).
    - An toàn ngoại lệ (Exception Safety): Vá lỗ hổng rò rỉ RAM chí mạng của
 C++11.
    - Hiệu suất: Tương đương 100% với việc dùng 'new' (Zero-overhead).
*/

// ==========================================
// CLASS PHỤ TRỢ CHO CÁC VÍ DỤ
// ==========================================
class HanhKhach {
public:
  std::string ten;

  // Constructor
  HanhKhach(std::string t) : ten(t) {
    std::cout << "  [+] Tao Hanh Khach: " << ten << "\n";
  }
  // Destructor
  ~HanhKhach() {
    std::cout << "  [-] Huy Hanh Khach: " << ten << " (Tu dong don RAM!)\n";
  }

  void chao() { std::cout << "      Xin chao, toi la " << ten << "\n"; }
};

// ==========================================
// 1. MỨC CƠ BẢN: CÚ PHÁP VÀ SỰ THAY THẾ 'NEW'
// ==========================================
void make_unique_co_ban() {
  std::cout << "\n--- 1. BAZIC: THAY THE HOAN TOAN NEW & DELETE ---\n";

  // a. CÁCH CŨ (C++11) - Dài dòng và phải xài chữ 'new'
  std::unique_ptr<HanhKhach> hk_cu(new HanhKhach("Le Van Cu"));

  // b. CÁCH MỚI (C++14) - Dung make_unique (Khuyên dùng tuyệt đối)
  // Cú pháp: std::make_unique<KieuDuLieu>(Cac_Tham_So_Cua_Constructor...)
  auto hk_moi = std::make_unique<HanhKhach>("Tran Thi Moi");

  hk_moi->chao();

} // Hết hàm, cả hk_cu và hk_moi tự động gọi hàm Hủy. Khong can ghi 'delete'!

// ==========================================
// 2. MỨC TRUNG CẤP: VÁ LỖ HỔNG RÒ RỈ BỘ NHỚ (C++11 BUG)
// ==========================================
// Hàm mô phỏng một tính toán bị lỗi và ném ra Exception
int tinh_toan_loi() {
  std::cout << "  [!] Dang chay tinh toan... BOM NO!\n";
  throw std::runtime_error("Loi he thong!");
  return 0;
}

// Hàm nhận vào một unique_ptr và 1 con số
void xu_ly_du_lieu(std::unique_ptr<HanhKhach> hk, int so_lieu) {
  std::cout << "  Da nhan hanh khach " << hk->ten << " va so lieu " << so_lieu
            << "\n";
}

void make_unique_bao_mat_exception() {
  std::cout << "\n--- 2. INTERMEDIATE: EXCEPTION SAFETY ---\n";

  // LỖ HỔNG KINH ĐIỂN TRONG C++11:
  try {
    std::cout << "> Thu dung cach C++11 (new):\n";
    /*
       VẤN ĐỀ: C++ Compiler có quyền đổi thứ tự thực thi của 3 bước sau:
       Bước 1: Chạy 'new HanhKhach("Nhan")' (Cấp phát RAM, đã tạo Object thành
       công) Bước 2: Chạy hàm tinh_toan_loi() Bước 3: Gói Object vào
       std::unique_ptr

       NẾU Compiler chạy Bước 1 -> Bước 2 (Bom nổ).
       Thì Bước 3 KHÔNG BAO GIỜ được chạy! Object "Nhan" đã tạo ra nằm trơ trọi
       trên RAM mà chưa kịp được bảo vệ bởi unique_ptr -> RÒ RỈ BỘ NHỚ VĨNH
       VIỄN!
    */
    // Uncomment dòng dưới để thấy memory leak (Hàm hủy sẽ không bao giờ chạy)
    // xu_ly_du_lieu(std::unique_ptr<HanhKhach>(new HanhKhach("Nhan")),
    // tinh_toan_loi());

    std::cout << "  (Da bi comment de tranh leak RAM thuc su)\n";

    std::cout << "\n> Thu dung cach C++14 (make_unique):\n";
    /*
       GIẢI PHÁP: make_unique là một hàm. Việc cấp phát RAM và gói vào
       unique_ptr được diễn ra BÊN TRONG hàm đó. Compiler không thể chen ngang
       hàm tinh_toan_loi() vào giữa quá trình này được. An toàn 100%!
    */
    xu_ly_du_lieu(std::make_unique<HanhKhach>("An"), tinh_toan_loi());

  } catch (const std::exception &e) {
    std::cout << "  [Bắt được lỗi]: " << e.what() << "\n";
  }
}

// ==========================================
// 3. MỨC NÂNG CAO: MAKE_UNIQUE CHO MẢNG ĐỘNG (ARRAYS)
// ==========================================
void make_unique_mang_dong() {
  std::cout << "\n--- 3. ADVANCED: MAKE_UNIQUE CHO MANG (ARRAYS) ---\n";

  // Thay vì: std::unique_ptr<int[]> mang(new int[5]());
  // Dùng make_unique, truyền <int[]> và cấp số lượng ở tham số:
  auto mang = std::make_unique<int[]>(5);

  for (int i = 0; i < 5; ++i) {
    mang[i] = i * 10;
    std::cout << "mang[" << i << "] = " << mang[i] << "\n";
  }

  // LƯU Ý: Không thể khởi tạo giá trị cụ thể cho mảng bằng make_unique
  // (như make_unique<int[]>{1, 2, 3} là KHÔNG được hỗ trợ).
  // Nó chỉ có thể khởi tạo mảng gồm toàn số 0.
}

// ==========================================
// 4. MỨC CHUYÊN GIA: BÍ MẬT DƯỚI NỀN VÀ TRƯỜNG HỢP CẤM DÙNG
// ==========================================

// BÍ MẬT DƯỚI NỀN: Làm sao std::make_unique có thể nhận "bất kỳ" tham số nào?
// Trả lời: Nó sử dụng Variadic Templates và std::forward (Perfect Forwarding).
// Dưới đây là cách bạn CÓ THỂ TỰ VIẾT RA make_unique nếu dùng C++11:

template <typename T, typename... Args>
std::unique_ptr<T> tu_che_make_unique(Args &&...args) {
  // Bê y nguyên toàn bộ tham số truyền vào hàm Constructor của T
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// TRƯỜNG HỢP DUY NHẤT VẪN PHẢI DÙNG 'NEW' HOẶC KHÔNG DÙNG MAKE_UNIQUE
struct FileDeleter {
  void operator()(FILE *f) const {
    if (f) {
      std::cout << "  [-] Dong File an toan!\n";
      fclose(f);
    }
  }
};

void make_unique_chuyen_gia_han_che() {
  std::cout << "\n--- 4. EXPERT: UNDER THE HOOD & LIMITATIONS ---\n";

  // 1. Dùng thử make_unique tự chế
  std::cout << "> Test tu_che_make_unique:\n";
  auto hk = tu_che_make_unique<HanhKhach>("Tu Che");

  // 2. GIỚI HẠN CHÍ MẠNG: Custom Deleter (Bộ hủy tùy chỉnh)
  // Khi bạn quản lý các tài nguyên đặc thù của hệ điều hành (như con trỏ FILE,
  // Socket...), bạn không thể dùng lệnh 'delete' mặc định, mà phải gọi 'fclose'
  // hoặc 'close'. std::make_unique KHÔNG CHO PHÉP bám kèm Custom Deleter!

  std::cout
      << "\n> Truong hop bat buoc khong dung make_unique (Custom Deleter):\n";

  // Ở đây ta BẮT BUỘC phải dùng cách khai báo C++11 cổ điển.
  // (Lưu ý: fopen có thể cần quyền ghi file thực tế trên HĐH của bạn)
  std::unique_ptr<FILE, FileDeleter> file_ptr(fopen("test.txt", "w"));
  if (file_ptr) {
    std::cout << "  [+] Mo file thanh cong. Chuan bi thoat pham vi de test "
                 "auto-close...\n";
  } else {
    std::cout << "  [!] Khong the tao file (co the do loi quyen HĐH), nhung "
                 "logic van dung.\n";
  }
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  make_unique_co_ban();
  make_unique_bao_mat_exception();
  make_unique_mang_dong();
  make_unique_chuyen_gia_han_che();

  std::cout << "\n--- HOAN THANH BAO CAO ---\n";
  return 0;
}