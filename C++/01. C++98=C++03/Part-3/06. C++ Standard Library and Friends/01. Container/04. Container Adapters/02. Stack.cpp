#include <iostream>
#include <list>
#include <stack>
#include <string>
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Cấu trúc dữ liệu: LIFO (Last-In, First-Out) - Vào sau Ra trước.
    Hãy tưởng tượng nó như một chồng đĩa: Bạn chỉ có thể đặt đĩa mới lên ĐỈNH,
    và chỉ có thể lấy đĩa ra từ ĐỈNH. Không thể rút đĩa ở giữa hay ở đáy.

 2. Bản chất (Container Adaptor): std::stack KHÔNG phải là một cấu trúc dữ liệu
 độc lập. Nó là một "Kẻ mạo danh" (Adaptor). Dưới nền, nó đeo mặt nạ cho một
 Container khác (Mặc định là std::deque) và ép Container đó phải tuân thủ luật
 LIFO (chỉ cho phép truy cập 1 đầu).

 3. Đặc tính cốt lõi:
    - KHÔNG CÓ ITERATOR: Bạn không thể dùng vòng lặp for để duyệt qua Stack!
    - Tốc độ: Thêm (push) và Lấy (pop) luôn là O(1).
*/

// ==========================================
// CLASS PHỤ TRỢ CHO PHẦN CHUYÊN GIA
// ==========================================
class HoSo {
public:
  std::string ten_ho_so;
  int ma_so;

  HoSo(std::string t, int m) : ten_ho_so(t), ma_so(m) {
    std::cout << "  [Tao moi] Ho so: " << ten_ho_so << "\n";
  }
  // Copy Constructor
  HoSo(const HoSo &other) : ten_ho_so(other.ten_ho_so), ma_so(other.ma_so) {
    std::cout << "  [Copy] Ho so: " << ten_ho_so << "\n";
  }
};

// Hàm phụ trợ để IN STACK (Xem Cạm bẫy ở Mức 1)
// Chú ý: Nhận tham số theo dạng COPY (Truyền tham trị), không dùng Tham chiếu
// (&)
void in_stack(std::stack<int> s_copy) {
  std::cout << "Đinh -> ";
  while (!s_copy.empty()) {
    std::cout << s_copy.top() << " "; // Đọc đỉnh
    s_copy.pop(); // Xóa đỉnh để đọc phần tử tiếp theo
  }
  std::cout << "<- Day\n";
}

// ==========================================
// 1. MỨC CƠ BẢN: CÁC THAO TÁC CỐT LÕI VÀ CẠM BẪY DUYỆT
// ==========================================
void stack_co_ban() {
  std::cout << "\n--- 1. BAZIC: PUSH, POP, TOP ---\n";

  std::stack<int> s;

  // 1. push(): Nhét dữ liệu vào ĐỈNH
  s.push(10);
  s.push(20);
  s.push(30);

  // 2. size(): Kiểm tra số lượng
  std::cout << "Kich thuoc stack: " << s.size() << "\n";

  // 3. CẠM BẪY KINH ĐIỂN: LÀM SAO ĐỂ IN STACK?
  // Vì Stack KHÔNG CÓ Iterator (không có begin(), end()), bạn KHÔNG THỂ dùng
  // vòng lặp for! Cách duy nhất để xem toàn bộ dữ liệu là: Đọc đỉnh -> Xóa đỉnh
  // -> Đọc đỉnh tiếp theo. NHƯNG nếu làm vậy trên Stack gốc, bạn sẽ XÓA SẠCH dữ
  // liệu của mình! GIẢI PHÁP: Truyền bản COPY của stack vào một hàm khác để in
  // (như hàm in_stack ở trên).
  std::cout << "Noi dung stack hien tai: ";
  in_stack(s);

  // 4. top() và pop()
  std::cout << "Phan tu tren dinh (top): " << s.top() << "\n"; // Đọc số 30

  std::cout << "Thuc hien pop() (Rut dinh ra)...\n";
  s.pop(); // Rút số 30 ném đi

  std::cout << "Phan tu tren dinh moi: " << s.top() << "\n"; // Bây giờ là 20
}

// ==========================================
// 2. MỨC TRUNG CẤP: THAY ĐỔI CONTAINER DƯỚI NỀN (UNDERLYING CONTAINER)
// ==========================================
void stack_doi_container() {
  std::cout << "\n--- 2. INTERMEDIATE: UNDERLYING CONTAINERS ---\n";

  // Mặc định std::stack<int> tương đương với std::stack<int, std::deque<int>>.
  // Nhược điểm của deque là cấp phát nhiều khối RAM nhỏ (chunks).

  // Bạn có thể ÉP Stack dùng std::vector làm cốt lõi để tận dụng
  // dải RAM liên tục (Cache-friendly) siêu nhanh:
  std::stack<int, std::vector<int>> stack_vector;
  stack_vector.push(100);
  stack_vector.push(200);

  // Thậm chí ép nó dùng std::list (Dù thực tế ít ai làm vì tốn RAM cho con
  // trỏ):
  std::stack<int, std::list<int>> stack_list;
  stack_list.push(500);

  std::cout << "Stack dung Vector - Dinh la: " << stack_vector.top() << "\n";
  std::cout << "Stack dung List   - Dinh la: " << stack_list.top() << "\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: TẠI SAO POP() LẠI KHÔNG TRẢ VỀ GIÁ TRỊ?
// ==========================================
void stack_tai_sao_pop_tra_ve_void() {
  std::cout << "\n--- 3. ADVANCED: EXCEPTION SAFETY TRONG C++ ---\n";

  std::stack<std::string> s;
  s.push("Tai lieu Mat");

  /*
  Nhiều người từ Java/C# sang C++ sẽ thắc mắc:
  Tại sao không thiết kế hàm pop() vừa XÓA vừa TRẢ VỀ giá trị luôn cho tiện?
  (Ví dụ: string s = s.pop();)

  TRẢ LỜI: VÌ LÝ DO AN TOÀN NGOẠI LỆ (EXCEPTION GUARANTEE).
  Nếu pop() trả về giá trị, nó phải COPY giá trị đó đưa cho bạn, sau đó XÓA phần
  tử trong Stack. Nhưng rủi ro là: Chuyện gì xảy ra nếu bộ nhớ RAM bị đầy đúng
  lúc đang COPY?
  - Quá trình COPY sẽ thất bại (ném ra ngoại lệ std::bad_alloc).
  - Nhưng lúc này phần tử trong Stack LẠI BỊ XÓA MẤT RỒI!
  -> KẾT QUẢ: Bạn bị MẤT TRẮNG dữ liệu mãi mãi.

  Vì vậy, C++ tách hành động này làm 2 bước riêng biệt:
  1. s.top(): Trả về tham chiếu đến đỉnh (Không copy, không bao giờ lỗi). Bạn tự
  lưu lại.
  2. s.pop(): Chỉ làm nhiệm vụ xóa (Cực nhanh và không bao giờ ném ngoại lệ).
  Nếu bước 1 lỗi, dữ liệu vẫn còn an toàn trong Stack!
  */

  // Quy trình chuẩn an toàn 100%:
  std::string gia_tri_lay_ra = s.top(); // Bước 1: Lưu lại an toàn
  s.pop();                              // Bước 2: Xóa an toàn

  std::cout << "Lay thanh cong: " << gia_tri_lay_ra << "\n";
}

// ==========================================
// 4. MỨC CHUYÊN GIA: EMPLACE() VÀ TỐI ƯU HÓA OBJECT
// ==========================================
void stack_chuyen_gia_emplace() {
  std::cout << "\n--- 4. EXPERT: EMPLACE VS PUSH ---\n";

  std::stack<HoSo> chong_ho_so;

  std::cout << "> Dung push():\n";
  // Phải tạo một Object tạm thời HoSo, sau đó COPY/MOVE nó vào đỉnh Stack.
  chong_ho_so.push(HoSo("Bao cao Q1", 101));

  std::cout << "\n> Dung emplace():\n";
  // Khởi tạo trực tiếp Object NGAY TẠI ĐỈNH của Stack. KHÔNG có quá trình Copy
  // nào xảy ra! Tiết kiệm bộ nhớ và CPU tuyệt đối.
  chong_ho_so.emplace("Bao cao Q2", 102);
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  stack_co_ban();
  stack_doi_container();
  stack_tai_sao_pop_tra_ve_void();
  stack_chuyen_gia_emplace();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}