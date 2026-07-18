#include <array>
#include <iostream>
#include <vector>

int main() {
  // =========================================================================
  // 1. MẢNG TĨNH CŨ & HIỆN ĐẠI (Giữ nguyên từ code của bạn)
  // =========================================================================
  std::cout << "--- 1. STD::ARRAY & C-STYLE ARRAY ---\n";
  int mang_co_dien[5] = {10, 20, 30, 40, 50};
  std::array<int, 3> mang_hien_dai = {1, 2, 3};
  std::cout << "Kich thuoc mang hien dai: " << mang_hien_dai.size() << "\n\n";

  // =========================================================================
  // 🌊 2. MẢNG ĐỘNG KIỂU CŨ (NEW[] VÀ DELETE[]) - CẤP PHÁT TRÊN HEAP
  // =========================================================================
  std::cout << "--- 2. MANG DONG THU CONG (HEAP MEMORY) ---\n";
  size_t kich_thuoc_nhap = 4; // Kích thước có thể thay đổi linh hoạt khi chạy

  // Cấp phát vùng nhớ trên Heap dùng con trỏ
  int *mang_heap = new int[kich_thuoc_nhap]{5, 10, 15, 20};

  std::cout << "Phan tu index 2 tren Heap: " << mang_heap[2] << "\n";

  // 🛑 CẠM BẪY CHÍ MẠNG: Phải luôn giải phóng bộ nhớ để tránh Memory Leak (Rò
  // rỉ)
  delete[] mang_heap;
  mang_heap = nullptr; // Xóa dấu vết con trỏ lạc hướng (Dangling Pointer)
  std::cout << "-> Da giai phong bo nho Heap an toan.\n\n";

  // =========================================================================
  // 🔀 3. MẢNG HAI CHIỀU / ĐA CHIỀU (MULTI-DIMENSIONAL ARRAY)
  // =========================================================================
  std::cout << "--- 3. MANG HAI CHIEU (MA TRAN 2x3) ---\n";
  // Bản chất trong bộ nhớ: Các hàng được xếp liên tiếp nhau thành một dải dài
  int ma_tran[2][3] = {
      {1, 2, 3}, // Hàng 0
      {4, 5, 6}  // Hàng 1
  };

  // Duyệt mảng 2 chiều bằng 2 vòng lặp lồng nhau (Nested loops)
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 3; ++j) {
      std::cout << ma_tran[i][j] << " ";
    }
    std::cout << "\n";
  }
  std::cout << "\n";

  // =========================================================================
  // 🏎️ 4. CON TRỎ NÂNG CAO & TỐI ƯU BỘ NHỚ VECTOR
  // =========================================================================
  std::cout << "--- 4. CON TRO & TOI UU VECTOR ---\n";

  // a. Bản chất: Tên của mảng tĩnh hoạt động như một con trỏ hằng trỏ vào phần
  // tử đầu tiên
  int *ptr_dau = mang_co_dien;
  std::cout << "Gia tri tai *ptr_dau: " << *ptr_dau
            << " (Tuong duong index 0)\n";
  std::cout << "Dich con tro *(ptr_dau + 1): " << *(ptr_dau + 1)
            << " (Tuong duong index 1)\n";

  // b. Cơ chế tối ưu bộ nhớ với std::vector
  std::vector<int> v;

  // Sử dụng reserve() để cấp phát trước dung lượng trống, tránh việc vector
  // phải liên tục sao chép và tái cấp phát vùng nhớ mới khi phình to âm thầm.
  v.reserve(100);
  v.push_back(10);
  v.push_back(20);

  std::cout << "Kich thuoc hien tai (Size): " << v.size() << "\n";
  std::cout << "Suc chua da cap phat truoc (Capacity): " << v.capacity()
            << "\n";

  // c. Duyệt vector chuyên nghiệp bằng Iterator thay vì dùng chỉ mục số
  std::cout << "Duyet Vector dung Iterator: ";
  for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
    std::cout << *it
              << " "; // Giải tham chiếu iterator để lấy giá trị giống con trỏ
  }
  std::cout << "\n------------------------------------------------\n";

  return 0;
}