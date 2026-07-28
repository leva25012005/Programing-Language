#include <iostream>
#include <string>
#include <vector>

int main() {
  // =========================================================================
  // 💡 ZONE 1: SELECTION STATEMENTS (CÂU LỆNH ĐIỀU KIỆN)
  // =========================================================================
  std::cout << "--- 1. SELECTION STATEMENTS ---" << std::endl;

  int diem = 85;
  bool co_co_gang = true;

  // 1.1 Cấu trúc if-else chuẩn với khối lệnh {}
  if (diem >= 90) {
    std::cout << "🌟 Xep loai: Kieu sa!" << std::endl;
  } else if (diem >= 80) {
    std::cout << "👍 Xep loai: Gioi!" << std::endl;
  } else {
    std::cout << "📚 Can co gang hon." << std::endl;
  }

  // 1.2 MỞ RỘNG: Đánh giá ngắn mạch (Short-circuit evaluation)
  // Vì 'co_co_gang' là true, C++ mới kiểm tra tiếp '(diem > 70)'. Nếu vế đầu
  // false, vế sau bị bỏ qua!
  if (co_co_gang && diem > 70) {
    std::cout << "✨ Ban co tinh dieu kien phu hop de nhan hoc bong."
              << std::endl;
  }

  // 1.3 Toán tử ba ngôi (Conditional Expression) - Thay thế if-else ngắn
  std::string ket_qua = (diem >= 50) ? "Qua mon" : "Hoc lai";
  std::cout << "Trang thai: " << ket_qua << std::endl;

  // 1.4 Switch-case và cạm bẫy "Fall-through"
  int lua_chon = 2;
  switch (lua_chon) {
  case 1:
    std::cout << "Ban chon che do De." << std::endl;
    break; // Ngăn chặn code chảy tuột xuống case dưới
  case 2:
    std::cout << "Ban chon che do Vua." << std::endl;
    break;
  default:
    std::cout << "Lua chon khong hop le." << std::endl;
    break;
  }
  std::cout << "------------------------------------------\n";

  // =========================================================================
  // 🔄 ZONE 2: ITERATION STATEMENTS (VÒNG LẶP CỔ ĐIỂN & DO-WHILE)
  // =========================================================================
  std::cout << "--- 2. ITERATION STATEMENTS (CLASSIC) ---" << std::endl;

  // 2.1 Vòng lặp for cổ điển (Bao gồm lệnh điều khiển break/continue)
  std::cout << "Vong lap for (in so chan, bo qua so 4, dung tai 8): ";
  for (int i = 0; i < 10; i++) {
    if (i == 4)
      continue; // ⏭️ Bỏ qua các lệnh phía dưới, nhảy ngay sang chu kỳ mới
    if (i == 8)
      break; // 🛑 Thoát hẳn khỏi vòng lặp ngay lập tức

    if (i % 2 == 0) {
      std::cout << i << " ";
    }
  }
  std::cout << std::endl;

  // 2.2 Vòng lặp while
  int count = 3;
  while (count > 0) {
    std::cout << "Dem nguoc while: " << count << std::endl;
    count--;
  }

  // 2.3 Vòng lặp do-while (Luôn chạy ít nhất 1 lần dù điều kiện sai)
  int gia_tri = 10;
  do {
    std::cout << "Lenh do-while chay lan dau tien ngay ca khi gia_tri > 5!"
              << std::endl;
  } while (gia_tri < 5);
  std::cout << "------------------------------------------\n";

  // =========================================================================
  // ⚡ ZONE 3: MỞ RỘNG NÂNG CAO - FOR EACH (RANGE-BASED FOR LOOP)
  // =========================================================================
  std::cout << "--- 3. MO RONG: FOR EACH (RANGE-BASED FOR) ---" << std::endl;

  std::vector<int> danh_sach_so = {10, 20, 30, 40};

  // Cách 1: Chỉ đọc giá trị (Tạo bản sao tạm thời)
  std::cout << "Duyet mang chi doc: ";
  for (int n : danh_sach_so) {
    std::cout << n << " ";
  }
  std::cout << std::endl;

  // Cách 2: Sử dụng Tham chiếu hằng (const &) -> Tối ưu hiệu năng, tránh copy
  // dữ liệu lớn
  std::cout << "Duyet const reference: ";
  for (const auto &n : danh_sach_so) {
    std::cout << n << " ";
  }
  std::cout << std::endl;

  // Cách 3: Thay đổi giá trị trực tiếp của các phần tử bằng Tham chiếu (&)
  for (int &n : danh_sach_so) {
    n *= 2; // Nhân đôi từng phần tử trong vector gốc
  }

  std::cout << "Mang sau khi bi thay doi boi for each tham chieu: ";
  for (int n : danh_sach_so) {
    std::cout << n << " ";
  }
  std::cout << "\n------------------------------------------\n";

  return 0;
}