# Type Casting trong C++

Trong C++, hệ thống kiểu dữ liệu (**Type System**) rất nghiêm ngặt. Việc chuyển đổi một dữ liệu từ kiểu này sang kiểu khác được gọi là **Ép kiểu (Type Casting / Conversion)**.

Ép kiểu trong C++ được chia làm hai thế giới:

- **Implicit Conversion (Ép kiểu ngầm định)**: Trình biên dịch tự thực hiện.
- **Explicit Conversion (Ép kiểu tường minh)**: Lập trình viên chủ động yêu cầu.

C++ nổi tiếng vì cung cấp một hệ thống **Explicit Cast** rất chi tiết và an toàn hơn nhiều so với ngôn ngữ C.

---

# 1. Implicit Conversion (Ép kiểu ngầm định)

Đây là lúc **Compiler** tự động chuyển đổi kiểu dữ liệu nếu thấy hợp lệ.

Có hai trường hợp:

## 1.1 Promotion (Widening Conversion)

Chuyển từ kiểu dữ liệu nhỏ sang kiểu lớn hơn.

```cpp
int x = 10;
double y = x;   // 10 -> 10.0
```

### Đặc điểm

- Không mất dữ liệu.
- Luôn an toàn.
- Compiler tự động thực hiện.

---

## 1.2 Demotion (Narrowing Conversion)

Chuyển từ kiểu lớn sang kiểu nhỏ.

```cpp
double pi = 3.14159;
int le = pi;
```

Kết quả:

```cpp
le == 3
```

Phần thập phân bị cắt bỏ.

### Lưu ý

Đây là nguyên nhân của rất nhiều lỗi.

Modern C++ khuyến khích dùng **List Initialization**:

```cpp
int le{pi};     // Compile Error
```

Compiler sẽ từ chối việc làm mất dữ liệu.

---

# 2. Explicit Conversion (Ép kiểu tường minh)

Khi lập trình viên chủ động yêu cầu ép kiểu.

---

## 2.1 C-Style Cast

```cpp
double pi = 3.14;

int x = (int)pi;
```

### Vì sao không nên dùng?

C-Style Cast giống như một **cây búa tạ**.

Nó thử mọi cách để ép kiểu mà không nói rõ mục đích.

Nhược điểm:

- Khó đọc.
- Khó tìm kiếm.
- Không rõ ý định.
- Có thể phá hỏng bộ nhớ.

---

# 2.2 Bốn Named Cast của C++

Modern C++ thay thế C-style cast bằng bốn công cụ riêng biệt.

---

# 3. `static_cast`

Đây là cast được sử dụng nhiều nhất.

Khoảng **90% nhu cầu ép kiểu** đều dùng `static_cast`.

Ví dụ:

```cpp
double d = 9.99;

int i = static_cast<int>(d);
```

Ví dụ khác:

```cpp
char c = 'A';

int ascii = static_cast<int>(c);
```

### Đặc điểm

- Kiểm tra tại Compile-time.
- An toàn.
- Dễ đọc.
- Thể hiện rõ mục đích.

---

# 4. `dynamic_cast`

Chỉ dùng trong OOP.

Dùng để:

- Downcasting
- Polymorphism
- Class có virtual function

Ví dụ:

```cpp
class Cha
{
public:
    virtual void in() {}
};

class Con : public Cha
{
public:
    void choi_game() {}
};

void xu_ly(Cha* ptr_cha)
{
    Con* ptr_con = dynamic_cast<Con*>(ptr_cha);

    if(ptr_con != nullptr)
    {
        ptr_con->choi_game();
    }
}
```

## Đặc điểm

Nếu ép kiểu thất bại:

```cpp
ptr_con == nullptr
```

không làm chương trình crash.

---

# 5. `const_cast`

Chức năng duy nhất:

> Thêm hoặc loại bỏ thuộc tính `const`.

Ví dụ:

```cpp
void ham_c(int* x)
{
}

int main()
{
    const int value = 10;

    ham_c(const_cast<int*>(&value));
}
```

## Lưu ý

Không được dùng để thay đổi giá trị của một biến thực sự là `const`.

Nếu làm vậy:

> Undefined Behavior

---

# 6. `reinterpret_cast`

Đây là cast nguy hiểm nhất.

Compiler bỏ qua toàn bộ kiểm tra kiểu dữ liệu.

Ý tưởng của nó:

> "Hãy xem vùng nhớ này như một kiểu dữ liệu khác."

Ví dụ:

```cpp
int x = 65;

Player* p = reinterpret_cast<Player*>(&x);
```

Compiler sẽ không kiểm tra đúng sai.

### Thường dùng trong

- Game Engine
- Embedded
- Driver
- Network
- Memory Mapping

---

# 7. User-defined Conversion

Không chỉ kiểu dữ liệu cơ bản.

Ngay cả class cũng có thể được ép kiểu.

---

## 7.1 Constructor một tham số

Nếu class có constructor nhận đúng một tham số thì Compiler sẽ xem đó là một phép chuyển đổi ngầm định.

Ví dụ:

```cpp
class DongHo
{
public:
    int giay;

    DongHo(int g)
        : giay(g)
    {
    }
};

void in_dong_ho(DongHo d)
{
    std::cout << d.giay;
}

int main()
{
    in_dong_ho(3600);
}
```

Thực tế Compiler tự viết:

```cpp
in_dong_ho(DongHo(3600));
```

---

## 7.2 Chặn ép kiểu ngầm định bằng `explicit`

```cpp
class DongHo
{
public:
    explicit DongHo(int g)
        : giay(g)
    {
    }
};
```

Bây giờ:

```cpp
in_dong_ho(3600);
```

sẽ báo lỗi.

Phải viết:

```cpp
in_dong_ho(DongHo(3600));
```

---

# 8. Conversion Operator

Bạn có thể tự định nghĩa cách class chuyển sang kiểu dữ liệu khác.

Ví dụ:

```cpp
class BinhXang
{
    double lit;

public:

    BinhXang(double l)
        : lit(l)
    {
    }

    operator double() const
    {
        return lit;
    }
};
```

Sử dụng:

```cpp
int main()
{
    BinhXang xe(45.5);

    double do_them = xe + 10.0;
}
```

Compiler sẽ tự gọi:

```cpp
xe.operator double()
```

để lấy giá trị kiểu `double`.

---

# Bảng tổng kết các kiểu ép kiểu

| Kiểu Cast          | Mục đích                     | Kiểm tra an toàn | Thời điểm kiểm tra |
| ------------------ | ---------------------------- | ---------------- | ------------------ |
| Implicit           | Compiler tự chuyển           | Có               | Compile-time       |
| C-style Cast       | Ép kiểu tổng quát            | Không            | Compile-time       |
| `static_cast`      | Chuyển đổi kiểu thông thường | Có               | Compile-time       |
| `dynamic_cast`     | Downcasting trong OOP        | Có               | Run-time           |
| `const_cast`       | Thêm/Bỏ `const`              | Có               | Compile-time       |
| `reinterpret_cast` | Chuyển đổi địa chỉ bộ nhớ    | Không            | Compile-time       |

---

# Khi nào nên dùng?

| Nhu cầu                | Giải pháp               |
| ---------------------- | ----------------------- |
| Chuyển `double -> int` | `static_cast`           |
| Chuyển `char -> int`   | `static_cast`           |
| Downcasting OOP        | `dynamic_cast`          |
| Bỏ `const`             | `const_cast`            |
| Chuyển đổi vùng nhớ    | `reinterpret_cast`      |
| Không rõ mục đích      | Không dùng C-style Cast |

---

# Best Practice (Modern C++)

✅ Luôn ưu tiên `static_cast`.

✅ Chỉ dùng `dynamic_cast` khi làm việc với đa hình (Polymorphism).

✅ Chỉ dùng `const_cast` khi bắt buộc tương thích với thư viện cũ.

✅ Hạn chế tối đa `reinterpret_cast`.

❌ Không dùng C-style Cast trong dự án Modern C++.

---

# Ghi nhớ

- **Implicit Conversion** → Compiler tự làm.
- **Explicit Conversion** → Lập trình viên chủ động.
- **`static_cast`** → Lựa chọn mặc định trong Modern C++.
- **`dynamic_cast`** → Downcasting an toàn.
- **`const_cast`** → Thêm/Bỏ `const`.
- **`reinterpret_cast`** → Công cụ nguy hiểm nhất, chỉ dùng khi thực sự cần.
- **`explicit`** → Chặn chuyển đổi ngầm định.
- **Conversion Operator** → Tự định nghĩa cách class chuyển sang kiểu khác.
