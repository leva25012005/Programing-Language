// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Tại sao phải giết chết #include?
    - Mỗi khi bạn #include <iostream>, trình biên dịch phải copy hàng chục ngàn
 dòng code từ file iostream dán vào file của bạn. Nếu dự án có 100 file cpp đều
 include iostream, nó phải copy và dịch lại 100 lần!
    - Lỗi đụng độ Macro (Macro Pollution): Ai đó định nghĩa #define MAX 100 ở
 một file .h nào đó, nó sẽ lây nhiễm và làm hỏng toàn bộ các file .cpp include
 nó.

 2. Module giải quyết thế nào?
    - Module được biên dịch ĐÚNG 1 LẦN thành một dạng nhị phân (Binary).
    - Khi file khác `import`, nó chỉ load cấu trúc nhị phân cực nhẹ vào bộ nhớ,
 thời gian build giảm 90%.
    - Macro (#define) bên trong Module bị nhốt lại, KHÔNG THỂ thoát ra ngoài lây
 nhiễm file khác.
*/

// ==========================================================================================
// FILE 1: ToanHoc.ixx (Primary Module Interface - Giao diện chính của Module)
// Định dạng: .ixx (Visual Studio) hoặc .cppm, .cxx (Clang, GCC)
// ==========================================================================================
/*
module; // BẮT BUỘC SỐ 1: Báo hiệu bắt đầu "Global Module Fragment" (Vùng chứa
Header cũ)

// Bạn CHỈ ĐƯỢC PHÉP dùng #include ở vùng này, trước khi khai báo 'export
module' #include <iostream> #include <string>

// BẮT BUỘC SỐ 2: Khai báo tên Module và cho phép xuất nó ra ngoài
export module ToanHoc;

// 1. EXPORT HÀM (Cho phép bên ngoài sử dụng)
export int Cong(int a, int b) {
    return a + b;
}

// 2. EXPORT BIẾN TOÀN CỤC (Biến toàn cục giờ đây an toàn hơn rất nhiều)
export const double PI = 3.14159265359;

// 3. KHÔNG EXPORT (Private - Ẩn giấu hoàn toàn khỏi thế giới bên ngoài)
// Hàm này chỉ được xài nội bộ bên trong file ToanHoc.ixx. File khác không thể
thấy nó. void hamBiMat() { std::cout << "Day la logic xu ly ngam cua Module
ToanHoc, khong ai the thay!\n";
}

// 4. EXPORT CẢ MỘT KHỐI (Block Export) - Đỡ mất công viết chữ export nhiều lần
export {
    class MayTinh {
    public:
        void TinhToanNangCao() {
            hamBiMat(); // Gọi hàm nội bộ thoải mái
            std::cout << "Dang tinh toan nang cao...\n";
        }
    };

    struct ToaDo {
        int x, y;
    };
}
*/

// ==========================================================================================
// FILE 2: ToanHoc_CaiDat.cpp (Module Implementation - File tách riêng phần Cài
// đặt) Mức Nâng Cao: Giống như việc tách .h và .cpp ngày xưa, bạn có thể tách
// Giao diện và Cài đặt của Module.
// ==========================================================================================
/*
module ToanHoc; // Báo cho Trình biên dịch: "File này là ruột của Module
ToanHoc"
                // Lưu ý KHÔNG có chữ 'export'.

// Giả sử trong ToanHoc.ixx bạn chỉ khai báo: export int Nhan(int a, int b);
// Thì ở file này, bạn viết thân hàm:
int Nhan(int a, int b) {
    return a * b;
}
*/

// ==========================================================================================
// FILE 3: VuKhi.ixx (Module Partitions - Phân vùng Module)
// Mức Chuyên Gia: Khi Module của bạn khổng lồ (hàng chục ngàn dòng),
// bạn phải chia nhỏ nó ra thành các Phân Vùng (Partitions).
// ==========================================================================================
/*
export module VuKhi:Kiem; // Khai báo đây là Phân vùng 'Kiem' thuộc Module
'VuKhi'

export class KiemThan {
public:
    void Chem() {
        // ...
    }
};
*/

// ==========================================================================================
// FILE 4: VuKhi_Chinh.ixx (Gom các phân vùng lại)
// ==========================================================================================
/*
export module VuKhi;

// Gom tất cả các tính năng từ phân vùng Kiem và cấp quyền cho bên ngoài sử dụng
export import :Kiem;
*/

// ==========================================================================================
// FILE 5: main.cpp (Nơi sử dụng Modules)
// ==========================================================================================
/*
#include <iostream> // Vẫn phải include iostream vì file main.cpp chưa biến
thành module

// KẺ KẾT LIỄU #INCLUDE LÀ ĐÂY:
import ToanHoc;
// import VuKhi; // Nếu dùng file 3 và 4

// Thử định nghĩa một Macro độc hại, thường gây lỗi biên dịch ở C++98
#define PI 9999

int main() {
    std::cout << "--- DEMO C++20 MODULES ---\n";

    // 1. Gọi hàm và biến đã được export từ Module ToanHoc
    int kq = Cong(10, 5);
    std::cout << "10 + 5 = " << kq << "\n";

    // 2. Chứng minh Module miễn nhiễm với Macro Pollution
    // Dù ở trên ta có `#define PI 9999`, nhưng PI của Module ToanHoc không hề
bị ảnh hưởng!
    // Bởi vì Module là một thế giới đóng, Macro của file main không thể tràn
vào trong Module được. std::cout << "So PI (Tu Module): " << ToanHoc::PI <<
"\n"; // Nếu sợ trùng tên, dùng ::

    // 3. Sử dụng Class từ Module
    MayTinh mt;
    mt.TinhToanNangCao();

    // 4. Ký tự báo lỗi!
    // hamBiMat(); // LỖI BIÊN DỊCH! Hàm này không được export, file main không
thể nhìn thấy.

    std::cout << "\n--- HOAN THANH ---\n";
    return 0;
}
*/

// ==========================================
// TỔNG KẾT KINH NGHIỆM THỰC CHIẾN MIGRATION (Chuyển đổi code cũ sang Module)
// ==========================================
/*
 1. Đừng cố gắng chuyển đổi tất cả sang Module ngay lập tức. C++20 cho phép
    Module (import) và Header cũ (#include) chung sống hòa bình trong cùng một
 dự án.

 2. Bất cứ khi nào cần dùng thư viện cũ (C-library, Windows.h, thư viện bên thứ
 3), BẮT BUỘC phải đặt lệnh `#include` vào giữa dòng `module;` và `export module
 Ten;`. Đây gọi là "Global Module Fragment". Đừng đặt #include xuống dưới, sẽ
 văng lỗi.

 3. Trình tự biên dịch (Build order) giờ đây rất quan trọng.
    - Thời C++98: File cpp nào compile trước cũng được.
    - Thời C++20: File `ToanHoc.ixx` bắt buộc phải được compile xong trước khi
 compile `main.cpp`. Các công cụ hiện đại như CMake (bản mới) và MSBuild đã tự
 động hóa việc tính toán chữ ký phụ thuộc này, bạn không cần làm bằng tay.
*/