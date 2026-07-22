# Exception Handling trong C++

Trong ngôn ngữ C (tiền nhiệm của C++), khi một hàm gặp lỗi, cách xử lý phổ biến là trả về một mã lỗi:

```cpp
return -1;
```

Vấn đề:

- Lập trình viên có thể quên kiểm tra mã lỗi.
- Chương trình tiếp tục chạy với dữ liệu sai.
- Lỗi phát sinh ở nơi khác rất khó truy vết.

---

## Exception trong C++ là gì?

**Exception (Ngoại lệ)** là một cơ chế xử lý lỗi của C++.

Nó hoạt động như một hệ thống **báo động đỏ**:

1. Phát hiện lỗi nghiêm trọng.
2. Dừng luồng thực thi hiện tại.
3. Ném ra một Exception.
4. Chuyển quyền xử lý sang khối `catch`.

Nếu không có ai xử lý Exception:

> Chương trình sẽ bị terminate (Crash).

---

# 1. Mức cơ bản: Bộ ba `try - throw - catch`

Đây là cú pháp nền tảng của Exception.

| Thành phần | Vai trò                            |
| ---------- | ---------------------------------- |
| `try`      | Chứa đoạn code có khả năng gây lỗi |
| `throw`    | Ném ra Exception                   |
| `catch`    | Bắt và xử lý Exception             |

---

## Ví dụ cơ bản

```cpp
#include <iostream>

double chia_hai_so(double a, double b)
{
    if (b == 0)
    {
        throw "Loi: Khong the chia cho 0!";
    }

    return a / b;
}


int main()
{
    try
    {
        std::cout << "Ket qua: "
                  << chia_hai_so(10, 2)
                  << "\n";


        std::cout << "Ket qua: "
                  << chia_hai_so(10, 0)
                  << "\n";


        // Không bao giờ chạy tới đây
        std::cout << "Chay tiep tuc...\n";
    }


    catch (const char* thong_bao_loi)
    {
        std::cout
            << "Da bat duoc loi: "
            << thong_bao_loi
            << "\n";
    }


    std::cout
        << "Chuong trinh van an toan!\n";


    return 0;
}
```

---

## Luồng hoạt động

```
try
 |
 |---- gọi hàm
 |
 |---- phát hiện lỗi
 |
throw Exception
 |
 |
catch bắt lỗi
 |
 |
tiếp tục chương trình
```

---

# 2. Mức trung cấp: Standard Exception Library

Trong thực tế:

Không nên throw trực tiếp chuỗi:

```cpp
throw "Error";
```

Vì:

- Khó phân loại lỗi.
- Khó mở rộng.
- Không hỗ trợ tốt OOP.

C++ cung cấp thư viện:

```cpp
#include <stdexcept>
```

---

# Nguyên tắc vàng

## Throw by Value

Khi ném lỗi:

```cpp
throw std::invalid_argument("Sai tham so");
```

Tạo ra một Object mới.

---

## Catch by Reference

Khi bắt lỗi:

```cpp
catch(const std::invalid_argument& e)
```

Không copy Object.

Lợi ích:

- Hiệu năng tốt hơn.
- Giữ nguyên tính đa hình.
- Không mất dữ liệu class con.

---

# Ví dụ sử dụng Standard Exception

```cpp
#include <iostream>
#include <stdexcept>


void kiem_tra_tuoi(int tuoi)
{
    if(tuoi < 0)
    {
        throw std::invalid_argument(
            "Tuoi khong duoc am!"
        );
    }


    if(tuoi > 150)
    {
        throw std::out_of_range(
            "Tuoi vuot qua gioi han!"
        );
    }
}


int main()
{
    try
    {
        kiem_tra_tuoi(200);
    }


    catch(const std::invalid_argument& e)
    {
        std::cout
            << "Loi tham so: "
            << e.what()
            << "\n";
    }


    catch(const std::out_of_range& e)
    {
        std::cout
            << "Loi gioi han: "
            << e.what()
            << "\n";
    }


    catch(...)
    {
        std::cout
            << "Loi khong xac dinh!\n";
    }
}
```

---

# Một số Exception chuẩn trong C++

| Exception               | Ý nghĩa                         |
| ----------------------- | ------------------------------- |
| `std::invalid_argument` | Tham số truyền vào không hợp lệ |
| `std::out_of_range`     | Vượt quá giới hạn               |
| `std::runtime_error`    | Lỗi khi chạy chương trình       |
| `std::logic_error`      | Lỗi logic chương trình          |
| `std::bad_alloc`        | Không cấp phát được bộ nhớ      |

---

# 3. Mức nâng cao: Stack Unwinding

Đây là cơ chế quan trọng nhất của Exception.

Khi một Exception được throw:

```
Function A
   |
   |
Function B
   |
   |
throw Exception
```

C++ sẽ quay ngược stack:

```
Function B bị hủy
        |
Function A bị hủy
        |
catch nhận Exception
```

Quá trình này gọi là:

# Stack Unwinding

---

# Destructor được gọi tự động

Ví dụ:

```cpp
class TaiNguyen
{
public:

    TaiNguyen()
    {
        std::cout<<"Mo tai nguyen\n";
    }


    ~TaiNguyen()
    {
        std::cout<<"Giai phong tai nguyen\n";
    }
};



void ham()
{
    TaiNguyen obj;

    throw std::runtime_error(
        "Co loi"
    );
}
```

Khi Exception xảy ra:

Destructor vẫn được gọi.

Kết quả:

```
Mo tai nguyen

Giai phong tai nguyen
```

---

# Exception + RAII

Đây là lý do Smart Pointer rất mạnh.

Ví dụ:

```cpp
std::unique_ptr<int> ptr(
    new int(100)
);

throw std::runtime_error("Error");
```

Khi Exception xảy ra:

`unique_ptr`

tự động gọi:

```cpp
delete
```

=> Không bị Memory Leak.

---

# 4. Modern C++: `noexcept`

Từ C++11:

```cpp
noexcept
```

được giới thiệu.

Ý nghĩa:

> Hàm cam kết không bao giờ ném Exception.

---

Ví dụ:

```cpp
void cap_nhat_diem() noexcept
{
    // Code chắc chắn không throw
}
```

---

# Tại sao cần noexcept?

Exception cần cơ chế theo dõi:

- Stack Unwinding.
- Metadata.
- Xử lý lỗi.

Điều này gây thêm chi phí.

`noexcept` cho Compiler biết:

> Không cần chuẩn bị cơ chế xử lý Exception.

---

# Ứng dụng quan trọng: std::vector

Khi vector mở rộng:

```
Memory cũ
 |
 | Move dữ liệu
 |
Memory mới
```

Vector kiểm tra:

## Move Constructor có noexcept?

---

## Có noexcept

```cpp
Object(Object&& other) noexcept
```

Vector:

```
Move dữ liệu
Nhanh
```

---

## Không noexcept

Vector lo sợ:

```
Move đang chạy
Exception xảy ra
Dữ liệu mất
```

Nó dùng:

```
Copy Constructor
```

Chậm hơn.

---

# So sánh C Error Code và Exception

| Tiêu chí              | C-style Error Code    | C++ Exception     |
| --------------------- | --------------------- | ----------------- |
| Bắt buộc xử lý        | Không                 | Có                |
| Bỏ qua lỗi            | Dễ dàng               | Crash             |
| Code chính            | Trộn với kiểm tra lỗi | Tách biệt         |
| Hiệu năng bình thường | Nhanh                 | Chậm hơn một chút |
| Debug                 | Khó                   | Dễ hơn            |
| Ứng dụng              | Embedded, hệ thống    | Desktop, Backend  |

---

# Khi nào nên dùng Exception?

## Nên dùng

Các lỗi bất thường:

- Không mở được file.
- Database mất kết nối.
- Không cấp phát được RAM.
- Network bị lỗi.

Ví dụ:

```cpp
throw std::runtime_error(
    "Database disconnected"
);
```

---

## Không nên dùng

Không dùng Exception cho logic bình thường.

Ví dụ:

Sai:

```cpp
throw "Sai mat khau";
```

Nên:

```cpp
return false;
```

hoặc:

```cpp
std::optional<User>
```

---

# Best Practice Modern C++

## Luôn:

✅ Throw Object

```cpp
throw std::runtime_error("Error");
```

---

✅ Catch bằng reference

```cpp
catch(const std::exception& e)
```

---

✅ Dùng RAII

```cpp
std::unique_ptr
std::vector
std::string
```

---

✅ Thêm noexcept cho:

- Destructor.
- Move Constructor.
- Move Assignment Operator.

---

# Tổng kết

| Thành phần       | Ý nghĩa                        |
| ---------------- | ------------------------------ |
| `try`            | Vùng code nguy hiểm            |
| `throw`          | Phát sinh Exception            |
| `catch`          | Xử lý lỗi                      |
| `std::exception` | Base class của Exception chuẩn |
| Stack Unwinding  | Cơ chế dọn dẹp stack           |
| RAII             | Tự động giải phóng tài nguyên  |
| `noexcept`       | Cam kết không throw lỗi        |

---

# Ghi nhớ khi đi làm

- Exception dùng cho **lỗi bất thường**, không dùng thay thế logic.
- Luôn bắt Exception bằng `const reference`.
- Dùng Standard Exception thay vì throw chuỗi.
- Tận dụng RAII để tránh Memory Leak.
- `noexcept` rất quan trọng trong Modern C++.
