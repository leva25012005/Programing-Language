#include <iostream>
#include <memory> // 💡 Bắt buộc phải có để sử dụng Smart Pointers (C++11)
#include <utility> // Cho std::move

int main() {
  // =========================================================================
  // 🏛️ MỤC 1: HẰNG CON TRỎ VÀ CON TRỎ HẰNG (CONST WITH POINTER)
  // =========================================================================
  std::cout << "--- 1. CONST VA CON TRO ---\n";
  int a = 10;
  int b = 20;

  // a. Con trỏ trỏ đến Hằng số (Pointer to Const)
  // Ý nghĩa: Không cho phép sửa giá trị của vùng nhớ ĐƯỢC TRỎ TỚI thông qua con
  // trỏ này.
  const int *ptr_to_const = &a;
  // *ptr_to_const = 99; // ❌ LỖI BIÊN DỊCH! Không được sửa giá trị qua ptr.
  ptr_to_const = &b; //  HỢP LỆ! Được phép đổi địa chỉ trỏ sang b.

  // b. Con trỏ Hằng (Const Pointer)
  // Ý nghĩa: Bản thân con trỏ bị cố định địa chỉ, KHÔNG được trỏ đi nơi khác.
  int *const const_ptr = &a;
  *const_ptr = 99; //  HỢP LỆ! Được phép sửa giá trị vùng nhớ gốc.
  // const_ptr = &b;     // ❌ LỖI BIÊN DỊCH! Địa chỉ của con trỏ đã bị khóa
  // chặt.

  std::cout << "Gia tri a sau khi sua qua const_ptr: " << a << "\n";
  std::cout << "------------------------------------------\n\n";

  // =========================================================================
  // 🔀 MỤC 2: CON TRỎ TRỎ ĐẾN CON TRỎ (POINTER TO POINTER)
  // =========================================================================
  std::cout << "--- 2. CON TRO CAP HAI (POINTER TO POINTER) ---\n";
  int x = 500;
  int *ptr_cap1 = &x;         // Con trỏ cấp 1 lưu địa chỉ của x
  int **ptr_cap2 = &ptr_cap1; // Con trỏ cấp 2 lưu địa chỉ của ptr_cap1

  std::cout << "Gia tri cua x: " << x << "\n";
  std::cout << "Giai tham chieu 1 lan (*ptr_cap1): " << *ptr_cap1 << "\n";

  // Phải giải tham chiếu 2 lần để đi xuyên qua 2 lớp địa chỉ và chạm vào x
  std::cout << "Giai tham chieu 2 lan (**ptr_cap2): " << **ptr_cap2 << "\n";
  std::cout << "------------------------------------------\n\n";

  // =========================================================================
  // 🧠 MỤC 3: CON TRỎ THÔNG MINH (MODERN C++ SMART POINTERS)
  // =========================================================================
  std::cout << "--- 3. CON TRO THONG MINH (RAII AUTOMATION) ---\n";

  { // Tạo một block scope nhỏ để kiểm tra tính tự động xóa
    // a. std::unique_ptr (Độc quyền sở hữu vùng nhớ)
    // Thay vì dùng new, ta dùng std::make_unique. Nó an toàn và tối ưu hơn.
    std::unique_ptr<int> u_ptr = std::make_unique<int>(888);
    std::cout << "Gia tri cua unique_ptr: " << *u_ptr << "\n";

    // std::unique_ptr<int> u_ptr2 = u_ptr; // ❌ LỖI BIÊN DỊCH! Không cho phép
    // sao chép.

    // Muốn chuyển quyền sở hữu sang con trỏ khác, ta buộc phải dùng std::move
    std::unique_ptr<int> u_ptr2 = std::move(u_ptr);
    if (u_ptr == nullptr) {
      std::cout << "-> u_ptr cu da bi lam rong sau khi move.\n";
    }
    std::cout << "Gia tri tai u_ptr2 moi: " << *u_ptr2 << "\n";

  } // 🌟 HẾT BLOCK SCOPE TẠI ĐÂY! u_ptr2 tự động gọi 'delete' vùng nhớ số 888
    // mà ta không cần gõ một dòng lệnh delete nào.

  std::cout
      << "-> Vung nho cua unique_ptr da duoc tu dong giai phong an toan!\n";
  std::cout << "------------------------------------------\n";

  return 0;
}