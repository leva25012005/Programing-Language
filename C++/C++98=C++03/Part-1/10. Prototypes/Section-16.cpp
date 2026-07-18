#include <iostream>
#include <memory> // 💡 Thư viện bắt buộc để sử dụng Con trỏ thông minh

// =========================================================================
// 🌐 ZONE 1: STATIC STORAGE DURATION (VÒNG ĐỜI TOÀN CỤC/TĨNH)
// =========================================================================
// Biến này tồn tại suốt thời gian chạy của chương trình, cấp phát khi start và
// xóa khi shutdown.
int bien_toan_cuc = 100;

void minhHoaStatic() {
  // 🌟 MỞ RỘNG: Biến tĩnh cục bộ (Static Local Variable)
  // Chỉ khởi tạo 1 lần duy nhất. Khi hàm kết thúc, biến KHÔNG bị xóa mà giữ
  // nguyên giá trị cho lần gọi sau!
  static int dem_so = 0;
  dem_so++;
  std::cout << "Ham minhHoaStatic duoc goi, dem_so = " << dem_so << std::endl;
}

int main() {
  // =========================================================================
  // 📦 ZONE 2: LOCAL & BLOCK SCOPE (AUTOMATIC STORAGE / STACK MEMORY)
  // =========================================================================
  std::cout << "--- 1. PHAM VI VA VONG DOI TREN STACK ---" << std::endl;

  int x = 10; // Phạm vi local của hàm main bắt đầu từ đây
  std::cout << "x o ngoai = " << x << std::endl;

  { // 🔴 Một Block Scope mới bắt đầu
    // Cạm bẫy Shadowing (Che khuất biến): Khai báo trùng tên x
    int x = 99; // x này là biến hoàn toàn mới, nằm ở ô nhớ khác trên Stack
    std::cout << "x o trong Block = " << x << " (Gia tri 99 da che khuat 10)"
              << std::endl;
  } // 🔴 Block Scope kết thúc -> Biến x (99) lập tức bị XÓA tự động khỏi Stack
    // (Lifetime kết thúc)

  std::cout << "x sau khi thoat Block = " << x << " (Gia tri 10 quay tro lai)"
            << std::endl;
  std::cout << "------------------------------------------\n";

  // Kiểm tra biến static cục bộ
  minhHoaStatic();
  minhHoaStatic(); // Bạn sẽ thấy giá trị tăng tiến chứ không bị reset
  std::cout << "------------------------------------------\n";

  // =========================================================================
  // 💾 ZONE 3: DYNAMIC STORAGE DURATION (HEAP MEMORY - CỔ ĐIỂN)
  // =========================================================================
  std::cout << "--- 2. CAP PHAT DONG CO DIEN (NEW / DELETE) ---" << std::endl;

  // Cấp phát động 1 biến đơn
  int *ptr = new int(123);
  std::cout << "Gia tri vung nho Heap: " << *ptr << std::endl;
  delete ptr; // 🛑 BẮT BUỘC: Giải phóng vùng nhớ để tránh rò rỉ bộ nhớ (Memory
              // Leak)
  ptr = nullptr; // Xóa xong gán nullptr để tránh lỗi con trỏ lạc hướng
                 // (Dangling Pointer)

  // Cấp phát động cho Mảng
  int *mang_dong = new int[3]{10, 20, 30};
  std::cout << "Mang dong phan tu thu 2: " << mang_dong[1] << std::endl;
  delete[] mang_dong; // 🛑 Nhớ dùng delete[] cho mảng!
  mang_dong = nullptr;
  std::cout << "------------------------------------------\n";

  // =========================================================================
  // 🧠 ZONE 4: MỞ RỘNG HIỆN ĐẠI - CON TRỎ THÔNG MINH (SMART POINTERS)
  // =========================================================================
  std::cout << "--- 3. CON TRO THONG MINH (C++11 SMART POINTERS) ---"
            << std::endl;

  {
    // Unique Pointer: Quản lý độc quyền vùng nhớ. Không cho phép sao chép
    // (copy). Đặc điểm: Tự động gọi 'delete' khi biến 'u_ptr' đi ra khỏi dấu
    // ngoặc nhọn } của block này!
    std::unique_ptr<int> u_ptr = std::make_unique<int>(888);
    std::cout << "Unique Pointer gia tri = " << *u_ptr << std::endl;

    // std::unique_ptr<int> u_ptr2 = u_ptr; // ❌ LỖI TRÌNH BIÊN DỊCH! Không cho
    // phép chung chạ tài nguyên.
  } // 😎 Hết block, vùng nhớ chứa số 888 TỰ ĐỘNG ĐƯỢC XÓA mà không cần gõ lệnh
    // delete!

  std::cout << "Giai phong thong minh thanh cong!" << std::endl;
  std::cout << "------------------------------------------\n";

  return 0;
}