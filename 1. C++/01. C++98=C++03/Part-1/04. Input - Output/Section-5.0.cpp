#include <iostream>
#include <string> // Cần thiết để làm việc với chuỗi văn bản (std::string)

int main() {
  // -------------------------------------------------------------------------
  // 1. NHẬP XUẤT CƠ BẢN VỚI SỐ (std::cin và std::cout)
  // -------------------------------------------------------------------------
  int tuoi = 0;
  double diem_so = 0.0;

  // Xuất thông báo ra màn hình
  std::cout << "--- 1. NHAP XUAT SO CO BAN ---" << std::endl;
  std::cout << "Nhap tuoi cua ban: ";

  // Nhập dữ liệu từ bàn phím và lưu vào biến 'tuoi'
  std::cin >> tuoi;

  std::cout << "Nhap diem thi (vi du: 8.5): ";
  std::cin >> diem_so;

  // Bạn có thể "nối" nhiều dữ liệu trên một dòng bằng nhiều dấu <<
  std::cout << "=> Ban " << tuoi << " tuoi va dat " << diem_so << " diem."
            << std::endl;
  std::cout << "------------------------------------------------" << std::endl;

  // -------------------------------------------------------------------------
  // 2. CÁI BẪY KINH ĐIỂN: NHẬP CHUỖI CÓ DẤU CÁCH (DÙNG CIN vs GETLINE)
  // -------------------------------------------------------------------------
  std::cout << "--- 2. NHAP CHUOI CHUA KHOANG TRANG ---" << std::endl;

  // Xóa bộ nhớ đệm (Buffer Flush)
  // Khi bạn bấm Enter ở bước nhập số trước đó, ký tự xuống dòng '\n' vẫn kẹt
  // lại trong bộ nhớ. std::cin.ignore() sẽ loại bỏ ký tự kẹt đó để không làm
  // hỏng lệnh nhập chuỗi phía sau.
  std::cin.ignore();

  std::string ho_ten;
  std::cout << "Nhap ho va ten day du cua ban: ";

  // NẾU DÙNG: std::cin >> ho_ten; -> Nó sẽ dừng lại ngay khi gặp dấu cách đầu
  // tiên! VÌ VẬY: Phải dùng std::getline() để đọc trọn vẹn cả dòng bao gồm cả
  // dấu cách.
  std::getline(std::cin, ho_ten);

  std::cout << "=> Xin chao ban: " << ho_ten << std::endl;
  std::cout << "------------------------------------------------" << std::endl;

  // -------------------------------------------------------------------------
  // 3. MỞ RỘNG TƯ DUY: SỰ KHÁC BIỆT GIỮA '\n' VÀ std::endl
  // -------------------------------------------------------------------------
  std::cout << "--- 3. HIEU VE HIEU NANG: '\\n' vs std::endl ---" << std::endl;

  // Cả hai đều xuống dòng, nhưng cơ chế bên dưới rất khác nhau:
  std::cout << "Dong 1" << std::endl; // Xuống dòng + Ép máy tính xóa bộ đệm để
                                      // in ra ngay (Tốn hiệu năng hơn)
  std::cout << "Dong 2"
            << '\n'; // Chỉ xuống dòng đơn thuần (Chạy nhanh hơn rất nhiều)

  return 0;
}