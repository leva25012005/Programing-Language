#include <algorithm>
#include <compare> // BẮT BUỘC: Thư viện cốt lõi cho Toán tử Tàu vũ trụ
#include <iostream>
#include <string>
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. TẠI SAO LẠI GỌI LÀ TÀU VŨ TRỤ?
    - Vì hình dáng của nó `<=>` trông giống hệt một chiếc đĩa bay (UFO).

 2. SỨ MỆNH TRONG C++20:
    - Trước C++20: Để class của bạn có thể so sánh đầy đủ và dùng được
 std::sort, bạn phải vã mồ hôi viết tới 6 toán tử: ==, !=, <, <=, >, >=.
    - Từ C++20: Chỉ cần VIẾT ĐÚNG 1 DÒNG `<=>`, Trình biên dịch sẽ "đẻ" ra
 (generate) toàn bộ 6 toán tử kia dưới nền.

 3. NÓ TRẢ VỀ CÁI GÌ?
    - Nó KHÔNG trả về bool (true/false) như toán tử `<` hay `>`.
    - Nó trả về một "Đối tượng Xếp hạng" (Ordering Category).
    - Cú pháp kiểm tra: (A <=> B) < 0 (Nghĩa là A nhỏ hơn B).
*/

// ==========================================
// 1. MỨC CƠ BẢN: PHÉP MÀU '= DEFAULT'
// ==========================================
struct SinhVien {
  std::string ten;
  int diem_pte;
  int tuoi;

  // PHÉP MÀU LÀ ĐÂY: Chỉ 1 dòng duy nhất!
  // Trình biên dịch sẽ tự động so sánh lần lượt từ trên xuống dưới:
  // So sánh 'ten' trước, nếu bằng nhau thì so 'diem_pte', nếu vẫn bằng thì so
  // 'tuoi'.
  auto operator<=>(const SinhVien &) const = default;
};

void spaceship_co_ban() {
  std::cout << "\n--- 1. BAZIC: AUTO GENERATE ---\n";

  SinhVien sv1 = {"Lê Văn Nhân", 75, 20};
  SinhVien sv2 = {"Lê Văn Nhân", 80, 20};

  // Mặc dù ta không hề viết toán tử < hay ==, code vẫn chạy mượt mà!
  if (sv1 < sv2) {
    std::cout << sv1.ten << " (PTE " << sv1.diem_pte << ") xep sau.\n";
  }
  if (sv1 != sv2) {
    std::cout << "Hai sinh vien nay khac nhau.\n";
  }

  // Kết hợp với thư viện Algorithm dễ dàng
  std::vector<SinhVien> danh_sach = {sv2, sv1, {"An", 90, 19}};
  std::sort(danh_sach.begin(),
            danh_sach.end()); // Tự động xép tăng dần theo Tên -> Điểm -> Tuổi
}

// ==========================================
// 2. MỨC TRUNG CẤP: 3 LOẠI KẾT QUẢ TRẢ VỀ CỦA `<=>`
// ==========================================
void ba_loai_ket_qua() {
  std::cout << "\n--- 2. INTERMEDIATE: RETURN TYPES ---\n";

  // a. std::strong_ordering (Xếp hạng Tuyệt đối)
  // Dùng cho số nguyên (int). Hai số bằng nhau thì MỌI MẶT phải giống hệt nhau.
  int a = 10, b = 20;
  std::strong_ordering cmp1 = (a <=> b);
  if (cmp1 == std::strong_ordering::less)
    std::cout << "a nho hon b\n";
  if (cmp1 == std::strong_ordering::equal)
    std::cout << "a giong het b\n";
  if (cmp1 == std::strong_ordering::greater)
    std::cout << "a lon hon b\n";

  // b. std::partial_ordering (Xếp hạng Tương đối / Không hoàn chỉnh)
  // Dùng cho số thực (float/double). Vì float có giá trị dị biệt là NaN (Not a
  // Number). NaN không lớn hơn, không nhỏ hơn, và cũng không bằng bất kỳ số
  // nào!
  float f1 = 3.14f;
  float f2 = __builtin_nanf(""); // Tạo ra lỗi NaN (Hoặc có thể lấy 0.0 / 0.0)

  std::partial_ordering cmp2 = (f1 <=> f2);
  if (cmp2 == std::partial_ordering::unordered) {
    std::cout << "Float: Khong the so sanh duoc (Unordered do chua NaN)!\n";
  }

  // c. std::weak_ordering (Xếp hạng Yếu)
  // Dùng cho các trường hợp hai đối tượng được coi là "tương đương" nhưng cấu
  // trúc bit không giống nhau. Ví dụ: So sánh chuỗi không phân biệt hoa thường
  // ("HELLO" được coi là tương đương "hello").
}

// ==========================================
// 3. MỨC NÂNG CAO: TÙY CHỈNH LUẬT SẮP XẾP (CUSTOM `<=>`)
// ==========================================
// Nếu `= default` so sánh lần lượt từ trên xuống, nhưng bạn lại muốn
// ưu tiên so sánh 'tuoi' trước rồi mới tới 'ten' thì sao?
struct NhanVienIT {
  std::string ten;
  int tuoi;

  // Tự viết hàm Tàu vũ trụ:
  std::strong_ordering operator<=>(const NhanVienIT &other) const {
    // 1. So sánh Tuổi trước
    if (auto cmp = tuoi <=> other.tuoi; cmp != 0) {
      return cmp; // Nếu tuổi khác nhau (lớn hơn hoặc nhỏ hơn), trả về kết quả
                  // luôn
    }
    // 2. Nếu tuổi BẰNG NHAU (cmp == 0), tiến hành so sánh Tên
    return ten <=> other.ten;
  }

  // QUAN TRỌNG: Khi bạn tự viết `<=>`, C++ sẽ KHÔNG tự đẻ ra toán tử `==` nữa.
  // Bạn phải yêu cầu nó tự đẻ ra `==` bằng lệnh default:
  bool operator==(const NhanVienIT &) const = default;
};

void spaceship_tuy_chinh() {
  std::cout << "\n--- 3. ADVANCED: CUSTOM SPACESHIP LOGIC ---\n";

  NhanVienIT nv1 = {"Binh", 25};
  NhanVienIT nv2 = {"An", 22};

  // Tuy Binh có chữ B đứng sau chữ A, nhưng Tuổi (25) > Tuổi (22)
  // Do ta đã lập trình ưu tiên Tuổi, nên Binh > An.
  if (nv1 > nv2) {
    std::cout << "Binh lon hon An (Vi uu tien so Tuoi truoc).\n";
  }
}

// ==========================================
// 4. MỨC CHUYÊN GIA: TỐI ƯU HÓA HIỆU NĂNG BẰNG TAY
// ==========================================
struct MangDuLieuLon {
  std::vector<int> du_lieu;

  // Giả sử ta cho default tàu vũ trụ
  auto operator<=>(const MangDuLieuLon &) const = default;

  /*
   BÍ MẬT HIỆU NĂNG:
   Nếu 2 mảng cực lớn hoàn toàn giống nhau, toán tử == được generate từ `<=>`
   sẽ phải dò TỪNG PHẦN TỬ MỘT để biết chúng bằng nhau (O(n)).

   Để tối ưu, Lập trình viên C++ kinh nghiệm sẽ tự viết lại `==`.
   Nếu kích thước (size) của 2 mảng khác nhau -> Kết luận luôn là KHÁC NHAU
   (O(1)). Chỉ khi kích thước bằng nhau, mới dùng `<=>` mặc định để dò tiếp.
  */
  bool operator==(const MangDuLieuLon &other) const {
    if (du_lieu.size() != other.du_lieu.size()) {
      return false; // Tốc độ O(1) thần thánh!
    }
    // Nếu size bằng nhau, phó thác việc dò từng phần tử cho C++ xử lý
    return (*this <=> other) == 0;
  }
};

void spaceship_hieu_nang() {
  std::cout << "\n--- 4. EXPERT: PERFORMANCE OPTIMIZATION ---\n";

  MangDuLieuLon m1 = {{1, 2, 3}};
  MangDuLieuLon m2 = {{1, 2, 3, 4, 5}};

  // Chạy cực kỳ nhanh vì hàm == tự chế phát hiện lệch size và chặn luôn
  if (m1 != m2) {
    std::cout << "Hai mang khac nhau. (Kiem tra size O(1) hoat dong)\n";
  }
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== C++20: SPACESHIP OPERATOR (<=>) =====\n";

  spaceship_co_ban();
  ba_loai_ket_qua();
  spaceship_tuy_chinh();
  spaceship_hieu_nang();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}