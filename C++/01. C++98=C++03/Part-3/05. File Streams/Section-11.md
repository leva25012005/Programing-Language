# Stream (Luồng) trong C++

Trong C++, **Stream (Luồng)** là một khái niệm trừu tượng dùng để mô tả quá trình truyền dữ liệu.

Hãy tưởng tượng dữ liệu giống như một dòng nước:

- **Input Stream (Luồng vào)**: Dòng dữ liệu chảy từ bên ngoài vào chương trình.
  - Bàn phím.
  - File.
  - Network.

- **Output Stream (Luồng ra)**: Dòng dữ liệu chảy từ chương trình ra bên ngoài.
  - Màn hình.
  - File.
  - Máy in.

Toàn bộ hệ thống Stream trong C++ được xây dựng dựa trên **Lập trình hướng đối tượng (OOP)**.

---

# 1. MỨC CƠ BẢN: Console I/O (`<iostream>`)

Đây là thư viện nhập xuất cơ bản nhất trong C++.

```cpp
#include <iostream>
```

Nó cung cấp 3 object toàn cục:

| Object      | Chức năng                |
| ----------- | ------------------------ |
| `std::cout` | Xuất dữ liệu ra màn hình |
| `std::cin`  | Nhập dữ liệu từ bàn phím |
| `std::cerr` | Xuất thông báo lỗi       |

---

# std::cout - Console Output

Dùng để xuất dữ liệu.

Toán tử:

```cpp
<<
```

được gọi là:

> Insertion Operator (Toán tử chèn)

Ví dụ:

```cpp
std::cout << "Hello C++";
```

---

# std::cin - Console Input

Dùng để nhận dữ liệu từ bàn phím.

Toán tử:

```cpp
>>
```

được gọi là:

> Extraction Operator (Toán tử trích xuất)

Ví dụ:

```cpp
int tuoi;

std::cin >> tuoi;
```

---

# std::cerr - Console Error

Dùng để xuất lỗi.

Điểm khác biệt:

|          | cout                | cerr     |
| -------- | ------------------- | -------- |
| Buffer   | Có                  | Không    |
| Tốc độ   | Nhanh hơn           | Chậm hơn |
| Mục đích | Output thông thường | Báo lỗi  |

`cout` gom dữ liệu trong buffer rồi xuất một lần.

`cerr` xuất ngay lập tức để đảm bảo lỗi vẫn xuất hiện nếu chương trình bị crash.

---

# Ví dụ Console I/O

```cpp
#include <iostream>

int main()
{
    int tuoi;


    std::cout << "Nhap tuoi: ";

    std::cin >> tuoi;


    if(tuoi < 0)
    {
        std::cerr
        << "Loi: Tuoi khong hop le!\n";
    }


    return 0;
}
```

---

# 2. MỨC TRUNG CẤP: File I/O (`<fstream>`)

C++ thiết kế File Stream giống Console Stream.

Bạn vẫn sử dụng:

```cpp
<<
```

và

```cpp
>>
```

để ghi và đọc dữ liệu.

---

# Các class File Stream

| Class           | Chức năng       |
| --------------- | --------------- |
| `std::ofstream` | Ghi file        |
| `std::ifstream` | Đọc file        |
| `std::fstream`  | Đọc và ghi file |

---

# Ghi và đọc File

```cpp
#include <iostream>
#include <fstream>
#include <string>


int main()
{

    // GHI FILE

    std::ofstream file_ghi("du_lieu.txt");


    if(file_ghi.is_open())
    {
        file_ghi << "Dong thu nhat\n";
        file_ghi << 12345 << "\n";

        file_ghi.close();
    }



    // DOC FILE

    std::ifstream file_doc("du_lieu.txt");


    if(!file_doc.is_open())
    {
        std::cerr
        << "Khong the mo file!\n";

        return 1;
    }


    std::string dong_chu;

    int con_so;


    std::getline(file_doc, dong_chu);

    file_doc >> con_so;



    std::cout
    << dong_chu
    << " "
    << con_so;


    return 0;
}
```

---

# 3. MỨC NÂNG CAO: String Stream (`<sstream>`)

`stringstream` là công cụ cực mạnh để xử lý chuỗi.

Thay vì dữ liệu đi:

```
RAM → Màn hình
```

hoặc:

```
RAM → File
```

thì dữ liệu đi:

```
String → Stream → Xử lý
```

---

# Ứng dụng: Tách chuỗi

Ví dụ:

Chuỗi:

```
Lap trinh C++ that thu vi
```

thành:

```
Lap
trinh
C++
that
thu
vi
```

---

## Code

```cpp
#include <iostream>
#include <sstream>
#include <string>


int main()
{

    std::string cau_van
        = "Lap trinh C++ that thu vi";


    std::stringstream luong_chuoi(cau_van);


    std::string tu;


    while(luong_chuoi >> tu)
    {
        std::cout
        << "Tu boc duoc: "
        << tu
        << "\n";
    }


    return 0;
}
```

---

# 4. MỨC CHUYÊN GIA: Stream State

Mỗi Stream đều có trạng thái bên trong.

Có 4 trạng thái:

| Hàm      | Ý nghĩa          |
| -------- | ---------------- |
| `good()` | Không có lỗi     |
| `eof()`  | Đã tới cuối file |
| `fail()` | Lỗi logic        |
| `bad()`  | Lỗi nghiêm trọng |

---

# Ví dụ lỗi nhập dữ liệu

```cpp
int so;


std::cin >> so;
```

Người dùng nhập:

```
ABC
```

Kết quả:

```
failbit = true
```

Stream bị khóa.

---

# Cách phục hồi Stream

```cpp
if(std::cin.fail())
{

    // Xóa trạng thái lỗi
    std::cin.clear();


    // Xóa dữ liệu lỗi trong buffer
    std::cin.ignore(32767, '\n');

}
```

---

# 5. Operator Overloading với Stream

Một trong những sức mạnh lớn nhất của C++:

> Cho phép tự định nghĩa cách object hoạt động với cout/cin.

Ví dụ:

Ta muốn:

```cpp
std::cout << nv;
```

thay vì:

```cpp
nv.in_thong_tin();
```

---

# Ví dụ

```cpp
#include <iostream>
#include <string>


class NhanVien
{

public:

    std::string ten;

    int tuoi;


    NhanVien(
        std::string t,
        int tu
    )
    :
    ten(t),
    tuoi(tu)
    {}



    friend std::ostream& operator<<(
        std::ostream& os,
        const NhanVien& nv
    )
    {

        os
        << "[Nhan Vien: "
        << nv.ten
        << ", Tuoi: "
        << nv.tuoi
        << "]";


        return os;

    }

};



int main()
{

    NhanVien nv1(
        "Le Van Nhan",
        25
    );


    std::cout << nv1;


    return 0;
}
```

---

# 6. Tối ưu tốc độ nhập xuất

Trong Competitive Programming:

`cin/cout` đôi khi chậm hơn `scanf/printf`.

Nguyên nhân:

C++ mặc định đồng bộ Stream với C.

---

# Giải pháp

Thêm hai dòng:

```cpp
std::ios_base::sync_with_stdio(false);

std::cin.tie(NULL);
```

---

# Ví dụ

```cpp
#include <iostream>


int main()
{

    std::ios_base::sync_with_stdio(false);

    std::cin.tie(NULL);



    int n;


    std::cin >> n;


    std::cout << n;


    return 0;
}
```

---

# Giải thích

## sync_with_stdio(false)

Ngắt đồng bộ:

```
C Stream
    |
    X
C++ Stream
```

Giúp tăng tốc.

Lưu ý:

Sau khi dùng:

```cpp
sync_with_stdio(false);
```

Không được trộn:

```cpp
printf()
scanf()
```

với:

```cpp
cout
cin
```

---

## cin.tie(NULL)

Mặc định:

```
cin
 |
 |
cout
```

Trước khi nhập dữ liệu, cout phải flush buffer.

Ngắt liên kết giúp nhanh hơn.

---

# Tổng kết

| Stream         | Công dụng     |
| -------------- | ------------- |
| `cout`         | Xuất màn hình |
| `cin`          | Nhập bàn phím |
| `cerr`         | Xuất lỗi      |
| `ofstream`     | Ghi file      |
| `ifstream`     | Đọc file      |
| `fstream`      | Đọc/Ghi file  |
| `stringstream` | Xử lý chuỗi   |

---

# Quy tắc ghi nhớ

```
Console:
<iostream>


File:
<fstream>


String parsing:
<sstream>


Custom Object:
operator<< / operator>>


Competitive Programming:
sync_with_stdio(false)
cin.tie(NULL)
```

Stream là nền tảng quan trọng của C++, giúp thống nhất cách xử lý dữ liệu từ console, file, chuỗi cho tới các hệ thống lớn như network và database.
