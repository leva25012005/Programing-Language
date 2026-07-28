#include <iostream>
#include <utility> // Sử dụng cho std::move

// =========================================================================
// 📦 1. QUẢN LÝ TÀI NGUYÊN (RAII) & NẠP CHỒNG TOÁN TỬ & THÀNH VIÊN TĨNH
// =========================================================================
class MangDong {
private:
  int *data;
  size_t kichThuoc;

public:
  // Thành viên tĩnh (Static Member) để đếm số lượng đối tượng đang tồn tại
  static int soLuongMang;

  // Constructor khởi tạo thông thường
  MangDong(size_t size) : kichThuoc{size}, data{new int[size]{0}} {
    soLuongMang++;
  }

  // Destructor giải phóng bộ nhớ
  ~MangDong() {
    delete[] data;
    soLuongMang--;
  }

  // 🌟 BỘ BA/BỘ NĂM QUYỀN LỰC (Rule of Three/Five)

  // a. Copy Constructor (Deep Copy)
  MangDong(const MangDong &other)
      : kichThuoc{other.kichThuoc}, data{new int[other.kichThuoc]} {
    for (size_t i = 0; i < kichThuoc; ++i)
      data[i] = other.data[i];
    soLuongMang++;
    std::cout << "👥 Copy Constructor: Da sao chep vung nho.\n";
  }

  // b. Move Constructor (Chuyển giao tài nguyên - C++11)
  MangDong(MangDong &&other) noexcept
      : data{other.data}, kichThuoc{other.kichThuoc} {
    other.data = nullptr; // Xóa con trỏ ở đối tượng cũ để tránh giải phóng nhầm
    other.kichThuoc = 0;
    soLuongMang++;
    std::cout << "⚡ Move Constructor: Da di chuyen tai nguyen.\n";
  }

  // c. Copy Assignment Operator (Toán tử gán copy)
  MangDong &operator=(const MangDong &other) {
    if (this == &other)
      return *this; // Tự gán
    delete[] data;  // Xóa tài nguyên cũ
    kichThuoc = other.kichThuoc;
    data = new int[kichThuoc];
    for (size_t i = 0; i < kichThuoc; ++i)
      data[i] = other.data[i];
    return *this;
  }

  // 🔀 NẠP CHỒNG TOÁN TỬ (Operator Overloading)
  // Nạp chồng toán tử index [] để truy cập phần tử như mảng thông thường
  int &operator[](size_t index) { return data[index]; }

  // 🤝 CƠ CHẾ BẠN BÈ (Friend Function)
  // Hàm inMang không phải là thành viên nhưng có quyền truy cập vùng private
  friend void inMang(const MangDong &arr);
};

// Khởi tạo thành viên tĩnh
int MangDong::soLuongMang = 0;

// Định nghĩa hàm bạn
void inMang(const MangDong &arr) {
  std::cout << "[Friend] Mang co " << arr.kichThuoc << " phan tu: ";
  for (size_t i = 0; i < arr.kichThuoc; ++i) {
    std::cout << arr.data[i] << " ";
  }
  std::cout << "\n";
}

// =========================================================================
// 💎 2. KẾ THỪA NÂNG CAO - GIẢI QUYẾT BÀI TOÁN KIM CƯƠNG (DIAMOND PROBLEM)
// =========================================================================
class ThietBi {
public:
  void chay() { std::cout << "⚙️ Thiet bi dang hoat dong.\n"; }
};

// Sử dụng từ khóa 'virtual' để tránh việc tạo ra 2 bản sao của ThietBi trong
// MayTinhBang
class MayAnh : virtual public ThietBi {};
class DienThoai : virtual public ThietBi {};

class MayTinhBang : public MayAnh, public DienThoai {};

// =========================================================================
// 🚀 HÀM CHÍNH (MAIN)
// =========================================================================
int main() {
  std::cout << "--- 1. Kiem tra Quan ly tai nguyen & Luu tru tinh ---\n";
  MangDong m1(3);
  m1[0] = 10;
  m1[1] = 20;
  m1[2] = 30; // Sử dụng toán tử [] đã nạp chồng

  MangDong m2 = std::move(m1); // Kích hoạt Move Constructor
  inMang(m2);                  // Gọi hàm bạn để in dữ liệu
  std::cout << "So luong mang hien tai: " << MangDong::soLuongMang << "\n\n";

  std::cout << "--- 2. Kiem tra Bai toan Kim cuong ---\n";
  MayTinhBang mtb;
  mtb.chay(); // Hợp lệ! Nhờ 'virtual inheritance', ham chay() khong bi mo ho
              // (ambiguous)

  return 0;
}