#include <iostream>

// --- CÁC HÀM MẪU DÙNG CHO CON TRỎ HÀM ---
int tinhTong(int a, int b) { return a + b; }

int tinhHieu(int a, int b) { return a - b; }

// Một hàm nhận Con Trỏ Hàm làm tham số (Callback Function)
void thucThiPhepTinh(int x, int y, int (*phep_toan)(int, int)) {
  // Gọi hàm gián tiếp thông qua con trỏ
  int ket_qua = phep_toan(x, y);
  std::cout << "Ket qua cua phep toan la: " << ket_qua << std::endl;
}

int main() {
  // =========================================================================
  // REGION 1: CON TRỎ CẤP 2 CƠ BẢN & MẢNG 2 CHIỀU ĐỘNG
  // =========================================================================
  std::cout << "--- 1. CON TRO CAP 2 CO BAN ---" << std::endl;
  int data = 42;
  int *ptr1 = &data;  // Con trỏ cấp 1 giữ địa chỉ của data
  int **ptr2 = &ptr1; // Con trỏ cấp 2 giữ địa chỉ của ptr1

  std::cout << "Gia tri data: " << data << std::endl;
  std::cout << "Gia tri qua ptr1 (*ptr1): " << *ptr1 << std::endl;
  std::cout << "Gia tri qua ptr2 (**ptr2): " << **ptr2 << std::endl;
  std::cout << "Dia chi cua ptr1 luu trong ptr2: " << ptr2 << std::endl;

  std::cout << "\n--- Ua dung: Cap phat dong mang 2 chieu (2 hang, 3 cot) ---"
            << std::endl;
  int so_hang = 2;
  int so_cot = 3;

  // Bước 1: Tạo ra một mảng động chứa các con trỏ cấp 1 (quản lý các hàng)
  int **matrix = new int *[so_hang];

  // Bước 2: Cấp phát bộ nhớ cho từng hàng (mỗi hàng là một mảng 1D)
  for (int i = 0; i < so_hang; ++i) {
    matrix[i] = new int[so_cot];
  }

  // Điền dữ liệu và sử dụng như mảng 2 chiều bình thường
  matrix[0][0] = 10;
  matrix[0][1] = 20;
  matrix[0][2] = 30;
  matrix[1][0] = 40;
  matrix[1][1] = 50;
  matrix[1][2] = 60;

  std::cout << "Phan tu matrix[1][1]: " << matrix[1][1] << std::endl;

  // Bước 3: GIẢI PHÓNG BỘ NHỚ (Cực kỳ quan trọng, phải giải phóng ngược từ
  // trong ra ngoài)
  for (int i = 0; i < so_hang; ++i) {
    delete[] matrix[i]; // Xóa từng hàng trước
  }
  delete[] matrix; // Xóa mảng con trỏ quản lý hàng sau
  matrix = nullptr;

  std::cout << "Da giai phong ma tran dong an toan!" << std::endl;
  std::cout << "------------------------------------------------\n";

  // =========================================================================
  // REGION 2: CON TRỎ HÀM (FUNCTION POINTER)
  // =========================================================================
  std::cout << "--- 2. CON TRO HAM ---" << std::endl;

  // Khai báo con trỏ hàm trỏ đến hàm có dạng: int name(int, int)
  int (*ptr_to_func)(int, int) = nullptr;

  // Cho con trỏ trỏ đến hàm tinhTong (không dùng dấu ngoặc đơn của hàm)
  ptr_to_func = tinhTong;
  std::cout << "Goi tinhTong qua con tro: " << ptr_to_func(5, 3) << std::endl;

  // Đổi hướng con trỏ sang hàm tinhHieu
  ptr_to_func = tinhHieu;
  std::cout << "Goi tinhHieu qua con tro: " << ptr_to_func(5, 3) << std::endl;

  std::cout << "\n--- Ung dung truyen Con tro ham vao ham khac ---"
            << std::endl;
  // Truyền trực tiếp tên hàm tinhTong làm đối số
  std::cout << "Truyen tinhTong: ";
  thucThiPhepTinh(10, 20, tinhTong);

  std::cout << "Truyen tinhHieu: ";
  thucThiPhepTinh(10, 20, tinhHieu);
  std::cout << "------------------------------------------------\n";

  return 0;
}