# Thao tác trên Mảng Hai Chiều (2D Array)

## Nguyên tắc bất di bất dịch

Đối với **mảng hai chiều tĩnh (Static 2D Array)**, nguyên tắc quan trọng nhất là:

> **Kích thước cố định (Fixed-size)**

Số **hàng** và **cột** được xác định ngay tại thời điểm biên dịch và **không thể thay đổi** trong suốt quá trình chương trình chạy.

Ví dụ:

```cpp
int ma_tran[2][3];
```

Mảng trên luôn có:

- 2 hàng
- 3 cột

Bạn **không thể**:

- thêm hàng
- xóa hàng
- thêm cột
- xóa cột

một cách trực tiếp.

---

# 1. Duyệt mảng (Traverse)

Có hai cách phổ biến để duyệt mảng hai chiều.

---

## Cách 1. Vòng lặp `for` truyền thống

Đây là cách quen thuộc nhất.

```cpp
for (int i = 0; i < 2; ++i)
{
    for (int j = 0; j < 3; ++j)
    {
        std::cout << ma_tran[i][j] << " ";
    }

    std::cout << '\n';
}
```

Trong đó:

- `i` là chỉ số hàng.
- `j` là chỉ số cột.

---

## Cách 2. Range-based for loop (Khuyến nghị)

Modern C++ cho phép duyệt mảng ngắn gọn hơn.

```cpp
std::cout << "Duyet mang bang Range-based for:\n";

for (const auto& hang : ma_tran)
{
    for (int phan_tu : hang)
    {
        std::cout << phan_tu << " ";
    }

    std::cout << '\n';
}
```

---

## Vì sao phải dùng `const auto&`?

```cpp
for (const auto& hang : ma_tran)
```

Ở đây:

```cpp
hang
```

không phải là một số nguyên.

Nó là **một hàng của ma trận**, tức là một **mảng con**.

Nếu bỏ dấu `&`:

```cpp
for (auto hang : ma_tran)
```

mảng con sẽ bị **array decay** thành con trỏ, dẫn đến lỗi hoặc hành vi không mong muốn.

Do đó:

```cpp
const auto&
```

là lựa chọn an toàn và được khuyến nghị.

---

## Độ phức tạp

Giả sử:

- `m` hàng
- `n` cột

Thời gian duyệt:

```text
O(m × n)
```

---

# 2. Sửa phần tử (Update)

Mảng hai chiều hoạt động giống như một bảng tọa độ.

Cú pháp:

```cpp
ma_tran[hàng][cột]
```

Ví dụ:

```cpp
ma_tran[1][2] = 99;
```

Nếu ban đầu:

```text
1 2 3
4 5 6
```

Sau khi sửa:

```text
1 2 3
4 5 99
```

---

## Độ phức tạp

Truy cập trực tiếp bằng chỉ số nên:

```text
O(1)
```

---

# 3. Thêm và Xóa trên mảng hai chiều (Mô phỏng)

Do kích thước bị cố định nên ta **không thể thêm hoặc xóa thật sự**.

Giải pháp là:

- cấp phát dư sức chứa
- dùng biến đếm số hàng thực tế

Ví dụ:

```cpp
int bang_du_lieu[10][10];
```

Nhưng hiện tại chỉ sử dụng:

```cpp
int so_hang = 2;
int so_cot = 3;
```

---

# Thêm một hàng

Ví dụ thêm:

```text
7 8 9
```

vào cuối.

```cpp
int bang_du_lieu[10][10] =
{
    {1,2,3},
    {4,5,6}
};

int so_hang = 2;
int so_cot = 3;

if (so_hang < 10)
{
    bang_du_lieu[so_hang][0] = 7;
    bang_du_lieu[so_hang][1] = 8;
    bang_du_lieu[so_hang][2] = 9;

    ++so_hang;
}
```

Sau đó:

```text
1 2 3
4 5 6
7 8 9
```

---

## Độ phức tạp

Thêm cuối:

```text
O(n)
```

Trong đó:

- `n` là số cột.

---

# Xóa một hàng

Muốn xóa:

```text
1 2 3
```

ta phải kéo các hàng phía dưới lên.

```cpp
int vi_tri_hang_xoa = 0;

if (so_hang > 0)
{
    for (int i = vi_tri_hang_xoa; i < so_hang - 1; ++i)
    {
        for (int j = 0; j < so_cot; ++j)
        {
            bang_du_lieu[i][j] =
                bang_du_lieu[i + 1][j];
        }
    }

    --so_hang;
}
```

Sau khi xóa:

```text
4 5 6
7 8 9
```

---

## Độ phức tạp

Giả sử:

- còn `m` hàng
- mỗi hàng có `n` cột

Thời gian:

```text
O(m × n)
```

---

# 4. Sắp xếp mảng hai chiều (Sort)

Đây là phần dễ gây nhầm lẫn nhất.

Không thể viết:

```cpp
std::sort(ma_tran);
```

và mong C++ hiểu bạn muốn sắp xếp toàn bộ ma trận.

Muốn sắp xếp đúng, cần hiểu cách dữ liệu được lưu trong RAM.

---

# Memory Layout (Row-major Order)

Mảng:

```cpp
int ma_tran[2][3] =
{
    {1,2,3},
    {4,5,6}
};
```

Trong bộ nhớ thực chất là:

```text
1 2 3 4 5 6
```

Không tồn tại "hàng" hay "cột" trong RAM.

Tất cả được lưu liên tiếp.

Đây gọi là:

> **Row-major Order**

---

# Kịch bản 1. Sắp xếp từng hàng

Mỗi hàng được xem như một mảng riêng.

```cpp
#include <algorithm>

for (int i = 0; i < 2; ++i)
{
    std::sort(
        ma_tran[i],
        ma_tran[i] + 3
    );
}
```

Ví dụ:

Ban đầu

```text
3 2 1
6 5 4
```

Sau khi sort:

```text
1 2 3
4 5 6
```

Mỗi hàng chỉ tự sắp xếp bên trong nó.

---

# Kịch bản 2. Sắp xếp toàn bộ ma trận

Nếu muốn xem cả ma trận là một mảng dài:

```text
3 2 1 6 5 4
```

ta lấy địa chỉ phần tử đầu tiên.

```cpp
int* con_tro_dau = &ma_tran[0][0];
```

Sau đó:

```cpp
std::sort(
    con_tro_dau,
    con_tro_dau + (2 * 3)
);
```

Kết quả:

```text
1 2 3
4 5 6
```

Lưu ý:

Các số có thể **đổi hàng**, vì toàn bộ ma trận được xem như một mảng duy nhất.

---

# Độ phức tạp

Giả sử:

- `m` hàng
- `n` cột

Tổng số phần tử:

```text
m × n
```

Thời gian:

```text
O((m × n) log(m × n))
```

---

# Góc nhìn thực tế

Trong:

- Game Development
- Image Processing
- Graphics Programming

lập trình viên thường **không dùng mảng hai chiều**.

Thay vào đó:

```cpp
int hinh_anh[rong * cao];
```

Ví dụ:

Ảnh:

```text
800 × 600
```

được lưu thành:

```cpp
int pixels[800 * 600];
```

---

# Chuyển tọa độ (x, y) sang chỉ số

Muốn truy cập điểm ảnh:

```text
(x, y)
```

ta tính:

```cpp
index = y * rong + x;
```

Ví dụ:

```text
Rộng = 800

Điểm:

x = 20
y = 10
```

thì:

```cpp
index = 10 * 800 + 20
```

```text
index = 8020
```

Sau đó:

```cpp
pixels[index]
```

chính là điểm ảnh tại tọa độ `(20, 10)`.

---

# Ưu điểm của mảng một chiều

So với mảng hai chiều truyền thống:

- Linh hoạt hơn.
- Không bị giới hạn số hàng và cột cố định.
- Hiệu suất truy cập tốt.
- Phù hợp với các thư viện đồ họa và game engine hiện đại.
- Quản lý bộ nhớ đơn giản hơn khi cấp phát động.

---

# Tóm tắt

| Thao tác             | Độ phức tạp               |
| -------------------- | ------------------------- |
| Truy cập phần tử     | **O(1)**                  |
| Cập nhật             | **O(1)**                  |
| Duyệt                | **O(m × n)**              |
| Thêm hàng (mô phỏng) | **O(n)**                  |
| Xóa hàng (mô phỏng)  | **O(m × n)**              |
| Sort từng hàng       | **O(m × n log n)**        |
| Sort toàn bộ ma trận | **O((m × n) log(m × n))** |

> **Kết luận:**  
> Mảng hai chiều tĩnh vẫn tuân theo nguyên tắc **kích thước cố định**. Các thao tác thêm và xóa chỉ có thể được **mô phỏng** bằng cách quản lý số hàng hoặc số cột thực tế. Trong các ứng dụng hiện đại như game hoặc xử lý ảnh, lập trình viên thường sử dụng **mảng một chiều kết hợp công thức chuyển đổi chỉ số** để đạt hiệu năng và tính linh hoạt cao hơn.
