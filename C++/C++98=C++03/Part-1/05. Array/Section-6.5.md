# Thao tác trên `std::vector`

`std::vector` là một **mảng động** trong C++ Standard Library (STL).

Khác với mảng tĩnh hay mảng động thủ công (`new[]` và `delete[]`), `std::vector`:

- Tự quản lý bộ nhớ.
- Tự động thay đổi kích thước.
- Cung cấp nhiều hàm thao tác mạnh mẽ.
- Là lựa chọn được khuyến nghị trong C++ hiện đại.

---

# 1. Sửa phần tử (Update)

Do các phần tử của `std::vector` được lưu liên tiếp trong bộ nhớ, việc truy cập theo chỉ số có độ phức tạp:

```text
O(1)
```

Giống hệt mảng thông thường.

---

## Cách 1. Dùng toán tử `[]`

```cpp
v[0] = 99;
```

Ưu điểm:

- Nhanh.
- Không có chi phí kiểm tra.

Nhược điểm:

- Không kiểm tra phạm vi.
- Truy cập sai chỉ số có thể gây **Undefined Behavior**.

---

## Cách 2. Dùng `.at()`

```cpp
v.at(1) = 88;
```

Ưu điểm:

- Kiểm tra phạm vi.
- Nếu chỉ số không hợp lệ sẽ ném ngoại lệ (`std::out_of_range`).

Ví dụ:

```cpp
v.at(100);
```

↓

```text
std::out_of_range
```

---

## So sánh

| `[]`                          | `.at()`              |
| ----------------------------- | -------------------- |
| Nhanh hơn                     | Chậm hơn một chút    |
| Không kiểm tra giới hạn       | Có kiểm tra giới hạn |
| Có thể gây Undefined Behavior | An toàn hơn          |

---

# 2. Thêm phần tử (Add / Insert)

Khác với mảng thông thường, `std::vector` tự quản lý số lượng phần tử.

Không cần:

```cpp
int so_luong;
```

Có thể lấy trực tiếp:

```cpp
v.size();
```

---

# Thêm vào cuối (`push_back`)

```cpp
v.push_back(30);
```

Ví dụ:

Ban đầu:

```text
10 20
```

Sau:

```cpp
v.push_back(30);
```

Kết quả:

```text
10 20 30
```

---

## Độ phức tạp

```text
Amortized O(1)
```

Điều này có nghĩa là:

- Phần lớn thời gian: **O(1)**.
- Đôi khi vector phải cấp phát lại bộ nhớ nên sẽ mất **O(n)**.
- Trung bình nhiều lần gọi vẫn đạt **O(1)**.

---

# `emplace_back()`

Nếu vector chứa các object phức tạp:

```cpp
std::vector<Person> ds;
```

thì nên dùng:

```cpp
ds.emplace_back("Nhan", 20);
```

thay vì:

```cpp
ds.push_back(Person("Nhan",20));
```

`emplace_back()` xây dựng đối tượng **trực tiếp trong vector**, giúp tránh tạo object tạm và có thể hiệu quả hơn.

---

# Thêm vào đầu hoặc giữa (`insert`)

Cú pháp:

```cpp
v.insert(iterator, value);
```

Lưu ý:

`insert()` **không nhận chỉ số (index)** mà nhận **Iterator**.

---

## Thêm vào đầu

```cpp
v.insert(v.begin(), 5);
```

Ví dụ:

```text
10 20 30
```

↓

```text
5 10 20 30
```

---

## Thêm vào giữa

```cpp
v.insert(v.begin() + 2, 15);
```

Ví dụ:

```text
10 20 30
```

↓

```text
10 20 15 30
```

---

## Điều gì xảy ra bên trong?

Khi gọi:

```cpp
insert()
```

`std::vector` sẽ:

1. Dịch các phần tử phía sau sang phải.
2. Chèn phần tử mới.
3. Nếu hết sức chứa (`capacity`), vector sẽ tự cấp phát vùng nhớ mới.

---

## Vai trò của `reserve()`

Ví dụ:

```cpp
v.reserve(100);
```

Điều này giúp vector cấp phát trước bộ nhớ cho 100 phần tử.

Khi gọi:

```cpp
push_back()
insert()
```

vector sẽ **không cần xin thêm RAM** nếu chưa vượt quá 100 phần tử.

Điều này giúp tăng hiệu năng đáng kể.

---

## Độ phức tạp

| Thao tác        | Độ phức tạp        |
| --------------- | ------------------ |
| `push_back()`   | **Amortized O(1)** |
| `insert()` đầu  | **O(n)**           |
| `insert()` giữa | **O(n)**           |

---

# 3. Xóa phần tử (Delete / Erase)

---

# Xóa cuối (`pop_back`)

```cpp
v.pop_back();
```

Ví dụ:

```text
10 20 30
```

↓

```text
10 20
```

---

## Độ phức tạp

```text
O(1)
```

---

# Xóa đầu

```cpp
v.erase(v.begin());
```

Ví dụ:

```text
10 20 30
```

↓

```text
20 30
```

---

# Xóa giữa

```cpp
v.erase(v.begin() + 1);
```

Ví dụ:

```text
10 20 30
```

↓

```text
10 30
```

---

# Xóa một khoảng

```cpp
v.erase(
    v.begin() + 1,
    v.begin() + 4
);
```

Ví dụ:

```text
10 20 30 40 50
```

↓

```text
10 50
```

---

# Xóa toàn bộ

```cpp
v.clear();
```

Sau đó:

```cpp
v.size() == 0
```

Nhưng:

```cpp
v.capacity()
```

vẫn giữ nguyên.

Điều này giúp vector tái sử dụng bộ nhớ nếu thêm dữ liệu sau đó.

---

## Độ phức tạp

| Thao tác       | Độ phức tạp |
| -------------- | ----------- |
| `pop_back()`   | **O(1)**    |
| `erase()` đầu  | **O(n)**    |
| `erase()` giữa | **O(n)**    |
| `clear()`      | **O(n)**    |

---

# 4. Sắp xếp (`std::sort`)

Thư viện:

```cpp
#include <algorithm>
```

---

## Sắp xếp tăng dần

```cpp
std::sort(
    v.begin(),
    v.end()
);
```

Ví dụ:

```text
5 2 8 1
```

↓

```text
1 2 5 8
```

---

## Sắp xếp giảm dần

```cpp
#include <functional>

std::sort(
    v.begin(),
    v.end(),
    std::greater<int>()
);
```

Ví dụ:

```text
5 2 8 1
```

↓

```text
8 5 2 1
```

---

## Độ phức tạp

```text
O(n log n)
```

---

# 5. Duyệt bằng Iterator

## Cách truyền thống

```cpp
for (
    std::vector<int>::iterator it = v.begin();
    it != v.end();
    ++it
)
{
    std::cout << *it << " ";
}
```

Đây là cú pháp đầy đủ.

Ưu điểm:

- Rõ ràng.
- Phù hợp để học về Iterator.

Nhược điểm:

- Dài.
- Khó đọc.

---

# Modern C++

C++11 giới thiệu:

```cpp
auto
```

để trình biên dịch tự suy luận kiểu dữ liệu.

Ví dụ:

```cpp
for (
    auto it = v.begin();
    it != v.end();
    ++it
)
{
    std::cout << *it << " ";
}
```

Kết quả hoàn toàn giống cách viết truyền thống nhưng ngắn gọn và dễ đọc hơn.

---

# Range-based for

Nếu chỉ cần đọc dữ liệu:

```cpp
for (int phan_tu : v)
{
    std::cout << phan_tu << " ";
}
```

Nếu muốn sửa dữ liệu:

```cpp
for (int& phan_tu : v)
{
    phan_tu *= 2;
}
```

Nếu chỉ đọc và tránh sao chép:

```cpp
for (const int& phan_tu : v)
{
    std::cout << phan_tu << " ";
}
```

---

## So sánh các cách duyệt

| Cách duyệt                           | Khi nào nên dùng                                   |
| ------------------------------------ | -------------------------------------------------- |
| `for (int i = 0; i < v.size(); ++i)` | Cần chỉ số (Index)                                 |
| Iterator                             | Cần thao tác với STL hoặc thuật toán chuẩn         |
| `auto` Iterator                      | Viết ngắn gọn, hiện đại                            |
| Range-based for                      | Chỉ cần đọc hoặc sửa từng phần tử, không cần Index |

---

# Tóm tắt độ phức tạp

| Thao tác                 | Độ phức tạp        |
| ------------------------ | ------------------ |
| Truy cập (`[]`, `.at()`) | **O(1)**           |
| Cập nhật                 | **O(1)**           |
| `push_back()`            | **Amortized O(1)** |
| `insert()` đầu/giữa      | **O(n)**           |
| `pop_back()`             | **O(1)**           |
| `erase()` đầu/giữa       | **O(n)**           |
| `clear()`                | **O(n)**           |
| `std::sort()`            | **O(n log n)**     |
| Duyệt                    | **O(n)**           |

> **Kết luận:**  
> `std::vector` là cấu trúc dữ liệu được khuyến nghị sử dụng trong C++ hiện đại nhờ khả năng quản lý bộ nhớ tự động, hỗ trợ nhiều thao tác tiện lợi và tích hợp chặt chẽ với **STL**. Đối với hầu hết các ứng dụng, `std::vector` nên được ưu tiên thay cho mảng tĩnh hoặc mảng động thủ công.
