#include <iostream>
#include <iterator> // Cho sregex_iterator
#include <regex>    // Thư viện bắt buộc cho Regex
#include <string>

// ==========================================
// LÝ THUYẾT NỀN TẢNG (QUY TẮC REGEX CƠ BẢN)
// ==========================================
/*
 Cú pháp Regex (Giống với hầu hết các ngôn ngữ khác):
    ^     : Bắt đầu chuỗi
    $     : Kết thúc chuỗi
    \d    : Chữ số (0-9)
    \w    : Chữ cái, chữ số và dấu gạch dưới (a-z, A-Z, 0-9, _)
    \s    : Khoảng trắng (space, tab, newline)
    +     : Xuất hiện 1 hoặc nhiều lần
    *     : Xuất hiện 0 hoặc nhiều lần
    ?     : Xuất hiện 0 hoặc 1 lần
    {n,m} : Xuất hiện từ n đến m lần
    [abc] : Chứa 1 trong các ký tự a, b hoặc c
    ( ...): Nhóm (Capture Group) - Dùng để bóc tách dữ liệu con.
*/

// ==========================================
// 1. MỨC CƠ BẢN: MATCH (Khớp TOÀN BỘ chuỗi)
// ==========================================
void regex_co_ban_match() {
  std::cout << "\n--- 1. BAZIC: std::regex_match (KIEM TRA TINH HOP LE) ---\n";

  // YÊU CẦU: Kiểm tra xem một chuỗi có CHÍNH XÁC là địa chỉ Email không?
  // Sử dụng R"(...)" để viết Raw String (Không cần escape \\)
  std::string pattern = R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)";

  // Biên dịch chuỗi thành Động cơ Regex
  // std::regex_constants::icase giúp BỎ QUA HOA/THƯỜNG (A-Z hay a-z đều được)
  std::regex email_regex(pattern, std::regex_constants::icase);

  std::vector<std::string> test_emails = {
      "nhan.le@gmail.com", "Loi@Email_Khong_Hop_Le", "admin@hcmute.edu.vn"};

  for (const auto &email : test_emails) {
    // regex_match BẮT BUỘC toàn bộ chuỗi phải khớp với pattern từ đầu (^) đến
    // cuối ($)
    if (std::regex_match(email, email_regex)) {
      std::cout << "  [OK]  " << email << "\n";
    } else {
      std::cout << "  [ERR] " << email << "\n";
    }
  }
}

// ==========================================
// 2. MỨC TRUNG CẤP: SEARCH (Tìm một phần & Nhóm dữ liệu)
// ==========================================
void regex_trung_cap_search() {
  std::cout
      << "\n--- 2. INTERMEDIATE: std::regex_search & CAPTURE GROUPS ---\n";

  std::string noi_dung = "Toi sinh ngay 15/08/1999 tai TP.HCM.";

  // YÊU CẦU: Tìm ngày tháng năm sinh.
  // Dùng dấu ngoặc đơn () để tạo ra các "Nhóm" (Capture Group) muốn bóc tách.
  std::regex date_regex(R"((\d{2})/(\d{2})/(\d{4}))");

  // std::smatch (String Match) là một mảng đặc biệt chứa kết quả bóc tách
  std::smatch ket_qua;

  // regex_search sẽ tìm mẫu chuỗi nằm ẨN BÊN TRONG đoạn văn (không cần khớp
  // toàn bộ) Lưu ý: Nó chỉ tìm thấy kết quả ĐẦU TIÊN!
  if (std::regex_search(noi_dung, ket_qua, date_regex)) {
    std::cout << "Da tim thay chuoi thoa man: " << ket_qua.str() << "\n";

    // ket_qua[0] luôn là TOÀN BỘ chuỗi khớp (15/08/1999)
    // ket_qua[1], [2], [3] tương ứng với các cặp ngoặc đơn () trong Regex
    std::cout << "  -> Ngay: " << ket_qua[1] << "\n";
    std::cout << "  -> Thang: " << ket_qua[2] << "\n";
    std::cout << "  -> Nam: " << ket_qua[3] << "\n";
  }
}

// ==========================================
// 3. MỨC NÂNG CAO: TÌM KIẾM TOÀN BỘ (Iterator)
// ==========================================
void regex_nang_cao_iterator() {
  std::cout << "\n--- 3. ADVANCED: std::sregex_iterator (TIM TAT CA) ---\n";

  std::string doan_van =
      "Lien he anh A qua so 090-123-4567 hoac chi B so 098-765-4321 ngay nhe.";

  // YÊU CẦU: Lấy ra TẤT CẢ các số điện thoại có định dạng xxx-xxx-xxxx
  std::regex phone_regex(R"(\d{3}-\d{3}-\d{4})");

  // Khởi tạo Iterator trỏ vào điểm đầu của chuỗi kết quả
  auto words_begin =
      std::sregex_iterator(doan_van.begin(), doan_van.end(), phone_regex);
  // Iterator rỗng (Trỏ vào điểm kết thúc)
  auto words_end = std::sregex_iterator();

  std::cout << "Cac so dien thoai tim duoc:\n";

  // Duyệt qua tất cả các kết quả giống như duyệt Vector
  for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
    std::smatch match = *i; // Lấy kết quả hiện tại
    std::cout << "  - " << match.str()
              << " (Nam o vi tri index: " << match.position() << ")\n";
  }
}

// ==========================================
// 4. MỨC CHUYÊN GIA: REPLACE (Thay thế mạnh mẽ)
// ==========================================
void regex_chuyen_gia_replace() {
  std::cout << "\n--- 4. EXPERT: std::regex_replace (MA THUAT THAY THE) ---\n";

  std::string du_lieu_tho =
      "Tran dau dien ra ngay 25/12/2026. Ve ban vao 01/01/2027.";

  std::regex date_regex(R"((\d{2})/(\d{2})/(\d{4}))");

  // YÊU CẦU 1: Che mờ toàn bộ ngày tháng (Censorship)
  // Cứ chỗ nào khớp Regex thì thay bằng "[DA CHE]"
  std::string che_mo = std::regex_replace(du_lieu_tho, date_regex, "[DA CHE]");
  std::cout << "1. Che mờ: " << che_mo << "\n";

  // YÊU CẦU 2: Đổi định dạng từ DD/MM/YYYY sang định dạng Quốc tế YYYY-MM-DD
  // MA THUẬT: Sử dụng ký hiệu $1, $2, $3 để chỉ định các Capture Group ()
  // $3 = Năm, $2 = Tháng, $1 = Ngày
  std::string chuan_hoa =
      std::regex_replace(du_lieu_tho, date_regex, "$3-$2-$1");
  std::cout << "2. Chuan hoa Format: " << chuan_hoa << "\n";
}

// ==========================================
// 5. HIỆN ĐẠI TỐI THƯỢNG: TOKENIZER (Tách chuỗi)
// ==========================================
void regex_tokenizer_split() {
  std::cout
      << "\n--- 5. MASTER: std::sregex_token_iterator (SPLIT CHUOI) ---\n";

  // Trong C++, string không có sẵn hàm .split() như Java hay Python.
  // Cách mạnh mẽ nhất để Split là dùng Regex Token Iterator.

  std::string data = "Apple, Banana; Orange | Mango    Grapes";

  // YÊU CẦU: Tách chuỗi bằng các dấu phẩy, chấm phẩy, gạch đứng, hoặc KHOẢNG
  // TRẮNG.
  std::regex delimiter(R"([,;|]|\s+)"); // Pattern đại diện cho "Vách ngăn"

  // THAM SỐ CUỐI CÙNG LÀ -1: Nghĩa là "Đừng lấy vách ngăn, hãy lấy phần CHỮ nằm
  // GIỮA các vách ngăn"
  auto token_begin =
      std::sregex_token_iterator(data.begin(), data.end(), delimiter, -1);
  auto token_end = std::sregex_token_iterator();

  std::cout << "Cac trai cay sau khi Split:\n";
  for (auto i = token_begin; i != token_end; ++i) {
    // Kiểm tra tránh lấy chuỗi rỗng nếu có nhiều vách ngăn dính nhau
    if (!i->str().empty()) {
      std::cout << "  -> " << *i << "\n";
    }
  }
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== KHOA HOC C++ REGEX (BIEU THUC CHINH QUY) =====\n";

  regex_co_ban_match();
  regex_trung_cap_search();
  regex_nang_cao_iterator();
  regex_chuyen_gia_replace();
  regex_tokenizer_split();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}