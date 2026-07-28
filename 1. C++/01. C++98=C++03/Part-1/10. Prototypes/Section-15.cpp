#include <algorithm> // Cần để dùng std::for_each với Lambda
#include <iostream>
#include <string>
#include <vector>


// ==========================================
// 📝 KHU VỰC KHAI BÁO HÀM (PROTOTYPES)
// ==========================================
void tangGiaTri(int x);
void tangGiaTriThat(int &x);
void inThongTin(const std::string &s);
void chaoHoi(std::string ten = "Khach hang");
auto tinhTong(int a, int b) { return a + b; }
void bieuDien(int n);
void bieuDien(double d);

// 🚀 MỞ RỘNG 3: Hàm Inline (Gợi ý compiler chèn thẳng code vào nơi gọi để tối
// ưu)
inline int nhanDoi(int n) { return n * 2; }

// 🔄 MỞ RỘNG 5: Hàm Đệ Quy (Hàm tự gọi lại chính nó)
// Ví dụ: Tính giai thừa của n (n!)
int giaiThua(int n) {
  if (n <= 1)
    return 1;                 // 🛑 Điều kiện dừng (Base case)
  return n * giaiThua(n - 1); // 🔄 Bước đệ quy
}

// ==========================================
// 🚀 HÀM CHÍNH (MAIN)
// ==========================================
int main() {
  // --- 1. KIỂM TRA TRUYỀN THAM SỐ VÀ NẠP CHỒNG ---
  int so = 10;
  tangGiaTri(so);
  tangGiaTriThat(so);

  std::string van_ban = "Hoc C++ cung Gemini";
  inThongTin(van_ban);

  chaoHoi();
  bieuDien(42);

  // --- 2. SỬ DỤNG HÀM INLINE ---
  std::cout << "--- HONG INLINE ---" << std::endl;
  std::cout << "Nhan doi 5: " << nhanDoi(5) << std::endl;
  std::cout << "------------------------------------\n";

  // --- 3. SỬ DỤNG HÀM ĐỆ QUY ---
  std::cout << "--- HONG DE QUY ---" << std::endl;
  std::cout << "Giai thua cua 5 (5!): " << giaiThua(5)
            << std::endl; // 5 * 4 * 3 * 2 * 1 = 120
  std::cout << "------------------------------------\n";

  // --- 4.⚡ MỞ RỘNG 4: LAMBDA EXPRESSION (HÀM ẨN DANH) ---
  std::cout << "--- HONG LAMBDA (AN DANH) ---" << std::endl;

  // Cú pháp: [capture](parameters) -> return_type { body };
  auto kiemTraChanLe = [](int n) {
    if (n % 2 == 0)
      std::cout << n << " la so chan.\n";
    else
      std::cout << n << " la so le.\n";
  };

  kiemTraChanLe(7);

  // 🛠️ Ứng dụng thực tế: Kết hợp Lambda với các thuật toán của C++
  std::vector<int> nums = {1, 2, 3};
  std::cout << "Bình phuong cac phan tu trong vector: ";
  std::for_each(nums.begin(), nums.end(),
                [](int x) { std::cout << x * x << " "; });
  std::cout << "\n------------------------------------\n";

  return 0;
}

// ==========================================
// 🛠️ KHU VỰC ĐỊNH NGHĨA HÀM
// ==========================================
void tangGiaTri(int x) { x++; }
void tangGiaTriThat(int &x) { x++; }
void inThongTin(const std::string &s) {
  std::cout << "Thong tin: " << s << "\n";
}
void chaoHoi(std::string ten) { std::cout << "Xin chao " << ten << "\n"; }
void bieuDien(int n) { std::cout << "In so nguyen: " << n << "\n"; }
void bieuDien(double d) { std::cout << "In so thuc: " << d << "\n"; }