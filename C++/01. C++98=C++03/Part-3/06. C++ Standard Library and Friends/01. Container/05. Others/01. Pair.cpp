#include <algorithm>
#include <iostream>
#include <string>
#include <tuple>   // Cho std::tie
#include <utility> // Bắt buộc: Thư viện chứa std::pair
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Bản chất: std::pair là một Struct cực kỳ đơn giản được C++ viết sẵn.
    Nó chỉ có đúng 2 biến thành viên công khai (public) là:
    - .first  (Lưu dữ liệu kiểu thứ 1)
    - .second (Lưu dữ liệu kiểu thứ 2)

 2. Sứ mệnh: Dùng để "đóng gói" 2 dữ liệu CÓ KIỂU KHÁC NHAU vào làm một.
    Nó thường được dùng để:
    - Trả về 2 giá trị cùng lúc từ một hàm.
    - Làm nền tảng cấu tạo nên phần tử của std::map (1 pair gồm Key và Value).
*/

// ==========================================
// CLASS PHỤ TRỢ CHO PHẦN CHUYÊN GIA
// ==========================================
class VuKhi {
public:
  VuKhi(int sat_thuong, float toc_do) {
    std::cout << "  [Tao Vu Khi] Sat thuong: " << sat_thuong << "\n";
  }
  // Copy Constructor để theo dõi
  VuKhi(const VuKhi &other) { std::cout << "  [Copy Vu Khi]\n"; }
};

// ==========================================
// 1. MỨC CƠ BẢN: KHỞI TẠO VÀ TRUY XUẤT
// ==========================================
void pair_co_ban() {
  std::cout << "\n--- 1. BAZIC: KHOI TAO & TRUY XUAT ---\n";

  // Cách 1: Khởi tạo rỗng rồi gán
  std::pair<int, std::string> p1;
  p1.first = 1;
  p1.second = "Mot";

  // Cách 2: Khởi tạo bằng ngoặc nhọn (List initialization - Khuyên dùng)
  std::pair<int, std::string> p2 = {2, "Hai"};

  // Cách 3: Dùng std::make_pair (C++11) - Trình biên dịch tự đoán kiểu dữ liệu
  auto p3 = std::make_pair(3, "Ba");

  std::cout << "p2 co first = " << p2.first << ", second = " << p2.second
            << "\n";
}

// ==========================================
// 2. MỨC TRUNG CẤP: SO SÁNH (LEXICOGRAPHICAL) VÀ MẢNG PAIR
// ==========================================
void pair_so_sanh_va_mang() {
  std::cout << "\n--- 2. INTERMEDIATE: SO SANH & VECTOR PAIR ---\n";

  std::pair<int, int> a = {1, 100};
  std::pair<int, int> b = {2, 10};
  std::pair<int, int> c = {1, 200};

  // LUẬT SO SÁNH (TỪ ĐIỂN):
  // Nó sẽ so sánh biến .first trước.
  // Nếu .first bằng nhau, nó MỚI so sánh tới biến .second.

  if (a < b)
    std::cout << "a < b (Vi a.first=1 nho hon b.first=2)\n";
  if (a < c)
    std::cout << "a < c (Vi first bang nhau, nhung a.second < c.second)\n";

  // ỨNG DỤNG: Sort một Vector Pair
  std::vector<std::pair<std::string, int>> ds_diem = {
      {"Nhan", 8}, {"An", 10}, {"Nhan", 9}};

  // Khi sort, nó sẽ tự động ưu tiên sắp xếp Tên (A-Z) trước.
  // Nếu Tên trùng nhau (Nhan), nó sẽ xếp theo Điểm tăng dần.
  std::sort(ds_diem.begin(), ds_diem.end());

  std::cout << "\nSau khi Sort:\n";
  for (const auto &p : ds_diem) {
    std::cout << "  " << p.first << ": " << p.second << "\n";
  }
}

// ==========================================
// 3. MỨC NÂNG CAO: HÀM TRẢ VỀ PAIR & UNPACKING (BUNG GÓI)
// ==========================================
// Một hàm trả về 2 giá trị: Thương số và Số dư
std::pair<int, int> chia_lay_du(int tu_so, int mau_so) {
  return {tu_so / mau_so, tu_so % mau_so};
}

void pair_ham_va_unpacking() {
  std::cout << "\n--- 3. ADVANCED: RETURNING PAIR & UNPACKING ---\n";

  // a. Cách C++98: Hứng nguyên cái Pair (Dài dòng, phải gọi .first, .second)
  std::pair<int, int> kq_cu = chia_lay_du(10, 3);
  std::cout << "C++98 -> Thuong: " << kq_cu.first << ", Du: " << kq_cu.second
            << "\n";

  // b. Cách C++11: Dùng std::tie để bung gói thẳng vào biến đã tạo sẵn
  int thuong, du;
  std::tie(thuong, du) = chia_lay_du(10, 3);

  // MẸO C++11: Nếu bạn chỉ quan tâm Số Dư, bỏ qua Thương (Dùng std::ignore)
  int chi_lay_du;
  std::tie(std::ignore, chi_lay_du) = chia_lay_du(14, 5);

  // c. Cách HIỆN ĐẠI TỐI THƯỢNG (C++17): Structured Bindings
  // Không cần khai báo biến trước, cực kỳ thanh lịch!
  auto [thuong_17, du_17] = chia_lay_du(10, 3);
  std::cout << "C++17 -> Thuong: " << thuong_17 << ", Du: " << du_17 << "\n";
}

// ==========================================
// 4. MỨC CHUYÊN GIA: STD::PIECEWISE_CONSTRUCT (MA THUẬT KHỞI TẠO)
// ==========================================
void pair_piecewise_construct() {
  std::cout << "\n--- 4. EXPERT: PIECEWISE CONSTRUCT ---\n";

  // VẤN ĐỀ: Khi bạn tạo một Pair chứa các Đối tượng (Class) phức tạp,
  // việc dùng {} hay make_pair sẽ tạo ra Object tạm thời rồi mới Copy/Move vào
  // Pair. Rất tốn kém!

  std::cout << "> Khoi tao thong thuong (Bi Copy):\n";
  std::pair<int, VuKhi> p1 = {1, VuKhi(100, 1.5f)};
  // Output: Sẽ gọi [Tao Vu Khi], sau đó gọi [Copy Vu Khi] để nhét vào Pair.

  std::cout << "\n> Khoi tao bang Piecewise Construct (Khong Copy):\n";
  // GIẢI PHÁP: std::piecewise_construct báo cho Pair biết:
  // "Đừng tạo object vội. Hãy cầm lấy các tham số tao đưa,
  // và tự chui vào RAM khởi tạo trực tiếp Object đi!"

  std::pair<int, VuKhi> p2(
      std::piecewise_construct,
      std::forward_as_tuple(2), // Tham số ném cho biến .first (int)
      std::forward_as_tuple(200,
                            2.0f) // Các tham số ném cho biến .second (VuKhi)
  );
  // Output: Chỉ gọi duy nhất 1 lần [Tao Vu Khi]. Hoàn toàn không tốn Copy!
  // Đây là kiến thức chuyên sâu thường dùng để tối ưu hóa trong Game Engine.
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  pair_co_ban();
  pair_so_sanh_va_mang();
  pair_ham_va_unpacking();
  pair_piecewise_construct();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}