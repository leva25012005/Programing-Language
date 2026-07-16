#include <array> // Cần để dùng std::array (C++11)
#include <iostream>
#include <vector> // Cần để dùng std::vector (Mảng động)


int main() {
  // -------------------------------------------------------------------------
  // 1. MẢNG TĨNH KIỂU C CỔ ĐIỂN (C-Style Array)
  // -------------------------------------------------------------------------
  // Khai báo và khởi tạo bằng dấu nhọn {}
  int mang_co_dien[5] = {10, 20, 30, 40, 50};

  std::cout << "--- 1. MANG CO DIEN ---" << std::endl;
  std::cout << "Phan tu dau tien (index 0): " << mang_co_dien[0] << std::endl;

  // Thay đổi giá trị
  mang_co_dien[1] = 99;

  // Duyệt mảng bằng vòng lặp for cơ bản
  std::cout << "Cac phan tu trong mang: ";
  for (int i = 0; i < 5; ++i) {
    std::cout << mang_co_dien[i] << " ";
  }
  std::cout << "\n------------------------------------------------\n";

  // -------------------------------------------------------------------------
  // 2. MẢNG TĨNH HIỆN ĐẠI (std::array - Khuyên Dùng Cho Mảng Cố Định)
  // -------------------------------------------------------------------------
  // Cú pháp: std::array<Kiểu_Dữ_Liệu, Số_Phần_Tử> tên_mảng;
  std::array<int, 3> mang_hien_dai = {1, 2, 3};

  std::cout << "--- 2. STD::ARRAY (C++11) ---" << std::endl;
  // Điểm mạnh: Biết tự quản lý kích thước của chính nó qua hàm .size()
  std::cout << "Kich thuoc cua mang: " << mang_hien_dai.size() << std::endl;

  // Tránh lỗi tràn mảng bằng hàm .at() thay vì dấu []
  // mang_hien_dai.at(5) -> Sẽ lập tức báo lỗi an toàn thay vì làm sập chương
  // trình âm thầm.
  std::cout << "------------------------------------------------\n";

  // -------------------------------------------------------------------------
  // 3. MẢNG ĐỘNG (std::vector - Vua Của Mọi Loại Mảng Trong Thực Tế)
  // -------------------------------------------------------------------------
  // Mảng tĩnh có nhược điểm chí mạng: Khai báo bao nhiêu dùng bấy nhiêu, không
  // co giãn được. std::vector xuất hiện để giải quyết việc đó: Tự động phình to
  // khi thêm phần tử.
  std::vector<int> mang_dong; // Ban đầu mảng rỗng (0 phần tử)

  // Thêm phần tử vào cuối mảng bằng .push_back()
  mang_dong.push_back(100);
  mang_dong.push_back(200);
  mang_dong.push_back(300);

  std::cout << "--- 3. MANG DONG (STD::VECTOR) ---" << std::endl;
  std::cout << "Kich thuoc sau khi them: " << mang_dong.size() << std::endl;
  std::cout << "Phan tu cuoi cung: " << mang_dong.back() << std::endl;

  // Duyệt mảng bằng cách viết hiện đại (Range-based for loop)
  std::cout << "Cac phan tu trong Vector: ";
  for (int x : mang_dong) {
    std::cout << x << " ";
  }
  std::cout << "\n------------------------------------------------\n";

  return 0;
}