#include <initializer_list> // Thư viện cốt lõi cho initializer_list
#include <iostream>
#include <string>
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Sự ra đời (C++11):
    Trước C++11, việc khởi tạo biến/mảng/object rất lộn xộn (dùng (), dùng =,
 dùng {}). C++11 đưa ra khái niệm "Uniform Initialization" (Khởi tạo đồng nhất),
 khuyên dùng {} cho MỌI THỨ.

 2. std::initializer_list<T> là gì?
    - Khi bạn viết {1, 2, 3, 4}, trình biên dịch sẽ tự động gom chúng lại thành
      một đối tượng tạm thời mang kiểu `std::initializer_list<int>`.
    - Dưới nền (Under the hood): Nó CỰC KỲ NHẸ. Bản chất nó chỉ chứa 2 con trỏ:
      Một con trỏ trỏ tới phần tử đầu tiên của mảng tạm trên Stack, và một biến
 lưu kích thước.
    - Truyền vào hàm: Vì nó rất nhẹ (thường chỉ 16 bytes), bạn NÊN truyền nó
 dưới dạng Giá trị (Pass by Value), không cần truyền Tham chiếu (Pass by const
 reference).

 3. Điểm mạnh lớn nhất:
    - Chống lỗi ép kiểu ngầm định làm mất dữ liệu (Narrowing Conversions).
    - Cung cấp cú pháp nhét hàng loạt dữ liệu vào Object cực kỳ thanh lịch.
*/

// ==========================================
// CLASS PHỤ TRỢ CHO PHẦN NÂNG CAO
// ==========================================
class GioHang {
private:
  std::vector<std::string> ds_san_pham;

public:
  // Constructor mặc định
  GioHang() { std::cout << "  [Tao] Gio hang rong\n"; }

  // Constructor nhận std::initializer_list (Vũ khí tối thượng của C++11)
  GioHang(std::initializer_list<std::string> danh_sach) {
    std::cout << "  [Tao] Gio hang tu Initializer List co " << danh_sach.size()
              << " mon:\n";

    // Vì initializer_list cung cấp begin() và end(), ta có thể dùng vòng lặp
    // for dễ dàng
    for (const auto &mon : danh_sach) {
      std::cout << "    + " << mon << "\n";
      ds_san_pham.push_back(mon);
    }
  }

  // Nạp chồng toán tử gán (Assignment Operator) bằng initializer_list
  GioHang &operator=(std::initializer_list<std::string> danh_sach) {
    std::cout << "  [Gan] Ghi de gio hang bang danh sach moi!\n";
    ds_san_pham.clear();
    for (const auto &mon : danh_sach) {
      ds_san_pham.push_back(mon);
    }
    return *this;
  }
};

// ==========================================
// 1. MỨC CƠ BẢN: UNIFORM INITIALIZATION & NARROWING
// ==========================================
void init_co_ban() {
  std::cout << "\n--- 1. BAZIC: KHOI TAO DONG NHAT & NARROWING ---\n";

  // Khởi tạo mọi thứ bằng Ngoặc Nhọn {}
  int a{10};                   // Khởi tạo biến int
  int mang[]{1, 2, 3};         // Khởi tạo mảng tĩnh
  std::vector<int> v{1, 2, 3}; // Khởi tạo Vector

  // SỰ BẢO VỆ CỦA NGOẶC NHỌN (Ngăn chặn Narrowing Conversion)
  double pi = 3.14159;

  // int x = pi;     // HỢP LỆ kiểu C cũ: Trình biên dịch im lặng xén mất đuôi,
  // x = 3 (RẤT NGUY HIỂM) int y(pi);      // HỢP LỆ kiểu C++ cũ: Vẫn xén mất
  // đuôi, y = 3

  // int z{pi};      // LỖI BIÊN DỊCH TRỰC TIẾP!
  // Dấu {} báo lỗi: "Không thể nhét 3.14 vào int vì sẽ mất dữ liệu!"

  std::cout
      << "Khuyen cao: Hay luon dung {} de khoi tao bien tu C++11 tro di!\n";
}

// ==========================================
// 2. MỨC TRUNG CẤP: HÀM NHẬN INITIALIZER_LIST
// ==========================================
// Lưu ý: Truyền thẳng bằng Giá trị (By Value) vì nó chỉ chứa 2 con trỏ, rất
// nhẹ.
int tinh_tong(std::initializer_list<int> danh_sach_so) {
  int tong = 0;
  // Dùng Range-based for loop
  for (int so : danh_sach_so) {
    tong += so;
  }
  return tong;
}

void init_ham_truyen_du_lieu() {
  std::cout << "\n--- 2. INTERMEDIATE: TRUYEN PARAMETER HANG LOAT ---\n";

  // Thay vì phải tạo 1 mảng/vector rồi truyền vào, ta ném thẳng {} vào hàm!
  int ket_qua = tinh_tong({10, 20, 30, 40, 50});

  std::cout << "Tong = " << ket_qua << "\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: TÍCH HỢP VÀO CLASS CỦA BẠN
// ==========================================
void init_class_constructor() {
  std::cout << "\n--- 3. ADVANCED: CUSTOM CLASS CONSTRUCTOR ---\n";

  // Gọi constructor có tham số std::initializer_list
  GioHang cart1 = {"Chuoi", "Tao", "Cam"};

  GioHang cart2; // Gọi default constructor

  // Gọi Assignment Operator với std::initializer_list
  cart2 = {"Laptop", "Chuot", "Ban Phim"};
}

// ==========================================
// 4. MỨC CHUYÊN GIA: CẠM BẪY ĐỘ ƯU TIÊN (HIJACKING)
// ==========================================
void init_cam_bay_uu_tien() {
  std::cout
      << "\n--- 4. EXPERT: CONSTRUCTOR HIJACKING (CAM BAY KINH DIEN) ---\n";

  // QUY TẮC CỨNG: Nếu Class của bạn có một Constructor nhận
  // std::initializer_list, Trình biên dịch sẽ LUÔN ƯU TIÊN gọi nó nếu bạn dùng
  // ngoặc nhọn {}, MẶC KỆ bạn có các constructor khác khớp tham số hay không!

  // Ví dụ kinh điển với std::vector:

  // Dùng ngoặc tròn (): Gọi constructor tạo ra 5 phần tử, mỗi phần tử có giá
  // trị 10.
  std::vector<int> v1(5, 10);
  std::cout << "v1(5, 10) co size: " << v1.size() << " -> Phan tu: ";
  for (int x : v1)
    std::cout << x << " ";
  std::cout << "\n";

  // Dùng ngoặc nhọn {}: Trình biên dịch thấy {}, nó LẬP TỨC bỏ qua constructor
  // (size, value) Nó nhảy vào gọi constructor std::initializer_list! Hậu quả:
  // Tạo ra vector có đúng 2 phần tử là số 5 và số 10.
  std::vector<int> v2{5, 10};
  std::cout << "v2{5, 10} co size: " << v2.size() << " -> Phan tu: ";
  for (int x : v2)
    std::cout << x << " ";
  std::cout << "\n";

  std::cout << "=> BAI HOC: Can than khi dung {} voi Vector/String neu ban "
               "muon chi dinh Kich thuoc!\n";
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  init_co_ban();
  init_ham_truyen_du_lieu();
  init_class_constructor();
  init_cam_bay_uu_tien();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}