#include <functional> // Cho std::hash
#include <iostream>
#include <string>
#include <unordered_set>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Cấu trúc dưới nền: SỬ DỤNG BẢNG BĂM (Hash Table), KHÔNG PHẢI CÂY!
 2. Cách hoạt động:
    - Khi bạn đưa vào một dữ liệu (Key), C++ sẽ đưa Key đó qua một HÀM BĂM (Hash
 Function).
    - Hàm băm sẽ biến Key đó thành một con số nguyên (gọi là Mã băm - Hash
 code).
    - C++ dùng con số này để bỏ dữ liệu vào một cái Thùng (Bucket) tương ứng
 trên RAM.
 3. Đặc tính cốt lõi:
    - KHÔNG SẮP XẾP: Dữ liệu in ra sẽ hoàn toàn lộn xộn, không theo thứ tự A-Z
 hay Tăng dần.
    - TỐC ĐỘ BÀN THỜ: Tìm kiếm, Thêm, Xóa gần như LUÔN LUÔN LÀ O(1) (Nhanh nhất
 trong STL).
 4. Sự khác biệt:
    - std::unordered_set: KHÔNG trùng lặp.
    - std::unordered_multiset: CHO PHÉP trùng lặp.
*/

// ==========================================
// CLASS & STRUCT PHỤ TRỢ CHO PHẦN CHUYÊN GIA
// ==========================================
struct ToaDo {
  int x;
  int y;

  // 1. ĐIỀU KIỆN BẮT BUỘC SỐ 1: Toán tử So sánh Bằng (==)
  // Khi hai phần tử có cùng Mã băm (Hash Collision), C++ cần biết cách so sánh
  // xem chúng có thực sự GIỐNG NHAU không.
  bool operator==(const ToaDo &other) const {
    return (x == other.x && y == other.y);
  }
};

// 2. ĐIỀU KIỆN BẮT BUỘC SỐ 2: Viết Hàm Băm (Custom Hash)
struct ToaDoHash {
  // Biến cấu trúc ToaDo thành 1 con số nguyên duy nhất kiểu size_t
  size_t operator()(const ToaDo &td) const {
    // Dùng std::hash có sẵn để băm từng số, sau đó trộn (XOR '^' và Dịch bit
    // '<<') chúng lại
    size_t hash_x = std::hash<int>()(td.x);
    size_t hash_y = std::hash<int>()(td.y);
    return hash_x ^ (hash_y << 1);
  }
};

// ==========================================
// 1. MỨC CƠ BẢN: ĐẶC TÍNH "KHÔNG SẮP XẾP" (UNORDERED)
// ==========================================
void unordered_set_co_ban() {
  std::cout << "\n--- 1. BAZIC: KHONG SAP XEP & DUY NHAT ---\n";

  std::unordered_set<std::string> ds_trai_cay = {"Cam", "Tao", "Chuoi", "Nho"};

  // Thêm dữ liệu trùng lặp (Sẽ bị phớt lờ)
  ds_trai_cay.insert("Cam");

  std::cout << "Kich thuoc: " << ds_trai_cay.size() << "\n"; // Vẫn là 4

  // IN RA SẼ THẤY LỘN XỘN (Không theo bảng chữ cái như std::set)
  std::cout << "Danh sach Trai cay (Lộn xộn): ";
  for (const auto &ten : ds_trai_cay) {
    std::cout << ten << " ";
  }
  std::cout << "\n";

  // Tìm kiếm siêu tốc độ O(1)
  // C++20: Dùng .contains() tiện lợi hơn .find()
  if (ds_trai_cay.contains("Tao")) {
    std::cout << "-> Co Tao trong danh sach!\n";
  }
}

// ==========================================
// 2. MỨC TRUNG CẤP: UNORDERED_MULTISET & COLLISION
// ==========================================
void unordered_multiset_thao_tac() {
  std::cout << "\n--- 2. INTERMEDIATE: MULTISET ---\n";

  std::unordered_multiset<int> ums = {10, 20, 20, 20, 30};

  // Đếm số lần xuất hiện (O(1) thay vì O(log n) như std::multiset)
  std::cout << "So lan xuat hien cua so 20: " << ums.count(20) << "\n";

  // Xóa theo Giá trị: XÓA TẤT CẢ SỐ 20
  ums.erase(20);
  std::cout << "Kich thuoc sau khi xoa so 20: " << ums.size()
            << " (Chi con 10 va 30)\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: BÍ MẬT BẢNG BĂM (BUCKETS & LOAD FACTOR)
// ==========================================
void unordered_set_kien_truc_bucket() {
  std::cout << "\n--- 3. ADVANCED: BUCKETS & REHASH ---\n";

  std::unordered_set<int> us;

  // TỐI ƯU HÓA TỐI THƯỢNG: Nếu bạn biết trước sẽ thêm 1000 phần tử,
  // HÃY DÙNG RESERVE! Nếu không, Bảng băm sẽ phải Rehash (đập đi xây lại) liên
  // tục rất tốn CPU.
  us.reserve(10);

  us.insert(1);
  us.insert(2);
  us.insert(15);
  us.insert(27);
  us.insert(5);

  // Bảng băm chia thành các "Thùng" (Buckets)
  std::cout << "Tong so Thung (Buckets) hien co: " << us.bucket_count() << "\n";

  // Load Factor: Tỉ lệ giữa (Số phần tử) / (Số thùng).
  // Mặc định C++ nếu Load Factor > 1.0 (Quá chật), nó sẽ tự động tạo thêm Thùng
  // (Rehash).
  std::cout << "Ti le lấp day (Load Factor): " << us.load_factor() << "\n";

  // KHÁM PHÁ CÁC THÙNG DƯỚI NỀN RAM
  std::cout << "\nKham pha tung Thung:\n";
  for (size_t i = 0; i < us.bucket_count(); ++i) {
    std::cout << "  Thung #" << i << " co " << us.bucket_size(i) << " phan tu";
    if (us.bucket_size(i) > 0) {
      std::cout << " -> Chứa: ";
      // Lặp qua các phần tử CỦA RIÊNG Thùng này
      for (auto it = us.begin(i); it != us.end(i); ++it) {
        std::cout << *it << " ";
      }
    }
    std::cout << "\n";
  }
}

// ==========================================
// 4. MỨC CHUYÊN GIA: LƯU TRỮ STRUCT/CLASS TÙY CHỈNH
// ==========================================
void unordered_set_custom_struct() {
  std::cout << "\n--- 4. EXPERT: CUSTOM HASH CHO CLASS ---\n";

  // Để cho ToaDo vào unordered_set, ta phải đưa Struct ToaDoHash vào làm tham
  // số thứ 2
  std::unordered_set<ToaDo, ToaDoHash> tap_hop_toa_do;

  tap_hop_toa_do.insert({10, 20});
  tap_hop_toa_do.insert({5, 8});
  tap_hop_toa_do.insert({10, 20}); // Trùng lặp (Sẽ bị phớt lờ nhờ toán tử ==)

  std::cout << "So luong Toa Do trong set: " << tap_hop_toa_do.size() << "\n";

  if (tap_hop_toa_do.contains({5, 8})) {
    std::cout << "Da tim thay Toa do (5, 8) bang Ham Bam tuy chinh!\n";
  }
}

// ==========================================
// 5. HIỆN ĐẠI TỐI THƯỢNG (C++17): MERGE (GỘP BẢNG BĂM)
// ==========================================
void unordered_set_cpp17_merge() {
  std::cout << "\n--- 5. MODERN C++17: MERGE (GOP SET O(1)) ---\n";

  std::unordered_set<int> set_A = {1, 2, 3};
  std::unordered_set<int> set_B = {3, 4, 5}; // Số 3 bị trùng

  // C++17 cho phép "chuyển dọn" toàn bộ các Node từ Set B sang Set A
  // KHÔNG tốn chi phí copy/cấp phát RAM. Nó chỉ trỏ lại các con trỏ trong Bảng
  // Băm.
  set_A.merge(set_B);

  std::cout << "Set A sau khi gop: ";
  for (int x : set_A)
    std::cout << x << " "; // 1, 2, 3, 4, 5

  std::cout
      << "\nSet B hien tai (Con chua nhung Node bi trung bi set A tu choi): ";
  for (int x : set_B)
    std::cout << x << " "; // Chi con lai so 3
  std::cout << "\n";
}

// ==========================================
// TỔNG KẾT LUẬT CHỌN SET HAY UNORDERED_SET
// ==========================================
/*
 1. BAO GIỜ THÌ DÙNG std::unordered_set?
    - TRONG 90% CÁC BÀI TOÁN THỰC TẾ. Khi bạn chỉ cần lưu trữ và kiểm tra
      "Thằng này có tồn tại trong danh sách không?", Bảng băm O(1) là Vua.
    - Ví dụ: Danh sách ID người dùng đã bị chặn (Blacklist), Danh sách từ khóa
 đã lọc.

 2. BAO GIỜ THÌ PHẢI QUAY VỀ std::set?
    - Khi bạn cần IN RA DỮ LIỆU THEO THỨ TỰ TĂNG DẦN/GIẢM DẦN.
    - Khi bạn cần dùng các hàm lower_bound, upper_bound để tìm "Cận trên/Cận
 dưới" (Hash Table KHÔNG làm được trò này vì nó lộn xộn).
    - Khi bạn tự viết Class Custom và lười viết Hàm Băm (Hash), viết operator <
 cho std::set dễ hơn nhiều.
*/

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  unordered_set_co_ban();
  unordered_multiset_thao_tac();
  unordered_set_kien_truc_bucket();
  unordered_set_custom_struct();
  unordered_set_cpp17_merge();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}