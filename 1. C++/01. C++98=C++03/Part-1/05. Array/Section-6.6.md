# Các cách triển khai Mảng Hai Chiều Động trong C++

Khi cần tạo một **mảng hai chiều có kích thước được quyết định trong lúc chương trình chạy (Runtime)**, C++ cung cấp nhiều cách triển khai khác nhau.

Mỗi cách đều có ưu và nhược điểm riêng về:

- Hiệu năng.
- Quản lý bộ nhớ.
- Độ an toàn.
- Khả năng mở rộng.

---

# Cách 1. Mảng của các con trỏ (Array of Pointers)

Đây là cách triển khai truyền thống bằng `new[]` và `delete[]`.

Ý tưởng:

- Cấp phát một mảng chứa các con trỏ (đại diện cho các hàng).
- Mỗi con trỏ lại trỏ đến một mảng một chiều (đại diện cho các cột).

Sơ đồ:

```text
ma_tran
   │
   ▼
+-------+-------+-------+
|  *    |   *   |   *   |
+-------+-------+-------+
    │        │        │
    ▼        ▼        ▼
+----+    +----+    +----+
|    |    |    |    |    |
|    |    |    |    |    |
|    |    |    |    |    |
+----+    +----+    +----+
```

Mỗi hàng được cấp phát độc lập trên Heap.

---

## Ví dụ

```cpp
#include <iostream>

int main()
{
    int so_hang = 3;
    int so_cot = 4;

    //==========================
    // CẤP PHÁT
    //==========================

    int** ma_tran = new int*[so_hang];

    for (int i = 0; i < so_hang; ++i)
    {
        ma_tran[i] = new int[so_cot];
    }

    //==========================
    // SỬ DỤNG
    //==========================

    ma_tran[1][2] = 99;

    std::cout
        << ma_tran[1][2]
        << '\n';

    //==========================
    // GIẢI PHÓNG
    //==========================

    for (int i = 0; i < so_hang; ++i)
    {
        delete[] ma_tran[i];
    }

    delete[] ma_tran;

    ma_tran = nullptr;
}
```

---

## Ưu điểm

- Có thể tạo **Jagged Array (mảng răng cưa)**.
- Mỗi hàng có thể có số cột khác nhau.

Ví dụ:

```text
*
* * *
* * * * * *
*
```

---

## Nhược điểm

- Phải gọi nhiều lần `new`.
- Phải gọi nhiều lần `delete`.
- Dễ gây **Memory Leak**.
- Bộ nhớ không liên tục.
- CPU Cache hoạt động kém hơn.

---

# Độ phức tạp

| Thao tác   | Độ phức tạp    |
| ---------- | -------------- |
| Truy cập   | **O(1)**       |
| Cập nhật   | **O(1)**       |
| Cấp phát   | **O(số hàng)** |
| Giải phóng | **O(số hàng)** |

---

# Cách 2. Mảng Một Chiều Giả Lập Hai Chiều (Flattened 1D Array)

Đây là phương pháp được sử dụng rất nhiều trong:

- Game Engine
- OpenCV
- AI
- Machine Learning
- Image Processing

Ý tưởng:

Không tạo nhiều hàng.

Chỉ tạo **một mảng một chiều rất lớn**.

```text
1 2 3 4 5 6 7 8 9 ...
```

Sau đó dùng công thức để quy đổi từ:

```text
(hàng, cột)
```

sang

```text
index
```

---

## Công thức

```cpp
index = y * so_cot + x;
```

Trong đó:

- `y` là hàng.
- `x` là cột.

---

## Ví dụ

```cpp
#include <iostream>

int main()
{
    int so_hang = 3;
    int so_cot = 4;

    //==========================
    // CẤP PHÁT
    //==========================

    int* ma_tran = new int[so_hang * so_cot];

    //==========================
    // SỬ DỤNG
    //==========================

    int y = 1;
    int x = 2;

    int index = y * so_cot + x;

    ma_tran[index] = 99;

    std::cout
        << ma_tran[index]
        << '\n';

    //==========================
    // GIẢI PHÓNG
    //==========================

    delete[] ma_tran;

    ma_tran = nullptr;
}
```

---

## Ví dụ chuyển đổi

Giả sử:

```text
3 hàng

4 cột
```

Bảng:

```text
0 1 2 3
4 5 6 7
8 9 10 11
```

Muốn lấy:

```text
(hàng = 2, cột = 1)
```

Ta tính:

```text
index = 2 * 4 + 1
```

```text
index = 9
```

---

## Ưu điểm

- Chỉ cần:

```cpp
new
```

một lần.

- Chỉ cần:

```cpp
delete
```

một lần.

- Bộ nhớ liên tục.

- CPU Cache hoạt động rất hiệu quả.

- Truy cập nhanh.

---

## Nhược điểm

Không thể viết:

```cpp
ma_tran[1][2]
```

Mà phải viết:

```cpp
ma_tran[y * so_cot + x]
```

---

# Độ phức tạp

| Thao tác   | Độ phức tạp |
| ---------- | ----------- |
| Truy cập   | **O(1)**    |
| Cập nhật   | **O(1)**    |
| Cấp phát   | **O(1)**    |
| Giải phóng | **O(1)**    |

---

# Cách 3. `std::vector` lồng nhau (Vector of Vectors)

Đây là phương pháp được khuyến nghị trong C++ hiện đại.

Ý tưởng:

Một `vector` chứa nhiều `vector<int>`.

```text
vector
   │
   ▼
+---------+
| vector  |
+---------+
| vector  |
+---------+
| vector  |
+---------+
```

---

## Ví dụ

```cpp
#include <iostream>
#include <vector>

int main()
{
    int so_hang = 3;
    int so_cot = 4;

    std::vector<std::vector<int>>
        ma_tran(
            so_hang,
            std::vector<int>(so_cot, 0)
        );

    ma_tran[1][2] = 99;

    std::cout
        << ma_tran[1][2]
        << '\n';

    // Không cần delete
}
```

---

## Ưu điểm

- Không cần:

```cpp
new
delete
```

- Không lo Memory Leak.

- Viết ngắn gọn.

- Hỗ trợ:

```cpp
ma_tran[hàng][cột]
```

- Có thể dùng toàn bộ STL.

---

## Nhược điểm

Giống cách 1:

Mỗi hàng vẫn là một `vector` riêng.

Do đó:

- Bộ nhớ không hoàn toàn liên tục.
- Chậm hơn Flattened Array một chút nếu ma trận cực lớn.

---

# So sánh ba cách

| Tiêu chí            | Mảng con trỏ | Flattened Array              | `std::vector<std::vector<T>>` |
| ------------------- | ------------ | ---------------------------- | ----------------------------- |
| Quản lý bộ nhớ      | Thủ công     | Thủ công                     | Tự động                       |
| Cần `delete`        | Có           | Có                           | Không                         |
| Memory Leak         | Dễ xảy ra    | Có thể xảy ra                | Không                         |
| Bộ nhớ liên tục     | Không        | Có                           | Không                         |
| CPU Cache           | Trung bình   | Tốt nhất                     | Trung bình                    |
| Hỗ trợ `[row][col]` | Có           | Không                        | Có                            |
| Hỗ trợ Jagged Array | Có           | Không                        | Có                            |
| Độ an toàn          | Thấp         | Trung bình                   | Cao                           |
| Mức độ khuyến nghị  | Thấp         | Cao trong bài toán hiệu năng | Cao trong hầu hết ứng dụng    |

---

# Khi nào nên dùng?

## Dùng `std::vector<std::vector<T>>`

Đây là lựa chọn mặc định trong đa số dự án C++ hiện đại.

Phù hợp khi:

- Làm ứng dụng thông thường.
- Phát triển phần mềm.
- Học STL.
- Không cần tối ưu hiệu năng ở mức cực hạn.

---

## Dùng Flattened 1D Array

Nên dùng khi:

- Game Engine.
- Đồ họa.
- OpenCV.
- AI.
- Machine Learning.
- Xử lý ảnh.
- Mô phỏng vật lý.
- Ma trận rất lớn (hàng triệu phần tử).

Đây là phương pháp cho hiệu năng cao nhất.

---

## Dùng Array of Pointers

Chỉ nên dùng khi:

- Học về quản lý bộ nhớ.
- Học Cấu trúc dữ liệu và Giải thuật.
- Bảo trì các dự án cũ (Legacy Code).
- Cần tạo **Jagged Array** với số cột mỗi hàng khác nhau.

---

# Kết luận

C++ hiện đại cung cấp nhiều cách triển khai mảng hai chiều động, mỗi cách phù hợp với một mục đích khác nhau:

- **`std::vector<std::vector<T>>`** là lựa chọn ưu tiên trong hầu hết các ứng dụng nhờ tính an toàn, dễ sử dụng và tự động quản lý bộ nhớ.
- **Flattened 1D Array** mang lại hiệu năng cao nhất nhờ bộ nhớ liên tục và tận dụng tốt CPU Cache, rất phù hợp cho các bài toán tính toán hiệu năng cao.
- **Array of Pointers** chủ yếu còn được sử dụng trong mục đích học tập, bảo trì mã nguồn cũ hoặc khi cần xây dựng các **Jagged Array**.
