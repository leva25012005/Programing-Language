#include <iostream>
#include <memory> // Bắt buộc: Thư viện chứa Smart Pointers
#include <string>
#include <vector>

// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Tại sao cần Smart Pointers?
    - Trong C++ cổ điển, dùng con trỏ thô (T*) với `new` và `delete` rất nguy
 hiểm.
    - Quên delete -> Rò rỉ bộ nhớ (Memory Leak). Delete 2 lần -> Crash chương
 trình.
    - Smart Pointers áp dụng triết lý RAII (Resource Acquisition Is
 Initialization): Khi một con trỏ thông minh bị hủy (thoát khỏi hàm/vòng lặp),
 NÓ SẼ TỰ ĐỘNG GỌI DELETE.

 2. Ba chàng lính ngự lâm của <memory>:
    - std::unique_ptr : Chế độ Độc tài. Tại một thời điểm, chỉ có 1 chủ nhân sở
 hữu vùng nhớ.
    - std::shared_ptr : Chế độ Cộng sản. Nhiều người cùng sở hữu 1 vùng nhớ.
 Chia sẻ qua Bộ đếm (Reference Count).
    - std::weak_ptr   : Kẻ quan sát. Nhìn trộm shared_ptr mà không làm tăng bộ
 đếm, dùng để phá Vòng lặp tử thần.
*/

// ==========================================
// CÁC CLASS PHỤ TRỢ (Để quan sát quá trình RAII)
// ==========================================
class VuKhi {
public:
  std::string ten;
  VuKhi(std::string t) : ten(t) {
    std::cout << "  [RAII] Vu khi '" << ten << "' duoc CHE TAO tren Heap!\n";
  }
  ~VuKhi() {
    std::cout << "  [RAII] Vu khi '" << ten
              << "' bi PHA HUY va giai phong RAM!\n";
  }
  void tan_cong() { std::cout << "  -> " << ten << " dang chem!\n"; }
};

// ==========================================
// 1. MỨC CƠ BẢN: STD::UNIQUE_PTR (CHỦ NHÂN ĐỘC TÔN)
// ==========================================
void smart_pointer_unique() {
  std::cout
      << "\n--- 1. BAZIC: UNIQUE_PTR (Khong chia se, khong overhead) ---\n";

  {
    // TẠO UNIQUE_PTR: LUÔN LUÔN dùng std::make_unique (Từ C++14 trở lên)
    // Tuyệt đối không dùng chữ 'new' ở đây để đảm bảo an toàn ngoại lệ
    // (Exception safety).
    std::unique_ptr<VuKhi> ptr1 = std::make_unique<VuKhi>("Kiem Phep");

    ptr1->tan_cong(); // Dùng mũi tên -> y hệt con trỏ thô

    // std::unique_ptr<VuKhi> ptr2 = ptr1; // LỖI BIÊN DỊCH! Cấm Copy!

    // MUỐN ĐỔI CHỦ? Phải dùng std::move (Tước quyền sở hữu)
    std::unique_ptr<VuKhi> chu_nhan_moi = std::move(ptr1);

    if (!ptr1) {
      std::cout << "  -> ptr1 da bi tuoc quyen va tro thanh nullptr.\n";
    }
    chu_nhan_moi->tan_cong();

  } // Vừa chạm ngoặc '}' này, chu_nhan_moi hết hạn -> Tự động gọi Destructor
    // của VuKhi!
  std::cout << "Khoi lenh unique_ptr da ket thuc.\n";
}

// ==========================================
// 2. MỨC TRUNG CẤP: STD::SHARED_PTR (ĐỒNG SỞ HỮU)
// ==========================================
void smart_pointer_shared() {
  std::cout << "\n--- 2. INTERMEDIATE: SHARED_PTR & REFERENCE COUNTING ---\n";

  // TẠO SHARED_PTR: LUÔN LUÔN dùng std::make_shared
  // Tại sao? make_shared giúp gộp việc cấp phát Data và Khối Điều Khiển
  // (Control Block) làm 1 lần duy nhất, giúp tối ưu hiệu năng và tránh phân
  // mảnh RAM.
  std::shared_ptr<VuKhi> shared1 = std::make_shared<VuKhi>("Khien Than");

  std::cout << "So nguoi dang giu Khien Than: " << shared1.use_count()
            << "\n"; // In ra 1

  {
    // Được phép COPY thoải mái!
    std::shared_ptr<VuKhi> shared2 = shared1;
    std::shared_ptr<VuKhi> shared3 = shared1;

    std::cout << "So nguoi giu Khien sau khi Copy: " << shared1.use_count()
              << "\n"; // In ra 3
  } // shared2 và shared3 bị hủy.

  // Bộ đếm tự động tụt xuống, nhưng Khien Than VẪN SỐNG vì shared1 vẫn còn giữ!
  std::cout << "So nguoi giu Khien sau khi thoat khoi lenh con: "
            << shared1.use_count() << "\n"; // In ra 1
} // shared1 chết -> Bộ đếm = 0 -> Xóa Khien Than khỏi RAM.

// ==========================================
// 3. MỨC NÂNG CAO: LỖ HỔNG CIRCULAR DEPENDENCY & WEAK_PTR
// ==========================================
// Lỗ hổng kinh điển: Cha trỏ tới Con (bằng shared), Con trỏ ngược lại Cha (bằng
// shared). Hậu quả: Cả hai khóa nhau, bộ đếm không bao giờ về 0 -> MEMORY LEAK
// vĩnh viễn!
class Con; // Khai báo trước

class Cha {
public:
  std::shared_ptr<Con> con_trai;
  Cha() { std::cout << "  [RAII] Nguoi Cha chao doi\n"; }
  ~Cha() { std::cout << "  [RAII] Nguoi Cha qua doi\n"; }
};

class Con {
public:
  // LIỀU THUỐC GIẢI: Thay vì dùng shared_ptr, Con chỉ dùng weak_ptr để "nhìn"
  // Cha. weak_ptr KHÔNG làm tăng bộ đếm use_count() của Cha.
  std::weak_ptr<Cha> nguoi_cha;

  Con() { std::cout << "  [RAII] Nguoi Con chao doi\n"; }
  ~Con() { std::cout << "  [RAII] Nguoi Con qua doi\n"; }
};

void smart_pointer_weak() {
  std::cout << "\n--- 3. ADVANCED: WEAK_PTR & VONG LAP TU THAN ---\n";

  auto cha = std::make_shared<Cha>();
  auto con = std::make_shared<Con>();

  // Thiết lập mối quan hệ
  cha->con_trai = con;
  con->nguoi_cha = cha; // Không làm tăng use_count của cha!

  std::cout << "Bo dem cua Cha: " << cha.use_count()
            << "\n"; // Vẫn là 1 nhờ weak_ptr
  std::cout << "Bo dem cua Con: " << con.use_count()
            << "\n"; // Là 2 (1 từ biến 'con', 1 từ 'cha->con_trai')

  // CÁCH SỬ DỤNG WEAK_PTR: Bạn không thể dùng -> trực tiếp trên weak_ptr.
  // Phải gọi hàm lock() để chuyển nó thành shared_ptr tạm thời (để đề phòng Cha
  // đã chết).
  std::shared_ptr<Cha> cha_tam_thoi = con->nguoi_cha.lock();
  if (cha_tam_thoi) {
    std::cout << "  -> Nguoi Cha van con song, co the tuong tac!\n";
  } else {
    std::cout << "  -> Nguoi Cha da qua doi!\n";
  }
} // Thoát hàm: cha chết -> bộ đếm Cha về 0 -> hủy Cha -> hủy 'con_trai' -> bộ
  // đếm Con về 0 -> hủy Con. Rất mượt!

// ==========================================
// 4. MỨC CHUYÊN GIA: CUSTOM DELETER (GỌI HÀM HỦY TÙY CHỈNH)
// ==========================================
// Đôi khi bạn dùng Smart Pointer để quản lý File (FILE* trong C), hoặc một
// Socket kết nối mạng. Khi đó hàm Hủy mặc định (delete) là sai! Bạn phải dùng
// fclose() hoặc close().

#include <cstdio> // Cho FILE*

void smart_pointer_custom_deleter() {
  std::cout << "\n--- 4. EXPERT: CUSTOM DELETER ---\n";

  // Viết một Lambda đóng File
  auto file_closer = [](FILE *fp) {
    if (fp) {
      std::cout << "  [Custom Deleter] Dang tu dong dong File an toan!\n";
      fclose(fp);
    }
  };

  // Đưa Lambda vào làm Custom Deleter cho unique_ptr
  // Cú pháp: std::unique_ptr<Kiểu, Kiểu_Deleter>
  std::unique_ptr<FILE, decltype(file_closer)> my_file(
      fopen("temp_test.txt", "w"), file_closer);

  if (my_file) {
    std::cout << "  -> Da mo file thanh cong. Du ban co quen dong, Smart "
                 "Pointer cung se lo!\n";
    // Ghi gì đó vào file...
  }
}

// ==========================================
// 5. MỨC BẬC THẦY: ÉP KIỂU VÀ MẢNG (ARRAYS)
// ==========================================
void smart_pointer_arrays_and_casts() {
  std::cout << "\n--- 5. MASTER: SMART POINTER ARRAYS & THANG CAP ---\n";

  // 1. Quản lý MẢNG bằng unique_ptr (Có [] ở hậu tố)
  // Nó sẽ tự động gọi 'delete[]' thay vì 'delete'.
  std::unique_ptr<VuKhi[]> tui_do = std::make_unique<VuKhi[]>(3);
  // Mảng 3 phần tử. Chú ý Constructor mặc định (không tham số) phải tồn tại.
  // Nhưng vì lớp VuKhi không có Constructor mặc định, dòng trên sẽ báo lỗi.

  // 2. Nâng cấp: Có thể CHUYỂN unique_ptr thành shared_ptr (Bằng std::move)
  // Rất hữu ích khi hàm trả về unique_ptr, nhưng người nhận lại muốn dùng
  // shared_ptr.
  std::unique_ptr<VuKhi> u_ptr = std::make_unique<VuKhi>("Cung Ten");
  std::shared_ptr<VuKhi> s_ptr = std::move(u_ptr); // Hợp lệ!

  std::cout << "  -> Da thang cap Cung Ten tu Unique len Shared.\n";

  // LƯU Ý: KHÔNG THỂ làm ngược lại (Không thể từ Shared rớt xuống Unique vì
  // không an toàn).
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  smart_pointer_unique();
  smart_pointer_shared();
  smart_pointer_weak();
  smart_pointer_custom_deleter();

  // Phần mảng yêu cầu VuKhi có Constructor không tham số, nên ta chỉ để comment
  // học thuật ở mục 5 smart_pointer_arrays_and_casts();

  std::cout << "\n--- HOAN THANH TIEU DIET CON TRO THO ---\n";
  return 0;
}