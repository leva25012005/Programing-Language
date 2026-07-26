#include <iostream>
#include <utility> // Sử dụng cho std::move và std::swap

// =========================================================================
// 🧩 1. TÍNH TRỪU TƯỢNG VỚI TỪ KHÓA OVERRIDE VÀ FINAL (C++11)
// =========================================================================
class NhanVien {
public:
  virtual void lamViec() = 0;
  virtual ~NhanVien() { std::cout << "🧹 Huy NhanVien (Base)\n"; }
};

// Lớp LapTrinhVien ghi đè hàm từ lớp cha
class LapTrinhVien : public NhanVien {
public:
  // 💡 Sử dụng 'override' để trình biên dịch kiểm tra tính chính xác khi ghi đè
  void lamViec() override {
    std::cout << "💻 Lap trinh vien dang viet code...\n";
  }
};

// 💡 Sử dụng 'final' để ngăn chặn bất kỳ lớp nào khác kế thừa từ
// LapTrinhVienCapCao
class LapTrinhVienCapCao final : public LapTrinhVien {
public:
  // Sử dụng cả 'override' và 'final' để các lớp sau không thể ghi đè hàm này
  // nữa
  void lamViec() override final {
    std::cout << "🚀 Lap trinh vien cap cao dang thiet ke he thong...\n";
  }
};

// class HuynhTruong : public LapTrinhVienCapCao {}; // ❌ LỖI BIÊN DỊCH! Lớp
// final không được phép kế thừa.

// =========================================================================
// 📦 2. MANG DONG HOÀN THIỆN: RULE OF FIVE & OPERATOR OVERLOADING
// =========================================================================
class MangDong {
private:
  int *data;
  size_t kichThuoc;

public:
  // Constructor khởi tạo
  MangDong(size_t size = 0)
      : kichThuoc{size}, data{size > 0 ? new int[size]{0} : nullptr} {}

  // Destructor
  ~MangDong() { delete[] data; }

  // a. Copy Constructor
  MangDong(const MangDong &other)
      : kichThuoc{other.kichThuoc},
        data{other.kichThuoc > 0 ? new int[other.kichThuoc] : nullptr} {
    for (size_t i = 0; i < kichThuoc; ++i)
      data[i] = other.data[i];
  }

  // b. Move Constructor (Khởi tạo di chuyển)
  MangDong(MangDong &&other) noexcept
      : data{other.data}, kichThuoc{other.kichThuoc} {
    other.data = nullptr;
    other.kichThuoc = 0;
  }

  // c. Copy Assignment Operator (Toán tử gán sao chép)
  MangDong &operator=(const MangDong &other) {
    if (this == &other)
      return *this;
    delete[] data;
    kichThuoc = other.kichThuoc;
    data = other.kichThuoc > 0 ? new int[kichThuoc] : nullptr;
    for (size_t i = 0; i < kichThuoc; ++i)
      data[i] = other.data[i];
    return *this;
  }

  // 🌟 d. MẢNH GHÉP MỚI: Move Assignment Operator (Toán tử gán di chuyển)
  // Dùng khi m2 đã tồn tại và ta muốn: m2 = std::move(m1);
  MangDong &operator=(MangDong &&other) noexcept {
    std::cout << "⚡ Move Assignment Operator duoc goi!\n";
    if (this == &other)
      return *this; // Tránh tự gán chính nó

    delete[] data; // 1. Giải phóng tài nguyên hiện tại của đối tượng bên trái
                   // (m2)

    data =
        other.data; // 2. Chiếm quyền sở hữu vùng nhớ từ đối tượng bên phải (m1)
    kichThuoc = other.kichThuoc;

    other.data = nullptr; // 3. Đưa đối tượng cũ về trạng thái an toàn
    other.kichThuoc = 0;

    return *this;
  }

  // Toán tử truy cập chỉ mục []
  int &operator[](size_t index) { return data[index]; }
  size_t getKichThuoc() const { return kichThuoc; }

  // 🌟 MẢNH GHÉP MỚI: Nạp chồng toán tử xuất (<<) và nhập (>>)
  // Cho phép dùng std::cout << đối_tượng và std::cin >> đối_tượng
  friend std::ostream &operator<<(std::ostream &os, const MangDong &arr) {
    os << "[ ";
    for (size_t i = 0; i < arr.kichThuoc; ++i) {
      os << arr.data[i] << " ";
    }
    os << "]";
    return os; // Trả về dòng xuất để có thể in liên tiếp (cout << a << b;)
  }

  friend std::istream &operator>>(std::istream &is, MangDong &arr) {
    for (size_t i = 0; i < arr.kichThuoc; ++i) {
      std::cout << "Nhap phan tu thu " << i << ": ";
      is >> arr.data[i];
    }
    return is;
  }
};

// =========================================================================
// 🚀 HÀM CHÍNH (MAIN)
// =========================================================================
int main() {
  std::cout << "--- 1. Kiem tra override / final ---\n";
  NhanVien *nv = new LapTrinhVienCapCao();
  nv->lamViec();
  delete nv;
  std::cout << "------------------------------------\n\n";

  std::cout << "--- 2. Kiem tra Toan tu Nhap / Xuat ---\n";
  MangDong m1(3);
  m1[0] = 10;
  m1[1] = 20;
  m1[2] = 30;

  // Giờ đây ta có thể in trực tiếp bằng std::cout thông qua toán tử << đã nạp
  // chồng!
  std::cout << "Mang m1 ban dau: " << m1 << "\n\n";

  std::cout << "--- 3. Kiem tra Toan tu gan di chuyen (Move Assignment) ---\n";
  MangDong m2(2); // m2 đã được khởi tạo trước đó với kích thước 2
  m2[0] = 99;
  m2[1] = 88;
  std::cout << "Mang m2 truoc khi gan: " << m2 << "\n";

  // Kích hoạt toán tử gán di chuyển (Move Assignment)
  m2 = std::move(m1);

  std::cout << "Mang m2 sau khi nhan di chuyen tu m1: " << m2 << "\n";
  std::cout << "Mang m1 sau khi bi di chuyen (an toan): " << m1 << "\n";

  return 0;
}