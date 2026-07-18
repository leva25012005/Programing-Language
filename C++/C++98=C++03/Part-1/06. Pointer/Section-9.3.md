# 3. Con trỏ hằng cấp hai (`const int* const* ptr`)

Tuyệt vời! Chúng ta đã hoàn thành hai mục đầu tiên về con trỏ. Bây giờ hãy bước vào phần cuối cùng và cũng là thử thách nhất về tư duy con trỏ:

## Mục 3: Con trỏ hằng cấp hai

```cpp
const int* const* ptr
```

Khi kết hợp từ khóa `const` với con trỏ cấp hai, chúng ta có thể **khóa dữ liệu ở các tầng khác nhau trong bộ nhớ**.

Điều này cực kỳ quan trọng trong:

- Bảo vệ dữ liệu quan trọng.
- Ngăn chặn các hàm con vô tình thay đổi địa chỉ.
- Thiết kế code an toàn hơn.

---

# 1. Các vị trí đặt `const` trong con trỏ cấp hai

Có 3 vị trí phổ biến để đặt `const`, tương ứng với 3 cấp độ bảo vệ:

| Cú pháp           | Cấp độ bảo vệ (bị khóa)  | Ý nghĩa                                                         |
| ----------------- | ------------------------ | --------------------------------------------------------------- |
| `const int** ptr` | Giá trị gốc (`**ptr`)    | Không cho phép sửa giá trị của biến số nguyên cuối cùng         |
| `int* const* ptr` | Con trỏ cấp một (`*ptr`) | Không cho phép thay đổi địa chỉ mà con trỏ cấp một đang trỏ tới |
| `int** const ptr` | Con trỏ cấp hai (`ptr`)  | Không cho phép bản thân con trỏ cấp hai trỏ sang nơi khác       |

---

# 2. Khóa toàn bộ các tầng

Khi kết hợp cả ba:

```cpp
const int* const* const ptr
```

Toàn bộ hệ thống sẽ được bảo vệ:

- Không thể thay đổi giá trị gốc.
- Không thể thay đổi địa chỉ của con trỏ cấp một.
- Không thể thay đổi địa chỉ của con trỏ cấp hai.

Đây là mức bảo vệ cao nhất.

---

# 3. Mã nguồn kiểm tra các cấp độ `const`

```cpp
#include <iostream>

int main() {

    std::cout
        << "--- 3. CON TRỎ HẰNG CẤP HAI (CONST POINTER TO POINTER) ---\n";


    int x = 10;
    int y = 20;


    int* ptr1 = &x;
    int* ptr2 = &y;


    // =========================================================================
    // CẤP ĐỘ 1: const int**
    // Khóa giá trị gốc
    // =========================================================================

    const int** cap1 = &ptr1;


    // ❌ LỖI BIÊN DỊCH
    // Không thể sửa giá trị của biến x thông qua cap1
    //
    // **cap1 = 99;


    // ✅ HỢP LỆ
    // Có thể thay đổi ptr1 trỏ sang vùng nhớ khác

    *cap1 = ptr2;



    // =========================================================================
    // CẤP ĐỘ 2: int* const*
    // Khóa con trỏ cấp một
    // =========================================================================

    int* const* cap2 = &ptr1;


    // ✅ HỢP LỆ
    // Có thể thay đổi giá trị gốc

    **cap2 = 99;


    // ❌ LỖI BIÊN DỊCH
    // Không thể thay đổi địa chỉ của ptr1
    //
    // *cap2 = ptr2;



    // =========================================================================
    // CẤP ĐỘ 3: const int* const*
    // Khóa giá trị gốc + con trỏ cấp một
    // =========================================================================

    const int* const* super_secure = &ptr1;


    // ❌ LỖI
    // Không thể sửa giá trị gốc
    //
    // **super_secure = 50;


    // ❌ LỖI
    // Không thể thay đổi con trỏ cấp một
    //
    // *super_secure = ptr2;


    // ✅ HỢP LỆ
    // Bản thân super_secure vẫn có thể trỏ sang con trỏ khác

    super_secure = &ptr2;



    std::cout
        << "-> Cac quy tac const cap hai da hoat dong chinh xac!\n";


    return 0;
}
```

---

# 4. Bài toán thực tế: Truyền mảng 2 chiều vào hàm

Giả sử ta có một bản đồ game dạng ma trận:

```cpp
int matrix[3][3];
```

Yêu cầu:

- Hàm chỉ được phép đọc dữ liệu.
- Không được sửa giá trị các ô.
- Không được thay đổi cấu trúc các hàng.

---

# 5. So sánh các kiểu khai báo

## Cách 1:

```cpp
int** matrix;
```

❌ Không an toàn.

Vì:

- Có thể thay đổi dữ liệu.

Ví dụ:

```cpp
**matrix = 100;
```

Giá trị trong ma trận bị thay đổi.

---

## Cách 2:

```cpp
const int** matrix;
```

Bảo vệ giá trị:

```cpp
**matrix = 100; // ❌
```

Nhưng chưa bảo vệ được cấu trúc con trỏ.

---

## Cách 3:

```cpp
const int* const* matrix;
```

Đây là lựa chọn an toàn hơn.

Nó bảo vệ:

| Thành phần               | Có thể thay đổi? |
| ------------------------ | ---------------- |
| Giá trị ô dữ liệu        | ❌ Không         |
| Địa chỉ các hàng         | ❌ Không         |
| Bản thân con trỏ tham số | ✅ Có            |

---

# 6. Quy tắc ghi nhớ `const`

Một quy tắc đơn giản:

> `const` sẽ khóa thứ nằm bên trái nó.

---

Ví dụ:

```cpp
const int* ptr;
```

Khóa:

```text
int
```

Không thể:

```cpp
*ptr = 10;
```

---

Ví dụ:

```cpp
int* const ptr;
```

Khóa:

```text
ptr
```

Không thể:

```cpp
ptr = &x;
```

---

Ví dụ:

```cpp
const int* const ptr;
```

Khóa cả:

```text
int
+
ptr
```

---

# 7. Kết luận

Khi làm việc với con trỏ cấp hai, vị trí đặt `const` quyết định tầng nào được bảo vệ.

Đối với bài toán truyền ma trận chỉ đọc:

```cpp
const int* const* matrix
```

là lựa chọn phù hợp hơn vì:

- Không thể sửa dữ liệu gốc.
- Không thể thay đổi các con trỏ cấp một.
- Giúp hàm an toàn hơn khi chỉ có nhiệm vụ hiển thị dữ liệu.
