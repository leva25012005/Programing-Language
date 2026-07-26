#include <iostream>
#include <string>
#include <vector>

// 4 Thư viện làm nên sức mạnh C++17
#include <any>
#include <cstddef> // Chứa std::byte
#include <optional>
#include <variant>


// ==========================================
// KỸ THUẬT MA THUẬT C++17 (Dùng cho std::variant)
// ==========================================
// Đây là "overloaded idiom". Một kỹ thuật kinh điển để gom nhiều hàm Lambda
// thành một bộ xử lý (Visitor) duy nhất cho std::variant.
template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};

// Deduction guide (Hướng dẫn suy luận kiểu cho compiler)
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

// ==========================================
// 1. STD::OPTIONAL (CÓ HOẶC KHÔNG)
// ==========================================
// Xưa: Trả về string rỗng "" hoặc con trỏ NULL nếu không tìm thấy.
// Nay: Trả về std::optional. Đọc vào là hiểu ngay hàm này có thể thất bại!
std::optional<std::string> tim_sinh_vien(int id) {
  if (id == 101)
    return "Le Van Nhan";
  if (id == 102)
    return "Tran Thi Binh";
  return std::nullopt; // Không tìm thấy (Tuyệt đối an toàn, rõ ràng)
}

void demo_optional() {
  std::cout << "\n--- 1. STD::OPTIONAL (CO HOAC KHONG) ---\n";

  // Test 1: Tìm thấy
  auto kq1 = tim_sinh_vien(101);

  // Kiểm tra an toàn
  if (kq1.has_value()) { // Hoặc viết gọn: if (kq1)
    std::cout << "Tim thay ID 101: " << kq1.value() << "\n";
    // Có thể dùng *kq1 giống con trỏ, nhưng .value() an toàn hơn vì nó ném lỗi
    // nếu rỗng.
  }

  // Test 2: Không tìm thấy và dùng giá trị mặc định (value_or)
  auto kq2 = tim_sinh_vien(999);

  // Nếu kq2 rỗng, nó sẽ tự động lấy chữ "Khach Vang Lai". Cực kỳ thanh lịch!
  std::string ten = kq2.value_or("Khach Vang Lai");
  std::cout << "Ket qua ID 999: " << ten << "\n";
}

// ==========================================
// 2. STD::VARIANT (CÔNG NGHỆ CAO CỦA UNION)
// ==========================================
// Xưa: Dùng `union` (Tiết kiệm RAM nhưng siêu nguy hiểm vì nó không biết mình
// đang chứa gì). Nay: Dùng `variant`. Nó luôn biết chính xác kiểu dữ liệu hiện
// tại là gì.
void demo_variant() {
  std::cout << "\n--- 2. STD::VARIANT (AN TOAN TUYET DOI) ---\n";

  // Biến v chỉ có thể chứa 1 trong 3 loại: int, double, hoặc string.
  std::variant<int, double, std::string> v = 10;

  // a. Đọc dữ liệu bằng std::get (Bắt lỗi an toàn)
  std::cout << "Gia tri (int): " << std::get<int>(v) << "\n";

  try {
    std::cout << std::get<double>(v); // Cố tình lấy double khi nó đang chứa int
  } catch (const std::bad_variant_access &e) {
    std::cout << "-> Da chan loi: Ban dang lay sai kieu du lieu!\n";
  }

  // b. Kiểm tra kieu hien tai
  v = 3.14; // Chuyển sang chứa double
  if (std::holds_alternative<double>(v)) {
    std::cout << "Hien tai v dang chua Double: " << std::get<double>(v) << "\n";
  }

  // c. PATTERN MATCHING (std::visit) - Đỉnh cao của Variant
  v = "Day la Chuoi C++17";

  std::cout << "Dung std::visit de xu ly tu dong: ";
  // Nó sẽ tự động kiểm tra xem v đang chứa gì và gọi Lambda tương ứng!
  std::visit(
      overloaded{
          [](int arg) { std::cout << "Xu ly so nguyen: " << arg << "\n"; },
          [](double arg) { std::cout << "Xu ly so thuc: " << arg << "\n"; },
          [](const std::string &arg) {
            std::cout << "Xu ly chuoi: " << arg << "\n";
          }},
      v);
}

// ==========================================
// 3. STD::ANY (CHỨA BẤT KỲ THỨ GÌ)
// ==========================================
// Xưa: Dùng `void*` (Tối tăm, ép kiểu sai là Crash chương trình lập tức).
// Nay: Dùng `std::any` (Bọc dữ liệu an toàn, nhớ kiểu gốc, ép sai sẽ ném
// Exception).
void demo_any() {
  std::cout << "\n--- 3. STD::ANY (THAY THE VOID*) ---\n";

  std::any bien_da_nang = 42; // Đang là int

  // a. Ép kiểu an toàn bằng std::any_cast
  std::cout << "Kieu Int: " << std::any_cast<int>(bien_da_nang) << "\n";

  bien_da_nang = std::string("Thay doi thanh chuoi"); // Đổi sang string mượt mà

  // b. Bắt lỗi ép sai kiểu (std::bad_any_cast)
  try {
    std::cout << std::any_cast<float>(bien_da_nang)
              << "\n"; // Cố tình ép sang float
  } catch (const std::bad_any_cast &e) {
    std::cout << "-> Chan loi Ep kieu std::any_cast: " << e.what() << "\n";
  }

  // c. Cách kiểm tra an toàn bằng Con trỏ (Không ném lỗi Exception)
  // Truyền ĐỊA CHỈ (&), nó sẽ trả về NULL nếu sai kiểu. Dùng nhiều trong Game
  // Engine.
  if (std::string *ptr = std::any_cast<std::string>(&bien_da_nang)) {
    std::cout << "Doc thanh cong qua con tro: " << *ptr << "\n";
  }
}

// ==========================================
// 4. STD::BYTE (DỮ LIỆU THÔ NHỊ PHÂN)
// ==========================================
// Xưa: Dùng `char` hoặc `unsigned char` để thao tác byte.
//      Rủi ro: char là KIỂU SỐ, lập trình viên có thể vô tình viết: byte_a +
//      byte_b. Trộn 2 địa chỉ nhị phân bằng phép cộng số học là cực kỳ ngu
//      ngốc!
// Nay: `std::byte`. Chỉ cho phép thao tác Dịch bit (<<, >>) và Logic bit (&, |,
// ^, ~). Cấm CỘNG TRỪ NHÂN CHIA!
void demo_byte() {
  std::cout << "\n--- 4. STD::BYTE (RAW MEMORY) ---\n";

  // Khởi tạo bằng mã Hexa
  std::byte b1{0x1A}; // 0001 1010
  std::byte b2{0x02}; // 0000 0010

  // CẤM: b1 + b2; // LỖI BIÊN DỊCH! std::byte không phải là số.

  // CHO PHÉP: Các phép toán nhị phân
  std::byte ket_qua =
      b1 | b2; // Phép OR: 0001 1010 | 0000 0010 = 0001 1010 (0x1A)

  // Dịch bit
  ket_qua <<= 1; // Dịch trái 1 bit: 0011 0100 (0x34 = 52 hệ thập phân)

  // Để in ra màn hình, BẮT BUỘC phải dịch ngược nó về Số nguyên (integer)
  std::cout << "Gia tri sau khi thao tac bit (Dec): "
            << std::to_integer<int>(ket_qua) << "\n";
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== VOCABULARY TYPES C++17 =====\n";

  demo_optional();
  demo_variant();
  demo_any();
  demo_byte();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}