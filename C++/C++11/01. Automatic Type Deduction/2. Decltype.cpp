#include <iostream>
#include <string>
#include <type_traits> // Chứa std::is_same để kiểm tra kiểu dữ liệu
#include <typeinfo>
#include <vector>

// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. decltype là gì?
    - Ra mắt cùng lúc với auto (C++11).
    - Cú pháp: decltype(thuc_the_hoac_bieu_thuc)
    - Nó YÊU CẦU Trình biên dịch (Compiler) nói cho nó biết CHÍNH XÁC kiểu dữ
 liệu của thực thể đó là gì ngay lúc biên dịch (Compile-time).

 2. Điểm ăn tiền của decltype:
    - Nó KHÔNG chạy đoạn code bên trong ngoặc đơn. Nó chỉ "nhìn" để đoán kiểu.
      (Ví dụ: decltype(ham_tinh_toan_nang_nhat_the_gioi()) sẽ trả ra kiểu dữ
 liệu ngay lập tức mà không hề làm chương trình bị chậm đi hay thực thi hàm đó).
    - Nó giữ LẠI TOÀN BỘ thuộc tính Hằng (const) và Tham chiếu (&).
*/

// ==========================================
// CÁC HÀM PHỤ TRỢ CHO PHẦN NÂNG CAO
// ==========================================
int ham_tra_ve_int() { return 99; }
int &ham_tra_ve_tham_chieu(int &x) { return x; }

class HanhLy {
public:
  double can_nang = 25.5;
};

// ==========================================
// 1. MỨC CƠ BẢN: SỰ KHÁC BIỆT VỚI AUTO
// ==========================================
void decltype_co_ban() {
  std::cout << "\n--- 1. BAZIC: DECLTYPE VS AUTO ---\n";

  const int x = 10;
  const int &ref_x = x;

  // a. AUTO: "Lột đồ" (Bỏ const, bỏ &)
  auto a = ref_x; // a chỉ là 'int'. Không const, không tham chiếu.
  a = 20;         // Đổi 'a' thoải mái, không ảnh hưởng 'x'.

  // b. DECLTYPE: "Giữ nguyên hiện trạng"
  decltype(ref_x) b = x; // b BẮT BUỘC mang kiểu 'const int&' (giống hệt ref_x).

  // b = 20; // LỖI BIÊN DỊCH! Vì b là hằng số tham chiếu (const int&).

  // Chứng minh bằng thư viện type_traits
  if (std::is_same_v<decltype(b), const int &>) {
    std::cout << "Chinh xac! b mang kieu (const int&)\n";
  }
}

// ==========================================
// 2. MỨC TRUNG CẤP: TRAILING RETURN TYPE (C++11)
// ==========================================
// VẤN ĐỀ: Khi viết Hàm Template, nếu ta cộng 2 kiểu dữ liệu (T và U) chưa biết
// trước, thì làm sao biết hàm sẽ trả về kiểu gì (int hay double)? Trình biên
// dịch đọc từ trái sang phải, lúc thấy return type thì nó chưa biết T và U là
// gì!

// GIẢI PHÁP: Dùng auto kết hợp mũi tên -> decltype (Trailing Return Type)
template <typename T, typename U>
auto cong_hai_so(T t, U u) -> decltype(t + u) {
  return t + u;
}

void decltype_trailing_return() {
  std::cout << "\n--- 2. INTERMEDIATE: TEMPLATE & TRAILING RETURN TYPE ---\n";

  // t là int (5), u là double (2.5) -> t + u sẽ là double.
  // decltype(t + u) sẽ tự động đoán ra hàm này phải trả về double!
  auto ket_qua = cong_hai_so(5, 2.5);

  if (std::is_same_v<decltype(ket_qua), double>) {
    std::cout << "Phep cong (int + double) da tu dong tra ve kieu (double): "
              << ket_qua << "\n";
  }
}

// ==========================================
// 3. MỨC NÂNG CAO: LUẬT BIỂU THỨC (MA THUẬT DẤU NGOẶC)
// ==========================================
void decltype_ma_thuat_ngoac() {
  std::cout << "\n--- 3. ADVANCED: MA THUAT DAU NGOAC KEP ---\n";

  int i = 100;

  // LUẬT 1 (Khai báo thực thể): Nếu bên trong là TÊN BIẾN đơn thuần -> Trả về
  // chính xác kiểu của biến đó.
  decltype(i) var1; // Kiểu int. Có thể khai báo mà chưa cần gán giá trị (như
                    // int bình thường).

  // LUẬT 2 (Biểu thức): Nếu bên trong là MỘT BIỂU THỨC sinh ra giá trị bên trái
  // (Lvalue)
  // -> decltype SẼ TRẢ VỀ THÊM DẤU THAM CHIẾU (&).

  // Hành động đặt i vào trong cặp ngoặc đơn (i) biến nó thành một "Biểu thức
  // Lvalue".
  decltype((i)) var2 = i; // Kiểu của var2 bây giờ là 'int&' (Tham chiếu)! Bắt
                          // buộc phải gán giá trị ngay.

  var2 = 999;
  std::cout << "Gia tri cua i sau khi sua var2: " << i
            << " (Da bi thay doi vi var2 la tham chieu)\n";

  // Truy cập thành viên của class
  HanhLy tui;
  decltype(tui.can_nang) var3 = 10;             // Kiểu double
  decltype((tui.can_nang)) var4 = tui.can_nang; // Kiểu double& (Tham chiếu)
}

// ==========================================
// 4. MỨC CHUYÊN GIA: DECLTYPE(AUTO) TRONG C++14
// ==========================================
// Giả sử ta muốn viết một hàm "chuyển tiếp" (wrapper) gọi hàm khác.
// Nếu ta dùng 'auto', nó sẽ làm mất chữ '&' của hàm gốc.
// Nếu ta dùng 'auto&', nó lại lỗi nếu hàm gốc trả về giá trị (không phải tham
// chiếu).

// GIẢI PHÁP TUYỆT ĐỐI (C++14): decltype(auto)
// Lệnh này nói: "Hàm bên trong trả về CÁI GÌ (dù là Tham chiếu, Hằng, hay Giá
// trị), xin hãy bê NGUYÊN XÌ CÁI ĐÓ làm kiểu trả về cho tôi, đừng lột bỏ gì
// cả!"

decltype(auto) wrapper_tra_ve_int() {
  return ham_tra_ve_int(); // Trả về int bình thường
}

decltype(auto) wrapper_tra_ve_tham_chieu(int &val) {
  return ham_tra_ve_tham_chieu(val); // Trả về int& chính xác 100%
}

void decltype_auto_cxx14() {
  std::cout << "\n--- 4. EXPERT: DECLTYPE(AUTO) TRONG C++14 ---\n";

  int diem = 5;
  // Hứng kết quả bằng decltype(auto) để đảm bảo không bị copy nếu là tham chiếu
  decltype(auto) diem_ref = wrapper_tra_ve_tham_chieu(diem);

  diem_ref = 10; // Thay đổi tham chiếu

  std::cout << "Diem sau khi sua qua wrapper: " << diem
            << " (Chuan C++14 Perfect Forwarding)\n";
}

// ==========================================
// 5. ỨNG DỤNG ĐỈNH CAO: KHÔNG ĐÁNH GIÁ BIỂU THỨC (UNEVALUATED CONTEXT)
// ==========================================
void decltype_unevaluated() {
  std::cout << "\n--- 5. MASTER: UNEVALUATED CONTEXT (KHONG CHAY CODE) ---\n";

  int *ptr = nullptr;

  // Cực kỳ nguy hiểm: Dùng con trỏ NULL (*ptr) sẽ gây Crash chương trình lập
  // tức (Segfault). NHƯNG, đặt trong decltype thì hoàn toàn an toàn! Vì
  // decltype chỉ "Nhìn" (*ptr) để biết nó là kiểu int&, chứ KHÔNG BAO GIỜ THỰC
  // THI lệnh đó.
  decltype(*ptr) gia_tri_ao = i; // i là biến khai báo từ xa, sửa tạm cho code
                                 // chạy được (thực tế nên trỏ vào biến hợp lệ).

  // Ứng dụng thực tế:
  // Bạn có thể kiểm tra xem Object đó gọi hàm A() thì sẽ sinh ra kiểu dữ liệu
  // gì, ngay cả khi bạn CHƯA HỀ khởi tạo Object đó trên RAM!
  decltype(std::declval<HanhLy>().can_nang) test_kieu; // Kieu double

  std::cout
      << "Da thoat khoi Segfault thanh cong vi decltype khong he chay code!\n";
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  // Biến toàn cục dùng mượn cho ví dụ phần 5
  int global_i = 0;

  decltype_co_ban();
  decltype_trailing_return();
  decltype_ma_thuat_ngoac();
  decltype_auto_cxx14();
  decltype_unevaluated();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}