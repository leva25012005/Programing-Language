# Smart Pointer trong C++

Trong C++ cổ điển, làm việc với **con trỏ thô (Raw Pointer: `new` và `delete`)** giống như bạn đang đi trên một bãi mìn.

Các vấn đề thường gặp:

- Quên `delete` → gây **Memory Leak** (rò rỉ bộ nhớ).
- Gọi `delete` hai lần → gây **Crash ứng dụng**.
- Trỏ vào vùng nhớ đã bị xóa → sinh ra **Dangling Pointer** (con trỏ lơ lửng).

Để giải quyết triệt để vấn đề này, **C++11** đã giới thiệu:

# Smart Pointer (Con trỏ thông minh)

Smart Pointer là các lớp bao bọc Raw Pointer và áp dụng nguyên tắc:

> **RAII (Resource Acquisition Is Initialization)**

Khi Smart Pointer ra khỏi phạm vi hoạt động (scope), nó sẽ tự động giải phóng tài nguyên.

Bạn không cần viết `delete` thủ công nữa.

Các loại Smart Pointer quan trọng:

- `std::unique_ptr`
- `std::shared_ptr`
- `std::weak_ptr`

---

# 1. std::unique_ptr - Chủ nhân độc quyền

## Ý tưởng

`std::unique_ptr` là Smart Pointer mặc định nên dùng trong hầu hết trường hợp.

Tên của nó thể hiện:

> Một thời điểm chỉ có duy nhất một đối tượng sở hữu vùng nhớ.

Đặc điểm:

- Không được phép copy.
- Chỉ được phép chuyển quyền sở hữu bằng `std::move`.
- Tự động gọi destructor khi hết phạm vi.

---

## Ví dụ

```cpp
#include <iostream>
#include <memory>

class NhanVien {
public:
    NhanVien() {
        std::cout << "Nhan vien duoc Tao!\n";
    }

    ~NhanVien() {
        std::cout << "Nhan vien bi Huy!\n";
    }

    void lam_viec() {
        std::cout << "Dang go code...\n";
    }
};


int main() {

    std::cout << "--- Bat dau pham vi ---\n";

    {
        // Khởi tạo bằng make_unique
        std::unique_ptr<NhanVien> ptr1
            = std::make_unique<NhanVien>();

        ptr1->lam_viec();


        // Không được copy
        // std::unique_ptr<NhanVien> ptr2 = ptr1;
        // ERROR!


        // Chuyển quyền sở hữu
        std::unique_ptr<NhanVien> ptr2
            = std::move(ptr1);


        if(ptr1 == nullptr) {
            std::cout
            << "ptr1 da mat quyen so huu\n";
        }

    }

    // ptr2 bị hủy ở đây
    // Destructor tự động gọi


    std::cout << "--- Ket thuc pham vi ---\n";

    return 0;
}
```

---

## Hiệu năng của unique_ptr

`std::unique_ptr` có:

- Chi phí bằng 0 (**Zero-overhead**).
- Kích thước RAM bằng đúng Raw Pointer.
- Không có Reference Counting.

Ví dụ:

```
Raw pointer:
8 bytes

unique_ptr:
8 bytes
```

=> Hiệu năng gần như giống hệt con trỏ thường.

---

# 2. std::shared_ptr - Đồng sở hữu

## Ý tưởng

Có những trường hợp một tài nguyên cần nhiều nơi cùng sử dụng.

Ví dụ:

- Một file âm thanh.
- Một texture trong game.
- Một object dùng chung giữa nhiều module.

Lúc này dùng:

```cpp
std::shared_ptr
```

---

# Cơ chế Reference Counting

Bên dưới `shared_ptr` có một:

```
Control Block
```

Chứa bộ đếm số lượng người sở hữu.

Ví dụ:

```
shared_ptr A
      |
      |
      v
   Object
      ^
      |
shared_ptr B
```

Reference Count:

```
A tạo:
count = 1


B copy:
count = 2


A bị hủy:
count = 1


B bị hủy:
count = 0

=> delete object
```

---

## Ví dụ

```cpp
#include <iostream>
#include <memory>


int main() {

    std::shared_ptr<int> ptr_chinh
        = std::make_shared<int>(100);


    std::cout
    << ptr_chinh.use_count()
    << "\n";


    {
        std::shared_ptr<int> ptr_phu_1 = ptr_chinh;
        std::shared_ptr<int> ptr_phu_2 = ptr_chinh;


        std::cout
        << ptr_chinh.use_count()
        << "\n";
    }


    std::cout
    << ptr_chinh.use_count()
    << "\n";


    return 0;
}
```

Output:

```
1
3
1
```

---

# Nhược điểm của shared_ptr

`shared_ptr` có chi phí cao hơn:

## Bộ nhớ

Raw Pointer:

```
8 bytes
```

unique_ptr:

```
8 bytes
```

shared_ptr:

```
16 bytes
```

Vì cần:

```
+----------------+
| Data Pointer   |
+----------------+
| Control Block  |
+----------------+
```

---

## CPU

Mỗi lần:

- Copy shared_ptr.
- Hủy shared_ptr.

Cần:

- Tăng Reference Count.
- Giảm Reference Count.

Các thao tác này thường dùng atomic operation để an toàn đa luồng.

---

# 3. std::weak_ptr - Giải quyết vòng lặp tham chiếu

## Vấn đề của shared_ptr

`shared_ptr` có lỗi nguy hiểm:

# Circular Dependency

Ví dụ:

```
NguoiChoi
    |
    | shared_ptr
    |
    v
NhiemVu
    |
    | shared_ptr
    |
    v
NguoiChoi
```

Hai object giữ nhau sống mãi.

Reference Count:

```
NguoiChoi = 1
NhiemVu   = 1
```

Khi chương trình kết thúc:

```
count không bao giờ = 0
```

=> Memory Leak.

---

# weak_ptr là gì?

`weak_ptr`:

- Không sở hữu dữ liệu.
- Không tăng Reference Count.
- Chỉ quan sát object.

---

# Ví dụ

```cpp
#include <iostream>
#include <memory>


class NhiemVu;


class NguoiChoi {

public:

    std::shared_ptr<NhiemVu> nhiem_vu_dang_lam;


    ~NguoiChoi()
    {
        std::cout
        << "Huy Nguoi Choi\n";
    }
};



class NhiemVu {

public:

    std::weak_ptr<NguoiChoi> chu_nhan;


    ~NhiemVu()
    {
        std::cout
        << "Huy Nhiem Vu\n";
    }
};



int main()
{

    auto nguoi_choi
        = std::make_shared<NguoiChoi>();


    auto nhiem_vu
        = std::make_shared<NhiemVu>();


    nguoi_choi->nhiem_vu_dang_lam
        = nhiem_vu;


    nhiem_vu->chu_nhan
        = nguoi_choi;


    return 0;
}
```

Kết quả:

```
Huy Nhiem Vu
Huy Nguoi Choi
```

Không còn Memory Leak.

---

# Sử dụng weak_ptr

Vì `weak_ptr` không giữ object sống nên cần:

```cpp
lock()
```

để lấy một `shared_ptr` tạm thời.

Ví dụ:

```cpp
if(auto ptr = weak.lock())
{
    ptr->lam_viec();
}
else
{
    std::cout << "Object da bi huy";
}
```

---

# 4. Quy tắc sử dụng trong Modern C++

## 1. Hạn chế new/delete

Không nên:

```cpp
NhanVien* nv = new NhanVien();
delete nv;
```

Nên:

```cpp
auto nv = std::make_unique<NhanVien>();
```

---

# 2. Mặc định dùng unique_ptr

Thiết kế ban đầu:

```
unique_ptr
      |
      |
Nếu không đủ
      |
      v
shared_ptr
```

---

# 3. Chỉ dùng shared_ptr khi thật sự cần

Không dùng vì:

> "Nó tiện"

Mà chỉ dùng khi:

> Nhiều nơi thật sự cùng sở hữu vòng đời object.

---

# 4. Khi nào dùng Raw Pointer?

Raw Pointer vẫn có thể dùng khi:

- Truyền tham số.
- Chỉ đọc dữ liệu.
- Không có quyền sở hữu.

Ví dụ:

```cpp
void hien_thi(const NhanVien* nv)
{
    nv->lam_viec();
}
```

Ở đây:

- Hàm chỉ quan sát.
- Không delete.
- Không quản lý vòng đời.

---

# Tổng kết

| Smart Pointer | Quyền sở hữu     | Copy  | Chi phí  | Khi dùng            |
| ------------- | ---------------- | ----- | -------- | ------------------- |
| `unique_ptr`  | Một chủ sở hữu   | Không | Rất thấp | Mặc định            |
| `shared_ptr`  | Nhiều chủ sở hữu | Có    | Cao hơn  | Chia sẻ tài nguyên  |
| `weak_ptr`    | Không sở hữu     | Không | Thấp     | Quan sát shared_ptr |

---

# Quy tắc vàng

```
Mặc định:
std::unique_ptr


Cần nhiều nơi sở hữu:
std::shared_ptr


Cần quan sát shared_ptr:
std::weak_ptr


Tránh:
new / delete thủ công
```

Smart Pointer là một trong những nền tảng quan trọng nhất của **Modern C++**, giúp code an toàn hơn, tránh memory leak và quản lý tài nguyên tự động.
