# `#define` trong C++

Khác với `template` hay `using` (được xử lý bởi **Compiler**), từ khóa `#define` thuộc về một thế giới hoàn toàn khác:

> **Preprocessor (Tiền xử lý).**

Hãy tưởng tượng **Preprocessor** giống như một anh thư ký sử dụng chức năng **Find & Replace** của Microsoft Word.

Quy trình hoạt động:

```text
Mã nguồn (.cpp)
        │
        ▼
Preprocessor
(#define, #include, #ifdef,...)
        │
        ▼
Mã nguồn đã được thay thế
        │
        ▼
Compiler
        │
        ▼
Mã máy (Machine Code)
```

Điểm quan trọng:

- `#define` **không phải C++ thật sự**.
- Compiler **không hiểu `#define`**.
- Preprocessor sẽ thay thế văn bản trước khi Compiler bắt đầu biên dịch.

---

# 1. Mức cơ bản: Object-like Macro

Đây là cách sử dụng phổ biến nhất của `#define`.

Thường dùng để định nghĩa các hằng số.

## Ví dụ

```cpp
#include <iostream>

#define PI 3.14159
#define SO_LUONG_TOI_DA 100
#define CHAO_HOI "Xin chao the gioi C++"

int main()
{
    double dien_tich = PI * 5 * 5;

    std::cout << CHAO_HOI << '\n';
}
```

---

## Preprocessor nhìn thấy gì?

Dòng:

```cpp
double dien_tich = PI * 5 * 5;
```

được thay thành:

```cpp
double dien_tich = 3.14159 * 5 * 5;
```

Compiler hoàn toàn **không biết** `PI` là gì.

---

## Hạn chế

`#define`

- Không có kiểu dữ liệu.
- Không có phạm vi (Scope).
- Không được Debugger nhận diện.

Ví dụ:

```cpp
#define PI 3.14
```

Compiler chỉ nhìn thấy:

```cpp
3.14
```

---

# 2. Mức trung cấp: Function-like Macro

Macro cũng có thể nhận tham số.

Ví dụ:

```cpp
#define BINH_PHUONG(x) x * x
```

---

# 2.1. Cạm bẫy: Thiếu ngoặc

## Sai

```cpp
#define BINH_PHUONG_LOI(x) x * x
```

Ví dụ:

```cpp
BINH_PHUONG_LOI(2 + 3)
```

Preprocessor thay thành:

```cpp
2 + 3 * 2 + 3
```

Theo thứ tự ưu tiên toán tử:

```text
2 + 6 + 3 = 11
```

Trong khi mong muốn là:

```text
25
```

---

## Đúng

```cpp
#define BINH_PHUONG_CHUAN(x) ((x) * (x))
```

Lúc này:

```cpp
BINH_PHUONG_CHUAN(2 + 3)
```

được thay thành:

```cpp
((2 + 3) * (2 + 3))
```

Kết quả:

```text
25
```

---

## Quy tắc sống còn

Luôn:

- Bọc từng tham số.

```cpp
(x)
```

- Bọc toàn bộ biểu thức.

```cpp
((x) * (x))
```

---

# 2.2. Cạm bẫy: Multiple Evaluation

Đây là lỗi nguy hiểm nhất của Macro.

Ví dụ:

```cpp
#define MAX(a,b) ((a) > (b) ? (a) : (b))
```

Sử dụng:

```cpp
int x = 5;
int y = 10;

int max_val = MAX(x++, y++);
```

Preprocessor biến thành:

```cpp
((x++) > (y++) ? (x++) : (y++))
```

Kết quả:

- `y++` có thể thực hiện hai lần.
- Giá trị biến bị thay đổi ngoài ý muốn.

---

## Quy tắc

Không truyền vào Macro các biểu thức như:

```cpp
x++
++x
--x
foo()
```

---

# 3. Mức nâng cao: Macro Operators

C/C++ cung cấp hai toán tử đặc biệt chỉ dành cho Macro.

---

# 3.1. Stringize Operator (`#`)

Toán tử:

```cpp
#
```

biến tên biến thành chuỗi.

Ví dụ:

```cpp
#define IN_BIEN(bien) \
std::cout << #bien << " = " << bien << '\n';
```

Sử dụng:

```cpp
int diem = 95;

IN_BIEN(diem);
```

Preprocessor sinh ra:

```cpp
std::cout
<< "diem"
<< " = "
<< diem
<< '\n';
```

Kết quả:

```text
diem = 95
```

---

## Khi nào dùng?

Thường dùng trong:

- Logging
- Assert
- Debug

---

# 3.2. Token Pasting (`##`)

Toán tử:

```cpp
##
```

nối hai token lại với nhau.

Ví dụ:

```cpp
#define TAO_BIEN(name,id) int name##id
```

Sử dụng:

```cpp
TAO_BIEN(player_,1) = 100;
```

Preprocessor sinh ra:

```cpp
int player_1 = 100;
```

Tiếp tục:

```cpp
TAO_BIEN(player_,2) = 200;
```

↓

```cpp
int player_2 = 200;
```

---

## Khi nào dùng?

Rất phổ biến trong:

- Game Engine
- Reflection
- Code Generation
- Embedded Systems

---

# 3.3. Variadic Macro

Macro cũng có thể nhận số lượng tham số không giới hạn.

Ví dụ:

```cpp
#define LOG_INFO(...) \
printf("INFO: " __VA_ARGS__)
```

Sử dụng:

```cpp
LOG_INFO("Diem = %d", diem);
```

↓

```cpp
printf("INFO: " "Diem = %d", diem);
```

---

# 4. Modern C++: Cuộc trốn chạy khỏi `#define`

Do `#define` quá nguy hiểm nên Modern C++ khuyến khích thay thế bằng các công cụ an toàn hơn.

---

# So sánh

| Mục đích  | `#define`          | Modern C++                            | Ưu điểm                      |
| --------- | ------------------ | ------------------------------------- | ---------------------------- |
| Hằng số   | `#define PI 3.14`  | `constexpr double PI = 3.14;`         | Có kiểu dữ liệu, Debug được  |
| Macro hàm | `#define MAX(a,b)` | `inline` hoặc `template`              | Không bị Multiple Evaluation |
| Sinh mã   | Macro              | `constexpr`, Template Metaprogramming | An toàn, kiểm tra kiểu       |

---

# Ví dụ thay thế

## Hằng số

Thay vì:

```cpp
#define PI 3.14159
```

nên dùng:

```cpp
constexpr double PI = 3.14159;
```

---

## Hàm

Thay vì:

```cpp
#define SQUARE(x) ((x)*(x))
```

nên dùng:

```cpp
inline int square(int x)
{
    return x * x;
}
```

Hoặc:

```cpp
template<typename T>
inline T square(T x)
{
    return x * x;
}
```

---

# So sánh `#define` và `constexpr`

| `#define`             | `constexpr`     |
| --------------------- | --------------- |
| Không có kiểu dữ liệu | Có kiểu dữ liệu |
| Không Debug được      | Debug được      |
| Không kiểm tra kiểu   | Kiểm tra kiểu   |
| Chỉ thay thế văn bản  | Là C++ thật sự  |
| Dễ sinh lỗi           | An toàn         |

---

# Tổng kết

`#define` là công cụ **thao tác văn bản** của Preprocessor.

Nó rất mạnh nhưng cũng rất nguy hiểm.

Hiện nay chỉ nên dùng trong các trường hợp:

- Header Guard (`#ifndef`)
- Logging Macro
- Stringize (`#`)
- Token Pasting (`##`)
- Variadic Macro

Đối với:

- Hằng số
- Hàm toán học
- Logic chương trình

hãy ưu tiên sử dụng các tính năng của Modern C++ như:

- `const`
- `constexpr`
- `inline`
- `template`

---

# Ghi nhớ nhanh

| Tính năng          | Mục đích                                   |
| ------------------ | ------------------------------------------ |
| `#define PI 3.14`  | Định nghĩa hằng số (không khuyến khích)    |
| `#define MAX(a,b)` | Macro có tham số                           |
| `#`                | Chuyển token thành chuỗi                   |
| `##`               | Ghép hai token thành một                   |
| `__VA_ARGS__`      | Macro nhận số lượng tham số không giới hạn |
| `constexpr`        | Thay thế hầu hết hằng số `#define`         |
| `inline`           | Thay thế Macro hàm                         |
| `template`         | Thay thế Macro tổng quát                   |
