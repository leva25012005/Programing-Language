#include <algorithm> // sort, for_each, generate
#include <atomic>    // std::atomic (Chống xung đột dữ liệu)
#include <chrono>    // Đo thời gian
#include <execution> // BẮT BUỘC CHO SONG SONG: std::execution::par, seq, par_unseq
#include <iostream>
#include <mutex>   // Khóa luồng (Lock)
#include <numeric> // std::reduce, std::accumulate
#include <random>
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. C++17 Execution Policies (Chính sách Thực thi):
    - std::execution::seq       : Chạy Tuần tự (Sequential) - Giống hệt C++11,
 chạy trên 1 luồng duy nhất.
    - std::execution::par       : Chạy Song song (Parallel) - C++ tự động chia
 nhỏ mảng và đẩy cho nhiều nhân CPU chạy cùng lúc.
    - std::execution::par_unseq : Song song + Vô hướng (Parallel & Unsequenced)
 - Vừa chạy đa luồng, vừa cho phép CPU dùng lệnh SIMD (Xử lý nhiều mảng cùng lúc
 trên mức phần cứng). NHƯNG nghiêm cấm dùng Khóa (Mutex) ở chế độ này.
    - std::execution::unseq     : (C++20) Vô hướng nhưng chạy trên 1 luồng.

 2. Cạm bẫy chí mạng (Data Race & Deadlock):
    - Khi hàng nghìn luồng cùng đọc/ghi vào MỘT biến duy nhất, dữ liệu sẽ bị
 giẫm đạp lên nhau sinh ra kết quả sai.
    - Giải pháp: Dùng std::atomic, std::mutex, hoặc chuyển sang tư duy
 Map-Reduce (std::reduce).

 3. Không phải lúc nào cũng nhanh hơn!
    - Việc tạo ra Luồng (Thread) và quản lý chúng tốn một lượng CPU nhất định
 (Overhead).
    - Nếu mảng của bạn quá nhỏ (< 10,000 phần tử) hoặc phép toán quá đơn giản,
 chạy 'par' sẽ CHẬM HƠN 'seq'.
*/

// ==========================================
// HÀM TIỆN ÍCH ĐO THỜI GIAN
// ==========================================
class Timer {
  std::chrono::high_resolution_clock::time_point start;
  std::string name;

public:
  Timer(std::string n)
      : name(n), start(std::chrono::high_resolution_clock::now()) {}
  ~Timer() {
    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "[Thoi gian] " << name << ": " << duration.count() << " ms\n";
  }
};

// ==========================================
// 1. MỨC CƠ BẢN: KIỂM CHỨNG TỐC ĐỘ SORT ĐA LUỒNG
// ==========================================
void parallel_sort_co_ban() {
  std::cout << "\n--- 1. BAZIC: SEQ vs PAR SORT ---\n";

  const size_t SO_LUONG = 5'000'000; // 5 triệu phần tử
  std::vector<double> v_goc(SO_LUONG);

  // Sinh số ngẫu nhiên
  std::mt19937 gen(12345);
  std::uniform_real_distribution<double> dist(1.0, 1000.0);
  for (auto &x : v_goc)
    x = dist(gen);

  std::vector<double> v_seq = v_goc;
  std::vector<double> v_par = v_goc;

  {
    Timer t("1. std::sort Tuan tu (seq)");
    std::sort(std::execution::seq, v_seq.begin(), v_seq.end());
  }

  {
    // Phép màu của C++17 nằm ở tham số đầu tiên: std::execution::par
    // Trình biên dịch sẽ tự động gọi toàn bộ các nhân CPU (Ví dụ: CPU 8 Core 16
    // Thread) để sort mảng này.
    Timer t("2. std::sort Song song (par)");
    std::sort(std::execution::par, v_par.begin(), v_par.end());
  }
}

// ==========================================
// 2. MỨC TRUNG CẤP: DATA RACE VÀ CÁCH KHẮC PHỤC TRONG FOR_EACH
// ==========================================
void parallel_for_each_data_race() {
  std::cout << "\n--- 2. INTERMEDIATE: DATA RACE & ATOMIC ---\n";

  std::vector<int> v(1'000'000, 1); // 1 triệu số 1

  int tong_sai = 0;
  std::atomic<int> tong_dung = 0;
  std::mutex mtx;
  int tong_mutex = 0;

  // a. CẠM BẪY (Data Race): Hàng ngàn luồng cùng cộng vào biến 'tong_sai' mà
  // không xin phép. Kết quả chắc chắn sẽ sai và thấp hơn 1.000.000
  std::for_each(std::execution::par, v.begin(), v.end(), [&](int x) {
    tong_sai += x; // LỖI NGHIÊM TRỌNG!
  });
  std::cout << "Tong (Bi Data Race, rac): " << tong_sai << " (Sai)\n";

  // b. CÁCH KHẮC PHỤC 1: Dùng std::atomic (Biến nguyên tử)
  // CPU đảm bảo mỗi luồng cộng vào đều phải chờ luồng kia cộng xong (rất an
  // toàn, nhưng hoi chậm).
  std::for_each(std::execution::par, v.begin(), v.end(),
                [&](int x) { tong_dung += x; });
  std::cout << "Tong (Dung std::atomic)  : " << tong_dung << " (Chuan)\n";

  // c. CÁCH KHẮC PHỤC 2: Dùng Mutex
  // Nhược điểm: Việc Lock và Unlock liên tục ở 1 triệu phần tử tạo ra "Nút thắt
  // cổ chai", làm cho thuật toán song song chạy CÒN CHẬM HƠN CẢ TUẦN TỰ!
  {
    Timer t("   -> Toc do khi dung Mutex (Rat cham)");
    std::for_each(std::execution::par, v.begin(), v.end(), [&](int x) {
      std::lock_guard<std::mutex> lock(mtx);
      tong_mutex += x;
    });
  }
}

// ==========================================
// 3. MỨC NÂNG CAO: BỎ ACCUMULATE, CHUYỂN SANG REDUCE (MAP-REDUCE)
// ==========================================
// Vấn đề của std::accumulate là nó được thiết kế CHỈ ĐỂ CHẠY TỪ TRÁI SANG PHẢI
// (Tuần tự). C++17 ra mắt std::reduce: Hàm này cộng các phần tử theo dạng CÂY
// (chia để trị), không quan tâm thứ tự, do đó nó sinh ra để chạy Song song!

void parallel_reduce() {
  std::cout << "\n--- 3. ADVANCED: ACCUMULATE vs REDUCE ---\n";

  std::vector<double> v(10'000'000, 1.5);

  {
    Timer t("1. std::accumulate (Bat buoc chay Tuan tu)");
    // Dù bạn cố truyền std::execution::par vào accumulate, Compiler cũng sẽ báo
    // lỗi!
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
  }

  {
    Timer t("2. std::reduce (Chay Song song Da luong)");
    // Reduce chia mảng thành N mảnh, mỗi Core CPU tính tổng 1 mảnh, rồi cộng
    // dồn kết quả các mảnh lại.
    double sum = std::reduce(std::execution::par, v.begin(), v.end(), 0.0);
  }
}

// ==========================================
// 4. MỨC CHUYÊN GIA: PAR_UNSEQ & TRANSFORM_REDUCE
// ==========================================
void parallel_transform_reduce() {
  std::cout << "\n--- 4. EXPERT: TRANSFORM_REDUCE (Map-Reduce hoan hao) ---\n";

  // Bài toán: Tính tổng BÌNH PHƯƠNG của 10 triệu phần tử.
  // Thay vì làm 2 bước: Bước 1 for_each để bình phương (Transform), Bước 2
  // reduce để cộng tổng. Ta gộp chung lại thành 1 hàm duy nhất:
  // transform_reduce (Tránh phải duyệt RAM 2 lần).

  std::vector<double> v(
      10'000'000, 2.0); // 10 triệu số 2.0 (Tổng bình phương sẽ là 40,000,000)

  {
    Timer t("std::transform_reduce voi par_unseq");

    // Cấu trúc transform_reduce(Policy, Begin, End, GiaTriKhoiTao,
    // Ham_Reduce(Cong dồn), Ham_Transform(Binh phuong))
    double ket_qua = std::transform_reduce(
        std::execution::par_unseq, // Vừa đa luồng, vừa tận dụng CPU SIMD
                                   // Vectorization
        v.begin(), v.end(), 0.0,
        std::plus<>(), // Reduce: Cộng dồn 2 nhánh lại
        [](double x) {
          return x * x;
        } // Transform: Lấy bình phương từng phần tử
    );

    std::cout << "   -> Ket qua: " << ket_qua << "\n";
  }

  /*
  LUẬT THÉP KHI DÙNG par_unseq:
  Bên trong hàm Lambda (phần Transform), bạn TUYỆT ĐỐI KHÔNG ĐƯỢC:
  1. Cấp phát bộ nhớ động (new, delete, vector.push_back).
  2. Dùng Mutex (std::lock_guard) -> Sẽ gây Deadlock treo máy tính vĩnh viễn!
  Vì phần cứng CPU (SIMD lanes) sẽ xử lý đồng loạt một cục dữ liệu mà không có
  cơ chế dừng/đợi hệ điều hành.
  */
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== C++17 PARALLEL ALGORITHMS =====\n";

  parallel_sort_co_ban();
  parallel_for_each_data_race();
  parallel_reduce();
  parallel_transform_reduce();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}