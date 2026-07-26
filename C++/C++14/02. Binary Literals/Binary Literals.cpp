#include <bitset> // Hỗ trợ in dữ liệu ra màn hình dưới dạng nhị phân
#include <cstdint> // Chứa các kiểu dữ liệu uint8_t, uint16_t (rất quan trọng khi làm việc với bit)
#include <iostream>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Binary Literals là gì?
    - Ra mắt từ C++14, cho phép bạn gán trực tiếp giá trị nhị phân (0 và 1) cho
 các biến số nguyên.
    - Cú pháp: Bắt đầu bằng tiền tố `0b` hoặc `0B`.

 2. Tại sao phải dùng?
    - Trước C++14, để làm việc với Bit/Thanh ghi (Registers) hoặc Cờ (Flags),
      lập trình viên phải dùng hệ Hexa (Thập lục phân - 0x...). Rất khó nhẩm
 bằng mắt thường.
    - Binary Literals giúp hiển thị trực quan phần cứng / bitmask ngay trên
 code.

 3. Tính năng đi kèm: Digit Separator (Dấu phẩy động)
    - C++14 cho phép dùng dấu nháy đơn `'` để ngắt các dãy số dài cho dễ đọc
      (Ví dụ: 0b1010'1111). Trình biên dịch sẽ tự động phớt lờ dấu này.
*/

// ==========================================
// 1. MỨC CƠ BẢN: KHỞI TẠO VÀ SO SÁNH CÁC HỆ CƠ SỐ
// ==========================================
void binary_co_ban() {
  std::cout << "\n--- 1. BAZIC: KHOI TAO & SO SANH ---\n";

  // Khởi tạo biến bằng 4 hệ cơ số khác nhau nhưng cùng mang 1 giá trị là 15
  uint8_t he_thap_phan = 15;    // Hệ 10 (Decimal)
  uint8_t he_bat_phan = 017;    // Hệ 8 (Octal) - Tiền tố 0
  uint8_t he_hexa = 0x0F;       // Hệ 16 (Hexadecimal) - Tiền tố 0x
  uint8_t he_nhi_phan = 0b1111; // Hệ 2 (Binary) - Tiền tố 0b (C++14)

  std::cout << "So sanh cac he co so: \n";
  if (he_nhi_phan == he_thap_phan && he_nhi_phan == he_hexa) {
    std::cout << "-> Tat ca deu bang 15 trong RAM!\n";
  }

  // ỨNG DỤNG C++14 DIGIT SEPARATOR (Dấu phân cách)
  // Tưởng tượng bạn phải đọc một dải bit của thanh ghi 16-bit:
  uint16_t kho_doc = 0b1010111100001100;

  // Thay vào đó, dùng dấu phẩy đơn (') để chia thành từng cụm 4 bit (Nibble)
  uint16_t de_doc = 0b1010'1111'0000'1100; // Hoàn toàn giống hệt dòng trên

  // LƯU Ý: std::cout mặc định LUÔN LUÔN in số nguyên dưới dạng hệ thập phân (Hệ
  // 10)
  std::cout << "Gia tri thap phan cua de_doc la: " << de_doc << "\n";
}

// ==========================================
// 2. MỨC TRUNG CẤP: IN NHỊ PHÂN RA MÀN HÌNH
// ==========================================
void binary_in_ra_man_hinh() {
  std::cout << "\n--- 2. INTERMEDIATE: HIEN THI NHI PHAN ---\n";

  uint8_t num = 0b0101'1010; // 90 hệ thập phân

  // Khác với hex (std::hex) hay octal (std::oct), std::cout KHÔNG CÓ std::bin!
  // Để in dạng nhị phân, ta phải ép kiểu nó qua std::bitset<So_Bit>

  std::cout << "In kieu thong thuong (He 10): " << (int)num
            << "\n"; // Ép int vì uint8_t có thể bị hiểu là ký tự char
  std::cout << "In kieu nhi phan (Dung bitset): " << std::bitset<8>(num)
            << "\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: THAO TÁC BIT (BITWISE OPERATIONS)
// ==========================================
void binary_thao_tac_bit() {
  std::cout << "\n--- 3. ADVANCED: BITWISE OPERATIONS ---\n";

  uint8_t a = 0b1100; // 12
  uint8_t b = 0b1010; // 10

  std::cout << "  a       = " << std::bitset<4>(a) << "\n";
  std::cout << "  b       = " << std::bitset<4>(b) << "\n";

  // 1. AND (&): Trả về 1 nếu CẢ HAI bit đều là 1
  std::cout << "  a & b   = " << std::bitset<4>(a & b) << "\n"; // 0b1000

  // 2. OR (|): Trả về 1 nếu MỘT TRONG HAI bit là 1
  std::cout << "  a | b   = " << std::bitset<4>(a | b) << "\n"; // 0b1110

  // 3. XOR (^): Trả về 1 nếu HAI BIT KHÁC NHAU
  std::cout << "  a ^ b   = " << std::bitset<4>(a ^ b) << "\n"; // 0b0110

  // 4. NOT (~): Đảo ngược toàn bộ bit (0 thành 1, 1 thành 0)
  std::cout << " ~a       = " << std::bitset<4>(~a) << "\n"; // 0b0011

  // 5. SHIFT LEFT (<<): Dịch trái (Nhân 2)
  std::cout << "  a << 1  = " << std::bitset<4>(a << 1)
            << "\n"; // 0b1000 (Bit 1 ngoài cùng bị đẩy mất)

  // 6. SHIFT RIGHT (>>): Dịch phải (Chia 2)
  std::cout << "  a >> 1  = " << std::bitset<4>(a >> 1) << "\n"; // 0b0110
}

// ==========================================
// 4. MỨC CHUYÊN GIA: BITMASK (QUẢN LÝ TRẠNG THÁI)
// ==========================================
// Trong Game hay Hệ điều hành, để tiết kiệm RAM, người ta gộp 8 trạng thái
// (True/False) vào chung MỘT biến số nguyên 8-bit (uint8_t) thay vì dùng 8 biến
// bool. Khởi tạo các "Mặt nạ" (Mask) bằng Binary Literals giúp code cực kỳ dễ
// hiểu.

const uint8_t DANG_BAY = 0b0000'0001;     // Bit số 0
const uint8_t TANG_HINH = 0b0000'0010;    // Bit số 1
const uint8_t BUI_MA_THUAT = 0b0000'0100; // Bit số 2
const uint8_t BI_TRUNG_DOC = 0b0000'1000; // Bit số 3

void binary_bitmask_expert() {
  std::cout << "\n--- 4. EXPERT: BITMASK & FLAGS ---\n";

  // Ban đầu nhân vật không có trạng thái nào (Tất cả bit = 0)
  uint8_t nhan_vat = 0b0000'0000;

  std::cout << "Trang thai hien tai: " << std::bitset<8>(nhan_vat) << "\n";

  // 1. SET BIT (Bật trạng thái): Dùng toán tử OR (|)
  nhan_vat = nhan_vat | DANG_BAY | BUI_MA_THUAT;
  std::cout << "Sau khi buff Bay va Ma Thuat: " << std::bitset<8>(nhan_vat)
            << "\n";

  // 2. CHECK BIT (Kiểm tra trạng thái): Dùng toán tử AND (&)
  if (nhan_vat & DANG_BAY) {
    std::cout << " -> Nhan vat DANG BAY tren khong!\n";
  }
  if (!(nhan_vat & BI_TRUNG_DOC)) {
    std::cout << " -> Nhan vat KHONG BI doc.\n";
  }

  // 3. CLEAR BIT (Tắt trạng thái): Dùng AND (&) kết hợp NOT (~)
  // ~DANG_BAY sẽ biến 0b0000'0001 thành 0b1111'1110.
  // AND với dãy này sẽ giữ nguyên mọi bit, trừ bit cuối cùng bị ép về 0.
  nhan_vat = nhan_vat & (~DANG_BAY);
  std::cout << "Sau khi bi huy buff Bay: " << std::bitset<8>(nhan_vat) << "\n";

  // 4. TOGGLE BIT (Đảo trạng thái On<->Off): Dùng XOR (^)
  // Nhấn nút tàng hình: Bấm 1 lần thì tàng hình, bấm phát nữa thì hiện nguyên
  // hình
  nhan_vat = nhan_vat ^ TANG_HINH;
  std::cout << "Bam nut Tang hinh (Lan 1): " << std::bitset<8>(nhan_vat)
            << "\n";

  nhan_vat = nhan_vat ^ TANG_HINH;
  std::cout << "Bam nut Tang hinh (Lan 2): " << std::bitset<8>(nhan_vat)
            << "\n";
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  binary_co_ban();
  binary_in_ra_man_hinh();
  binary_thao_tac_bit();
  binary_bitmask_expert();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}