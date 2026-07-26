# Template trong C++ (Toàn diện)

Ở phần trước, chúng ta đã tìm hiểu những khái niệm cơ bản về **Template**. Lần này, chúng ta sẽ đi sâu hơn vào bản chất và những kỹ thuật nâng cao của Template trong C++.

> **Điều quan trọng cần nhớ:**  
> Template không chỉ đơn giản là "khuôn mẫu". Trong C++, hệ thống Template là **Turing-complete**, nghĩa là bạn hoàn toàn có thể viết một chương trình chạy **ngay trong quá trình biên dịch (Compile-time)**, trước khi chương trình thực sự được tạo thành file `.exe`.

---

# 1. Bản chất của Template

## Template không phải là Code

Một sự thật quan trọng:

> Đoạn mã Template bạn viết **không tồn tại trong file thực thi**.

Nó chỉ là một **bản thiết kế (Blueprint)**.

Khi Compiler gặp lời gọi:

```cpp
tim_max<int>(5, 10);
```

Compiler sẽ:

1. Sao chép bản thiết kế.
2. Thay `T` thành `int`.
3. Sinh ra một hàm mới.
4. Biên dịch hàm đó thành mã máy.

Quá trình này gọi là:

> **Template Instantiation (Khởi tạo khuôn mẫu)**

---

## Ví dụ

```cpp
template <typename T>
T tim_max(T a, T b)
{
    return a > b ? a : b;
}

int main()
{
    tim_max(5, 10);
    tim_max(5.5, 2.2);
}
```

Compiler sẽ âm thầm sinh ra:

```cpp
int tim_max(int a, int b)
{
    return a > b ? a : b;
}
```

và

```cpp
double tim_max(double a, double b)
{
    return a > b ? a : b;
}
```

Nếu bạn **không bao giờ gọi** Template thì Compiler cũng **không sinh ra bất kỳ hàm nào**.

---

# 2. Chuyên biệt hóa Template (Template Specialization)

Đôi khi khuôn mẫu chung không phù hợp với mọi kiểu dữ liệu.

Lúc này ta dùng **Specialization**.

Có hai loại:

- Full Specialization
- Partial Specialization

---

## 2.1. Full Specialization

Áp dụng cho **một kiểu dữ liệu cụ thể**.

Ví dụ:

```cpp
template<>
class KhoChua<char>
{
    ...
};
```

Compiler sẽ dùng phiên bản này thay vì phiên bản tổng quát.

---

## 2.2. Partial Specialization

Đây là kỹ thuật mạnh hơn.

Bạn **không cố định kiểu dữ liệu**, mà chỉ thu hẹp phạm vi.

Ví dụ:

```cpp
template<typename T>
class KhoChua
{
public:

    void thong_bao()
    {
        std::cout
        << "Kho chua gia tri binh thuong\n";
    }
};
```

---

Nếu `T` là con trỏ:

```cpp
template<typename T>
class KhoChua<T*>
{
public:

    void thong_bao()
    {
        std::cout
        << "Kho chua con tro\n";
    }
};
```

---

## Ví dụ

```cpp
KhoChua<int> a;
```

↓

```
Kho chua gia tri binh thuong
```

---

```cpp
KhoChua<int*> b;
```

↓

```
Kho chua con tro
```

---

> **Lưu ý**

Partial Specialization:

- Chỉ áp dụng cho **Class Template**
- Không áp dụng cho **Function Template**

---

# 3. Template Metaprogramming (TMP)

Đây là nơi Template trở thành "ma thuật".

Ý tưởng:

> Bắt Compiler làm việc thay Runtime.

Ví dụ:

Tính:

```
5!
```

ngay khi đang Compile.

---

## Ví dụ

```cpp
template<int N>
struct GiaiThua
{
    enum
    {
        gia_tri =
        N * GiaiThua<N-1>::gia_tri
    };
};
```

Điểm dừng:

```cpp
template<>
struct GiaiThua<0>
{
    enum
    {
        gia_tri = 1
    };
};
```

Sử dụng:

```cpp
std::cout
<< GiaiThua<5>::gia_tri;
```

---

Compiler sẽ tính sẵn:

```
5!
=
120
```

và ghi trực tiếp số:

```
120
```

vào file thực thi.

Runtime không cần tính toán nữa.

Độ phức tạp gần như:

```
O(0)
```

---

## Ngày nay

Modern C++ có:

```cpp
constexpr
```

để thực hiện Compile-time dễ đọc hơn.

Tuy nhiên:

Template Metaprogramming vẫn là nền tảng của rất nhiều thư viện lớn như:

- STL
- Boost
- Eigen

---

# 4. SFINAE và Type Traits

## 4.1. SFINAE

Tên đầy đủ:

> **Substitution Failure Is Not An Error**

Ý tưởng:

Compiler thử thay kiểu dữ liệu vào Template.

Nếu:

```
Thay vào tạo ra code vô nghĩa
```

Compiler **không báo lỗi**.

Nó chỉ đơn giản:

```
Loại bỏ Template đó
```

và thử Template khác.

---

## 4.2. Type Traits

Thư viện:

```cpp
#include <type_traits>
```

cho phép kiểm tra đặc điểm của kiểu dữ liệu.

Ví dụ:

```cpp
std::is_class_v<T>
```

Kiểm tra:

```
Có phải Class?
```

---

```cpp
std::is_arithmetic_v<T>
```

Kiểm tra:

```
Có phải kiểu số?
```

---

## Ví dụ

```cpp
template<typename T>

std::enable_if_t<
std::is_class_v<T>>

in_do_dai(T x)
{
    std::cout
    << x.size();
}
```

Phiên bản này chỉ tồn tại nếu:

```
T là Class
```

---

Phiên bản khác:

```cpp
template<typename T>

std::enable_if_t<
std::is_arithmetic_v<T>>

in_do_dai(T)
{
    std::cout
    << "Khong co chieu dai";
}
```

---

Sử dụng:

```cpp
std::string s = "Hello";

in_do_dai(s);
```

↓

```
5
```

---

```cpp
int x = 100;

in_do_dai(x);
```

↓

```
Khong co chieu dai
```

---

# 5. C++20 Concepts

SFINAE rất mạnh.

Nhưng...

Code cực kỳ khó đọc.

Ví dụ:

```cpp
std::enable_if_t<
std::is_class_v<T>>
```

dài và khó hiểu.

---

## Concepts

C++20 giới thiệu:

```cpp
concept
```

để thay thế.

---

## Ví dụ

```cpp
template<typename T>

concept TinhCongDuoc =
requires(T a, T b)
{
    a + b;
}
&&
std::is_arithmetic_v<T>;
```

Concept trên quy định:

- Phải cộng được.
- Phải là kiểu số.

---

Sau đó:

```cpp
template<TinhCongDuoc T>

T cong_hai_so(T a, T b)
{
    return a + b;
}
```

---

Sử dụng:

```cpp
cong_hai_so(5,10);
```

↓

Hợp lệ.

---

```cpp
cong_hai_so("Hello","World");
```

↓

Compiler báo lỗi ngay lập tức vì:

```
string
```

không thỏa mãn Concept.

---

# So sánh SFINAE và Concepts

| Tiêu chí             | SFINAE                       | Concepts    |
| -------------------- | ---------------------------- | ----------- |
| Phiên bản C++        | C++11                        | C++20       |
| Dễ đọc               | ❌                           | ✅          |
| Thông báo lỗi        | Khó hiểu                     | Rõ ràng     |
| Viết Template        | Phức tạp                     | Đơn giản    |
| Khuyến nghị hiện nay | Chỉ dùng khi cần tương thích | Nên sử dụng |

---

# Tổng kết

Template là một trong những tính năng mạnh nhất của C++.

Qua các phiên bản C++, Template đã phát triển từ một công cụ sinh mã đơn giản thành cả một hệ thống lập trình lúc biên dịch.

Những kiến thức quan trọng cần nhớ:

| Chủ đề                   | Ý nghĩa                                         |
| ------------------------ | ----------------------------------------------- |
| Template Instantiation   | Compiler sinh mã từ khuôn mẫu                   |
| Full Specialization      | Chuyên biệt hóa hoàn toàn                       |
| Partial Specialization   | Chuyên biệt hóa một phần (chỉ Class Template)   |
| Template Metaprogramming | Tính toán lúc Compile-time                      |
| SFINAE                   | Loại bỏ Template không phù hợp thay vì báo lỗi  |
| Type Traits              | Kiểm tra đặc tính kiểu dữ liệu                  |
| Concepts                 | Cách hiện đại để ràng buộc Template trong C++20 |
