# STL Containers trong C++

STL (Standard Template Library) cung cấp nhiều loại **Container** khác nhau để lưu trữ dữ liệu. Mỗi loại được thiết kế để giải quyết một nhu cầu riêng về tốc độ truy cập, thêm/xóa dữ liệu và tìm kiếm.

---

# 1. MỨC CƠ BẢN: Sequence Containers (Vật chứa Tuyến tính)

Đây là các cấu trúc lưu trữ dữ liệu theo một đường thẳng (phần tử này đứng sau phần tử kia).

---

## a. `std::vector` (Vua của các Container)

Nếu bạn phân vân không biết dùng gì, hãy luôn chọn **`std::vector`**. Đây là **mảng động (Dynamic Array)** của C++.

### Đặc điểm

- Các phần tử được lưu liên tiếp trên cùng một vùng nhớ.
- Cho phép truy cập ngẫu nhiên bằng chỉ số.

### Ưu điểm

- Truy cập cực nhanh (`O(1)`).
- Duyệt dữ liệu rất nhanh nhờ tận dụng CPU Cache.
- `push_back()` có độ phức tạp trung bình là **O(1)**.

### Nhược điểm

- Chèn hoặc xóa ở đầu hoặc giữa rất chậm (`O(n)`).
- Khi hết dung lượng phải cấp phát lại toàn bộ mảng.

---

## b. `std::deque` (Double-ended Queue)

Phát âm là **"deck"**.

Giống `vector` nhưng cho phép thêm và xóa ở cả hai đầu rất nhanh.

### Đặc điểm

- Không lưu trên một vùng nhớ liên tục.
- Được xây dựng từ nhiều khối bộ nhớ nhỏ (Chunks).

### Ưu điểm

- `push_front()`
- `push_back()`
- `pop_front()`
- `pop_back()`

đều có độ phức tạp gần **O(1)**.

### Nhược điểm

- Truy cập ngẫu nhiên chậm hơn `vector` một chút.
- Không tối ưu CPU Cache bằng `vector`.

---

## c. `std::list` và `std::forward_list`

### `std::list`

Danh sách liên kết đôi (Doubly Linked List).

Mỗi node biết:

- Node trước
- Node sau

### `std::forward_list`

Danh sách liên kết đơn (Singly Linked List).

Mỗi node chỉ biết:

- Node kế tiếp

### Ưu điểm

- Chèn/xóa ở bất kỳ vị trí nào đều rất nhanh (`O(1)` nếu đã có iterator).

### Nhược điểm

- Không hỗ trợ truy cập bằng chỉ số.
- Muốn đến phần tử thứ 100 phải đi từng node.

```cpp
list[100];   // ❌ Không tồn tại
```

---

## d. `std::array` (C++11)

Phiên bản hiện đại của mảng tĩnh C.

```cpp
std::array<int, 10> arr;
```

### Đặc điểm

- Kích thước cố định lúc biên dịch.
- Không thể thay đổi kích thước.

### Ưu điểm

- Có đầy đủ hàm STL.
- Có `.size()`.
- Có `.at()` để kiểm tra vượt biên.

---

# 2. MỨC TRUNG CẤP: Associative Containers (Tự sắp xếp)

Các container này luôn giữ dữ liệu theo thứ tự.

**Cấu trúc dữ liệu bên dưới:**

- Red-Black Tree

---

## a. `std::set`

Lưu các giá trị duy nhất.

Ví dụ:

```cpp
std::set<int> s;

s.insert(5);
s.insert(5);
s.insert(5);
```

Kết quả:

```
5
```

---

## b. `std::multiset`

Cho phép trùng lặp.

Ví dụ:

```cpp
std::multiset<int> s;

s.insert(5);
s.insert(5);
s.insert(5);
```

Kết quả:

```
5
5
5
```

---

## c. `std::map`

Lưu dữ liệu theo cặp:

```
Key -> Value
```

Ví dụ:

```cpp
std::map<std::string,int> diem;
```

### Đặc điểm

- Key không được trùng.
- Luôn tự động sắp xếp theo Key.

Ví dụ:

```cpp
#include <iostream>
#include <map>
#include <string>

int main()
{
    std::map<std::string,int> diem_thi;

    diem_thi["Nhan"] = 9;
    diem_thi["An"] = 10;
    diem_thi["Binh"] = 8;

    for (const auto& [ten, diem] : diem_thi)
    {
        std::cout << ten << " : " << diem << '\n';
    }
}
```

Kết quả:

```
An
Binh
Nhan
```

---

## d. `std::multimap`

Giống `map` nhưng cho phép nhiều phần tử có cùng Key.

---

# 3. MỨC NÂNG CAO: Unordered Containers (Hash Table)

Ra đời từ C++11.

Bao gồm:

- `std::unordered_set`
- `std::unordered_multiset`
- `std::unordered_map`
- `std::unordered_multimap`

---

## Cấu trúc bên dưới

Hash Table.

---

## Đặc điểm

Không sắp xếp dữ liệu.

Ví dụ:

```cpp
unordered_map
```

có thể in ra

```
Nhan
Binh
An
```

hoặc

```
An
Nhan
Binh
```

không theo bất kỳ quy luật nào.

---

## Ưu điểm

Tìm kiếm:

```
O(1)
```

Thêm:

```
O(1)
```

Xóa:

```
O(1)
```

(rất nhanh trong đa số trường hợp)

---

## Khi nào dùng?

Khi:

- Không cần dữ liệu theo thứ tự.
- Chỉ cần tra cứu cực nhanh.

Ví dụ:

- ID
- Username
- Password
- Cache
- Dictionary

---

# 4. MỨC CHUYÊN GIA: Container Adapters

Đây **không phải** là container mới.

Chúng chỉ là lớp "bọc" (Adapter) lên các container khác.

---

## a. `std::stack`

Ngăn xếp (LIFO).

```
Last In First Out
```

Ví dụ:

```
3
2
1
```

---

### Các hàm

```cpp
push()

pop()

top()

empty()

size()
```

---

## b. `std::queue`

Hàng đợi (FIFO)

```
First In First Out
```

Ví dụ:

```
1
2
3
```

---

### Các hàm

```cpp
push()

pop()

front()

back()

empty()

size()
```

---

## c. `std::priority_queue`

Hàng đợi ưu tiên.

Được xây dựng trên:

```
Heap
```

Phần tử lớn nhất (mặc định) luôn nằm ở đầu.

Ví dụ:

```
5
9
2
8
```

Khi lấy ra:

```
9
8
5
2
```

Ứng dụng:

- AI
- Dijkstra
- A\*
- Scheduling
- CPU Task

---

# Bảng tổng hợp các loại Container

| Nhu cầu                   | Container nên dùng    | Complexity tìm kiếm |
| ------------------------- | --------------------- | ------------------- |
| Lưu trữ thông thường      | `std::vector`         | `O(n)`              |
| Thêm/xóa đầu và cuối      | `std::deque`          | `O(n)`              |
| Chèn/xóa liên tục ở giữa  | `std::list`           | `O(n)`              |
| Mảng kích thước cố định   | `std::array`          | `O(n)`              |
| Kiểm tra tồn tại nhanh    | `std::unordered_set`  | `O(1)`              |
| Tra cứu Key → Value nhanh | `std::unordered_map`  | `O(1)`              |
| Key luôn được sắp xếp     | `std::map`            | `O(log n)`          |
| Giá trị không trùng       | `std::set`            | `O(log n)`          |
| Cho phép trùng            | `std::multiset`       | `O(log n)`          |
| Stack (LIFO)              | `std::stack`          | `top(): O(1)`       |
| Queue (FIFO)              | `std::queue`          | `front(): O(1)`     |
| Hàng đợi ưu tiên          | `std::priority_queue` | `top(): O(1)`       |

---

# Tóm tắt nhanh

## Sequence Containers

- `std::vector`
- `std::deque`
- `std::list`
- `std::forward_list`
- `std::array`

---

## Associative Containers

- `std::set`
- `std::multiset`
- `std::map`
- `std::multimap`

---

## Unordered Containers

- `std::unordered_set`
- `std::unordered_multiset`
- `std::unordered_map`
- `std::unordered_multimap`

---

## Container Adapters

- `std::stack`
- `std::queue`
- `std::priority_queue`

---

# Quy tắc chọn Container

✅ Không biết dùng gì → **`std::vector`**

✅ Cần tra cứu nhanh → **`std::unordered_map`**

✅ Cần dữ liệu luôn sắp xếp → **`std::map`**

✅ Cần thêm/xóa liên tục → **`std::list`**

✅ Cần thao tác hai đầu → **`std::deque`**

✅ Cần hàng đợi → **`std::queue`**

✅ Cần ngăn xếp → **`std::stack`**

✅ Cần ưu tiên phần tử lớn nhất/nhỏ nhất → **`std::priority_queue`**
