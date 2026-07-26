#include <iostream>
#include <string>

// 📑 Khai báo lớp cơ sở (Base Class)
class TaiKhoanNganHang {
  // 🔒 Private: Chỉ các phương thức bên trong lớp này mới được truy cập trực
  // tiếp
private:
  std::string chuTaiKhoan;
  double soDu;

  // 🛡️ Protected: Lớp này và các lớp con (kế thừa) được phép truy cập
protected:
  std::string maSoBiMat;

  // 🔓 Public: Bất kỳ ai (kể cả hàm main) cũng có thể truy cập thông qua đối
  // tượng
public:
  // 🏗️ Hàm khởi tạo (Constructor) - Sinh ra đối tượng
  TaiKhoanNganHang(std::string ten, double tienNap, std::string maMat)
      : chuTaiKhoan{ten}, soDu{tienNap}, maSoBiMat{maMat} {
    std::cout << "🏗️ [Constructor] Tai khoan cua " << chuTaiKhoan
              << " da duoc tao.\n";
  }

  // 🧹 Hàm hủy (Destructor) - Giải phóng đối tượng khi ra khỏi phạm vi (scope)
  ~TaiKhoanNganHang() {
    std::cout << "🧹 [Destructor] Tai khoan cua " << chuTaiKhoan
              << " da duoc dong an toan.\n";
  }

  // ⚙️ Phương thức (Member Function) - Hành vi của lớp
  void guiTien(double soTien) {
    if (soTien > 0) {
      soDu += soTien;
      std::cout << "💰 Da gui " << soTien
                << " vao tai khoan. So du hien tai: " << soDu << "\n";
    }
  }

  void inThongTin() const {
    std::cout << "👤 Chu tai khoan: " << chuTaiKhoan << " | So du: " << soDu
              << "\n";
  }
};

// 👪 Lớp con kế thừa từ TaiKhoanNganHang để minh họa phạm vi 'protected'
class TaiKhoanVip : public TaiKhoanNganHang {
public:
  TaiKhoanVip(std::string ten, double tienNap, std::string maMat)
      : TaiKhoanNganHang(ten, tienNap, maMat) {}

  void tietLoMaBiMat() {
    // Hợp lệ vì maSoBiMat là 'protected', lớp con có thể truy cập trực tiếp
    std::cout << "🔑 [VIP] Ma so bi mat la: " << maSoBiMat << "\n";
  }
};

// 🚀 Hàm chính của chương trình
int main() {
  std::cout << "--- Khoi tao doi tuong ---\n";
  // Khởi tạo đối tượng 'tk' từ bản thiết kế Lớp 'TaiKhoanNganHang'
  TaiKhoanNganHang tk("Nguyen Van A", 1000.0, "1234-XYZ");

  // Sử dụng các phương thức public
  tk.inThongTin();
  tk.guiTien(500.0);

  // ❌ Lệnh dưới đây sẽ bị LỖI BIÊN DỊCH nếu bỏ comment vì soDu la private:
  // tk.soDu = 999999.0;

  std::cout << "\n--- Minh hoa lop con (Kethua & Protected) ---\n";
  TaiKhoanVip tkVip("Tran Thi B", 5000.0, "VIP-999");
  tkVip.tietLoMaBiMat();

  std::cout << "\n--- Ket thuc chuong trinh ---\n";
  return 0;
}