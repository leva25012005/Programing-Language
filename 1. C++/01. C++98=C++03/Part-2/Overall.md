# LẬP TRÌNH HƯỚNG ĐỐI TƯỢNG (OOP) TRONG C++ — TOÀN TẬP

> Tài liệu này đi từ khái niệm nền tảng đến các kỹ thuật nâng cao/hiện đại (C++11 → C++20), kèm ví dụ code có thể biên dịch chạy thử.

---

## MỤC LỤC

1. Tổng quan về OOP
2. Class và Object
3. Constructor & Destructor
4. Tính đóng gói (Encapsulation)
5. Con trỏ `this`, thành viên `static`
6. Tính kế thừa (Inheritance)
7. Tính đa hình (Polymorphism)
8. Tính trừu tượng (Abstraction) & Interface
9. Nạp chồng toán tử (Operator Overloading)
10. `friend` — bạn của class
11. Rule of Three/Five/Zero, Move Semantics
12. Đa kế thừa & Virtual Inheritance (Diamond Problem)
13. Template & Generic Programming
14. Exception Handling trong OOP
15. Smart Pointer & RAII
16. Design Pattern cơ bản áp dụng OOP
17. C++ hiện đại: OOP với C++11 → C++20
18. Sai lầm thường gặp & Best Practice
19. Lộ trình luyện tập đề xuất

---

## 1. TỔNG QUAN VỀ OOP

OOP (Object-Oriented Programming) tổ chức chương trình xoay quanh **đối tượng (object)** — thực thể gói gọn cả **dữ liệu (thuộc tính)** và **hành vi (phương thức)**. C++ là ngôn ngữ lai (hỗ trợ cả thủ tục lẫn OOP), dựa trên 4 trụ cột:

| Trụ cột                      | Ý nghĩa                                                       |
| ---------------------------- | ------------------------------------------------------------- |
| **Encapsulation** (Đóng gói) | Giấu dữ liệu nội bộ, chỉ cho truy cập qua interface công khai |
| **Abstraction** (Trừu tượng) | Chỉ phơi bày cái cần thiết, ẩn chi tiết cài đặt               |
| **Inheritance** (Kế thừa)    | Class con tái sử dụng & mở rộng class cha                     |
| **Polymorphism** (Đa hình)   | Một interface, nhiều cách hiện thực                           |

**Vì sao dùng OOP:** dễ bảo trì, tái sử dụng code, mô hình hóa gần với tư duy thực tế, dễ mở rộng theo nguyên lý SOLID.

---

## 2. CLASS VÀ OBJECT

### 2.1 Khai báo class cơ bản

```cpp
#include <iostream>
#include <string>
using namespace std;

class Sinhvien {
private:
    string ten;
    int tuoi;

public:
    // Setter
    void setTen(string t) { ten = t; }
    void setTuoi(int t) {
        if (t > 0) tuoi = t;
    }

    // Getter
    string getTen() const { return ten; }
    int getTuoi() const { return tuoi; }

    void hienThi() const {
        cout << "Ten: " << ten << ", Tuoi: " << tuoi << endl;
    }
};

int main() {
    Sinhvien sv;          // object cấp phát trên stack
    sv.setTen("An");
    sv.setTuoi(20);
    sv.hienThi();

    Sinhvien* p = new Sinhvien(); // object cấp phát trên heap
    p->setTen("Binh");
    p->hienThi();
    delete p;              // luôn phải giải phóng heap object
}
```

### 2.2 `struct` vs `class`

Về bản chất **giống hệt nhau** trong C++, chỉ khác **mức truy cập mặc định**:

- `struct`: mặc định `public`
- `class`: mặc định `private`

Quy ước: `struct` dùng cho các "gói dữ liệu thuần" (POD), `class` dùng khi có hành vi + bất biến (invariant) cần bảo vệ.

### 2.3 Vùng nhớ: stack vs heap

```cpp
Sinhvien a;                 // stack: tự hủy khi ra khỏi scope
Sinhvien* b = new Sinhvien; // heap: tồn tại tới khi delete, hoặc dùng smart pointer
```

---

## 3. CONSTRUCTOR & DESTRUCTOR

### 3.1 Các loại constructor

```cpp
class Diem {
    int x, y;
public:
    Diem() : x(0), y(0) { cout << "Default ctor\n"; }               // default
    Diem(int a, int b) : x(a), y(b) { cout << "Param ctor\n"; }      // tham số
    Diem(const Diem& d) : x(d.x), y(d.y) { cout << "Copy ctor\n"; }  // sao chép
    Diem(Diem&& d) noexcept : x(d.x), y(d.y) { cout << "Move ctor\n"; } // di chuyển (C++11)

    ~Diem() { cout << "Destructor\n"; } // hủy
};
```

- **Member initializer list** (`: x(a), y(b)`) hiệu quả hơn gán trong thân hàm vì khởi tạo trực tiếp, không tạo giá trị mặc định rồi gán lại. **Bắt buộc dùng** với: thành viên `const`, thành viên là tham chiếu, thành viên không có default constructor.
- Thứ tự khởi tạo thành viên tuân theo **thứ tự khai báo trong class**, không phải thứ tự trong initializer list.

### 3.2 Constructor Delegation (C++11)

```cpp
class HinhChuNhat {
    double rong, cao;
public:
    HinhChuNhat(double r, double c) : rong(r), cao(c) {}
    HinhChuNhat() : HinhChuNhat(1.0, 1.0) {}  // gọi ctor khác
};
```

### 3.3 `explicit` — chặn chuyển đổi ngầm

```cpp
class Tien {
public:
    explicit Tien(double v) { /* ... */ }
};

// void nap(Tien t);
// nap(100);   // lỗi nếu không explicit sẽ ngầm chuyển 100 -> Tien(100)
// nap(Tien(100)); // OK, phải tường minh
```

Quy tắc: **constructor 1 tham số nên luôn có `explicit`**, trừ khi bạn cố ý muốn cho phép chuyển đổi ngầm.

### 3.4 Destructor & thứ tự hủy

Thứ tự hủy đối tượng **ngược lại** thứ tự khởi tạo. Với kế thừa: destructor lớp con chạy trước, rồi tới lớp cha.

```cpp
class Base { public: ~Base() { cout << "~Base\n"; } };
class Derived : public Base { public: ~Derived() { cout << "~Derived\n"; } };
// delete new Derived(); => in ra "~Derived" rồi "~Base"
```

---

## 4. TÍNH ĐÓNG GÓI (ENCAPSULATION)

### 4.1 Ba mức truy cập

| Từ khóa     | Truy cập từ bên ngoài | Truy cập từ class con |
| ----------- | --------------------- | --------------------- |
| `public`    | Có                    | Có                    |
| `protected` | Không                 | Có                    |
| `private`   | Không                 | Không                 |

```cpp
class TaiKhoan {
private:
    double soDu;
public:
    TaiKhoan(double sd) : soDu(sd) {}
    void napTien(double amt) { if (amt > 0) soDu += amt; }
    void rutTien(double amt) {
        if (amt > 0 && amt <= soDu) soDu -= amt;
        else cout << "Giao dich khong hop le\n";
    }
    double xemSoDu() const { return soDu; }
};
```

Lợi ích: bảo vệ **bất biến (invariant)** — ví dụ số dư không bao giờ âm — vì mọi thay đổi phải đi qua hàm kiểm soát.

### 4.2 `const` correctness

```cpp
class Vector2D {
    double x, y;
public:
    double getX() const { return x; }   // hàm const: không sửa thành viên
    void setX(double v) { x = v; }      // hàm non-const
};

void inRaX(const Vector2D& v) {
    cout << v.getX();   // chỉ gọi được hàm const trên object const
}
```

---

## 5. CON TRỎ `this` & THÀNH VIÊN `static`

### 5.1 `this`

`this` là con trỏ ẩn trỏ tới chính đối tượng gọi phương thức, dùng khi:

```cpp
class Diem {
    int x, y;
public:
    Diem& setX(int x) { this->x = x; return *this; }   // phân biệt tham số/thành viên
    Diem& setY(int y) { this->y = y; return *this; }
};
// Method chaining:
Diem d;
d.setX(1).setY(2);
```

### 5.2 `static` member

```cpp
class DemDoiTuong {
    static int soLuong;   // khai báo — chia sẻ cho MỌI object
public:
    DemDoiTuong() { soLuong++; }
    ~DemDoiTuong() { soLuong--; }
    static int getSoLuong() { return soLuong; } // hàm static: không có "this"
};
int DemDoiTuong::soLuong = 0;  // định nghĩa ngoài class (bắt buộc, trừ khi dùng inline C++17)
```

C++17 cho phép `inline static int soLuong = 0;` ngay trong class, không cần định nghĩa ngoài.

---

## 6. TÍNH KẾ THỪA (INHERITANCE)

### 6.1 Kế thừa cơ bản

```cpp
class DongVat {
protected:
    string ten;
public:
    DongVat(string t) : ten(t) {}
    void an() { cout << ten << " dang an\n"; }
};

class Cho : public DongVat {
public:
    Cho(string t) : DongVat(t) {}
    void sua() { cout << ten << " sua: Gau gau\n"; }
};

int main() {
    Cho c("Mi");
    c.an();   // kế thừa từ DongVat
    c.sua();
}
```

### 6.2 Mức độ kế thừa: `public`, `protected`, `private`

| Kiểu kế thừa            | `public` thành `→` | `protected` thành `→` | `private` thành `→` |
| ----------------------- | ------------------ | --------------------- | ------------------- |
| `class D : public B`    | public             | protected             | không truy cập được |
| `class D : protected B` | protected          | protected             | không truy cập được |
| `class D : private B`   | private            | private               | không truy cập được |

`public` inheritance mô hình quan hệ **"is-a"** (Cho **là một** DongVat) — đây là kiểu phổ biến nhất.

### 6.3 Các dạng kế thừa

- **Đơn (single):** 1 cha - 1 con
- **Đa mức (multilevel):** A → B → C
- **Phân cấp (hierarchical):** 1 cha - nhiều con
- **Đa kế thừa (multiple):** nhiều cha - 1 con (xem mục 12)
- **Lai (hybrid):** kết hợp các dạng trên

```cpp
class A { public: void f() { cout << "A::f\n"; } };
class B : public A {};
class C : public B { };   // multilevel: C kế thừa cả A lẫn B
```

### 6.4 Che khuất (Hiding) và gọi hàm lớp cha

```cpp
class Cha { public: void hello() { cout << "Cha::hello\n"; } };
class Con : public Cha {
public:
    void hello() {          // che khuất (hide) hàm cha — KHÔNG phải override
        Cha::hello();        // gọi tường minh hàm cha
        cout << "Con::hello\n";
    }
};
```

### 6.5 Gọi constructor lớp cha

Constructor **không được kế thừa mặc định**. Phải gọi tường minh qua initializer list (như ví dụ 6.1), nếu không C++ tự gọi default constructor của cha (nếu có).

C++11 cho phép **kế thừa constructor** tường minh:

```cpp
class Con : public Cha {
public:
    using Cha::Cha;   // kế thừa toàn bộ constructor của Cha
};
```

---

## 7. TÍNH ĐA HÌNH (POLYMORPHISM)

### 7.1 Đa hình tại thời điểm biên dịch (Compile-time / Static)

**a) Nạp chồng hàm (Function Overloading)**

```cpp
class TinhToan {
public:
    int cong(int a, int b) { return a + b; }
    double cong(double a, double b) { return a + b; }
    int cong(int a, int b, int c) { return a + b + c; }
};
```

**b) Nạp chồng toán tử** — xem mục 9.

**c) Template** — xem mục 13. Đây cũng là một dạng đa hình tĩnh (generic).

### 7.2 Đa hình tại thời điểm chạy (Runtime / Dynamic) — quan trọng nhất

Dùng `virtual` + con trỏ/tham chiếu lớp cha để gọi đúng hàm của lớp con thực sự tại **runtime** (late binding), thông qua cơ chế **vtable** (bảng hàm ảo).

```cpp
class HinhHoc {
public:
    virtual double dienTich() const { return 0; }
    virtual void inThongTin() const {
        cout << "Dien tich = " << dienTich() << endl;
    }
    virtual ~HinhHoc() = default;  // destructor ảo — BẮT BUỘC (xem 7.4)
};

class HinhTron : public HinhHoc {
    double bk;
public:
    HinhTron(double r) : bk(r) {}
    double dienTich() const override { return 3.14159 * bk * bk; }
};

class HinhVuong : public HinhHoc {
    double canh;
public:
    HinhVuong(double c) : canh(c) {}
    double dienTich() const override { return canh * canh; }
};

void xuLy(HinhHoc* h) {
    h->inThongTin();   // gọi ĐÚNG hàm dienTich() của lớp con, dù con trỏ là kiểu HinhHoc*
}

int main() {
    HinhHoc* ds[] = { new HinhTron(2), new HinhVuong(3) };
    for (auto* h : ds) xuLy(h);
    for (auto* h : ds) delete h;
}
```

### 7.3 Từ khóa quan trọng

| Từ khóa            | Ý nghĩa                                                                                                                                        |
| ------------------ | ---------------------------------------------------------------------------------------------------------------------------------------------- |
| `virtual`          | Cho phép override, kích hoạt liên kết muộn (late binding)                                                                                      |
| `override` (C++11) | Xác nhận hàm đang **ghi đè** hàm ảo của lớp cha — trình biên dịch sẽ báo lỗi nếu chữ ký sai, tránh bug "tưởng override mà thực ra tạo hàm mới" |
| `final` (C++11)    | Ngăn không cho override thêm (trên hàm) hoặc ngăn kế thừa thêm (trên class)                                                                    |

```cpp
class A { public: virtual void f() final {} };  // không class con nào được override f nữa
class B final : public A {};                     // không ai được kế thừa B nữa
```

### 7.4 Vì sao destructor phải là `virtual`

```cpp
class Base { public: ~Base() { cout << "~Base\n"; } };       // KHÔNG virtual
class Derived : public Base { public: ~Derived() { cout << "~Derived\n"; } };

Base* p = new Derived();
delete p;   // NẾU ~Base không virtual: chỉ gọi ~Base(), RÒ RỈ BỘ NHỚ / Undefined Behavior
```

**Quy tắc vàng:** Nếu class có **bất kỳ hàm virtual nào** hoặc dự định làm lớp cha để đa hình qua con trỏ, destructor **phải** `virtual`.

### 7.5 Cơ chế bên dưới: vtable & vptr (kiến thức nâng cao)

- Mỗi class có hàm `virtual` sẽ được trình biên dịch sinh ra một **vtable** (bảng con trỏ hàm ảo) dùng chung cho mọi object của class đó.
- Mỗi object có một con trỏ ẩn **vptr** trỏ tới vtable tương ứng với kiểu thực sự của nó.
- Khi gọi hàm ảo qua con trỏ/tham chiếu, chương trình tra `vptr → vtable → địa chỉ hàm` tại runtime → đây là lý do đa hình runtime có **chi phí hiệu năng nhỏ** (một lần tra bảng + không inline được) so với hàm thường.

### 7.6 Pure Virtual Function & Abstract Class

```cpp
class HinhHoc {
public:
    virtual double dienTich() const = 0;  // hàm ảo thuần túy — không có thân hàm
};
// HinhHoc h;  // LỖI: không thể tạo object từ abstract class
```

Class chứa ≥1 pure virtual function gọi là **abstract class** — chỉ dùng làm lớp cha, không thể khởi tạo trực tiếp. Xem thêm mục 8.

---

## 8. TÍNH TRỪU TƯỢNG (ABSTRACTION) & INTERFACE

### 8.1 Abstract Class làm "hợp đồng" (contract)

```cpp
class IHinhHoc {   // quy ước đặt tên "I" cho interface
public:
    virtual double chuVi() const = 0;
    virtual double dienTich() const = 0;
    virtual ~IHinhHoc() = default;
};
```

C++ không có từ khóa `interface` như Java/C#, nhưng **abstract class chỉ chứa toàn hàm pure virtual** đóng vai trò tương đương.

### 8.2 Abstract class có thể có cả hàm thường

```cpp
class Nhanvien {
protected:
    string ten;
    double luongCoBan;
public:
    Nhanvien(string t, double l) : ten(t), luongCoBan(l) {}
    virtual double tinhLuong() const = 0;   // buộc lớp con phải cài đặt
    void inTen() const { cout << "Nhan vien: " << ten << endl; }  // hàm dùng chung
    virtual ~Nhanvien() = default;
};

class NhanvienChinhThuc : public Nhanvien {
public:
    NhanvienChinhThuc(string t, double l) : Nhanvien(t, l) {}
    double tinhLuong() const override { return luongCoBan * 1.2; }
};
```

### 8.3 Trừu tượng hóa qua thiết kế API

Trừu tượng không chỉ là abstract class — nó còn là nguyên tắc thiết kế: **người dùng class chỉ cần biết "làm gì" (what), không cần biết "làm thế nào" (how)**. Ví dụ `std::vector` ẩn hoàn toàn cách quản lý bộ nhớ động bên trong.

---

## 9. NẠP CHỒNG TOÁN TỬ (OPERATOR OVERLOADING)

```cpp
class PhanSo {
    int tu, mau;
public:
    PhanSo(int t = 0, int m = 1) : tu(t), mau(m) {}

    // Toán tử thành viên
    PhanSo operator+(const PhanSo& p) const {
        return PhanSo(tu * p.mau + p.tu * mau, mau * p.mau);
    }
    bool operator==(const PhanSo& p) const {
        return tu * p.mau == p.tu * mau;
    }
    PhanSo& operator+=(const PhanSo& p) {
        tu = tu * p.mau + p.tu * mau;
        mau = mau * p.mau;
        return *this;
    }

    // Toán tử [] , () — cho object hoạt động như mảng/hàm
    // Toán tử tiền tố/hậu tố ++
    PhanSo& operator++() { tu += mau; return *this; }        // ++p (tiền tố)
    PhanSo operator++(int) { PhanSo tmp = *this; ++(*this); return tmp; } // p++ (hậu tố)

    // Toán tử xuất/nhập luồng — PHẢI là hàm friend/tự do vì đối tượng bên trái là ostream
    friend ostream& operator<<(ostream& os, const PhanSo& p) {
        os << p.tu << "/" << p.mau;
        return os;
    }
};

int main() {
    PhanSo a(1, 2), b(1, 3);
    PhanSo c = a + b;
    cout << c << endl;   // 5/6
}
```

**Nguyên tắc:**

- Toán tử làm thay đổi object bên trái, hoặc cần truy cập `this` tự nhiên (`+=`, `[]`, `()`, `->`) → nên là **hàm thành viên**.
- Toán tử đối xứng hoặc có toán hạng trái không phải kiểu class (`<<`, `>>`, hoặc `int + PhanSo`) → nên là **hàm tự do/friend**.
- Không thể tạo toán tử mới, không đổi được độ ưu tiên/số ngôi của toán tử.

---

## 10. `friend` — BẠN CỦA CLASS

```cpp
class TaiKhoan {
    double soDu;
public:
    TaiKhoan(double sd) : soDu(sd) {}
    friend void kiemToan(const TaiKhoan& tk);  // hàm friend
    friend class NganHang;                     // class friend
};

void kiemToan(const TaiKhoan& tk) {
    cout << "So du: " << tk.soDu << endl;  // truy cập private hợp lệ
}

class NganHang {
public:
    void inSoDu(const TaiKhoan& tk) { cout << tk.soDu << endl; }
};
```

**Lưu ý:** `friend` phá vỡ một phần encapsulation — nên dùng **hạn chế**, chỉ khi thật sự cần (ví dụ toán tử `<<`, các hàm/class hợp tác chặt chẽ). `friend` **không đối xứng** và **không kế thừa**: A là friend của B không có nghĩa B là friend của A, và lớp con của friend không tự động là friend.

---

## 11. RULE OF THREE / FIVE / ZERO, MOVE SEMANTICS

### 11.1 Vấn đề: Shallow copy khi có con trỏ thành viên

```cpp
class Buffer {
    int* data;
    int size;
public:
    Buffer(int s) : size(s), data(new int[s]) {}
    ~Buffer() { delete[] data; }
    // Nếu KHÔNG viết copy constructor:
    // Buffer b2 = b1;  --> b2.data trỏ CÙNG địa chỉ với b1.data
    // Khi cả hai bị destructor delete[] cùng 1 vùng nhớ -> DOUBLE FREE (lỗi nghiêm trọng)
};
```

### 11.2 Rule of Three (C++98/03)

Nếu class tự quản lý tài nguyên (con trỏ raw, file handle...), cần định nghĩa **cả ba**: destructor, copy constructor, copy assignment operator.

```cpp
class Buffer {
    int* data; int size;
public:
    Buffer(int s) : size(s), data(new int[s]) {}
    ~Buffer() { delete[] data; }

    // Copy constructor — deep copy
    Buffer(const Buffer& other) : size(other.size), data(new int[other.size]) {
        for (int i = 0; i < size; i++) data[i] = other.data[i];
    }
    // Copy assignment — deep copy, tự kiểm tra self-assignment
    Buffer& operator=(const Buffer& other) {
        if (this == &other) return *this;
        delete[] data;
        size = other.size;
        data = new int[size];
        for (int i = 0; i < size; i++) data[i] = other.data[i];
        return *this;
    }
};
```

### 11.3 Rule of Five (C++11) — thêm Move semantics

Thêm **move constructor** và **move assignment** để tránh copy tốn kém khi object là **rvalue** (tạm thời) — "cướp" tài nguyên thay vì sao chép.

```cpp
class Buffer {
    int* data; int size;
public:
    Buffer(int s) : size(s), data(new int[s]) {}
    ~Buffer() { delete[] data; }
    Buffer(const Buffer& other) : size(other.size), data(new int[other.size]) {
        std::copy(other.data, other.data + size, data);
    }
    Buffer& operator=(const Buffer& other) { /* như trên */ return *this; }

    // Move constructor: "cướp" con trỏ, không cấp phát mới
    Buffer(Buffer&& other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
    }
    // Move assignment
    Buffer& operator=(Buffer&& other) noexcept {
        if (this == &other) return *this;
        delete[] data;
        data = other.data; size = other.size;
        other.data = nullptr; other.size = 0;
        return *this;
    }
};

Buffer taoBuffer() {
    Buffer b(1000);
    return b;              // move (hoặc NRVO), không copy toàn bộ mảng
}
```

`std::move(x)` chỉ là **ép kiểu** `x` thành rvalue reference, báo cho trình biên dịch "được phép cướp tài nguyên của x", bản thân nó không di chuyển gì cả.

### 11.4 Rule of Zero (khuyến nghị hiện đại)

Cách tốt nhất: **đừng tự quản lý tài nguyên thô**. Dùng `std::vector`, `std::string`, `std::unique_ptr`... — các lớp này đã tự lo Rule of Five, class của bạn không cần viết gì thêm (compiler sinh default đúng đắn).

```cpp
class Buffer {
    std::vector<int> data;   // Rule of Zero: không cần viết dtor/copy/move nào cả
public:
    Buffer(int s) : data(s) {}
};
```

---

## 12. ĐA KẾ THỪA & VIRTUAL INHERITANCE

### 12.1 Đa kế thừa (Multiple Inheritance)

```cpp
class ThietBiDien { public: void batDien() { cout << "Bat dien\n"; } };
class MayTinh      { public: void khoiDong() { cout << "Khoi dong\n"; } };

class Laptop : public ThietBiDien, public MayTinh {
};

Laptop lt;
lt.batDien(); lt.khoiDong();  // dùng được cả hai
```

### 12.2 Diamond Problem (vấn đề hình thoi)

```
      A
     / \
    B   C
     \ /
      D
```

```cpp
class A { public: int x; };
class B : public A {};
class C : public A {};
class D : public B, public C {};

D d;
// d.x = 5;   // LỖI biên dịch: mơ hồ (ambiguous) — D có 2 bản sao của A::x (qua B và qua C)
d.B::x = 5;   // phải chỉ rõ đường nào
```

### 12.3 Giải quyết bằng Virtual Inheritance

```cpp
class A { public: int x; };
class B : virtual public A {};
class C : virtual public A {};
class D : public B, public C {};

D d;
d.x = 5;   // OK — chỉ còn DUY NHẤT một bản sao của A trong D
```

`virtual` trong kế thừa đảm bảo class gốc chung (`A`) chỉ được cấp phát **một lần duy nhất** dù có bao nhiêu đường kế thừa dẫn tới nó. Đánh đổi: tăng độ phức tạp bộ nhớ (cần con trỏ vbase) và hơi giảm hiệu năng truy cập.

**Lời khuyên thực tế:** Đa kế thừa dữ liệu dễ gây rối; nên ưu tiên **đa kế thừa interface** (abstract class chỉ chứa pure virtual) để tránh diamond problem hoàn toàn, giống mô hình Java/C#.

---

## 13. TEMPLATE & GENERIC PROGRAMMING

### 13.1 Class Template

```cpp
template <typename T>
class NganXep {
    vector<T> data;
public:
    void push(const T& v) { data.push_back(v); }
    void pop() { data.pop_back(); }
    T top() const { return data.back(); }
    bool empty() const { return data.empty(); }
};

NganXep<int> s1;
NganXep<string> s2;
```

### 13.2 Kết hợp Template + OOP: CRTP (Curiously Recurring Template Pattern)

Kỹ thuật nâng cao để đạt "đa hình tĩnh" (static polymorphism) — không cần vtable, không có chi phí runtime:

```cpp
template <typename Derived>
class HinhHocCRTP {
public:
    double dienTich() const {
        return static_cast<const Derived*>(this)->tinhDienTich();
    }
};

class HinhTron : public HinhHocCRTP<HinhTron> {
    double bk;
public:
    HinhTron(double r) : bk(r) {}
    double tinhDienTich() const { return 3.14159 * bk * bk; }
};
```

### 13.3 Template chuyên biệt hóa (Specialization) & Concepts (C++20)

```cpp
// C++20: ràng buộc kiểu bằng concept, rõ ràng hơn SFINAE truyền thống
template <typename T>
concept CoTheCong = requires(T a, T b) { a + b; };

template <CoTheCong T>
T tongHai(T a, T b) { return a + b; }
```

---

## 14. EXCEPTION HANDLING TRONG OOP

```cpp
class SoDuKhongDu : public std::exception {
    string msg;
public:
    SoDuKhongDu(string s) : msg(s) {}
    const char* what() const noexcept override { return msg.c_str(); }
};

class TaiKhoan {
    double soDu;
public:
    TaiKhoan(double sd) : soDu(sd) {}
    void rut(double amt) {
        if (amt > soDu) throw SoDuKhongDu("Khong du tien de rut!");
        soDu -= amt;
    }
};

int main() {
    TaiKhoan tk(100);
    try {
        tk.rut(500);
    } catch (const SoDuKhongDu& e) {
        cout << "Loi: " << e.what() << endl;
    } catch (const std::exception& e) {
        cout << "Loi khac: " << e.what() << endl;
    }
}
```

**Nguyên tắc:** custom exception nên kế thừa `std::exception` (hoặc `std::runtime_error`), override `what() const noexcept`, và **luôn bắt theo tham chiếu `const&`** để tránh object slicing (xem mục 18).

---

## 15. SMART POINTER & RAII

**RAII** (Resource Acquisition Is Initialization): tài nguyên được cấp phát trong constructor, giải phóng trong destructor — gắn vòng đời tài nguyên với vòng đời object, tránh rò rỉ ngay cả khi có exception.

```cpp
#include <memory>

class Engine { public: void run() { cout << "Engine running\n"; } };

class Car {
    std::unique_ptr<Engine> engine;   // sở hữu độc quyền, không copy được, chỉ move
public:
    Car() : engine(std::make_unique<Engine>()) {}
    void start() { engine->run(); }
};   // engine tự động delete khi Car bị hủy — không cần viết destructor thủ công

std::shared_ptr<Engine> shared1 = std::make_shared<Engine>();
std::shared_ptr<Engine> shared2 = shared1;  // đếm tham chiếu = 2, tự huỷ khi count về 0

std::weak_ptr<Engine> weak1 = shared1;  // không tăng ref count, tránh circular reference
```

| Smart pointer | Ngữ nghĩa sở hữu                    | Copy được?                       |
| ------------- | ----------------------------------- | -------------------------------- |
| `unique_ptr`  | Sở hữu độc quyền                    | Không (chỉ move)                 |
| `shared_ptr`  | Sở hữu chia sẻ (đếm tham chiếu)     | Có                               |
| `weak_ptr`    | Không sở hữu, quan sát `shared_ptr` | Có (nhưng không giữ sống object) |

**Best practice hiện đại:** hạn chế tối đa `new`/`delete` thủ công trong class OOP — dùng smart pointer để tự động hóa quản lý vòng đời, kết hợp với Rule of Zero (mục 11.4).

---

## 16. DESIGN PATTERN CƠ BẢN ÁP DỤNG OOP

### 16.1 Singleton

```cpp
class Logger {
    Logger() {}
public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    static Logger& instance() {
        static Logger inst;   // C++11: thread-safe khởi tạo lazy
        return inst;
    }
    void log(string msg) { cout << "[LOG] " << msg << endl; }
};
// Logger::instance().log("Hello");
```

### 16.2 Factory Method

```cpp
class SanPham { public: virtual void hienThi() = 0; virtual ~SanPham() = default; };
class SanPhamA : public SanPham { public: void hienThi() override { cout << "A\n"; } };
class SanPhamB : public SanPham { public: void hienThi() override { cout << "B\n"; } };

class Factory {
public:
    static std::unique_ptr<SanPham> tao(char loai) {
        if (loai == 'A') return std::make_unique<SanPhamA>();
        return std::make_unique<SanPhamB>();
    }
};
```

### 16.3 Strategy Pattern (thay hành vi runtime bằng đa hình)

```cpp
class ChienLuocSapXep { public: virtual void sapXep(vector<int>&) = 0; virtual ~ChienLuocSapXep()=default; };
class QuickSort : public ChienLuocSapXep { public: void sapXep(vector<int>& v) override { /*...*/ } };
class BubbleSort : public ChienLuocSapXep { public: void sapXep(vector<int>& v) override { /*...*/ } };

class BoSapXep {
    std::unique_ptr<ChienLuocSapXep> chienLuoc;
public:
    BoSapXep(std::unique_ptr<ChienLuocSapXep> cl) : chienLuoc(std::move(cl)) {}
    void thucThi(vector<int>& v) { chienLuoc->sapXep(v); }
};
```

### 16.4 Observer Pattern (ý tưởng nền, ứng dụng trong GUI/Event system)

```cpp
class NguoiQuanSat { public: virtual void capNhat(int giaTri) = 0; virtual ~NguoiQuanSat()=default; };

class ChuThe {
    vector<NguoiQuanSat*> danhSach;
    int trangThai;
public:
    void dangKy(NguoiQuanSat* n) { danhSach.push_back(n); }
    void thayDoi(int gt) {
        trangThai = gt;
        for (auto* n : danhSach) n->capNhat(trangThai);
    }
};
```

---

## 17. OOP VỚI C++ HIỆN ĐẠI (C++11 → C++20)

| Tính năng                  | Phiên bản | Vai trò với OOP                                               |
| -------------------------- | --------- | ------------------------------------------------------------- |
| `override`, `final`        | C++11     | An toàn khi ghi đè hàm ảo                                     |
| Move semantics, `&&`       | C++11     | Hiệu năng khi truyền/trả object lớn                           |
| `= default`, `= delete`    | C++11     | Kiểm soát tường minh hàm sinh tự động                         |
| `auto`, lambda             | C++11     | Kết hợp OOP với lập trình hàm (functor thay virtual đơn giản) |
| `std::function`            | C++11     | Lưu trữ hành vi (callback) không cần class + virtual          |
| Structured bindings        | C++17     | Giải nén nhiều giá trị trả về gọn hơn                         |
| `inline` static member     | C++17     | Không cần định nghĩa static ngoài class                       |
| Concepts                   | C++20     | Ràng buộc kiểu cho template rõ ràng, thay SFINAE              |
| Modules                    | C++20     | Thay thế header truyền thống, biên dịch nhanh hơn             |
| `<=>` (spaceship operator) | C++20     | Tự sinh toàn bộ toán tử so sánh chỉ với 1 định nghĩa          |

```cpp
// = default / = delete
class KhongCopy {
public:
    KhongCopy() = default;
    KhongCopy(const KhongCopy&) = delete;            // cấm copy
    KhongCopy& operator=(const KhongCopy&) = delete;
};

// Spaceship operator C++20
class Diem {
    int x, y;
public:
    auto operator<=>(const Diem&) const = default;  // tự sinh <, >, <=, >=, ==, !=
};

// Lambda như một "object hàm" nhẹ thay cho functor class truyền thống
auto cong = [](int a, int b) { return a + b; };
```

---

## 18. SAI LẦM THƯỜNG GẶP & BEST PRACTICE

### 18.1 Object Slicing

```cpp
class Base { public: virtual void f() { cout << "Base\n"; } };
class Derived : public Base { public: void f() override { cout << "Derived\n"; } };

void inRa(Base b) { b.f(); }   // truyền theo GIÁ TRỊ -> cắt bớt phần Derived

Derived d;
inRa(d);   // in ra "Base" — SAI Ý ĐỊNH (object bị "cắt")
```

**Cách tránh:** luôn truyền đa hình qua **con trỏ** hoặc **tham chiếu** (`Base&`, `Base*`), không bao giờ truyền theo giá trị.

### 18.2 Quên `virtual` destructor → rò rỉ bộ nhớ (đã nêu ở 7.4)

### 18.3 Gọi hàm ảo trong constructor/destructor

```cpp
class Base {
public:
    Base() { init(); }         // NGUY HIỂM
    virtual void init() { cout << "Base::init\n"; }
};
class Derived : public Base {
public:
    void init() override { cout << "Derived::init\n"; }
};
Derived d;   // in ra "Base::init" chứ KHÔNG PHẢI "Derived::init"
```

Lý do: khi constructor của `Base` chạy, phần `Derived` **chưa được khởi tạo**, nên vtable tại thời điểm đó vẫn trỏ về `Base`. Đây là hành vi đặc thù C++ (khác Java) — cần đặc biệt lưu ý.

### 18.4 So sánh `==` giữa các kiểu con qua con trỏ cha (dùng sai đa hình cho so sánh)

Nên tránh so sánh bằng operator ảo phức tạp; cân nhắc pattern **Visitor** hoặc so sánh sau khi `dynamic_cast` kiểm tra kiểu.

### 18.5 Lạm dụng kế thừa thay vì Composition

Nguyên tắc thiết kế nổi tiếng: **"Favor composition over inheritance"** — chỉ dùng kế thừa khi thực sự có quan hệ **is-a** chặt chẽ và ổn định; nếu chỉ cần tái sử dụng code, hãy dùng **composition** (has-a, chứa object khác làm thành viên).

```cpp
// Thay vì: class Xe : public DongCo, public BanhXe {...}  (kế thừa lạm dụng)
class Xe {
    DongCo dongCo;    // composition — "Xe CÓ MỘT DongCo"
    BanhXe banhXe[4];
};
```

### 18.6 `dynamic_cast` & RTTI (Run-Time Type Information)

```cpp
Base* b = new Derived();
if (Derived* d = dynamic_cast<Derived*>(b)) {
    d->hamRiengCuaDerived();   // an toàn, trả nullptr nếu ép sai kiểu
}
```

Dùng khi thật cần biết kiểu cụ thể tại runtime — nhưng cần thường xuyên `dynamic_cast` là dấu hiệu thiết kế đa hình chưa tốt (nên để hành vi nằm trong hàm `virtual`).

---

## 19. LỘ TRÌNH LUYỆN TẬP ĐỀ XUẤT

1. **Cơ bản:** viết class `HocSinh`, `SanPham`, `TaiKhoanNganHang` — thực hành encapsulation, constructor/destructor, getter/setter.
2. **Kế thừa & đa hình:** xây hệ thống `HinhHoc` (Tròn, Vuông, TamGiac) hoặc `NhanVien` (ThoiVu, ChinhThuc, QuanLy) dùng virtual function.
3. **Toán tử:** cài class `PhanSo`, `MaTran`, `VectorToanHoc` với nạp chồng toán tử đầy đủ.
4. **Quản lý tài nguyên:** viết class tự quản lý mảng động, áp dụng Rule of Five, sau đó viết lại bằng Rule of Zero với `std::vector`.
5. **Nâng cao:** thực hành 3 pattern (Singleton, Factory, Strategy) trong 1 project nhỏ (ví dụ: hệ thống quản lý đơn hàng).
6. **Hiện đại:** refactor toàn bộ project trên bằng smart pointer, `override`/`final`, và (nếu dùng C++20) concepts + spaceship operator.
7. **Đọc thêm:** cuốn _"Effective C++"_ và _"Effective Modern C++"_ của Scott Meyers — kinh điển cho OOP + best practice C++ nâng cao.

---

### Tổng kết ghi nhớ nhanh

- 4 trụ cột: **Encapsulation – Abstraction – Inheritance – Polymorphism**
- Đa hình runtime cần: `virtual` + con trỏ/tham chiếu + destructor ảo
- Quản lý tài nguyên: ưu tiên **Rule of Zero** + smart pointer thay vì tự `new`/`delete`
- Thiết kế: **composition over inheritance**, interface (abstract class) thay vì đa kế thừa dữ liệu
- C++ hiện đại giúp OOP an toàn & ngắn gọn hơn nhiều so với C++98/03
