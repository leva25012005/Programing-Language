#Iterator trong C++ STL

Trong kiến trúc của **C++ STL(Standard Template Library) **,
    có * * 3 trụ cột chính ** :

    -**Container **(Vật chứa dữ liệu) -
        **Algorithm **(Thuật toán xử lý) -
        **Iterator **(Trình lặp / Con trỏ ảo)

             Nếu **Container **là các vương quốc tách biệt(Mảng, Cây, Bảng băm),
    và **Algorithm **là những người thợ xây,
    thì **Iterator **chính là những cây cầu.

    Nhờ có Iterator,
    thuật toán `std::sort` không cần quan tâm nó đang sắp
    xếp `std::vector` hay `std::deque`; nó chỉ cần biết cách đi từ `begin()` đến `end()`.

---

# 1. MỨC CƠ BẢN : Cú pháp và Triết lý `[begin, end)`

Về bản chất, **Iterator hoạt động giống hệt một con trỏ thô (Raw Pointer)**.

Bạn sử dụng:

- `*` để truy cập giá trị.
- `++` để di chuyển sang phần tử tiếp theo.

## Triết lý khoảng nửa mở `[begin, end)`

Đây là một trong những nguyên tắc thiết kế quan trọng nhất của STL.

- `begin()` → Trỏ tới phần tử đầu tiên.
- `end()` → Trỏ tới **vị trí ngay sau phần tử cuối cùng** (*Past-the-end*).

> **Lưu ý**
>
> `end()` **không chứa dữ liệu**. Tuyệt đối không dereference (`*end()`).

### Vì sao `end()` không trỏ tới phần tử cuối?

Vì cách thiết kế này giúp:

- Biểu diễn container rỗng:
  ```cpp
  begin() == end()
  ```
- Vòng lặp có điều kiện dừng thống nhất:

```cpp
while (it != end())
```

---

## Ví dụ

```cpp
#include <iostream>
#include <vector>

int main() {
  std::vector<int> v = {10, 20, 30};

  // Cách 1: Iterator truyền thống
  for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
    std::cout << *it << " ";

    *it = *it + 1;
  }

  // Cách 2: auto (C++11)
  for (auto it = v.begin(); it != v.end(); ++it) {
  }

  // Cách 3: Range-based for
  for (int &value : v) {
  }
}
```

---

# 2. MỨC TRUNG CẤP : Các loại Iterator(Iterator Categories)

Không phải Iterator nào cũng có khả năng giống nhau.

C++ chia thành nhiều cấp độ.

| Category | Khả năng | Container |
|----------|----------|-----------|
| Input / Output | Đọc hoặc ghi một chiều | `std::cin`, `std::cout` |
| Forward | Chỉ đi tới (`++`) | `std::forward_list` |
| Bidirectional | Đi tới và lùi (`++`, `--`) | `std::list`, `std::set`, `std::map` |
| Random Access | Nhảy cóc (`+`, `-`, `[]`) | `std::vector`, `std::deque` |
| Contiguous (C++17) | Random Access + bộ nhớ liên tục | `std::array`, `std::vector` |

---

## Ví dụ

`std::sort` yêu cầu:

- **Random Access Iterator**

Do đó:

```cpp
std::vector<int> v;
std::sort(v.begin(), v.end());      // OK
```

Trong khi:

```cpp
std::list<int> l;

std::sort(l.begin(), l.end());      // Lỗi
```

Bởi vì `list` chỉ hỗ trợ **Bidirectional Iterator**.

---

# 3. MỨC NÂNG CAO : Iterator Adapters

Iterator Adapter là các "biến thể" của Iterator thông thường.

---

## 3.1 Reverse Iterator

Cho phép duyệt ngược.

- `rbegin()`
- `rend()`

```cpp
std::vector<int> v = {1,2,3,4};

for(auto it = v.rbegin();
    it != v.rend();
    ++it)
{
  std::cout << *it << " ";
}
```

Kết quả:

```
4 3 2 1
```

Lưu ý:

Mặc dù duyệt ngược nhưng vẫn dùng:

```cpp
++it
```

---

## 3.2 Const Iterator

Nếu chỉ đọc dữ liệu:

```cpp
cbegin()

cend()
```

Ví dụ:

```cpp
for(auto it = v.cbegin();
    it != v.cend();
    ++it)
{
  std::cout << *it;

  //*it = 100; // Lỗi biên dịch
}
```

Giúp bảo vệ dữ liệu.

---

## 3.3 Insert Iterator

Thường kết hợp với các thuật toán STL.

Ví dụ:

```cpp
#include <algorithm>
#include <iterator>


std::vector<int> source = {1,2,3};
std::vector<int> destination;
```

Sai:

```cpp
std::copy(
    source.begin(),
    source.end(),
    destination.begin()
);
```

Container đích chưa có phần tử.

Đúng:

```cpp
std::copy(
    source.begin(),
    source.end(),
    std::back_inserter(destination)
);
```

`back_inserter()` sẽ tự động gọi:

```cpp
destination.push_back(...)
```

---

# 4. MỨC CHUYÊN GIA

## 4.1 Dịch chuyển Iterator an toàn

Không phải Iterator nào cũng hỗ trợ:

```cpp
it + 5
```

Ví dụ:

```cpp
std::list
```

không hỗ trợ Random Access.

Thay vào đó dùng:

```cpp
std::next(it, n)
```

```cpp
std::prev(it, n)
```

```cpp
std::distance(it1, it2)
```

Ví dụ:

```cpp
auto it = std::next(v.begin(), 5);

auto it2 = std::prev(v.end(), 2);

auto d = std::distance(v.begin(), v.end());
```

### Độ phức tạp

| Hàm | Vector | List |
|------|--------|------|
| next | O(1) | O(n) |
| prev | O(1) | O(n) |
| distance | O(1) | O(n) |

---

## 4.2 Iterator Invalidation (Lỗi nguy hiểm nhất)

Đây là lỗi rất phổ biến.

Ví dụ:

```cpp
std::vector<int> v = {1,2,3};

auto it = v.begin();
```

Sau đó:

```cpp
for(int i = 0; i < 1000; i++)
{
  v.push_back(i);
}
```

Điều gì xảy ra?

Khi `vector` hết capacity:

- Xin vùng RAM mới
- Copy toàn bộ dữ liệu
- Giải phóng vùng RAM cũ

Iterator cũ vẫn trỏ tới RAM đã bị thu hồi.

```cpp
std::cout << *it;
```

=> **Undefined Behavior**

Có thể:

- Crash
- In rác
- Chạy sai

---

#Cách khắc phục

## 1. Không lưu Iterator quá lâu

Đặc biệt với:

- vector
- deque

---

## 2. reserve()

```cpp
std::vector<int> v;

v.reserve(100000);
```

Giảm số lần Reallocation.

---

## 3. Luôn dùng Iterator mới sau erase()

Sai:

```cpp
v.erase(it);

++it;
```

Đúng:

```cpp
it = v.erase(it);
```

Bởi vì `erase()` trả về Iterator hợp lệ tiếp theo.

---

#Tổng kết

## Các loại Iterator

| Iterator | Container |
|-----------|-----------|
| Input/Output | Stream |
| Forward | forward_list |
| Bidirectional | list, set, map |
| Random Access | vector, deque |
| Contiguous | vector, array |

---

## Iterator Adapter

| Adapter | Chức năng |
|----------|-----------|
| `rbegin()` / `rend()` | Duyệt ngược |
| `cbegin()` / `cend()` | Chỉ đọc |
| `back_inserter()` | Tự động push_back |
| `front_inserter()` | Tự động push_front |
| `inserter()` | Chèn vào vị trí bất kỳ |

---

## Hàm hỗ trợ

| Hàm | Mục đích |
|------|----------|
| `next()` | Tiến Iterator |
| `prev()` | Lùi Iterator |
| `distance()` | Khoảng cách giữa hai Iterator |

---

## Những lỗi cần tránh

- Dereference `end()`
- Dùng Iterator sau khi `vector` reallocate
- Dùng Iterator cũ sau `erase()`
- Lưu Iterator quá lâu khi container có thể thay đổi kích thước

---

#Best Practices

- Ưu tiên dùng `auto` với Iterator.
- Dùng `cbegin()`/`cend()` nếu chỉ đọc.
- Dùng `std::next()` và `std::prev()` thay vì cộng/trừ Iterator.
- Sau `erase()`, luôn nhận lại Iterator trả về.
- Nếu thêm nhiều phần tử vào `vector`, hãy gọi `reserve()` trước.
- Hiểu rõ **Iterator Invalidation** để tránh các lỗi khó phát hiện.