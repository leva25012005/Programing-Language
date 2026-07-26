#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

// ==========================================
// CLASS PHỤ TRỢ CHO PHẦN NÂNG CAO
// ==========================================
class NhanVien {
public:
    std::string ten;
    int tuoi;

    NhanVien(std::string t, int tu) : ten(t), tuoi(tu) {
        std::cout << "  [Tao moi] Nhan vien: " << ten << "\n";
    }
    
    // Copy Constructor (Để quan sát khi vector sao chép phần tử)
    NhanVien(const NhanVien& other) : ten(other.ten), tuoi(other.tuoi) {
        std::cout << "  [Copy] Nhan vien: " << ten << "\n";
    }
    
    // Move Constructor (Để quan sát tối ưu hóa bộ nhớ)
    NhanVien(NhanVien&& other) noexcept : ten(std::move(other.ten)), tuoi(other.tuoi) {
        std::cout << "  [Move] Nhan vien: " << ten << "\n";
    }
};

// ==========================================
// 1. MỨC CƠ BẢN: KHỞI TẠO VÀ TRUY XUẤT
// ==========================================
void vector_co_ban() {
    std::cout << "\n--- 1. BAZIC: KHOI TAO & TRUY XUAT ---\n";
    
    // Khởi tạo
    std::vector<int> v1;                     // Vector rỗng
    std::vector<int> v2 = {10, 20, 30, 40};  // Khởi tạo có sẵn dữ liệu (List initialization)
    std::vector<int> v3(5, 100);             // Tạo vector có 5 phần tử, mỗi phần tử đều là 100

    // Thêm phần tử vào cuối (O(1))
    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);

    // Truy xuất dữ liệu
    std::cout << "Phan tu dau tien (v2): " << v2.front() << "\n";
    std::cout << "Phan tu cuoi cung (v2): " << v2.back() << "\n";
    
    // Dùng [] - Nhanh nhưng KHÔNG kiểm tra tràn viền
    std::cout << "Phan tu thu hai (v2): " << v2[1] << "\n";
    
    // Dùng .at() - Chậm hơn một chút nhưng AN TOÀN (Ném ngoại lệ out_of_range nếu tràn viền)
    try {
        std::cout << v2.at(10) << "\n"; 
    } catch (const std::out_of_range& e) {
        std::cout << "Loi truy xuat: " << e.what() << "\n";
    }

    // Duyệt vector (Range-based for loop - Khuyên dùng)
    std::cout << "Duyet v1: ";
    for (const int& val : v1) { // Dùng const & để tránh copy dữ liệu
        std::cout << val << " ";
    }
    std::cout << "\n";
}

// ==========================================
// 2. MỨC TRUNG CẤP: QUẢN LÝ BỘ NHỚ (Size vs Capacity)
// ==========================================
void vector_bo_nho() {
    std::cout << "\n--- 2. INTERMEDIATE: SIZE vs CAPACITY ---\n";
    
    std::vector<int> v;
    std::cout << "Ban dau - Size: " << v.size() << ", Capacity: " << v.capacity() << "\n";

    // Khi vượt quá capacity, vector tự động nhân đôi bộ nhớ (Reallocation)
    for (int i = 0; i < 17; ++i) {
        v.push_back(i);
    }
    std::cout << "Sau 17 lan push - Size: " << v.size() << ", Capacity: " << v.capacity() << "\n";
    // Size = 17 (Số lượng thực tế đang dùng). Capacity thường là 32 (Dung lượng RAM đã cấp phát).

    // TỐI ƯU HÓA: Dùng reserve() nếu biết trước số lượng phần tử để tránh Reallocation tốn CPU
    std::vector<int> v_toi_uu;
    v_toi_uu.reserve(100); // Xin ngay 100 chỗ trong RAM từ đầu
    std::cout << "V_toi_uu - Size: " << v_toi_uu.size() << ", Capacity: " << v_toi_uu.capacity() << "\n";

    // Thu hồi RAM thừa
    v.shrink_to_fit(); 
    std::cout << "Sau khi shrink_to_fit - Size: " << v.size() << ", Capacity: " << v.capacity() << "\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: THAO TÁC SÂU VÀ THUẬT TOÁN
// ==========================================
void vector_thao_tac() {
    std::cout << "\n--- 3. ADVANCED: THEM/XOA O GIUA & THUAT TOAN ---\n";
    
    std::vector<int> v = {10, 20, 30, 40, 50};

    // Chèn vào giữa (Rất chậm O(n) vì phải dịch chuyển mảng)
    v.insert(v.begin() + 2, 99); // Chèn số 99 vào vị trí index 2

    // Xóa ở giữa (Cũng rất chậm O(n))
    v.erase(v.begin() + 1); // Xóa phần tử ở index 1 (số 20)

    // Duyệt qua Iterator
    std::cout << "Sau khi insert/erase: ";
    for (auto it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // Kết hợp với thư viện <algorithm>
    // 1. Đảo ngược mảng
    std::reverse(v.begin(), v.end());
    
    // 2. Sắp xếp tăng dần
    std::sort(v.begin(), v.end());
    
    // 3. Sắp xếp giảm dần (Dùng Lambda expression)
    std::sort(v.begin(), v.end(), [](int a, int b) {
        return a > b;
    });

    // 4. Lọc và xóa phần tử có điều kiện (Erase-Remove Idiom - Tiêu chuẩn trước C++20)
    // Xóa tất cả các số lẻ
    v.erase(std::remove_if(v.begin(), v.end(), [](int x) {
        return x % 2 != 0;
    }), v.end());
    
    /* 
    LƯU Ý C++20: Bạn có thể dùng hàm cực ngắn gọn sau thay cho Erase-Remove Idiom:
    std::erase_if(v, [](int x) { return x % 2 != 0; });
    */

    std::cout << "Sau khi xoa so le: ";
    for (int val : v) std::cout << val << " ";
    std::cout << "\n";
}

// ==========================================
// 4. MỨC CHUYÊN GIA: emplace_back vs push_back
// ==========================================
void vector_chuyen_gia_objects() {
    std::cout << "\n--- 4. EXPERT: EMPLACE_BACK vs PUSH_BACK ---\n";
    
    std::vector<NhanVien> danh_sach;
    // Cấp phát trước 2 chỗ để tránh Reallocation làm nhiễu log quan sát
    danh_sach.reserve(2); 

    std::cout << "Dung push_back:\n";
    // push_back tạo ra một Object tạm thời, sau đó MỚI COPY/MOVE vào vector. Rất tốn kém.
    danh_sach.push_back(NhanVien("Nguyen Van A", 25)); 

    std::cout << "\nDung emplace_back:\n";
    // emplace_back nhận thẳng tham số, và tự động gọi Constructor ĐỂ TẠO OBJECT NGAY BÊN TRONG VECTOR.
    // Không hề có Copy hay Move thừa thãi nào!
    danh_sach.emplace_back("Tran Thi B", 22); 
}

// ==========================================
// 5. VECTOR 2 CHIỀU (MA TRẬN)
// ==========================================
void vector_2d() {
    std::cout << "\n--- 5. VECTOR 2D (MA TRAN) ---\n";
    
    int so_hang = 3;
    int so_cot = 4;
    
    // Khởi tạo vector 2D với 3 hàng, mỗi hàng là một vector có 4 cột mang giá trị mặc định là 0
    std::vector<std::vector<int>> matrix(so_hang, std::vector<int>(so_cot, 0));

    matrix[1][2] = 99; // Gán giá trị

    for (const auto& hang : matrix) {
        for (const int& phan_tu : hang) {
            std::cout << phan_tu << "\t";
        }
        std::cout << "\n";
    }
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
    vector_co_ban();
    vector_bo_nho();
    vector_thao_tac();
    vector_chuyen_gia_objects();
    vector_2d();
    
    std::cout << "\n--- HOAN THANH ---\n";
    return 0;
}