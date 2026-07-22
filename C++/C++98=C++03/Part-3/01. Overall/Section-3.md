# Từ khóa `using` trong C++

Từ khóa `using` trong C++ là một trong những từ khóa có màn **"lột xác" ngoạn mục nhất**.

- **Trước C++11:** `using` chủ yếu được dùng để làm việc với **Namespace** và **Class**.
- **Từ C++11 trở đi:** `using` được nâng cấp để:
  - Thay thế hoàn toàn `typedef`.
  - Hỗ trợ **Template Alias**.
  - Hỗ trợ **Constructor Inheritance**.
  - Giải quyết **Name Hiding** trong kế thừa.

Ngày nay, `using` là một trong những công cụ quan trọng nhất của **Modern C++**.

---

# 1. Mức cơ bản: Quản lý Namespace

Đây là chức năng đầu tiên mà hầu hết người học C++ đều gặp.

Nó cho phép **nhập các tên (identifier)** từ một namespace vào phạm vi hiện tại.

---

## 1.1. Nhập khẩu toàn bộ Namespace

### Cú pháp

```cpp
using namespace TenNamespace;
```

Ví dụ:

```cpp
#include <iostream>

using namespace std;

int main()
{
    cout << "Khong can viet std::cout nua!\n";

    vector<int> v;
}
```

Sau khi viết:

```cpp
using namespace std;
```

ta có thể sử dụng:

```cpp
cout
cin
string
vector
endl
```

thay vì:

```cpp
std::cout
std::cin
std::string
std::vector
std::endl
```

---

## Cạm bẫy

Việc sử dụng:

```cpp
using namespace std;
```

gây ra:

- Name Pollution (Ô nhiễm không gian tên)
- Dễ trùng tên hàm
- Dễ trùng tên class
- Dễ trùng tên biến

Đặc biệt:

> **Không bao giờ viết `using namespace std;` trong file `.h` hoặc `.hpp`.**

---

# 1.2. Nhập khẩu có chọn lọc

Đây là **Best Practice** trong C++ hiện đại.

Ví dụ:

```cpp
#include <iostream>
#include <vector>

using std::cout;
using std::vector;

int main()
{
    cout << "An toan hon rat nhieu!\n";

    vector<int> v;

    // std::cin vẫn phải viết đầy đủ
}
```

Nếu chưa viết:

```cpp
using std::cin;
```

thì:

```cpp
cin >> x;
```

sẽ báo lỗi.

---

# So sánh hai cách sử dụng

| Cách                   | Ưu điểm   | Nhược điểm                   |
| ---------------------- | --------- | ---------------------------- |
| `using namespace std;` | Viết ngắn | Dễ gây Name Pollution        |
| `using std::cout;`     | An toàn   | Chỉ import đúng thứ cần dùng |

---

# 2. Mức trung cấp: Type Alias (Bí danh kiểu dữ liệu)

Từ C++11, `using` được dùng để thay thế `typedef`.

---

## 2.1. Đặt tên mới cho kiểu dữ liệu

### Cách cũ

```cpp
typedef unsigned long long int ULL;
```

---

### Cách mới

```cpp
using ULL = unsigned long long int;
```

---

Ví dụ:

```cpp
using ID_List = std::vector<int>;

int main()
{
    ULL tien_ty = 9999999999;

    ID_List danh_sach = {1,2,3};
}
```

---

## Vì sao `using` tốt hơn `typedef`?

So sánh:

```cpp
typedef unsigned long long int ULL;
```

với:

```cpp
using ULL = unsigned long long int;
```

`using` đọc giống phép gán:

```text
ULL = unsigned long long int
```

rất trực quan.

---

# 2.2. Function Pointer Alias

Đây là nơi `using` thể hiện ưu thế rõ nhất.

Giả sử cần khai báo:

> Con trỏ tới một hàm nhận hai `int` và trả về `int`.

---

### Cách cũ

```cpp
typedef int (*PhepToanToanHoc)(int,int);
```

---

### Cách mới

```cpp
using PhepToanToanHoc = int (*)(int,int);
```

Dễ đọc hơn rất nhiều.

---

# 3. Mức nâng cao: Can thiệp vào Kế thừa (Inheritance)

`using` còn được sử dụng trong lập trình hướng đối tượng.

Có hai ứng dụng rất quan trọng.

---

# 3.1. Giải quyết Name Hiding

Giả sử:

```cpp
class Cha
{
public:

    void ban_sung()
    {
        cout << "Pew Pew";
    }

    void ban_sung(int so_dan)
    {

    }
};
```

Class con:

```cpp
class Con : public Cha
{
public:

    void ban_sung(double laser)
    {

    }
};
```

Lúc này:

```cpp
Con c;

c.ban_sung();
```

sẽ báo lỗi.

Lý do:

Class con đã định nghĩa:

```cpp
ban_sung(double)
```

=> C++ che toàn bộ các hàm:

```cpp
ban_sung()
ban_sung(int)
```

của class cha.

---

## Giải pháp

```cpp
class Con : public Cha
{
public:

    using Cha::ban_sung;

    void ban_sung(double laser)
    {

    }
};
```

Lúc này:

```cpp
Con c;

c.ban_sung();
c.ban_sung(5);
c.ban_sung(3.14);
```

đều hợp lệ.

---

# 3.2. Kế thừa Constructor

Thông thường:

Constructor **không được kế thừa**.

Ví dụ:

```cpp
class NhanVien
{
public:

    NhanVien(string ten);

    NhanVien(string ten,int tuoi);
};
```

Nếu tạo:

```cpp
class GiamDoc : public NhanVien
{

};
```

thì:

```cpp
GiamDoc gd("Nhan",30);
```

sẽ báo lỗi.

---

## Giải pháp

```cpp
class GiamDoc : public NhanVien
{
public:

    using NhanVien::NhanVien;
};
```

Sau đó:

```cpp
GiamDoc gd("Nhan",30);
```

hoạt động bình thường.

---

# 4. Mức đỉnh cao: Alias Template

Đây là tính năng khiến `typedef` gần như bị thay thế hoàn toàn.

---

## `typedef` không làm được

Ví dụ:

```cpp
template<typename T>
typedef std::vector<T> MyVector;
```

❌ Không hợp lệ.

---

## `using` làm được

```cpp
template<typename T>
using MyVector = std::vector<T>;
```

---

Ví dụ:

```cpp
MyVector<int> ds1;

MyVector<double> ds2;
```

---

## Ví dụ với `std::map`

```cpp
#include <map>
#include <string>

template<typename T>
using TuDienCuaToi =
    std::map<std::string,T>;

int main()
{
    TuDienCuaToi<int> bang_diem;

    bang_diem["Player1"] = 100;

    TuDienCuaToi<double> bang_mau;

    bang_mau["Boss"] = 999.9;
}
```

Nhờ vậy:

- Key luôn là `std::string`
- Value có thể thay đổi tùy ý.

---

# So sánh `typedef` và `using`

| Tiêu chí                          | `typedef` | `using` |
| --------------------------------- | --------- | ------- |
| Đặt bí danh kiểu dữ liệu          | ✅        | ✅      |
| Cú pháp dễ đọc                    | ❌        | ✅      |
| Hỗ trợ Function Pointer           | ✅        | ✅      |
| Hỗ trợ Template Alias             | ❌        | ✅      |
| Được khuyến nghị trong Modern C++ | ❌        | ✅      |

---

# Tổng kết

`using` có thể được xem là **"chiếc chìa khóa vạn năng"** của C++ hiện đại.

Nó được sử dụng để:

- Mở khóa Namespace.

```cpp
using std::cout;
```

---

- Thay thế `typedef`.

```cpp
using ULL = unsigned long long;
```

---

- Giải quyết Name Hiding.

```cpp
using Base::Function;
```

---

- Kế thừa Constructor.

```cpp
using Base::Base;
```

---

- Tạo Alias Template.

```cpp
template<typename T>
using MyVector = std::vector<T>;
```

---

# Ghi nhớ nhanh

| Cách dùng                                | Mục đích               |
| ---------------------------------------- | ---------------------- |
| `using namespace std;`                   | Mở toàn bộ namespace   |
| `using std::cout;`                       | Chỉ mở một thành phần  |
| `using ULL = unsigned long long;`        | Thay thế `typedef`     |
| `using Base::Function;`                  | Giải quyết Name Hiding |
| `using Base::Base;`                      | Kế thừa Constructor    |
| `template<typename T> using Alias = ...` | Tạo Alias Template     |
