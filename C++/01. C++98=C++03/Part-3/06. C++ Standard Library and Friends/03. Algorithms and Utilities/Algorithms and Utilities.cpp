#include <algorithm>  // std::sort, std::find, std::copy, ...
#include <chrono>     // Đo lường thời gian
#include <functional> // std::function, std::greater
#include <iostream>
#include <iterator> // std::back_inserter
#include <string>
#include <tuple>   // std::tuple, std::get
#include <utility> // std::pair, std::swap, std::move
#include <vector>


// ==========================================
// CÁC HÀM VÀ CLASS PHỤ TRỢ
// ==========================================
void in_mang(const std::vector<int> &v, const std::string &msg) {
  std::cout << msg << ": ";
  for (int x : v)
    std::cout << x << " ";
  std::cout << "\n";
}

// Struct dùng cho ví dụ std::stable_sort
struct SinhVien {
  std::string ten;
  int diem;
};

// Class mô phỏng tài nguyên nặng để demo std::move
class TaiNguyenNang {
public:
  std::string data;
  TaiNguyenNang(std::string d) : data(d) {}

  // Copy Constructor (Tốn kém)
  TaiNguyenNang(const TaiNguyenNang &other) {
    data = other.data;
    std::cout << "    [Copy] Da sao chep: " << data << "\n";
  }

  // Move Constructor (Siêu nhanh, cướp tài nguyên)
  TaiNguyenNang(TaiNguyenNang &&other) noexcept {
    data = std::move(other.data);
    std::cout << "    [Move] Da dich chuyen (cuop) data!\n";
  }
};

// ==========================================
// PHẦN I: ALGORITHMS (<algorithm>)
// ==========================================

// 1. Sắp xếp và Tìm kiếm
void demo_sort_search() {
  std::cout << "\n--- I.1. SORTING & SEARCHING ---\n";
  std::vector<int> v = {5, 2, 8, 1, 9, 2};
  in_mang(v, "Ban dau");

  // a. std::sort (O(n log n)) - Mặc định tăng dần
  std::sort(v.begin(), v.end());
  in_mang(v, "Sau khi sort (Tang dan)");

  // Dùng Lambda + std::greater để sắp xếp giảm dần
  std::sort(v.begin(), v.end(), std::greater<int>());
  in_mang(v, "Sau khi sort (Giam dan)");

  // b. std::stable_sort (Giữ nguyên thứ tự ban đầu nếu bằng nhau)
  std::vector<SinhVien> ds = {{"An", 8}, {"Binh", 9}, {"Cuong", 8}};
  std::stable_sort(ds.begin(), ds.end(),
                   [](const SinhVien &a, const SinhVien &b) {
                     return a.diem > b.diem; // Xếp điểm giảm dần
                   });
  std::cout << "Stable Sort (Diem Giam dan): ";
  // Binh (9) lên đầu, An (8) và Cuong (8) giữ nguyên thứ tự An đứng trước Cuong
  for (auto &sv : ds)
    std::cout << sv.ten << "(" << sv.diem << ") ";
  std::cout << "\n";

  // Đưa mảng về tăng dần để demo tìm kiếm nhị phân
  std::sort(v.begin(), v.end());
  in_mang(v, "\nMang dang xep tang de tim kiem");

  // c. std::binary_search (O(log n)) - Trả về true/false
  bool co_so_8 = std::binary_search(v.begin(), v.end(), 8);
  std::cout << "Co so 8 khong? " << (co_so_8 ? "Co" : "Khong") << "\n";

  // d. lower_bound & upper_bound (Siêu quan trọng trong lập trình thi đấu)
  // lower_bound: Tìm Iterator trỏ tới phần tử ĐẦU TIÊN >= X
  auto it_lower = std::lower_bound(v.begin(), v.end(), 2);
  // upper_bound: Tìm Iterator trỏ tới phần tử ĐẦU TIÊN > X
  auto it_upper = std::upper_bound(v.begin(), v.end(), 2);

  std::cout << "lower_bound cua 2 (Vi tri >= 2): index "
            << std::distance(v.begin(), it_lower) << "\n";
  std::cout << "upper_bound cua 2 (Vi tri > 2): index "
            << std::distance(v.begin(), it_upper) << "\n";
}

// 2. Thao tác không sửa đổi (Non-modifying)
void demo_non_modifying() {
  std::cout << "\n--- I.2. NON-MODIFYING ALGORITHMS ---\n";
  std::vector<int> v = {10, 20, 30, 20, 40};

  // a. std::find (O(n)) - Tìm vị trí đầu tiên
  auto it = std::find(v.begin(), v.end(), 30);
  if (it != v.end())
    std::cout << "Tim thay 30 o index: " << std::distance(v.begin(), it)
              << "\n";

  // b. std::count (O(n)) - Đếm số lần xuất hiện
  std::cout << "So lan xuat hien cua 20: " << std::count(v.begin(), v.end(), 20)
            << "\n";

  // c. std::for_each - Áp dụng hàm lên từng phần tử
  std::cout << "Dung for_each in cong them 1: ";
  std::for_each(v.begin(), v.end(), [](int x) { std::cout << x + 1 << " "; });
  std::cout << "\n";

  // d. all_of / any_of / none_of (Trả về true/false)
  bool tat_ca_la_so_chan =
      std::all_of(v.begin(), v.end(), [](int x) { return x % 2 == 0; });
  bool co_so_lon_hon_35 =
      std::any_of(v.begin(), v.end(), [](int x) { return x > 35; });

  std::cout << "Tat ca la so chan? " << (tat_ca_la_so_chan ? "True" : "False")
            << "\n";
  std::cout << "Co so nao > 35 khong? " << (co_so_lon_hon_35 ? "True" : "False")
            << "\n";
}

// 3. Thao tác sửa đổi (Modifying)
void demo_modifying() {
  std::cout << "\n--- I.3. MODIFYING ALGORITHMS ---\n";
  std::vector<int> v1 = {1, 2, 3, 4, 5};
  std::vector<int> v2;

  // a. std::copy - Copy từ v1 sang v2
  // Lưu ý: v2 đang rỗng, phải dùng std::back_inserter để tự động thêm bộ nhớ
  std::copy(v1.begin(), v1.end(), std::back_inserter(v2));
  in_mang(v2, "Copy sang v2");

  // b. std::reverse - Đảo ngược
  std::reverse(v2.begin(), v2.end());
  in_mang(v2, "Reverse v2");

  // c. std::fill - Phủ toàn bộ bằng 1 giá trị
  std::fill(v2.begin(), v2.begin() + 3, 99); // Phủ 3 phần tử đầu thành 99
  in_mang(v2, "Fill 3 phan tu dau tien");

  // d. Erase-Remove Idiom (Xóa phần tử siêu cấp)
  std::vector<int> v3 = {10, 20, 20, 30, 20, 40};
  // std::remove KHÔNG thực sự xóa, nó đẩy các số 20 về cuối và trả về iterator
  // ở biên. Phải gọi thêm hàm erase của vector để chém bỏ phần đuôi đó.
  v3.erase(std::remove(v3.begin(), v3.end(), 20), v3.end());
  in_mang(v3, "Sau khi xoa tat ca so 20");

  // e. std::unique - Xóa các phần tử TRÙNG LẶP ĐỨNG CẠNH NHAU
  std::vector<int> v4 = {1, 1, 2, 2, 2, 3, 1};
  std::sort(v4.begin(), v4.end()); // Bắt buộc phải sort trước khi dùng unique
  v4.erase(std::unique(v4.begin(), v4.end()), v4.end());
  in_mang(v4, "Sau khi xoa trung lap (unique)");
}

// 4. Tìm Cực đại / Cực tiểu
void demo_min_max() {
  std::cout << "\n--- I.4. MIN/MAX ALGORITHMS ---\n";

  // a. min / max giữa 2 số (hoặc nhiều số dùng ngoặc nhọn {})
  std::cout << "Max cua 10 va 20 la: " << std::max(10, 20) << "\n";
  std::cout << "Min cua 5, 8, 2, 9 la: " << std::min({5, 8, 2, 9}) << "\n";

  // b. min_element / max_element trên container (Trả về Iterator)
  std::vector<int> v = {45, 12, 89, 23, 7};
  auto it_min = std::min_element(v.begin(), v.end());
  auto it_max = std::max_element(v.begin(), v.end());

  std::cout << "Phan tu nho nhat trong mang: " << *it_min << "\n";
  std::cout << "Phan tu lon nhat trong mang: " << *it_max << "\n";
}

// ==========================================
// PHẦN II: UTILITIES (Tiện ích)
// ==========================================

// 1. Cặp dữ liệu và Move Semantics (<utility>)
void demo_utility() {
  std::cout << "\n--- II.1. UTILITY: PAIR & MOVE SEMANTICS ---\n";

  // a. std::pair & std::make_pair
  std::pair<int, std::string> p1 = std::make_pair(1, "Mot");
  std::pair<int, std::string> p2 = {2, "Hai"}; // Cách C++11 ngắn gọn hơn
  std::cout << "Pair: " << p1.first << " - " << p1.second << "\n";

  // b. std::swap
  int a = 10, b = 99;
  std::swap(a, b);
  std::cout << "Sau swap: a=" << a << ", b=" << b << "\n";

  // c. std::move - Ma thuật tối ưu hiệu năng
  std::cout << "Demo std::move:\n";
  TaiNguyenNang obj1("Du lieu Khong Lo");

  // Copy thông thường (Tốn RAM, tốn thời gian)
  TaiNguyenNang obj2 = obj1;

  // Dùng std::move để ÉP kiểu thành rvalue.
  // Trình biên dịch sẽ gọi Move Constructor, CƯỚP thẳng vùng nhớ của obj1 đưa
  // cho obj3.
  TaiNguyenNang obj3 = std::move(obj1);

  // Lúc này obj1 đã biến thành một cái xác không hồn (chuỗi rỗng).
  std::cout << "    Data cua obj1 sau move: [" << obj1.data << "]\n";
}

// 2. Tuple (<tuple>)
void demo_tuple() {
  std::cout << "\n--- II.2. TUPLE (Bo ba, bo bon...) ---\n";

  // Khởi tạo Tuple chứa 3 kiểu dữ liệu khác nhau
  std::tuple<int, double, std::string> my_tuple = {10, 3.14, "C++"};

  // Truy xuất dữ liệu (Bắt buộc phải truyền index dưới dạng hằng số < >)
  std::cout << "Phan tu 0: " << std::get<0>(my_tuple) << "\n";
  std::cout << "Phan tu 2: " << std::get<2>(my_tuple) << "\n";

  // C++17: Structured Bindings - Cách tuyệt vời nhất để bung gói Tuple/Pair
  auto [so_nguyen, so_thuc, chuoi] = my_tuple;
  std::cout << "C++17 Bung goi tuple: " << so_nguyen << ", " << so_thuc << ", "
            << chuoi << "\n";
}

// 3. Functional (<functional>)
// Một hàm mồi để ném vào std::function
void chao_hoi(std::string ten) {
  std::cout << "Xin chao " << ten << " tu ham thuong!\n";
}

void demo_functional() {
  std::cout << "\n--- II.3. FUNCTIONAL ---\n";

  // std::function là cái túi ba gang, có thể đựng BẤT KỲ hàm nào có cùng chữ ký
  // Cú pháp: std::function< KieuTraVe(ThamSo1, ThamSo2) >
  std::function<void(std::string)> tui_chua_ham;

  // Bỏ hàm thông thường vào túi
  tui_chua_ham = chao_hoi;
  tui_chua_ham("Nhan");

  // Bỏ Lambda Function vào túi
  tui_chua_ham = [](std::string ten) {
    std::cout << "Hello " << ten << " tu Lambda!\n";
  };
  tui_chua_ham("AI");
}

// 4. Chrono (<chrono>) - Đo thời gian
void demo_chrono() {
  std::cout << "\n--- II.4. CHRONO: DO THOI GIAN THUC THI ---\n";

  // Ghi lại thời điểm bắt đầu
  auto start = std::chrono::high_resolution_clock::now();

  // Giả lập một thuật toán tốn thời gian (Duyệt vòng lặp)
  long long sum = 0;
  for (volatile int i = 0; i < 10000000; ++i) {
    sum += i; // Dùng volatile để cản Compiler xóa vòng lặp tối ưu
  }

  // Ghi lại thời điểm kết thúc
  auto end = std::chrono::high_resolution_clock::now();

  // Tính toán độ lệch thời gian (Ép sang kiểu mili giây)
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout << "Thoi gian chay vong lap: " << duration.count()
            << " milliseconds.\n";
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== KHOA HOC C++ STANDARD LIBRARY =====\n";

  demo_sort_search();
  demo_non_modifying();
  demo_modifying();
  demo_min_max();

  demo_utility();
  demo_tuple();
  demo_functional();
  demo_chrono();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}