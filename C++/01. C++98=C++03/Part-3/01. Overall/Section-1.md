# Các Từ Khóa Quan Trọng Trong C++

---

# 1. Nhóm Đặt Tên Chống Trùng Lặp (Namespace)

## `namespace` (Không gian tên)

### Ý nghĩa

C++ được thiết kế cho các dự án lớn, nơi nhiều nhóm lập trình viên cùng phát triển một hệ thống.

Giả sử:

- Team A viết một hàm `draw()` dùng để vẽ hình 2D.
- Team B cũng viết một hàm `draw()` dùng để vẽ hình 3D.

Nếu gộp hai đoạn mã lại, chương trình sẽ báo lỗi **trùng tên hàm**.

`namespace` được tạo ra để giải quyết vấn đề này bằng cách đóng gói các hàm, biến hoặc lớp vào những **không gian tên (namespace)** khác nhau.

---

## Ví dụ

```cpp
namespace TeamA
{
    void draw()
    {
        // Vẽ 2D
    }
}

namespace TeamB
{
    void draw()
    {
        // Vẽ 3D
    }
}

int main()
{
    TeamA::draw();
    TeamB::draw();
}
```

---

## `std` là gì?

Khi viết:

```cpp
std::cout
```

ta đang sử dụng:

- `cout`
- nằm trong namespace
- có tên là `std` (**Standard Library**).

Ví dụ:

```cpp
std::cout << "Hello World!";
```

---

## Lợi ích của `namespace`

- Tránh trùng tên hàm.
- Tránh trùng tên biến.
- Tránh trùng tên lớp.
- Giúp tổ chức mã nguồn rõ ràng.

---

# 2. Nhóm Đặt Tên Gọi Khác Cho Kiểu Dữ Liệu (Type Alias)

Đôi khi kiểu dữ liệu quá dài.

Ví dụ:

```cpp
unsigned long long int
```

Việc phải viết nhiều lần rất bất tiện.

Ta có thể tạo một **tên gọi khác (Alias)**.

---

## 2.1. `typedef` (Cách cổ điển)

### Ý nghĩa

`typedef` tạo một tên mới cho một kiểu dữ liệu đã tồn tại.

### Cú pháp

```cpp
typedef Kiểu_Cũ Tên_Mới;
```

---

### Ví dụ

```cpp
typedef unsigned long long int ull;
```

Sau đó có thể sử dụng:

```cpp
ull tien_tai_khoan = 999999999;
```

thay vì:

```cpp
unsigned long long int tien_tai_khoan = 999999999;
```

---

## 2.2. `using` (Cách hiện đại - Khuyên dùng từ C++11)

### Ý nghĩa 1: Thay thế `typedef`

`using` thực hiện đúng chức năng của `typedef`, nhưng cú pháp trực quan và dễ đọc hơn.

### Cú pháp

```cpp
using Tên_Mới = Kiểu_Cũ;
```

---

### Ví dụ

```cpp
using ull = unsigned long long int;
```

Tương đương với:

```cpp
typedef unsigned long long int ull;
```

Nhưng dễ đọc hơn.

---

### Ý nghĩa 2: Mở namespace

Có thể viết:

```cpp
using namespace std;
```

Lúc này:

```cpp
cout << "Hello";
```

thay vì:

```cpp
std::cout << "Hello";
```

---

## Lưu ý

Trong các dự án lớn:

```cpp
using namespace std;
```

không được khuyến khích vì:

- Dễ gây trùng tên.
- Làm mất ý nghĩa của `namespace`.

Thay vào đó nên viết đầy đủ:

```cpp
std::cout
std::cin
std::string
```

---

# So sánh `typedef` và `using`

| `typedef`               | `using`                 |
| ----------------------- | ----------------------- |
| Có từ C                 | Có từ C++11             |
| Cú pháp khó đọc hơn     | Cú pháp giống phép gán  |
| Hỗ trợ kém với Template | Hỗ trợ Template tốt hơn |
| Hiện nay ít dùng        | Khuyến nghị sử dụng     |

---

# 3. Nhóm Tiền Xử Lý (Preprocessor)

Các lệnh bắt đầu bằng dấu:

```cpp
#
```

không phải là mã C++ chạy lúc Runtime.

Chúng là các **chỉ thị dành cho Preprocessor**.

Preprocessor sẽ chạy **trước khi chương trình được biên dịch**.

---

# 3.1. `#define`

## Ý nghĩa

`#define` giống như chức năng:

> **Find & Replace**

Preprocessor sẽ thay thế toàn bộ văn bản trước khi biên dịch.

---

## Ví dụ

```cpp
#define PI 3.14159
```

Khi viết:

```cpp
double s = PI * 5 * 5;
```

Preprocessor sẽ biến thành:

```cpp
double s = 3.14159 * 5 * 5;
```

---

## Macro có tham số

```cpp
#define MAX(a,b) ((a) > (b) ? (a) : (b))
```

Ví dụ:

```cpp
MAX(5,8)
```

sẽ được thay thành:

```cpp
((5) > (8) ? (5) : (8))
```

---

## Hạn chế

`#define`:

- Không có kiểu dữ liệu.
- Không kiểm tra kiểu.
- Không có phạm vi (Scope).

Trong C++ hiện đại, thay vì:

```cpp
#define PI 3.14159
```

người ta dùng:

```cpp
const double PI = 3.14159;
```

an toàn hơn rất nhiều.

---

# 3.2. `#ifndef`

## Ý nghĩa

`#ifndef`

viết tắt của:

> **If Not Defined**

Thường kết hợp với:

```cpp
#define
```

và

```cpp
#endif
```

để tạo **Header Guard**.

---

## Vấn đề

Giả sử:

```
main.cpp

↓

#include "NhanVien.h"

↓

NhanVien.h
```

Đồng thời:

```
PhongBan.h

↓

#include "NhanVien.h"
```

Nếu:

```
main.cpp
```

cũng include:

```
PhongBan.h
```

thì:

```
NhanVien.h
```

bị include hai lần.

Kết quả:

```text
Redefinition Error
```

---

## Header Guard

```cpp
#ifndef HEADER_H_
#define HEADER_H_

struct NhanVien
{
    int id;
};

#endif
```

---

## Cách hoạt động

Lần đầu:

```text
HEADER_H_
```

chưa tồn tại.

↓

Định nghĩa:

```cpp
#define HEADER_H_
```

↓

Code được biên dịch.

---

Lần thứ hai:

```text
HEADER_H_
```

đã tồn tại.

↓

Toàn bộ file sẽ bị bỏ qua.

↓

Không còn lỗi định nghĩa lại.

---

# 4. Nhóm Khuôn Mẫu Sinh Code (Template)

## `template`

### Ý nghĩa

`template` cho phép viết một đoạn mã **tổng quát**, không phụ thuộc vào kiểu dữ liệu.

Trình biên dịch sẽ tự sinh ra các hàm hoặc lớp tương ứng trong quá trình biên dịch.

---

## Ví dụ

```cpp
template<typename T>
T binh_phuong(T x)
{
    return x * x;
}
```

Có thể dùng với:

```cpp
int
float
double
long long
```

Ví dụ:

```cpp
binh_phuong(5);
```

↓

Sinh ra:

```cpp
int binh_phuong(int);
```

---

```cpp
binh_phuong(2.5);
```

↓

Sinh ra:

```cpp
double binh_phuong(double);
```

---

## Lợi ích

- Không cần viết nhiều hàm giống nhau.
- Giảm lặp mã.
- Tăng khả năng tái sử dụng.

---

# Tổng kết

| Từ khóa     | Mục đích                                                  |
| ----------- | --------------------------------------------------------- |
| `namespace` | Tạo không gian tên để tránh trùng tên hàm, biến và lớp    |
| `typedef`   | Đặt tên mới cho kiểu dữ liệu                              |
| `using`     | Đặt tên mới cho kiểu dữ liệu hoặc mở namespace            |
| `#define`   | Thay thế văn bản trước khi biên dịch (Macro)              |
| `#ifndef`   | Ngăn một file Header bị include nhiều lần (Header Guard)  |
| `template`  | Tạo khuôn mẫu để sinh hàm hoặc lớp cho nhiều kiểu dữ liệu |

---

# Ghi nhớ nhanh

- **`namespace`** → Xây "ngôi nhà" riêng để chống trùng tên.
- **`typedef` / `using`** → Đặt biệt danh (Alias) cho kiểu dữ liệu.
- **`#define`** → Tìm và thay thế văn bản trước khi biên dịch.
- **`#ifndef`** → Header Guard, chống include trùng lặp.
- **`template`** → Khuôn mẫu sinh mã nguồn tự động cho nhiều kiểu dữ liệu.
