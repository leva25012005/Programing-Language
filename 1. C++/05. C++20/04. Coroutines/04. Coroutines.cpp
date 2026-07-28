#include <chrono>
#include <coroutine> // Bắt buộc phải có cho C++20 Coroutines
#include <exception>
#include <iostream>
#include <thread>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Hàm bình thường (Subroutine):
    Gọi hàm -> Chạy một mạch từ đầu đến cuối -> Gọi return -> Kết thúc.
    Nếu hàm đang tải 1 file 10GB, toàn bộ chương trình (hoặc luồng đó) sẽ BỊ
 ĐÓNG BĂNG (Block).

 2. Hàm Coroutine:
    Gọi hàm -> Chạy được một nửa -> TẠM DỪNG (Suspend), trả lại quyền điều khiển
 cho CPU làm việc khác
    -> Khi nào rảnh hoặc có lệnh, quay lại TIẾP TỤC (Resume) ngay tại dòng code
 vừa dừng.

 3. Ba thanh gươm quyền lực:
    - co_yield: Trả về một giá trị, sau đó TẠM DỪNG hàm. (Dùng để tạo
 Generator).
    - co_await: Tạm dừng hàm để chờ một tác vụ bất đồng bộ nào đó hoàn thành.
 (Dùng trong I/O, Network).
    - co_return: Kết thúc hẳn Coroutine và trả về kết quả cuối cùng.
*/

// ==========================================
// PHẦN 1: co_yield VÀ GENERATOR (Máy phát điện)
// ==========================================

// Đây là "Lớp vỏ" mà Coroutine sẽ trả về cho hàm main.
struct MayPhatSo {
  // 1. ĐỘNG CƠ CỐT LÕI: C++ bắt buộc phải có struct tên chính xác là
  // 'promise_type'
  struct promise_type {
    int gia_tri_hien_tai; // Nơi lưu trữ giá trị do co_yield nhả ra

    // Khởi tạo Lớp vỏ trả về cho hàm gọi
    MayPhatSo get_return_object() {
      return MayPhatSo{
          std::coroutine_handle<promise_type>::from_promise(*this)};
    }

    // Khi vừa gọi hàm, có tạm dừng ngay lập tức không? (suspend_always: Có)
    std::suspend_always initial_suspend() { return {}; }

    // Khi hàm kết thúc, có dừng lại để dọn dẹp không?
    std::suspend_always final_suspend() noexcept { return {}; }

    // BẮT KẾT QUẢ TỪ TỪ KHÓA 'co_yield'
    std::suspend_always yield_value(int value) {
      gia_tri_hien_tai = value;
      return {}; // Tạm dừng sau khi nhả giá trị
    }

    // Nếu coroutine dùng return rỗng
    void return_void() {}

    // Xử lý nếu có Exception văng ra trong Coroutine
    void unhandled_exception() { std::terminate(); }
  };

  // Tay cầm (Handle) để điều khiển Coroutine (Bấm nút Pause/Play)
  std::coroutine_handle<promise_type> dieu_khien;

  // Constructor & Destructor
  explicit MayPhatSo(std::coroutine_handle<promise_type> h) : dieu_khien(h) {}
  ~MayPhatSo() {
    if (dieu_khien)
      dieu_khien.destroy();
  } // Rất quan trọng: Phải hủy để chống Leak RAM

  // Hàm tiện ích để hàm main xài
  bool tiep_tuc() {
    dieu_khien.resume();       // BẤM NÚT PLAY!
    return !dieu_khien.done(); // Kiểm tra xem coroutine đã chạy hết chưa
  }
  int lay_gia_tri() { return dieu_khien.promise().gia_tri_hien_tai; }
};

// ==========================================
// PHẦN 2: HÀM COROUTINE SỬ DỤNG co_yield
// ==========================================
// Vì hàm này có chứa từ khóa co_yield, Trình biên dịch sẽ tự động biến nó thành
// Coroutine!
MayPhatSo tao_day_so_fibonacci(int so_luong) {
  std::cout << "  [Coroutine] Bat dau tinh Fibonacci...\n";
  int a = 0, b = 1;

  for (int i = 0; i < so_luong; ++i) {
    // TẠM DỪNG TẠI ĐÂY! Nhả biến 'a' ra ngoài, trả CPU cho hàm main.
    // Chờ đến khi nào hàm main gọi .tiep_tuc() thì mới chạy vòng lặp tiếp theo.
    co_yield a;

    int temp = a;
    a = b;
    b = temp + b;
  }
  std::cout << "  [Coroutine] Da hoan thanh!\n";
}

// ==========================================
// PHẦN 3: co_await, co_return VÀ TASK BẤT ĐỒNG BỘ
// ==========================================

// Struct mô phỏng một sự kiện mất thời gian (Ví dụ: Tải file từ Mạng)
struct TacVuChoDoi {
  // 1. Có cần chờ không? (false = có, phải dừng lại chờ)
  bool await_ready() const noexcept { return false; }

  // 2. Chạy hàm này ngay khoảnh khắc Coroutine bị tạm dừng
  void await_suspend(std::coroutine_handle<>) const noexcept {
    std::cout << "  [Network] Dang tai du lieu (Mat 2 giay)...\n";
    // Giả lập hệ thống mạng đang tải (Trong thực tế sẽ dùng callback/epoll để
    // không block thread)
    std::this_thread::sleep_for(std::chrono::seconds(2));
  }

  // 3. Chạy hàm này khi Coroutine được đánh thức (Resume)
  int await_resume() const noexcept {
    std::cout << "  [Network] Tai xong!\n";
    return 999; // Dữ liệu tải về
  }
};

// Khung sườn (Boilerplate) cho hàm dùng co_await / co_return
struct NhiemVuAsync {
  struct promise_type {
    int ket_qua_cuoi_cung;
    NhiemVuAsync get_return_object() {
      return NhiemVuAsync{
          std::coroutine_handle<promise_type>::from_promise(*this)};
    }
    std::suspend_always initial_suspend() { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void unhandled_exception() { std::terminate(); }

    // BẮT KẾT QUẢ TỪ TỪ KHÓA 'co_return'
    void return_value(int value) { ket_qua_cuoi_cung = value; }
  };

  std::coroutine_handle<promise_type> dieu_khien;
  explicit NhiemVuAsync(std::coroutine_handle<promise_type> h)
      : dieu_khien(h) {}
  ~NhiemVuAsync() {
    if (dieu_khien)
      dieu_khien.destroy();
  }

  void chay() { dieu_khien.resume(); }
  int lay_ket_qua() { return dieu_khien.promise().ket_qua_cuoi_cung; }
};

// ==========================================
// PHẦN 4: HÀM COROUTINE SỬ DỤNG co_await & co_return
// ==========================================
NhiemVuAsync tai_file_tu_server() {
  std::cout << "  [Coroutine] Chuan bi goi Network...\n";

  // TẠM DỪNG COROUTINE TẠI ĐÂY! Nhường CPU cho việc khác.
  // Dữ liệu sẽ được nhận vào biến 'data' sau khi quá trình chờ hoàn tất.
  int data = co_await TacVuChoDoi{};

  std::cout << "  [Coroutine] Da nhan duoc data, bat dau xu ly...\n";

  // KẾT THÚC COROUTINE (Không dùng return bình thường được nữa)
  co_return data * 2;
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== 1. DEMO CO_YIELD (GENERATOR) =====\n";
  // Goi hàm KHÔNG LÀM NÓ CHẠY NGAY. Nó bị tạm dừng (suspend_always) ở ngay cửa
  // hàm.
  MayPhatSo may_phat = tao_day_so_fibonacci(5);

  std::cout << "[Main] Da khoi tao may phat. CPU van thuoc ve Main.\n";

  // Hàm main chủ động điều khiển Coroutine bằng tay (Bấm nút Play)
  while (may_phat.tiep_tuc()) {
    std::cout << "[Main] Nhan duoc so: " << may_phat.lay_gia_tri() << "\n";
    std::cout << "[Main] Co the lam viec khac o day roi moi bam Play tiep...\n";
  }

  std::cout << "\n===== 2. DEMO CO_AWAIT & CO_RETURN (ASYNC TASK) =====\n";
  NhiemVuAsync nhiem_vu = tai_file_tu_server();

  std::cout << "[Main] Da khoi tao nhiem vu. Bam PLAY de bat dau.\n";
  nhiem_vu.chay(); // Bấm play, chạy đến chỗ co_await thì nó sẽ thực thi
                   // await_suspend

  std::cout << "[Main] Ket qua cuoi cung tu co_return: "
            << nhiem_vu.lay_ket_qua() << "\n";

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}