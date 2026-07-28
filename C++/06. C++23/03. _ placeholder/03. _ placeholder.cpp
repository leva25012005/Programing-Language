#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <tuple>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY - C++23)
// ==========================================
/*
 1. Biến bỏ qua `_` (Name-independent variables) là gì?
    - Trước C++23: Dấu gạch dưới `_` chỉ là một cái tên biến bình thường hợp lệ.
      Bạn chỉ được khai báo nó MỘT LẦN trong một phạm vi (scope). Nếu báo lần 2
 sẽ lỗi.
    - Từ C++23: `_` chính thức được nâng cấp thành "Biến vô danh" (Placeholder).
      Nó nói với Trình biên dịch (Compiler): "Tôi phải khai báo biến này cho đủ
 cú pháp, nhưng tôi sẽ KHÔNG BAO GIỜ xài tới nó!".

 2. Sức mạnh cốt lõi:
    - CHO PHÉP TRÙNG LẶP: Bạn có thể khai báo hàng chục biến tên `_` trong cùng
 một phạm vi (scope) mà không hề bị báo lỗi "Redefinition" (Định nghĩa lại).
    - TỐI ƯU CẢNH BÁO: Compiler sẽ tự động hiểu nó mang cờ ngầm
 `[[maybe_unused]]`, từ đó không văng ra những cảnh báo (warning) ngứa mắt như
 "unused variable".

 3. Cảnh báo: Code dưới đây CHỈ chạy được trên Trình biên dịch hỗ trợ C++23
 (thêm cờ -std=c++23).
*/

// ==========================================
// HÀM PHỤ TRỢ MÔ PHỎNG DỮ LIỆU
// ==========================================
std::tuple<int, std::string, double, std::string> lay_ho_so_diep_vien() {
  // Trả về: ID, Tên thật, Lương, Mật danh
  return {007, "James Bond", 9999.99, "007-MI6"};
}

// ==========================================
// 1. MỨC CƠ BẢN: STRUCTURED BINDINGS (BUNG GÓI)
// ==========================================
void placeholder_co_ban() {
  std::cout << "\n--- 1. BAZIC: BỎ QUA GIA TRI TRONG TUPLE ---\n";

  // Yêu cầu: Ta chỉ quan tâm đến ID và Mật danh, không thèm nhìn tới Tên thật
  // và Lương. C++23: Dùng `_` nhiều lần thoải mái để lấp chỗ trống!
  auto [id, _, _, mat_danh] = lay_ho_so_diep_vien();

  std::cout << "  Lay duoc ID: " << id << "\n";
  std::cout << "  Lay duoc Mat danh: " << mat_danh << "\n";

  // Nếu bạn thử đọc biến `_` lúc này?
  // LỖI BIÊN DỊCH (Ambiguous reference): Vì có tới 2 biến `_`, C++ không biết
  // bạn gọi thằng nào. std::cout << _; // CẤM!
}

// ==========================================
// 2. MỨC TRUNG CẤP: DÙNG TRONG VÒNG LẶP FOR (MAP)
// ==========================================
void placeholder_vong_lap() {
  std::cout << "\n--- 2. INTERMEDIATE: BỎ QUA KEY/VALUE TRONG MAP ---\n";

  std::map<std::string, int> bang_diem = {{"Nhan", 10}, {"An", 9}, {"Binh", 8}};

  int tong_diem = 0;

  // Giả sử ta muốn tính tổng điểm của cả lớp, KHÔNG quan tâm Tên là gì.
  // Nếu dùng `auto [ten, diem]` thì Compiler sẽ phàn nàn biến `ten` không được
  // xài. C++23: Lấp `_` vào vị trí của Key!
  for (const auto &[_, diem] : bang_diem) {
    tong_diem += diem;
  }

  std::cout << "  Tong diem ca lop la: " << tong_diem << "\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: RAII VÀ ĐA KHÓA (MUTEX LOCKS)
// ==========================================
std::mutex khoa_nha;
std::mutex khoa_xe;

void placeholder_raii_locks() {
  std::cout << "\n--- 3. ADVANCED: DÙNG CHO CÁC ĐỐI TƯỢNG GUARD/RAII ---\n";

  // Kỹ thuật RAII: Tạo ra object chỉ để nó tự kích hoạt Constructor (Khóa)
  // và Destructor (Mở khóa) lúc hết hàm, hoàn toàn không cần gọi tên object đó.

  // C++17 (Lỗi):
  // std::lock_guard<std::mutex> _ {khoa_nha};
  // std::lock_guard<std::mutex> _ {khoa_xe};  <- Báo lỗi trùng tên biến '_'!

  // C++23 (Hợp lệ hoàn toàn):
  std::lock_guard<std::mutex> _{khoa_nha};
  std::lock_guard<std::mutex> _{khoa_xe};

  std::cout << "  Da khoa ca Nha lan Xe ma khong can dat ten thua thai (VD: "
               "lock1, lock2)!\n";

  // Vừa thoát hàm này, cả 2 dấu `_` tự động hủy, nhả khóa ra an toàn.
}

// ==========================================
// 4. MỨC CHUYÊN GIA: BÍ MẬT BACKWARDS COMPATIBILITY (TƯƠNG THÍCH NGƯỢC)
// ==========================================
void placeholder_chuyen_gia() {
  std::cout << "\n--- 4. EXPERT: TUONG THICH NGUOC (DOC DUOC NEU CHI CO 1 "
               "BIEN) ---\n";

  // Nếu bạn KHÔNG trùng lặp, nghĩa là trong scope này chỉ có duy nhất MỘT biến
  // `_`
  int _ = 42;

  // C++23 cực kỳ thông minh: Nó vẫn cho phép bạn ĐỌC giá trị của `_`
  // nhằm đảm bảo các code C++ cũ (dùng `_` làm tên biến thật) không bị hỏng!
  std::cout << "  Bien '_' duy nhat trong scope mang gia tri: " << _ << "\n";

  // NHƯNG nếu bạn thêm 1 biến `_` nữa vào đây...
  int _ = 99; // Hợp lệ ở C++23

  // ...Thì lập tức biến `_` trở thành "Un-nameable" (Vô Danh).
  // Dòng dưới đây nếu bỏ comment sẽ văng LỖI BIÊN DỊCH ngay:
  // std::cout << _; // LỖI: Reference to '_' is ambiguous (Có tới 2 biến _, gọi
  // ai?)
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== C++23 FEATURE: '_' PLACEHOLDER =====\n";

  placeholder_co_ban();
  placeholder_vong_lap();
  placeholder_raii_locks();
  placeholder_chuyen_gia();

  std::cout << "\n===== HOAN THANH (Nho build bang -std=c++23) =====\n";
  return 0;
}