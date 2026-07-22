# Algorithms và Utilities trong C++ STL

Trong **C++ STL (Standard Template Library)**, ngoài **Containers** và **Iterators**, còn có hai nhóm thành phần cực kỳ quan trọng:

- **Algorithms** (`<algorithm>`): Cung cấp các thuật toán xử lý dữ liệu tổng quát.
- **Utilities** (`<utility>`, `<tuple>`, `<functional>`, `<chrono>`...): Cung cấp các công cụ hỗ trợ lập trình hiện đại.

> Hầu hết các thuật toán trong STL đều hoạt động trên **cặp Iterator `[begin(), end())`**, nhờ đó chúng có thể sử dụng với nhiều loại container khác nhau.

---

# I. Algorithms (`<algorithm>`)

```cpp
#include <algorithm>
```

---

# 1. Sắp xếp và Tìm kiếm (Sorting & Searching)

## `std::sort()`

Sắp xếp các phần tử theo thứ tự tăng dần.

- Độ phức tạp: **O(n log n)**
- Chỉ hoạt động với **Random Access Iterator** (`vector`, `deque`, `array`...)

### Cú pháp

```cpp
std::sort(begin, end);
```

### Ví dụ

```cpp
std::vector<int> v = {5,3,8,1};

std::sort(v.begin(), v.end());
```

Kết quả:

```
1 3 5 8
```

### Sắp xếp giảm dần

```cpp
std::sort(v.begin(), v.end(), std::greater<int>());
```

Hoặc

```cpp
std::sort(v.begin(), v.end(),
[](int a, int b)
{
    return a > b;
});
```

---

## `std::stable_sort()`

Hoạt động giống `sort()` nhưng:

- Giữ nguyên thứ tự ban đầu của các phần tử bằng nhau.

Ví dụ

```
An   8
Binh 8
Nam  9
```

Sau `stable_sort()` theo điểm:

```
An   8
Binh 8
Nam  9
```

An vẫn đứng trước Bình.

---

## `std::binary_search()`

Tìm kiếm nhị phân.

Điều kiện:

Container **đã được sắp xếp**.

```cpp
bool found =
std::binary_search(
    v.begin(),
    v.end(),
    10
);
```

Kết quả:

- `true`
- `false`

Độ phức tạp:

```
O(log n)
```

---

## `std::lower_bound()`

Trả về Iterator đầu tiên có giá trị:

```
>= value
```

Ví dụ

```cpp
std::vector<int> v =
{
    1,2,4,4,4,6,8
};

auto it =
std::lower_bound(
    v.begin(),
    v.end(),
    4
);
```

Iterator trỏ tới số 4 đầu tiên.

---

## `std::upper_bound()`

Trả về Iterator đầu tiên có giá trị:

```
> value
```

Ví dụ

```cpp
auto it =
std::upper_bound(
    v.begin(),
    v.end(),
    4
);
```

Iterator sẽ trỏ tới số 6.

---

# 2. Thuật toán không sửa đổi (Non-modifying)

---

## `std::find()`

Tìm phần tử đầu tiên.

```cpp
auto it =
std::find(
    v.begin(),
    v.end(),
    20
);
```

Nếu không tìm thấy

```
it == v.end()
```

Độ phức tạp:

```
O(n)
```

---

## `std::count()`

Đếm số lần xuất hiện.

```cpp
int c =
std::count(
    v.begin(),
    v.end(),
    5
);
```

---

## `std::for_each()`

Áp dụng hàm lên từng phần tử.

```cpp
std::for_each(
    v.begin(),
    v.end(),
[](int x)
{
    std::cout << x << " ";
});
```

---

## `std::all_of()`

Tất cả đều thỏa điều kiện?

```cpp
bool ok =
std::all_of(
    v.begin(),
    v.end(),
[](int x)
{
    return x > 0;
});
```

---

## `std::any_of()`

Có ít nhất một phần tử thỏa điều kiện?

---

## `std::none_of()`

Không có phần tử nào thỏa điều kiện?

---

# 3. Thuật toán sửa đổi (Modifying)

---

## `std::copy()`

Sao chép dữ liệu.

```cpp
std::copy(
    src.begin(),
    src.end(),
    dst.begin()
);
```

Nếu container đích rỗng:

```cpp
std::copy(
    src.begin(),
    src.end(),
    std::back_inserter(dst)
);
```

---

## `std::reverse()`

Đảo ngược container.

```cpp
std::reverse(
    v.begin(),
    v.end()
);
```

---

## `std::fill()`

Gán toàn bộ phần tử.

```cpp
std::fill(
    v.begin(),
    v.end(),
    0
);
```

---

## `std::unique()`

Loại bỏ các phần tử trùng nhau **liền kề**.

```cpp
std::sort(v.begin(), v.end());

auto it =
std::unique(
    v.begin(),
    v.end()
);

v.erase(it, v.end());
```

Lưu ý:

`unique()` **không xóa thật**.

Nó chỉ dồn các phần tử trùng về cuối.

---

## `std::remove()`

Di chuyển các phần tử cần xóa về cuối.

```cpp
auto it =
std::remove(
    v.begin(),
    v.end(),
    5
);

v.erase(it, v.end());
```

Đây được gọi là:

**Erase-Remove Idiom**

---

# 4. Min / Max

---

## `std::min()`

```cpp
std::min(a,b)
```

---

## `std::max()`

```cpp
std::max(a,b)
```

---

## `std::min_element()`

Tìm Iterator nhỏ nhất.

```cpp
auto it =
std::min_element(
    v.begin(),
    v.end()
);
```

---

## `std::max_element()`

Tìm Iterator lớn nhất.

```cpp
auto it =
std::max_element(
    v.begin(),
    v.end()
);
```

---

# II. Utilities

---

# 1. `<utility>`

```cpp
#include <utility>
```

---

## `std::pair`

Lưu hai giá trị khác kiểu.

```cpp
std::pair<std::string,int> student;

student.first = "Nhan";
student.second = 10;
```

---

## `std::make_pair()`

```cpp
auto p =
std::make_pair("Nhan",10);
```

---

## `std::swap()`

Hoán đổi hai biến.

```cpp
std::swap(a,b);
```

---

## `std::move()`

Chuyển đối tượng sang **rvalue reference**.

```cpp
std::string s1 = "Hello";

std::string s2 =
std::move(s1);
```

Sau đó

```
s1
```

không còn sở hữu dữ liệu nữa.

Move Semantics giúp:

- Không copy bộ nhớ.
- Tăng hiệu năng.

---

# 2. `<tuple>`

```cpp
#include <tuple>
```

---

## `std::tuple`

Lưu nhiều giá trị khác kiểu.

```cpp
std::tuple<
std::string,
int,
double
> person;
```

Khởi tạo

```cpp
auto person =
std::make_tuple(
    "Nhan",
    20,
    8.5
);
```

Truy cập

```cpp
std::get<0>(person);
std::get<1>(person);
std::get<2>(person);
```

---

# 3. `<functional>`

```cpp
#include <functional>
```

---

## `std::function`

Lưu bất kỳ callable nào.

Ví dụ

```cpp
std::function<int(int,int)> f;

f =
[](int a,int b)
{
    return a+b;
};

std::cout
<< f(3,4);
```

---

## Functors

### `std::greater<T>()`

Sắp xếp giảm dần.

```cpp
std::sort(
    v.begin(),
    v.end(),
    std::greater<int>()
);
```

---

### `std::less<T>()`

Sắp xếp tăng dần.

---

### `std::plus<T>()`

Phép cộng.

```cpp
std::plus<int> add;

std::cout
<< add(3,5);
```

---

# 4. `<chrono>`

```cpp
#include <chrono>
```

Dùng để đo thời gian chạy chương trình.

---

## Đồng hồ độ phân giải cao

```cpp
auto start =
std::chrono::
high_resolution_clock::
now();
```

Sau khi chạy thuật toán

```cpp
auto end =
std::chrono::
high_resolution_clock::
now();
```

Tính thời gian

```cpp
auto duration =
std::chrono::
duration_cast
<
std::chrono::milliseconds
>
(end-start);
```

In kết quả

```cpp
std::cout
<< duration.count()
<< " ms";
```

---

# Tổng kết

## Nhóm Sorting & Searching

| Hàm               | Chức năng       | Độ phức tạp |
| ----------------- | --------------- | ----------- |
| `sort()`          | Sắp xếp         | O(n log n)  |
| `stable_sort()`   | Sắp xếp ổn định | O(n log n)  |
| `binary_search()` | Tìm nhị phân    | O(log n)    |
| `lower_bound()`   | Phần tử ≥ value | O(log n)    |
| `upper_bound()`   | Phần tử > value | O(log n)    |

---

## Nhóm Non-modifying

| Hàm          | Chức năng             |
| ------------ | --------------------- |
| `find()`     | Tìm phần tử           |
| `count()`    | Đếm                   |
| `for_each()` | Áp dụng hàm           |
| `all_of()`   | Tất cả đúng           |
| `any_of()`   | Có ít nhất một đúng   |
| `none_of()`  | Không có phần tử đúng |

---

## Nhóm Modifying

| Hàm         | Chức năng                     |
| ----------- | ----------------------------- |
| `copy()`    | Sao chép                      |
| `reverse()` | Đảo ngược                     |
| `fill()`    | Gán giá trị                   |
| `unique()`  | Loại bỏ phần tử trùng liền kề |
| `remove()`  | Xóa theo Erase-Remove Idiom   |

---

## Min / Max

| Hàm             | Chức năng         |
| --------------- | ----------------- |
| `min()`         | Giá trị nhỏ hơn   |
| `max()`         | Giá trị lớn hơn   |
| `min_element()` | Iterator nhỏ nhất |
| `max_element()` | Iterator lớn nhất |

---

## Utilities

| Thư viện       | Thành phần                            |
| -------------- | ------------------------------------- |
| `<utility>`    | `pair`, `swap`, `move`                |
| `<tuple>`      | `tuple`, `get`                        |
| `<functional>` | `function`, `greater`, `less`, `plus` |
| `<chrono>`     | Đo thời gian chạy chương trình        |

---

# Best Practices

- Luôn ưu tiên dùng các thuật toán STL thay vì tự viết khi có thể.
- Kết hợp `sort()` với `binary_search()`, `lower_bound()`, `upper_bound()` để đạt hiệu năng tối ưu.
- Khi dùng `remove()` hoặc `unique()`, luôn kết hợp với `erase()`.
- Sử dụng `std::move()` để tận dụng **Move Semantics**, tránh sao chép dữ liệu không cần thiết.
- Dùng `<chrono>` để đo và tối ưu hiệu năng thuật toán.
