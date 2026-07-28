#include <iostream>
#include <meta> // Thư viện tương lai (C++26) chứa các hàm xử lý std::meta::info
#include <string>
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY - C++26 REFLECTION)
// ==========================================
/*
 1. Reflection là gì?
    - Là khả năng của ngôn ngữ tự "soi gương" và phân tích cấu trúc của chính nó
      (tên Class là gì, có bao nhiêu biến, tên biến là gì, kiểu dữ liệu gì).
    - Các ngôn ngữ như Java, C# dùng "Dynamic Reflection" (chạy lúc Runtime ->
 Làm chậm chương trình).
    - C++26 dùng "Static Reflection" (chạy lúc Compile-time -> HIỆU NĂNG = 0 CHI
 PHÍ).

 2. Hai vũ khí tối thượng:
    - Toán tử `^` (Reflection Operator): Đặt trước một thực thể để lấy "Thông
 tin Meta" (std::meta::info). Ví dụ: constexpr auto meta_class = ^SinhVien;

    - Toán tử `[: :]` (Splicer Operator): Dịch ngược Thông tin Meta trở lại
 thành code C++ thật. Ví dụ: obj.[: meta_bien_ten :] = "Nguyen Van A";

 3. Vòng lặp Compile-time (template for):
    - Dùng để lặp qua danh sách các biến được lấy từ Reflection. Vòng lặp này sẽ
 tự động được Trình biên dịch "trải phẳng" (unroll) ra thành các dòng code tĩnh.
*/

// ==========================================
// CLASS MẪU ĐỂ REFLECT (KHÔNG CẦN BẤT KỲ MACRO NÀO)
// ==========================================
struct NhanVien {
  std::string ho_ten;
  int tuoi;
  double luong;
  bool dang_lam_viec;
};

enum class PhongBan { NhanSu, KeToan, KyThuat, Marketing };

// ==========================================
// 1. MỨC CƠ BẢN: LẤY THÔNG TIN (META INFO) BẰNG `^`
// ==========================================
void reflection_co_ban() {
  std::cout << "\n--- 1. BAZIC: TOAN TU ^ VA THONG TIN META ---\n";

  // Thu thập thông tin của struct NhanVien vào một hằng số Compile-time
  constexpr std::meta::info meta_NV = ^NhanVien;

  // In ra tên của Class (identifier_of)
  std::cout << "Ten Struct: " << std::meta::identifier_of(meta_NV) << "\n";

  // Đếm số lượng biến bên trong Class
  // nonstatic_data_members_of trả về một mảng std::vector<std::meta::info> lúc
  // biên dịch
  constexpr auto danh_sach_bien = std::meta::nonstatic_data_members_of(meta_NV);
  std::cout << "So luong thuoc tinh: " << danh_sach_bien.size() << "\n";

  // Khai báo một biến mới bằng cách "Bung" (Splice) meta info
  typename[:meta_NV:] nhan_vien_moi; // Tuong duong voi: NhanVien nhan_vien_moi;
  nhan_vien_moi.tuoi = 25;
}

// ==========================================
// 2. MỨC TRUNG CẤP: TỰ ĐỘNG SINH HÀM LOG (AUTO-LOGGING)
// ==========================================
// Ngày xưa: Bạn phải tự viết std::cout cho từng biến. Hễ thêm 1 biến vào struct
// là phải sửa hàm in! C++26: Viết 1 hàm duy nhất dùng cho MỌI struct trên đời!

template <typename T> void tu_dong_log_toan_bo_bien(const T &obj) {
  std::cout << "\n--- 2. INTERMEDIATE: AUTO-LOGGER CHUNG CHO MOI CLASS ---\n";

  constexpr auto meta_type = ^T;
  std::cout << "Du lieu cua [" << std::meta::identifier_of(meta_type) << "]:\n";

  // Vòng lặp 'template for' sẽ trải phẳng mảng Meta Info thành code thực tế
  template for (constexpr auto member :
                std::meta::nonstatic_data_members_of(meta_type)) {
    // identifier_of(member): Lấy tên biến dạng chuỗi (ví dụ: "ho_ten", "tuoi")
    // obj.[: member :]: Truy xuất trực tiếp vào giá trị của biến đó (ví dụ:
    // obj.ho_ten)
    std::cout << "  -> " << std::meta::identifier_of(member) << " = "
              << obj.[:member:]
        << "\n";
  }
}

// ==========================================
// 3. MỨC NÂNG CAO: TỰ ĐỘNG CHUYỂN ĐỔI STRUCT <-> JSON
// ==========================================
// Đây chính là tính năng được mong chờ nhất. Không cần dùng thư viện
// nlohmann/json và khai báo MACRO rườm rà. Code tự động bóc tách Struct biến
// thành JSON.

// Hàm Helper để xử lý chuỗi (thêm ngoặc kép) và số (giữ nguyên)
std::string to_json_value(const std::string &val) { return "\"" + val + "\""; }
std::string to_json_value(int val) { return std::to_string(val); }
std::string to_json_value(double val) { return std::to_string(val); }
std::string to_json_value(bool val) { return val ? "true" : "false"; }

template <typename T> std::string struct_to_json(const T &obj) {
  std::string json = "{";
  bool is_first = true;

  template for (constexpr auto member :
                std::meta::nonstatic_data_members_of(^T)) {
    if (!is_first) {
      json += ", ";
    }

    // Sinh key JSON từ tên biến
    json += "\"";
    json += std::string(std::meta::identifier_of(member));
    json += "\": ";

    // Sinh value JSON từ giá trị thực của biến, dùng helper overload ở trên
    json += to_json_value(obj.[:member:]);

    is_first = false;
  }

  json += "}";
  return json;
}

void demo_auto_json() {
  std::cout << "\n--- 3. ADVANCED: ZERO-BOILERPLATE JSON SERIALIZATION ---\n";

  NhanVien nv = {"Le Van Nhan", 30, 2500.50, true};

  std::string json_result = struct_to_json(nv);

  std::cout << "JSON sinh ra tu dong:\n" << json_result << "\n";
  // Output: {"ho_ten": "Le Van Nhan", "tuoi": 30, "luong": 2500.500000,
  // "dang_lam_viec": true}
}

// ==========================================
// 4. MỨC CHUYÊN GIA: BIẾN ENUM THÀNH CHUỖI KHÔNG CẦN LỆNH SWITCH
// ==========================================
// Suốt 20 năm, nếu bạn muốn in Enum ra chữ, bạn phải viết 1 hàm switch-case
// khổng lồ. Với Reflection, điều đó kết thúc.

template <typename EnumType> std::string enum_to_string(EnumType gia_tri_enum) {
  // Duyệt qua mọi thành phần (enumerators) của kiểu Enum
  template for (constexpr auto en_member :
                std::meta::enumerators_of(^EnumType)) {
    // Dùng Splice [: :] để lấy giá trị số của nó và so sánh với giá trị đang
    // xét
    if (gia_tri_enum == [:en_member:]) {
      // Nếu khớp, trả về TÊN CHỮ CỦA NÓ!
      return std::string(std::meta::identifier_of(en_member));
    }
  }
  return "Unknown";
}

void demo_enum_to_string() {
  std::cout << "\n--- 4. EXPERT: ENUM TO STRING MAGIC ---\n";

  PhongBan pb = PhongBan::KyThuat;

  std::cout << "Gia tri phong ban (dang so): " << (int)pb << "\n";
  std::cout << "Gia tri phong ban (dang chu): " << enum_to_string(pb) << "\n";
  // Tự động in ra: "KyThuat"
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== KHOA HOC C++26: STATIC REFLECTION =====\n";
  std::cout << "(Luu y: Chay tren Clang-Reflection-Branch voi co -std=c++26 "
               "-fexperimental-reflection)\n";

  // 1. Co ban
  reflection_co_ban();

  // 2. Auto Logging
  NhanVien nv = {"Christopher", 28, 9999.99, true};
  tu_dong_log_toan_bo_bien(nv);

  // Khai bao 1 Struct bat ky khac, auto-logger van hoat dong ma khong can sua
  // code!
  struct XeHoi {
    std::string hang;
    int nam_sx;
  };
  XeHoi xe = {"Toyota", 2026};
  tu_dong_log_toan_bo_bien(xe);

  // 3. JSON
  demo_auto_json();

  // 4. Enum
  demo_enum_to_string();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}