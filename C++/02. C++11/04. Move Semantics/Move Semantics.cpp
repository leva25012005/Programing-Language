#include <iostream>
#include <string>
#include <utility> // Bắt buộc cho std::move và std::forward
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Vấn đề của C++ cũ (trước C++11):
    Khi bạn gán hoặc truyền một Object khổng lồ (ví dụ mảng 1 triệu phần tử),
    C++ bắt buộc phải COPY (sao chép) toàn bộ dữ liệu đó sang một vùng RAM mới.
    Điều này cực kỳ tốn CPU và RAM.

 2. Lvalue và Rvalue:
    - Lvalue (Left value): Là những biến có TÊN, có ĐỊA CHỈ trên RAM, tồn tại
 lâu dài (vd: int x = 10; 'x' là lvalue).
    - Rvalue (Right value): Là những giá trị TẠM THỜI, không có tên, sinh ra rồi
 chết ngay ở cuối dòng code (vd: số '10', kết quả của hàm 'tinh_tong()').

 3. Move Semantics (Ngữ nghĩa Di chuyển):
    - Thay vì COPY, ta "CƯỚP" (Steal) con trỏ bộ nhớ của các đối tượng TẠM THỜI
 (Rvalue) bởi vì đằng nào chúng cũng sắp bị tiêu hủy. Điều này biến O(N) thành
 O(1).
*/

// ==========================================
// CLASS CHUYÊN GIA: HIỆN THỰC QUY TẮC 5 (RULE OF FIVE)
// ==========================================
class MangDong {
private:
  int *data;
  size_t size;

public:
  // 1. Constructor mặc định
  MangDong(size_t s) : size(s), data(new int[s]) {
    std::cout << "  [Tao moi] Cap phat " << size << " phan tu.\n";
  }

  // 2. Destructor (Hàm hủy)
  ~MangDong() {
    if (data != nullptr) {
      std::cout << "  [Huy] Xoa bo nho.\n";
      delete[] data;
    } else {
      std::cout << "  [Huy] Object rong (Da bi Move).\n";
    }
  }

  // 3. COPY CONSTRUCTOR (Sao chép sâu - Deep Copy) -> Rất tốn kém!
  MangDong(const MangDong &other)
      : size(other.size), data(new int[other.size]) {
    for (size_t i = 0; i < size; ++i) {
      data[i] = other.data[i];
    }
    std::cout << "  [COPY Constructor] Đã COPY cham chap " << size
              << " phan tu.\n";
  }

  // 4. COPY ASSIGNMENT (Toán tử gán sao chép)
  MangDong &operator=(const MangDong &other) {
    if (this != &other) {
      delete[] data; // Xóa dữ liệu cũ của chính mình
      size = other.size;
      data = new int[other.size];
      for (size_t i = 0; i < size; ++i)
        data[i] = other.data[i];
      std::cout << "  [COPY Assignment] Đã COPY cham chap.\n";
    }
    return *this;
  }

  // ==========================================
  // 5. MOVE CONSTRUCTOR (Ma thuật Di chuyển - O(1))
  // Cú pháp: && (Rvalue Reference - Tham chiếu rvalue)
  // TỪ KHÓA BẮT BUỘC: noexcept (Cam kết không ném ngoại lệ, cực kỳ quan trọng
  // cho std::vector)
  // ==========================================
  MangDong(MangDong &&other) noexcept : size(other.size), data(other.data) {
    // Bước 1: "Cướp" con trỏ data và size từ 'other' (Đã làm ở danh sách khởi
    // tạo)

    // Bước 2: CẮT ĐỨT liên kết của 'other' với vùng nhớ đó
    // Nếu không làm bước này, khi 'other' bị Hủy, nó sẽ delete luôn vùng nhớ ta
    // vừa cướp!
    other.data = nullptr;
    other.size = 0;

    std::cout << "  [MOVE Constructor] Đã CƯỚP tài nguyên thành công (O(1)).\n";
  }

  // 6. MOVE ASSIGNMENT (Toán tử gán di chuyển)
  MangDong &operator=(MangDong &&other) noexcept {
    if (this != &other) {
      delete[] data; // Dọn dẹp nhà cửa của mình trước

      // Cướp tài nguyên
      data = other.data;
      size = other.size;

      // Xóa trí nhớ của kẻ bị cướp
      other.data = nullptr;
      other.size = 0;

      std::cout << "  [MOVE Assignment] Đã dọn dẹp và CƯỚP tài nguyên.\n";
    }
    return *this;
  }
};

// ==========================================
// 1. MỨC CƠ BẢN: RVALUE REFERENCE (&&)
// ==========================================
void demo_lvalue_rvalue() {
  std::cout << "\n--- 1. BAZIC: LVALUE, RVALUE & THAM CHIEU && ---\n";

  int x = 10; // 'x' là Lvalue, '10' là Rvalue

  // Tham chiếu thông thường (&) CHỈ có thể trỏ tới Lvalue
  int &ref_x = x;
  // int& ref_10 = 10; // LỖI BIÊN DỊCH! Không thể trỏ & vào số 10 tạm thời.

  // THAM CHIẾU RVALUE (&&): Chuyên dùng để bắt các giá trị tạm thời!
  int &&rref_10 = 10;
  rref_10 = 99; // Hoàn toàn hợp lệ, ta đã kéo dài tuổi thọ của số 10 và sửa nó.
  std::cout << "Gia tri rref_10: " << rref_10 << "\n";
}

// ==========================================
// 2. MỨC TRUNG CẤP: STD::MOVE LÀ GÌ?
// ==========================================
void demo_std_move() {
  std::cout << "\n--- 2. INTERMEDIATE: BẢN CHẤT CỦA STD::MOVE ---\n";

  MangDong a(100);

  // Nếu gán b = a, Trình biên dịch sẽ gọi COPY CONSTRUCTOR vì 'a' là một
  // Lvalue.
  std::cout << "> Gan thong thuong (COPY):\n";
  MangDong b = a;

  // std::move(a) LÀM CÁI GÌ?
  // SỰ THẬT: std::move KHÔNG HỀ di chuyển bất cứ thứ gì!
  // Nó chỉ là một cú "Ép kiểu" (Cast), biến 'a' từ Lvalue thành Rvalue
  // Reference (&&). Bằng cách này, nó đánh lừa Trình biên dịch để Trình biên
  // dịch chọn gọi MOVE CONSTRUCTOR thay vì Copy.

  std::cout << "\n> Gan bang std::move (MOVE):\n";
  MangDong c = std::move(a);
  // 'a' bây giờ là một cái xác rỗng (data = nullptr). Đừng dùng 'a' nữa!
}

// ==========================================
// 3. MỨC NÂNG CAO: TẠI SAO PHẢI CÓ 'NOEXCEPT'?
// ==========================================
void demo_vector_reallocation() {
  std::cout
      << "\n--- 3. ADVANCED: QUYỀN NĂNG CỦA NOEXCEPT TRONG STD::VECTOR ---\n";

  std::vector<MangDong> v;

  // Ép vector cấp phát đúng 1 chỗ
  v.reserve(1);

  std::cout << "> Nhet phan tu dau tien vao vector:\n";
  v.push_back(MangDong(10)); // Truyền Rvalue ẩn danh -> Gọi Move Constructor

  std::cout
      << "\n> Nhet phan tu thu 2 (Vuot Capacity, Vector phai Reallocation):\n";
  // Khi Reallocation, Vector đập bỏ RAM cũ, xây RAM mới to hơn.
  // Nó cần dọn phần tử số 1 sang nhà mới.
  // NẾU Move Constructor có 'noexcept': Vector tin tưởng dùng MOVE (nhanh).
  // NẾU KHÔNG CÓ 'noexcept': Vector sợ quá trình Move sinh lỗi làm mất dữ liệu,
  // nó sẽ ngậm ngùi dùng COPY (chậm).
  v.push_back(MangDong(20));
}

// ==========================================
// 4. MỨC CHUYÊN GIA: PERFECT FORWARDING (CHUYỂN TIẾP HOÀN HẢO)
// ==========================================
// Vấn đề: Khi viết Hàm bọc (Wrapper), nếu ta nhận tham số, nó sẽ luôn biến
// thành Lvalue bên trong hàm, làm phá hỏng cơ chế Move khi truyền tiếp vào hàm
// khác.

void ham_xu_ly(MangDong &obj) { std::cout << "    => Nhận LVALUE (Sẽ Copy)\n"; }
void ham_xu_ly(MangDong &&obj) {
  std::cout << "    => Nhận RVALUE (Sẽ Move)\n";
}

// Sử dụng Universal Reference (T&&) và std::forward
template <typename T> void wrapper(T &&arg) {
  // Nếu chỉ gọi: ham_xu_ly(arg); -> Sẽ LUÔN LUÔN gọi hàm LVALUE. (Vì biến 'arg'
  // có tên, nó là Lvalue).

  // std::forward sẽ giữ NGUYÊN BẢN CHẤT của tham số truyền vào ban đầu.
  // Nếu truyền vào Lvalue -> Nó giữ là Lvalue.
  // Nếu truyền vào Rvalue -> Nó cast lại thành Rvalue (như std::move).
  ham_xu_ly(std::forward<T>(arg));
}

void demo_perfect_forwarding() {
  std::cout << "\n--- 4. EXPERT: PERFECT FORWARDING (STD::FORWARD) ---\n";

  MangDong x(5);

  std::cout << "> Truyen bien x (Lvalue) qua Wrapper:\n";
  wrapper(x);

  std::cout << "\n> Truyen object an danh (Rvalue) qua Wrapper:\n";
  wrapper(MangDong(5));
}

// ==========================================
// TỔNG KẾT LUẬT LẬP TRÌNH HIỆN ĐẠI
// ==========================================
/*
 1. RULE OF ZERO: Nếu Class của bạn chỉ chứa các thành phần chuẩn như
 std::vector, std::string, std::unique_ptr... BẠN KHÔNG CẦN VIẾT Rule of 5. C++
 sẽ tự động sinh ra Copy và Move Constructor chuẩn xịn cho bạn.

 2. KHI NÀO DÙNG STD::MOVE?
    - Bất cứ khi nào bạn có một biến không cần dùng tới nữa, và bạn muốn tống nó
      vào một class/hàm khác để tiết kiệm hiệu năng.

 3. CẠM BẪY LỚN NHẤT:
    - Return std::move(x); từ một hàm biến cục bộ là một SAI LẦM!
    Trình biên dịch C++ hiện đại có tính năng RVO (Return Value Optimization)
    giúp xóa bỏ 100% chi phí copy mà không cần std::move. Việc chèn std::move
 vào return sẽ phá hỏng RVO và làm code chậm đi!
*/

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  demo_lvalue_rvalue();
  demo_std_move();
  demo_vector_reallocation();
  demo_perfect_forwarding();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}