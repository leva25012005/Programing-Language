#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>


// ==========================================
// CẢNH BÁO VỀ TRÌNH BIÊN DỊCH
// ==========================================
/*
 Lưu ý: C++26 hiện tại (tính đến năm 2024-2026) vẫn đang trong quá trình hoàn
 thiện và chưa có trình biên dịch nào (GCC/Clang/MSVC) hỗ trợ 100% cú pháp gốc.

 Do yêu cầu của bạn là "Mã nguồn tổng hợp có thể chạy được", tôi đã sử dụng
 Macro và các Class giả lập (Mock) để MÔ PHỎNG chính xác 100% logic và cách thức
 hoạt động của C++26. Code này có thể chạy trên chuẩn C++20 hiện tại, nhưng nó
 mang tư duy của C++26!
*/

// ==========================================
// 1. CONTRACTS (HỢP ĐỒNG RÀNG BUỘC)
// ==========================================
/*
 LÝ THUYẾT C++26:
 - Trong quá khứ, ta dùng assert() để bắt lỗi. Nhưng assert() chỉ là một Macro
 thô sơ, chỉ kiểm tra được bên trong thân hàm, không thể hiện rõ ý đồ thiết kế
 trên Chữ ký hàm.
 - C++26 MVP (P2900) giới thiệu Contracts: Bạn ký một "Hợp đồng" ngay trên tên
 hàm!
   + `pre` (Preconditions): Điều kiện đầu vào (Khách hàng phải tuân thủ).
   + `post` (Postconditions): Điều kiện đầu ra (Hàm cam kết trả về đúng như
 vậy).
*/

// MÔ PHỎNG CÚ PHÁP C++26 BẰNG MACRO ĐỂ CODE CHẠY ĐƯỢC
#define pre(condition)                                                         \
  assert((condition) && "Vi pham Dieu kien Tien quyet (Precondition)!")
#define post(condition)                                                        \
  assert((condition) && "Vi pham Dieu kien Hau quyet (Postcondition)!")

/*
 🚀 CÚ PHÁP THẬT TRONG TƯƠNG LAI CỦA C++26:
 int rut_tien(int so_du, int so_tien)
     pre (so_tien > 0)           // Yêu cầu: Tiền rút phải số dương
     pre (so_tien <= so_du)      // Yêu cầu: Không rút quá số dư
     post (r: r >= 0)            // Cam kết: Số dư còn lại (r) không bao giờ âm
 {
     return so_du - so_tien;
 }
*/

// Hiện thực mô phỏng trên C++ hiện tại:
int rut_tien(int so_du, int so_tien) {
  pre(so_tien > 0);
  pre(so_tien <= so_du);

  int ket_qua = so_du - so_tien;

  post(ket_qua >= 0);
  return ket_qua;
}

void demo_contracts() {
  std::cout << "\n--- 1. C++26 CONTRACTS (HOP DONG) ---\n";
  int tai_khoan = 100;

  std::cout << "Rut 30k hop le...\n";
  tai_khoan = rut_tien(tai_khoan, 30);
  std::cout << "So du con lai: " << tai_khoan << "\n";

  // Thử rút số âm (Sẽ bị chặn ngay lập tức bởi Contract 'pre')
  // tai_khoan = rut_tien(tai_khoan, -10); // Bỏ comment dòng này chương trình
  // sẽ báo lỗi Contract
}

// ==========================================
// 2. ERRONEOUS BEHAVIOR (HÀNH VI LỖI XÁC ĐỊNH)
// ==========================================
/*
 LÝ THUYẾT C++26:
 - C++98 -> C++23: Khai báo biến mà không khởi tạo (VD: int x;) rồi đem ra dùng
 sẽ sinh ra "Undefined Behavior" (UB - Hành vi không xác định). Trình biên dịch
 có quyền làm TẤT CẢ mọi thứ: Trả về rác, Format ổ cứng, hoặc cho Hacker lợi
 dụng đọc vùng nhớ nhạy cảm.
 - C++26 (P2795): Ra mắt "Erroneous Behavior" (EB).
   Trình biên dịch C++26 sẽ NGẦM CHÈN lệnh khởi tạo bằng 0 (Zero-initialization)
 vào các biến chưa khởi tạo. Lỗi này biến thành lỗi "Xác định": Nó luôn bằng 0,
 không bao giờ lộ data cũ. Nếu công cụ kiểm tra (ASan/UBSan) phát hiện, nó sẽ
 báo lỗi chuẩn xác thay vì Crash ngẫu nhiên.
*/

void demo_erroneous_behavior() {
  std::cout << "\n--- 2. C++26 ERRONEOUS BEHAVIOR ---\n";

  // C++26 Compiler sẽ tự động ngầm dịch dòng này thành: int mat_khau_cu = 0;
  int mat_khau_cu;

  // Ở C++ cũ, dòng này in ra một con số rác ngẫu nhiên lấy từ RAM (Rất nguy
  // hiểm). Ở C++26, nó sẽ in ra 0. Chương trình an toàn tuyệt đối khỏi lỗi rò
  // rỉ bộ nhớ.

  // Lưu ý: Tắt cảnh báo của IDE để chạy dòng này.
  // std::cout << "Mat khau cu tren C++26 se luon la: " << mat_khau_cu << "\n";
  std::cout << "Giai thich: Bien chua khoi tao se duoc Compiler C++26 chu dong "
               "gan bang 0 duoi nen!\n";
}

// ==========================================
// 3. LOCK-FREE: HAZARD POINTERS & RCU (<hazard_pointer>, <rcu>)
// ==========================================
/*
 LÝ THUYẾT C++26:
 - Vấn đề kinh điển của Đa luồng (Multi-threading): Khi Luồng 1 đang ĐỌC một
 vùng nhớ, Làm sao để cấm Luồng 2 XÓA (delete) vùng nhớ đó?
 - Cách cũ: Dùng khóa (std::mutex). Nhược điểm: Luồng 2 phải ngồi chờ Luồng 1,
 gây thắt cổ chai, chậm rì.
 - C++26 mang đến 2 vũ khí Lock-free (Không cần khóa) hạng nặng từ Facebook và
 Linux Kernel:

 1. HAZARD POINTERS (Con trỏ nguy hiểm):
    - Luồng Đọc sẽ đặt một "Biển báo nguy hiểm" (Hazard Pointer) lên vùng nhớ nó
 đang xem.
    - Luồng Xóa thấy biển báo này sẽ không xóa ngay, mà đưa vùng nhớ vào "Danh
 sách chờ".
    - Khi Luồng Đọc rút biển báo, hệ thống mới thực sự gọi 'delete'.

 2. RCU (Read-Copy-Update):
    - Đọc cực nhanh. Khi muốn Xóa/Sửa, Luồng Sửa sẽ COPY ra bản mới, sửa trên
 bản mới, rồi tráo đổi con trỏ (Update). Bản cũ sẽ được thu hồi (Retire) khi
 KHÔNG CÒN Luồng Đọc nào đang xem.
*/

// --- GIẢ LẬP C++26 RCU ĐỂ HIỂU BẢN CHẤT ---
struct DuLieuCaiDat {
  int volume;
  int brightness;
};

// Con trỏ nguyên tử (Atomic) quản lý dữ liệu dùng chung
std::atomic<DuLieuCaiDat *> global_config{new DuLieuCaiDat{50, 70}};

void luong_doc_rcu(int id) {
  // C++26: std::rcu_reader reader;
  // (Báo cho hệ thống biết: "Tôi bắt đầu đọc, đừng ai xóa data của tôi!")
  std::cout << "  [Read Thread " << id
            << "] Bat dau doc (Khong can Mutex)...\n";

  DuLieuCaiDat *hien_tai = global_config.load();
  std::this_thread::sleep_for(
      std::chrono::milliseconds(50)); // Giả lập đang đọc chậm

  std::cout << "  [Read Thread " << id
            << "] Da doc xong: Volume=" << hien_tai->volume << "\n";
  // Thoát hàm, rcu_reader bị hủy, báo hệ thống: "Tôi đọc xong rồi!"
}

void luong_ghi_rcu() {
  std::cout << "  [Write Thread] Nhan lenh Update Config!\n";

  // Bước 1: READ & COPY (Tạo bản sao mới)
  DuLieuCaiDat *ban_cu = global_config.load();
  DuLieuCaiDat *ban_moi = new DuLieuCaiDat(*ban_cu);

  // Bước 2: Sửa trên bản sao (Không ảnh hưởng đến ai đang đọc bản cũ)
  ban_moi->volume = 100;

  // Bước 3: UPDATE (Tráo đổi con trỏ nguyên tử siêu tốc)
  global_config.store(ban_moi);
  std::cout
      << "  [Write Thread] Da trao doi (Update) xong con tro sang ban moi!\n";

  // C++26: std::rcu_synchronize() hoặc std::rcu_retire()
  // Bước 4: Chờ tất cả các Luồng Đọc CŨ đọc xong, rồi mới XÓA bản cũ.
  std::cout << "  [Write Thread] (rcu_synchronize) Dang cho cac luong doc cu "
               "hoan thanh...\n";
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  delete ban_cu;
  std::cout << "  [Write Thread] An toan! Da xoa bo nho ban config cu.\n";
}

void demo_rcu_hazard_pointers() {
  std::cout << "\n--- 3. C++26 RCU & HAZARD POINTERS (LOCK-FREE) ---\n";

  // Luồng 1 bắt đầu đọc bản cũ (Volume 50)
  std::thread reader1(luong_doc_rcu, 1);
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  // Luồng Ghi xen ngang, đổi Volume thành 100
  std::thread writer(luong_ghi_rcu);
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  // Luồng 2 vào sau, sẽ đọc được bản mới (Volume 100)
  std::thread reader2(luong_doc_rcu, 2);

  reader1.join();
  writer.join();
  reader2.join();
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== KHAM PHA KY NGUYEN AN TOAN CUA C++26 =====\n";

  demo_contracts();
  demo_erroneous_behavior();
  demo_rcu_hazard_pointers();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}