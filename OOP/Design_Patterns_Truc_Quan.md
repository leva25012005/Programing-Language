# 23 DESIGN PATTERNS — HỌC QUA ẨN DỤ ĐỜI THƯỜNG

> Mỗi pattern = 1 câu chuyện quen thuộc. Đọc xong câu chuyện, bạn sẽ **tự suy ra** được code trông như thế nào — thay vì phải học thuộc.

**Cách đọc tài liệu này:**

- 🎬 **Tình huống đời thường** — đọc trước, hình dung ra bối cảnh
- 💡 **Bản chất pattern** — chuyển từ đời thường sang tư duy lập trình
- 🖼️ **Sơ đồ trực quan** — nhìn cấu trúc bằng mắt
- 🧠 **Ghi nhớ bằng 1 câu** — câu thần chú để không quên
- ⚠️ **Cạm bẫy** — khi nào KHÔNG nên dùng

---

## MỤC LỤC NHANH

**🏗️ NHÓM KHỞI TẠO — "Sinh ra object thế nào cho khéo"**

1. Singleton — Chỉ một Tổng thống
2. Factory Method — Gọi món ở nhà hàng
3. Abstract Factory — Mua nguyên bộ nội thất theo phong cách
4. Builder — Order trà sữa tùy chỉnh từng bước
5. Prototype — Photocopy hồ sơ

**🧩 NHÓM CẤU TRÚC — "Ghép object lại với nhau thế nào"**

6. Adapter — Adapter sạc điện thoại quốc tế
7. Bridge — Remote TV đa năng
8. Composite — Cây thư mục Windows
9. Decorator — Order trà sữa thêm topping
10. Facade — Nút "Start" trên xe hơi
11. Flyweight — Rừng cây trong game
12. Proxy — Trợ lý lọc cuộc gọi cho sếp

**🎭 NHÓM HÀNH VI — "Các object giao tiếp với nhau thế nào"**

13. Chain of Responsibility — Quy trình duyệt chi phí công ty
14. Command — Remote nhà thông minh
15. Interpreter — Máy tính hiểu biểu thức
16. Iterator — Nút Next trên đầu đĩa CD
17. Mediator — Tháp không lưu điều phối máy bay
18. Memento — Save Game / Ctrl+Z
19. Observer — Đăng ký kênh Youtube
20. State — Đèn giao thông
21. Strategy — Google Maps chọn phương tiện
22. Template Method — Quy trình pha mì gói
23. Visitor — Nhân viên thuế đi kiểm tra từng cửa hàng

---

# 🏗️ NHÓM KHỞI TẠO (CREATIONAL)

## 1. Singleton — "Chỉ một Tổng thống"

### 🎬 Tình huống

Một quốc gia tại một thời điểm **chỉ có đúng một** Tổng thống. Không ai được tự phong mình làm Tổng thống thứ hai. Mọi quyết định quốc gia đều đi qua **cùng một người** đó.

### 💡 Bản chất

Có những object trong chương trình **chỉ nên tồn tại đúng 1 lần**: bộ nhớ cấu hình ứng dụng, kết nối tới máy in, hoặc sổ ghi log. Nếu để mỗi nơi trong code tự `new` một bản riêng, dữ liệu sẽ bị phân mảnh, không đồng bộ.

### 🖼️ Sơ đồ

```
   Module A ──┐
   Module B ──┼──► [ Logger — DUY NHẤT 1 instance ]
   Module C ──┘
```

### 🧠 Ghi nhớ

> "Dù ai gọi bao nhiêu lần, vẫn chỉ nhận về **một** cái duy nhất."

### ⚠️ Cạm bẫy

Lạm dụng Singleton biến nó thành "biến toàn cục trá hình" — code khó test, khó theo dõi ai đang sửa trạng thái chung. Chỉ dùng khi _thực sự_ cần đúng 1 instance.

---

## 2. Factory Method — "Gọi món ở nhà hàng"

### 🎬 Tình huống

Bạn vào nhà hàng, gọi "cho tôi một ly cà phê". Bạn **không cần biết** đầu bếp pha bằng máy nào, loại hạt gì, quy trình ra sao. Bạn chỉ quan tâm: gọi món → nhận món.

### 💡 Bản chất

Thay vì code của bạn tự tay `new XeMay()` hay `new OTo()` ở khắp nơi (gắn chặt vào class cụ thể), bạn giao việc "tạo ra phương tiện" cho một **"nhà máy"**. Muốn đổi loại phương tiện, chỉ cần đổi nhà máy, code gọi món không đổi.

### 🖼️ Sơ đồ

```
   Client            "Cho tôi 1 phương tiện"
     │
     ▼
 [NhaMayVanChuyen] ──► quyết định cụ thể
     │
     ├─► NhaMayXeMay → tạo ra XeMay
     └─► NhaMayOTo   → tạo ra OTo
```

### 🧠 Ghi nhớ

> "Tôi không tự nấu — tôi gọi món, bếp tự quyết định cách làm."

### ⚠️ Cạm bẫy

Nếu chỉ có 1-2 loại object đơn giản, không cần cả một hệ thống Factory — dùng `new` trực tiếp cho gọn.

---

## 3. Abstract Factory — "Mua nguyên bộ nội thất theo phong cách"

### 🎬 Tình huống

Bạn mua nội thất theo phong cách **Scandinavian**: ghế, bàn, đèn — tất cả phải **đồng bộ phong cách**, không thể lẫn ghế Scandinavian với đèn Gothic. Cửa hàng có "combo" riêng cho từng phong cách để bạn chọn nguyên bộ.

### 💡 Bản chất

Khác Factory Method (chỉ tạo 1 loại sản phẩm), Abstract Factory tạo ra **cả một họ sản phẩm liên quan**, đảm bảo chúng luôn "hợp gu" với nhau — ví dụ bộ giao diện Windows (Button + Checkbox kiểu Windows) hay bộ giao diện MacOS.

### 🖼️ Sơ đồ

```
                 ┌─► WinFactory ──► [WinButton, WinCheckbox]  (đồng bộ 1 họ)
   GUIFactory ───┤
                 └─► MacFactory ──► [MacButton, MacCheckbox]  (đồng bộ 1 họ)
```

### 🧠 Ghi nhớ

> "Mua cả bộ, không mua lẻ — để đảm bảo mọi thứ hợp phong cách."

### ⚠️ Cạm bẫy

Thêm 1 loại sản phẩm mới vào họ (VD: thêm Slider) nghĩa là phải sửa **tất cả** các factory hiện có — khá cồng kềnh khi hệ thống lớn dần.

---

## 4. Builder — "Order trà sữa tùy chỉnh từng bước"

### 🎬 Tình huống

Bạn order trà sữa: chọn size → chọn mức đường → chọn mức đá → chọn topping. Mỗi bước là một lựa chọn riêng, và bạn có thể **bỏ qua bước nào không cần**. Không ai muốn gọi món qua một constructor với 15 tham số kiểu `traSua(size, duong, da, topping, loaiTra, ...)`.

### 💡 Bản chất

Khi object cần khởi tạo có **quá nhiều tham số tùy chọn**, Builder cho phép xây dựng nó **từng bước một**, dễ đọc, dễ chọn linh hoạt, thay vì nhồi hết vào 1 constructor khổng lồ.

### 🖼️ Sơ đồ

```
NhaXeBuilder
   .xayMong()     ┐
   .xayTuong()    ├─► từng bước rõ ràng, chọn cái nào cần
   .xayMai()      ┘
   .build()  ──► NhaXe hoàn chỉnh
```

### 🧠 Ghi nhớ

> "Xây từng viên gạch một, thay vì nhồi hết công thức vào một dòng."

### ⚠️ Cạm bẫy

Object đơn giản (2-3 thuộc tính) thì Builder là thừa thãi — constructor thường là đủ.

---

## 5. Prototype — "Photocopy hồ sơ"

### 🎬 Tình huống

Bạn cần 10 bản hợp đồng gần giống nhau, chỉ khác tên khách hàng. Thay vì soạn lại từ đầu mỗi lần (tốn công), bạn **photocopy** bản mẫu rồi chỉnh sửa phần khác biệt.

### 💡 Bản chất

Khi việc tạo mới một object từ đầu **tốn kém** (phải đọc file, gọi database, tính toán phức tạp...), Prototype cho phép **nhân bản (clone)** một object mẫu có sẵn — nhanh hơn nhiều so với dựng lại từ đầu.

### 🖼️ Sơ đồ

```
   [Object mẫu] ──clone()──► [Bản sao 1]
                 ──clone()──► [Bản sao 2]
                 ──clone()──► [Bản sao 3]
```

### 🧠 Ghi nhớ

> "Đừng viết lại — photocopy rồi sửa chỗ cần sửa."

### ⚠️ Cạm bẫy

Phải cẩn thận giữa **shallow copy** (chỉ sao chép địa chỉ, dùng chung dữ liệu bên trong — dễ gây bug) và **deep copy** (sao chép toàn bộ dữ liệu độc lập).

---

# 🧩 NHÓM CẤU TRÚC (STRUCTURAL)

## 6. Adapter — "Adapter sạc điện thoại quốc tế"

### 🎬 Tình huống

Bạn mang sạc điện thoại chuẩn ổ cắm Việt Nam sang Anh Quốc. Ổ cắm bên đó **khác chuẩn hoàn toàn**. Bạn không thể sửa lại cục sạc, cũng không sửa được ổ cắm tường của khách sạn — giải pháp là mua một **đầu chuyển (adapter)** đứng giữa hai bên.

### 💡 Bản chất

Khi hai hệ thống có interface không tương thích (VD: code của bạn cần `thanhToan()` nhưng thư viện bên thứ 3 chỉ có `pay()`), Adapter đóng vai trò "phiên dịch" — bọc bên ngoài class cũ để nó nói được "ngôn ngữ" mà hệ thống của bạn hiểu.

### 🖼️ Sơ đồ

```
[Hệ thống của bạn] ──cần──► ThanhToanChuan.thanhToan()
                                     ▲
                              [VNPayAdapter]   ← "phiên dịch"
                                     │
                              VNPayAPI.pay()   ← thư viện bên thứ 3, không sửa được
```

### 🧠 Ghi nhớ

> "Không sửa được hai đầu — thì thêm một cục chuyển đổi ở giữa."

### ⚠️ Cạm bẫy

Nếu bạn sửa được trực tiếp class gốc, sửa luôn sẽ đơn giản hơn là thêm 1 lớp Adapter không cần thiết.

---

## 7. Bridge — "Remote TV đa năng"

### 🎬 Tình huống

Một chiếc remote đa năng có thể điều khiển TV Samsung, LG, Sony — remote không cần biết chi tiết bên trong từng hãng TV hoạt động ra sao. Đồng thời, hãng TV có ra mẫu remote mới (có thêm nút Voice) cũng không ảnh hưởng gì tới việc TV hoạt động.

### 💡 Bản chất

Bridge tách phần "giao diện điều khiển" (abstraction) ra khỏi phần "cách thực thi cụ thể" (implementation) — hai bên phát triển **độc lập** với nhau. Tránh việc nếu có 3 loại remote × 5 loại TV thì phải viết 15 class con (bùng nổ tổ hợp).

### 🖼️ Sơ đồ

```
   DieuKhien (abstraction)          ThietBi (implementation)
        │                                  │
   DieuKhienCoBan ────cầu nối────►  TV, Radio, ...

   → Kết hợp TỰ DO: bất kỳ DieuKhien nào + bất kỳ ThietBi nào
```

### 🧠 Ghi nhớ

> "Cây cầu nối 2 bờ độc lập — mỗi bờ tự phát triển riêng, không đụng nhau."

### ⚠️ Cạm bẫy

Là pattern trừu tượng nhất trong nhóm Structural — chỉ đáng dùng khi bạn thực sự thấy 2 chiều biến đổi độc lập (VD: nhiều loại điều khiển × nhiều loại thiết bị), nếu không sẽ làm code phức tạp hóa không cần thiết.

---

## 8. Composite — "Cây thư mục Windows"

### 🎬 Tình huống

Trong Windows Explorer, bạn chọn một Folder và nhấn "Xóa" — nó xóa luôn mọi File và Folder con bên trong. Bạn thao tác với **1 File đơn lẻ** hay **cả 1 Folder chứa hàng trăm file** theo **cùng một cách** (click chuột phải → Delete).

### 💡 Bản chất

Composite tổ chức object thành cấu trúc **cây phân cấp** (folder chứa file và folder con), và cho phép xử lý phần tử đơn lẻ (File) và tổ hợp (Folder) **qua cùng một interface** — code gọi `hienThi()` không cần biết đang xử lý 1 file hay cả cây thư mục.

### 🖼️ Sơ đồ

```
             📁 root
            /        \
        📄 a.txt    📁 sub
                        \
                       📄 b.txt

  → root.hienThi() sẽ tự đệ quy xuống mọi File/Folder con
```

### 🧠 Ghi nhớ

> "Một chiếc lá hay cả một cái cây — bấm nút giống hệt nhau."

### ⚠️ Cạm bẫy

Nếu các phần tử lá và phần tử tổ hợp có hành vi quá khác biệt, ép chúng vào chung 1 interface có thể phản tác dụng, gây interface "bẩn" (có phương thức vô nghĩa với 1 trong 2 loại).

---

## 9. Decorator — "Order trà sữa thêm topping"

### 🎬 Tình huống

Trà sữa gốc: 20k. Thêm trân châu: +5k. Thêm thạch: +5k. Bạn có thể chồng bao nhiêu topping tùy thích, và mỗi lần thêm, giá + mô tả đều **cộng dồn** — mà không cần tạo ra một class riêng cho từng tổ hợp "TràSữaTrânChâuThạchSữa".

### 💡 Bản chất

Decorator cho phép "bọc" thêm hành vi mới quanh một object **tại runtime**, xếp chồng nhiều lớp tùy ý — thay vì kế thừa tĩnh (sẽ nổ số lượng class: TràSữa, TràSữaTrânChâu, TràSữaThạch, TràSữaTrânChâuThạch...).

### 🖼️ Sơ đồ

```
CaPheDen (20k)
   └── bọc bởi ThemSua (+5k) = 25k
          └── bọc bởi ThemDuong (+2k) = 27k
```

### 🧠 Ghi nhớ

> "Từng lớp giấy gói chồng lên nhau — mỗi lớp thêm 1 chút, không đổi cái bên trong."

### ⚠️ Cạm bẫy

Chồng quá nhiều lớp decorator khiến việc debug khó khăn (phải lần theo nhiều lớp bọc để hiểu hành vi cuối cùng).

---

## 10. Facade — "Nút Start trên xe hơi"

### 🎬 Tình huống

Bạn bấm nút "Start" trên xe — chỉ 1 cái bấm. Nhưng bên trong, hàng chục hệ thống được kích hoạt: bơm nhiên liệu, đánh lửa, kiểm tra cảm biến, khởi động động cơ... Bạn hoàn toàn **không cần biết** thứ tự và chi tiết của các bước đó.

### 💡 Bản chất

Facade cung cấp **một cổng vào đơn giản** cho một hệ thống con phức tạp — giấu đi sự rắc rối bên trong, để code gọi ở ngoài chỉ cần gọi 1 hàm duy nhất.

### 🖼️ Sơ đồ

```
   Client ──"datHang()"──► [DatHangFacade]
                                  │
                    ┌─────────────┼─────────────┐
                    ▼             ▼             ▼
               KhoHang       ThanhToan      VanChuyen
            (kiểm tra tồn) (xử lý tiền)  (đặt vận chuyển)
```

### 🧠 Ghi nhớ

> "Một nút bấm — cả hệ thống phía sau tự vận hành, bạn không cần biết chi tiết."

### ⚠️ Cạm bẫy

Facade không nên "ôm đồm" quá nhiều logic nghiệp vụ vào chính nó — nó chỉ nên là lớp điều phối gọi các subsystem, không phải nơi viết logic thực sự.

---

## 11. Flyweight — "Rừng cây trong game"

### 🎬 Tình huống

Một khu rừng trong game có 1 triệu cây. Nếu mỗi cây là 1 object riêng với đầy đủ texture, model 3D (rất nặng) → tràn bộ nhớ. Thực ra, phần lớn cây **dùng chung** model và texture, chỉ khác nhau ở **vị trí đặt**.

### 💡 Bản chất

Flyweight tách dữ liệu của object thành 2 phần: phần **dùng chung** (intrinsic — model, texture) được cache và tái sử dụng, phần **riêng** (extrinsic — vị trí x,y) được truyền vào khi cần vẽ. Nhờ vậy tiết kiệm bộ nhớ khi có số lượng object khổng lồ.

### 🖼️ Sơ đồ

```
        [FontFactory / cache]
       "A-Arial" ──dùng chung bởi──► vị trí (0,0), (10,0), (20,5), ...
       "B-Arial" ──dùng chung bởi──► vị trí (5,5), (15,15), ...
```

### 🧠 Ghi nhớ

> "Model cây chỉ có 1 bản — chỉ tọa độ trồng là khác nhau."

### ⚠️ Cạm bẫy

Chỉ đáng áp dụng khi số lượng object **cực lớn** và phần dữ liệu dùng chung **thực sự nặng** — nếu không, độ phức tạp thêm vào không đáng để đánh đổi.

---

## 12. Proxy — "Trợ lý lọc cuộc gọi cho sếp"

### 🎬 Tình huống

Sếp bận rộn, mọi cuộc gọi đều qua trợ lý trước. Trợ lý quyết định: cuộc gọi nào quan trọng thì mới chuyển cho sếp, cuộc gọi spam thì chặn lại. Người gọi _tưởng_ đang nói chuyện trực tiếp, nhưng thực chất luôn đi qua "người đại diện" trước.

### 💡 Bản chất

Proxy là một object đứng ra **thay mặt** cho object thật, kiểm soát việc truy cập tới nó — có thể dùng để: trì hoãn tạo object nặng tới khi cần (lazy loading), kiểm tra quyền truy cập, ghi log, hay cache kết quả.

### 🖼️ Sơ đồ

```
   Client ──gọi──► [AnhProxy] ──(chỉ tạo khi cần)──► [AnhThat — nặng, tốn tài nguyên]
```

### 🧠 Ghi nhớ

> "Không nói chuyện trực tiếp — luôn qua một người gác cổng trước."

### ⚠️ Cạm bẫy

Thêm 1 lớp trung gian đồng nghĩa thêm 1 bước gọi hàm — nếu không cần kiểm soát truy cập gì đặc biệt, Proxy chỉ làm chậm và phức tạp hóa code.

---

# 🎭 NHÓM HÀNH VI (BEHAVIORAL)

## 13. Chain of Responsibility — "Quy trình duyệt chi phí công ty"

### 🎬 Tình huống

Bạn xin duyệt chi phí 3 triệu. Trưởng nhóm chỉ được duyệt tới 2 triệu → tự động chuyển lên Quản lý (duyệt tới 5 triệu) → nếu vẫn vượt, chuyển tiếp lên Giám đốc. Bạn (người xin duyệt) không cần biết ai sẽ là người duyệt cuối cùng.

### 💡 Bản chất

Request đi qua **một chuỗi handler**, mỗi handler tự quyết định: xử lý luôn, hay đẩy tiếp cho người kế tiếp trong chuỗi. Người gửi request không cần biết trước ai sẽ xử lý.

### 🖼️ Sơ đồ

```
Đơn 3 triệu ──► [Trưởng nhóm] ──(vượt quyền)──► [Quản lý] ──(vượt quyền)──► [Giám đốc] ✔ duyệt
```

### 🧠 Ghi nhớ

> "Không giải quyết được thì chuyển lên cấp trên — cứ thế đến khi có người xử lý."

### ⚠️ Cạm bẫy

Nếu chuỗi quá dài mà không có ai xử lý, request có thể "rơi" ra cuối chuỗi mà không ai biết — cần luôn có cơ chế xử lý mặc định ở cuối.

---

## 14. Command — "Remote nhà thông minh"

### 🎬 Tình huống

Nút "Bật đèn phòng khách" trên app điều khiển nhà thông minh không trực tiếp gọi tới bóng đèn. Nó tạo ra một **"lệnh"** (Command) chứa đầy đủ thông tin cần làm gì. Nhờ đóng gói thành lệnh, app có thể lưu lại lịch sử, và cho phép **Undo** ("Hoàn tác lệnh vừa bật đèn" → tự tắt lại).

### 💡 Bản chất

Command đóng gói một hành động (kèm mọi thông tin cần thiết để thực hiện nó) thành một object độc lập — tách rời "ai gửi lệnh" khỏi "ai thực thi lệnh", đồng thời cho phép xếp hàng đợi, ghi log, và Undo/Redo.

### 🖼️ Sơ đồ

```
[CongTac] ──nhận(BatDenCommand)──► lưu vào lịch sử ──► thực thi ──► DenLed.bat()
                                          │
                                     undo() ──► DenLed.tat()
```

### 🧠 Ghi nhớ

> "Đóng gói yêu cầu thành 1 gói hàng — có thể gửi đi, lưu lại, hoặc hoàn trả (undo)."

### ⚠️ Cạm bẫy

Với hành động đơn giản, không cần Undo/queue, việc tạo hẳn 1 class Command riêng là dư thừa.

---

## 15. Interpreter — "Máy tính hiểu biểu thức"

### 🎬 Tình huống

Máy tính bỏ túi nhận vào chuỗi `"5 + 3"` và hiểu được đó là phép cộng giữa 5 và 3, rồi trả về 8. Nó phải "diễn giải" từng phần của biểu thức theo đúng quy tắc ngữ pháp đã định nghĩa trước.

### 💡 Bản chất

Interpreter định nghĩa cấu trúc ngữ pháp của một ngôn ngữ đơn giản bằng cây các object (mỗi loại biểu thức là 1 class), rồi "diễn giải" (evaluate) bằng cách đệ quy qua cây đó.

### 🖼️ Sơ đồ

```
        PhepCong
        /      \
      So(5)    So(3)

   dienGiai() = 5 + 3 = 8
```

### 🧠 Ghi nhớ

> "Biến câu văn/biểu thức thành 1 cái cây, rồi đọc cây đó theo quy tắc."

### ⚠️ Cạm bẫy

Chỉ hợp với ngữ pháp **đơn giản**; ngôn ngữ phức tạp nên dùng công cụ sinh parser chuyên dụng (ANTLR...) thay vì tự viết Interpreter tay.

---

## 16. Iterator — "Nút Next trên đầu đĩa CD"

### 🎬 Tình huống

Bạn bấm "Next" trên đầu đĩa CD để chuyển bài — bạn không cần biết đĩa CD lưu bài hát theo cấu trúc vật lý nào (rãnh xoắn ốc, track...). Bạn chỉ cần: Next, Next, Next — và luôn nhận được bài tiếp theo.

### 💡 Bản chất

Iterator cung cấp cách duyệt tuần tự qua các phần tử của một tập hợp mà **không cần biết cấu trúc lưu trữ bên trong** (mảng, linked list, cây...). Đây là lý do `for (auto& x : collection)` trong C++ hay `foreach` trong C# hoạt động được trên rất nhiều loại cấu trúc dữ liệu khác nhau.

### 🖼️ Sơ đồ

```
[BoSuuTap: ẩn cấu trúc bên trong] ──begin()/next()──► A → B → C → (hết)
```

### 🧠 Ghi nhớ

> "Chỉ cần biết bấm Next — không cần biết đĩa CD ghi dữ liệu kiểu gì."

### ⚠️ Cạm bẫy

Hầu hết ngôn ngữ hiện đại (C++, C#, Java...) đã có sẵn cơ chế Iterator tích hợp — hiếm khi cần tự viết từ đầu.

---

## 17. Mediator — "Tháp không lưu điều phối máy bay"

### 🎬 Tình huống

Máy bay không tự nói chuyện trực tiếp với nhau để tránh va chạm — quá hỗn loạn và nguy hiểm nếu có 50 máy bay cùng lúc. Thay vào đó, **tất cả** đều liên lạc qua Tháp không lưu — tháp điều phối toàn bộ.

### 💡 Bản chất

Khi nhiều object cần giao tiếp với nhau theo kiểu chằng chịt (mỗi cái biết về tất cả những cái còn lại), Mediator đưa toàn bộ logic giao tiếp vào **một điểm trung tâm**, giảm sự phụ thuộc trực tiếp giữa các object.

### 🖼️ Sơ đồ

```
   NguoiDung A ──┐
   NguoiDung B ──┼──► [ChatRoom — Mediator] ──► chuyển tiếp tin nhắn cho đúng người
   NguoiDung C ──┘

   (A, B, C KHÔNG gọi trực tiếp lẫn nhau)
```

### 🧠 Ghi nhớ

> "Không nói chuyện tay đôi lung tung — mọi giao tiếp qua một trung tâm điều phối."

### ⚠️ Cạm bẫy

Nếu không cẩn thận, Mediator có thể phình to thành "class biết tuốt", ôm quá nhiều logic — trở thành điểm nghẽn khó bảo trì.

---

## 18. Memento — "Save Game / Ctrl+Z"

### 🎬 Tình huống

Bạn đang chơi game, trước khi vào trận đánh khó, bạn **Save**. Đánh thua thì **Load lại** đúng trạng thái đã lưu — mà bản thân bạn (người chơi) không cần biết game lưu dữ liệu save dưới định dạng gì.

### 💡 Bản chất

Memento cho phép lưu lại (snapshot) trạng thái nội bộ của object tại một thời điểm, để khôi phục sau này — mà **không vi phạm encapsulation** (không cần phơi bày toàn bộ cấu trúc dữ liệu nội bộ ra ngoài).

### 🖼️ Sơ đồ

```
[VanBan] ──luuTrangThai()──► [Memento: "Bản nháp 1"]
                                    │ lưu trong
                                    ▼
                            [LichSuUndo — Caretaker]
                                    │ khoiPhuc()
                                    ▼
[VanBan] ◄──────── quay lại "Bản nháp 1"
```

### 🧠 Ghi nhớ

> "Chụp ảnh trạng thái hiện tại, cất vào ngăn kéo, cần thì lấy ra dùng lại."

### ⚠️ Cạm bẫy

Nếu trạng thái object rất lớn, lưu quá nhiều Memento (mỗi lần Undo) có thể gây tốn bộ nhớ đáng kể.

---

## 19. Observer — "Đăng ký kênh Youtube"

### 🎬 Tình huống

Bạn nhấn "Đăng ký" (Subscribe) một kênh Youtube. Từ giờ, mỗi khi kênh đó đăng video mới, bạn **tự động** nhận được thông báo — mà không cần liên tục vào kiểm tra kênh đó có gì mới không.

### 💡 Bản chất

Observer định nghĩa quan hệ 1-nhiều: khi 1 object (Subject/kênh Youtube) thay đổi trạng thái, **tất cả** object đã đăng ký (Observer/subscriber) đều tự động được thông báo. Đây là nền tảng của mọi hệ thống Event/Callback, Pub-Sub.

### 🖼️ Sơ đồ

```
[Kênh Youtube] ──đăng video mới──► thông báo tự động tới TẤT CẢ:
        │
        ├──► Subscriber A
        ├──► Subscriber B
        └──► Subscriber C
```

### 🧠 Ghi nhớ

> "Đăng ký một lần — có tin gì mới thì tự động được báo, không cần hỏi lại."

### ⚠️ Cạm bẫy

Nếu quên "hủy đăng ký" (unsubscribe) khi không cần nữa, Observer cũ vẫn bị giữ tham chiếu → rò rỉ bộ nhớ (memory leak), một lỗi rất phổ biến trong thực tế.

---

## 20. State — "Đèn giao thông"

### 🎬 Tình huống

Đèn giao thông có 3 trạng thái: Đỏ, Vàng, Xanh. Ở trạng thái Đỏ, xe phải dừng; ở Xanh, xe được đi. **Cùng một chiếc đèn**, nhưng hành vi (cho phép đi hay không) hoàn toàn thay đổi tùy trạng thái hiện tại.

### 💡 Bản chất

State cho phép object thay đổi hành vi khi trạng thái nội bộ thay đổi — nhìn như thể nó đã "biến hình" thành class khác. Thay vì viết một khối `if/switch` khổng lồ kiểm tra trạng thái ở mọi hàm, mỗi trạng thái được tách thành 1 class riêng, tự biết cách xử lý và tự biết khi nào chuyển sang trạng thái kế tiếp.

### 🖼️ Sơ đồ

```
[MayBanHang]
   trạng thái: ChoTien ──nhanTien()──► DaNhanTien ──layHang()──► (quay lại) ChoTien
```

### 🧠 Ghi nhớ

> "Đèn đỏ thì dừng, đèn xanh thì đi — cùng một cột đèn, hành vi đổi theo trạng thái."

### ⚠️ Cạm bẫy

Dễ nhầm với Strategy (mục 21) — điểm khác biệt: State tự chuyển đổi trạng thái _bên trong nó_, còn Strategy được client _chọn từ bên ngoài_ và không tự chuyển đổi.

---

## 21. Strategy — "Google Maps chọn phương tiện"

### 🎬 Tình huống

Bạn tìm đường trên Google Maps: cùng điểm A tới điểm B, nhưng bạn có thể chọn "Đi bộ", "Xe máy", hay "Ô tô" — mỗi lựa chọn dùng **thuật toán tính đường khác nhau** (đường bộ đi được ngõ nhỏ, ô tô phải theo đường lớn). Bạn đổi lựa chọn bất cứ lúc nào.

### 💡 Bản chất

Strategy định nghĩa một họ thuật toán, đóng gói riêng từng cái, và cho phép **đổi thuật toán tại runtime** theo nhu cầu — mà code gọi (Google Maps engine) không cần sửa gì khi thêm thuật toán mới.

### 🖼️ Sơ đồ

```
[GioHang] ──chọn chiến lược──► ThanhToanMoMo   (đổi được bất cứ lúc nào)
                          hoặc► ThanhToanTienMat
```

### 🧠 Ghi nhớ

> "Cùng đích đến — nhưng bạn tự chọn cách đi."

### ⚠️ Cạm bẫy

Xem thêm phân biệt với State ở mục 20 — nếu việc "chọn thuật toán" luôn cố định do lập trình viên set sẵn (không đổi runtime theo nhu cầu người dùng), có thể chỉ cần hàm thường là đủ, không cần cả pattern.

---

## 22. Template Method — "Quy trình pha mì gói"

### 🎬 Tình huống

Dù là mì tôm hay mì Ý ăn liền, quy trình luôn theo đúng khung: **Đun nước sôi → Đổ vào mì → Đợi vài phút → Trộn gia vị → Ăn**. Chỉ có **gia vị** và **cách trộn** là khác nhau tùy loại mì — còn khung quy trình luôn cố định.

### 💡 Bản chất

Template Method định nghĩa **bộ khung cố định** của một thuật toán trong lớp cha, chỉ để lớp con override một vài bước cụ thể — cấu trúc tổng thể (thứ tự các bước) không đổi. Đây là cơ chế đứng sau rất nhiều framework quen thuộc.

### 🖼️ Sơ đồ

```
PhaDoUong.phaChe()  ← khung CỐ ĐỊNH, không cho sửa
   1. dunNuoc()          (dùng chung)
   2. pha()               ← lớp con override (Trà khác Cà phê)
   3. rotVaoLy()         (dùng chung)
   4. themGiaVi()         ← lớp con override
```

### 🧠 Ghi nhớ

> "Công thức nấu ăn thì giữ nguyên các bước — chỉ đổi nguyên liệu ở vài bước cụ thể."

### ⚠️ Cạm bẫy

Nếu các bước con quá khác biệt nhau (không còn chung 1 khung logic), ép chúng vào Template Method sẽ gượng ép — nên xem lại có đúng bài toán không.

---

## 23. Visitor — "Nhân viên thuế đi kiểm tra từng cửa hàng"

### 🎬 Tình huống

Nhân viên thuế đến khu chợ, lần lượt "ghé thăm" từng cửa hàng: hàng sách tính thuế 5%, hàng điện tử tính thuế 10%. Nhân viên thuế **mang theo** toàn bộ cách tính, chứ **không phải** chính cửa hàng tự biết cách tính thuế của mình.

### 💡 Bản chất

Visitor tách một thuật toán/hành vi ra khỏi cấu trúc dữ liệu mà nó thao tác — cho phép thêm hành vi mới cho một nhóm class có sẵn (VD: thêm cách "TinhKhuyenMai" mới) mà **không cần sửa** các class đó, chỉ cần viết thêm 1 Visitor mới.

### 🖼️ Sơ đồ

```
[TinhThueVisitor] ── ghé thăm ──► Sach.chapNhan(visitor)     → thuế 5%
                    ── ghé thăm ──► DoDienTu.chapNhan(visitor) → thuế 10%
```

### 🧠 Ghi nhớ

> "Người kiểm tra mang theo quy tắc riêng, đi ghé từng đối tượng — đối tượng không tự biết quy tắc đó."

### ⚠️ Cạm bẫy

Nếu hệ thống **thường xuyên thêm loại đối tượng mới** (thêm Sach, DoDienTu, ThucPham...), Visitor sẽ bắt bạn phải sửa **mọi** Visitor hiện có mỗi lần thêm — pattern này hợp hơn khi tập hợp class đối tượng ổn định, còn hành vi mới thêm liên tục.

---

# 🧠 BẢNG GHI NHỚ TỔNG HỢP — 23 CÂU THẦN CHÚ

| #   | Pattern                 | Câu chuyện                   | Ghi nhớ trong 1 câu                       |
| --- | ----------------------- | ---------------------------- | ----------------------------------------- |
| 1   | Singleton               | Chỉ một Tổng thống           | Gọi bao nhiêu lần cũng chỉ 1              |
| 2   | Factory Method          | Gọi món nhà hàng             | Không tự nấu, để bếp quyết                |
| 3   | Abstract Factory        | Nội thất theo phong cách     | Mua cả bộ, không mua lẻ                   |
| 4   | Builder                 | Order trà sữa tùy chỉnh      | Xây từng bước, không nhồi 1 dòng          |
| 5   | Prototype               | Photocopy hồ sơ              | Copy rồi sửa, không viết lại              |
| 6   | Adapter                 | Adapter sạc quốc tế          | Thêm cục chuyển đổi ở giữa                |
| 7   | Bridge                  | Remote TV đa năng            | Cầu nối 2 bờ độc lập                      |
| 8   | Composite               | Cây thư mục Windows          | Lá hay cả cây, thao tác như nhau          |
| 9   | Decorator               | Trà sữa thêm topping         | Từng lớp gói chồng lên nhau               |
| 10  | Facade                  | Nút Start xe hơi             | Một nút, cả hệ thống tự chạy              |
| 11  | Flyweight               | Rừng cây trong game          | Model chung, chỉ tọa độ riêng             |
| 12  | Proxy                   | Trợ lý lọc cuộc gọi          | Không nói trực tiếp, qua gác cổng         |
| 13  | Chain of Responsibility | Duyệt chi phí công ty        | Không xử lý được thì đẩy lên trên         |
| 14  | Command                 | Remote nhà thông minh        | Đóng gói lệnh, gửi và hoàn tác được       |
| 15  | Interpreter             | Máy tính hiểu biểu thức      | Biến câu thành cây, đọc theo quy tắc      |
| 16  | Iterator                | Nút Next đầu đĩa CD          | Chỉ cần bấm Next, không cần biết cấu trúc |
| 17  | Mediator                | Tháp không lưu               | Không nói tay đôi, qua trung tâm          |
| 18  | Memento                 | Save Game / Ctrl+Z           | Chụp ảnh trạng thái, cần thì phục hồi     |
| 19  | Observer                | Đăng ký kênh Youtube         | Đăng ký 1 lần, tự động nhận tin mới       |
| 20  | State                   | Đèn giao thông               | Hành vi đổi theo trạng thái hiện tại      |
| 21  | Strategy                | Google Maps chọn phương tiện | Cùng đích, tự chọn cách đi                |
| 22  | Template Method         | Pha mì gói                   | Giữ khung, đổi vài bước cụ thể            |
| 23  | Visitor                 | Nhân viên thuế đi kiểm tra   | Người ghé thăm mang theo quy tắc riêng    |

---

## 🎯 3 CẶP PATTERN DỄ NHẦM LẪN NHẤT

**Strategy vs. State**

- Strategy: **bạn** (client) chủ động chọn thuật toán, object không tự đổi.
- State: **object tự** chuyển trạng thái dựa trên logic nội bộ của chính nó.

**Factory Method vs. Abstract Factory**

- Factory Method: tạo ra **1 sản phẩm**.
- Abstract Factory: tạo ra **1 họ sản phẩm** phải đồng bộ với nhau.

**Decorator vs. Proxy**

- Decorator: **thêm hành vi mới** (mở rộng chức năng).
- Proxy: **kiểm soát truy cập** tới object gốc (không thêm chức năng mới, chỉ canh gác/trì hoãn).

---

## 📌 CÁCH HỌC HIỆU QUẢ NHẤT

1. Che phần 💡 và 🖼️, chỉ đọc phần 🎬 — thử tự đoán xem pattern này giải quyết vấn đề gì.
2. Với mỗi pattern, tự nghĩ ra **1 ví dụ khác** ngoài đời mà bạn từng gặp — nếu nghĩ ra được, nghĩa là bạn đã thực sự hiểu bản chất chứ không phải học vẹt.
3. Khi code, hễ thấy mình đang viết `if/switch` dài dòng theo loại đối tượng → dừng lại, tự hỏi: "đây có phải là lúc cần Strategy/State không?"
4. Muốn xem code cụ thể (C++ và C#) cho từng pattern trong bảng này, xem lại tài liệu **"23 Design Pattern — Tổng hợp kèm code C++ & C#"** đã tạo trước đó — hai tài liệu này bổ trợ cho nhau: một để **hiểu**, một để **code**.
