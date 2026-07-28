#include <algorithm> // std::ranges::sort, std::ranges::copy
#include <iostream>
#include <map>
#include <ranges> // BẮT BUỘC CHO C++20 RANGES: std::views
#include <string>
#include <vector>

// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 SỨ MỆNH CỦA C++20 RANGES: Cuộc cách mạng lớn nhất kể từ C++11.
 1. Xóa sổ begin() và end(): Bạn chỉ cần quăng nguyên cái Mảng (Container) vào
 hàm.
 2. Toán tử Đường ống (Pipe '|'): Giúp code chảy từ trái sang phải, từ trên
 xuống dưới giống hệt tư duy con người, thay vì lồng ghép các hàm từ trong ra
 ngoài khó đọc.
 3. Lazy Evaluation (Lười biếng): Views KHÔNG tạo ra mảng mới trong RAM. Nó
 giống như một "chiếc kính lúp" đặt lên mảng gốc. Dữ liệu chỉ thực sự bị xử lý
    khi bạn dùng vòng lặp for duyệt qua nó. Giúp tiết kiệm RAM tuyệt đối!
*/

// ==========================================
// CLASS PHỤ TRỢ CHO PHẦN CHUYÊN GIA
// ==========================================
struct NhanVien {
  std::string ten;
  int tuoi;
  double luong;
};

// Hàm tiện ích in View (Chạy được với mọi loại View/Range)
void in_range(auto &&range, const std::string &thong_diep) {
  std::cout << thong_diep << ": ";
  for (auto val : range)
    std::cout << val << " ";
  std::cout << "\n";
}

// ==========================================
// 1. MỨC CƠ BẢN: TẠM BIỆT BEGIN() VÀ END()
// ==========================================
void ranges_co_ban() {
  std::cout << "\n--- 1. BAZIC: TAM BIET BEGIN() & END() ---\n";

  std::vector<int> v = {7, 1, 4, 3, 9, 2};

  // NGÀY XƯA (C++98 -> C++17):
  // std::sort(v.begin(), v.end());

  // C++20: Gọn gàng, tao nhã. Truyền cả mảng vào!
  std::ranges::sort(v);

  in_range(v, "Mang sau khi std::ranges::sort");

  // Đảo ngược nguyên mảng
  std::ranges::reverse(v);
  in_range(v, "Mang sau khi std::ranges::reverse");
}

// ==========================================
// 2. MỨC TRUNG CẤP: TOÁN TỬ ĐƯỜNG ỐNG (PIPE |) & VIEWS
// ==========================================
void ranges_duong_ong_views() {
  std::cout << "\n--- 2. INTERMEDIATE: VIEWS & TOAN TU PIPE (|) ---\n";

  std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  // ĐỀ BÀI: Lấy ra các số CHẴN, sau đó BÌNH PHƯƠNG chúng lên.
  // LƯU Ý: Biến 'ket_qua' ở đây KHÔNG PHẢI là một std::vector. Nó là một Cấu
  // trúc View ảo. Không có 1 byte RAM nào được cấp phát thêm cho 'ket_qua'.

  auto ket_qua =
      numbers |
      std::views::filter([](int n) { return n % 2 == 0; }) // Giữ lại số chẵn
      | std::views::transform([](int n) { return n * n; }); // Bình phương

  std::cout << "Ket qua cua duong ong (Filter -> Transform): ";

  // TÍNH LƯỜI BIẾNG (LAZY): Chỉ khi vòng lặp này chạy, các phép tính %2 và n*n
  // mới thực sự diễn ra!
  for (int n : ket_qua) {
    std::cout << n << " "; // In ra: 4 16 36 64 100
  }
  std::cout << "\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: TỔ HỢP VIEWS & VÒNG LẶP VÔ TẬN
// ==========================================
void ranges_to_hop_va_vo_tan() {
  std::cout << "\n--- 3. ADVANCED: TAKE, DROP & INFINITE VIEWS ---\n";

  std::vector<int> v = {10, 20, 30, 40, 50, 60, 70, 80};

  // a. Bỏ qua 2 phần tử đầu (drop), sau đó chỉ lấy 3 phần tử tiếp theo (take),
  // rồi đảo ngược (reverse)
  auto to_hop = v | std::views::drop(2) // Bỏ 10, 20
                | std::views::take(3)   // Lấy 30, 40, 50
                | std::views::reverse;  // Đảo ngược thành 50, 40, 30

  in_range(to_hop, "To hop Drop(2) -> Take(3) -> Reverse");

  // b. MA THUẬT: IOTA (Sinh số vô tận)
  // std::views::iota(1) sẽ tạo ra một danh sách đếm từ 1 đến Vô cực!
  // Nếu dùng vector, RAM sẽ nổ tung (Memory Leak/OOM).
  // Nhưng vì là View lười biếng, máy tính sẽ không tính trước.

  auto day_so_vo_tan = std::views::iota(1);

  // Yêu cầu: Sinh số từ 1 đến vô cực -> Lọc lấy số chia hết cho 7 -> Chỉ lấy 5
  // số đầu tiên.
  auto lay_5_so_chia_het_7 =
      day_so_vo_tan | std::views::filter([](int n) { return n % 7 == 0; }) |
      std::views::take(5); // Dừng lại sau khi đủ 5 số, không bị treo máy!

  in_range(lay_5_so_chia_het_7, "5 so dau tien chia het cho 7 tu vo cuc");
}

// ==========================================
// 4. MỨC CHUYÊN GIA: PROJECTIONS (VŨ KHÍ TỐI THƯỢNG)
// ==========================================
void ranges_projections() {
  std::cout << "\n--- 4. EXPERT: PROJECTIONS (GOC CHIEU) ---\n";

  std::vector<NhanVien> cty = {
      {"Nhan", 25, 2000.0}, {"An", 30, 1500.0}, {"Binh", 22, 1800.0}};

  // NGÀY XƯA: Để sắp xếp class theo Tuổi, bạn phải viết Lambda rườm rà:
  // std::sort(cty.begin(), cty.end(), [](const NhanVien& a, const NhanVien& b)
  // { return a.tuoi < b.tuoi; });

  // C++20 PROJECTIONS: Tham số thứ 3 của hàm Sort cho phép bạn chiếu thẳng vào
  // biến cần so sánh! Sắp xếp tăng dần theo Tuổi
  std::ranges::sort(cty, std::less{}, &NhanVien::tuoi);

  std::cout << "Sap xep nhan vien tang dan theo Tuoi:\n";
  for (const auto &nv : cty) {
    std::cout << "  " << nv.ten << " (" << nv.tuoi << " tuoi)\n";
  }

  // Kết hợp Projections vào Pipeline: Lấy tên của những nhân viên lương > 1600
  auto nhung_nguoi_giau =
      cty |
      std::views::filter([](double luong) { return luong > 1600.0; },
                         &NhanVien::luong) // Filter chiếu vào lương
      |
      std::views::transform(&NhanVien::ten); // Transform để chỉ lấy biến 'ten'

  in_range(nhung_nguoi_giau, "Ten nhung nguoi luong > 1600");
}

// ==========================================
// 5. CÁC VIEWS ĐẶC BIỆT KHÁC (KEYS, VALUES CỦA MAP)
// ==========================================
void ranges_map_views() {
  std::cout << "\n--- 5. DICTIONARY VIEWS: KEYS & VALUES ---\n";

  std::map<std::string, int> bang_diem = {
      {"Nhan", 9}, {"Trang", 10}, {"Hieu", 8}};

  // Không cần dùng it->first và it->second nữa!
  auto chi_lay_ten = bang_diem | std::views::keys;
  auto chi_lay_diem = bang_diem | std::views::values;

  in_range(chi_lay_ten, "Danh sach hoc sinh");
  in_range(chi_lay_diem, "Danh sach diem so");

  // ==========================================
  // LÀM SAO ĐỂ LƯU VIEW VÀO LẠI VECTOR?
  // ==========================================
  // Nhắc lại: View là ẢO. Nếu mảng gốc bị xóa, View sẽ trỏ vào vùng nhớ rác
  // (Dangling). Để "Vật chất hóa" (Materialize) View thành Vector thật:

  std::vector<int> diem_thuc_te;

  // Ở C++20, phải dùng ranges::copy để đổ View vào Vector mới.
  // (Từ C++23, bạn có thể gọi thẳng: auto vec = chi_lay_diem |
  // std::ranges::to<std::vector>();)
  std::ranges::copy(chi_lay_diem, std::back_inserter(diem_thuc_te));

  std::cout << "Size cua vector diem_thuc_te: " << diem_thuc_te.size() << "\n";
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  ranges_co_ban();
  ranges_duong_ong_views();
  ranges_to_hop_va_vo_tan();
  ranges_projections();
  ranges_map_views();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}
// Hệ thống Concepts (Ràng buộc Template)
// Thư viện Ranges (Xử lý tập hợp)