#include <iostream>
#include <string>

// =========================================================================
// 🧩 1. TÍNH TRỪU TƯỢNG (ABSTRACTION)
// =========================================================================
// Lớp trừu tượng (Abstract Class) đóng vai trò làm khung thiết kế chuẩn
class NhanVien {
public:
  // Hàm ảo thuần túy (= 0), bắt buộc các lớp con phải tự định nghĩa lại
  virtual void lamViec() = 0;
  virtual void tinhLuong() = 0;

  // ⚠️ Luôn cần hàm hủy ảo (Virtual Destructor) khi dùng đa hình
  virtual ~NhanVien() {
    std::cout << "🧹 [Destructor] Da giai phong NhanVien (Base).\n";
  }
};

// Khai báo trước (Forward Declaration) lớp BanKiemToan để dùng trong lớp
// LapTrinhVien
class BanKiemToan;

// =========================================================================
// 🔒 2. TÍNH ĐÓNG GÓI (ENCAPSULATION) & QUYỀN BẠN BÈ (FRIEND)
// =========================================================================
class LapTrinhVien : public NhanVien {
private:
  std::string ten;
  double luongCoBan; // Dữ liệu nhạy cảm được giấu kín

protected:
  std::string maDuAn; // Lớp con kế thừa từ LapTrinhVien sẽ truy cập được

public:
  LapTrinhVien(std::string t, double luong, std::string duAn)
      : ten{t}, luongCoBan{luong}, maDuAn{duAn} {}

  ~LapTrinhVien() override {
    std::cout << "🧹 [Destructor] Da giai phong LapTrinhVien: " << ten << ".\n";
  }

  // 🎭 Ghi đè phương thức (Tính đa hình)
  void lamViec() override {
    std::cout << "💻 " << ten << " dang code tinh nang moi cho du an " << maDuAn
              << ".\n";
  }

  void tinhLuong() override {
    std::cout << "💰 Luong nhan duoc: " << luongCoBan << " USD.\n";
  }

  // 🤝 Khai báo Lớp Bạn (Friend Class)
  friend class BanKiemToan;
};

// Lớp bạn có quyền vào vùng private của LapTrinhVien
class BanKiemToan {
public:
  void kiemTraThuNhap(const LapTrinhVien &ltv) {
    // Truy cập thẳng vào 'luongCoBan' dù nó là private
    std::cout << "🔍 [Kiem Toan] Xac minh thu nhap hop phap: " << ltv.luongCoBan
              << " USD.\n";
  }
};

// =========================================================================
// 👪 3. TÍNH KẾ THỪA (INHERITANCE)
// =========================================================================
// Lớp LapTrinhVienCapCao kế thừa từ LapTrinhVien
class LapTrinhVienCapCao : public LapTrinhVien {
private:
  double luongThuong;

public:
  LapTrinhVienCapCao(std::string t, double luong, std::string duAn,
                     double thuong)
      : LapTrinhVien(t, luong, duAn), luongThuong{thuong} {}

  // Truy cập được 'maDuAn' của lớp cha vì nó là protected
  void quanLyHeThong() {
    std::cout << "🚀 Dang toi uu kien truc he thong cho du an: " << maDuAn
              << "\n";
  }
};

// =========================================================================
// 🚀 4. TÍNH ĐA HÌNH (POLYMORPHISM) TRONG HÀM MAIN
// =========================================================================
int main() {
  // 🎭 Sử dụng con trỏ lớp cha (NhanVien*) trỏ đến đối tượng lớp con
  // (LapTrinhVien)
  NhanVien *nhanVien = new LapTrinhVien("Minh C++", 3000, "AI-Project");

  // Tại Runtime, chương trình tự nhận biết để gọi đúng hàm của lớp con
  nhanVien->lamViec();
  nhanVien->tinhLuong();

  std::cout << "\n--- Kiem tra quyen ban be (Friend Class) ---\n";
  LapTrinhVien *ltvThuThuong = dynamic_cast<LapTrinhVien *>(nhanVien);
  if (ltvThuThuong) {
    BanKiemToan kt;
    kt.kiemTraThuNhap(*ltvThuThuong);
  }

  std::cout << "\n--- Giai phong bo nho ---\n";
  delete nhanVien; // Nhờ virtual destructor, cả lớp con và lớp cha đều được hủy
                   // an toàn

  return 0;
}