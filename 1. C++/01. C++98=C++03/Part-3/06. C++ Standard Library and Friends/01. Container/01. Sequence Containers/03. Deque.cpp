#include <algorithm>
#include <deque>
#include <iostream>
#include <string>


// ==========================================
// CLASS PHỤ TRỢ CHO PHẦN CHUYÊN GIA
// ==========================================
class KhachHang {
public:
  std::string ten;
  int the_vip;

  KhachHang(std::string t, int vip) : ten(t), the_vip(vip) {
    std::cout << "  [Tao moi] Khach hang: " << ten << "\n";
  }

  // Copy Constructor
  KhachHang(const KhachHang &other) : ten(other.ten), the_vip(other.the_vip) {
    std::cout << "  [Copy] Khach hang: " << ten << "\n";
  }

  // Move Constructor
  KhachHang(KhachHang &&other) noexcept
      : ten(std::move(other.ten)), the_vip(other.the_vip) {
    std::cout << "  [Move] Khach hang: " << ten << "\n";
  }
};

// ==========================================
// 1. MỨC CƠ BẢN: KHỞI TẠO VÀ THAO TÁC 2 ĐẦU
// ==========================================
void deque_co_ban() {
  std::cout << "\n--- 1. BAZIC: THAO TAC HAI DAU (O(1)) ---\n";

  // Khởi tạo
  std::deque<int> dq1;                // Deque rỗng
  std::deque<int> dq2 = {10, 20, 30}; // Khởi tạo có sẵn dữ liệu
  std::deque<int> dq3(5, 100); // 5 phần tử, mỗi phần tử giá trị 100

  // ĐÂY LÀ SỨC MẠNH CỦA DEQUE SO VỚI VECTOR: Thêm vào ĐẦU siêu nhanh (O(1))
  dq1.push_front(2); // Thêm vào đầu
  dq1.push_front(1);

  // Thêm vào CUỐI cũng siêu nhanh (O(1))
  dq1.push_back(3);
  dq1.push_back(4);
  // dq1 lúc này: 1, 2, 3, 4

  // Rút phần tử ra khỏi 2 đầu
  dq1.pop_front(); // Bỏ số 1
  dq1.pop_back();  // Bỏ số 4
  // dq1 lúc này: 2, 3

  // Truy xuất dữ liệu (Giống hệt Vector - Tốc độ O(1))
  std::cout << "Phan tu dau tien: " << dq1.front() << "\n";
  std::cout << "Phan tu cuoi cung: " << dq1.back() << "\n";

  // Dùng [] không kiểm tra tràn viền, dùng .at() an toàn có ném lỗi
  std::cout << "Phan tu index 0 (dq1[0]): " << dq1[0] << "\n";

  std::cout << "Duyet dq1: ";
  for (const int &val : dq1) {
    std::cout << val << " ";
  }
  std::cout << "\n";
}

// ==========================================
// 2. MỨC TRUNG CẤP: BÍ MẬT BỘ NHỚ (Không có Capacity)
// ==========================================
void deque_bo_nho() {
  std::cout << "\n--- 2. INTERMEDIATE: BI MAT BO NHO DEQUE ---\n";

  std::deque<int> dq;

  // SỰ KHÁC BIỆT CHÍNH: Deque KHÔNG CÓ hàm .capacity() và .reserve() như
  // Vector! Tại sao? Vì Vector là một mảng khổng lồ liên tục. Khi hết chỗ, nó
  // phải đập đi xây lại ở chỗ khác. Deque là "Mảng của các mảng nhỏ" (Map of
  // chunks). Khi hết chỗ, nó chỉ đơn giản là xin hệ điều hành một cục RAM nhỏ
  // mới (chunk) và nối con trỏ vào cục cũ. Nên nó không bao giờ cần phải
  // Reallocation toàn bộ dữ liệu.

  std::cout << "Size hien tai: " << dq.size() << "\n";

  for (int i = 0; i < 1000; ++i) {
    dq.push_back(i);
    // Quá trình push_back/push_front không bao giờ làm các con trỏ hiện tại bị
    // vô hiệu hóa (Không giống vector, khi reallocate sẽ làm dangling pointer).
  }

  std::cout << "Size sau khi them 1000 phan tu: " << dq.size() << "\n";

  // Vẫn có thể thu hồi các chunk RAM thừa nếu có
  dq.shrink_to_fit();
}

// ==========================================
// 3. MỨC NÂNG CAO: THAO TÁC Ở GIỮA VÀ THUẬT TOÁN
// ==========================================
void deque_thao_tac() {
  std::cout << "\n--- 3. ADVANCED: THEM/XOA O GIUA & THUAT TOAN ---\n";

  std::deque<int> dq = {10, 20, 30, 40, 50};

  // Chèn vào giữa (CỰC KỲ CHẬM - O(n)). Giống vector, nó phải đẩy các phần tử
  // khác ra.
  dq.insert(dq.begin() + 2, 99); // Chèn số 99 vào index 2

  // Xóa ở giữa (Cũng chậm O(n))
  dq.erase(dq.begin() + 1); // Xóa phần tử ở index 1 (số 20)

  // Nhờ có Random Access Iterator (như Vector), Deque dùng được MỌI thuật toán
  // chuẩn! Khác với list, list không dùng được std::sort.

  // Đảo ngược
  std::reverse(dq.begin(), dq.end());

  // Sắp xếp
  std::sort(dq.begin(), dq.end());

  std::cout << "Sau khi insert/erase, reverse va sort: ";
  for (int val : dq)
    std::cout << val << " ";
  std::cout << "\n";
}

// ==========================================
// 4. MỨC CHUYÊN GIA: EMPLACE_FRONT / BACK VÀ ĐỐI TƯỢNG
// ==========================================
void deque_chuyen_gia_objects() {
  std::cout << "\n--- 4. EXPERT: EMPLACE VS PUSH ---\n";

  std::deque<KhachHang> hang_doi;

  std::cout << "Dung push_front:\n";
  // Tạo object tạm -> Copy/Move vào deque -> Hủy object tạm
  hang_doi.push_front(KhachHang("Nguyen Van A", 1));

  std::cout << "\nDung emplace_front:\n";
  // Khởi tạo trực tiếp thẳng vào vùng nhớ của Deque, KHÔNG COPY, KHÔNG MOVE!
  hang_doi.emplace_front("Tran Thi B", 2);

  std::cout << "\nDung emplace_back:\n";
  hang_doi.emplace_back("Le Van C", 3);
}

// ==========================================
// TỔNG KẾT LUẬT CHỌN DEQUE HAY VECTOR (Kinh nghiệm thực chiến)
// ==========================================
/*
1. MẶC ĐỊNH: Luôn dùng Vector. Tốc độ duyệt (for loop) của Vector
   luôn nhanh hơn Deque nhờ nằm liền kề trên 1 dải RAM (CPU Cache Friendly).
2. DÙNG DEQUE KHI:
   - Bạn cần liên tục THÊM/XÓA ở cả ĐẦU và CUỐI (Ví dụ: Game luân chuyển thẻ
bài).
   - Dữ liệu quá lớn (hàng triệu phần tử), việc cấp phát một mảng khổng lồ liền
kề của Vector có thể thất bại do RAM bị phân mảnh. Deque dùng các chunk nhỏ nên
dễ thở hơn.
*/

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  deque_co_ban();
  deque_bo_nho();
  deque_thao_tac();
  deque_chuyen_gia_objects();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}