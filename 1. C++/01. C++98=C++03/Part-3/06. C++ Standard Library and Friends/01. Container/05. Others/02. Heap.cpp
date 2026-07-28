#include <algorithm> // Bắt buộc: Chứa make_heap, push_heap, pop_heap, sort_heap
#include <functional> // Chứa std::greater
#include <iostream>
#include <string>
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Heap (Đống) là gì?
    - Là một Cây nhị phân hoàn chỉnh được trải phẳng ra và lưu trên một Mảng
 (Vector).
    - Max-Heap (Mặc định): Giá trị lớn nhất luôn nằm ở node gốc (vị trí index
 0).
    - Min-Heap: Giá trị nhỏ nhất luôn nằm ở gốc.

 2. Tại sao không dùng std::priority_queue?
    - std::priority_queue CẤM bạn dùng vòng lặp để duyệt qua các phần tử.
    - Dùng thuật toán Heap trên std::vector, bạn có thể TỰ DO duyệt mảng, in ấn,
      hoặc biến nó trở lại thành mảng bình thường bất cứ lúc nào.

 3. Quy tắc cốt lõi của STL Heap:
    - Để THÊM: Phải v.push_back() vào vector TRƯỚC, rồi mới gọi
 std::push_heap().
    - Để RÚT: Phải gọi std::pop_heap() TRƯỚC, rồi mới v.pop_back() khỏi vector.
*/

// ==========================================
// CLASS & STRUCT PHỤ TRỢ CHO PHẦN CHUYÊN GIA
// ==========================================
struct NhiemVu {
  std::string ten;
  int diem_quan_trong;

  NhiemVu(std::string t, int diem) : ten(t), diem_quan_trong(diem) {}
};

// Functor so sánh tùy chỉnh
struct SoSanhNhiemVu {
  // Tạo Max-Heap dựa trên điểm quan trọng
  bool operator()(const NhiemVu &a, const NhiemVu &b) const {
    return a.diem_quan_trong < b.diem_quan_trong;
  }
};

// Hàm tiện ích để in vector
void in_vector(const std::vector<int> &v, const std::string &thong_diep) {
  std::cout << thong_diep << ": ";
  for (int x : v)
    std::cout << x << " ";
  std::cout << "\n";
}

// ==========================================
// 1. MỨC CƠ BẢN: TẠO VÀ THAO TÁC VỚI MAX-HEAP
// ==========================================
void heap_co_ban() {
  std::cout << "\n--- 1. BAZIC: MAX-HEAP MAC DINH ---\n";

  std::vector<int> v = {3, 1, 4, 1, 5, 9};
  in_vector(v, "Vector ban dau (Chua phai Heap)");

  // 1. BIẾN VECTOR THÀNH HEAP (O(N))
  std::make_heap(v.begin(), v.end());
  // Lúc này v.front() CHẮC CHẮN là số lớn nhất (số 9). Các số sau nằm lộn xộn
  // theo cấu trúc Cây.
  in_vector(v, "Sau khi make_heap (Max-Heap)");
  std::cout << " -> Phan tu lon nhat (Noc) la: " << v.front() << "\n";

  // 2. THÊM PHẦN TỬ MỚI (O(log N))
  // BƯỚC A: Nhét vào cuối mảng trước (Nó đang phá vỡ cấu trúc Heap)
  v.push_back(7);
  // BƯỚC B: Ra lệnh cho C++ sắp xếp lại phần tử cuối cùng cho đúng chuẩn Heap
  std::push_heap(v.begin(), v.end());
  in_vector(v, "Sau khi push_heap so 7");

  // 3. RÚT PHẦN TỬ LỚN NHẤT RA KẾT VÀ HỦY (O(log N))
  // BƯỚC A: pop_heap KHÔNG xóa phần tử. Nó chỉ lấy thằng Lớn Nhất trên Nóc,
  // tống xuống CUỐI vector, và đôn thằng khác lên làm Nóc.
  std::pop_heap(v.begin(), v.end());
  std::cout << "Phan tu bi loai (dang nam o cuoi vector): " << v.back() << "\n";

  // BƯỚC B: Bây giờ mới thực sự XÓA nó khỏi bộ nhớ RAM
  v.pop_back();
  in_vector(v, "Sau khi pop_heap va pop_back");
}

// ==========================================
// 2. MỨC TRUNG CẤP: MIN-HEAP & KIỂM TRA HEAP
// ==========================================
void heap_min_va_kiem_tra() {
  std::cout << "\n--- 2. INTERMEDIATE: MIN-HEAP & IS_HEAP ---\n";

  std::vector<int> v = {8, 2, 9, 4, 7, 1};

  // ĐỂ TẠO MIN-HEAP: Phải nhét std::greater<int>() vào TẤT CẢ các hàm Heap
  std::make_heap(v.begin(), v.end(), std::greater<int>());
  in_vector(v, "Min-Heap (Nho nhat len noc)"); // Gốc chắc chắn là 1

  // Kiểm tra xem mảng có chuẩn Heap không (O(N))
  if (std::is_heap(v.begin(), v.end(), std::greater<int>())) {
    std::cout << " -> Chuan, mảng nay dang tuan thu luat Min-Heap!\n";
  }

  // Thử phá vỡ luật: Sửa số trên nóc thành 100
  v.front() = 100;
  if (!std::is_heap(v.begin(), v.end(), std::greater<int>())) {
    std::cout << " -> Bao dong! Cau truc Min-Heap da bi pha vo!\n";
  }

  // C++11: is_heap_until - Tìm xem nó bị hỏng chuẩn Heap từ vị trí nào
  auto it = std::is_heap_until(v.begin(), v.end(), std::greater<int>());
  std::cout << " -> Heap bi hong bat dau tu so: " << *it << "\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: HEAP SORT (SẮP XẾP VUN ĐỐNG)
// ==========================================
void heap_sort_nang_cao() {
  std::cout << "\n--- 3. ADVANCED: HEAP SORT ---\n";

  std::vector<int> v = {10, 5, 30, 20, 50, 40};

  // Bước 1: Phải biến nó thành Heap trước
  std::make_heap(v.begin(), v.end());
  in_vector(v, "Max-Heap hien tai");

  // Bước 2: sort_heap (O(N log N))
  // Hàm này sẽ liên tục gọi pop_heap để vứt thằng lớn nhất xuống cuối mảng,
  // lặp đi lặp lại cho đến khi mảng trở thành một mảng ĐÃ SẮP XẾP TĂNG DẦN!
  std::sort_heap(v.begin(), v.end());

  // LƯU Ý KINH ĐIỂN: Sau khi gọi sort_heap, mảng ĐÃ MẤT TÍNH CHẤT HEAP!
  // Bạn không thể gọi push_heap hay pop_heap được nữa. Nó đã trở thành mảng
  // thường.
  in_vector(v, "Sau khi sort_heap (Da duoc xep Tang dan)");
}

// ==========================================
// 4. MỨC CHUYÊN GIA: HEAP VỚI CLASS TÙY CHỈNH
// ==========================================
void heap_chuyen_gia_class() {
  std::cout << "\n--- 4. EXPERT: CUSTOM CLASS HEAP ---\n";

  std::vector<NhiemVu> ds_nhiem_vu = {{"Ngu", 10},
                                      {"Hoc C++", 100}, // Sẽ nổi lên đầu
                                      {"Choi Game", 50},
                                      {"Quet nha", 30}};

  SoSanhNhiemVu bo_so_sanh;

  // Truyền Functor vào make_heap
  std::make_heap(ds_nhiem_vu.begin(), ds_nhiem_vu.end(), bo_so_sanh);

  std::cout << "Nhiem vu quan trong nhat tren dinh Heap la: "
            << ds_nhiem_vu.front().ten << "\n";

  // Thêm nhiệm vụ mới
  ds_nhiem_vu.push_back(NhiemVu("Cap cuu", 999));
  std::push_heap(ds_nhiem_vu.begin(), ds_nhiem_vu.end(), bo_so_sanh);

  std::cout << "Khang cap! Nhiem vu tren dinh da doi thanh: "
            << ds_nhiem_vu.front().ten << "\n";

  // Rút dần các nhiệm vụ ra để xử lý
  std::cout << "\nBat dau xu ly theo muc do uu tien:\n";
  while (!ds_nhiem_vu.empty()) {
    std::pop_heap(ds_nhiem_vu.begin(), ds_nhiem_vu.end(), bo_so_sanh);

    NhiemVu nv_hien_tai = ds_nhiem_vu.back();
    ds_nhiem_vu.pop_back(); // Hủy

    std::cout << "  - Xu ly: " << nv_hien_tai.ten
              << " (Diem: " << nv_hien_tai.diem_quan_trong << ")\n";
  }
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  heap_co_ban();
  heap_min_va_kiem_tra();
  heap_sort_nang_cao();
  heap_chuyen_gia_class();

  std::cout << "\n--- HOAN THANH ---\n";
  return 0;
}