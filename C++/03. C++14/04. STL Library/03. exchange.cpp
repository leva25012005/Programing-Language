#include <iostream>
#include <string>
#include <utility> // Bắt buộc: Chứa std::exchange, std::move
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. std::exchange là gì?
    - Ra mắt từ C++14, nằm trong thư viện <utility>.
    - Cú pháp: std::exchange(bien_cu, gia_tri_moi);
    - Chức năng: Gán 'gia_tri_moi' vào 'bien_cu', đồng thời TRẢ VỀ giá trị ban
 đầu của 'bien_cu'.
    - Tất cả diễn ra trong đúng 1 thao tác duy nhất.

 2. Bản chất dưới nền (Mã nguồn thực tế của C++):
    template<class T, class U = T>
    T exchange(T& obj, U&& new_val) {
        T old_val = std::move(obj);      // Lưu lại giá trị cũ
        obj = std::forward<U>(new_val);  // Gán giá trị mới vào
        return old_val;                  // Trả về giá trị cũ
    }

 3. Sứ mệnh lớn nhất:
    - Rút ngắn code khi thiết lập lại trạng thái (Reset state).
    - LÀM SẠCH VÀ TỐI ƯU Move Constructor / Move Assignment (Tránh việc quên gán
 con trỏ về nullptr).
*/

// ==========================================
// 1. MỨC CƠ BẢN: THAY THẾ VÀ LẤY LẠI GIÁ TRỊ CŨ
// ==========================================
void exchange_co_ban() {
  std::cout << "\n--- 1. BAZIC: THAY THE VA TRA VE ---\n";

  int hien_tai = 100;
  std::cout << "Gia tri ban dau: " << hien_tai << "\n";

  // Truyền thống (Tốn 3 dòng code):
  // int tam = hien_tai;
  // hien_tai = 200;
  // int qua_khu = tam;

  // Hiện đại với std::exchange (1 dòng duy nhất):
  int qua_khu = std::exchange(hien_tai, 200);

  std::cout << "Gia tri sau khi exchange: " << hien_tai << "\n";
  std::cout << "Gia tri nhan duoc (qua khu): " << qua_khu << "\n";
}

// ==========================================
// 2. MỨC TRUNG CẤP: ỨNG DỤNG TRONG LOGIC TRẠNG THÁI (STATE)
// ==========================================
void exchange_ung_dung_logic() {
  std::cout << "\n--- 2. INTERMEDIATE: LOGIC TRANG THAI ---\n";

  bool is_first_time = true;

  for (int i = 1; i <= 3; ++i) {
    std::cout << "Lan lap thu " << i << ": ";

    // Tuyệt chiêu thực thi ĐÚNG 1 LẦN:
    // std::exchange(is_first_time, false) sẽ gán is_first_time = false cho các
    // lần sau, NHƯNG ở lần lặp đầu tiên, nó vẫn trả về 'true' để lọt vào vòng
    // if!
    if (std::exchange(is_first_time, false)) {
      std::cout << "Day la lan DAU TIEN chay doan code nay!\n";
    } else {
      std::cout << "Doan code nay da tro thanh binh thuong.\n";
    }
  }

  // Ứng dụng duyệt mảng dạng chuỗi: In phần tử trước đó
  int so_truoc_do = 0;
  std::vector<int> danh_sach = {10, 20, 30};
  std::cout << "\nLuu vet gia tri cu: \n";
  for (int so_hien_tai : danh_sach) {
    std::cout << "  - Truoc do la: " << std::exchange(so_truoc_do, so_hien_tai)
              << ", Hien tai la: " << so_hien_tai << "\n";
  }
}

// ==========================================
// 3. MỨC CHUYÊN GIA: MOVE SEMANTICS (VŨ KHÍ TỐI THƯỢNG)
// ==========================================
// Giả lập một class tự quản lý bộ nhớ động (Con trỏ thô)
class HinhAnh {
private:
  int *data_pixel;
  size_t kich_thuoc;

public:
  // Constructor khởi tạo
  HinhAnh(size_t size) : kich_thuoc(size) {
    data_pixel = new int[size];
    std::cout << "  [Tao] Cap phat " << size << " pixels.\n";
  }

  // Destructor (Chống Memory Leak)
  ~HinhAnh() {
    if (data_pixel != nullptr) {
      delete[] data_pixel;
      std::cout << "  [Huy] Giai phong bo nho.\n";
    }
  }

  // ========================================================
  // MA THUẬT NẰM Ở ĐÂY: MOVE CONSTRUCTOR (CƯỚP TÀI NGUYÊN)
  // ========================================================
  HinhAnh(HinhAnh &&other) noexcept
      // Thay vì phải viết dài dòng ở trong ngoặc nhọn:
      // this->data_pixel = other.data_pixel;
      // other.data_pixel = nullptr;  <-- Rất hay quên dòng này, dẫn đến Crash
      // khi Destructor chạy (Double Free)!

      // Dùng std::exchange khởi tạo ngay trên Initialization List:
      : data_pixel(std::exchange(other.data_pixel, nullptr)),
        kich_thuoc(std::exchange(other.kich_thuoc, 0)) {
    std::cout << "  [Move Constructor] Da cuop tai nguyen thanh cong!\n";
  }

  // ========================================================
  // TƯƠNG TỰ VỚI MOVE ASSIGNMENT OPERATOR
  // ========================================================
  HinhAnh &operator=(HinhAnh &&other) noexcept {
    if (this != &other) {
      // 1. Dọn dẹp tài nguyên cũ của chính mình trước khi đi cướp
      delete[] data_pixel;

      // 2. Cướp tài nguyên của 'other' và an toàn khóa 'other' lại bằng
      // std::exchange
      data_pixel = std::exchange(other.data_pixel, nullptr);
      kich_thuoc = std::exchange(other.kich_thuoc, 0);

      std::cout << "  [Move Assignment] Da xoa cu, cuop moi thanh cong!\n";
    }
    return *this;
  }

  // Ngăn chặn Copy để tập trung test Move
  HinhAnh(const HinhAnh &) = delete;
  HinhAnh &operator=(const HinhAnh &) = delete;
};

void exchange_trong_move_semantics() {
  std::cout << "\n--- 3. EXPERT: STD::EXCHANGE TRONG MOVE CONSTRUCTOR ---\n";

  std::cout << "> Khoi tao HinhAnh A:\n";
  HinhAnh img_A(1000);

  std::cout << "\n> Move HinhAnh A sang HinhAnh B:\n";
  // img_A sẽ bị cướp data. data_pixel của img_A sẽ bị std::exchange gán thành
  // nullptr.
  HinhAnh img_B = std::move(img_A);

  std::cout << "\n> Khoi tao HinhAnh C va Move Assignment tu B sang C:\n";
  HinhAnh img_C(500);
  img_C = std::move(img_B);

  std::cout << "\n> Ket thuc ham, cac Destructor bat dau chay (Chi co C con "
               "data de xoa, A va B da la nullptr nen khong bi Crash):\n";
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  exchange_co_ban();
  exchange_ung_dung_logic();
  exchange_trong_move_semantics();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}