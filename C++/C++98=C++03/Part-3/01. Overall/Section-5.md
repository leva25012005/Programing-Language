# `#ifndef` trong C++

Tiếp tục câu chuyện về **Preprocessor (Tiền xử lý)** — "anh thư ký" sử dụng công cụ **Find & Replace**.

Nếu:

- `#define` là lệnh yêu cầu anh thư ký **thay thế văn bản**,
- thì `#ifndef` (_If Not Defined_) chính là **cửa an ninh** giúp anh thư ký kiểm tra xem một tên đã từng được định nghĩa hay chưa.

---

# 1. Mức cơ bản: Header Guard

## Vấn đề: Multiple Inclusion

Trong C++, câu lệnh:

```cpp
#include "File.h"
```

không hề "liên kết" file theo cách thông minh.

Preprocessor chỉ đơn giản:

```text
Copy nội dung File.h
        ↓
Paste vào vị trí #include
```

Nếu cùng một file bị `#include` nhiều lần thì nội dung của nó cũng bị copy nhiều lần.

---

## Ví dụ

Giả sử có cấu trúc:

```text
ToanHoc.h
```

↓

```text
VatLy.h
```

```cpp
#include "ToanHoc.h"
```

↓

```text
main.cpp
```

```cpp
#include "ToanHoc.h"
#include "VatLy.h"
```

---

Khi Preprocessor hoạt động:

```text
main.cpp

↓

Copy ToanHoc.h

↓

Copy VatLy.h

↓

VatLy.h lại copy ToanHoc.h
```

Kết quả:

```text
ToanHoc.h xuất hiện 2 lần
```

Ví dụ:

```cpp
struct ToaDo
{
    int x;
    int y;
};
```

sẽ xuất hiện hai bản giống hệt nhau.

Compiler báo lỗi:

```text
Redefinition of 'ToaDo'
```

---

# Giải pháp: Header Guard

Bọc toàn bộ file Header bằng:

```cpp
#ifndef TOAN_HOC_H
#define TOAN_HOC_H

struct ToaDo
{
    int x;
    int y;
};

#endif
```

---

## Cách hoạt động

### Lần đầu

Preprocessor kiểm tra:

```cpp
#ifndef TOAN_HOC_H
```

↓

`TOAN_HOC_H` chưa tồn tại

↓

Cho phép đi tiếp

↓

```cpp
#define TOAN_HOC_H
```

↓

Đánh dấu:

```text
TOAN_HOC_H đã tồn tại
```

↓

Copy toàn bộ code.

---

### Lần thứ hai

Lại gặp:

```cpp
#ifndef TOAN_HOC_H
```

Nhưng lần này:

```text
TOAN_HOC_H đã được định nghĩa
```

↓

Điều kiện sai.

↓

Preprocessor bỏ qua toàn bộ đoạn:

```cpp
...
#endif
```

Không còn lỗi định nghĩa trùng.

---

# 2. Mức trung cấp: Conditional Compilation

Khác với:

```cpp
if (...) { }
```

là chạy khi chương trình đang hoạt động (**Runtime**),

thì:

```cpp
#ifndef
#ifdef
#if
```

được xử lý ngay khi **Compile-time**.

Chúng quyết định:

- Đoạn code có được biên dịch hay không.
- Nếu không thì bị xóa hoàn toàn khỏi chương trình.

---

## Ví dụ: Giá trị mặc định

Giả sử thư viện của bạn cho phép người dùng tự chọn kích thước Buffer.

Nếu họ không khai báo:

```cpp
#define BUFFER_SIZE 2048
```

thì thư viện sẽ tự cấp giá trị mặc định.

```cpp
#ifndef BUFFER_SIZE

#define BUFFER_SIZE 512

#pragma message(
"Canh bao: Dang dung BUFFER_SIZE mac dinh."
)

#endif

int buffer[BUFFER_SIZE];
```

---

Ý nghĩa:

Nếu:

```cpp
BUFFER_SIZE
```

chưa tồn tại

↓

thì tạo:

```cpp
BUFFER_SIZE = 512
```

Ngược lại, nếu người dùng đã định nghĩa thì thư viện sẽ không ghi đè.

---

# 3. Mức nâng cao: Cross-Platform

Một chương trình có thể cần chạy trên:

- Windows
- Linux
- macOS

Các Compiler sẽ tự động định nghĩa những Macro đặc biệt.

Ví dụ:

```text
_WIN32
```

khi biên dịch trên Windows.

---

## Ví dụ

```cpp
#include <iostream>

int main()
{
#ifdef _WIN32

    std::cout
        << "Dang chay tren Windows\n";

#endif

#ifndef _WIN32

    std::cout
        << "Dang chay tren Linux hoac macOS\n";

#endif
}
```

---

### Khi biên dịch trên Windows

Preprocessor giữ lại:

```cpp
std::cout
<< "Dang chay tren Windows";
```

và xóa toàn bộ phần Linux/macOS.

---

### Khi biên dịch trên Linux

Ngược lại:

```cpp
std::cout
<< "Dang chay tren Linux hoac macOS";
```

được giữ lại.

Phần Windows biến mất hoàn toàn.

---

## Lợi ích

Một file nguồn:

```text
Game.cpp
```

có thể sinh ra:

- Windows (.exe)
- Linux
- macOS

mà không cần sửa mã nguồn.

---

# 4. Modern C++: `#pragma once`

Header Guard hoạt động rất tốt nhưng có một nhược điểm:

Bạn phải tự đặt tên.

Ví dụ:

```cpp
#ifndef MATH_H
```

Nếu một thư viện khác cũng dùng:

```cpp
#ifndef MATH_H
```

thì hai Header Guard sẽ xung đột.

---

## Giải pháp hiện đại

Ngày nay chỉ cần:

```cpp
#pragma once

struct ToaDo
{
    int x;
    int y;
};
```

---

## `#pragma once` hoạt động như thế nào?

Nó nói với Compiler:

> "Nếu file này đã được include một lần thì bỏ qua mọi lần include tiếp theo."

Không cần:

```cpp
#ifndef
#define
#endif
```

---

## Ưu điểm

- Không cần đặt tên Header Guard.
- Không lo trùng tên Macro.
- Code ngắn gọn.
- Compiler có thể tối ưu tốc độ biên dịch.

---

## Hạn chế

`#pragma once`

không nằm trong chuẩn chính thức của C++.

Tuy nhiên:

- GCC ✅
- Clang ✅
- MSVC ✅

đều hỗ trợ hoàn toàn.

Ngày nay nó gần như trở thành **tiêu chuẩn công nghiệp**.

---

# So sánh Header Guard và `#pragma once`

| Header Guard        | `#pragma once`                       |
| ------------------- | ------------------------------------ |
| Chuẩn C/C++         | Không nằm trong chuẩn                |
| Cần đặt tên Macro   | Không cần                            |
| Dài hơn             | Chỉ một dòng                         |
| Có thể bị trùng tên | Không bị                             |
| Hỗ trợ mọi Compiler | Hầu hết Compiler hiện đại đều hỗ trợ |

---

# Khi nào dùng?

## Header Guard

```cpp
#ifndef MY_HEADER_H
#define MY_HEADER_H

...

#endif
```

- Muốn tuân thủ chuẩn C/C++ tuyệt đối.
- Cần hỗ trợ các Compiler rất cũ.

---

## `#pragma once`

```cpp
#pragma once
```

- Dự án Modern C++.
- Visual Studio.
- GCC.
- Clang.
- CMake.

Đây là lựa chọn phổ biến trong hầu hết các dự án hiện đại.

---

# Tổng kết

`#ifndef` là công cụ kiểm tra:

> **"Nếu tên này chưa được định nghĩa..."**

Ứng dụng quan trọng nhất:

- Header Guard.
- Conditional Compilation.
- Thiết lập giá trị mặc định.
- Cross-platform.

Trong Modern C++, phần lớn lập trình viên thay thế Header Guard bằng:

```cpp
#pragma once
```

để mã nguồn ngắn gọn và dễ bảo trì hơn.

---

# Ghi nhớ nhanh

| Chỉ thị                 | Ý nghĩa                         |
| ----------------------- | ------------------------------- |
| `#define`               | Định nghĩa Macro                |
| `#ifndef`               | Nếu chưa được định nghĩa        |
| `#ifdef`                | Nếu đã được định nghĩa          |
| `#endif`                | Kết thúc khối kiểm tra          |
| `#pragma once`          | Chỉ include file Header một lần |
| Header Guard            | Chống include nhiều lần         |
| Conditional Compilation | Biên dịch có điều kiện          |
| `_WIN32`                | Macro nhận biết Windows         |
