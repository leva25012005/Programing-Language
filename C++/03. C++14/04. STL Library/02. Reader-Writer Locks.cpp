#include <chrono>
#include <iostream>
#include <map>
#include <mutex>
#include <shared_mutex> // C++14 cung cấp std::shared_timed_mutex, C++17 thêm std::shared_mutex
#include <string>
#include <thread>
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. BÀI TOÁN:
    - Nếu dùng std::mutex thông thường: Khi 100 luồng chỉ muốn ĐỌC dữ liệu,
 chúng vẫn phải xếp hàng chờ nhau từng đứa một. Rất lãng phí thời gian!

 2. GIẢI PHÁP: Khóa Đọc - Ghi (Reader-Writer Lock)
    - Cơ chế ĐỌC (Shared): Hàng nghìn luồng có thể CÙNG VÀO ĐỌC một lúc.
    - Cơ chế GHI (Exclusive): Khi có 1 luồng muốn GHI, nó phải đợi tất cả các
 luồng đọc đi ra hết. Khi nó đang GHI, KHÔNG MỘT AI (cả đọc lẫn ghi) được phép
 vào.

 3. CÔNG CỤ TRONG C++:
    - Vật quản lý (Lõi khóa): `std::shared_timed_mutex` (C++14) hoặc
 `std::shared_mutex` (C++17 - Nhanh hơn vì bỏ đi tính năng hẹn giờ).
    - Lớp bọc Đọc: `std::shared_lock<T>` -> Khóa chia sẻ.
    - Lớp bọc Ghi: `std::unique_lock<T>` -> Khóa độc quyền.
*/

// ==========================================
// CÔNG CỤ PHỤ TRỢ (Tránh in ra màn hình bị lộn xộn)
// ==========================================
std::mutex cout_mutex;
void safe_print(const std::string &msg) {
  std::lock_guard<std::mutex> lock(cout_mutex);
  std::cout << msg << "\n";
}

// ==========================================
// 1. MỨC CƠ BẢN: ĐỌC ĐỒNG THỜI - GHI ĐỘC QUYỀN
// ==========================================
class DuLieuDungChung {
private:
  int data = 0;
  // Lõi khóa đa năng (C++14)
  std::shared_timed_mutex rw_lock;

public:
  // HÀM ĐỌC: Cho phép nhiều luồng chạy song song
  void doc_du_lieu(int id_luong) {
    // Sử dụng SHARED_LOCK: Các luồng Đọc không hề chặn nhau!
    std::shared_lock<std::shared_timed_mutex> lock(rw_lock);

    safe_print("[LUONG DOC " + std::to_string(id_luong) + "] Bat dau doc...");
    std::this_thread::sleep_for(
        std::chrono::milliseconds(500)); // Giả lập tốn thời gian
    safe_print("[LUONG DOC " + std::to_string(id_luong) +
               "] Hoan thanh. Gia tri = " + std::to_string(data));
  }

  // HÀM GHI: Chặn TẤT CẢ mọi luồng khác
  void ghi_du_lieu(int id_luong, int gia_tri_moi) {
    // Sử dụng UNIQUE_LOCK: Phải đợi Cả Đọc và Ghi khác đi ra hết mới được vào
    std::unique_lock<std::shared_timed_mutex> lock(rw_lock);

    safe_print(">>> [LUONG GHI " + std::to_string(id_luong) +
               "] DA KHOA DOC QUYEN! Nguoi khac phai doi...");
    data = gia_tri_moi;
    std::this_thread::sleep_for(
        std::chrono::milliseconds(1000)); // Quá trình ghi tốn 1 giây
    safe_print(">>> [LUONG GHI " + std::to_string(id_luong) +
               "] Ghi xong. Xa khoa!");
  }
};

void demo_co_ban() {
  std::cout << "\n--- 1. BAZIC: SHARED LOCK vs UNIQUE LOCK ---\n";
  DuLieuDungChung db;
  std::vector<std::thread> danh_sach_luong;

  // Chạy 3 luồng đọc (Chúng sẽ chạy CÙNG LÚC vì dùng Shared Lock)
  for (int i = 1; i <= 3; ++i)
    danh_sach_luong.emplace_back(&DuLieuDungChung::doc_du_lieu, &db, i);

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  // Chạy 1 luồng ghi. Nó phải ĐỢI 3 luồng đọc trên xong, rồi mới Khóa Độc
  // Quyền.
  danh_sach_luong.emplace_back(&DuLieuDungChung::ghi_du_lieu, &db, 99, 100);

  // Chạy thêm 2 luồng đọc. Chúng sẽ bị CHẶN ĐỨNG ở ngoài cửa cho đến khi luồng
  // Ghi làm xong.
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  for (int i = 4; i <= 5; ++i)
    danh_sach_luong.emplace_back(&DuLieuDungChung::doc_du_lieu, &db, i);

  for (auto &t : danh_sach_luong)
    t.join();
}

// ==========================================
// 2. MỨC TRUNG CẤP: KHÓA HẸN GIỜ (TIMED MUTEX)
// ==========================================
// Điểm khác biệt lớn nhất của shared_timed_mutex (C++14) so với shared_mutex
// (C++17) là khả năng "Đợi một lúc, nếu không được thì bỏ cuộc" (Timeout).

class DuLieuHenGio {
private:
  std::shared_timed_mutex rw_lock;

public:
  void ghi_cham_chap() {
    std::unique_lock<std::shared_timed_mutex> lock(rw_lock);
    safe_print("[GHI CHINH] Dang nam giu khoa 2 giay...");
    std::this_thread::sleep_for(std::chrono::seconds(2));
  }

  void thu_doc_co_hen_gio() {
    // Cố gắng xin Khóa Đọc, nhưng chỉ kiên nhẫn đợi tối đa 500ms
    std::shared_lock<std::shared_timed_mutex> lock(rw_lock, std::defer_lock);

    if (lock.try_lock_shared_for(std::chrono::milliseconds(500))) {
      safe_print("[DOC] Tuyet voi! Lay duoc khoa roi.");
      // lock sẽ tự động mở khi hết hàm
    } else {
      safe_print(
          "[DOC] Doi 500ms roi ma khong ai cho vao. Toi bo cuoc (Timeout)!");
      // Ứng dụng thực tế: Báo lỗi cho người dùng, hoặc quay ra làm việc khác
      // thay vì treo game/app.
    }
  }
};

void demo_timeout() {
  std::cout << "\n--- 2. INTERMEDIATE: TRY_LOCK_FOR (Hẹn giờ) ---\n";
  DuLieuHenGio db;

  // Một luồng ghi giữ cửa 2 giây
  std::thread t_ghi(&DuLieuHenGio::ghi_cham_chap, &db);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  // Luồng đọc này chỉ đợi 500ms -> Chắc chắn sẽ thất bại và in ra thông báo "Bo
  // cuoc"
  std::thread t_doc(&DuLieuHenGio::thu_doc_co_hen_gio, &db);

  t_ghi.join();
  t_doc.join();
}

// ==========================================
// 3. MỨC NÂNG CAO: BỘ NHỚ ĐỆM DNS (CACHE) THỰC TẾ
// ==========================================
// Đây là bài toán điển hình nhất của Reader-Writer Lock:
// Đọc thì liên tục (Hàng nghìn request mỗi giây), nhưng Ghi (Cập nhật Cache)
// thì rất hiếm khi xảy ra.
class DNSCache {
private:
  std::map<std::string, std::string> cache;
  std::shared_timed_mutex
      cache_lock; // Hoặc std::shared_mutex (C++17) cho nhanh hơn
public:
  // Hàng nghìn luồng có thể tra cứu IP cùng lúc
  std::string tim_ip(const std::string &domain) {
    std::shared_lock<std::shared_timed_mutex> lock(cache_lock);
    auto it = cache.find(domain);
    if (it != cache.end()) {
      return it->second;
    }
    return "Khong Tim Thay";
  }

  // Hiếm khi mới có 1 luồng cập nhật Cache. Nhưng khi nó cập nhật, cấm ai được
  // đọc mảng dở dang.
  void cap_nhat_dns(const std::string &domain, const std::string &ip) {
    std::unique_lock<std::shared_timed_mutex> lock(cache_lock);
    cache[domain] = ip;
    safe_print("    [SYSTEM] Da cap nhat IP cho: " + domain);
  }
};

void demo_ung_dung_thuc_te() {
  std::cout << "\n--- 3. ADVANCED: UNG DUNG DNS CACHE ---\n";
  DNSCache dns;
  dns.cap_nhat_dns("google.com", "8.8.8.8");

  // Khởi tạo luồng đọc liên tục
  auto doc_lien_tuc = [&dns](int id) {
    for (int i = 0; i < 3; ++i) {
      std::string ip = dns.tim_ip("google.com");
      safe_print("[User " + std::to_string(id) + "] Tra cuu google.com -> " +
                 ip);
      std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
  };

  std::vector<std::thread> users;
  for (int i = 1; i <= 3; ++i)
    users.emplace_back(doc_lien_tuc, i);

  // Kẻ xen ngang cập nhật hệ thống
  std::this_thread::sleep_for(std::chrono::milliseconds(30));
  dns.cap_nhat_dns("google.com", "8.8.4.4"); // Lock độc quyền!

  for (auto &u : users)
    u.join();
}

// ==========================================
// 4. MỨC CHUYÊN GIA: CẠM BẪY "WRITE STARVATION"
// ==========================================
/*
 KHÁI NIỆM WRITE STARVATION (Nạn đói của luồng Ghi):
  Giả sử luồng Đọc 1 (R1) đang giữ Khóa Đọc.
  -> Luồng Ghi (W1) đến, phải đứng ngoài xếp hàng.
  -> Lúc W1 đang chờ, luồng Đọc 2 (R2) đến. Hệ điều hành thấy: "À, đang ở chế độ
 Đọc (Shared), vậy cho R2 vào luôn!".
  -> R1 xong đi ra, nhưng R2 vẫn ở trong.
  -> Luồng Đọc 3 (R3) lại đến, và lại được vào.
  -> HẬU QUẢ: W1 xếp hàng từ rất sớm nhưng BỊ BỎ ĐÓI MÃI MÃI, vì lượng người
 "Đọc" cứ liên tục đi vào cửa Shared mà không bao giờ vắng ngắt để W1 kịp lẻn
 vào cửa Unique.

 GIẢI PHÁP TRONG C++:
  Tin vui: Standard C++ (từ đa số các trình biên dịch như GCC, Clang, MSVC) đã
 thiết kế std::shared_mutex thiên vị luồng Ghi (Writer-Preference). Tức là: Ngay
 khi W1 nộp đơn xin vào (unique_lock), hệ thống sẽ DỪNG KHÔNG CHO các luồng Đọc
 (R2, R3) mới chui vào cửa nữa. R2 và R3 phải đứng ngoài xếp hàng sau lưng W1.
 W1 chỉ đợi R1 làm xong là được vào ngay.

 LƯU Ý KHI ĐI LÀM:
  1. Không có cơ chế HẠ CẤP/NÂNG CẤP KHÓA AN TOÀN (Lock Upgrade/Downgrade) trong
 C++ chuẩn. Bạn không thể đang cầm shared_lock rồi phù phép biến nó thành
 unique_lock (Sẽ gây Deadlock lập tức). Bạn phải: `shared_lock.unlock() ->
 unique_lock.lock()`.
  2. Nếu thời gian thao tác (đọc/ghi) RẤT NGẮN (dưới 1 micro giây), thì dùng
 `std::mutex` thường lại CHẠY NHANH HƠN `std::shared_mutex`! Vì chi phí ngầm
 (Overhead) để quản lý hàng đợi Chia sẻ/Độc quyền của shared_mutex là khá lớn.
*/

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  demo_co_ban();
  demo_timeout();
  demo_ung_dung_thuc_te();

  std::cout << "\n--- HOAN THANH ---\n";
  std::cout << "Kien thuc chuyen gia ve 'Write Starvation' vui long doc phan "
               "comment so 4 trong code.\n";
  return 0;
}