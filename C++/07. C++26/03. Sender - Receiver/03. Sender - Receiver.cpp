#include <chrono>
#include <iostream>
#include <string>
#include <thread>

/*
  Trong C++26, các thư viện này sẽ là chuẩn.
  Nếu bạn dùng trình biên dịch cũ hơn, nó thường nằm ở <stdexec/execution.hpp>
*/
#include <execution>

// Tạo bí danh (alias) cho dễ gõ, chuẩn phong cách làm việc với C++26
namespace ex = std::execution;
namespace tt = std::this_thread;

// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. VẤN ĐỀ CỦA C++ CŨ:
    - Tạo `std::thread` tốn rất nhiều thời gian và RAM của Hệ điều hành.
    - `std::async` thì không thể ghép nối (compose) và thường block luồng chính.

 2. TRIẾT LÝ SENDER/RECEIVER (C++26):
    - Sender (Người gửi): Là bản mô tả của một công việc (Task). Nó mô tả công
 việc sẽ làm gì, nhưng CHƯA HỀ CHẠY (Lazy Evaluation) cho đến khi bạn ra lệnh.
    - Receiver (Người nhận): Nơi hứng kết quả khi Sender làm xong. Có 3 trạng
 thái: Thành công (set_value), Lỗi (set_error), hoặc Bị hủy (set_stopped).
    - Scheduler (Bộ lập lịch): Quyết định CÔNG VIỆC CHẠY Ở ĐÂU (Luồng hiện tại,
      Thread Pool, hay thậm chí là Card màn hình GPU).

 3. TOÁN TỬ QUYỀN NĂNG `|`:
    Giống như ống nước (Pipeline) trong Linux. Dữ liệu chảy từ Sender này qua
 Sender khác.
*/

// ==========================================
// 1. MỨC CƠ BẢN: TẠO SENDER VÀ ĐỢI KẾT QUẢ
// ==========================================
void execution_co_ban() {
  std::cout << "\n--- 1. BAZIC: KHOI TAO SENDER (LAZY) ---\n";

  // Tạo ra một Sender cực kỳ đơn giản: Chỉ gửi đi con số 42.
  // ĐIỂM QUAN TRỌNG: Lúc này code CHƯA CHẠY. Nó chỉ là một bản vẽ mô tả công
  // việc.
  auto sender_don_gian = ex::just(42);

  std::cout << "Sender da duoc tao, nhung chua he chay...\n";

  // Để Sender thực sự chạy, ta phải nối nó với một Receiver.
  // sync_wait chính là một Receiver bóp nghẹt luồng hiện tại để đợi kết quả.
  // Kết quả trả về luôn được bọc trong một std::optional và tuple.
  auto ket_qua = tt::sync_wait(sender_don_gian);

  if (ket_qua.has_value()) {
    // Lấy giá trị đầu tiên trong tuple ra
    std::cout << "Da chay xong! Nhan duoc: " << std::get<0>(ket_qua.value())
              << "\n";
  }
}

// ==========================================
// 2. MỨC TRUNG CẤP: PIPELINE VÀ TOÁN TỬ `|`
// ==========================================
void execution_pipeline() {
  std::cout << "\n--- 2. INTERMEDIATE: KET NOI TOAN TU '|' (PIPELINE) ---\n";

  // Tưởng tượng bạn đang ráp các ống nước lại với nhau.
  // ex::then đóng vai trò nhận dữ liệu từ ống trước, xử lý, rồi đẩy cho ống
  // sau.
  auto day_chuyen_san_xuat =
      ex::just(10, 5) | ex::then([](int a, int b) {
        std::cout << "  [Buoc 1] Nhan " << a << " va " << b
                  << ". Dang cong lai...\n";
        return a + b;
      }) |
      ex::then([](int tong) {
        std::cout << "  [Buoc 2] Nhan " << tong << ". Dang nhan doi...\n";
        return tong * 2;
      }) |
      ex::then([](int cuoi_cung) {
        return std::to_string(cuoi_cung) + " la ket qua cuoi cung!";
      });

  std::cout << "Day chuyen da lap rap xong (Chua chay).\n";
  std::cout << "Bam nut khoi dong (sync_wait)...\n";

  auto [chuoi_kq] = tt::sync_wait(day_chuyen_san_xuat).value();
  std::cout << chuoi_kq << "\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: SCHEDULER & ĐIỀU CHUYỂN LUỒNG (TRANSFER)
// ==========================================
/*
  C++26 cho phép bạn kiểm soát TỪNG BƯỚC của công việc sẽ chạy ở ĐÂU (CPU nào).
  Lưu ý: Để minh họa, ta giả lập một `thread_pool` chuẩn.
*/
struct MockThreadPool {
  // Giả lập một Scheduler (Bộ lập lịch đa luồng)
  auto get_scheduler() { return ex::inline_scheduler{}; }
  // Trong thực tế sẽ là: std::execution::static_thread_pool pool(4);
  // pool.get_scheduler();
};

void execution_chuyen_luong() {
  std::cout << "\n--- 3. ADVANCED: DIEU HUONG LUONG (SCHEDULING) ---\n";

  MockThreadPool pool_xu_ly_anh;
  auto gpu_scheduler = pool_xu_ly_anh.get_scheduler();

  auto main_thread_id = std::this_thread::get_id();
  std::cout << "Luong Main hien tai co ID: " << main_thread_id << "\n";

  // Kịch bản: Tải ảnh (ở luồng Main) -> Đẩy sang ThreadPool để Xử lý -> Bê lại
  // luồng Main để In ra màn hình
  auto tac_vu_xu_ly_anh =
      ex::just("image.png")
      // Bước 1: Vẫn đang ở luồng Main
      | ex::then([](std::string ten_file) {
          std::cout << "  [Main Thread] Dang tai " << ten_file << "...\n";
          return ten_file;
        })
      // BƯỚC 2: MA THUẬT! `transfer` sẽ bê toàn bộ các việc phía sau ném sang
      // Thread Pool!
      | ex::transfer(gpu_scheduler) | ex::then([](std::string ten_file) {
          std::cout << "  [Worker Thread " << std::this_thread::get_id()
                    << "] Dang ap dung Filter cho " << ten_file << "\n";
          std::this_thread::sleep_for(
              std::chrono::milliseconds(100)); // Giả lập tốn thời gian
          return "Anh_Da_Xu_Ly.png";
        });

  auto [ket_qua] = tt::sync_wait(tac_vu_xu_ly_anh).value();
  std::cout << "Da hoan thanh xu ly anh: " << ket_qua << "\n";
}

// ==========================================
// 4. MỨC CHUYÊN GIA: CHẠY SONG SONG (CONCURRENCY VỚI WHEN_ALL)
// ==========================================
void execution_chay_song_song() {
  std::cout << "\n--- 4. EXPERT: WHEN_ALL (CHAY SONG SONG THUC SU) ---\n";

  // Khai báo 3 công việc độc lập nhau
  auto task_tai_am_thanh = ex::just("audio.mp3") | ex::then([](std::string f) {
                             std::cout << "  Tai xong am thanh.\n";
                             return 100; // Trả về dung lượng (MB)
                           });

  auto task_tai_hinh_anh = ex::just("video.mp4") | ex::then([](std::string f) {
                             std::cout << "  Tai xong video.\n";
                             return 500;
                           });

  auto task_tai_phu_de = ex::just("sub.srt") | ex::then([](std::string f) {
                           std::cout << "  Tai xong phu de.\n";
                           return 2;
                         });

  // when_all: Ra lệnh GỘP 3 công việc này lại và CHẠY ĐỒNG THỜI.
  // Nó chỉ hoàn thành khi CẢ 3 đều hoàn thành.
  auto task_tong_hop =
      ex::when_all(task_tai_am_thanh, task_tai_hinh_anh, task_tai_phu_de)
      // Kết quả của when_all sẽ truyền 3 giá trị xuống hàm then tiếp theo
      | ex::then([](int size_audio, int size_video, int size_sub) {
          int tong = size_audio + size_video + size_sub;
          std::cout << "  => Da gop toan bo tai nguyen. Tong dung luong: "
                    << tong << " MB\n";
          return tong;
        });

  tt::sync_wait(task_tong_hop);
}

// ==========================================
// 5. CẢNH GIỚI TỐI CAO: XỬ LÝ LỖI (LET_ERROR) VÀ HỦY TÁC VỤ
// ==========================================
void execution_xu_ly_loi() {
  std::cout << "\n--- 5. MASTER: LET_ERROR (XU LY NGOAI LE BAT DONG BO) ---\n";

  // Trong môi trường đa luồng cũ, nếu 1 luồng quăng Exception (throw),
  // bắt nó (try-catch) là một ác mộng. Sender/Receiver làm điều đó cực kỳ thanh
  // lịch.

  auto task_rui_ro =
      ex::just(0) |
      ex::then([](int val) -> int {
        std::cout << "  Dang thuc hien phep chia cho " << val << "...\n";
        if (val == 0)
          throw std::runtime_error("Loi chia cho 0!");
        return 100 / val;
      })
      // Kênh xử lý lỗi (Kênh này chỉ kích hoạt nếu code bên trên quăng throw)
      | ex::let_error([](std::exception_ptr eptr) {
          try {
            if (eptr)
              std::rethrow_exception(eptr);
          } catch (const std::exception &e) {
            std::cout << "  [Phat hien loi bat dong bo]: " << e.what() << "\n";
          }
          // Trả về một Sender khác chứa giá trị mặc định để khôi phục hệ thống
          return ex::just(-1);
        });

  auto [ket_qua] = tt::sync_wait(task_rui_ro).value();
  std::cout << "Ket qua tra ve sau khi khoi phuc loi: " << ket_qua << "\n";
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== KHOA HOC C++26: STD::EXECUTION (SENDER/RECEIVER) =====\n";

  execution_co_ban();
  execution_pipeline();
  execution_chuyen_luong();
  execution_chay_song_song();
  execution_xu_ly_loi();

  std::cout << "\n===== HOAN THANH KIEN TRUC BAT DONG BO C++26 =====\n";
  return 0;
}