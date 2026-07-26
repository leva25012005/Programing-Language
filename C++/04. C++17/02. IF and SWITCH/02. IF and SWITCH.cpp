#include <iostream>
#include <map>
#include <mutex>
#include <random>
#include <string>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY - C++17)
// ==========================================
/*
 1. Vấn đề trước C++17 (Scope Leak - Rác phạm vi):
    Khi bạn muốn kiểm tra một hàm trả về (ví dụ map.find()), bạn phải khai báo
 biến `it` ở BÊN NGOÀI lệnh if. Sau khi if kết thúc, biến `it` vẫn sống nhe răng
 trong RAM, gây tốn bộ nhớ và dễ bị gọi nhầm ở các dòng code bên dưới.

 2. Giải pháp C++17 (Init-statement):
    - Cú pháp: if ( Khởi_tạo_biến ; Điều_kiện_kiểm_tra ) { ... }
    - Cú pháp: switch ( Khởi_tạo_biến ; Điều_kiện ) { ... }

 3. Tác dụng thần kỳ:
    - Biến sinh ra ngay lúc mở ngoặc `if`, và BỊ TIÊU DIỆT ngay lập tức khi khối
 `if/else` kết thúc.
    - Giúp code cực kỳ sạch sẽ, kiểm soát bộ nhớ (RAII) tuyệt đối an toàn.
*/

// ==========================================
// 1. MỨC CƠ BẢN: IF VỚI INIT-STATEMENT
// ==========================================
void if_co_ban() {
  std::cout << "\n--- 1. BAZIC: IF KHOI TAO TRONG C++17 ---\n";

  std::map<int, std::string> danh_ba = {{1, "Nhan"}, {2, "An"}};

  // CÁCH CŨ (C++11): Biến 'it' bị tràn ra ngoài (Scope leak)
  auto it_cu = danh_ba.find(1);
  if (it_cu != danh_ba.end()) {
    std::cout << "Cach cu: Da tim thay " << it_cu->second << "\n";
  }
  // Ở đây 'it_cu' vẫn tồn tại! Trở thành rác nếu không dùng tới nữa.

  // CÁCH MỚI (C++17): Sạch sẽ, gọn gàng
  if (auto it_moi = danh_ba.find(2); it_moi != danh_ba.end()) {
    std::cout << "Cach moi (C++17): Da tim thay " << it_moi->second << "\n";
  }
  // Lệnh này SẼ BÁO LỖI BIÊN DỊCH vì 'it_moi' đã bốc hơi khỏi RAM:
  // std::cout << it_moi->second; // ERROR!
}

// ==========================================
// 2. MỨC TRUNG CẤP: SWITCH VỚI INIT-STATEMENT VÀ ELSE
// ==========================================
// Hàm giả lập lấy mã lỗi từ Server
int ket_noi_server() { return 404; }

void switch_va_else() {
  std::cout << "\n--- 2. INTERMEDIATE: SWITCH & ELSE SCOPE ---\n";

  // a. Ứng dụng trên SWITCH
  // Gọi hàm, lưu vào biến 'status', rồi đưa 'status' vào switch để kiểm tra
  switch (int status = ket_noi_server(); status) {
  case 200:
    std::cout << "Ket noi thanh cong (200)\n";
    break;
  case 404:
    std::cout << "Loi: Khong tim thay (404)\n";
    break;
  default:
    std::cout << "Ma loi khong xac dinh: " << status << "\n";
    break;
  }
  // Tương tự, biến 'status' đã bị hủy ở đây.

  // b. BÍ MẬT CỦA ELSE
  // Biến khởi tạo trong 'if' VẪN CÒN TỒN TẠI trong các khối 'else if' và 'else'
  // đi kèm!
  std::map<int, std::string> ds = {{10, "Muoi"}};

  if (auto it = ds.find(99); it != ds.end()) {
    std::cout << "Tim thay: " << it->second << "\n";
  } else {
    // Hoàn toàn HỢP LỆ! Biến 'it' vẫn sống trong khối else này
    // (để ta có thể kiểm tra xem nó có bằng end() hay không).
    if (it == ds.end()) {
      std::cout << "Vao khoi ELSE: Key 99 khong ton tai!\n";
    }
  }
}

// ==========================================
// 3. MỨC NÂNG CAO: COMBO THẦN THÁNH VỚI STRUCTURED BINDINGS
// ==========================================
void if_voi_structured_bindings() {
  std::cout << "\n--- 3. ADVANCED: IF + STRUCTURED BINDINGS ---\n";

  std::map<int, std::string> kho_hang;
  kho_hang.insert({100, "Kiem"});

  // VẤN ĐỀ: Hàm insert() của map trả về 1 std::pair<Iterator, bool (Thành công
  // hay ko)>. C++17 cho phép bạn VỪA Bung gói (Unpack) Pair này, VỪA đưa nó vào
  // lệnh if!

  // Cú pháp tối thượng:
  // 1. Bung gói kết quả trả về thành 2 biến: 'it' (con trỏ) và 'thanh_cong'
  // (true/false)
  // 2. Dấu chấm phẩy ;
  // 3. Kiểm tra biến 'thanh_cong'
  if (auto [it, thanh_cong] = kho_hang.insert({100, "Khien"}); thanh_cong) {
    std::cout << "Da them thanh cong mon do: " << it->second << "\n";
  } else {
    std::cout << "Them that bai! Key 100 da bi chiem boi: " << it->second
              << "\n";
  }
}

// ==========================================
// 4. MỨC CHUYÊN GIA: QUẢN LÝ TÀI NGUYÊN & MULTITHREADING (ĐA LUỒNG)
// ==========================================
std::mutex mtx; // Khóa đa luồng (Mutex)
std::string du_lieu_dung_chung = "Data quan trong";

void do_du_lieu_tu_luong_khac() {
  std::cout << "\n--- 4. EXPERT: MULTITHREADING LOCK & POINTERS ---\n";

  // TRONG LẬP TRÌNH ĐA LUỒNG (Multithreading):
  // Bạn thường phải "Khóa" (Lock) dữ liệu lại trước khi kiểm tra nó.
  // Nếu quên Mở khóa (Unlock), chương trình sẽ bị treo (Deadlock).

  // Bằng cách dùng If Init-statement, Khóa sẽ tự động MỞ NGAY LẬP TỨC
  // khi khối if kết thúc, tuyệt đối an toàn và không giữ khóa lâu hơn mức cần
  // thiết!

  if (std::lock_guard<std::mutex> khoa(mtx); !du_lieu_dung_chung.empty()) {
    std::cout << "Da khoa Mutex an toan! Du lieu dang doc: "
              << du_lieu_dung_chung << "\n";
    // Làm gì đó tốn thời gian ở đây...
  }
  // Ngay tại dấu ngoặc nhọn này, 'khoa' bị hủy -> Mutex TỰ ĐỘNG MỞ KHÓA
  // (Unlock). Các luồng (Thread) khác không bị chờ đợi oan uổng.

  std::cout << "Mutex da duoc mo, cac Thread khac co the truy cap du lieu.\n";
}

// MỘT VÍ DỤ CHUYÊN GIA KHÁC: Xử lý Con trỏ thô (Raw Pointers)
struct NhanVien {
  std::string ten = "Boss";
};

NhanVien *tim_nhan_vien() {
  return new NhanVien(); // Giả lập tìm thấy nhân viên
}

void xu_ly_con_tro() {
  std::cout << "\n--- UNG DUNG KIEM TRA CON TRO TUC THI ---\n";

  // Gọi hàm trả về con trỏ, gắn vào 'ptr'. Kiểm tra ngay 'ptr' có bị NULL
  // không.
  if (NhanVien *ptr = tim_nhan_vien(); ptr != nullptr) {
    std::cout << "Da tim thay nhan vien: " << ptr->ten << "\n";
    delete ptr; // Tránh rò rỉ bộ nhớ (Thực tế nên dùng std::unique_ptr)
  }
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  if_co_ban();
  switch_va_else();
  if_voi_structured_bindings();
  do_du_lieu_tu_luong_khac();
  xu_ly_con_tro();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}