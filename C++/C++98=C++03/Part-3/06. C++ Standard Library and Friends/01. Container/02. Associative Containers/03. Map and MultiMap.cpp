#include <functional> // Chứa std::greater
#include <iostream>
#include <map>
#include <string>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Bản chất: Giống như std::set, std::map được xây dựng bằng Cây Đỏ Đen
 (Red-Black Tree).
 2. Cấu trúc dữ liệu: Lưu trữ theo CẶP (Key - Value) dưới dạng std::pair<const
 Key, Value>.
    - Chìa khóa (Key) dùng để tìm kiếm và được TỰ ĐỘNG SẮP XẾP. Key là ĐỘC TÔN
 (không thể sửa đổi).
    - Giá trị (Value) là dữ liệu đi kèm với Key, có thể sửa đổi thoải mái.
 3. Sự khác biệt:
    - std::map: Mỗi Key là DUY NHẤT. Nếu thêm Key đã tồn tại, nó sẽ phớt lờ.
    - std::multimap: Cho phép NHIỀU Key trùng nhau (Ví dụ: Từ điển tiếng Anh, 1
 từ có nhiều nghĩa).
 4. Hiệu năng: Tìm kiếm, Thêm, Xóa đều tốn O(log n).
*/

// ==========================================
// CLASS PHỤ TRỢ CHO PHẦN NÂNG CAO
// ==========================================
struct SoSanhDoDaiTen {
  // Sắp xếp Từ điển ưu tiên Tên ngắn đứng trước, Tên dài đứng sau
  bool operator()(const std::string &a, const std::string &b) const {
    if (a.length() == b.length())
      return a < b;
    return a.length() < b.length();
  }
};

class SinhVien {
public:
  std::string ten;
  SinhVien() : ten("Vo Danh") { std::cout << "  [Tao Default]\n"; }
  SinhVien(std::string t) : ten(t) {
    std::cout << "  [Tao Co Tham So] " << ten << "\n";
  }
  // Copy Constructor
  SinhVien(const SinhVien &other) : ten(other.ten) {
    std::cout << "  [Copy] " << ten << "\n";
  }
};

// ==========================================
// 1. MỨC CƠ BẢN: MAP VÀ MULTIMAP
// ==========================================
void map_vs_multimap_co_ban() {
  std::cout << "\n--- 1. BAZIC: MAP & MULTIMAP ---\n";

  // a. std::map
  std::map<int, std::string> bang_diem;

  // 3 cách thêm dữ liệu vào map
  bang_diem.insert(std::make_pair(1, "Mot")); // Cách 1: Cổ điển
  bang_diem.insert({2, "Hai"});               // Cách 2: Ngắn gọn (C++11)
  bang_diem[3] = "Ba"; // Cách 3: Toán tử [] (Tiện lợi nhất)

  bang_diem[3] = "BA_MOI"; // Ghi đè Value của Key 3

  std::cout << "Duyet std::map (Giong cuon Tu dien):\n";
  // Sử dụng C++17 Structured Binding để tách Key và Value cho code đẹp
  for (const auto &[diem, ten] : bang_diem) {
    std::cout << "  Key: " << diem << " -> Value: " << ten << "\n";
  }

  // b. std::multimap
  std::multimap<std::string, std::string> tu_dien_anh_viet;

  // NHƯỢC ĐIỂM: multimap KHÔNG HỖ TRỢ toán tử [] vì 1 Key có thể có n Value,
  // C++ không biết bạn muốn lấy cái nào!
  tu_dien_anh_viet.insert({"Run", "Chay"});
  tu_dien_anh_viet.insert(
      {"Run", "Dieu Hanh"}); // Hoàn toàn hợp lệ, chứa 2 Key "Run"

  std::cout << "\nDuyet std::multimap:\n";
  for (const auto &[tu, nghia] : tu_dien_anh_viet) {
    std::cout << "  Tu: " << tu << " -> Nghia: " << nghia << "\n";
  }
}

// ==========================================
// 2. MỨC TRUNG CẤP: CẠM BẪY TOÁN TỬ [] VÀ .AT()
// ==========================================
void map_cam_bay_truy_xuat() {
  std::cout << "\n--- 2. INTERMEDIATE: CAM BAY TOAN TU [] ---\n";

  std::map<std::string, int> diem_thi;
  diem_thi["Nhan"] = 10;

  // CẠM BẪY KINH ĐIỂN CỦA C++:
  // Nếu bạn dùng [] để đọc một Key KHÔNG TỒN TẠI, C++ sẽ TỰ ĐỘNG TẠO Key đó
  // và gán Value mặc định (với int là số 0). Rác data sẽ sinh ra!
  std::cout << "Kiem tra diem cua An: " << diem_thi["An"] << "\n";

  // Lúc này, "An" đã chui vào trong map với điểm = 0!
  std::cout << "So luong sinh vien trong Map hien tai: " << diem_thi.size()
            << "\n"; // In ra 2

  // GIẢI PHÁP:
  // Nếu chỉ muốn ĐỌC dữ liệu an toàn, hãy dùng .at(). Nó sẽ ném lỗi nếu Key
  // không có.
  try {
    std::cout << diem_thi.at("Binh") << "\n";
  } catch (const std::out_of_range &e) {
    std::cout << "Binh khong ton tai! Ban da duoc an toan.\n";
  }
}

// ==========================================
// 3. MỨC NÂNG CAO: TÌM KIẾM TRONG MULTIMAP (equal_range)
// ==========================================
void map_tim_kiem_multimap() {
  std::cout << "\n--- 3. ADVANCED: MULTIMAP EQUAL_RANGE ---\n";

  std::multimap<std::string, std::string> danh_ba = {
      {"Nhan", "090-111"},
      {"An", "090-222"},
      {"Nhan", "090-333"}, // Nhan xài 2 số điện thoại
      {"Nhan", "090-444"}};

  // Làm sao để lấy ra TẤT CẢ các số điện thoại của "Nhan"?
  // Dùng equal_range(Key): Nó trả về 1 cặp con trỏ (Iterator) chứa cận ĐẦU và
  // cận CUỐI của dải Key đó.
  auto [it_dau, it_cuoi] = danh_ba.equal_range("Nhan");

  std::cout << "Cac so dien thoai cua Nhan la:\n";
  for (auto it = it_dau; it != it_cuoi; ++it) {
    std::cout << "  - " << it->second
              << "\n"; // it->first là Key, it->second là Value
  }
}

// ==========================================
// 4. MỨC CHUYÊN GIA: C++17 TỐI ƯU HÓA INSERT (try_emplace)
// ==========================================
void map_chuyen_gia_cpp17() {
  std::cout << "\n--- 4. EXPERT: C++17 TRY_EMPLACE & INSERT_OR_ASSIGN ---\n";

  std::map<int, SinhVien> ds_lop;

  // a. Vấn đề của hàm emplace() thông thường:
  // Mặc dù gọi là emplace, nhưng C++ vẫn phải gọi Constructor tạo Object Tạm
  // TRƯỚC để kiểm tra xem Key đó có tồn tại chưa. Nếu có rồi, nó ném Object tạm
  // vào sọt rác (cực kỳ phí CPU).
  std::cout << "Dung thu emplace (Key 1):\n";
  ds_lop.emplace(1, "Le Van Nhan"); // Key chưa có -> Tạo Object thành công

  std::cout << "Thu emplace LAI Key 1 (Da ton tai):\n";
  ds_lop.emplace(1, "Ke Pha Hoai"); // Key đã có -> Tự nhiên tạo ra 1 object tạm
                                    // "Ke Pha Hoai" rồi vứt đi!

  // b. Giải pháp C++17: try_emplace
  // Nó kiểm tra Key CÓ TỒN TẠI CHƯA trước khi đụng đến Value.
  // Nếu Key có rồi, nó BỎ QUA việc khởi tạo Value -> Tiết kiệm 100% tài nguyên!
  std::cout << "\nDung try_emplace (Key 1 da ton tai):\n";
  ds_lop.try_emplace(1, "Nguoi Vo Hinh"); // Hoàn toàn im lặng, không có Object
                                          // thừa nào được tạo ra!

  // c. C++17: insert_or_assign
  // Nếu Key CÓ CHƯA -> Tạo mới. Nếu Key CÓ RỒI -> Ghi đè Value.
  // Thay thế hoàn hảo cho toán tử [] khi muốn gán giá trị.
  ds_lop.insert_or_assign(1, SinhVien("Nhan Da Doi Ten"));
}

// ==========================================
// 5. HIỆN ĐẠI TỐI THƯỢNG (C++17): ĐỔI TÊN KEY (NODE EXTRACTION)
// ==========================================
void map_doi_ten_key_cpp17() {
  std::cout << "\n--- 5. MODERN C++17: NODE EXTRACTION (DOI TEN KEY) ---\n";

  // Trong Map, Key luôn là hằng số (const). Làm sao để sửa Key?
  // Trước C++17: Phải XÓA Key cũ -> THÊM Key mới (Phải cấp phát lại vùng nhớ,
  // mất thời gian Copy Value). Từ C++17: Bứng thẳng cục Node ra -> Sửa Key lơ
  // lửng -> Nhét lại vào Cây.

  std::map<int, std::string> id_nhan_vien = {{100, "Giam Doc"},
                                             {101, "Truong Phong"}};

  std::cout << "Truoc khi sua: Giam Doc co ID = 100\n";

  // 1. "Bứng" Node chứa Key 100 ra khỏi Map
  auto node = id_nhan_vien.extract(100);

  if (!node.empty()) {
    // 2. Thay đổi Key trực tiếp trên Node đang lơ lửng ngoài RAM
    node.key() = 999;

    // 3. Cắm Node trở lại Map
    id_nhan_vien.insert(std::move(node));
  }

  std::cout << "Sau khi extract va insert lai, duyet Map:\n";
  for (const auto &[id, chuc_vu] : id_nhan_vien) {
    std::cout << "  ID: " << id << " -> " << chuc_vu << "\n";
  }
}

// ==========================================
// 6. MAP TÙY CHỈNH LUẬT SẮP XẾP (CUSTOM COMPARATOR)
// ==========================================
void map_thay_doi_luat_sap_xep() {
  std::cout << "\n--- 6. CUSTOM SORT: XEP THEO DO DAI CHUOI ---\n";

  // Khai báo std::map với bộ so sánh tùy chỉnh 'SoSanhDoDaiTen'
  std::map<std::string, int, SoSanhDoDaiTen> tu_dien_diem = {
      {"Nhan", 9}, {"An", 10}, {"Christopher", 7}, {"Binh", 8}};

  // Kết quả: An (2 chữ cái) -> Nhan (4) -> Binh (4) -> Christopher (11)
  for (const auto &[ten, diem] : tu_dien_diem) {
    std::cout << ten << " (" << ten.length() << " ky tu): " << diem
              << " diem\n";
  }
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  map_vs_multimap_co_ban();
  map_cam_bay_truy_xuat();
  map_tim_kiem_multimap();
  map_chuyen_gia_cpp17();
  map_doi_ten_key_cpp17();
  map_thay_doi_luat_sap_xep();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}