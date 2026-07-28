#include <algorithm>
#include <forward_list>
#include <iostream>
#include <list>
#include <string>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Single List (Danh sách liên kết đơn - std::forward_list):
    Mỗi phần tử (Node) chỉ lưu Dữ liệu + 1 Con trỏ chỉ đường đến người ĐỨNG SAU
 nó. Nó chỉ có thể đi tiến, không thể lùi. Siêu tiết kiệm RAM.

 2. Doubly List (Danh sách liên kết kép - std::list):
    Mỗi Node lưu Dữ liệu + 1 Con trỏ ĐỨNG TRƯỚC + 1 Con trỏ ĐỨNG SAU.
    Có thể đi tiến và lùi thoải mái. Tốn thêm RAM cho con trỏ lùi.

 3. Circular List (Danh sách liên kết vòng):
    Node cuối cùng thay vì trỏ vào NULL, nó trỏ ngược lại Node đầu tiên tạo
 thành vòng tròn. C++ STL KHÔNG CÓ sẵn Circular List, nhưng bạn có thể tự chế
 bằng cách kiểm tra nếu Iterator == end() thì gán nó về begin().
*/

// ==========================================
// CLASS PHỤ TRỢ CHO PHẦN NÂNG CAO
// ==========================================
class HanhKhach {
public:
  std::string ten;
  int ghe;

  HanhKhach(std::string t, int g) : ten(t), ghe(g) {
    std::cout << "  [Tao moi] Hanh khach: " << ten << "\n";
  }

  // Copy Constructor
  HanhKhach(const HanhKhach &other) : ten(other.ten), ghe(other.ghe) {
    std::cout << "  [Copy] Hanh khach: " << ten << "\n";
  }
};

// ==========================================
// 1. MỨC CƠ BẢN: std::list (DOUBLY LINKED LIST)
// ==========================================
void list_co_ban() {
  std::cout << "\n--- 1. BAZIC: std::list (DOUBLY LIST) ---\n";

  std::list<int> l = {20, 30, 40};

  // Thêm/Xóa ở 2 đầu siêu nhanh O(1)
  l.push_front(10);
  l.push_back(50);
  l.pop_front();

  // NHƯỢC ĐIỂM CHÍ MẠNG: Không có Random Access (Toán tử [])
  // std::cout << l[2]; // LỖI BIÊN DỊCH! Phải đi bộ từ đầu.

  // Cách lấy phần tử thứ 3 (index 2): Phải dùng std::next()
  auto it = std::next(l.begin(), 2);
  std::cout << "Phan tu thu 3 la: " << *it << "\n";

  std::cout << "Duyet std::list: ";
  for (int val : l)
    std::cout << val << " ";
  std::cout << "\n";
}

// ==========================================
// 2. MỨC ĐẶC THÙ: std::forward_list (SINGLY LINKED LIST)
// ==========================================
void forward_list_co_ban() {
  std::cout << "\n--- 2. SPECIAL: std::forward_list (SINGLY LIST) ---\n";

  std::forward_list<int> fl = {2, 3, 4};

  // CHỈ CÓ THỂ THÊM Ở ĐẦU!
  fl.push_front(1);
  // fl.push_back(5); // LỖI BIÊN DỊCH! Trình biên dịch không biết đuôi ở đâu.
  // fl.size();       // LỖI BIÊN DỊCH! Không có hàm đếm size để tiết kiệm tối
  // đa CPU/RAM.

  // MUỐN CHÈN VÀO GIỮA? Phải dùng insert_after (Chèn SAU một vị trí)
  auto it = fl.begin();
  fl.insert_after(it, 99); // Chèn số 99 ngay SAU số 1

  std::cout << "Duyet std::forward_list: ";
  for (int val : fl)
    std::cout << val << " ";
  std::cout << "\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: QUYỀN NĂNG THỰC SỰ CỦA LIST (SPLICE)
// ==========================================
void list_suc_manh_splice() {
  std::cout << "\n--- 3. ADVANCED: QUYEN NANG SPLICE O(1) ---\n";

  // Sự vĩ đại nhất của Linked List là bạn có thể "bẻ gãy" các con trỏ
  // và ghép nguyên một dải dữ liệu từ List này sang List khác trong chớp mắt
  // (O(1)). Vector làm điều này mất O(n) vì phải copy bộ nhớ.

  std::list<int> list1 = {1, 2, 3};
  std::list<int> list2 = {100, 200, 300, 400};

  // Yêu cầu: Cắt toàn bộ list2, nhét vào ngay SAU số 1 của list1.
  auto it_vitri = std::next(list1.begin(), 1); // Trỏ tới số 2

  // Thực hiện cắt ghép thần tốc (Không tốn 1 byte Copy nào)
  list1.splice(it_vitri, list2);

  std::cout << "List 1 sau khi splice: ";
  for (int val : list1)
    std::cout << val << " ";
  std::cout << "\nList 2 hien tai (Da bi rut can): Co " << list2.size()
            << " phan tu.\n";
}

// ==========================================
// 4. MỨC CHUYÊN GIA: CÁC HÀM CỦA RIÊNG LIST & EMPLACE
// ==========================================
void list_chuyen_gia_algorithms() {
  std::cout << "\n--- 4. EXPERT: ALGORITHMS RIENG CUA LIST ---\n";

  std::list<int> l = {50, 10, 30, 20, 10, 10, 40};

  // 1. LỖI KINH ĐIỂN: std::sort(l.begin(), l.end());
  // Std::sort chỉ chạy được trên mảng liên tục (như Vector/Deque).
  // Giải pháp: List có hàm sort() riêng của nó (dùng thuật toán Merge Sort).
  l.sort();
  std::cout << "Sau l.sort(): ";
  for (int val : l)
    std::cout << val << " ";
  std::cout << "\n";

  // 2. XÓA TRÙNG LẶP LIÊN TIẾP (Phải sort trước mới có tác dụng)
  l.unique();
  std::cout << "Sau l.unique() (Xoa cac so 10 thua): ";
  for (int val : l)
    std::cout << val << " ";
  std::cout << "\n";

  // 3. XÓA THEO ĐIỀU KIỆN (Không cần trò Erase-Remove Idiom phức tạp như
  // vector)
  l.remove_if([](int x) { return x >= 40; }); // Xóa hết ai >= 40
  std::cout << "Sau remove_if (>= 40): ";
  for (int val : l)
    std::cout << val << " ";
  std::cout << "\n";

  // 4. KIỂM CHỨNG TỐI ƯU HÓA EMPLACE
  std::cout << "\nKiem chung emplace_back vs push_back:\n";
  std::list<HanhKhach> chuyen_bay;

  std::cout << "  - push_back:\n";
  chuyen_bay.push_back(HanhKhach("Van A", 1)); // Phải Copy object tạm

  std::cout << "  - emplace_back:\n";
  chuyen_bay.emplace_back("Thi B",
                          2); // Khởi tạo trực tiếp vào Node, miễn phí copy!
}

// ==========================================
// TỔNG KẾT LUẬT CHỌN LIST (Kinh nghiệm thực chiến)
// ==========================================
/*
 1. Gần như 95% thời gian đi làm, bạn SẼ KHÔNG DÙNG std::list. Hãy dùng
 std::vector.
 2. Tại sao? Vì tuy list chèn/xóa ở giữa O(1) trên lý thuyết, nhưng để "tìm"
 được chỗ chèn đó, bạn phải lội bộ O(n). Cộng thêm việc các Node nằm rải rác
    khắp RAM khiến CPU Cache bị lủng (Cache Miss), làm list chạy RẤT CHẬM trong
 thực tế.
 3. CHỈ DÙNG LIST KHI:
    - Bạn có nhu cầu dùng hàm splice() để chuyển nhượng dữ liệu khổng lồ giữa
 các danh sách.
    - Dữ liệu của bạn cực kỳ lớn, việc copy bộ nhớ (Reallocation) của Vector là
 quá đắt đỏ.
 4. Dùng std::forward_list khi bạn cần làm thuật toán băm (Hash Table) tự chế
    cần tối giản dung lượng RAM đến mức vắt kiệt từng byte.
*/

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  list_co_ban();
  forward_list_co_ban();
  list_suc_manh_splice();
  list_chuyen_gia_algorithms();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}