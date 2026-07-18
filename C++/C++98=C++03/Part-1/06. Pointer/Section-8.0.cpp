#include <iostream>

int main() {
  // ==========================================
  // 1. CƠ BẢN VỀ CON TRỎ (& và *)
  // ==========================================
  std::cout << "--- 1. CO BAN VE CON TRO ---" << std::endl;
  int x = 10;

  // Dấu & dùng để lấy "địa chỉ nhà" của biến x
  int *ptr = &x;

  std::cout << "Gia tri cua x: " << x << std::endl;
  std::cout << "Dia chi cua x trong RAM (&x): " << &x << std::endl;
  std::cout << "Gia tri ma con tro 'ptr' dang luu: " << ptr << std::endl;

  // Dấu * (Toán tử giải tham chiếu) dùng để "mở khóa" địa chỉ,
  // đi vào tận nơi và thay đổi giá trị gốc
  *ptr = 20;
  std::cout << "Gia tri cua x sau khi thay doi qua con tro (*ptr = 20): " << x
            << std::endl;
  std::cout << "--------------------------------\n";

  // ==========================================
  // 2. CON TRỎ VÀ MẢNG (Pointer Arithmetic)
  // ==========================================
  std::cout << "--- 2. CON TRO VA MANG ---" << std::endl;
  int arr[3] = {100, 200, 300};

  // BÍ MẬT: Tên mảng thực chất là một con trỏ trỏ đến phần tử đầu tiên (arr[0])
  int *arr_ptr = arr;

  std::cout << "Phan tu arr[0]: " << arr[0] << std::endl;
  std::cout << "Gia tri tai *arr_ptr: " << *arr_ptr << std::endl;

  // Toán học con trỏ: Cộng 1 vào con trỏ nghĩa là nó tự động nhảy
  // sang ô nhớ của phần tử tiếp theo trong mảng
  std::cout << "Gia tri tai *(arr_ptr + 1): " << *(arr_ptr + 1)
            << " (Chinh la arr[1])" << std::endl;
  std::cout << "--------------------------------\n";

  // ==========================================
  // 3. CẤP PHÁT BỘ NHỚ ĐỘNG (Dynamic Memory)
  // ==========================================
  std::cout << "--- 3. CAP PHAT DONG ---" << std::endl;

  // Dùng 'new' để xin hệ điều hành cấp phát 1 ô nhớ mới ngay lúc chương trình
  // đang chạy
  int *dynamic_ptr = new int;
  *dynamic_ptr = 999; // Gán giá trị cho ô nhớ vừa xin

  std::cout << "Gia tri vung nho dong: " << *dynamic_ptr << std::endl;

  // QUY TẮC SỐ 1: Có vay (new) thì phải có trả (delete)
  // Nếu quên delete, máy tính sẽ bị rò rỉ bộ nhớ (Memory Leak)
  delete dynamic_ptr;

  // Sau khi trả, con trỏ vẫn trỏ về địa chỉ cũ (nhưng vùng đó ko còn là của ta
  // nữa). Gán nó bằng nullptr để đảm bảo an toàn tuyệt đối.
  dynamic_ptr = nullptr;

  std::cout << "Da xoa va giai phong bo nho an toan!" << std::endl;
  std::cout << "--------------------------------\n";

  return 0;
}