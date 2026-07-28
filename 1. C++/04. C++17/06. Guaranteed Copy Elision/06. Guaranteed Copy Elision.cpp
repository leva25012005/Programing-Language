#include <iostream>
#include <string>
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Vấn đề trước C++17:
    Khi một hàm trả về (return) một Đối tượng (Object), C++ truyền thống sẽ tạo
 ra một Object tạm trong hàm -> Copy/Move nó ra ngoài cho biến hứng -> Hủy
 Object tạm. Việc này vô cùng lãng phí CPU và RAM nếu Object đó là một Ma trận
 1000x1000.

 2. Giải pháp C++17 (Guaranteed Copy Elision):
    C++17 ra ĐẠO LUẬT cấm trình biên dịch thực hiện Copy/Move khi trả về một
 Object tạm thời (prvalue). Thay vì tạo ra ở trong hàm rồi Copy ra ngoài, C++17
 sẽ cấp phát VÙNG NHỚ CỦA BIẾN HỨNG Ở BÊN NGOÀI, rồi luồn con trỏ vùng nhớ đó
 vào trong hàm để khởi tạo TRỰC TIẾP.
    => Chi phí Copy/Move giảm xuống đúng bằng 0 byte, 0 chu kỳ CPU!

 3. Hệ quả vĩ đại:
    Bạn có thể thoải mái viết các hàm "Factory" trả về những Object nặng hàng
 Gigabyte, hoặc thậm chí trả về những Object bị "Cấm Copy/Cấm Move" (như
 std::mutex).
*/

// ==========================================
// CLASS PHỤ TRỢ (Để theo dõi quá trình Copy/Move)
// ==========================================

// 1. Class theo dõi sự kiện
class DoiTuongKhongLo {
public:
  DoiTuongKhongLo() { std::cout << "  [+] Da KHOI TAO truc tiep tren RAM!\n"; }
  DoiTuongKhongLo(int x) {
    std::cout << "  [+] KHOI TAO truc tiep voi tham so (" << x << ")\n";
  }

  // Copy Constructor (Tốn kém - Ta không muốn cái này chạy)
  DoiTuongKhongLo(const DoiTuongKhongLo &) {
    std::cout << "  [!] COPY Constructor duoc goi (Ton kem CPU/RAM!)\n";
  }

  // Move Constructor (Nhanh hơn, nhưng vẫn tốn 1 chút chi phí chuyển giao)
  DoiTuongKhongLo(DoiTuongKhongLo &&) noexcept {
    std::cout
        << "  [~] MOVE Constructor duoc goi (Nhanh, nhung van ton chi phi)\n";
  }

  ~DoiTuongKhongLo() { std::cout << "  [-] Da HUY doi tuong\n"; }
};

// 2. Class BỊ CẤM Copy và Move (Mô phỏng các object hệ thống như std::mutex,
// thread)
class DoiTuongBatDong {
public:
  DoiTuongBatDong() { std::cout << "  [+] Khoi tao Doi tuong BAT DONG\n"; }

  // = delete nghĩa là CẤM SỬ DỤNG. Hễ đụng vào là LỖI BIÊN DỊCH.
  DoiTuongBatDong(const DoiTuongBatDong &) = delete;            // Cấm Copy
  DoiTuongBatDong(DoiTuongBatDong &&) = delete;                 // Cấm Move
  DoiTuongBatDong &operator=(const DoiTuongBatDong &) = delete; // Cấm gán Copy
  DoiTuongBatDong &operator=(DoiTuongBatDong &&) = delete; // Cấm gán Move
};

// ==========================================
// 1. MỨC CƠ BẢN: TRẢ VỀ OBJECT TẠM THỜI (PRVALUE)
// ==========================================
// Hàm này trả về một Object tạm thời vô danh
DoiTuongKhongLo tao_doi_tuong_co_ban() {
  std::cout << "  -> Dang o trong ham tao_doi_tuong_co_ban()...\n";
  // Trong C++11/14: Lệnh này có thể kích hoạt Move Constructor hoặc RVO (Tùy
  // hứng của Compiler). Trong C++17: BẮT BUỘC không có bất kỳ Copy hay Move nào
  // được diễn ra.
  return DoiTuongKhongLo();
}

void demo_co_ban() {
  std::cout << "\n--- 1. BAZIC: GUARANTEED COPY ELISION ---\n";
  // Biến obj được tạo ra. Vùng nhớ của obj được luồn thẳng vào hàm
  // tao_doi_tuong_co_ban().
  DoiTuongKhongLo obj = tao_doi_tuong_co_ban();

  // Kết quả: Bạn sẽ CHỈ THẤY ĐÚNG 1 DÒNG "[+] Da KHOI TAO truc tiep tren RAM!".
  // Không hề có dòng [!] Copy hay [~] Move nào xuất hiện.
}

// ==========================================
// 2. MỨC NÂNG CAO: TRẢ VỀ OBJECT "CẤM DI CHUYỂN"
// ==========================================
DoiTuongBatDong tao_doi_tuong_bat_dong() {
  std::cout << "  -> Dang o trong ham tao_doi_tuong_bat_dong()...\n";
  return DoiTuongBatDong();
}

void demo_nang_cao() {
  std::cout << "\n--- 2. ADVANCED: TRA VE OBJECT CAM COPY/MOVE ---\n";

  // ĐIỀU KỲ DIỆU CỦA C++17 NẰM Ở ĐÂY!
  // Nếu bạn compile đoạn code này bằng C++14 -> LỖI ĐỎ CHÓT NGAY LẬP TỨC!
  // Vì C++14 bắt buộc class phải có Copy/Move Constructor hợp lệ thì nó mới cho
  // return. C++17 biết chắc chắn nó KHÔNG CẦN copy/move, nên nó CHO PHÉP đoạn
  // code này chạy hợp lệ!

  DoiTuongBatDong block = tao_doi_tuong_bat_dong();
}

// ==========================================
// 3. MỨC CHUYÊN GIA: KHỞI TẠO CHUYỂN TIẾP (FACTORY PATTERN)
// ==========================================
// Kỹ thuật này thường dùng để tạo các Object cấu hình phức tạp
DoiTuongKhongLo factory_tao_vu_khi(int sat_thuong) {
  std::cout << "  -> [Factory] Bat dau che tao vu khi...\n";
  return DoiTuongKhongLo(
      sat_thuong); // Truyền tham số để khởi tạo trực tiếp ở đích đến
}

void demo_chuyen_gia_factory() {
  std::cout << "\n--- 3. EXPERT: FACTORY PATTERN 0-COST ---\n";

  // Lệnh này trông có vẻ như gọi hàm tạo ra 1 object, rồi gán sang object khác.
  // Nhưng thực tế dưới nền RAM: Hàm factory tự gọi constructor(999) trực tiếp
  // lên vùng nhớ của 'super_weapon'.
  DoiTuongKhongLo super_weapon = factory_tao_vu_khi(999);
}

// ==========================================
// 4. CẠM BẪY CHÍ MẠNG: NRVO (Named Return Value Optimization)
// ==========================================
// C++17 CHỈ BẮT BUỘC tối ưu (Guaranteed) đối với Object Vô Danh (prvalue như
// 'return Type();'). Nếu bạn KHAI BÁO CÓ TÊN (lvalue) rồi mới return, nó rơi
// vào trường hợp NRVO.
DoiTuongKhongLo cạm_bay_nrvo() {
  std::cout << "  -> Dang o trong ham cạm_bay_nrvo()...\n";

  DoiTuongKhongLo bien_cuc_bo(50); // Đã đặt tên là 'bien_cuc_bo'

  // Khi return 1 biến có tên, Compiler THƯỜNG SẼ TỐI ƯU (bỏ qua copy/move).
  // NHƯNG ĐÂY KHÔNG PHẢI LUẬT BẮT BUỘC (Not Guaranteed).
  // Vì vậy, Compiler BẮT BUỘC class của bạn phải MỞ KHÓA Move hoặc Copy
  // Constructor để dự phòng. Nếu class bị 'delete' Copy/Move như
  // (DoiTuongBatDong), đoạn code này sẽ báo lỗi biên dịch!
  return bien_cuc_bo;
}

void demo_cam_bay() {
  std::cout << "\n--- 4. WARNING: NRVO vs PRVALUE ---\n";

  DoiTuongKhongLo obj = cạm_bay_nrvo();
  // Chạy trên IDE hiện đại, nó vẫn thường tối ưu thành công (Không gọi
  // Copy/Move). Nhưng về mặt ngữ nghĩa chuẩn (Standard Specification), compiler
  // bắt buộc phải kiểm tra xem hàm Move/Copy có tồn tại và hợp lệ hay không.
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== C++17 GUARANTEED COPY ELISION =====\n";

  demo_co_ban();
  demo_nang_cao();
  demo_chuyen_gia_factory();
  demo_cam_bay();

  std::cout << "\n===== HOAN THANH KHOI CHAY =====\n";
  return 0;
}