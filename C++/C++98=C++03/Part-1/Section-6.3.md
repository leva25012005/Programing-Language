# Thao tác trên Mảng Động Thủ Công (`new[]` và `delete[]`)

Khác với **mảng tĩnh**, mảng động được cấp phát trên **Heap** nên kích thước của nó không bị cố định ngay từ khi biên dịch.

Bạn có thể **cấp phát lại (Reallocation)** để tạo một mảng mới có kích thước phù hợp với nhu cầu.

Tuy nhiên, việc này đồng nghĩa với việc lập trình viên phải **tự quản lý bộ nhớ**, bao gồm:

- Cấp phát (`new[]`)
- Sao chép dữ liệu
- Giải phóng (`delete[]`)
- Cập nhật con trỏ

---

# 1. Sửa, Duyệt và Sắp xếp

Do mảng động thực chất chỉ là **một con trỏ trỏ đến phần tử đầu tiên**, nên cách sử dụng gần như giống hệt **C-Style Array**.

## Ví dụ

```cpp
#include <iostream>
#include <algorithm>

int main()
{
    int kich_thuoc = 4;

    int* mang = new int[kich_thuoc]{10, 20, 30, 40};

    // ==========================
    // SỬA
    // ==========================

    mang[1] = 99;

    // ==========================
    // DUYỆT
    // ==========================

    for (int i = 0; i < kich_thuoc; ++i)
    {
        std::cout << mang[i] << " ";
    }

    std::cout << '\n';

    // ==========================
    // SẮP XẾP
    // ==========================

    std::sort(mang, mang + kich_thuoc);

    delete[] mang;
}
```

---

## Độ phức tạp

| Thao tác              | Độ phức tạp    |
| --------------------- | -------------- |
| Truy cập              | **O(1)**       |
| Cập nhật              | **O(1)**       |
| Duyệt                 | **O(n)**       |
| Sắp xếp (`std::sort`) | **O(n log n)** |

---

# 2. Thêm phần tử (Insert)

Đối với mảng động thủ công, bạn **không thể mở rộng trực tiếp** vùng nhớ hiện tại.

Muốn thêm phần tử, bắt buộc phải thực hiện **Reallocation**.

---

## Quy trình 4 bước

### Bước 1

Tạo một mảng mới lớn hơn **1 phần tử**.

```cpp
int* mang_moi = new int[kich_thuoc + 1];
```

---

### Bước 2

Sao chép dữ liệu sang mảng mới và chèn phần tử.

---

### Bước 3

Giải phóng mảng cũ.

```cpp
delete[] mang;
```

---

### Bước 4

Cho con trỏ cũ trỏ sang mảng mới.

```cpp
mang = mang_moi;
kich_thuoc++;
```

---

## Ví dụ

```cpp
#include <iostream>

int main()
{
    int kich_thuoc = 3;

    int* mang = new int[kich_thuoc]{10, 20, 30};

    // Thêm 99 vào index = 1
    int vi_tri_chen = 1;
    int gia_tri_chen = 99;

    // ==========================
    // BƯỚC 1
    // ==========================

    int* mang_moi = new int[kich_thuoc + 1];

    // ==========================
    // BƯỚC 2
    // ==========================

    for (int i = 0, j = 0; i < kich_thuoc + 1; ++i)
    {
        if (i == vi_tri_chen)
        {
            mang_moi[i] = gia_tri_chen;
        }
        else
        {
            mang_moi[i] = mang[j];
            ++j;
        }
    }

    // ==========================
    // BƯỚC 3
    // ==========================

    delete[] mang;

    // ==========================
    // BƯỚC 4
    // ==========================

    mang = mang_moi;
    ++kich_thuoc;

    std::cout << "Mang sau khi them:\n";

    for (int i = 0; i < kich_thuoc; ++i)
    {
        std::cout << mang[i] << " ";
    }

    delete[] mang;
    mang = nullptr;
}
```

### Kết quả

```text
10 99 20 30
```

---

## Thêm vào các vị trí

| Vị trí | Giá trị `vi_tri_chen` |
| ------ | --------------------- |
| Đầu    | `0`                   |
| Giữa   | Bất kỳ                |
| Cuối   | `kich_thuoc`          |

---

## Độ phức tạp

| Thao tác  | Độ phức tạp                                    |
| --------- | ---------------------------------------------- |
| Thêm đầu  | **O(n)**                                       |
| Thêm giữa | **O(n)**                                       |
| Thêm cuối | **O(n)** _(vì phải cấp phát lại toàn bộ mảng)_ |

---

# 3. Xóa phần tử (Delete)

Việc xóa cũng phải thực hiện **Reallocation**.

---

## Quy trình 4 bước

### Bước 1

Tạo mảng mới nhỏ hơn một phần tử.

```cpp
int* mang_moi = new int[kich_thuoc - 1];
```

---

### Bước 2

Copy toàn bộ dữ liệu ngoại trừ phần tử cần xóa.

---

### Bước 3

Giải phóng mảng cũ.

```cpp
delete[] mang;
```

---

### Bước 4

Cho con trỏ trỏ sang mảng mới.

```cpp
mang = mang_moi;
kich_thuoc--;
```

---

## Ví dụ

```cpp
#include <iostream>

int main()
{
    int kich_thuoc = 4;

    int* mang = new int[kich_thuoc]{10, 20, 30, 40};

    // Xóa phần tử tại index = 2
    int vi_tri_xoa = 2;

    // ==========================
    // BƯỚC 1
    // ==========================

    int* mang_moi = new int[kich_thuoc - 1];

    // ==========================
    // BƯỚC 2
    // ==========================

    for (int i = 0, j = 0; i < kich_thuoc; ++i)
    {
        if (i != vi_tri_xoa)
        {
            mang_moi[j] = mang[i];
            ++j;
        }
    }

    // ==========================
    // BƯỚC 3
    // ==========================

    delete[] mang;

    // ==========================
    // BƯỚC 4
    // ==========================

    mang = mang_moi;
    --kich_thuoc;

    std::cout << "Mang sau khi xoa:\n";

    for (int i = 0; i < kich_thuoc; ++i)
    {
        std::cout << mang[i] << " ";
    }

    delete[] mang;
    mang = nullptr;
}
```

### Kết quả

```text
10 20 40
```

---

## Xóa tại các vị trí

| Vị trí | Giá trị `vi_tri_xoa` |
| ------ | -------------------- |
| Đầu    | `0`                  |
| Giữa   | Bất kỳ               |
| Cuối   | `kich_thuoc - 1`     |

---

## Độ phức tạp

| Thao tác | Độ phức tạp                                        |
| -------- | -------------------------------------------------- |
| Xóa đầu  | **O(n)**                                           |
| Xóa giữa | **O(n)**                                           |
| Xóa cuối | **O(n)** _(vì vẫn phải cấp phát lại toàn bộ mảng)_ |

---

# 4. Cơ chế Reallocation

Mỗi lần thêm hoặc xóa phần tử, chương trình đều phải:

```text
Mảng cũ

↓

Xin vùng nhớ mới

↓

Copy toàn bộ dữ liệu

↓

delete[] mảng cũ

↓

Con trỏ trỏ sang mảng mới
```

Ví dụ:

```text
10 20 30

↓

10 20 30 40

↓

(delete mảng cũ)

↓

Con trỏ cập nhật
```

Điều này làm cho thao tác thêm/xóa trên mảng động thủ công trở nên tốn kém.

---

# 5. Nguy cơ Memory Leak

Nếu quên:

```cpp
delete[] mang;
```

vùng nhớ trên Heap sẽ **không bao giờ được giải phóng**.

Ví dụ:

```cpp
int* mang = new int[100];

// Quên delete[]
```

Mỗi lần chương trình chạy hoặc vòng lặp cấp phát mới, RAM sẽ bị chiếm thêm.

Hiện tượng này gọi là:

> **Memory Leak (Rò rỉ bộ nhớ)**

Nếu xảy ra nhiều lần:

- RAM tăng liên tục.
- Chương trình ngày càng chậm.
- Có thể dẫn đến treo ứng dụng hoặc hết bộ nhớ.

---

# 6. Tại sao ngày nay ít ai dùng `new[]` và `delete[]`?

Chỉ để thêm một phần tử, lập trình viên phải:

1. Xin vùng nhớ mới.
2. Copy toàn bộ dữ liệu.
3. Tự giải phóng vùng nhớ cũ.
4. Cập nhật con trỏ.
5. Đảm bảo không quên `delete[]`.

Quá trình này vừa dài, vừa dễ phát sinh lỗi.

---

# 7. `std::vector` hoạt động như thế nào?

Thực tế, **`std::vector`** cũng sử dụng ý tưởng **Reallocation** ở bên trong (under the hood).

Khác biệt là mọi thao tác quản lý bộ nhớ đã được thư viện chuẩn thực hiện tự động.

Ví dụ:

```cpp
std::vector<int> v;

v.push_back(10);
v.push_back(20);
v.push_back(30);
```

Khi cần mở rộng, `std::vector` sẽ tự:

- Cấp phát vùng nhớ mới.
- Sao chép dữ liệu.
- Giải phóng vùng nhớ cũ.
- Cập nhật con trỏ.

Lập trình viên **không cần** gọi:

```cpp
new
delete
```

hay tự viết vòng lặp sao chép.

---

# Tóm tắt

| Thao tác            | Mảng động thủ công (`new[]`)                                 | `std::vector`                                       |
| ------------------- | ------------------------------------------------------------ | --------------------------------------------------- |
| Cấp phát            | Thủ công                                                     | Tự động                                             |
| Giải phóng          | Thủ công (`delete[]`)                                        | Tự động                                             |
| Thêm phần tử        | Reallocation thủ công                                        | `push_back()`                                       |
| Xóa phần tử         | Reallocation thủ công                                        | `erase()`                                           |
| Nguy cơ Memory Leak | Có                                                           | Không (nếu dùng đúng cách)                          |
| Độ an toàn          | Thấp                                                         | Cao                                                 |
| Khuyến nghị         | Chỉ dùng để học hoặc khi thật sự cần quản lý bộ nhớ mức thấp | Nên sử dụng trong hầu hết các ứng dụng C++ hiện đại |
