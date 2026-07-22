#include <functional> // Cho std::greater (Min-Heap)
#include <iostream>
#include <queue> // Chứa cả std::queue và std::priority_queue
#include <string>
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Container Adapters là gì?
    - Chúng không tự quản lý RAM. Chúng bọc một container khác (như vector,
 deque) ở bên trong.
    - ĐẶC ĐIỂM CHÍ MẠNG: CHÚNG KHÔNG CÓ ITERATOR! Bạn không thể dùng vòng lặp
 for (auto x : q) để duyệt qua hàng đợi. Cách duy nhất để lấy dữ liệu là "Rút ra
 và Hủy đi" (Pop).

 2. std::queue (Hàng đợi FIFO - First In, First Out)
    - Nguyên tắc xếp hàng: Ai vào trước, phục vụ trước.
    - Bên dưới nền: Đeo mặt nạ lên std::deque.

 3. std::priority_queue (Hàng đợi Ưu tiên)
    - Nguyên tắc VIP: Ai có độ ưu tiên cao nhất sẽ ĐƯỢC TRỒI LÊN ĐẦU HÀNG, bất
 kể đến lúc nào.
    - Bên dưới nền: Đeo mặt nạ lên std::vector, sử dụng cấu trúc dữ liệu HEAP
 (Cây nhị phân).
    - Mặc định: Là MAX-HEAP (Phần tử LỚN NHẤT nằm ở đỉnh).
*/

// ==========================================
// CLASS & STRUCT PHỤ TRỢ CHO PHẦN CHUYÊN GIA
// ==========================================
struct BenhNhan {
  std::string ten;
  int muc_do_khan_cap; // Càng cao càng nguy hiểm

  BenhNhan(std::string t, int khan_cap) : ten(t), muc_do_khan_cap(khan_cap) {}

  // Cách 1: Nạp chồng toán tử < ngay bên trong Struct
  // priority_queue luôn tìm kiếm phần tử "Lớn nhất" dựa trên toán tử <
  bool operator<(const BenhNhan &other) const {
    // Trả về true nếu 'this' nhỏ hơn 'other' (Nghĩa là 'other' sẽ được ưu tiên
    // nổi lên trên)
    return this->muc_do_khan_cap < other.muc_do_khan_cap;
  }
};

// Cách 2: Viết Functor So sánh tùy chỉnh (Dùng khi không thể sửa code của
// Struct)
struct SoSanhDoDaiTen {
  bool operator()(const std::string &a, const std::string &b) const {
    // Tên dài hơn sẽ nổi lên đầu hàng
    return a.length() < b.length();
  }
};

// ==========================================
// 1. MỨC CƠ BẢN: STD::QUEUE (XẾP HÀNG MUA VÉ)
// ==========================================
void queue_co_ban() {
  std::cout << "\n--- 1. BAZIC: STD::QUEUE (FIFO) ---\n";

  std::queue<std::string> hang_doi;

  // Vào hàng (push / emplace) - O(1)
  hang_doi.push("Nguoi thu 1");
  hang_doi.push("Nguoi thu 2");
  hang_doi.emplace("Nguoi thu 3"); // Dùng emplace để tối ưu (tạo trực tiếp)

  std::cout << "So nguoi dang xep hang: " << hang_doi.size() << "\n";
  std::cout << "Nguoi dung dau: " << hang_doi.front() << "\n";
  std::cout << "Nguoi dung cuoi: " << hang_doi.back() << "\n";

  // CÁCH DUY NHẤT ĐỂ DUYỆT QUEUE: Lấy ra -> Xử lý -> Xóa đi
  std::cout << "Bat dau phuc vu:\n";
  while (!hang_doi.empty()) {
    std::cout << "  -> Dang phuc vu: " << hang_doi.front() << "\n";
    hang_doi.pop(); // Xóa người đứng đầu để người thứ 2 lên thay
  }
}

// ==========================================
// 2. MỨC TRUNG CẤP: PRIORITY_QUEUE (MAX-HEAP)
// ==========================================
void priority_queue_max_heap() {
  std::cout << "\n--- 2. INTERMEDIATE: MAX-HEAP PRIORITY QUEUE ---\n";

  // Mặc định là Max-Heap (Số lớn nhất luôn trồi lên đỉnh)
  std::priority_queue<int> pq;

  pq.push(10);
  pq.push(50); // Số 50 sẽ tự động nổi lên vị trí đầu tiên (O(log n))
  pq.push(20);
  pq.push(5);

  // LƯU Ý: priority_queue KHÔNG CÓ hàm .front() và .back().
  // Nó chỉ có duy nhất hàm .top() (Lấy phần tử trên đỉnh Heap)

  std::cout << "Lay du lieu ra tu Max-Heap: ";
  while (!pq.empty()) {
    std::cout << pq.top() << " "; // Kết quả chắc chắn là: 50 20 10 5
    pq.pop();
  }
  std::cout << "\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: PRIORITY_QUEUE (MIN-HEAP)
// ==========================================
void priority_queue_min_heap() {
  std::cout << "\n--- 3. ADVANCED: MIN-HEAP (Nho nhat len dau) ---\n";

  // ĐỂ TẠO MIN-HEAP (Số nhỏ nhất trồi lên): Bạn phải truyền đủ 3 tham số
  // Template!
  // 1. Kiểu dữ liệu (int)
  // 2. Container nền tảng (std::vector<int>)
  // 3. Functor so sánh (std::greater<int>)
  std::priority_queue<int, std::vector<int>, std::greater<int>> min_heap;

  min_heap.push(10);
  min_heap.push(50);
  min_heap.push(20);
  min_heap.push(5); // Số 5 nhỏ nhất sẽ lên đỉnh

  std::cout << "Lay du lieu ra tu Min-Heap: ";
  while (!min_heap.empty()) {
    std::cout << min_heap.top() << " "; // Kết quả: 5 10 20 50
    min_heap.pop();
  }
  std::cout << "\n";
}

// ==========================================
// 4. MỨC CHUYÊN GIA: ƯU TIÊN VỚI CLASS TÙY CHỈNH
// ==========================================
void priority_queue_custom_class() {
  std::cout << "\n--- 4. EXPERT: CUSTOM CLASS & FUNCTOR ---\n";

  // a. Sử dụng toán tử < được nạp chồng bên trong Struct BenhNhan
  std::priority_queue<BenhNhan> phong_cap_cuu;

  phong_cap_cuu.emplace("Benh nhan A (Dau dau)", 1);
  phong_cap_cuu.emplace("Benh nhan B (Gay tay)", 5);
  phong_cap_cuu.emplace("Benh nhan C (Dung tim)",
                        10); // Khẩn cấp nhất! Sẽ trồi lên đầu.
  phong_cap_cuu.emplace("Benh nhan D (Sot)", 3);

  std::cout << "> Danh sach cap cuu (Uu tien muc do cao):\n";
  while (!phong_cap_cuu.empty()) {
    BenhNhan vip = phong_cap_cuu.top();
    std::cout << "  - Chon: " << vip.ten << " (Muc do: " << vip.muc_do_khan_cap
              << ")\n";
    phong_cap_cuu.pop();
  }

  // b. Sử dụng Functor So sánh tùy chỉnh từ bên ngoài (SoSanhDoDaiTen)
  // Tham số Template: <Kiểu DL, Container nền, Functor so sánh>
  std::priority_queue<std::string, std::vector<std::string>, SoSanhDoDaiTen>
      chuoi_dai_nhat;

  chuoi_dai_nhat.push("Meo");
  chuoi_dai_nhat.push("Khung Long");
  chuoi_dai_nhat.push("Cho");

  std::cout << "\n> Lay ra Chuoi dai nhat truoc:\n";
  while (!chuoi_dai_nhat.empty()) {
    std::cout << "  - " << chuoi_dai_nhat.top()
              << "\n"; // Khung Long -> Meo -> Cho
    chuoi_dai_nhat.pop();
  }
}

// ==========================================
// TỔNG KẾT KINH NGHIỆM THỰC CHIẾN
// ==========================================
/*
 1. BAO GIỜ THÌ DÙNG std::queue?
    - Dành cho các bài toán BFS (Duyệt theo chiều rộng trong Đồ thị).
    - Các hệ thống hàng đợi công việc, luân chuyển tin nhắn (Message Queue).

 2. BAO GIỜ THÌ DÙNG std::priority_queue?
    - Dành cho thuật toán tìm đường (A*, Dijkstra) cực kỳ quan trọng trong lập
 trình Game/AI.
    - Dành cho bài toán "Tìm K phần tử lớn nhất/nhỏ nhất trong mảng 1 triệu phần
 tử". (Dùng Min-Heap kích thước K luôn chạy siêu nhanh O(N log K) thay vì Sort
 toàn bộ mảng O(N log N)).

 3. CẠM BẪY ĐAU ĐỚN:
    - Nhớ kỹ tên hàm lấy giá trị: Queue dùng .front(), Priority_Queue dùng
 .top().
    - Nhiều người quen tay viết pq.front() và gãi đầu không hiểu tại sao
 Compiler báo lỗi.
*/

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  queue_co_ban();
  priority_queue_max_heap();
  priority_queue_min_heap();
  priority_queue_custom_class();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}