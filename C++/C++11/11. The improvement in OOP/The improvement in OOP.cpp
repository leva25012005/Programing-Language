#include <iostream>
#include <string>

// ==========================================
// PHẦN 1: TỪ KHÓA OVERRIDE VÀ FINAL (C++11)
// ==========================================
namespace OverrideFinal {

// 1. LỚP CHA (Base Class)
class DongVat {
public:
  virtual ~DongVat() = default; // Luôn dùng virtual destructor cho lớp cha

  // Hàm ảo mong muốn lớp con ghi đè
  virtual void tao_am_thanh() const { std::cout << "Dong vat keu...\n"; }

  // Hàm ảo bình thường
  virtual void di_chuyen() { std::cout << "Dong vat di chuyen...\n"; }
};

// 2. TỪ KHÓA OVERRIDE
class Cho : public DongVat {
public:
  // override: Cảnh sát chính tả!
  // Nếu lớp cha đổi tên hàm thành tao_am_thanh_2(), hoặc bạn quên chữ 'const',
  // C++11 sẽ BÁO LỖI BIÊN DỊCH ngay lập tức thay vì âm thầm tạo ra một hàm mới.
  void tao_am_thanh() const override { std::cout << "Cho: Gau Gau!\n"; }

  // void tao_am_thanh() override {} // LỖI BIÊN DỊCH! Thiếu chữ 'const' so với
  // lớp cha void tao_an_thanh() const override {} // LỖI BIÊN DỊCH! Gõ nhầm chữ
  // 'm' thành 'n'

  // 3. TỪ KHÓA FINAL (Trên Hàm)
  // Lớp Cho quyết định hàm di_chuyen() là hoàn hảo,
  // cấm tất cả các lớp con của Cho (như ChoSanh) ghi đè hàm này nữa.
  void di_chuyen() override final { std::cout << "Cho chay bang 4 chan.\n"; }
};

class ChoSanh : public Cho {
public:
  // void di_chuyen() override {} // LỖI BIÊN DỊCH! Hàm di_chuyen ở lớp Cho đã
  // bị 'final' cấm!
};

// 4. TỪ KHÓA FINAL (Trên Lớp)
// Cấm bất kỳ ai kế thừa từ lớp Meo. Meo là tận cùng của gia phả.
class Meo final : public DongVat {
public:
  void tao_am_thanh() const override { std::cout << "Meo: Meo Meo!\n"; }
};

// class MeoRung : public Meo {}; // LỖI BIÊN DỊCH! Lớp Meo đã được chốt
// 'final'.

void demo() {
  std::cout << "\n--- 1. OVERRIDE & FINAL ---\n";
  Cho c;
  c.tao_am_thanh();
  c.di_chuyen();
  Meo m;
  m.tao_am_thanh();
}
} // namespace OverrideFinal

// ==========================================
// PHẦN 2: = DEFAULT VÀ = DELETE (C++11)
// ==========================================
namespace DefaultDelete {

class TaiKhoanNganHang {
private:
  std::string chu_tai_khoan;
  double so_du;

public:
  // 1. TỪ KHÓA = DEFAULT
  // Khi bạn tự viết một Constructor có tham số, C++ sẽ TỰ ĐỘNG XÓA Constructor
  // mặc định (không tham số). Nếu muốn lấy lại nó, thay vì viết { } rỗng, hãy
  // dùng '= default'. Giúp code chạy tối ưu hơn và rõ ràng ý đồ của lập trình
  // viên.
  TaiKhoanNganHang() = default;

  TaiKhoanNganHang(std::string ten, double tien)
      : chu_tai_khoan(ten), so_du(tien) {}

  // 2. TỪ KHÓA = DELETE (Chống Copy)
  // Cực kỳ hữu ích để ngăn chặn người khác sao chép (copy) đối tượng này.
  // Tài khoản ngân hàng là duy nhất, không thể lỡ tay copy ra một tài khoản y
  // hệt!
  TaiKhoanNganHang(const TaiKhoanNganHang &other) =
      delete; // Cấm Copy Constructor
  TaiKhoanNganHang &
  operator=(const TaiKhoanNganHang &other) = delete; // Cấm Phép gán Copy

  // 3. TỪ KHÓA = DELETE (Chống ép kiểu ngầm định - Chuyên gia)
  // Chỉ cho phép nạp tiền bằng số nguyên (int), CẤM nạp số thực (double) để
  // tránh sai số.
  void nap_tien(int so_tien) {
    so_du += so_tien;
    std::cout << "Da nap: " << so_tien << "\n";
  }
  void
  nap_tien(double) = delete; // Nạp số thập phân sẽ báo Lỗi Biên Dịch lập tức!

  void in_thong_tin() const {
    std::cout << "TK: " << chu_tai_khoan << " | So du: " << so_du << "\n";
  }
};

void demo() {
  std::cout << "\n--- 2. DEFAULT & DELETE ---\n";

  TaiKhoanNganHang tk1("Le Van Nhan", 1000);

  // TaiKhoanNganHang tk2 = tk1; // LỖI BIÊN DỊCH! Copy Constructor đã bị '=
  // delete'

  tk1.nap_tien(500); // Hợp lệ (int)
  // tk1.nap_tien(50.5); // LỖI BIÊN DỊCH! Hàm nạp double đã bị '= delete'

  tk1.in_thong_tin();
}
} // namespace DefaultDelete

// ==========================================
// PHẦN 3: ỦY THÁC HÀM KHỞI TẠO (DELEGATING CONSTRUCTORS)
// ==========================================
namespace Delegating {

class HinhChuNhat {
private:
  int chieu_dai;
  int chieu_rong;
  std::string mau_sac;

public:
  // 1. CONSTRUCTOR GỐC (Chứa logic chính)
  // Nơi duy nhất xử lý cấp phát, gán giá trị, kiểm tra lỗi...
  HinhChuNhat(int d, int r, std::string m) {
    if (d <= 0 || r <= 0) {
      chieu_dai = 1;
      chieu_rong = 1; // Giá trị an toàn
    } else {
      chieu_dai = d;
      chieu_rong = r;
    }
    mau_sac = m;
    std::cout << "  [Tao Hinh] " << chieu_dai << "x" << chieu_rong
              << " - Mau: " << mau_sac << "\n";
  }

  // 2. CONSTRUCTOR ỦY THÁC (Delegating)
  // Thay vì phải viết lại đoạn code gán biến (hoặc gọi hàm init() như C++98),
  // Nó gọi thẳng Constructor Gốc thông qua Danh sách Khởi tạo (Initializer
  // List).

  // Tạo Hình vuông (Chỉ truyền 1 cạnh) -> Gọi Constructor 3 tham số
  HinhChuNhat(int canh) : HinhChuNhat(canh, canh, "Trang") {
    // Code ở đây sẽ chạy SAU KHI Constructor Gốc chạy xong.
    std::cout << "  -> (Day la hinh vuong dac biet)\n";
  }

  // Tạo Hình mặc định (Không tham số) -> Gọi Constructor 3 tham số
  HinhChuNhat() : HinhChuNhat(10, 5, "Den") {
    std::cout << "  -> (Day la hinh mac dinh)\n";
  }
};

void demo() {
  std::cout << "\n--- 3. DELEGATING CONSTRUCTORS ---\n";

  std::cout << "Goi Constructor mac dinh:\n";
  HinhChuNhat h1;

  std::cout << "\nGoi Constructor 1 tham so (Hinh vuong):\n";
  HinhChuNhat h2(20);

  std::cout << "\nGoi Constructor day du:\n";
  HinhChuNhat h3(15, 8, "Do");
}
} // namespace Delegating

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== OOP MODERN C++ FEATURES =====\n";

  OverrideFinal::demo();
  DefaultDelete::demo();
  Delegating::demo();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}