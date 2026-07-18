#Con trỏ hàm nâng cao, `using` / `typedef` và Lambda Expression

Trong C++,
con trỏ
hàm(Function Pointer) là một công cụ mạnh giúp truyền hàm như một tham số,
tạo Callback hoặc xây dựng các hệ thống linh hoạt.

    Tuy nhiên,
    cú pháp của con trỏ hàm truyền thống khá dài và dễ viết sai.

    Ví dụ :

```cpp
int (*phep_toan)(int, int);
```

    Để giải quyết vấn đề này,
    C++ cung cấp hai công cụ :

    - `typedef` (Cách cũ) - `using` (C++ 11 - Khuyến nghị)

        Ngoài ra,
    C++ 11 còn bổ sung **Lambda Expression **,
    cho phép định nghĩa một hàm ngay tại nơi cần sử dụng mà không cần khai báo
    trước
        .

    -- -

# 1. `typedef` - Đặt bí danh cho kiểu dữ liệu

`typedef` giúp đặt một tên mới cho một kiểu dữ liệu đã tồn tại.

    Đối với con trỏ hàm :

```cpp
typedef int (*PhepToanCu)(int, int);
```

    Nghĩa là :

    > `PhepToanCu` là một kiểu dữ liệu đại diện cho con trỏ tới hàm nhận
          hai `int` và trả về `int`.

      Thay vì phải viết :

```cpp
int (*ptr)(int, int);
```

    ta có thể viết :

```cpp
PhepToanCu ptr;
```

Code ngắn gọn hơn rất nhiều.

---

# 2. `using` (C ++ 11)

Từ C++11 trở đi, `using` được khuyến nghị thay thế `typedef`.

Cú pháp:

```cpp
using TenBiDanh = KieuDuLieu;
```

    Ví dụ :

```cpp
using PhepToanMoi = int(*)(int, int);

```

    Ý nghĩa hoàn toàn giống :

```cpp typedef int (*PhepToanCu)(int, int);

```

    -- -

    ##So sánh

    ## # `typedef`

```cpp
typedef int (*PhepToan)(int, int);

```

    ## # `using`

```cpp
using PhepToan = int(*)(int, int);

```

`using` có cú pháp gần giống phép gán nên dễ đọc hơn.

    -- -

#So sánh `typedef` và `using`

    | `typedef` | `using` | | -- -- -- -- -- - | -- -- -- -- - | | Có từ C |
    Có từ C++ 11 | | Cú pháp khó đọc | Cú pháp rõ ràng | |
    Khó dùng với Template | Hỗ trợ Template Alias | | Vẫn được hỗ trợ |
    Khuyến nghị sử dụng |

    -- -

# 3. Ví dụ hoàn chỉnh

```cpp
#include <iostream>

       //==============================
       // typedef
       //==============================

       typedef int (*PhepToanCu)(int, int);

//==============================
// using
//==============================

using PhepToanMoi = int (*)(int, int);

//==============================

int tinhTong(int a, int b) { return a + b; }

void thucThi(int x, int y, PhepToanMoi phep_toan) {
  std::cout << "Ket qua: " << phep_toan(x, y) << '\n';
}

int main() {
  PhepToanMoi ptr = tinhTong;

  std::cout << ptr(10, 5);
}
```

    Kết quả :

```text 15

```

    -- -

# 4. Lambda Expression

    Lambda là **một hàm không
    tên(Anonymous Function) *
    *.

     Bạn có thể định nghĩa ngay tại nơi sử dụng.

     Ví dụ :

```cpp
[](int a, int b) {
  return a + b;
}
```

    Lambda này tương đương với :

```cpp int
    cong(int a, int b) {
  return a + b;
}
```

    Nhưng không cần đặt tên
        .

    -- -

#Cú pháp tổng quát

```cpp
[capture](parameter)
        ->return_type {
  body
}
```

    Trong đó :

    |
    Thành phần | Ý nghĩa | | -- -- -- -- -- -- | -- -- -- -- -- | | `[]` |
    Capture variables | | `()` | Danh sách tham số |
    | `->` | Kiểu trả về(có thể bỏ nếu compiler suy luận được) | | `{}

` | Thân hàm |

    -- -

#Ví dụ đơn giản

```cpp auto tinhNhan = [](int a, int b) { return a * b; };

```

    Gọi :

```cpp
std::cout
    << tinhNhan(4, 5);
```

    Kết quả

```text 20

```

    -- -

#Lambda có kiểu dữ liệu gì ?

    Lambda có một kiểu dữ liệu đặc biệt do compiler tự sinh.

    Do đó gần như luôn dùng :

```cpp
auto
```

    Ví dụ :

```cpp
auto hello = [] { std::cout << "Hello"; };

```

    -- -

# 5. Truyền Lambda làm Callback

    Một ưu điểm lớn của Lambda là truyền trực tiếp vào hàm.

    Ví dụ :

```cpp
thucThi(20, 4, [](int a, int b) { return a / b; });

```

    Không cần viết :

```cpp
int chia(int a, int b) {
  return a / b;
}
```

    Giúp code ngắn gọn hơn rất nhiều.

    -- -

#Luồng thực thi

```text
    thucThi()

        │
        ▼

    Callback

        │
        ▼

    Lambda

        │
        ▼

    return a
    / b
```

    -- -

#Ví dụ hoàn chỉnh

```cpp
#include <iostream>

    using PhepToan = int(*)(int, int);

void thucThi(int x, int y, PhepToan f) { std::cout << f(x, y) << '\n'; }

int main() {
  auto nhan = [](int a, int b) { return a * b; };

  std::cout << nhan(4, 5) << '\n';

  thucThi(20, 4, [](int a, int b) { return a / b; });
}
```

    Kết quả :

```text 20 5

```

    -- -

#Khi nào nên dùng Lambda ?

    Lambda rất phù hợp khi :

    -Làm việc với
    STL(`sort`, `for_each`, `find_if`, ...)
        .-
    Truyền Callback.- Xử lý sự kiện(GUI, Qt, WinForms, ...).-
    Viết các hàm ngắn chỉ sử dụng một lần.-
    Tránh tạo quá nhiều hàm nhỏ trong chương trình
        .

    -- -

#Khi nào không nên dùng Lambda ?

    Không nên dùng Lambda nếu :

    -Hàm quá dài.-
    Hàm được sử dụng ở nhiều nơi.-
    Logic phức tạp cần tái sử dụng.

    Trong các trường hợp đó,
    nên tách thành một hàm riêng để dễ bảo trì.

        -- -

#Tóm tắt

        | Thành phần | Mục đích | | -- -- -- -- -- -- | -- -- -- -- -- |
        | Function Pointer | Lưu địa chỉ của hàm |
        | `typedef` | Đặt bí danh cho kiểu dữ liệu(cách cũ) |
        | `using` | Đặt bí danh cho kiểu dữ liệu(C++ 11, khuyến nghị) |
        | Lambda Expression | Tạo hàm ẩn danh ngay tại nơi sử dụng |
        | `auto` | Compiler tự suy luận kiểu của Lambda |

        > **Kết luận : ** > Trong C++ hiện đại,
    nên ưu tiên sử dụng **`using`**thay cho `typedef` để khai báo bí danh kiểu
    dữ liệu vì cú pháp rõ ràng và dễ mở rộng
        .Đối với các hàm ngắn hoặc Callback chỉ sử dụng một lần,
    **Lambda Expression **là lựa chọn tối ưu nhờ tính ngắn gọn,
    dễ đọc và tích hợp tốt với STL.
