#include <iostream>
#include <string>
#include <thread>

// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Nỗi đau của quá khứ (Trước C++17):
    - Quy tắc Định nghĩa Duy nhất (ODR - One Definition Rule): Trong toàn bộ project, 
      một biến (hoặc hàm) chỉ được phép định nghĩa (cấp phát bộ nhớ) ĐÚNG MỘT LẦN.
    - Nếu bạn định nghĩa biến tĩnh (static) hoặc biến toàn cục (global) trong file .h, 
      và có 2 file .cpp cùng #include file .h đó -> Linker sẽ thấy 2 biến trùng tên!
      -> Gây ra lỗi kinh điển: "Multiple Definition".
    - Hệ quả: Bạn BẮT BUỘC phải khai báo ở .h và đi tìm 1 file .cpp để gán giá trị. 
      Điều này khiến việc viết các thư viện "Header-only" (như thư viện nlohmann/json) 
      trở nên cực kỳ cực khổ.

 2. Cứu tinh `inline variable` (C++17):
    - Khi dán nhãn `inline` vào một biến, bạn đang ra lệnh cho Linker: 
      "Dù mày có thấy biến này xuất hiện ở 100 file .cpp khác nhau, 
      xin hãy BỎ QUA LỖI, và tự động GỘP chúng lại thành DUY NHẤT 1 VÙNG NHỚ trên RAM cho tao!"
    - Nhờ đó, ta có thể khởi tạo biến static/global NGAY BÊN TRONG file .h.
*/

// ==========================================
// 1. MỨC CƠ BẢN: BIẾN TĨNH CỦA CLASS (STATIC MEMBER)
// ==========================================

// --- CÁCH CŨ (Trước C++17) - Mô phỏng ---
class KieuCu {
public:
    static int so_luong; 
    // Không được viết: static int so_luong = 10; ở đây! (Sẽ báo lỗi biên dịch)
};
// Bắt buộc phải khởi tạo lại ở một file .cpp nào đó:
int KieuCu::so_luong = 10; 


// --- CÁCH HIỆN ĐẠI (C++17) ---
class KieuHienDai {
public:
    // Tuyệt vời! Vừa khai báo, vừa khởi tạo ngay trong Class (Nằm trong file .h)
    inline static int so_luong = 100;
    
    // Thậm chí dùng với các Object phức tạp như std::string
    inline static std::string ten_he_thong = "He Thong AI Siêu Viet";
};


// ==========================================
// 2. MỨC TRUNG CẤP: BIẾN TOÀN CỤC TRONG THƯ VIỆN HEADER-ONLY
// ==========================================
/* 
 Giả sử toàn bộ Namespace dưới đây nằm trong file "MyLibrary.h". 
 Bất kỳ file .cpp nào #include "MyLibrary.h" đều xài chung 1 biến cấu hình này,
 không bao giờ bị lỗi Multiple Definition.
*/
namespace ThuVienCuaToi {
    // Biến toàn cục nội tuyến (Global inline variable)
    inline int muc_do_canh_bao = 1;
    
    inline void in_canh_bao(const std::string& msg) {
        if (muc_do_canh_bao >= 1) {
            std::cout << "[CANH BAO]: " << msg << "\n";
        }
    }
}


// ==========================================
// 3. MỨC NÂNG CAO: CONSTEXPR MẶC ĐỊNH LÀ INLINE
// ==========================================
class HangSoHeThong {
public:
    // Từ C++17, từ khóa 'constexpr static' NGẦM ĐỊNH BAO GỒM LUÔN 'inline'.
    // Bạn không cần phải viết 'inline constexpr static'. Trình biên dịch tự hiểu!
    constexpr static double PI = 3.14159265359;
    constexpr static int SO_LUONG_MAX = 999;
};


// ==========================================
// 4. MỨC CHUYÊN GIA: SINGLETON PATTERN GỌN NHẤT LỊCH SỬ
// ==========================================
// Nhờ inline variable, việc viết mẫu thiết kế Singleton (Mẫu Độc Bản) 
// giờ đây chỉ cần vài dòng ngay trong file Header, không cần chật vật với file .cpp nữa.
class KetNoiDatabase {
private:
    // Constructor private (Cấm tạo object mới)
    KetNoiDatabase() { 
        std::cout << "  [Singleton] Khoi tao Ket Noi Database (Chi chay 1 lan duy nhat!)\n"; 
    }

public:
    // Chặn copy
    KetNoiDatabase(const KetNoiDatabase&) = delete;

    // C++17 MAGIC: Khởi tạo luôn Instance tĩnh tại đây!
    inline static KetNoiDatabase instance;

    void truy_van() { 
        std::cout << "  [Singleton] Dang truy van du lieu...\n"; 
    }
};


// ==========================================
// 5. HIỆN ĐẠI TỐI THƯỢNG: INLINE KẾT HỢP THREAD_LOCAL
// ==========================================
// Biến thread_local nghĩa là: Mỗi một Luồng (Thread) sẽ có một bản sao riêng của biến này.
// Việc kết hợp inline + thread_local giúp bạn khai báo biến luồng cục bộ an toàn trong file Header.
namespace DaLuong {
    // Mỗi Thread sẽ có một biến 'id_luong' riêng, không ai tranh giành RAM của ai.
    // Và không bị lỗi Linker khi #include nhiều lần.
    inline thread_local int id_luong_hien_tai = 0;
}

void chay_tren_luong_con() {
    DaLuong::id_luong_hien_tai = 99; // Chỉ đổi giá trị trên luồng con
    std::cout << "Gia tri tren Luong Con: " << DaLuong::id_luong_hien_tai << "\n";
}


// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
    std::cout << "===== 1. INLINE STATIC MEMBER =====\n";
    std::cout << "So luong hien dai: " << KieuHienDai::so_luong << "\n";
    std::cout << "Ten he thong: " << KieuHienDai::ten_he_thong << "\n";

    std::cout << "\n===== 2. HEADER-ONLY GLOBAL VARIABLE =====\n";
    ThuVienCuaToi::muc_do_canh_bao = 2; // Đổi cấu hình toàn cục
    ThuVienCuaToi::in_canh_bao("Ket noi mang khong on dinh!");

    std::cout << "\n===== 3. CONSTEXPR IMPLICIT INLINE =====\n";
    std::cout << "Hang so PI: " << HangSoHeThong::PI << "\n";

    std::cout << "\n===== 4. C++17 SINGLETON =====\n";
    // instance đã được khởi tạo tự động ở trên, ta chỉ việc lấy ra xài
    KetNoiDatabase::instance.truy_van();

    std::cout << "\n===== 5. INLINE THREAD_LOCAL =====\n";
    DaLuong::id_luong_hien_tai = 1; // Luồng Main mang số 1
    
    std::thread luong_con(chay_tren_luong_con);
    luong_con.join(); // Đợi luồng con chạy xong

    // Quay lại luồng Main, biến này vẫn là 1 (Không bị luồng con ghi đè)
    std::cout << "Gia tri tren Luong Main: " << DaLuong::id_luong_hien_tai << "\n";

    std::cout << "\n===== HOAN THANH =====\n";
    return 0;
}