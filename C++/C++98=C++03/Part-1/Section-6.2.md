# Thao tác trên Mảng trong C++

---

# 1. Thao tác Thêm (Insert / Add)

Đối với **mảng tĩnh (Static Array)**, để thêm phần tử vào **đầu** hoặc **giữa** mảng, bạn **bắt buộc phải dịch chuyển (shift)** toàn bộ các phần tử phía sau sang phải **1 ô** để tạo khoảng trống.

Riêng trường hợp thêm vào **cuối mảng**, nếu vẫn còn ô trống thì chỉ cần gán giá trị và tăng số lượng phần tử.

## Ví dụ

```cpp
#include <iostream>
#include <array>

int main() {
    // Sức chứa tối đa là 10, nhưng hiện tại chỉ dùng 3 phần tử
    std::array<int, 10> mang = {10, 20, 30};
    int so_luong = 3;

    std::cout << "--- THAO TAC THEM ---\n";

    // ============================
    // 1. Thêm vào CUỐI
    // ============================
    int gia_tri_them = 40;

    if (so_luong < mang.size()) {
        mang[so_luong] = gia_tri_them;
        so_luong++;
    }

    // ============================
    // 2. Thêm vào ĐẦU
    // ============================
    int gia_tri_dau = 5;

    if (so_luong < mang.size()) {

        // Dịch toàn bộ sang phải
        for (int i = so_luong; i > 0; --i) {
            mang[i] = mang[i - 1];
        }

        mang[0] = gia_tri_dau;
        so_luong++;
    }

    // ============================
    // 3. Thêm vào VỊ TRÍ BẤT KỲ
    // ============================
    int vi_tri_chen = 2;
    int gia_tri_chen = 15;

    if (so_luong < mang.size()) {

        for (int i = so_luong; i > vi_tri_chen; --i) {
            mang[i] = mang[i - 1];
        }

        mang[vi_tri_chen] = gia_tri_chen;
        so_luong++;
    }

    // Kết quả:
    // 5 10 15 20 30 40

    for (int i = 0; i < so_luong; ++i)
        std::cout << mang[i] << " ";

    std::cout << '\n';
}
```

---

## Độ phức tạp

| Thao tác  | Độ phức tạp |
| --------- | ----------- |
| Thêm cuối | **O(1)**    |
| Thêm đầu  | **O(n)**    |
| Thêm giữa | **O(n)**    |

---

# 2. Thao tác Xóa (Delete / Remove)

Để xóa một phần tử khỏi mảng, ta thực hiện ngược lại với thao tác thêm:

- Dịch toàn bộ các phần tử phía sau sang trái.
- Giảm số lượng phần tử thực tế đi 1.

## Ví dụ

```cpp
#include <iostream>
#include <array>

int main() {

    std::array<int, 10> mang = {10, 20, 30, 40, 50};
    int so_luong = 5;

    std::cout << "--- THAO TAC XOA ---\n";

    // ============================
    // 1. Xóa cuối
    // ============================
    if (so_luong > 0) {
        so_luong--;
    }

    // ============================
    // 2. Xóa đầu
    // ============================
    if (so_luong > 0) {

        for (int i = 0; i < so_luong - 1; ++i) {
            mang[i] = mang[i + 1];
        }

        so_luong--;
    }

    // ============================
    // 3. Xóa tại vị trí bất kỳ
    // ============================
    int vi_tri_xoa = 1;

    if (vi_tri_xoa >= 0 && vi_tri_xoa < so_luong) {

        for (int i = vi_tri_xoa; i < so_luong - 1; ++i) {
            mang[i] = mang[i + 1];
        }

        so_luong--;
    }

    // Kết quả:
    // 20 40

    for (int i = 0; i < so_luong; ++i)
        std::cout << mang[i] << " ";

    std::cout << '\n';
}
```

---

## Độ phức tạp

| Thao tác | Độ phức tạp |
| -------- | ----------- |
| Xóa cuối | **O(1)**    |
| Xóa đầu  | **O(n)**    |
| Xóa giữa | **O(n)**    |

---

# 3. Giải thích cơ chế dịch chuyển (Shifting)

## Khi chèn (Shift Right)

Ví dụ ban đầu:

```text
Index : 0   1   2
Value :10  20  30
```

Muốn chèn số **15** vào vị trí **1**

Trước tiên phải dịch chuyển:

```text
10 20 30
      ↓
10 20 30 30
```

Tiếp tục:

```text
10 20 20 30
```

Sau đó mới chèn:

```text
10 15 20 30
```

### Vì sao phải duyệt từ cuối về đầu?

Nếu duyệt từ đầu:

```cpp
for (...)
    mang[i + 1] = mang[i];
```

thì dữ liệu sẽ bị ghi đè.

Do đó phải chạy:

```cpp
for (int i = so_luong; i > vi_tri; --i)
```

để luôn sao chép từ cuối trước.

---

## Khi xóa (Shift Left)

Ví dụ:

```text
10 20 30 40
```

Xóa số **20**

Thực hiện:

```text
10 30 30 40
```

↓

```text
10 30 40 40
```

Giảm:

```cpp
so_luong--;
```

Kết quả logic:

```text
10 30 40
```

---

# 4. Chi phí hiệu năng

| Thao tác            | Độ phức tạp |
| ------------------- | ----------- |
| Truy cập theo Index | **O(1)**    |
| Cập nhật            | **O(1)**    |
| Thêm cuối           | **O(1)**    |
| Xóa cuối            | **O(1)**    |
| Thêm đầu            | **O(n)**    |
| Xóa đầu             | **O(n)**    |
| Thêm giữa           | **O(n)**    |
| Xóa giữa            | **O(n)**    |

Khi mảng có hàng triệu phần tử, việc dịch chuyển dữ liệu sẽ tiêu tốn đáng kể thời gian CPU.

---

# 5. Sửa phần tử (Update)

Do các phần tử nằm liên tiếp trong bộ nhớ, ta có thể truy cập trực tiếp thông qua **Index**.

Lưu ý:

- Chỉ số (Index) bắt đầu từ **0**.

## Ví dụ

```cpp
// C-Style Array
mang_co_dien[0] = 99;

// std::array
mang_hien_dai[1] = 88;

// An toàn hơn
mang_hien_dai.at(2) = 77;
```

### Vì sao nên dùng `.at()`?

Nếu truy cập ngoài phạm vi:

```cpp
mang_hien_dai.at(100);
```

chương trình sẽ phát hiện lỗi.

Trong khi:

```cpp
mang_hien_dai[100];
```

có thể gây **Undefined Behavior**.

---

# 6. Duyệt mảng (Traverse)

Có hai cách phổ biến.

---

## Cách 1. Range-based for (Khuyến nghị)

```cpp
for (int& phan_tu : mang_hien_dai)
{
    std::cout << phan_tu << " ";
}
```

Nếu chỉ đọc:

```cpp
for (const int& phan_tu : mang_hien_dai)
```

Nếu muốn sửa:

```cpp
for (int& phan_tu : mang_hien_dai)
```

---

## Cách 2. For truyền thống

```cpp
for (int i = 0; i < 5; ++i)
{
    std::cout << mang_co_dien[i] << " ";
}
```

Ưu điểm:

- Biết được Index.
- Thuận tiện xử lý thuật toán.

---

# 7. Sắp xếp mảng (Sort)

C++ cung cấp sẵn:

```cpp
std::sort()
```

trong thư viện:

```cpp
#include <algorithm>
```

---

## Sắp xếp `std::array`

```cpp
std::sort(
    mang_hien_dai.begin(),
    mang_hien_dai.end()
);
```

---

## Sắp xếp C-Style Array

```cpp
std::sort(
    mang_co_dien,
    mang_co_dien + 5
);
```

---

## Sắp xếp giảm dần

```cpp
#include <functional>

std::sort(
    mang_hien_dai.begin(),
    mang_hien_dai.end(),
    std::greater<int>()
);
```

---

# 8. Sự thật về Thêm và Xóa trên mảng tĩnh

Mảng tĩnh có kích thước **cố định** ngay từ khi khai báo.

Ví dụ:

```cpp
int mang[100];
```

Nó **không thể mở rộng** thành:

```text
101 phần tử
```

cũng không thể tự thu nhỏ.

---

## Cách làm phổ biến

Khai báo:

```cpp
int mang[100];
```

Theo dõi số lượng thực tế:

```cpp
int so_luong_hien_tai = 3;
```

Ví dụ:

```text
100 ô

Nhưng chỉ dùng

3 ô đầu
```

---

## Thêm

```cpp
mang[so_luong_hien_tai] = gia_tri;
so_luong_hien_tai++;
```

---

## Xóa

- Dịch trái.
- Giảm:

```cpp
so_luong_hien_tai--;
```

---

## Nhược điểm

- Phải tự quản lý kích thước.
- Dễ phát sinh lỗi.
- Phải viết nhiều vòng lặp dịch chuyển.
- Không linh hoạt.

Đây là lý do trong C++ hiện đại, người ta thường sử dụng **`std::vector`** thay cho mảng tĩnh khi cần thêm hoặc xóa phần tử thường xuyên.
