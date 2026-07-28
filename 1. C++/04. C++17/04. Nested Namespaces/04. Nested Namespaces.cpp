#include <iostream>
#include <string>

// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Namespace (Không gian tên) là gì?
    - Dùng để gom nhóm mã nguồn (hàm, class, biến) nhằm tránh XUNG ĐỘT TÊN (Name
 collision).
    - Ví dụ: Thư viện A có class String, thư viện B cũng có class String. Nhờ
 namespace mà ta phân biệt được A::String và B::String.

 2. Không gian tên lồng nhau (Nested Namespaces):
    - Khi dự án lớn lên, bạn cần phân cấp rõ ràng hơn (VD: Core -> Network ->
 Http).
    - TRƯỚC C++17: Phải viết lùi lề (indent) liên tục, gây ra hội chứng "Mũi tên
 đâm thủng màn hình" rất xấu và khó đọc.
    - TỪ C++17: Cho phép gộp toàn bộ đường dẫn namespace trên MỘT DÒNG duy nhất
 bằng toán tử `::`.
*/

// ==========================================
// 1. MỨC CƠ BẢN: CÁCH VIẾT CŨ (C++98) VÀ MỚI (C++17)
// ==========================================

// --- CÁCH CŨ KÉM HIỆU QUẢ (TRƯỚC C++17) ---
namespace LegacyCore {
namespace Network {
namespace Http {
class Request_Cu {
public:
  void send() { std::cout << "  [Legacy] Gui HTTP Request...\n"; }
};
} // namespace Http
} // namespace Network
} // namespace LegacyCore

// --- CÁCH HIỆN ĐẠI TỐI ƯU (TỪ C++17) ---
// Gộp 3 cấp độ lồng nhau chỉ trên 1 dòng! Giúp code phẳng, không bị thụt lề vô
// nghĩa.
namespace Core::Network::Http {

class Request {
public:
  std::string url;
  Request(std::string u) : url(u) {}
  void send() {
    std::cout << "  [C++17] Gui HTTP Request toi: " << url << "\n";
  }
};

void hien_thi_status() { std::cout << "  [C++17] Tinh trang Http: OK\n"; }
} // namespace Core::Network::Http

// ==========================================
// 2. MỨC TRUNG CẤP: BÍ DANH (ALIAS) VÀ USING
// ==========================================
void namespace_trung_cap() {
  std::cout << "\n--- 2. INTERMEDIATE: ALIAS & USING ---\n";

  // a. Cú pháp gọi đầy đủ (Full Qualification) - Hơi dài dòng
  Core::Network::Http::Request req1("google.com");
  req1.send();

  // b. BÍ DANH (Namespace Alias): Đổi tên cả 1 chuỗi dài thành 1 tên ngắn gọn
  // Cực kỳ hữu ích khi không muốn gõ lại toàn bộ đường dẫn lồng nhau
  namespace NetHttp = Core::Network::Http;

  NetHttp::Request req2("youtube.com");
  req2.send();

  // c. USING DECLARATION: Chỉ "mở khóa" duy nhất 1 hàm/class cần dùng vào phạm
  // vi hiện tại
  using Core::Network::Http::hien_thi_status;
  hien_thi_status(); // Gọi trực tiếp không cần NetHttp::

  // d. USING DIRECTIVE (CẢNH BÁO): Mở khóa toàn bộ. Không nên dùng trong file
  // Header (.h) using namespace Core::Network::Http;
}

// ==========================================
// 3. MỨC NÂNG CAO: KẾT HỢP VỚI KHÔNG GIAN TÊN VÔ DANH
// ==========================================
// Anonymous Namespace (Không gian tên vô danh)
// Tính năng: Bất cứ thứ gì nằm trong này đều bị GIẤU KÍN 100% trong file .cpp
// hiện tại. Các file .cpp khác không thể Extern hoặc gọi đến chúng được (Thay
// thế cho từ khóa 'static' kiểu C).

namespace Core::Network::Http {
namespace {                               // Vô danh lồng bên trong
std::string secret_token = "ABC-XYZ-123"; // Chỉ file hiện tại mới thấy biến này

void ma_hoa_token() {
  std::cout << "  [Internal] Da ma hoa token: " << secret_token << "\n";
}
} // namespace

// Các hàm công khai trong Http có thể xài thoải mái tài nguyên vô danh bên trên
void connect_api() {
  std::cout << "\n--- 3. ADVANCED: ANONYMOUS NAMESPACE ---\n";
  ma_hoa_token();
  std::cout << "  [Public] Ket noi API thanh cong!\n";
}
} // namespace Core::Network::Http

// ==========================================
// 4. MỨC CHUYÊN GIA: INLINE NESTED NAMESPACE (C++20)
// ==========================================
/*
 Tình huống thực tế: Bạn làm thư viện, và ra mắt Phiên bản 2 (V2).
 Bạn muốn code cũ xài V1 không bị lỗi, nhưng người dùng mới chỉ cần gọi
 `Core::Math::cong()` là nó sẽ TỰ ĐỘNG chạy hàm của V2 (phiên bản mới nhất) mà
 không cần gõ `Core::Math::V2::cong()`.

 Tính năng 'inline namespace' làm được điều đó: Nó hợp nhất dữ liệu của
 namespace con vào làm một với namespace cha. C++20 cho phép viết gộp từ khóa
 `inline` trực tiếp trên dòng khai báo Nested Namespace.
*/

namespace Core::Math {
// Không có inline -> Người dùng phải gõ Core::Math::V1::cong()
namespace V1 {
void cong() {
  std::cout << "  [Math] Chay ham cong() cua Phien ban V1 cu ky.\n";
}
} // namespace V1
} // namespace Core::Math

// Cú pháp C++20: Gộp 'inline' ngay trên 1 dòng lồng nhau!
namespace Core::Math::inline V2 {
void cong() {
  std::cout << "  [Math] Chay ham cong() cua Phien ban V2 SIEU TOC DO!\n";
}
void tru() { std::cout << "  [Math] Chay ham tru() cua Phien ban V2.\n"; }
} // namespace Core::Math::inline V2

void namespace_chuyen_gia_cxx20() {
  std::cout << "\n--- 4. EXPERT: INLINE NESTED NAMESPACE (C++20) ---\n";

  // Gọi hàm V1: Bắt buộc phải chỉ định đích danh V1
  Core::Math::V1::cong();

  // XẾP HẠNG CHUYÊN GIA: Gọi hàm cong() KHÔNG CẦN CHỈ ĐỊNH VERSION
  // Trình biên dịch sẽ tự động nhảy vào namespace 'inline V2' để lấy hàm!
  // Đây là chuẩn mực lập trình Thư viện (Versioning) hiện đại nhất.
  Core::Math::cong();
  Core::Math::tru();
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== C++ NESTED NAMESPACE TUTORIAL =====\n";

  // 1. Dùng tính năng cơ bản (C++17)
  Core::Network::Http::Request req("vnexpress.net");
  req.send();

  // 2. Trung cấp
  namespace_trung_cap();

  // 3. Nâng cao (Anonymous)
  Core::Network::Http::connect_api();

  // 4. Chuyên gia (Inline C++20)
  namespace_chuyen_gia_cxx20();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}