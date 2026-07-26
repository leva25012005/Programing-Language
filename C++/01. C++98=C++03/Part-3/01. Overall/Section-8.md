# Lambda Expression trong C++

## Giới thiệu

Trước khi **C++11** ra đời, nếu muốn sắp xếp một mảng theo một quy tắc đặc biệt, bạn phải:

1. Viết một hàm riêng.
2. Đặt nó ở đầu file.
3. Truyền tên hàm đó vào `std::sort`.

Ví dụ:

```cpp
bool giam_dan(int a, int b)
{
    return a > b;
}

std::sort(v.begin(), v.end(), giam_dan);
```

Điều này khiến mã nguồn:

- Bị phân tán.
- Khó đọc.
- Những hàm chỉ dùng đúng một lần vẫn phải khai báo riêng.

---

## Cuộc cách mạng của C++11

**Lambda Expression** (Hàm vô danh) cho phép bạn:

> **Viết một hàm ngay tại vị trí cần sử dụng.**

Không cần đặt tên.
Không cần khai báo trước.

---

# 1. MỨC CƠ BẢN: Giải phẫu một Lambda

Cú pháp đầy đủ:

```cpp
[capture_list] (parameters) -> return_type
{
    body
}
```

Trong đó:

| Thành phần | Ý nghĩa                 |
| ---------- | ----------------------- |
| `[]`       | Capture List (Bắt biến) |
| `()`       | Danh sách tham số       |
| `->`       | Kiểu trả về             |
| `{}`       | Thân hàm                |

---

## Capture List

```cpp
[]
```

Đây là phần đặc biệt nhất của Lambda.

Nếu để trống:

```cpp
[]
```

Lambda gần giống như một hàm thông thường.

---

## Parameters

```cpp
(int a, int b)
```

Hoạt động giống hệt tham số của hàm.

---

## Return Type

```cpp
-> bool
```

Thông thường có thể bỏ qua vì Compiler tự suy luận.

Ví dụ:

```cpp
[](int a, int b)
{
    return a > b;
}
```

Compiler hiểu đây là:

```cpp
[](int a, int b) -> bool
```

---

## Body

Là nơi chứa logic xử lý.

---

# Ví dụ: Sắp xếp giảm dần

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
    std::vector<int> v = {5,2,8,1,9};

    std::sort(v.begin(), v.end(),
        [](int a, int b)
        {
            return a > b;
        });

    for(int x : v)
        std::cout << x << ' ';
}
```

Kết quả:

```
9 8 5 2 1
```

---

# 2. MỨC TRUNG CẤP: Capture List

Điểm mạnh nhất của Lambda là có thể sử dụng biến bên ngoài.

Ví dụ:

```cpp
int x = 10;
```

Một hàm bình thường:

```cpp
void foo()
{
}
```

không thể truy cập `x`.

Nhưng Lambda thì có thể.

---

## Capture theo Copy

```cpp
[x]
```

Lambda tạo một bản sao của `x`.

Ví dụ:

```cpp
int x = 10;

auto f = [x]()
{
    std::cout << x;
};
```

---

## Capture theo Reference

```cpp
[&x]
```

Lambda giữ tham chiếu tới biến gốc.

```cpp
int x = 10;

auto f = [&x]()
{
    x = 100;
};
```

Sau khi gọi:

```cpp
f();
```

Biến ngoài cũng thay đổi:

```cpp
x == 100
```

---

## Capture nhiều biến

```cpp
[x, y]
```

Copy cả hai.

---

```cpp
[&x, &y]
```

Reference cả hai.

---

## Capture toàn bộ

Copy tất cả:

```cpp
[=]
```

Reference tất cả:

```cpp
[&]
```

---

# Ví dụ

```cpp
#include <iostream>

int main()
{
    int x = 10;
    int y = 20;

    auto test = [x, &y]()
    {
        y = 50;

        std::cout
            << x
            << ' '
            << y
            << '\n';
    };

    test();

    std::cout
        << x
        << ' '
        << y;
}
```

Kết quả:

```
10 50
10 50
```

Lưu ý:

- `x` không đổi.
- `y` thay đổi.

---

# 3. MỨC NÂNG CAO: mutable

Nếu Capture theo Copy:

```cpp
[x]
```

thì biến sẽ trở thành:

```cpp
const
```

Ví dụ:

```cpp
int x = 10;

auto f = [x]()
{
    x++;
};
```

Compiler báo lỗi.

---

## mutable

Muốn thay đổi bản sao đó:

```cpp
[x]() mutable
{
    x++;
}
```

Ví dụ:

```cpp
int mang_song = 3;

auto choi_game =
    [mang_song]() mutable
{
    mang_song--;

    std::cout
        << mang_song
        << '\n';
};

choi_game();
choi_game();

std::cout << mang_song;
```

Kết quả:

```
2
1
3
```

Biến gốc không đổi.

---

# Under the Hood

Lambda thực chất không phải là hàm.

Compiler sẽ tạo một class ẩn.

Ví dụ:

```cpp
[x]()
{
    return x + 1;
}
```

gần giống như:

```cpp
class Anonymous
{
private:

    int x;

public:

    Anonymous(int value)
        : x(value)
    {
    }

    int operator()()
    {
        return x + 1;
    }
};
```

Đó là lý do Lambda còn được gọi là:

> **Functor**

---

# 4. MODERN C++ (C++14 trở lên)

## Generic Lambda (C++14)

Có thể dùng `auto`.

```cpp
auto cong =
[](auto a, auto b)
{
    return a + b;
};
```

Ví dụ:

```cpp
std::cout << cong(5,10);
```

Kết quả:

```
15
```

---

Chuỗi:

```cpp
std::cout
<< cong(std::string("A"),"B");
```

Kết quả:

```
AB
```

Hoạt động giống Function Template.

---

## Capture Initializer (C++14)

Có thể tạo biến ngay trong Capture.

```cpp
int x = 10;

auto f =
[z = x + 5]()
{
    std::cout << z;
};
```

Kết quả:

```
15
```

---

Đây cũng là nền tảng để Move Resource.

Ví dụ:

```cpp
std::unique_ptr<int> ptr =
std::make_unique<int>(10);

auto f =
[p = std::move(ptr)]()
{
};
```

---

## Lambda Template (C++20)

Có thể viết Template ngay trong Lambda.

```cpp
auto cong =
[]<typename T>(T a, T b)
{
    return a + b;
};
```

Ví dụ:

```cpp
cong(5,10);
```

Hợp lệ.

---

```cpp
cong(5,3.14);
```

Compiler báo lỗi vì khác kiểu.

---

# So sánh Lambda và Function

| Tiêu chí                    | Function | Lambda |
| --------------------------- | -------- | ------ |
| Có tên                      | Có       | Không  |
| Viết ngay tại chỗ           | Không    | Có     |
| Capture biến ngoài          | Không    | Có     |
| Có thể truyền như đối tượng | Có       | Có     |
| Compiler tạo Functor        | Không    | Có     |

---

# Khi nào nên dùng Lambda?

Rất phù hợp với:

- `std::sort`
- `std::find_if`
- `std::for_each`
- Callback
- Thread
- Event
- GUI
- Async Programming

---

# Ví dụ Comparator

```cpp
std::sort(
    v.begin(),
    v.end(),
    [](int a, int b)
    {
        return a > b;
    });
```

---

# Ví dụ Callback

```cpp
button.onClick(
[]
{
    std::cout
    << "Button clicked";
});
```

---

# Những lưu ý quan trọng

## Tránh lạm dụng `[&]`

Ví dụ:

```cpp
auto f =
[&]()
{
    ...
};
```

Nếu Lambda tồn tại lâu hơn các biến bên ngoài (ví dụ chạy đa luồng), sẽ xuất hiện:

> **Dangling Reference**

đây là nguyên nhân gây crash rất phổ biến.

---

## Ưu tiên Capture rõ ràng

Nên viết:

```cpp
[x]
```

hoặc

```cpp
[&x]
```

Thay vì:

```cpp
[&]
```

---

## Dùng `auto` khi phù hợp

Generic Lambda giúp giảm rất nhiều code.

---

# Tóm tắt

- Lambda được giới thiệu từ **C++11**.
- Cho phép viết hàm ngay tại nơi sử dụng.
- Capture List giúp Lambda truy cập biến bên ngoài.
- `mutable` cho phép sửa bản sao của biến Capture.
- Lambda thực chất là một **Functor** do Compiler tự sinh.
- C++14 bổ sung **Generic Lambda** và **Capture Initializer**.
- C++20 bổ sung **Lambda Template**.
- Lambda là lựa chọn hàng đầu cho các **Comparator**, **Callback**, **Algorithm STL** và lập trình hiện đại.
