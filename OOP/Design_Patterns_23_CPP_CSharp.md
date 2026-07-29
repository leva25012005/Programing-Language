# 23 DESIGN PATTERN (GoF) — TỔNG HỢP KÈM CODE C++ & C#

> Tài liệu dựa trên phân loại chuẩn GoF (Gang of Four) — 3 nhóm, 23 mẫu. Mỗi pattern gồm: **mục đích**, **mức độ phổ biến thực tế**, **code minh họa C++** và **C#**.
>
> Ký hiệu mức độ phổ biến: ⭐⭐⭐ Rất hay dùng · ⭐⭐ Thường gặp · ⭐ Ít gặp hơn (nhưng vẫn cần biết)

---

## MỤC LỤC

**A. Creational (Khởi tạo) — 5 mẫu**

> 1. Singleton ⭐⭐⭐
> 2. Factory Method ⭐⭐⭐
> 3. Abstract Factory ⭐⭐
> 4. Builder ⭐⭐⭐
> 5. Prototype ⭐

**B. Structural (Cấu trúc) — 7 mẫu**

> 6. Adapter ⭐⭐⭐
> 7. Bridge ⭐
> 8. Composite ⭐⭐
> 9. Decorator ⭐⭐⭐
> 10. Facade ⭐⭐⭐
> 11. Flyweight ⭐
> 12. Proxy ⭐⭐

**C. Behavioral (Hành vi) — 11 mẫu**

> 13. Chain of Responsibility ⭐⭐
> 14. Command ⭐⭐⭐
> 15. Interpreter ⭐
> 16. Iterator ⭐⭐⭐
> 17. Mediator ⭐⭐
> 18. Memento ⭐⭐
> 19. Observer ⭐⭐⭐
> 20. State ⭐⭐
> 21. Strategy ⭐⭐⭐
> 22. Template Method ⭐⭐⭐
> 23. Visitor ⭐

---

# A. CREATIONAL PATTERNS (NHÓM KHỞI TẠO)

> Che giấu logic tạo đối tượng, tránh dùng `new`/`new` trực tiếp lan tràn khắp code, giúp chương trình linh hoạt hơn khi quyết định đối tượng nào cần tạo trong tình huống nào.

## 1. Singleton ⭐⭐⭐

**Mục đích:** Đảm bảo một class chỉ có **duy nhất một instance** trong toàn bộ chương trình, và cung cấp điểm truy cập toàn cục tới nó. Thường dùng cho: Logger, cấu hình ứng dụng (Config), kết nối Database pool, cache.

**Lưu ý:** Singleton bị nhiều người coi là "anti-pattern" nếu lạm dụng (khó test, tạo trạng thái toàn cục ẩn) — chỉ dùng khi thực sự cần đúng 1 instance.

### C++

```cpp
class Logger {
private:
    Logger() {}  // constructor private
public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger& getInstance() {
        static Logger instance;   // C++11: khởi tạo lazy, thread-safe
        return instance;
    }
    void log(const std::string& msg) {
        std::cout << "[LOG] " << msg << std::endl;
    }
};

// Sử dụng:
Logger::getInstance().log("Ung dung khoi dong");
```

### C#

```csharp
public sealed class Logger
{
    private static readonly Lazy<Logger> instance = new Lazy<Logger>(() => new Logger());
    private Logger() { }
    public static Logger Instance => instance.Value;

    public void Log(string msg) => Console.WriteLine($"[LOG] {msg}");
}

// Sử dụng:
Logger.Instance.Log("Ung dung khoi dong");
```

---

## 2. Factory Method ⭐⭐⭐

**Mục đích:** Định nghĩa một interface để tạo đối tượng, nhưng **để lớp con quyết định** class nào sẽ được khởi tạo. Giúp code không phụ thuộc trực tiếp vào class cụ thể.

**Ví dụ thực tế:** hệ thống thông báo cần gửi Email/SMS/Push notification tùy cấu hình, hoặc game tạo quái vật (Orc/Troll) theo màn chơi.

### C++

```cpp
class PhuongTien {
public:
    virtual void giao() = 0;
    virtual ~PhuongTien() = default;
};
class XeMay : public PhuongTien {
public: void giao() override { std::cout << "Giao bang xe may\n"; }
};
class OTo : public PhuongTien {
public: void giao() override { std::cout << "Giao bang o to\n"; }
};

class NhaMayVanChuyen {
public:
    virtual std::unique_ptr<PhuongTien> taoPhuongTien() = 0;
    virtual ~NhaMayVanChuyen() = default;
};
class NhaMayXeMay : public NhaMayVanChuyen {
public: std::unique_ptr<PhuongTien> taoPhuongTien() override { return std::make_unique<XeMay>(); }
};
class NhaMayOTo : public NhaMayVanChuyen {
public: std::unique_ptr<PhuongTien> taoPhuongTien() override { return std::make_unique<OTo>(); }
};

// Sử dụng:
std::unique_ptr<NhaMayVanChuyen> nhaMay = std::make_unique<NhaMayOTo>();
nhaMay->taoPhuongTien()->giao();
```

### C#

```csharp
public abstract class PhuongTien { public abstract void Giao(); }
public class XeMay : PhuongTien { public override void Giao() => Console.WriteLine("Giao bang xe may"); }
public class OTo : PhuongTien { public override void Giao() => Console.WriteLine("Giao bang o to"); }

public abstract class NhaMayVanChuyen
{
    public abstract PhuongTien TaoPhuongTien();
}
public class NhaMayOTo : NhaMayVanChuyen
{
    public override PhuongTien TaoPhuongTien() => new OTo();
}

// Sử dụng:
NhaMayVanChuyen nhaMay = new NhaMayOTo();
nhaMay.TaoPhuongTien().Giao();
```

---

## 3. Abstract Factory ⭐⭐

**Mục đích:** Cung cấp interface để tạo ra **cả một họ (family) các đối tượng liên quan** mà không cần chỉ rõ class cụ thể. Khác Factory Method ở chỗ: Factory Method tạo 1 loại sản phẩm, Abstract Factory tạo **nhiều loại sản phẩm liên quan tới nhau** (đồng bộ theo "họ").

**Ví dụ thực tế:** UI Toolkit đa nền tảng — họ Windows (WindowsButton, WindowsCheckbox) và họ MacOS (MacButton, MacCheckbox) phải đồng bộ theo từng OS.

### C++

```cpp
class Button { public: virtual void render() = 0; virtual ~Button() = default; };
class Checkbox { public: virtual void render() = 0; virtual ~Checkbox() = default; };

class WinButton : public Button { public: void render() override { std::cout << "Windows Button\n"; } };
class WinCheckbox : public Checkbox { public: void render() override { std::cout << "Windows Checkbox\n"; } };
class MacButton : public Button { public: void render() override { std::cout << "Mac Button\n"; } };
class MacCheckbox : public Checkbox { public: void render() override { std::cout << "Mac Checkbox\n"; } };

class GUIFactory {
public:
    virtual std::unique_ptr<Button> taoButton() = 0;
    virtual std::unique_ptr<Checkbox> taoCheckbox() = 0;
    virtual ~GUIFactory() = default;
};
class WinFactory : public GUIFactory {
public:
    std::unique_ptr<Button> taoButton() override { return std::make_unique<WinButton>(); }
    std::unique_ptr<Checkbox> taoCheckbox() override { return std::make_unique<WinCheckbox>(); }
};
class MacFactory : public GUIFactory {
public:
    std::unique_ptr<Button> taoButton() override { return std::make_unique<MacButton>(); }
    std::unique_ptr<Checkbox> taoCheckbox() override { return std::make_unique<MacCheckbox>(); }
};

// Sử dụng: chọn factory theo OS, mọi component sinh ra luôn đồng bộ 1 style
std::unique_ptr<GUIFactory> factory = std::make_unique<MacFactory>();
factory->taoButton()->render();
factory->taoCheckbox()->render();
```

### C#

```csharp
public interface IButton { void Render(); }
public interface ICheckbox { void Render(); }

public class WinButton : IButton { public void Render() => Console.WriteLine("Windows Button"); }
public class WinCheckbox : ICheckbox { public void Render() => Console.WriteLine("Windows Checkbox"); }
public class MacButton : IButton { public void Render() => Console.WriteLine("Mac Button"); }
public class MacCheckbox : ICheckbox { public void Render() => Console.WriteLine("Mac Checkbox"); }

public interface IGUIFactory
{
    IButton TaoButton();
    ICheckbox TaoCheckbox();
}
public class WinFactory : IGUIFactory
{
    public IButton TaoButton() => new WinButton();
    public ICheckbox TaoCheckbox() => new WinCheckbox();
}
public class MacFactory : IGUIFactory
{
    public IButton TaoButton() => new MacButton();
    public ICheckbox TaoCheckbox() => new MacCheckbox();
}

// Sử dụng:
IGUIFactory factory = new MacFactory();
factory.TaoButton().Render();
factory.TaoCheckbox().Render();
```

---

## 4. Builder ⭐⭐⭐

**Mục đích:** Tách quá trình xây dựng một đối tượng phức tạp (có nhiều tham số tùy chọn) ra khỏi class biểu diễn nó, cho phép tạo đối tượng **từng bước** với nhiều cấu hình khác nhau mà không cần constructor với hàng chục tham số.

**Ví dụ thực tế:** dựng đối tượng `HttpRequest`, `PizzaOrder`, `SqlQueryBuilder`...

### C++

```cpp
class NhaXe {
    std::string mong, tuong, mai;
public:
    void setMong(std::string s) { mong = s; }
    void setTuong(std::string s) { tuong = s; }
    void setMai(std::string s) { mai = s; }
    void moTa() { std::cout << mong << " - " << tuong << " - " << mai << std::endl; }
};

class NhaXeBuilder {
    NhaXe nha;
public:
    NhaXeBuilder& xayMong() { nha.setMong("Mong be tong"); return *this; }
    NhaXeBuilder& xayTuong() { nha.setTuong("Tuong gach"); return *this; }
    NhaXeBuilder& xayMai() { nha.setMai("Mai ton"); return *this; }
    NhaXe build() { return nha; }
};

// Sử dụng: method chaining, dễ đọc, dễ chọn bước cần
NhaXe nha = NhaXeBuilder().xayMong().xayTuong().xayMai().build();
nha.moTa();
```

### C#

```csharp
public class NhaXe
{
    public string Mong, Tuong, Mai;
    public void MoTa() => Console.WriteLine($"{Mong} - {Tuong} - {Mai}");
}

public class NhaXeBuilder
{
    private readonly NhaXe nha = new NhaXe();
    public NhaXeBuilder XayMong() { nha.Mong = "Mong be tong"; return this; }
    public NhaXeBuilder XayTuong() { nha.Tuong = "Tuong gach"; return this; }
    public NhaXeBuilder XayMai() { nha.Mai = "Mai ton"; return this; }
    public NhaXe Build() => nha;
}

// Sử dụng:
var nha = new NhaXeBuilder().XayMong().XayTuong().XayMai().Build();
nha.MoTa();
```

---

## 5. Prototype ⭐

**Mục đích:** Tạo đối tượng mới bằng cách **sao chép (clone)** một đối tượng mẫu (prototype) có sẵn, thay vì khởi tạo từ đầu — hữu ích khi việc tạo đối tượng tốn kém (đọc file, gọi DB...) hoặc cần nhân bản nhanh với cấu hình gần giống nhau.

### C++

```cpp
class HinhDang {
public:
    virtual std::unique_ptr<HinhDang> clone() const = 0;
    virtual void ve() const = 0;
    virtual ~HinhDang() = default;
};
class HinhTron : public HinhDang {
    int banKinh;
public:
    HinhTron(int bk) : banKinh(bk) {}
    std::unique_ptr<HinhDang> clone() const override {
        return std::make_unique<HinhTron>(*this);   // dùng copy constructor
    }
    void ve() const override { std::cout << "Hinh tron, ban kinh = " << banKinh << std::endl; }
};

// Sử dụng:
std::unique_ptr<HinhDang> mau = std::make_unique<HinhTron>(10);
std::unique_ptr<HinhDang> banSao = mau->clone();
banSao->ve();
```

### C#

```csharp
public abstract class HinhDang
{
    public abstract HinhDang Clone();
    public abstract void Ve();
}
public class HinhTron : HinhDang
{
    public int BanKinh;
    public HinhTron(int bk) { BanKinh = bk; }
    public override HinhDang Clone() => (HinhDang)this.MemberwiseClone(); // shallow clone có sẵn của .NET
    public override void Ve() => Console.WriteLine($"Hinh tron, ban kinh = {BanKinh}");
}

// Sử dụng:
var mau = new HinhTron(10);
var banSao = mau.Clone();
banSao.Ve();
```

> Lưu ý: `MemberwiseClone()` trong C# chỉ **shallow copy** — nếu object có thành viên tham chiếu (reference type) cần clone sâu, phải tự viết logic deep copy tương tự copy constructor trong C++.

---

# B. STRUCTURAL PATTERNS (NHÓM CẤU TRÚC)

> Định nghĩa cách các class/object kết hợp với nhau để tạo thành cấu trúc lớn hơn, linh hoạt và dễ mở rộng hơn.

## 6. Adapter ⭐⭐⭐

**Mục đích:** Chuyển đổi interface của một class thành interface khác mà client mong đợi — giúp các class **không tương thích** có thể làm việc chung với nhau. Giống "đầu chuyển nguồn điện" giữa các chuẩn ổ cắm khác nhau.

**Ví dụ thực tế:** tích hợp thư viện thanh toán bên thứ 3 có interface khác với interface chuẩn hệ thống đang dùng.

### C++

```cpp
// Interface hệ thống đang dùng
class ThanhToanChuan { public: virtual void thanhToan(double soTien) = 0; virtual ~ThanhToanChuan()=default; };

// Thư viện bên thứ 3, interface khác, không sửa được code của họ
class VNPayAPI {
public:
    void pay(double amount) { std::cout << "VNPay thanh toan: " << amount << std::endl; }
};

// Adapter: "khoác áo" VNPayAPI để nó tuân theo ThanhToanChuan
class VNPayAdapter : public ThanhToanChuan {
    VNPayAPI vnpay;
public:
    void thanhToan(double soTien) override { vnpay.pay(soTien); }
};

// Sử dụng: client chỉ biết ThanhToanChuan, không cần biết VNPayAPI
std::unique_ptr<ThanhToanChuan> tt = std::make_unique<VNPayAdapter>();
tt->thanhToan(500000);
```

### C#

```csharp
public interface IThanhToanChuan { void ThanhToan(double soTien); }

public class VNPayAPI  // thư viện bên thứ ba
{
    public void Pay(double amount) => Console.WriteLine($"VNPay thanh toan: {amount}");
}

public class VNPayAdapter : IThanhToanChuan
{
    private readonly VNPayAPI vnpay = new VNPayAPI();
    public void ThanhToan(double soTien) => vnpay.Pay(soTien);
}

// Sử dụng:
IThanhToanChuan tt = new VNPayAdapter();
tt.ThanhToan(500000);
```

---

## 7. Bridge ⭐

**Mục đích:** Tách một abstraction ra khỏi phần cài đặt (implementation) của nó, để cả hai có thể **thay đổi độc lập**. Tránh việc kế thừa bùng nổ tổ hợp (VD: Hình × Màu → nếu dùng kế thừa sẽ ra N×M class con).

### C++

```cpp
// Implementation
class ThietBi { public: virtual void batAmThanh(int vol) = 0; virtual ~ThietBi()=default; };
class TV : public ThietBi { public: void batAmThanh(int vol) override { std::cout << "TV am luong " << vol << std::endl; } };
class Radio : public ThietBi { public: void batAmThanh(int vol) override { std::cout << "Radio am luong " << vol << std::endl; } };

// Abstraction — chứa "cầu nối" tới ThietBi thay vì kế thừa nó
class DieuKhien {
protected:
    ThietBi* thietBi;
public:
    DieuKhien(ThietBi* tb) : thietBi(tb) {}
    virtual void tangAmLuong() = 0;
    virtual ~DieuKhien() = default;
};
class DieuKhienCoBan : public DieuKhien {
public:
    DieuKhienCoBan(ThietBi* tb) : DieuKhien(tb) {}
    void tangAmLuong() override { thietBi->batAmThanh(10); }
};

// Sử dụng: kết hợp bất kỳ DieuKhien nào với bất kỳ ThietBi nào — độc lập nhau
TV tv;
DieuKhienCoBan dk(&tv);
dk.tangAmLuong();
```

### C#

```csharp
public interface IThietBi { void BatAmThanh(int vol); }
public class TV : IThietBi { public void BatAmThanh(int vol) => Console.WriteLine($"TV am luong {vol}"); }
public class Radio : IThietBi { public void BatAmThanh(int vol) => Console.WriteLine($"Radio am luong {vol}"); }

public abstract class DieuKhien
{
    protected IThietBi thietBi;
    public DieuKhien(IThietBi tb) { thietBi = tb; }
    public abstract void TangAmLuong();
}
public class DieuKhienCoBan : DieuKhien
{
    public DieuKhienCoBan(IThietBi tb) : base(tb) { }
    public override void TangAmLuong() => thietBi.BatAmThanh(10);
}

// Sử dụng:
var dk = new DieuKhienCoBan(new TV());
dk.TangAmLuong();
```

---

## 8. Composite ⭐⭐

**Mục đích:** Tổ chức các đối tượng thành **cấu trúc cây phân cấp** (part-whole hierarchy), cho phép client xử lý object đơn lẻ và tổ hợp object theo **cùng một cách** (cùng interface).

**Ví dụ thực tế:** hệ thống thư mục/file (Folder chứa File và Folder con), cây UI component (Panel chứa Button, Panel con...).

### C++

```cpp
class FileSystemItem {
public:
    virtual void hienThi(int indent = 0) const = 0;
    virtual ~FileSystemItem() = default;
};
class File : public FileSystemItem {
    std::string ten;
public:
    File(std::string t) : ten(t) {}
    void hienThi(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "- " << ten << std::endl;
    }
};
class Folder : public FileSystemItem {
    std::string ten;
    std::vector<std::unique_ptr<FileSystemItem>> con;
public:
    Folder(std::string t) : ten(t) {}
    void them(std::unique_ptr<FileSystemItem> item) { con.push_back(std::move(item)); }
    void hienThi(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "+ " << ten << std::endl;
        for (auto& c : con) c->hienThi(indent + 2);  // đệ quy — Folder xử lý con giống File
    }
};

// Sử dụng:
auto root = std::make_unique<Folder>("root");
root->them(std::make_unique<File>("a.txt"));
auto sub = std::make_unique<Folder>("sub");
sub->them(std::make_unique<File>("b.txt"));
root->them(std::move(sub));
root->hienThi();
```

### C#

```csharp
public abstract class FileSystemItem
{
    public abstract void HienThi(int indent = 0);
}
public class FileItem : FileSystemItem
{
    private readonly string ten;
    public FileItem(string t) { ten = t; }
    public override void HienThi(int indent = 0) => Console.WriteLine(new string(' ', indent) + "- " + ten);
}
public class Folder : FileSystemItem
{
    private readonly string ten;
    private readonly List<FileSystemItem> con = new List<FileSystemItem>();
    public Folder(string t) { ten = t; }
    public void Them(FileSystemItem item) => con.Add(item);
    public override void HienThi(int indent = 0)
    {
        Console.WriteLine(new string(' ', indent) + "+ " + ten);
        foreach (var c in con) c.HienThi(indent + 2);
    }
}

// Sử dụng:
var root = new Folder("root");
root.Them(new FileItem("a.txt"));
var sub = new Folder("sub");
sub.Them(new FileItem("b.txt"));
root.Them(sub);
root.HienThi();
```

---

## 9. Decorator ⭐⭐⭐

**Mục đích:** Gắn thêm trách nhiệm/hành vi mới cho object **tại runtime**, một cách linh hoạt, thay vì dùng kế thừa tĩnh (tránh bùng nổ số lượng class con).

**Ví dụ thực tế:** pha cà phê thêm Sữa/Đường/Đá, luồng I/O có thể bọc thêm Buffer/Compression/Encryption (giống `java.io` hoặc `System.IO.Stream` decorators).

### C++

```cpp
class CaPhe { public: virtual std::string moTa() const = 0; virtual double gia() const = 0; virtual ~CaPhe()=default; };
class CaPheDen : public CaPhe {
public:
    std::string moTa() const override { return "Ca phe den"; }
    double gia() const override { return 20000; }
};

// Decorator cơ sở — cũng LÀ MỘT CaPhe, đồng thời BỌC một CaPhe khác
class CaPheDecorator : public CaPhe {
protected:
    std::unique_ptr<CaPhe> caPheGoc;
public:
    CaPheDecorator(std::unique_ptr<CaPhe> c) : caPheGoc(std::move(c)) {}
};
class ThemSua : public CaPheDecorator {
public:
    ThemSua(std::unique_ptr<CaPhe> c) : CaPheDecorator(std::move(c)) {}
    std::string moTa() const override { return caPheGoc->moTa() + " + Sua"; }
    double gia() const override { return caPheGoc->gia() + 5000; }
};

// Sử dụng: chồng nhiều lớp decorator tùy ý
std::unique_ptr<CaPhe> ly = std::make_unique<ThemSua>(std::make_unique<CaPheDen>());
std::cout << ly->moTa() << " - " << ly->gia() << std::endl;
```

### C#

```csharp
public abstract class CaPhe
{
    public abstract string MoTa();
    public abstract double Gia();
}
public class CaPheDen : CaPhe
{
    public override string MoTa() => "Ca phe den";
    public override double Gia() => 20000;
}
public abstract class CaPheDecorator : CaPhe
{
    protected CaPhe caPheGoc;
    public CaPheDecorator(CaPhe c) { caPheGoc = c; }
}
public class ThemSua : CaPheDecorator
{
    public ThemSua(CaPhe c) : base(c) { }
    public override string MoTa() => caPheGoc.MoTa() + " + Sua";
    public override double Gia() => caPheGoc.Gia() + 5000;
}

// Sử dụng:
CaPhe ly = new ThemSua(new CaPheDen());
Console.WriteLine($"{ly.MoTa()} - {ly.Gia()}");
```

---

## 10. Facade ⭐⭐⭐

**Mục đích:** Cung cấp một interface **đơn giản, thống nhất** cho một tập hợp interface phức tạp bên trong subsystem — client không cần biết chi tiết các thành phần bên trong hoạt động ra sao.

**Ví dụ thực tế:** class `OrderService.DatHang()` gọi ẩn bên trong `KiemTraKho`, `TinhPhiVanChuyen`, `XuLyThanhToan`, `GuiEmail`...

### C++

```cpp
class KhoHang { public: void kiemTraTon(std::string sp) { std::cout << "Kiem tra ton kho: " << sp << std::endl; } };
class ThanhToan { public: void xuLy(double tien) { std::cout << "Xu ly thanh toan: " << tien << std::endl; } };
class VanChuyen { public: void datVanChuyen() { std::cout << "Dat don vi van chuyen\n"; } };

// Facade — gom logic phức tạp thành 1 hàm duy nhất
class DatHangFacade {
    KhoHang kho; ThanhToan tt; VanChuyen vc;
public:
    void datHang(std::string sp, double gia) {
        kho.kiemTraTon(sp);
        tt.xuLy(gia);
        vc.datVanChuyen();
        std::cout << "Dat hang thanh cong!\n";
    }
};

// Sử dụng: client chỉ gọi 1 dòng, không cần biết 3 bước bên trong
DatHangFacade().datHang("Laptop", 20000000);
```

### C#

```csharp
public class KhoHang { public void KiemTraTon(string sp) => Console.WriteLine($"Kiem tra ton kho: {sp}"); }
public class ThanhToan { public void XuLy(double tien) => Console.WriteLine($"Xu ly thanh toan: {tien}"); }
public class VanChuyen { public void DatVanChuyen() => Console.WriteLine("Dat don vi van chuyen"); }

public class DatHangFacade
{
    private readonly KhoHang kho = new KhoHang();
    private readonly ThanhToan tt = new ThanhToan();
    private readonly VanChuyen vc = new VanChuyen();

    public void DatHang(string sp, double gia)
    {
        kho.KiemTraTon(sp);
        tt.XuLy(gia);
        vc.DatVanChuyen();
        Console.WriteLine("Dat hang thanh cong!");
    }
}

// Sử dụng:
new DatHangFacade().DatHang("Laptop", 20000000);
```

---

## 11. Flyweight ⭐

**Mục đích:** Tối ưu bộ nhớ khi cần tạo **số lượng rất lớn object tương tự nhau** — bằng cách chia sẻ (share) phần dữ liệu chung (intrinsic state), chỉ lưu riêng phần khác biệt (extrinsic state) cho từng object.

**Ví dụ thực tế:** hiển thị hàng triệu ký tự trong trình soạn thảo văn bản (font/style dùng chung), hoặc cây trong game (model/texture dùng chung, chỉ vị trí là riêng).

### C++

```cpp
class KyTuFont {   // phần dùng chung (intrinsic) — tốn bộ nhớ nếu tạo lại mỗi lần
    char kyTu; std::string font;
public:
    KyTuFont(char k, std::string f) : kyTu(k), font(f) {}
    void ve(int x, int y) const {  // x, y là extrinsic — truyền vào khi cần, không lưu trong object dùng chung
        std::cout << "Ve '" << kyTu << "' font " << font << " tai (" << x << "," << y << ")\n";
    }
};

class FontFactory {
    std::map<std::string, std::unique_ptr<KyTuFont>> cache;
public:
    KyTuFont* layKyTu(char k, const std::string& font) {
        std::string key = std::string(1, k) + font;
        if (cache.find(key) == cache.end())
            cache[key] = std::make_unique<KyTuFont>(k, font);
        return cache[key].get();   // trả về object DÙNG CHUNG, không tạo mới nếu đã có
    }
};

// Sử dụng:
FontFactory factory;
factory.layKyTu('A', "Arial")->ve(0, 0);
factory.layKyTu('A', "Arial")->ve(10, 0);  // dùng lại cùng 1 object 'A'-Arial
```

### C#

```csharp
public class KyTuFont
{
    private readonly char kyTu; private readonly string font;
    public KyTuFont(char k, string f) { kyTu = k; font = f; }
    public void Ve(int x, int y) => Console.WriteLine($"Ve '{kyTu}' font {font} tai ({x},{y})");
}

public class FontFactory
{
    private readonly Dictionary<string, KyTuFont> cache = new Dictionary<string, KyTuFont>();
    public KyTuFont LayKyTu(char k, string font)
    {
        string key = k + font;
        if (!cache.ContainsKey(key)) cache[key] = new KyTuFont(k, font);
        return cache[key];
    }
}

// Sử dụng:
var factory = new FontFactory();
factory.LayKyTu('A', "Arial").Ve(0, 0);
factory.LayKyTu('A', "Arial").Ve(10, 0);
```

---

## 12. Proxy ⭐⭐

**Mục đích:** Cung cấp một object **thay mặt/đại diện** cho object thật, kiểm soát truy cập tới nó — dùng để: lazy loading (chỉ tạo object thật khi cần), kiểm soát quyền truy cập, logging, caching...

**Ví dụ thực tế:** load ảnh độ phân giải cao chỉ khi thực sự hiển thị (Virtual Proxy), kiểm tra quyền trước khi cho gọi API thật (Protection Proxy).

### C++

```cpp
class Anh { public: virtual void hienThi() = 0; virtual ~Anh() = default; };

class AnhThat : public Anh {
    std::string duongDan;
public:
    AnhThat(std::string path) : duongDan(path) {
        std::cout << "Dang tai anh tu file (ton kem): " << duongDan << std::endl;
    }
    void hienThi() override { std::cout << "Hien thi anh: " << duongDan << std::endl; }
};

class AnhProxy : public Anh {   // Proxy — trì hoãn việc tạo AnhThat tới khi thực sự cần
    std::string duongDan;
    std::unique_ptr<AnhThat> anhThat;
public:
    AnhProxy(std::string path) : duongDan(path) {}
    void hienThi() override {
        if (!anhThat) anhThat = std::make_unique<AnhThat>(duongDan);  // lazy init
        anhThat->hienThi();
    }
};

// Sử dụng: tạo AnhProxy không tốn gì cả, chỉ khi gọi hienThi() lần đầu mới thực sự load
Anh* anh = new AnhProxy("photo.jpg");
anh->hienThi();
delete anh;
```

### C#

```csharp
public interface IAnh { void HienThi(); }

public class AnhThat : IAnh
{
    private readonly string duongDan;
    public AnhThat(string path)
    {
        duongDan = path;
        Console.WriteLine($"Dang tai anh tu file (ton kem): {duongDan}");
    }
    public void HienThi() => Console.WriteLine($"Hien thi anh: {duongDan}");
}

public class AnhProxy : IAnh
{
    private readonly string duongDan;
    private AnhThat anhThat;
    public AnhProxy(string path) { duongDan = path; }
    public void HienThi()
    {
        anhThat ??= new AnhThat(duongDan);
        anhThat.HienThi();
    }
}

// Sử dụng:
IAnh anh = new AnhProxy("photo.jpg");
anh.HienThi();
```

---

# C. BEHAVIORAL PATTERNS (NHÓM HÀNH VI)

> Tập trung vào cách các object **giao tiếp và phân chia trách nhiệm** với nhau.

## 13. Chain of Responsibility ⭐⭐

**Mục đích:** Cho một request đi qua **một chuỗi các handler**, mỗi handler tự quyết định xử lý hay chuyển tiếp cho handler tiếp theo. Tránh việc client phải biết chính xác ai xử lý request.

**Ví dụ thực tế:** hệ thống duyệt đơn nghỉ phép (Trưởng nhóm → Quản lý → Giám đốc tùy số ngày), middleware xử lý HTTP request.

### C++

```cpp
class DonNghiPhep { public: int soNgay; };

class NguoiDuyet {
protected:
    std::unique_ptr<NguoiDuyet> tiepTheo;
public:
    void thietLapTiepTheo(std::unique_ptr<NguoiDuyet> n) { tiepTheo = std::move(n); }
    virtual void xuLy(const DonNghiPhep& don) {
        if (tiepTheo) tiepTheo->xuLy(don);
        else std::cout << "Khong ai duyet duoc don nay\n";
    }
    virtual ~NguoiDuyet() = default;
};
class TruongNhom : public NguoiDuyet {
public:
    void xuLy(const DonNghiPhep& don) override {
        if (don.soNgay <= 2) std::cout << "Truong nhom duyet don " << don.soNgay << " ngay\n";
        else NguoiDuyet::xuLy(don);   // chuyển tiếp
    }
};
class QuanLy : public NguoiDuyet {
public:
    void xuLy(const DonNghiPhep& don) override {
        if (don.soNgay <= 5) std::cout << "Quan ly duyet don " << don.soNgay << " ngay\n";
        else NguoiDuyet::xuLy(don);
    }
};

// Sử dụng:
auto truongNhom = std::make_unique<TruongNhom>();
truongNhom->thietLapTiepTheo(std::make_unique<QuanLy>());
truongNhom->xuLy(DonNghiPhep{3});  // Quản lý sẽ duyệt vì > 2 ngày
```

### C#

```csharp
public class DonNghiPhep { public int SoNgay; }

public abstract class NguoiDuyet
{
    protected NguoiDuyet tiepTheo;
    public void ThietLapTiepTheo(NguoiDuyet n) => tiepTheo = n;
    public virtual void XuLy(DonNghiPhep don)
    {
        if (tiepTheo != null) tiepTheo.XuLy(don);
        else Console.WriteLine("Khong ai duyet duoc don nay");
    }
}
public class TruongNhom : NguoiDuyet
{
    public override void XuLy(DonNghiPhep don)
    {
        if (don.SoNgay <= 2) Console.WriteLine($"Truong nhom duyet don {don.SoNgay} ngay");
        else base.XuLy(don);
    }
}
public class QuanLy : NguoiDuyet
{
    public override void XuLy(DonNghiPhep don)
    {
        if (don.SoNgay <= 5) Console.WriteLine($"Quan ly duyet don {don.SoNgay} ngay");
        else base.XuLy(don);
    }
}

// Sử dụng:
var truongNhom = new TruongNhom();
truongNhom.ThietLapTiepTheo(new QuanLy());
truongNhom.XuLy(new DonNghiPhep { SoNgay = 3 });
```

---

## 14. Command ⭐⭐⭐

**Mục đích:** Đóng gói một request/hành động thành một object độc lập — cho phép tham số hóa hành động, xếp hàng đợi (queue), ghi log, và **hỗ trợ undo/redo**.

**Ví dụ thực tế:** nút bấm trong GUI (mỗi nút gắn 1 Command), hệ thống Undo/Redo trong trình soạn thảo.

### C++

```cpp
class Command { public: virtual void thucThi() = 0; virtual void huy() = 0; virtual ~Command()=default; };

class DenLed {
public:
    void bat() { std::cout << "Den: BAT\n"; }
    void tat() { std::cout << "Den: TAT\n"; }
};
class BatDenCommand : public Command {
    DenLed& den;
public:
    BatDenCommand(DenLed& d) : den(d) {}
    void thucThi() override { den.bat(); }
    void huy() override { den.tat(); }
};

class CongTac {   // Invoker — không cần biết chi tiết DenLed hoạt động ra sao
    std::vector<std::unique_ptr<Command>> lichSu;
public:
    void nhan(std::unique_ptr<Command> cmd) {
        cmd->thucThi();
        lichSu.push_back(std::move(cmd));
    }
    void undo() {
        if (!lichSu.empty()) { lichSu.back()->huy(); lichSu.pop_back(); }
    }
};

// Sử dụng:
DenLed den;
CongTac congTac;
congTac.nhan(std::make_unique<BatDenCommand>(den));
congTac.undo();   // tự động gọi den.tat()
```

### C#

```csharp
public interface ICommand { void ThucThi(); void Huy(); }

public class DenLed
{
    public void Bat() => Console.WriteLine("Den: BAT");
    public void Tat() => Console.WriteLine("Den: TAT");
}
public class BatDenCommand : ICommand
{
    private readonly DenLed den;
    public BatDenCommand(DenLed d) { den = d; }
    public void ThucThi() => den.Bat();
    public void Huy() => den.Tat();
}

public class CongTac
{
    private readonly Stack<ICommand> lichSu = new Stack<ICommand>();
    public void Nhan(ICommand cmd) { cmd.ThucThi(); lichSu.Push(cmd); }
    public void Undo() { if (lichSu.Count > 0) lichSu.Pop().Huy(); }
}

// Sử dụng:
var den = new DenLed();
var congTac = new CongTac();
congTac.Nhan(new BatDenCommand(den));
congTac.Undo();
```

---

## 15. Interpreter ⭐

**Mục đích:** Định nghĩa cách biểu diễn ngữ pháp của một ngôn ngữ đơn giản, và xây dựng interpreter để "diễn giải" các câu trong ngôn ngữ đó. Ít dùng trong ứng dụng thường ngày, chủ yếu gặp khi viết công cụ phân tích biểu thức, engine rule, hoặc mini DSL.

**Ví dụ thực tế:** máy tính biểu thức số học đơn giản `"5 + 3"`.

### C++

```cpp
class BieuThuc { public: virtual int dienGiai() const = 0; virtual ~BieuThuc()=default; };

class So : public BieuThuc {
    int giaTri;
public:
    So(int v) : giaTri(v) {}
    int dienGiai() const override { return giaTri; }
};
class PhepCong : public BieuThuc {
    std::unique_ptr<BieuThuc> trai, phai;
public:
    PhepCong(std::unique_ptr<BieuThuc> t, std::unique_ptr<BieuThuc> p) : trai(std::move(t)), phai(std::move(p)) {}
    int dienGiai() const override { return trai->dienGiai() + phai->dienGiai(); }
};

// Sử dụng: biểu diễn "5 + 3" bằng cây object
BieuThuc* bt = new PhepCong(std::make_unique<So>(5), std::make_unique<So>(3));
std::cout << bt->dienGiai() << std::endl;  // 8
delete bt;
```

### C#

```csharp
public interface IBieuThuc { int DienGiai(); }

public class So : IBieuThuc
{
    private readonly int giaTri;
    public So(int v) { giaTri = v; }
    public int DienGiai() => giaTri;
}
public class PhepCong : IBieuThuc
{
    private readonly IBieuThuc trai, phai;
    public PhepCong(IBieuThuc t, IBieuThuc p) { trai = t; phai = p; }
    public int DienGiai() => trai.DienGiai() + phai.DienGiai();
}

// Sử dụng:
IBieuThuc bt = new PhepCong(new So(5), new So(3));
Console.WriteLine(bt.DienGiai());  // 8
```

---

## 16. Iterator ⭐⭐⭐

**Mục đích:** Cung cấp cách **duyệt qua các phần tử** của một collection mà không cần lộ ra cấu trúc lưu trữ bên trong (mảng, linked list, cây...). C++ (`begin()/end()`) và C# (`IEnumerable`/`foreach`) đã tích hợp sẵn pattern này ở tầng ngôn ngữ.

### C++

```cpp
class BoSuuTap {
    std::vector<std::string> items;
public:
    void them(std::string s) { items.push_back(s); }
    // Cung cấp iterator chuẩn để dùng range-based for
    auto begin() { return items.begin(); }
    auto end() { return items.end(); }
};

// Sử dụng: nhờ có begin()/end(), dùng được range-based for tự nhiên
BoSuuTap bst;
bst.them("A"); bst.them("B");
for (const auto& item : bst) std::cout << item << std::endl;
```

### C#

```csharp
public class BoSuuTap : IEnumerable<string>
{
    private readonly List<string> items = new List<string>();
    public void Them(string s) => items.Add(s);
    public IEnumerator<string> GetEnumerator() => items.GetEnumerator();
    IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
}

// Sử dụng: nhờ implement IEnumerable, dùng được foreach tự nhiên
var bst = new BoSuuTap();
bst.Them("A"); bst.Them("B");
foreach (var item in bst) Console.WriteLine(item);
```

---

## 17. Mediator ⭐⭐

**Mục đích:** Giảm sự phụ thuộc chằng chịt giữa nhiều object bằng cách đưa hết logic giao tiếp qua **một đối tượng trung gian (mediator)** — các object không gọi trực tiếp lẫn nhau nữa, chỉ giao tiếp qua mediator.

**Ví dụ thực tế:** phòng chat (mọi user gửi tin nhắn qua ChatRoom thay vì gọi trực tiếp lẫn nhau), Air Traffic Control điều phối máy bay.

### C++

```cpp
class ChatRoom;
class NguoiDung {
    std::string ten;
    ChatRoom* phong;
public:
    NguoiDung(std::string t, ChatRoom* p) : ten(t), phong(p) {}
    std::string layTen() const { return ten; }
    void gui(const std::string& msg);
    void nhan(const std::string& msg) { std::cout << ten << " nhan: " << msg << std::endl; }
};
class ChatRoom {   // Mediator
    std::vector<NguoiDung*> thanhVien;
public:
    void tham(NguoiDung* nd) { thanhVien.push_back(nd); }
    void guiTin(NguoiDung* nguoiGui, const std::string& msg) {
        for (auto* nd : thanhVien)
            if (nd != nguoiGui) nd->nhan(nguoiGui->layTen() + ": " + msg);
    }
};
void NguoiDung::gui(const std::string& msg) { phong->guiTin(this, msg); }

// Sử dụng:
ChatRoom phong;
NguoiDung a("An", &phong), b("Binh", &phong);
phong.tham(&a); phong.tham(&b);
a.gui("Xin chao!");   // b sẽ nhận, a và b không gọi trực tiếp lẫn nhau
```

### C#

```csharp
public class ChatRoom  // Mediator
{
    private readonly List<NguoiDung> thanhVien = new List<NguoiDung>();
    public void Tham(NguoiDung nd) => thanhVien.Add(nd);
    public void GuiTin(NguoiDung nguoiGui, string msg)
    {
        foreach (var nd in thanhVien)
            if (nd != nguoiGui) nd.Nhan($"{nguoiGui.Ten}: {msg}");
    }
}
public class NguoiDung
{
    public string Ten;
    private readonly ChatRoom phong;
    public NguoiDung(string ten, ChatRoom p) { Ten = ten; phong = p; }
    public void Gui(string msg) => phong.GuiTin(this, msg);
    public void Nhan(string msg) => Console.WriteLine($"{Ten} nhan: {msg}");
}

// Sử dụng:
var phong = new ChatRoom();
var a = new NguoiDung("An", phong);
var b = new NguoiDung("Binh", phong);
phong.Tham(a); phong.Tham(b);
a.Gui("Xin chao!");
```

---

## 18. Memento ⭐⭐

**Mục đích:** Lưu lại (snapshot) trạng thái nội bộ của object mà **không vi phạm encapsulation**, để có thể khôi phục lại trạng thái đó sau này. Nền tảng cho tính năng Undo.

**Ví dụ thực tế:** Undo trong trình soạn thảo văn bản, save/load game state.

### C++

```cpp
class Memento {   // "ảnh chụp" trạng thái — immutable
    std::string noiDung;
public:
    Memento(std::string s) : noiDung(s) {}
    std::string layNoiDung() const { return noiDung; }
};
class VanBan {   // Originator
    std::string noiDung;
public:
    void soan(std::string s) { noiDung = s; }
    std::string xem() const { return noiDung; }
    Memento luuTrangThai() const { return Memento(noiDung); }
    void khoiPhuc(const Memento& m) { noiDung = m.layNoiDung(); }
};
class LichSuUndo {   // Caretaker — chỉ giữ Memento, không đọc/sửa nội dung bên trong
    std::vector<Memento> lichSu;
public:
    void luu(const Memento& m) { lichSu.push_back(m); }
    Memento layGanNhat() { Memento m = lichSu.back(); lichSu.pop_back(); return m; }
};

// Sử dụng:
VanBan vb; LichSuUndo undo;
vb.soan("Ban nhap 1"); undo.luu(vb.luuTrangThai());
vb.soan("Ban nhap 2");
vb.khoiPhuc(undo.layGanNhat());   // quay lại "Ban nhap 1"
std::cout << vb.xem() << std::endl;
```

### C#

```csharp
public class Memento
{
    public string NoiDung { get; }
    public Memento(string s) { NoiDung = s; }
}
public class VanBan
{
    private string noiDung;
    public void Soan(string s) => noiDung = s;
    public string Xem() => noiDung;
    public Memento LuuTrangThai() => new Memento(noiDung);
    public void KhoiPhuc(Memento m) => noiDung = m.NoiDung;
}
public class LichSuUndo
{
    private readonly Stack<Memento> lichSu = new Stack<Memento>();
    public void Luu(Memento m) => lichSu.Push(m);
    public Memento LayGanNhat() => lichSu.Pop();
}

// Sử dụng:
var vb = new VanBan(); var undo = new LichSuUndo();
vb.Soan("Ban nhap 1"); undo.Luu(vb.LuuTrangThai());
vb.Soan("Ban nhap 2");
vb.KhoiPhuc(undo.LayGanNhat());
Console.WriteLine(vb.Xem());
```

---

## 19. Observer ⭐⭐⭐

**Mục đích:** Định nghĩa quan hệ **một-nhiều** giữa các object — khi 1 object (Subject) thay đổi trạng thái, tất cả object phụ thuộc (Observer) tự động được thông báo. Nền tảng của Event/Callback system, MVC, Reactive Programming.

**Ví dụ thực tế:** hệ thống thông báo (Subject = kênh Youtube, Observer = subscriber), event trong UI, `INotifyPropertyChanged` trong WPF.

### C++

```cpp
class Observer { public: virtual void capNhat(float gia) = 0; virtual ~Observer()=default; };

class ChungKhoan {   // Subject
    std::vector<Observer*> danhSach;
    float gia;
public:
    void dangKy(Observer* o) { danhSach.push_back(o); }
    void thayDoiGia(float g) {
        gia = g;
        for (auto* o : danhSach) o->capNhat(gia);   // thông báo tất cả observer
    }
};
class NhaDauTu : public Observer {
    std::string ten;
public:
    NhaDauTu(std::string t) : ten(t) {}
    void capNhat(float gia) override { std::cout << ten << " nhan tin: gia = " << gia << std::endl; }
};

// Sử dụng:
ChungKhoan ck;
NhaDauTu nd1("An"), nd2("Binh");
ck.dangKy(&nd1); ck.dangKy(&nd2);
ck.thayDoiGia(105.5);   // cả 2 nhà đầu tư đều được báo tự động
```

### C#

```csharp
public interface IObserver { void CapNhat(float gia); }

public class ChungKhoan
{
    private readonly List<IObserver> danhSach = new List<IObserver>();
    public void DangKy(IObserver o) => danhSach.Add(o);
    public void ThayDoiGia(float gia)
    {
        foreach (var o in danhSach) o.CapNhat(gia);
    }
}
public class NhaDauTu : IObserver
{
    private readonly string ten;
    public NhaDauTu(string t) { ten = t; }
    public void CapNhat(float gia) => Console.WriteLine($"{ten} nhan tin: gia = {gia}");
}

// Sử dụng (C# còn có event/delegate tích hợp sẵn để làm điều này gọn hơn):
var ck = new ChungKhoan();
var nd1 = new NhaDauTu("An"); var nd2 = new NhaDauTu("Binh");
ck.DangKy(nd1); ck.DangKy(nd2);
ck.ThayDoiGia(105.5f);
```

---

## 20. State ⭐⭐

**Mục đích:** Cho phép object **thay đổi hành vi khi trạng thái nội bộ thay đổi** — nhìn bề ngoài như object đã đổi class. Thay thế các khối `if/switch` khổng lồ kiểm tra trạng thái bằng cách đưa mỗi trạng thái thành 1 class riêng.

**Ví dụ thực tế:** máy bán hàng tự động (trạng thái: ChoTien → DaNhanTien → DangGiaoHang), trạng thái đơn hàng (MoiTao → DaXacNhan → DangGiao → HoanThanh).

### C++

```cpp
class MayBanHang;
class TrangThai { public: virtual void nhanTien(MayBanHang& m) = 0; virtual void layHang(MayBanHang& m) = 0; virtual ~TrangThai()=default; };

class MayBanHang {
    std::unique_ptr<TrangThai> trangThai;
public:
    MayBanHang(std::unique_ptr<TrangThai> t) : trangThai(std::move(t)) {}
    void doiTrangThai(std::unique_ptr<TrangThai> t) { trangThai = std::move(t); }
    void nhanTien() { trangThai->nhanTien(*this); }
    void layHang() { trangThai->layHang(*this); }
};

class ChoTien : public TrangThai {
public:
    void nhanTien(MayBanHang& m) override;  // định nghĩa bên dưới do cần forward declare
    void layHang(MayBanHang&) override { std::cout << "Chua nhan tien, khong lay duoc hang\n"; }
};
class DaNhanTien : public TrangThai {
public:
    void nhanTien(MayBanHang&) override { std::cout << "Da nhan tien roi\n"; }
    void layHang(MayBanHang& m) override {
        std::cout << "Dang giao hang...\n";
        m.doiTrangThai(std::make_unique<ChoTien>());   // quay lại trạng thái đầu
    }
};
void ChoTien::nhanTien(MayBanHang& m) {
    std::cout << "Da nhan tien, moi lay hang\n";
    m.doiTrangThai(std::make_unique<DaNhanTien>());
}

// Sử dụng: hành vi thay đổi tự động theo trạng thái, không cần if/switch ở MayBanHang
MayBanHang may(std::make_unique<ChoTien>());
may.layHang();   // "Chua nhan tien..."
may.nhanTien();  // chuyển sang DaNhanTien
may.layHang();   // "Dang giao hang..."
```

### C#

```csharp
public interface ITrangThai
{
    void NhanTien(MayBanHang m);
    void LayHang(MayBanHang m);
}
public class MayBanHang
{
    private ITrangThai trangThai;
    public MayBanHang(ITrangThai t) { trangThai = t; }
    public void DoiTrangThai(ITrangThai t) => trangThai = t;
    public void NhanTien() => trangThai.NhanTien(this);
    public void LayHang() => trangThai.LayHang(this);
}
public class ChoTien : ITrangThai
{
    public void NhanTien(MayBanHang m)
    {
        Console.WriteLine("Da nhan tien, moi lay hang");
        m.DoiTrangThai(new DaNhanTien());
    }
    public void LayHang(MayBanHang m) => Console.WriteLine("Chua nhan tien, khong lay duoc hang");
}
public class DaNhanTien : ITrangThai
{
    public void NhanTien(MayBanHang m) => Console.WriteLine("Da nhan tien roi");
    public void LayHang(MayBanHang m)
    {
        Console.WriteLine("Dang giao hang...");
        m.DoiTrangThai(new ChoTien());
    }
}

// Sử dụng:
var may = new MayBanHang(new ChoTien());
may.LayHang();
may.NhanTien();
may.LayHang();
```

---

## 21. Strategy ⭐⭐⭐

**Mục đích:** Định nghĩa một họ thuật toán, đóng gói từng thuật toán riêng, cho phép **thay đổi thuật toán tại runtime** mà không sửa code client. Rất giống State nhưng khác ý định: Strategy chọn thuật toán theo _nhu cầu client_, State tự chuyển đổi theo _trạng thái nội bộ_.

**Ví dụ thực tế:** nhiều phương thức thanh toán (MoMo/VNPay/Tiền mặt), nhiều thuật toán sắp xếp/nén file có thể chọn tùy tình huống.

### C++

```cpp
class ChienLuocThanhToan { public: virtual void thanhToan(double soTien) = 0; virtual ~ChienLuocThanhToan()=default; };
class ThanhToanMoMo : public ChienLuocThanhToan {
public: void thanhToan(double s) override { std::cout << "Thanh toan qua MoMo: " << s << std::endl; }
};
class ThanhToanTienMat : public ChienLuocThanhToan {
public: void thanhToan(double s) override { std::cout << "Thanh toan tien mat: " << s << std::endl; }
};

class GioHang {
    std::unique_ptr<ChienLuocThanhToan> chienLuoc;
public:
    void datChienLuoc(std::unique_ptr<ChienLuocThanhToan> cl) { chienLuoc = std::move(cl); }
    void thanhToan(double soTien) { chienLuoc->thanhToan(soTien); }
};

// Sử dụng: đổi thuật toán bất kỳ lúc nào, GioHang không cần biết chi tiết
GioHang gh;
gh.datChienLuoc(std::make_unique<ThanhToanMoMo>());
gh.thanhToan(200000);
gh.datChienLuoc(std::make_unique<ThanhToanTienMat>());  // đổi runtime
gh.thanhToan(150000);
```

### C#

```csharp
public interface IChienLuocThanhToan { void ThanhToan(double soTien); }
public class ThanhToanMoMo : IChienLuocThanhToan
{
    public void ThanhToan(double s) => Console.WriteLine($"Thanh toan qua MoMo: {s}");
}
public class ThanhToanTienMat : IChienLuocThanhToan
{
    public void ThanhToan(double s) => Console.WriteLine($"Thanh toan tien mat: {s}");
}

public class GioHang
{
    private IChienLuocThanhToan chienLuoc;
    public void DatChienLuoc(IChienLuocThanhToan cl) => chienLuoc = cl;
    public void ThanhToan(double soTien) => chienLuoc.ThanhToan(soTien);
}

// Sử dụng:
var gh = new GioHang();
gh.DatChienLuoc(new ThanhToanMoMo());
gh.ThanhToan(200000);
gh.DatChienLuoc(new ThanhToanTienMat());
gh.ThanhToan(150000);
```

---

## 22. Template Method ⭐⭐⭐

**Mục đích:** Định nghĩa **bộ khung (skeleton)** của thuật toán trong lớp cha, để lớp con override một số bước cụ thể mà **không thay đổi cấu trúc tổng thể** của thuật toán. Đây chính là cơ chế đứng sau các framework (VD: React lifecycle, JUnit `setUp/test/tearDown`).

**Ví dụ thực tế:** quy trình pha đồ uống (Trà/Cà phê đều: Đun nước → Pha → Rót → Thêm gia vị, nhưng bước "Pha" và "Thêm gia vị" khác nhau).

### C++

```cpp
class PhaDoUong {
public:
    void phaChe() {    // template method — bộ khung KHÔNG cho override toàn bộ
        dunNuoc();
        pha();
        rotVaoLy();
        themGiaVi();
    }
    void dunNuoc() { std::cout << "Dun nuoc soi\n"; }
    void rotVaoLy() { std::cout << "Rot vao ly\n"; }
    virtual void pha() = 0;         // bước lớp con PHẢI cài đặt
    virtual void themGiaVi() = 0;   // bước lớp con PHẢI cài đặt
    virtual ~PhaDoUong() = default;
};
class PhaTra : public PhaDoUong {
public:
    void pha() override { std::cout << "Nga tra\n"; }
    void themGiaVi() override { std::cout << "Them chanh\n"; }
};
class PhaCaPhe : public PhaDoUong {
public:
    void pha() override { std::cout << "Loc ca phe\n"; }
    void themGiaVi() override { std::cout << "Them duong sua\n"; }
};

// Sử dụng: gọi cùng 1 hàm phaChe(), nhưng hành vi bước con khác nhau
std::unique_ptr<PhaDoUong> tra = std::make_unique<PhaTra>();
tra->phaChe();
```

### C#

```csharp
public abstract class PhaDoUong
{
    public void PhaChe()   // template method — sealed logic, dùng "public" thường (không virtual)
    {
        DunNuoc();
        Pha();
        RotVaoLy();
        ThemGiaVi();
    }
    public void DunNuoc() => Console.WriteLine("Dun nuoc soi");
    public void RotVaoLy() => Console.WriteLine("Rot vao ly");
    public abstract void Pha();
    public abstract void ThemGiaVi();
}
public class PhaTra : PhaDoUong
{
    public override void Pha() => Console.WriteLine("Nga tra");
    public override void ThemGiaVi() => Console.WriteLine("Them chanh");
}

// Sử dụng:
PhaDoUong tra = new PhaTra();
tra.PhaChe();
```

---

## 23. Visitor ⭐

**Mục đích:** Tách một thuật toán ra khỏi cấu trúc object mà nó thao tác — cho phép **thêm hành vi mới** cho một nhóm class mà **không cần sửa** các class đó, bằng cách đưa logic vào "Visitor" bên ngoài. Đánh đổi: nếu thêm class mới trong hệ thống thì phải sửa mọi Visitor.

**Ví dụ thực tế:** tính thuế/tính giá cho nhiều loại sản phẩm khác nhau (Sách/ĐồĐiệnTử/ThựcPhẩm) mà không muốn nhồi hết logic tính thuế vào từng class sản phẩm.

### C++

```cpp
class Sach; class DoDienTu;
class Visitor {
public:
    virtual void tham(Sach& s) = 0;
    virtual void tham(DoDienTu& d) = 0;
    virtual ~Visitor() = default;
};
class SanPham { public: virtual void chapNhan(Visitor& v) = 0; virtual ~SanPham()=default; };
class Sach : public SanPham {
public: double gia = 100000;
    void chapNhan(Visitor& v) override { v.tham(*this); }
};
class DoDienTu : public SanPham {
public: double gia = 5000000;
    void chapNhan(Visitor& v) override { v.tham(*this); }
};

class TinhThueVisitor : public Visitor {   // logic tính thuế nằm NGOÀI class sản phẩm
public:
    void tham(Sach& s) override { std::cout << "Thue sach (5%): " << s.gia * 0.05 << std::endl; }
    void tham(DoDienTu& d) override { std::cout << "Thue do dien tu (10%): " << d.gia * 0.10 << std::endl; }
};

// Sử dụng: thêm quy tắc tính mới chỉ cần viết Visitor mới, KHÔNG sửa Sach/DoDienTu
Sach sach; DoDienTu dientu;
TinhThueVisitor thue;
sach.chapNhan(thue);
dientu.chapNhan(thue);
```

### C#

```csharp
public interface IVisitor
{
    void Tham(Sach s);
    void Tham(DoDienTu d);
}
public abstract class SanPham { public abstract void ChapNhan(IVisitor v); }
public class Sach : SanPham
{
    public double Gia = 100000;
    public override void ChapNhan(IVisitor v) => v.Tham(this);
}
public class DoDienTu : SanPham
{
    public double Gia = 5000000;
    public override void ChapNhan(IVisitor v) => v.Tham(this);
}

public class TinhThueVisitor : IVisitor
{
    public void Tham(Sach s) => Console.WriteLine($"Thue sach (5%): {s.Gia * 0.05}");
    public void Tham(DoDienTu d) => Console.WriteLine($"Thue do dien tu (10%): {d.Gia * 0.10}");
}

// Sử dụng:
var sach = new Sach(); var dientu = new DoDienTu();
var thue = new TinhThueVisitor();
sach.ChapNhan(thue);
dientu.ChapNhan(thue);
```

---

# TỔNG KẾT

## Bảng so sánh nhanh — khi nào dùng pattern nào

| Vấn đề bạn gặp                                                                | Pattern nên nghĩ tới    |
| ----------------------------------------------------------------------------- | ----------------------- |
| Cần đúng 1 instance duy nhất (config, logger)                                 | Singleton               |
| Tạo object nhưng để lớp con quyết định loại nào                               | Factory Method          |
| Tạo cả họ object phải đồng bộ với nhau                                        | Abstract Factory        |
| Object có quá nhiều tham số tùy chọn khi khởi tạo                             | Builder                 |
| Tạo object mới bằng cách nhân bản object có sẵn                               | Prototype               |
| 2 interface không tương thích cần ghép nối                                    | Adapter                 |
| Muốn thêm hành vi runtime, tránh nổ tổ hợp class con                          | Decorator               |
| Subsystem phức tạp, muốn 1 cổng vào đơn giản                                  | Facade                  |
| Cấu trúc dữ liệu dạng cây (file/folder, UI tree)                              | Composite               |
| Cần kiểm soát truy cập / lazy load object nặng                                | Proxy                   |
| Nhiều object cần thay đổi hành vi theo thuật toán khác nhau, chọn tùy runtime | Strategy                |
| Hành vi object thay đổi theo trạng thái nội bộ                                | State                   |
| 1 thay đổi cần thông báo tới nhiều nơi (event, pub/sub)                       | Observer                |
| Cần Undo/Redo, hoặc muốn tham số hóa 1 hành động                              | Command                 |
| Cần snapshot & khôi phục trạng thái                                           | Memento                 |
| Bộ khung thuật toán cố định, chi tiết từng bước khác nhau                     | Template Method         |
| Request cần đi qua nhiều handler tùy điều kiện                                | Chain of Responsibility |
| Nhiều object giao tiếp chằng chịt, muốn tập trung qua 1 điểm                  | Mediator                |
| Duyệt qua tập hợp phần tử mà không lộ cấu trúc lưu trữ                        | Iterator                |
| Thêm hành vi mới cho nhóm class có sẵn mà không sửa class đó                  | Visitor                 |

## Nguyên tắc thiết kế nền tảng phía sau các pattern (SOLID)

- **S**ingle Responsibility — mỗi class chỉ nên có 1 lý do để thay đổi
- **O**pen/Closed — mở để mở rộng, đóng để sửa đổi (Strategy, Decorator, Template Method thể hiện rõ nguyên tắc này)
- **L**iskov Substitution — lớp con phải thay thế được lớp cha mà không phá vỡ chương trình
- **I**nterface Segregation — nhiều interface nhỏ chuyên biệt tốt hơn 1 interface lớn ôm đồm
- **D**ependency Inversion — phụ thuộc vào abstraction (interface), không phụ thuộc class cụ thể (Factory, Strategy, Bridge đều dựa trên nguyên tắc này)

## Lời khuyên khi học Design Pattern

1. **Đừng học thuộc lòng, hãy hiểu vấn đề** mà pattern đó giải quyết — pattern chỉ là công cụ, không phải mục tiêu.
2. **Đừng lạm dụng** — dùng pattern khi bài toán thực sự cần, không phải để "khoe" đã học pattern. Code đơn giản luôn tốt hơn code dùng pattern không cần thiết.
3. Bắt đầu với các pattern ⭐⭐⭐ (Singleton, Factory Method, Builder, Adapter, Decorator, Facade, Command, Observer, Strategy, Template Method, Iterator) — đây là nhóm bạn sẽ gặp thường xuyên nhất trong công việc thực tế.
4. Thực hành bằng cách **refactor code cũ** của chính bạn — tìm đoạn code có `if/switch` dài dòng theo loại đối tượng → thử áp dụng Strategy/State; tìm đoạn code tạo object phức tạp → thử Builder/Factory.

## Tài liệu tham khảo thêm

- Refactoring.Guru — trang tổng hợp trực quan rất tốt về 23 pattern (có hình minh họa)
- Sách "Design Patterns: Elements of Reusable Object-Oriented Software" (Gang of Four, 1994) — nguồn gốc của 23 pattern
- Sách "Head First Design Patterns" (Eric Freeman) — cách tiếp cận dễ hiểu, nhiều ví dụ trực quan
