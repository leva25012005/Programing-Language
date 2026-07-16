# `void*` (Con trỏ vạn năng) và `std::function` (Con trỏ hàm Generic)

Trong C++, có hai công cụ rất quan trọng thường được sử dụng khi làm việc với các API tổng quát hoặc lập trình hiện đại:

- **`void*`** – Con trỏ vạn năng (Generic Pointer)
- **`std::function`** – Bộ bao (Wrapper) cho mọi đối tượng có thể gọi được (Callable Object)

Mặc dù cả hai đều hướng đến tính "tổng quát", nhưng chúng phục vụ hai mục đích hoàn toàn khác nhau:

- `void*` giúp lưu địa chỉ của **mọi kiểu dữ liệu**.
- `std::function` giúp lưu **mọi loại hàm**.

---

# 1. `void*` (Con trỏ vạn năng)

`void*` là một con trỏ **không có kiểu dữ liệu xác định**.

Nó có thể lưu địa chỉ của bất kỳ kiểu dữ liệu nào:

- `int`
- `double`
- `char`
- `struct`
- `class`
- ...

Ví dụ:

```cpp
int x = 10;

void* ptr = &x;
```

Hoặc:

```cpp
double y = 3.14;

ptr = &y;
```

Điều này hoàn toàn hợp lệ.

---

## Tại sao gọi là "vạn năng"?

Thông thường:

```cpp
int*
```

chỉ trỏ được tới:

```cpp
int
```

Trong khi:

```cpp
void*
```

có thể trỏ tới mọi kiểu dữ liệu.

Ví dụ:

```cpp
int a = 5;
double b = 8.5;
char c = 'A';

void* ptr;

ptr = &a;
ptr = &b;
ptr = &c;
```

Không có lỗi biên dịch.

---

# Tại sao không thể giải tham chiếu trực tiếp?

Ví dụ:

```cpp
void* ptr = &a;

std::cout << *ptr;
```

❌ Sai.

Compiler báo lỗi.

---

## Nguyên nhân

Để đọc dữ liệu:

```cpp
*ptr
```

Compiler phải biết:

- Đọc bao nhiêu byte?
- Kiểu dữ liệu là gì?
- Giải mã dữ liệu như thế nào?

Ví dụ:

| Kiểu     | Kích thước (thường gặp) |
| -------- | ----------------------- |
| `char`   | 1 byte                  |
| `int`    | 4 byte                  |
| `double` | 8 byte                  |

`void*` không mang thông tin kiểu dữ liệu nên compiler không biết phải đọc bao nhiêu byte.

---

# Ép kiểu trước khi sử dụng

Muốn dùng `void*`, phải ép kiểu về đúng kiểu dữ liệu.

Ví dụ:

```cpp
int x = 2026;

void* ptr = &x;

int* p = static_cast<int*>(ptr);

std::cout << *p;
```

Kết quả:

```text
2026
```

---

## Ví dụ với `double`

```cpp
double pi = 99.5;

void* ptr = &pi;

double* p = static_cast<double*>(ptr);

std::cout << *p;
```

Kết quả:

```text
99.5
```

---

# Ví dụ hoàn chỉnh

```cpp
int ma_so = 2026;
double ti_le = 99.5;

void* ptr = &ma_so;

int* p1 = static_cast<int*>(ptr);

std::cout << *p1 << '\n';

ptr = &ti_le;

double* p2 = static_cast<double*>(ptr);

std::cout << *p2 << '\n';
```

Kết quả:

```text
2026
99.5
```

---

# Lưu ý quan trọng

`static_cast`

không kiểm tra xem việc ép kiểu có đúng hay không.

Ví dụ:

```cpp
void* ptr = &ti_le;

int* p = static_cast<int*>(ptr);
```

Compiler **không báo lỗi**.

Nhưng:

```cpp
std::cout << *p;
```

là **Undefined Behavior**.

---

# Điều gì xảy ra nếu ép sai kiểu?

Giả sử:

```cpp
double x = 99.5;

void* ptr = &x;

int* p = static_cast<int*>(ptr);

std::cout << *p;
```

`double`

được lưu bằng:

```text
8 byte
```

Trong khi:

```cpp
int*
```

chỉ đọc:

```text
4 byte
```

CPU sẽ đọc 4 byte đầu tiên của một giá trị `double` rồi diễn giải chúng như một `int`.

Kết quả:

- Có thể là số rất lớn.
- Có thể là số âm.
- Có thể thay đổi giữa các lần chạy.
- Không có ý nghĩa.

Đây chính là:

```text
Undefined Behavior
```

---

# Compiler có ngăn được không?

Không.

Ví dụ:

```cpp
void* ptr = &x;

int* p = static_cast<int*>(ptr);
```

Compiler cho phép.

Lý do:

`void*`

được thiết kế để có thể ép về mọi kiểu con trỏ.

Compiler không biết địa chỉ đó thật sự chứa kiểu gì.

Do đó, trách nhiệm thuộc về lập trình viên.

---

# Tóm tắt về `void*`

| Đặc điểm                                 | Giá trị |
| ---------------------------------------- | ------- |
| Trỏ mọi kiểu dữ liệu                     | ✔       |
| Có thể giải tham chiếu trực tiếp         | ❌      |
| Phải ép kiểu                             | ✔       |
| Compiler kiểm tra đúng sai khi ép kiểu   | ❌      |
| Có thể gây Undefined Behavior nếu ép sai | ✔       |

---

# 2. `std::function`

Con trỏ hàm truyền thống:

```cpp
int (*f)(int,int);
```

chỉ lưu được:

- Hàm tự do (Free Function)

Không lưu được:

- Lambda có Capture.
- Functor.
- Hàm thành viên.
- Đối tượng Callable.

---

## `std::function`

C++11 giới thiệu:

```cpp
std::function
```

để giải quyết vấn đề trên.

Nó có thể lưu mọi đối tượng có thể gọi được (**Callable Object**).

---

# Cú pháp

```cpp
std::function<
    ReturnType(ParameterTypes...)
>
```

Ví dụ:

```cpp
std::function<int(int,int)>
```

nghĩa là:

- Nhận hai `int`
- Trả về `int`

---

# Ví dụ

```cpp
#include <functional>

std::function<int(int,int)> f;
```

---

# Hàm thông thường

```cpp
int tinhTong(int a,int b)
{
    return a+b;
}
```

Gán:

```cpp
f = tinhTong;
```

Gọi:

```cpp
std::cout << f(10,5);
```

Kết quả:

```text
15
```

---

# Lambda

```cpp
std::function<int(int,int)> f =
[](int a,int b)
{
    return a*b;
};
```

Kết quả:

```text
50
```

---

# Callback

```cpp
void thucThiCallback(
    int x,
    int y,
    std::function<int(int,int)> f)
{
    std::cout
        << f(x,y)
        << '\n';
}
```

Có thể truyền:

```cpp
thucThiCallback(
    10,
    5,
    tinhTong
);
```

Hoặc:

```cpp
thucThiCallback(
    10,
    5,
    [](int a,int b)
    {
        return a*b;
    });
```

Đều hợp lệ.

---

# Ví dụ hoàn chỉnh

```cpp
#include <iostream>
#include <functional>

int tinhTong(int a,int b)
{
    return a+b;
}

void thucThiCallback(
    int x,
    int y,
    std::function<int(int,int)> f)
{
    std::cout
        << f(x,y)
        << '\n';
}

int main()
{
    thucThiCallback(
        10,
        5,
        tinhTong);

    thucThiCallback(
        10,
        5,
        [](int a,int b)
        {
            return a*b;
        });
}
```

Kết quả:

```text
15
50
```

---

# So sánh Function Pointer và `std::function`

| Function Pointer                       | `std::function`                          |
| -------------------------------------- | ---------------------------------------- |
| Chỉ lưu Free Function                  | Lưu mọi Callable                         |
| Không hỗ trợ Lambda có Capture         | Hỗ trợ                                   |
| Không hỗ trợ Functor                   | Hỗ trợ                                   |
| Không hỗ trợ Member Function trực tiếp | Hỗ trợ (kết hợp `std::bind` hoặc Lambda) |
| Nhanh hơn một chút                     | Linh hoạt hơn                            |
| Cú pháp khó                            | Cú pháp rõ ràng                          |

---

# Khi nào dùng?

## Dùng `void*`

Khi:

- Thiết kế API tổng quát.
- Làm việc với thư viện C.
- Hệ điều hành.
- Driver.
- Embedded.
- Các API mức thấp (Low-level).

Trong C++ hiện đại, `void*` ít được sử dụng hơn vì có nhiều lựa chọn an toàn hơn như Template hoặc `std::variant`.

---

## Dùng `std::function`

Khi:

- Callback.
- Event System.
- GUI.
- Thread.
- Async Programming.
- STL.
- Lambda.
- Dependency Injection.

Đây là lựa chọn phổ biến trong C++ hiện đại.

---

# Kết luận

- **`void*`** là một **con trỏ vạn năng**, có thể lưu địa chỉ của mọi kiểu dữ liệu nhưng không mang thông tin về kiểu. Vì vậy, trước khi sử dụng bắt buộc phải ép kiểu về đúng loại con trỏ. Nếu ép sai, chương trình vẫn biên dịch nhưng sẽ dẫn đến **Undefined Behavior** và compiler không thể ngăn chặn lỗi này.

- **`std::function`** là một **bộ bao cho mọi đối tượng có thể gọi được (Callable Object)**. So với con trỏ hàm truyền thống, `std::function` linh hoạt hơn rất nhiều vì có thể lưu hàm thông thường, Lambda (kể cả có Capture), Functor và nhiều loại Callable khác. Đây là lựa chọn được khuyến nghị trong C++ hiện đại khi xây dựng Callback hoặc các hệ thống hướng sự kiện.
