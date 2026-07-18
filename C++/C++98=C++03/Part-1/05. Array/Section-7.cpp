#include <iostream>
#include <vector>

int main() {
  // -------------------------------------------------------------------------
  // 1. MẢNG 2 CHIỀU TĨNH CỔ ĐIỂN (C-Style 2D Array)
  // -------------------------------------------------------------------------
  // Khai báo ma trận 2 hàng, 3 cột và khởi tạo giá trị
  int mang_2d[2][3] = {
      {1, 2, 3}, // Hàng 0
      {4, 5, 6}  // Hàng 1
  };

  std::cout << "--- 1. MANG 2 CHIEU TINH ---" << std::endl;
  // Truy cập phần tử ở Hàng 1, Cột 2 (chính là số 6)
  std::cout << "Phan tu tai [1][2] la: " << mang_2d[1][2] << std::endl;

  // Thay đổi giá trị
  mang_2d[0][0] = 99; // Đổi số 1 thành 99

  // Duyệt mảng 2 chiều: Ta dùng 2 vòng lặp for lồng nhau
  // Vòng lặp ngoài duyệt qua từng HÀNG, vòng lặp trong duyệt qua từng CỘT
  std::cout << "In toan bo ma tran tinh:\n";
  for (int i = 0; i < 2; ++i) {   // i đại diện cho hàng
    for (int j = 0; j < 3; ++j) { // j đại diện cho cột
      std::cout << mang_2d[i][j] << " ";
    }
    std::cout << '\n'; // Hết một hàng thì xuống dòng
  }
  std::cout << "------------------------------------------------\n";

  // -------------------------------------------------------------------------
  // 2. MẢNG 2 CHIỀU ĐỘNG (Vector lồng Vector - Khuyên Dùng Trong Thực Tế)
  // -------------------------------------------------------------------------
  // Bản chất là một Vector chứa các Vector khác bên trong:
  // std::vector<std::vector<Kiểu>> Khởi tạo ma trận động có 3 hàng, mỗi hàng
  // chứa 4 phần tử (tất cả bằng 0)
  std::vector<std::vector<int>> matrix_dong(3, std::vector<int>(4, 0));

  // Gán giá trị thủ công
  matrix_dong[0][1] = 7;
  matrix_dong[2][3] = 88;

  // Điểm mạnh: Có thể thêm hàng hoặc thêm cột bất kỳ lúc nào bằng .push_back()
  std::vector<int> hang_moi = {10, 20, 30, 40};
  matrix_dong.push_back(hang_moi); // Bây giờ ma trận tự động tăng lên 4 hàng!

  std::cout << "--- 2. VECTOR 2 CHIEU (DYNAMIC) ---" << std::endl;
  std::cout << "So luong hang hien tai: " << matrix_dong.size() << std::endl;
  std::cout << "So luong cot cua hang dau tien: " << matrix_dong[0].size()
            << std::endl;

  // Cách duyệt Vector 2D hiện đại dùng Range-based for loop
  std::cout << "In toan bo ma tran dong:\n";
  for (const auto &hang :
       matrix_dong) { // Duyệt qua từng hàng (mỗi hàng là một vector)
    for (int phan_tu : hang) { // Duyệt qua từng phần tử trong hàng đó
      std::cout << phan_tu << "\t"; // Dùng \t (tab) để căn lề cho thẳng cột
    }
    std::cout << '\n';
  }
  std::cout << "------------------------------------------------\n";

  return 0;
}