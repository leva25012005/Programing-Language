#include <functional> // Chứa std::greater
#include <iostream>
#include <set>
#include <string>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Cấu trúc dưới nền: std::set và std::multiset được cài đặt bằng Cây Tìm Kiếm
 Nhị Phân Cân Bằng (Cụ thể là Red-Black Tree - Cây Đỏ Đen).
 2. Đặc tính cốt lõi:
    - TỰ ĐỘNG SẮP XẾP: Bất kể bạn nhét dữ liệu vào theo thứ tự nào, nó luôn tự
 sắp xếp (mặc định tăng dần).
    - TỐC ĐỘ: Thêm (Insert), Xóa (Erase), Tìm kiếm (Find) luôn ổn định ở mức
 O(log n).
 3. Sự khác biệt:
    - std::set: KHÔNG CHO PHÉP trùng lặp. Mỗi phần tử là duy nhất.
    - std::multiset: CHO PHÉP trùng lặp (Có thể chứa nhiều giá trị giống nhau).
 4. Nhược điểm: Không thể truy cập bằng Index (như set[5] là sai). Bạn không thể
 thay đổi trực tiếp giá trị của một phần tử (vì sẽ làm hỏng cấu trúc Cây). Muốn
 sửa, phải Xóa cũ -> Thêm mới.
*/

// ==========================================
// CLASS & STRUCT PHỤ TRỢ CHO PHẦN NÂNG CAO
// ==========================================
// Bộ so sánh tùy chỉnh (Custom Comparator)
struct SoSanhChieuDaiChuoi {
  // Dạy cho Set biết: Chuỗi nào ngắn hơn thì đứng trước
  bool operator()(const std::string &a, const std::string &b) const {
    if (a.length() == b.length()) {
      return a < b; // Nếu dài bằng nhau thì xếp theo Alphabet
    }
    return a.length() < b.length();
  }
};

// ==========================================
// 1. MỨC CƠ BẢN: SET VÀ MULTISET KHÁC NHAU RA SAO?
// ==========================================
void set_vs_multiset() {
  std::cout << "\n--- 1. BAZIC: SET vs MULTISET ---\n";

  std::set<int> my_set;
  std::multiset<int> my_multiset;

  // Nhét số 10 vào 3 lần cho cả hai
  for (int i = 0; i < 3; ++i) {
    my_set.insert(10);
    my_multiset.insert(10);
  }

  my_set.insert(5);
  my_multiset.insert(5);
  my_set.insert(20);
  my_multiset.insert(20);

  // Kết quả của std::set: Chỉ giữ 1 số 10, tự động sắp xếp 5 -> 10 -> 20
  std::cout << "std::set size (" << my_set.size() << "): ";
  for (int x : my_set)
    std::cout << x << " ";

  // Kết quả của std::multiset: Giữ cả 3 số 10, tự động sắp xếp 5 -> 10 -> 10 ->
  // 10 -> 20
  std::cout << "\nstd::multiset size (" << my_multiset.size() << "): ";
  for (int x : my_multiset)
    std::cout << x << " ";
  std::cout << "\n";
}

// ==========================================
// 2. MỨC TRUNG CẤP: TÌM KIẾM VÀ XÓA O(log n)
// ==========================================
void set_tim_kiem_va_xoa() {
  std::cout << "\n--- 2. INTERMEDIATE: FIND & ERASE ---\n";

  std::multiset<int> ms = {10, 20, 20, 20, 30, 40};

  // a. Đếm số lần xuất hiện (O(log n + k) với k là số lượng phần tử trùng)
  std::cout << "So luong so 20: " << ms.count(20) << "\n";

  // b. Tìm kiếm (Tuyệt đối KHÔNG DÙNG std::find của algorithm, HÃY DÙNG .find()
  // CỦA RIÊNG SET) Nếu dùng std::find, nó sẽ lội bộ O(n). Hàm .find() của Set
  // xài thuật toán trên Cây O(log n).
  auto it = ms.find(30);
  if (it != ms.end()) {
    std::cout << "Da tim thay: " << *it << "\n";
  }

  // c. Tính năng C++20: .contains() - Ngắn gọn hơn .find() khi chỉ cần hỏi
  // Có/Không if (ms.contains(30)) { std::cout << "Co chua 30!\n"; }

  // d. Xóa (Erase) - CẨN THẬN VỚI MULTISET!
  // Xóa theo Giá trị: SẼ XÓA TOÀN BỘ CÁC SỐ 20 TRONG MULTISET!
  ms.erase(20);
  std::cout << "Sau khi xoa theo gia tri 20, size: " << ms.size() << "\n";

  // Nếu chỉ muốn xóa MỘT số 20 (giả sử ms có lại nhiều số 20), phải xóa bằng
  // con trỏ Iterator:
  ms.insert(50);
  ms.insert(50);
  ms.insert(50);
  auto it_50 = ms.find(50); // Chỉ trỏ tới số 50 đầu tiên
  ms.erase(it_50);          // Chỉ 1 số 50 bay màu, còn lại 2 số
  std::cout << "Sau khi xoa theo con tro 50, so luong so 50 con lai: "
            << ms.count(50) << "\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: TÌM KIẾM NHỊ PHÂN VỚI BOUNDS
// ==========================================
void set_binary_search_bounds() {
  std::cout << "\n--- 3. ADVANCED: LOWER_BOUND & UPPER_BOUND ---\n";

  std::set<int> s = {10, 20, 30, 40, 50, 60};

  // lower_bound(X): Tìm phần tử ĐẦU TIÊN LỚN HƠN HOẶC BẰNG X
  auto it_lower = s.lower_bound(35); // Trả về 40
  std::cout << "lower_bound(35) = " << *it_lower << "\n";

  // upper_bound(X): Tìm phần tử ĐẦU TIÊN LỚN HƠN HẲN X
  auto it_upper = s.upper_bound(40); // Không trả về 40, mà trả về 50
  std::cout << "upper_bound(40) = " << *it_upper << "\n";

  // Kỹ thuật Xóa một dải (Range erase): Xóa từ 30 đến hết 50
  // Ta lấy cận dưới của 30 và cận trên của 50
  s.erase(s.lower_bound(30), s.upper_bound(50));
  std::cout << "Sau khi xoa dange [30, 50]: ";
  for (int x : s)
    std::cout << x << " "; // Chỉ còn 10, 20, 60
  std::cout << "\n";
}

// ==========================================
// 4. MỨC CHUYÊN GIA: CUSTOM COMPARATORS (THAY ĐỔI LUẬT SẮP XẾP)
// ==========================================
void set_custom_sort() {
  std::cout << "\n--- 4. EXPERT: CUSTOM COMPARATOR ---\n";

  // a. Sắp xếp giảm dần (Dùng functor std::greater có sẵn)
  std::set<int, std::greater<int>> giam_dan = {1, 5, 2, 9, 3};
  std::cout << "Set giam dan: ";
  for (int x : giam_dan)
    std::cout << x << " ";
  std::cout << "\n";

  // b. Sắp xếp tùy chỉnh bằng Struct viết tay (So sánh chiều dài chuỗi)
  std::set<std::string, SoSanhChieuDaiChuoi> tu_vung = {
      "Banana", "Apple", "Kiwi", "Strawberry", "Fig"};

  std::cout << "Set xep theo chieu dai chuoi: ";
  for (const auto &str : tu_vung)
    std::cout << str << " ";
  std::cout << "\n";
  // Kết quả: Fig (3) -> Kiwi (4) -> Apple (5) -> Banana (6) -> Strawberry (10)
}

// ==========================================
// 5. HIỆN ĐẠI TỐI THƯỢNG (C++17): NODE EXTRACTION
// ==========================================
void set_cpp17_node_extraction() {
  std::cout << "\n--- 5. MODERN C++17: NODE EXTRACTION ---\n";

  // Vấn đề kinh điển: Làm sao để chuyển 1 phần tử từ Set này sang Set khác?
  // Cách cũ: Xóa ở Set 1 (tốn O(log n)) -> Tạo MỚI ở Set 2 (Tốn thêm RAM và
  // Copy, O(log n)). C++17 cho phép "Bứng nguyên cái Node (cục RAM)" ra và cắm
  // sang Set khác mà KHÔNG TỐN CHI PHÍ CẤP PHÁT LẠI RAM.

  std::set<int> set_a = {1, 2, 3};
  std::set<int> set_b = {7, 8, 9};

  // Bước 1: "Bứng" (Extract) số 2 ra khỏi set_a. Biến node_2 bây giờ ôm trọn
  // vùng nhớ đó.
  auto node_2 = set_a.extract(2);

  // Có thể chỉnh sửa dữ liệu của Node ngay lúc nó đang bay lơ lửng ngoài không
  // gian (Xưa nay Set cấm sửa)
  node_2.value() = 99;

  // Bước 2: "Cắm" (Insert) node đó vào set_b. KHÔNG hề có cấp phát động xảy ra!
  set_b.insert(std::move(node_2));

  std::cout << "Set A hien tai: ";
  for (int x : set_a)
    std::cout << x << " "; // 1, 3

  std::cout << "\nSet B hien tai: ";
  for (int x : set_b)
    std::cout << x << " "; // 7, 8, 9, 99
  std::cout << "\n";
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  set_vs_multiset();
  set_tim_kiem_va_xoa();
  set_binary_search_bounds();
  set_custom_sort();
  set_cpp17_node_extraction();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}