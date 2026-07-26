#include <iostream>
#include <map>
#include <string>
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Range-based for loop là gì?
    - Ra mắt từ C++11, nó là "Cú pháp ngọt ngào" (Syntactic sugar) giúp duyệt
 qua toàn bộ một vật chứa (Container) hoặc mảng một cách cực kỳ ngắn gọn và an
 toàn.

 2. Bản chất dưới nền: Trình biên dịch C++ sẽ âm thầm dịch vòng lặp này thành:
    for (auto it = v.begin(); it != v.end(); ++it) {
        auto phan_tu = *it;
        // ... code của bạn ...
    }
    -> Do đó, nó KHÔNG hề chậm hơn vòng lặp Iterator truyền thống (Hiệu năng
 bằng nhau tuyệt đối).

 3. Điều kiện để một cấu trúc dữ liệu dùng được Range-based for:
    - Cấu trúc đó phải có sẵn 2 hàm: .begin() và .end() (Hoặc có hàm
 std::begin(x) hỗ trợ).
*/

// ==========================================
// CLASS PHỤ TRỢ CHO PHẦN CHUYÊN GIA
// ==========================================
// Giả sử ta muốn dùng vòng lặp cho một Class tự định nghĩa (Không phải vector
// chuẩn)
class DoiBong {
private:
  std::vector<std::string> cau_thu;

public:
  DoiBong() { cau_thu = {"Ronaldo", "Messi", "Neymar"}; }

  // BÍ QUYẾT ĐỂ RANGE-BASED FOR HOẠT ĐỘNG:
  // Phải cung cấp 2 hàm begin() và end() trả về Iterator.
  auto begin() const { return cau_thu.begin(); }
  auto end() const { return cau_thu.end(); }
};

// ==========================================
// 1. MỨC CƠ BẢN: QUY TẮC "COPY" VÀ "THAM CHIẾU"
// ==========================================
void range_for_co_ban() {
  std::cout << "\n--- 1. BAZIC: AUTO, AUTO& VÀ CONST AUTO& ---\n";

  std::vector<std::string> vu_khi = {"Kiem", "Cung", "Gay"};

  // a. ĐỌC DỮ LIỆU BẰNG COPY (KHÔNG KHUYÊN DÙNG VỚI OBJECT LỚN)
  // Từng chữ "Kiem", "Cung"... sẽ bị HỆ ĐIỀU HÀNH COPY SANG một vùng RAM mới
  // tên là 'vk'. Vừa tốn RAM, vừa tốn thời gian.
  for (auto vk : vu_khi) {
    vk = "Da bi gay"; // Sửa bản copy, KHÔNG ảnh hưởng mảng gốc
  }

  // b. SỬA DỮ LIỆU BẰNG THAM CHIẾU (auto&)
  // 'vk' lúc này là BẢN THÂN phần tử trong mảng. Không có phép copy nào xảy ra.
  for (auto &vk : vu_khi) {
    vk += " (Da cuong hoa)"; // Sửa trực tiếp vào mảng gốc
  }

  // c. CHỈ ĐỌC DỮ LIỆU TỐI ƯU (const auto&) - CHUẨN MỰC KHI ĐI LÀM (BEST
  // PRACTICE) Hoàn toàn không copy, và Compiler khóa không cho bạn lỡ tay sửa
  // dữ liệu.
  std::cout << "Danh sach sau khi cuong hoa:\n";
  for (const auto &vk : vu_khi) {
    std::cout << "  - " << vk << "\n";
  }
}

// ==========================================
// 2. MỨC TRUNG CẤP: DÙNG VỚI MẢNG CŨ VÀ KHỞI TẠO TRỰC TIẾP
// ==========================================
void range_for_mang_va_khoi_tao() {
  std::cout << "\n--- 2. INTERMEDIATE: C-ARRAY & BRACE-INIT-LIST ---\n";

  // a. Hoạt động trơn tru với Mảng tĩnh kiểu C cổ điển
  // (Lưu ý: Mảng C không có hàm .begin(), nhưng C++ tự động dùng std::begin()
  // dưới nền)
  int mang_cu[] = {10, 20, 30};
  std::cout << "Duyet mang C: ";
  for (int &so : mang_cu) {
    so *= 2; // X2 giá trị
    std::cout << so << " ";
  }
  std::cout << "\n";

  // b. Duyệt trực tiếp qua một danh sách khởi tạo (Brace-init-list)
  // Không cần phải tạo ra biến vector nào cả!
  std::cout << "Duyet truc tiep: ";
  for (const char *ten : {"Nhan", "An", "Binh"}) {
    std::cout << ten << " ";
  }
  std::cout << "\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: DUYỆT MAP VỚI STRUCTURED BINDINGS (C++17)
// ==========================================
void range_for_cxx17() {
  std::cout << "\n--- 3. ADVANCED: C++17 STRUCTURED BINDINGS ---\n";

  std::map<int, std::string> danh_ba = {{101, "Le Van Nhan"},
                                        {102, "Tran Thi B"}};

  // TRƯỚC C++17: Phải dùng .first và .second rất xấu
  // for (const auto& cap : danh_ba) { cout << cap.first << " " << cap.second; }

  // TỪ C++17: Bung gói (Unpack) Pair trực tiếp ngay trong vòng lặp!
  // Cực kỳ thanh lịch, giống hệt Python hay JavaScript.
  for (const auto &[id, ten] : danh_ba) {
    std::cout << "  ID: " << id << " -> Ten: " << ten << "\n";
  }
}

// ==========================================
// 4. MỨC CHUYÊN GIA: C++20 INIT-STATEMENT VÀ CLASS TÙY CHỈNH
// ==========================================
void range_for_cxx20_va_class() {
  std::cout << "\n--- 4. EXPERT: C++20 INIT-STATEMENT & CUSTOM CLASS ---\n";

  // a. ÁP DỤNG LÊN CLASS TỰ VIẾT
  // Class DoiBong không hề kế thừa vector, nhưng vì ta đã định nghĩa begin() và
  // end(), C++ vẫn cho phép Range-based for "luồn" vào trong để duyệt!
  DoiBong vn_team;
  std::cout << "Danh sach doi bong (Custom Class):\n";
  for (const auto &cau_thu : vn_team) {
    std::cout << "  - " << cau_thu << "\n";
  }

  // b. C++20 INIT-STATEMENT (CHÈN BIẾN KHỞI TẠO VÀO RANGE-FOR)
  // NHƯỢC ĐIỂM KINH ĐIỂN của Range-for trước C++20: Nó KHÔNG CÓ BIẾN ĐẾM (index
  // i). Nếu bạn muốn in ra "1. Ronaldo, 2. Messi...", bạn phải tạo biến 'i' ở
  // ngoài vòng lặp, làm biến 'i' bị rò rỉ (leak) ra phạm vi bên ngoài, gây rác
  // code. TỪ C++20: Khai báo biến 'i' ngay bên trong Range-for, giống y hệt
  // vòng lặp for cổ điển!

  std::cout << "\nDem Index voi C++20 Init-statement:\n";
  std::vector<std::string> ds = {"A", "B", "C"};

  // Cú pháp C++20: for (Khởi_tạo ; auto& biến : vật_chứa)
  for (int i = 1; const auto &phan_tu : ds) {
    std::cout << "  Top " << i++ << ": " << phan_tu << "\n";
  }
  // Vừa ra khỏi vòng lặp này, biến 'i' sẽ bị tiêu hủy lập tức -> Code siêu
  // sạch!
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  range_for_co_ban();
  range_for_mang_va_khoi_tao();
  range_for_cxx17();
  range_for_cxx20_va_class();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}