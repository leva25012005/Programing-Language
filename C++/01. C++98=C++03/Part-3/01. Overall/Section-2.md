# Namespace trong C++

`namespace` (Không gian tên) được tạo ra để giải quyết một bài toán rất quan trọng trong các dự án lớn:

> **Đụng độ tên (Name Collision).**

Khi bạn chỉ làm việc với một file hoặc một chương trình nhỏ, gần như bạn sẽ không cảm nhận được sự cần thiết của `namespace`.

Tuy nhiên, trong các dự án lớn:

- Có hàng chục hoặc hàng trăm lập trình viên cùng phát triển.
- Có nhiều thư viện được sử dụng cùng lúc (Đồ họa, Âm thanh, Mạng, Toán học,...).

Việc hai thư viện cùng định nghĩa một hàm như:

```cpp
draw()
```

hoặc

```cpp
init()
```

là điều hoàn toàn bình thường.

Nếu không có `namespace`, chương trình sẽ không biết phải gọi hàm nào và dẫn đến lỗi **Name Collision**.

---

# 1. Cú pháp cơ bản

## Khai báo Namespace

Để tạo một namespace, sử dụng cú pháp:

```cpp
namespace TenNamespace
{
    // Khai báo hàm
    // Biến
    // Class
}
```

---

## Ví dụ

```cpp
#include <iostream>

// Namespace cho thư viện đồ họa
namespace Graphics
{
    int resolution_x = 1920;

    void draw()
    {
        std::cout << "Dang ve hinh anh.\n";
    }
}

// Namespace cho thư viện âm thanh
namespace Audio
{
    int volume = 80;

    void draw()
    {
        std::cout << "Dang ve bieu do song am.\n";
    }
}

int main()
{
    Graphics::draw();
    Audio::draw();

    std::cout << "Volume: "
              << Audio::volume
              << '\n';
}
```

---

## Toán tử `::`

Dấu

```cpp
::
```

được gọi là:

> **Scope Resolution Operator** (Toán tử phân giải phạm vi)

Nó dùng để chỉ rõ thành phần thuộc namespace nào.

Ví dụ:

```cpp
Graphics::draw();
```

hoặc

```cpp
std::cout
```

---

# 2. Từ khóa `using`

Việc phải viết:

```cpp
Graphics::draw();
Graphics::resolution_x;
Graphics::...
```

liên tục sẽ khá dài.

`using` giúp rút ngắn việc này.

---

## 2.1. `using namespace`

### Cú pháp

```cpp
using namespace Graphics;
```

Lúc này:

```cpp
draw();
```

thay vì

```cpp
Graphics::draw();
```

---

### Ví dụ

```cpp
using namespace Graphics;

int main()
{
    draw();
}
```

---

## Cạm bẫy

Nếu tiếp tục viết:

```cpp
using namespace Audio;
```

thì:

```cpp
draw();
```

sẽ trở thành:

```text
Graphics::draw()
hay
Audio::draw()
```

Trình biên dịch không thể xác định.

Kết quả:

```text
Ambiguous Call
```

---

# Luật thép

Không bao giờ viết:

```cpp
using namespace std;
```

ở phạm vi toàn cục (**Global Scope**) trong các file:

```
.h
.hpp
```

Lý do:

Toàn bộ file nào `#include` header đó cũng sẽ bị "ô nhiễm namespace".

Điều này dễ gây xung đột tên và rất khó bảo trì.

---

## Chỉ nên dùng trong phạm vi cục bộ

Ví dụ:

```cpp
int main()
{
    using namespace std;

    cout << "Hello";
}
```

---

# 2.2. Chỉ `using` một thành phần

Đây là cách được khuyến nghị.

Ví dụ:

```cpp
using std::cout;
using std::endl;
```

Khi đó:

```cpp
cout << "Hello";
```

được phép.

Trong khi:

```cpp
cin >> x;
```

vẫn phải viết:

```cpp
std::cin >> x;
```

---

## Ví dụ

```cpp
using std::cout;
using std::endl;

int main()
{
    cout << "Hello" << endl;

    std::cin.get();
}
```

---

# 3. Các tính năng nâng cao của Namespace

---

# 3.1. Namespace lồng nhau (Nested Namespace)

Namespace có thể chứa namespace khác.

Ví dụ:

```cpp
namespace GameEngine
{
    namespace Physics
    {
        void applyGravity()
        {

        }
    }

    namespace Network
    {
        void connect()
        {

        }
    }
}
```

Gọi:

```cpp
GameEngine::Physics::applyGravity();
```

---

## Cú pháp mới từ C++17

Có thể viết ngắn hơn:

```cpp
namespace GameEngine::Audio
{
    void playSound()
    {

    }
}
```

---

# 3.2. Namespace Alias

Nếu namespace quá dài:

```cpp
namespace Company_Project_Module
{

}
```

ta có thể đặt biệt danh.

Ví dụ:

```cpp
namespace MyMod = Company_Project_Module;
```

Sau đó:

```cpp
MyMod::doSomething();
```

thay vì:

```cpp
Company_Project_Module::doSomething();
```

---

## Ví dụ

```cpp
namespace Company_Project_Module
{
    void doSomething()
    {

    }
}

int main()
{
    namespace MyMod = Company_Project_Module;

    MyMod::doSomething();
}
```

---

# 3.3. Unnamed Namespace

Có thể tạo namespace không tên.

```cpp
namespace
{

}
```

Các thành phần bên trong chỉ được phép sử dụng trong chính file `.cpp` đó.

Ví dụ:

```cpp
namespace
{
    void helper()
    {
        std::cout << "Chi duoc goi trong file nay";
    }
}

void calculate()
{
    helper();
}
```

File khác dù:

```cpp
#include
```

hay

```cpp
extern
```

cũng không thể truy cập.

---

## Khi nào dùng?

Rất phù hợp để:

- Helper Function
- Helper Variable
- Hàm nội bộ
- Biến nội bộ

Thay thế cho:

```cpp
static
```

ở phạm vi Global.

---

# 3.4. Namespace mở rộng

Một namespace không cần khai báo toàn bộ tại một chỗ.

Có thể mở nhiều lần.

Ví dụ:

### File `phan_1.h`

```cpp
namespace MathLib
{
    void add();
}
```

---

### File `phan_2.h`

```cpp
namespace MathLib
{
    void sub();
}
```

---

Sau khi biên dịch:

```cpp
MathLib
```

sẽ có:

```cpp
add();
sub();
```

---

# So sánh các cách sử dụng `using`

| Cách sử dụng           | Ưu điểm            | Nhược điểm                          |
| ---------------------- | ------------------ | ----------------------------------- |
| `using namespace std;` | Viết ngắn          | Dễ gây trùng tên                    |
| `using std::cout;`     | An toàn            | Chỉ import đúng thành phần cần dùng |
| `std::cout`            | Chuyên nghiệp nhất | Phải gõ dài hơn                     |

---

# Tổng kết

| Tính năng               | Ý nghĩa                                  |
| ----------------------- | ---------------------------------------- |
| `namespace`             | Chống trùng tên giữa các thư viện        |
| `using namespace`       | Mở toàn bộ namespace                     |
| `using std::cout`       | Chỉ mở một thành phần                    |
| Nested Namespace        | Namespace lồng nhau                      |
| Namespace Alias         | Đặt biệt danh cho namespace              |
| Unnamed Namespace       | Ẩn các hàm và biến trong một file `.cpp` |
| Discontiguous Namespace | Có thể khai báo namespace ở nhiều nơi    |

---

# Ghi nhớ nhanh

- Luôn bọc thư viện hoặc dự án của bạn trong một `namespace`.
- Hạn chế sử dụng `using namespace std;`, đặc biệt trong các file Header (`.h`, `.hpp`).
- Ưu tiên viết đầy đủ:

```cpp
std::cout
std::cin
std::vector
std::string
```

để mã nguồn rõ ràng và chuyên nghiệp hơn.

- Sử dụng **Unnamed Namespace** để ẩn các hàm hỗ trợ (helper functions) và biến chỉ dùng trong nội bộ file `.cpp`.
