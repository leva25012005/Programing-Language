#include <cassert>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <vector>


// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Vấn đề lịch sử (Trước C++23):
    - C++ không cho phép truyền nhiều tham số vào ngoặc vuông.
      Lệnh matrix[x, y] thực chất là LỖI, vì dấu phẩy ',' bị hiểu nhầm là Comma
 Operator (Toán tử phẩy của C cũ, nó sẽ ném x đi và chỉ lấy giá trị y).

 2. Cách giải quyết trong quá khứ:
    - Cách 1: Dùng matrix(x, y). Nhược điểm: Nhìn giống gọi hàm (Function call)
 chứ không giống truy xuất mảng (Array access). Kém trực quan.
    - Cách 2: Dùng matrix[x][y]. Nhược điểm: Nếu dùng mảng 1D giả làm 2D, bạn
 phải viết thêm một Proxy Class (Lớp trung gian) rất phức tạp để "hứng" ngoặc
 vuông đầu tiên, sau đó mới truyền tiếp vào ngoặc vuông thứ hai.

 3. Cuộc cách mạng C++23:
    - Trình biên dịch chính thức cho phép `operator[]` nhận bao nhiêu tham số
 tùy thích.
    - Cú pháp cực kỳ tự nhiên: matrix[x, y, z].
*/

// ==========================================
// 1. MỨC CƠ BẢN: MA TRẬN 2D VỚI C++23
// ==========================================
class MaTran2D {
private:
  std::vector<int> data;
  size_t rows;
  size_t cols;

public:
  MaTran2D(size_t r, size_t c) : rows(r), cols(c), data(r * c, 0) {}

  // C++23: QUÁ TẢI TOÁN TỬ [] VỚI 2 THAM SỐ
  // Chuyển đổi tọa độ 2D (r, c) thành index 1D liền kề trên RAM (r * cols + c)
  int &operator[](size_t r, size_t c) {
    if (r >= rows || c >= cols)
      throw std::out_of_range("Vuot qua gioi han ma tran!");
    return data[r * cols + c];
  }

  // Phiên bản const (Chỉ đọc)
  const int &operator[](size_t r, size_t c) const {
    if (r >= rows || c >= cols)
      throw std::out_of_range("Vuot qua gioi han ma tran!");
    return data[r * cols + c];
  }

  void in_ma_tran() const {
    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        // Sử dụng toán tử mới ngay bên trong class
        std::cout << (*this)[i, j] << "\t";
      }
      std::cout << "\n";
    }
  }
};

void demo_co_ban() {
  std::cout << "\n--- 1. BAZIC: C++23 MULTIDIMENSIONAL OPERATOR[] ---\n";

  MaTran2D mat(3, 3);

  // TRỰC QUAN NHƯ TOÁN HỌC: [hang, cot]
  mat[0, 0] = 1;
  mat[1, 1] = 5;
  mat[2, 2] = 9;
  mat[0, 2] = 99; // Hàng 0, Cột 2

  mat.in_ma_tran();
}

// ==========================================
// 2. MỨC TRUNG CẤP: SO SÁNH VỚI NHỮNG HÀNH TRANG QUÁ KHỨ
// ==========================================
class MaTranCoDien {
private:
  std::vector<int> data;
  size_t rows, cols;

public:
  MaTranCoDien(size_t r, size_t c) : rows(r), cols(c), data(r * c, 0) {}

  // CÁCH 1 (C++98): Dùng operator()
  // Kém trực quan vì dấu ngoặc tròn thường dùng cho Hành động/Hàm.
  int &operator()(size_t r, size_t c) { return data[r * cols + c]; }

  // CÁCH 2 (C++98): Dùng toán tử [][] qua Proxy Class
  // Rất rườm rà, phải tạo một class con chỉ để hứng tọa độ cột.
  class ProxyHang {
    MaTranCoDien &parent;
    size_t row;

  public:
    ProxyHang(MaTranCoDien &p, size_t r) : parent(p), row(r) {}
    int &operator[](size_t col) { return parent.data[row * parent.cols + col]; }
  };

  ProxyHang operator[](size_t r) { return ProxyHang(*this, r); }
};

void demo_lich_su() {
  std::cout << "\n--- 2. INTERMEDIATE: NOI KHO TRUOC C++23 ---\n";

  MaTranCoDien mat_cu(3, 3);

  mat_cu(0, 1) = 10; // Nhìn giống gọi hàm matrix()
  mat_cu[0][2] = 20; // Phải tạo Proxy Object rất cồng kềnh dưới nền

  std::cout << "Dung (): " << mat_cu(0, 1) << " | Dung [][]: " << mat_cu[0][2]
            << "\n";
  std::cout << "-> C++23 matrix[x, y] sinh ra de tieu diet ca 2 cach tren!\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: VARIADIC TEMPLATES (MA TRẬN N CHIỀU)
// ==========================================
// C++23 cho phép operator[] kết hợp với Variadic Templates để nhận vô hạn tham
// số!
template <size_t... Dims> // Nhận vào kích thước các chiều (ví dụ: MaTranND<2,
                          // 3, 4>)
                          class MaTranND {
private:
  std::vector<int> data;
  // Tính tổng số lượng phần tử cần thiết (Nhân tất cả các chiều lại với nhau)
  static constexpr size_t total_size = (Dims * ... * 1);

public:
  MaTranND() : data(total_size, 0) {}

  // KỸ THUẬT SIÊU CẤP: operator[] nhận N tham số (Tọa độ của N chiều)
  template <typename... Indices> int &operator[](Indices... indices) {
    static_assert(
        sizeof...(indices) == sizeof...(Dims),
        "Loi: So luong toa do truyen vao phai BẰNG so chieu cua Ma Tran!");

    // Tính toán Index 1D phẳng từ N tọa độ đa chiều (Flat index calculation)
    // Đây là thuật toán cốt lõi của các thư viện Tensor như PyTorch/TensorFlow
    size_t flat_index = 0;
    size_t multipliers[] = {Dims...};
    size_t coords[] = {static_cast<size_t>(indices)...};
    size_t current_multiplier = 1;

    // Tính ngược từ chiều cuối lên chiều đầu (Row-major order)
    for (int i = sizeof...(Dims) - 1; i >= 0; --i) {
      flat_index += coords[i] * current_multiplier;
      current_multiplier *= multipliers[i];
    }

    return data[flat_index];
  }
};

void demo_nang_cao() {
  std::cout << "\n--- 3. ADVANCED: N-DIMENSIONAL VARIADIC OPERATOR[] ---\n";

  // Khởi tạo một Tensor 3D (Khối rubik 3x3x3)
  MaTranND<3, 3, 3> rubik;

  // Gán dữ liệu bằng C++23 operator[] (3 tham số tọa độ X, Y, Z)
  rubik[0, 0, 0] = 100;
  rubik[1, 2, 1] = 200;
  rubik[2, 2, 2] = 999;

  // rubik[1, 1] = 50; // LỖI BIÊN DỊCH: Tensor 3D bắt buộc phải truyền đủ 3 tọa
  // độ!

  std::cout << "Toa do [0,0,0]: " << rubik[0, 0, 0] << "\n";
  std::cout << "Toa do [1,2,1]: " << rubik[1, 2, 1] << "\n";
  std::cout << "Toa do [2,2,2]: " << rubik[2, 2, 2] << "\n";
}

// ==========================================
// 4. MỨC CHUYÊN GIA: BỘ ĐÔI HOÀN HẢO std::mdspan (C++23)
// ==========================================
/*
 ĐỈNH CAO CỦA C++23 KHÔNG NẰM Ở VIỆC BẠN TỰ VIẾT CLASS.
 C++23 ra mắt thư viện `<mdspan>` (Multidimensional Span).
 Nó là một "lớp mặt nạ" (View) đa chiều có chi phí bằng 0 (Zero-overhead),
 bọc lên một mảng 1D liền kề (như std::vector hoặc mảng C).
 Và đương nhiên, std::mdspan SỬ DỤNG SẴN C++23 operator[] đa chiều!
*/

#if __cplusplus >= 202302L
#include <mdspan> // Yêu cầu trình biên dịch cực kỳ hiện đại

void demo_chuyen_gia_mdspan() {
  std::cout << "\n--- 4. EXPERT: STD::MDSPAN (C++23) ---\n";

  std::vector<int> mang_1d(12, 0); // Mảng 1D chứa 12 phần tử

  // Tạo một mặt nạ 2D (3 hàng, 4 cột) bọc lên mảng 1D
  // Không hề tốn chi phí Copy RAM!
  std::mdspan<int, std::extents<size_t, 3, 4>> ma_tran_ao(mang_1d.data());

  // Thao tác với mảng 1D thông qua mặt nạ 2D bằng cú pháp C++23
  ma_tran_ao[0, 0] = 1;
  ma_tran_ao[1, 2] = 42;
  ma_tran_ao[2, 3] = 99;

  std::cout << "Du lieu thuc the duoi RAM (mang 1D):\n";
  for (int x : mang_1d)
    std::cout << x << " ";
  // Output: 1 0 0 0 0 0 42 0 0 0 0 99
  std::cout << "\n";
}
#else
void demo_chuyen_gia_mdspan() {
  std::cout << "\n--- 4. EXPERT: STD::MDSPAN (C++23) ---\n";
  std::cout << "(Trinh bien dich cua ban chua ho tro <mdspan>. Hay nang cap "
               "GCC 13+ hoac Clang 16+ de dung tinh nang nay.)\n";
}
#endif

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== C++23: MULTIDIMENSIONAL OPERATOR[] =====\n";

  demo_co_ban();
  demo_lich_su();
  demo_nang_cao();
  demo_chuyen_gia_mdspan();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}