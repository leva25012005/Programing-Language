#include <atomic> // Hỗ trợ biến nguyên tử (Lock-free)
#include <chrono>
#include <condition_variable> // Hỗ trợ giao tiếp/chờ đợi giữa các luồng
#include <future> // Hỗ trợ std::async, std::promise, std::future
#include <iostream>
#include <mutex> // Hỗ trợ khóa (lock) để bảo vệ dữ liệu chung
#include <queue>
#include <string>
#include <thread> // Hỗ trợ tạo và quản lý luồng
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 Trước C++11, để viết code đa luồng, bạn phải dùng POSIX Threads (Linux) hoặc
 Windows API (Windows). Từ C++11, C++ cung cấp bộ thư viện Đa luồng Chuẩn, chạy
 được trên MỌI hệ điều hành:

 1. <thread>: Sinh ra các "nhân công" (luồng) chạy song song.
 2. <mutex>: Cái ổ khóa. Đảm bảo tại 1 thời điểm chỉ có 1 nhân công được đụng
 vào dữ liệu.
 3. <atomic>: Biến "nguyên tử". Biến đặc biệt không cần ổ khóa (lock-free) mà
 vẫn an toàn.
 4. <condition_variable>: Cái chuông báo. Giúp một luồng "ngủ" và chờ luồng khác
 đánh thức.
 5. <future> & std::async: Giao việc cho luồng khác và lấy "Kết quả trả về"
 trong tương lai.
*/

// Tiện ích để giả lập thời gian chạy (giúp dễ quan sát luồng)
void lam_viec_nang(int mili_giay) {
  std::this_thread::sleep_for(std::chrono::milliseconds(mili_giay));
}

// ==========================================
// 1. MỨC CƠ BẢN: STD::THREAD VÀ TRUYỀN THAM SỐ
// ==========================================
void in_loi_chao(std::string ten, int so_lan) {
  for (int i = 0; i < so_lan; ++i) {
    std::cout << "  [" << ten << "] dang chay lan " << i + 1 << "\n";
    lam_viec_nang(50);
  }
}

// LƯU Ý: Truyền tham chiếu vào thread phải bọc trong std::ref()
void tang_gia_tri(int &x) { x += 100; }

void thread_co_ban() {
  std::cout << "\n--- 1. BAZIC: KHOI TAO THREAD ---\n";

  int ket_qua = 0;

  // Khởi tạo luồng (Nó sẽ bắt đầu chạy NGAY LẬP TỨC)
  std::thread t1(in_loi_chao, "Luong A", 3);
  std::thread t2(in_loi_chao, "Luong B", 3);
  std::thread t3(
      tang_gia_tri,
      std::ref(ket_qua)); // Bắt buộc dùng std::ref để truyền tham chiếu

  // QUY TẮC SỐNG CÒN: Phải gọi join() hoặc detach() trước khi biến thread bị
  // hủy. join() bắt Luồng Chính (Main thread) phải ĐỨNG ĐỢI t1, t2, t3 làm xong
  // mới được chạy tiếp.
  t1.join();
  t2.join();
  t3.join();

  std::cout << "Ket qua bien tham chieu: " << ket_qua << "\n";
}

// ==========================================
// 2. MỨC TRUNG CẤP: MUTEX VÀ DATA RACE
// ==========================================
int tai_khoan_ngan_hang = 0;
std::mutex mtx; // Ổ khóa bảo vệ tài khoản

void nap_tien_khong_an_toan() {
  for (int i = 0; i < 100000; ++i) {
    tai_khoan_ngan_hang++; // DATA RACE! Nhiều luồng cùng ghi đè lên biến này
                           // gây sai số.
  }
}

void nap_tien_an_toan() {
  for (int i = 0; i < 100000; ++i) {
    // std::lock_guard sẽ tự động KHOÁ (lock) khi khởi tạo,
    // và tự động MỞ KHOÁ (unlock) khi vòng lặp kết thúc (Chuẩn RAII).
    std::lock_guard<std::mutex> khoa(mtx);
    tai_khoan_ngan_hang++;
  }
}

void mutex_bao_ve_du_lieu() {
  std::cout << "\n--- 2. INTERMEDIATE: MUTEX & LOCK_GUARD ---\n";

  tai_khoan_ngan_hang = 0;
  std::thread t1(nap_tien_an_toan);
  std::thread t2(nap_tien_an_toan);

  t1.join();
  t2.join();
  std::cout << "Tai khoan sau khi 2 luong nap an toan: " << tai_khoan_ngan_hang
            << " (Chuan 200000)\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: STD::ATOMIC (LOCK-FREE)
// ==========================================
// Mutex rất an toàn nhưng LÀM CHẬM CHƯƠNG TRÌNH (vì các luồng phải xếp hàng đợi
// nhau). std::atomic giải quyết vấn đề này ở cấp độ Phần cứng (Hardware/CPU).
// Nó đảm bảo phép toán ++ được thực hiện trọn vẹn trong 1 nhịp đồng hồ, không
// luồng nào chen ngang được.

std::atomic<int> luot_xem{0};

void tang_luot_xem() {
  for (int i = 0; i < 100000; ++i) {
    luot_xem++; // Hoàn toàn Thread-safe, cực kỳ nhanh, không cần Mutex!
  }
}

void atomic_lock_free() {
  std::cout << "\n--- 3. ADVANCED: STD::ATOMIC (SIEU NHANH) ---\n";

  std::thread t1(tang_luot_xem);
  std::thread t2(tang_luot_xem);

  t1.join();
  t2.join();
  std::cout << "Tong luot xem (Atomic): " << luot_xem
            << " (Chuan 200000 ma khong can Khoa)\n";
}

// ==========================================
// 4. MỨC CHUYÊN GIA 1: GIAO TIẾP VỚI CONDITION VARIABLE
// ==========================================
// Mô hình Nhà sản xuất - Người tiêu dùng (Producer - Consumer)
std::queue<int> bang_chuyen;
std::mutex mtx_bc;
std::condition_variable chuong_bao;
bool da_san_xuat_xong = false;

void NguoiTieuDung() {
  std::cout << "  [Tieu Dung] Dang cho hang...\n";

  // Bắt buộc dùng std::unique_lock đi kèm với condition_variable (không dùng
  // lock_guard được)
  std::unique_lock<std::mutex> khoa(mtx_bc);

  // Lệnh wait: "Hãy nhả ổ khóa ra và đi NGỦ. Chỉ thức dậy khi nghe tiếng chuông
  // báo (notify), VÀ điều kiện trong Lambda function trả về true".
  chuong_bao.wait(khoa,
                  [] { return !bang_chuyen.empty() || da_san_xuat_xong; });

  while (!bang_chuyen.empty()) {
    std::cout << "  [Tieu Dung] Da tieu thu san pham so: "
              << bang_chuyen.front() << "\n";
    bang_chuyen.pop();
  }
}

void NhaSanXuat() {
  lam_viec_nang(200); // Giả lập đang sản xuất
  {
    std::lock_guard<std::mutex> khoa(mtx_bc);
    bang_chuyen.push(101);
    bang_chuyen.push(102);
    da_san_xuat_xong = true;
    std::cout << "  [San Xuat] Da lam xong 2 san pham! Bam chuong!\n";
  }
  // Đánh thức 1 luồng đang ngủ (hoặc notify_all() để đánh thức tất cả)
  chuong_bao.notify_one();
}

void condition_variable_demo() {
  std::cout << "\n--- 4. EXPERT 1: CONDITION VARIABLE ---\n";
  std::thread tieu_dung(NguoiTieuDung);
  std::thread san_xuat(NhaSanXuat);

  tieu_dung.join();
  san_xuat.join();
}

// ==========================================
// 5. MỨC CHUYÊN GIA 2: STD::ASYNC & STD::FUTURE
// ==========================================
// std::thread có nhược điểm: Nó không thể trực tiếp dùng chữ `return` để trả về
// kết quả. std::async sinh ra để giải quyết việc này.

int tinh_toan_phuc_tap(int a, int b) {
  std::cout << "  [Async] Bat dau tinh toan tren luong phu...\n";
  lam_viec_nang(500); // Giả lập tính toán mất 0.5s
  return a * b;
}

void future_va_async() {
  std::cout << "\n--- 5. EXPERT 2: FUTURE & ASYNC ---\n";

  // Giao việc cho luồng khác bằng std::async
  // std::launch::async BẮT BUỘC C++ phải tạo luồng mới chạy song song ngay lập
  // tức. Kết quả tương lai sẽ được bỏ vào chiếc hộp std::future.
  std::future<int> hop_ket_qua =
      std::async(std::launch::async, tinh_toan_phuc_tap, 10, 20);

  std::cout
      << "  [Main] Luong chinh van dang lam viec khac, khong bi block...\n";
  lam_viec_nang(100);

  std::cout
      << "  [Main] Can ket qua roi, tien hanh lay ket qua tu hop future...\n";

  // Lệnh .get() sẽ CHẶN (Block) luồng chính lại, đứng đợi cho đến khi luồng phụ
  // tính xong. Nếu tính xong rồi thì lấy ra lập tức. Lệnh .get() CHỈ ĐƯỢC GỌI 1
  // LẦN DUY NHẤT.
  int ket_qua = hop_ket_qua.get();

  std::cout << "Ket qua tinh toan la: " << ket_qua << "\n";
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== KHOA HOC C++ MULTITHREADING =====\n";

  thread_co_ban();
  mutex_bao_ve_du_lieu();
  atomic_lock_free();
  condition_variable_demo();
  future_va_async();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}