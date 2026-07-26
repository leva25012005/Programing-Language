#include <filesystem> // Thư viện nhân vật chính của chúng ta
#include <fstream>    // Để tạo file thực tế test thử
#include <iostream>
#include <string>
#include <system_error> // Cho std::error_code


// TẠO BÍ DANH (Namespace Alias):
// Vì chữ std::filesystem quá dài, mọi lập trình viên trên thế giới đều viết tắt
// nó thành 'fs'
namespace fs = std::filesystem;

// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Class cốt lõi: fs::path
    - Nó là trái tim của thư viện. Chứa đường dẫn (dạng chuỗi).
    - Ma thuật: Nó TỰ ĐỘNG nhận diện hệ điều hành. Dù bạn viết dấu xuyệt tới "/"
 (Linux) hay xuyệt ngược "\" (Windows), nó đều tự động chuẩn hóa cho đúng HĐH
 lúc chạy.

 2. Nạp chồng toán tử chia ( / ):
    - C++17 đã nạp chồng dấu chia / để NỐI ĐƯỜNG DẪN cực kỳ thanh lịch.
    - Thay vì viết: string path = folder + "\\" + filename;
    - Giờ ta viết: path = folder / filename; (Tự động thêm dấu xuyệt chuẩn).
*/

// Hàm phụ trợ tạo file để test
void tao_file_test(const fs::path &p, const std::string &noi_dung) {
  std::ofstream file(p);
  if (file)
    file << noi_dung;
}

// ==========================================
// 1. MỨC CƠ BẢN: PHẪU THUẬT ĐƯỜNG DẪN (PATH)
// ==========================================
void fs_co_ban_phau_thuat_path() {
  std::cout << "\n--- 1. BAZIC: PHAU THUAT DUONG DAN (PATH) ---\n";

  // Khởi tạo một đường dẫn (Chưa cần biết file có tồn tại thật hay không)
  fs::path p = "C:/DuLieu/Nam2026/BaoCao.txt";

  // Phẫu thuật lấy từng bộ phận cực kỳ dễ dàng (Thứ mà C++ cũ phải viết vòng
  // lặp cắt chuỗi)
  std::cout << "Duong dan goc: " << p << "\n";
  std::cout << "  - Thu muc cha (parent_path): " << p.parent_path() << "\n";
  std::cout << "  - Ten file co duoi (filename) : " << p.filename() << "\n";
  std::cout << "  - Ten file KO duoi (stem)    : " << p.stem() << "\n";
  std::cout << "  - Duoi file (extension)      : " << p.extension() << "\n";

  // Nối đường dẫn bằng toán tử ( / )
  fs::path thu_muc_moi = p.parent_path() / "HinhAnh" / "avatar.png";
  std::cout << "  - Noi duong dan moi: " << thu_muc_moi << "\n";
}

// ==========================================
// 2. MỨC TRUNG CẤP: TẠO, COPY, ĐỔI TÊN, XÓA THƯ MỤC
// ==========================================
void fs_thao_tac_thu_muc() {
  std::cout << "\n--- 2. INTERMEDIATE: CRUD (TAO/SUA/XOA FILE) ---\n";

  fs::path dir = "Sandbox_ThuMuc";
  fs::path file_goc = dir / "ban_goc.txt";
  fs::path file_copy = dir / "ban_sao.txt";

  // a. KIỂM TRA & TẠO THƯ MỤC
  if (!fs::exists(dir)) {
    // create_directory: Tạo 1 cấp. create_directories: Tạo nhiều cấp (VD:
    // a/b/c)
    fs::create_directory(dir);
    std::cout << "> Da tao thu muc: " << dir << "\n";
  }

  // Tạo 1 file test thực tế vào trong thư mục đó
  tao_file_test(file_goc, "Xin chao C++17 Filesystem!");

  // b. KIỂM TRA THUỘC TÍNH
  if (fs::is_regular_file(file_goc))
    std::cout << "> " << file_goc.filename() << " la mot File chuan.\n";
  if (fs::is_directory(dir))
    std::cout << "> " << dir << " la mot Thu muc.\n";

  // c. COPY & ĐỔI TÊN (RENAME)
  // std::filesystem::copy_options giúp ghi đè nếu file đã tồn tại
  fs::copy_file(file_goc, file_copy, fs::copy_options::overwrite_existing);
  std::cout << "> Da copy tu " << file_goc.filename() << " sang "
            << file_copy.filename() << "\n";

  fs::path file_doi_ten = dir / "ban_sao_da_doi_ten.txt";
  if (fs::exists(file_copy)) {
    fs::rename(
        file_copy,
        file_doi_ten); // Lệnh rename cũng có thể dùng để Move (Di chuyển) file
    std::cout << "> Da doi ten thanh: " << file_doi_ten.filename() << "\n";
  }

  // Xem kích thước file
  std::cout << "> Kich thuoc file goc: " << fs::file_size(file_goc)
            << " bytes\n";
}

// ==========================================
// 3. MỨC NÂNG CAO: DUYỆT CÂY THƯ MỤC (ITERATOR)
// ==========================================
void fs_duyet_thu_muc() {
  std::cout << "\n--- 3. ADVANCED: DUYET CAY THU MUC ---\n";

  fs::path dir = "Sandbox_ThuMuc";
  fs::create_directories(dir / "ThuMucCon_1"); // Tạo thêm thư mục con để test

  std::cout << ">>> DUYET 1 CAP (directory_iterator):\n";
  // Lặp qua tất cả các file/folder TRỰC TIẾP bên trong thư mục (Không chui vào
  // sâu hơn)
  for (const auto &entry : fs::directory_iterator(dir)) {
    std::cout << "    |-- " << entry.path().filename();
    if (entry.is_directory())
      std::cout << " [Thu Muc]";
    std::cout << "\n";
  }

  std::cout << "\n>>> DUYET DE QUY DA CAP (recursive_directory_iterator):\n";
  // Nó sẽ chui vào tận cùng ngóc ngách của tất cả các thư mục con (Giống lệnh
  // 'tree' trên Windows)
  for (const auto &entry : fs::recursive_directory_iterator(dir)) {
    // Dùng depth() để biết đang ở độ sâu bao nhiêu, từ đó thụt lề cho đẹp
    std::string khoang_cach(entry.depth() * 4, ' ');
    std::cout << khoang_cach << "|-- " << entry.path().filename() << "\n";
  }
}

// ==========================================
// 4. MỨC CHUYÊN GIA: BẮT LỖI AN TOÀN VỚI ERROR_CODE
// ==========================================
void fs_chuyen_gia_xu_ly_loi() {
  std::cout << "\n--- 4. EXPERT: ERROR_CODE (KHONG DUNG EXCEPTION) ---\n";

  // VẤN ĐỀ CHÍ MẠNG:
  // Nếu bạn kiểm tra fs::file_size() của một file mà BẠN KHÔNG CÓ QUYỀN TRUY
  // CẬP (Access Denied), hàm sẽ "ném ra" một Exception (Ngoại lệ), nếu không
  // try-catch, chương trình sẽ Crash!

  fs::path duong_dan_ao =
      "C:/Mot_Thu_Muc_Khong_Ton_Tai_Hoac_Cam_Truy_Cap/file.txt";

  // CÁCH 1: Dùng Try-Catch (Tốn hiệu năng, code dài dòng)
  try {
    auto size = fs::file_size(duong_dan_ao);
  } catch (const fs::filesystem_error &e) {
    std::cout << "[Ngoai le] Bat duoc loi: " << e.what() << "\n";
  }

  // CÁCH 2 TỐI THƯỢNG (Chuẩn Công nghiệp): Dùng std::error_code
  // Hầu hết mọi hàm trong <filesystem> đều có 1 phiên bản nhận thêm tham số
  // std::error_code. Thay vì ném Exception gây Crash, nó sẽ "âm thầm" ghi mã
  // lỗi vào biến này.

  std::error_code ec;

  // Gọi phiên bản có tham số 'ec' ở cuối
  auto size = fs::file_size(duong_dan_ao, ec);

  // Kiểm tra xem mã lỗi có bật lên không (Giá trị khác 0 là có lỗi)
  if (ec) {
    std::cout << "[An toan] Loi xay ra (Error Code): " << ec.message() << "\n";
    std::cout << "-> Chuong trinh van song sot ma khong can Try-Catch!\n";
  }
}

// ==========================================
// HÀM DỌN DẸP (XÓA TOÀN BỘ)
// ==========================================
void don_dep_sandbox() {
  std::cout << "\n--- DON DEP ---\n";
  fs::path dir = "Sandbox_ThuMuc";

  if (fs::exists(dir)) {
    // fs::remove() chỉ xóa được file hoặc thư mục RỖNG.
    // fs::remove_all() cực kỳ nguy hiểm, nó xóa vĩnh viễn (không vào thùng rác)
    // thư mục và mọi thứ bên trong!
    std::uintmax_t so_file_da_xoa = fs::remove_all(dir);
    std::cout << "> Da xoa an toan thu muc test va " << so_file_da_xoa - 1
              << " file ben trong.\n";
  }
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== KHOA HOC C++17 <FILESYSTEM> =====\n";

  fs_co_ban_phau_thuat_path();
  fs_thao_tac_thu_muc();
  fs_duyet_thu_muc();
  fs_chuyen_gia_xu_ly_loi();

  don_dep_sandbox();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}