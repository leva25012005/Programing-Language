#include <functional> // Cung cấp std::hash
#include <iostream>
#include <string>
#include <unordered_map>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Cấu trúc dưới nền: BẢNG BĂM (Hash Table). Giống std::unordered_set nhưng lưu
 theo CẶP (Key-Value).
 2. Cơ chế: Key được băm (hash) thành một số nguyên, sau đó nhét Value vào cái
 Thùng (Bucket) tương ứng.
 3. Đặc tính cốt lõi:
    - KHÔNG SẮP XẾP: Các cặp Key-Value nằm lộn xộn trong bộ nhớ, không theo thứ
 tự nào cả.
    - SIÊU TỐC ĐỘ: Tìm kiếm, Thêm, Xóa trung bình luôn là O(1) (tốc độ hằng số).
 4. Sự khác biệt:
    - std::unordered_map: Key là DUY NHẤT. Dùng toán tử [] thoải mái.
    - std::unordered_multimap: Cho phép NHIỀU Key trùng nhau. KHÔNG có toán tử
 [].
*/

// ==========================================
// CLASS & STRUCT PHỤ TRỢ CHO PHẦN CHUYÊN GIA
// ==========================================
struct ToaDo {
  int x, y;

  // 1. BẮT BUỘC SỐ 1: Toán tử == (Để xử lý đụng độ Hash Collision)
  bool operator==(const ToaDo &other) const {
    return (x == other.x && y == other.y);
  }
};

// 2. BẮT BUỘC SỐ 2: Viết Hàm Băm (Custom Hash) cho Key
struct ToaDoHash {
  size_t operator()(const ToaDo &td) const {
    size_t h1 = std::hash<int>()(td.x);
    size_t h2 = std::hash<int>()(td.y);
    return h1 ^ (h2 << 1); // Trộn mã băm bằng XOR và Bitwise Shift
  }
};

class VatPham {
public:
  std::string ten;
  VatPham() : ten("Trang bi rong") {
    std::cout << "  [Tao Default Vat Pham]\n";
  }
  VatPham(std::string t) : ten(t) {
    std::cout << "  [Tao Co Tham So] " << ten << "\n";
  }
};

// ==========================================
// 1. MỨC CƠ BẢN: TRUY XUẤT VÀ CẠM BẪY TOÁN TỬ []
// ==========================================
void unordered_map_co_ban() {
  std::cout << "\n--- 1. BAZIC: UNORDERED_MAP & TOAN TU [] ---\n";

  std::unordered_map<std::string, int> gia_ban;

  gia_ban["Kiem"] = 500;
  gia_ban["Khien"] = 300;
  gia_ban.insert({"Giap", 1000});

  // DUYỆT (Lưu ý kết quả in ra sẽ lộn xộn, không theo thứ tự bảng chữ cái)
  std::cout << "Bang gia (Lon xon):\n";
  for (const auto &[ten, gia] : gia_ban) {
    std::cout << "  - " << ten << ": " << gia << " Vang\n";
  }

  // CẠM BẪY TOÁN TỬ []: Nếu truy cập Key KHÔNG TỒN TẠI, nó sẽ TỰ ĐỘNG TẠO Key
  // đó!
  std::cout << "Gia cua 'Cung' (chua ton tai): " << gia_ban["Cung"]
            << "\n"; // In ra 0
  std::cout << "Kich thuoc hien tai: " << gia_ban.size()
            << " (Cung da bi them vao!)\n";

  // CÁCH AN TOÀN: Dùng .at() hoặc .find() / .contains() (C++20)
  try {
    std::cout << gia_ban.at("Giay") << "\n";
  } catch (const std::out_of_range &e) {
    std::cout << "-> Bắt lỗi: 'Giay' chua co trong cua hang.\n";
  }
}

// ==========================================
// 2. MỨC TRUNG CẤP: UNORDERED_MULTIMAP
// ==========================================
void unordered_multimap_thao_tac() {
  std::cout << "\n--- 2. INTERMEDIATE: UNORDERED_MULTIMAP ---\n";

  // Một học sinh có thể tham gia NHIỀU câu lạc bộ (Key trùng nhau)
  std::unordered_multimap<std::string, std::string> cau_lac_bo;

  cau_lac_bo.insert({"Nhan", "Bong Da"});
  cau_lac_bo.insert({"An", "Boi Loi"});
  cau_lac_bo.insert({"Nhan", "Am Nhac"});
  cau_lac_bo.insert({"Nhan", "Tin Hoc"});

  // KHÔNG CÓ TOÁN TỬ []: cau_lac_bo["Nhan"] là LỖI BIÊN DỊCH!

  // Lấy TẤT CẢ câu lạc bộ của "Nhan" ra bằng equal_range (O(1))
  std::cout << "Cac CLB ma Nhan tham gia:\n";
  auto [it_dau, it_cuoi] = cau_lac_bo.equal_range("Nhan");

  for (auto it = it_dau; it != it_cuoi; ++it) {
    std::cout << "  - " << it->second << "\n";
  }
}

// ==========================================
// 3. MỨC NÂNG CAO: BÍ MẬT BẢNG BĂM (BUCKETS)
// ==========================================
void unordered_map_buckets() {
  std::cout << "\n--- 3. ADVANCED: BUCKETS & REHASH ---\n";

  std::unordered_map<int, std::string> um;

  // NGUYÊN TẮC VÀNG: Luôn dùng reserve() nếu biết trước số lượng phần tử
  // Giúp tránh việc Bảng Băm đập đi xây lại (Rehash) làm giật lag.
  um.reserve(10);

  um[1] = "Mot";
  um[2] = "Hai";
  um[11] = "Muoi Mot";

  std::cout << "So Thung (Bucket): " << um.bucket_count() << "\n";
  std::cout << "Ti le lap day (Load Factor): " << um.load_factor() << "\n";

  // Khám phá các phần tử đang nằm chung 1 Thùng (Hash Collision)
  for (size_t i = 0; i < um.bucket_count(); ++i) {
    if (um.bucket_size(i) > 0) {
      std::cout << "  Thung #" << i << " chua: ";
      for (auto it = um.begin(i); it != um.end(i); ++it) {
        std::cout << "[" << it->first << ":" << it->second << "] ";
      }
      std::cout << "\n";
    }
  }
}

// ==========================================
// 4. MỨC CHUYÊN GIA: C++17 TRY_EMPLACE & CUSTOM KEY
// ==========================================
void unordered_map_cpp17_custom_key() {
  std::cout << "\n--- 4. EXPERT: TRY_EMPLACE & CUSTOM STRUCT KEY ---\n";

  // 1. C++17 try_emplace (Tối ưu hóa cực độ)
  std::unordered_map<int, VatPham> tui_do;

  std::cout << "> Dat 'Kiem' vao vi tri so 1:\n";
  tui_do.try_emplace(1, "Kiem Nhua"); // Thành công, khởi tạo "Kiem Nhua"

  std::cout << "\n> Co gang de 'Khien' vao vi tri so 1 (Da ton tai):\n";
  tui_do.try_emplace(1, "Khien Go");
  // try_emplace phát hiện Key 1 đã có, NÓ HỦY BỎ VIỆC TẠO "Khien Go" NGAY LẬP
  // TỨC! Sẽ không có dòng log "[Tao Co Tham So] Khien Go" nào in ra -> Tiết
  // kiệm CPU/RAM tuyệt đối.

  // 2. Sử dụng Custom Struct (ToaDo) làm Key
  std::unordered_map<ToaDo, std::string, ToaDoHash> ban_do;

  ban_do[{10, 10}] = "Kho Bau";
  ban_do[{0, 0}] = "Diem Xuat Phat";

  if (ban_do.contains({10, 10})) { // .contains() là của C++20
    std::cout << "\nTai toa do (10,10) co: " << ban_do[{10, 10}] << "\n";
  }
}

// ==========================================
// 5. HIỆN ĐẠI TỐI THƯỢNG (C++17): ĐỔI TÊN KEY (NODE EXTRACTION)
// ==========================================
void unordered_map_node_extraction() {
  std::cout << "\n--- 5. MODERN C++17: DOI TEN KEY O(1) ---\n";

  std::unordered_map<std::string, std::string> session_users = {
      {"token_abc", "Le Van Nhan"}, {"token_xyz", "Tran Thi B"}};

  // Muốn đổi Key "token_abc" thành "token_123" mà không cần xóa đi tạo lại
  // Value.
  auto node = session_users.extract("token_abc");

  if (!node.empty()) {
    node.key() =
        "token_123"; // Đổi Key trực tiếp khi Node đang lơ lửng ngoài RAM
    session_users.insert(std::move(node)); // Cắm lại vào Bảng Băm
  }

  for (const auto &[token, ten] : session_users) {
    std::cout << "  Token: " << token << " -> User: " << ten << "\n";
  }
}

// ==========================================
// TỔNG KẾT LUẬT CHỌN (MAP vs UNORDERED_MAP)
// ==========================================
/*
 1. LUÔN CHỌN `std::unordered_map` LÀM MẶC ĐỊNH:
    - Nếu bạn chỉ cần tra cứu dữ liệu (như Từ điển, Cache, Đếm số lần xuất
 hiện).
    - Hiệu suất O(1) của Bảng Băm vượt trội hoàn toàn so với O(log n) của Cây Đỏ
 Đen khi dữ liệu lớn.

 2. CHUYỂN SANG `std::map` KHI VÀ CHỈ KHI:
    - Yêu cầu bài toán bắt buộc phải in dữ liệu ra theo THỨ TỰ TĂNG DẦN / GIẢM
 DẦN.
    - Cần dùng `.lower_bound()` và `.upper_bound()` để tìm khoảng giá trị.
    - Bạn lười viết Hàm Băm (Hash Function) rườm rà cho các Struct tự định
 nghĩa.
*/

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  unordered_map_co_ban();
  unordered_multimap_thao_tac();
  unordered_map_buckets();
  unordered_map_cpp17_custom_key();
  unordered_map_node_extraction();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}