# 4. Thói quen tối ưu hóa: Prefix (`++i`) vs Postfix (`i++`)

Trong C++, cả **Prefix Increment (`++i`)** và **Postfix Increment (`i++`)** đều có chức năng tăng giá trị của biến lên `1`. Tuy nhiên, cách hoạt động bên trong của chúng khác nhau, và điều này ảnh hưởng đến hiệu suất khi làm việc với các object phức tạp.

---

# Prefix Increment (`++i`)

Toán tử Prefix sẽ:

1. Tăng giá trị của biến trước.
2. Trả về chính biến đó sau khi đã tăng.

Ví dụ:

```cpp
int a = 5;

int b = ++a;
```

Quá trình diễn ra:

```text
a = 5

++a
↓
a tăng thành 6
↓
Trả về chính a (giá trị 6)

Kết quả:
a = 6
b = 6
```

---

# Postfix Increment (`i++`)

Toán tử Postfix sẽ:

1. Lưu lại giá trị hiện tại.
2. Tăng biến lên.
3. Trả về giá trị cũ đã lưu.

Ví dụ:

```cpp
int c = 5;

int d = c++;
```

Quá trình diễn ra:

```text
c = 5

c++
↓
Lưu bản sao của c (5)
↓
Tăng c thành 6
↓
Trả về bản sao (5)

Kết quả:
c = 6
d = 5
```

---

# Điều gì xảy ra bên trong?

## Prefix (`++i`)

```cpp
++i;
```

Có thể hình dung như:

```cpp
i = i + 1;
return i;
```

Không cần tạo bản sao của đối tượng.

---

## Postfix (`i++`)

Có thể hình dung như:

```cpp
temp = i;
i = i + 1;
return temp;
```

Tức là phải:

1. Tạo một bản sao.
2. Tăng giá trị thật.
3. Trả về bản sao.

---

# So sánh

| Prefix (`++i`)                                | Postfix (`i++`)                         |
| --------------------------------------------- | --------------------------------------- |
| Tăng trước                                    | Trả về trước rồi mới tăng               |
| Không tạo bản sao                             | Phải tạo bản sao                        |
| Nhanh hơn với object                          | Chậm hơn với object                     |
| Khuyến khích sử dụng khi không cần giá trị cũ | Chỉ dùng khi cần giá trị trước khi tăng |

---

# Ví dụ với kiểu dữ liệu cơ bản

```cpp
int i = 5;

std::cout << ++i << '\n';
```

Kết quả:

```text
6
```

Sau đó:

```cpp
int j = 5;

std::cout << j++ << '\n';
```

Kết quả:

```text
5
```

Giá trị của `j` sau đó là:

```text
6
```

---

# Ví dụ với object

Giả sử có lớp:

```cpp
class Number {
public:
    Number& operator++() {
        // Prefix
        value++;
        return *this;
    }

    Number operator++(int) {
        // Postfix
        Number temp = *this;
        value++;
        return temp;
    }

private:
    int value = 0;
};
```

Có thể thấy:

- **Prefix** trả về tham chiếu (`reference`) đến chính object.
- **Postfix** phải tạo thêm object `temp` để lưu giá trị cũ.

Điều này làm Postfix tốn thêm chi phí sao chép.

---

# Ví dụ với Iterator trong STL

Giả sử:

```cpp
std::vector<int> nums = {1,2,3,4,5};
```

### Prefix

```cpp
for (auto it = nums.begin(); it != nums.end(); ++it)
{
    std::cout << *it << " ";
}
```

### Postfix

```cpp
for (auto it = nums.begin(); it != nums.end(); it++)
{
    std::cout << *it << " ";
}
```

Hai đoạn code đều cho kết quả:

```text
1 2 3 4 5
```

Tuy nhiên:

- `++it` chỉ tăng iterator và trả về chính iterator.
- `it++` phải tạo một iterator tạm để lưu giá trị cũ trước khi tăng.

Đối với iterator của các container lớn hoặc các iterator tùy chỉnh (custom iterator), việc tạo bản sao có thể tốn chi phí đáng kể.

---

# Khi nào nên dùng Prefix và Postfix?

| Trường hợp                        | Nên dùng                                    |
| --------------------------------- | ------------------------------------------- |
| Chỉ cần tăng biến                 | `++i`                                       |
| Cần giá trị sau khi tăng          | `++i`                                       |
| Cần giá trị trước khi tăng        | `i++`                                       |
| Vòng lặp với Iterator             | `++it`                                      |
| Biến kiểu `int`, `char`, `double` | Cả hai gần như không khác biệt về hiệu năng |

---

# Lời khuyên

- Với các kiểu dữ liệu cơ bản như `int`, `char`, `float`, `double`, trình biên dịch hiện đại thường tối ưu tốt nên sự khác biệt về hiệu năng gần như không đáng kể.
- Với các **object lớn**, **iterator trong STL**, hoặc **custom class**, `++i` thường hiệu quả hơn vì không cần tạo bản sao tạm.
- Do đó, hãy hình thành thói quen:
  - **Ưu tiên dùng `++i` khi chỉ cần tăng giá trị.**
  - **Chỉ dùng `i++` khi thực sự cần sử dụng giá trị trước khi tăng.**

> **Kết luận:**  
> `++i` và `i++` có cùng mục đích là tăng giá trị lên `1`, nhưng `++i` hoạt động trực tiếp trên đối tượng và thường tối ưu hơn. Vì vậy, trong các trường hợp không cần giá trị cũ, **`++i` là lựa chọn được khuyến nghị trong C++ hiện đại**.
