#include <iostream>
#include <map>
#include <string>
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Tính năng này là gì?
    - Ra mắt ở C++20. Cú pháp:
      for ( [Khởi_tạo_biến_tạm] ; [Khai_báo_duyệt] : [Vùng_dữ_liệu] ) { ... }

 2. Tại sao nó ra đời? Giải quyết 2 bài toán lớn:
    - Bài toán 1 (Scope Leak): Giữ cho các biến phụ trợ (như biến đếm index,
 hoặc mảng tạm) chỉ sống BÊN TRONG vòng lặp. Thoát vòng lặp là nó tự hủy, không
 làm rác code bên ngoài.
    - Bài toán 2 (Dangling Reference): Sửa lỗi kinh điển khi duyệt qua thuộc
 tính của một Object tạm thời (Temporary Object) bị hủy quá sớm gây Crash chương
 trình.
*/

// ==========================================
// CLASS PHỤ TRỢ ĐỂ DEMO LỖI (Dành cho Phần 3)
// ==========================================
class TuiDo {
private:
  std::vector<std::string> danh_sach;

public:
  TuiDo() {
    std::cout << "  [Tui Do] Duoc tao ra trong RAM.\n";
    danh_sach = {"Kiem", "Khien", "Binh Mau"};
  }
  ~TuiDo() { std::cout << "  [Tui Do] Bị tieu huy, xoa khoi RAM!\n"; }

  // Hàm này trả về THAM CHIẾU đến mảng bên trong object
  const std::vector<std::string> &layDanhSach() const { return danh_sach; }
};

// Hàm tạo ra một Object tạm thời (Temporary / Rvalue)
TuiDo taoTuiDoAo() { return TuiDo(); }

std::vector<int> layDuLieuTuMang() { return {10, 20, 30, 40, 50}; }

// ==========================================
// 1. MỨC CƠ BẢN: KIỂM SOÁT PHẠM VI (SCOPE) & BIẾN ĐẾM INDEX
// ==========================================
void cpp20_co_ban() {
  std::cout << "\n--- 1. BAZIC: KHOI TAO BIEN DEM INDEX ---\n";

  std::vector<std::string> ten = {"Nhan", "An", "Binh"};

  // TRƯỚC C++20: Bạn phải khai báo biến đếm 'i' ở ngoài.
  // Thoát vòng lặp rồi, biến 'i' vẫn còn sống sờ sờ gây rác bộ nhớ/trùng tên.
  // int i = 0;
  // for (const auto& t : ten) { ... ++i; }

  // TỪ C++20: Đưa thẳng biến 'i' vào phần Init-statement của vòng lặp
  std::cout << "Duyet kem theo Index:\n";
  for (int i = 1; const auto &t : ten) {
    std::cout << "  STT " << i++ << ": " << t << "\n";
  }
  // Ngay tại dòng này, biến 'i' đã bị HỦY hoàn toàn. Code cực kỳ sạch sẽ!
}

// ==========================================
// 2. MỨC TRUNG CẤP: KHỞI TẠO CONTAINER TẠM THỜI
// ==========================================
void cpp20_trung_cap_khoi_tao_mang() {
  std::cout << "\n--- 2. INTERMEDIATE: KHOI TAO MANG TAM THOI ---\n";

  // Thay vì phải tạo một biến 'mang_tam' ở ngoài, ta nhét luôn lệnh lấy dữ liệu
  // (từ một hàm, API, hoặc Database) vào ngay bên trong định dạng của C++20.

  for (auto mang_tam = layDuLieuTuMang(); auto &phan_tu : mang_tam) {
    std::cout << phan_tu << " ";
    // phan_tu *= 2; // Bạn hoàn toàn có thể sửa đổi dữ liệu của mang_tam tại
    // đây
  }
  std::cout << "\n";
  // Thoát vòng lặp, 'mang_tam' tự động được thu hồi RAM. Không để lại dấu vết.
}

// ==========================================
// 3. MỨC NÂNG CAO: CỨU CÁNH CỦA DANGLING REFERENCE (QUAN TRỌNG NHẤT)
// ==========================================
void cpp20_nang_cao_dangling_reference() {
  std::cout << "\n--- 3. ADVANCED: GIAI CUU LOI 'DANGLING REFERENCE' ---\n";

  /*
  LỖI KINH ĐIỂN MÀ 99% LẬP TRÌNH VIÊN TỪNG MẮC PHẢI TRONG C++11:

      for (const auto& mon_do : taoTuiDoAo().layDanhSach()) {
          std::cout << mon_do; // CRASH CHƯƠNG TRÌNH !!! (Hoặc in ra rác)
      }

  GIẢI THÍCH LỖI:
  1. Hàm taoTuiDoAo() tạo ra một Object vô danh (tạm thời).
  2. C++ gọi .layDanhSach() để lấy cái THAM CHIẾU của mảng bên trong nó.
  3. Ngay sau dấu ngoặc ')' của vòng for, Object tạm thời đó BỊ HỦY NGAY LẬP TỨC
  (gọi Destructor).
  4. Mảng bên trong nó cũng bị xóa theo.
  5. Vòng for bắt đầu chạy và cố gắng truy cập vào tham chiếu 'mon_do' -> Vùng
  nhớ đã chết -> CRASH!
  */

  std::cout << "> Bat dau dung C++20 de giai quyet:\n";

  // GIẢI PHÁP C++20:
  // Ta khởi tạo một biến 'tui_do' ngay trong Init-statement.
  // Biến này sẽ MỞ RỘNG THỜI GIAN SỐNG của Object tạm thời cho đến khi vòng lặp
  // kết thúc!

  for (auto tui_do = taoTuiDoAo(); const auto &mon_do : tui_do.layDanhSach()) {
    std::cout << "  -> Lay duoc: " << mon_do << "\n";
  }
  // Phải đến ĐÂY (sau khi duyệt xong), Destructor của TuiDo mới được gọi!
  // Rất an toàn và hoàn hảo.
}

// ==========================================
// 4. MỨC CHUYÊN GIA: KẾT HỢP VỚI STRUCTURED BINDING (C++17)
// ==========================================
std::map<int, std::string> layDanhSachSinhVien() {
  return {{1, "Le Van Nhan"}, {2, "Tran Thi Binh"}, {3, "Nguyen Van An"}};
}

void cpp20_chuyen_gia_mix_features() {
  std::cout << "\n--- 4. EXPERT: MIX VOI STRUCTURED BINDING (C++17) ---\n";

  // Ở đây ta kết hợp 3 công nghệ hiện đại nhất của C++:
  // 1. Khởi tạo map trong Init-statement (C++20).
  // 2. Khởi tạo biến đếm count (C++20).
  // 3. Bung gói (Structured Binding) Key-Value trực tiếp (C++17).

  for (auto ds_lop = layDanhSachSinhVien(),
            count = std::map<int, std::string>();
       const auto &[ma_sv, ten_sv] : ds_lop) {
    // Chú ý mẹo nhỏ ở trên: Nếu muốn khai báo biến đếm 'count' hoặc 'i' chung
    // với 'ds_lop', chúng PHẢI CÙNG KIỂU BẢN CHẤT (luật của C++).
    // Nên ở ví dụ phức tạp này, khai báo gom chung hơi rườm rà.
    // Thực tế người ta thường chỉ khởi tạo 1 container là đủ.
  }

  // Cách viết đẹp và thông dụng nhất của các chuyên gia:
  for (auto ds_lop = layDanhSachSinhVien();
       const auto &[ma_sv, ten_sv] : ds_lop) {
    std::cout << "  MSSV " << ma_sv << " la ban: " << ten_sv << "\n";
  }
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== C++20 INIT-STATEMENT TRONG RANGE-BASED FOR =====\n";

  cpp20_co_ban();
  cpp20_trung_cap_khoi_tao_mang();
  cpp20_nang_cao_dangling_reference();
  cpp20_chuyen_gia_mix_features();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}