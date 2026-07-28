#include <iostream>
#include <string>
#include <thread>
#include <vector>

// ĐÂY LÀ THƯ VIỆN ĐẠI DIỆN CHO <net> (Networking TS) TRONG TƯƠNG LAI
#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/use_awaitable.hpp>

using boost::asio::ip::tcp; // Giao thức TCP
namespace asio = boost::asio;

// ==========================================
// LÝ THUYẾT NỀN TẢNG (THEORY)
// ==========================================
/*
 1. Trái tim của Networking TS: `io_context` (Bộ điều phối I/O)
    - Mọi chương trình mạng trong C++ đều cần 1 io_context. Nó đóng vai trò là
      Tổng đài viên (Event Loop) giao tiếp với Hệ điều hành (Windows/Linux) để
      biết khi nào dữ liệu mạng đến hoặc đi.

 2. Resolver (Người phân giải tên miền):
    - Biến "www.google.com" thành địa chỉ IP (VD: 142.250.190.46) để máy tính
 hiểu.

 3. Socket (Ổ cắm mạng):
    - Điểm cuối của kết nối. Nó giống như ống nước, dữ liệu chảy vào/ra qua đây.
*/

// ==========================================
// 1. MỨC CƠ BẢN: CLIENT ĐỒNG BỘ (SYNCHRONOUS) - TẢI 1 TRANG WEB
// ==========================================
// ĐỒNG BỘ nghĩa là Code sẽ bị "đóng băng" (Block) chờ cho đến khi tải xong
// mạng.
void client_dong_bo_co_ban() {
  std::cout << "\n--- 1. BAZIC: SYNCHRONOUS HTTP CLIENT ---\n";
  try {
    asio::io_context io; // 1. Khởi tạo trái tim của hệ thống
    tcp::resolver resolver(io);
    tcp::socket socket(io);

    // 2. Phân giải tên miền (DNS) của trang example.com cổng 80 (HTTP)
    auto endpoints = resolver.resolve("example.com", "80");

    // 3. Kết nối Socket tới IP vừa tìm được
    asio::connect(socket, endpoints);
    std::cout << "[Client] Da ket noi toi example.com!\n";

    // 4. Tạo gói tin HTTP GET Request
    std::string request = "GET / HTTP/1.1\r\n"
                          "Host: example.com\r\n"
                          "Connection: close\r\n\r\n";

    // 5. Gửi gói tin đi
    asio::write(socket, asio::buffer(request));

    // 6. Đọc gói tin trả về từ Server
    char reply[1024];
    size_t do_dai = socket.read_some(asio::buffer(reply, 1024));

    std::cout << "[Client] Phan hoi tu Server:\n";
    std::cout.write(reply, do_dai);
    std::cout << "...\n"; // Chỉ in một đoạn ngắn cho đỡ dài

  } catch (std::exception &e) {
    std::cerr << "Loi Mang: " << e.what() << "\n";
  }
}

// ==========================================
// 2. MỨC TRUNG CẤP: MÔ HÌNH BẤT ĐỒNG BỘ (CALLBACKS)
// ==========================================
// BẤT ĐỒNG BỘ: Gửi yêu cầu mạng xong, CPU đi làm việc khác.
// Khi nào mạng tải xong, Hệ điều hành sẽ gọi hàm "Callback" để báo tin.
// ĐÂY CHÍNH LÀ MÔ HÌNH MÀ C++ ĐANG MUỐN THAY THẾ (Vì Callback Hell - Code rối
// rắm).

void ket_noi_bat_dong_bo(asio::io_context &io) {
  auto socket = std::make_shared<tcp::socket>(io);
  auto resolver = std::make_shared<tcp::resolver>(io);

  // Bắt đầu chuỗi Callback (Hàm nọ lồng hàm kia)
  resolver->async_resolve(
      "example.com", "80",
      [socket](const boost::system::error_code &error,
               tcp::resolver::results_type results) {
        if (!error) {
          asio::async_connect(
              *socket, results,
              [](const boost::system::error_code &error,
                 const tcp::endpoint &) {
                if (!error) {
                  std::cout << "[Async] Ket noi thanh cong! (Qua Callback)\n";
                  // Tưởng tượng gọi thêm async_write, rồi async_read... Code sẽ
                  // lùi sâu vào trong (Callback Hell).
                }
              });
        }
      });
}

void chay_demo_callback() {
  std::cout << "\n--- 2. INTERMEDIATE: ASYNCHRONOUS CALLBACKS ---\n";
  asio::io_context io;
  ket_noi_bat_dong_bo(io);

  std::cout << "Lenh nay chay NGAY LẬP TỨC (Khong bi block boi mang)...\n";

  // BẮT BUỘC PHẢI CÓ LỆNH NÀY: Kích hoạt Event Loop để các hàm Callbacks bên
  // trên được thực thi.
  io.run();
}

// ==========================================
// 3. MỨC CHUYÊN GIA: SERVER BẤT ĐỒNG BỘ VỚI C++20 COROUTINES
// ==========================================
// TƯƠNG LAI CỦA C++ NETWORKING LÀ ĐÂY!
// Kết hợp Networking TS với C++20 Coroutines (co_await).
// Code viết trông như Đồng bộ (dễ hiểu, tuân tự từ trên xuống dưới), nhưng dưới
// nền nó chạy Bất đồng bộ 100%!

// Một "Luồng ảo" (Coroutine) xử lý từng Client kết nối tới
asio::awaitable<void> xu_ly_client(tcp::socket socket) {
  try {
    char data[1024];
    for (;;) { // Vòng lặp lắng nghe Client mãi mãi
      // co_await: Tạm "đóng băng" luồng ảo này, nhường CPU cho việc khác.
      // Khi nào Client gửi data tới, nó mới tự động "thức dậy" chạy tiếp.
      std::size_t n = co_await socket.async_read_some(asio::buffer(data),
                                                      asio::use_awaitable);

      // Tạo ra chức năng "Echo Server": Client gửi gì, nhại lại y hệt
      co_await asio::async_write(socket, asio::buffer(data, n),
                                 asio::use_awaitable);
    }
  } catch (std::exception &e) {
    std::cout << "[Server] Client ngat ket noi: " << e.what() << "\n";
  }
}

// Coroutine lắng nghe các kết nối mới
asio::awaitable<void> lang_nghe_port_8080() {
  // Executor là người quản lý tài nguyên của coroutine này (tương tự
  // io_context)
  auto executor = co_await asio::this_coro::executor;

  // Mở port 8080 trên Localhost
  tcp::acceptor acceptor(executor, {tcp::v4(), 8080});
  std::cout << "[Server] Dang lang nghe tren cong 8080...\n";

  for (;;) {
    // co_await: Ngủ đông chờ cho đến khi có 1 Client thực sự kết nối tới
    tcp::socket socket = co_await acceptor.async_accept(asio::use_awaitable);
    std::cout << "[Server] Co 1 Client vua ket noi vao!\n";

    // C++20 co_spawn: Bắn ra một Coroutine mới để phục vụ Client này,
    // trong khi vòng lặp này quay lại tiếp tục chờ Client khác.
    asio::co_spawn(executor, xu_ly_client(std::move(socket)), asio::detached);
  }
}

void chay_server_cxx20() {
  std::cout << "\n--- 3. EXPERT: ASYNC ECHO SERVER VỚI C++20 COROUTINES ---\n";
  try {
    asio::io_context io_context(1); // Chạy trên 1 Thread duy nhất

    // Kích hoạt Coroutine chính
    asio::co_spawn(io_context, lang_nghe_port_8080(), asio::detached);

    // Sẽ Block tại đây để Server chạy vĩnh viễn
    // Ghi chú: Trong code demo này, tôi sẽ comment lệnh io_context.run() lại
    // để chương trình có thể kết thúc. Nếu muốn chạy Server thật, hãy mở
    // comment ra.

    std::cout << "[Demo] De chay server that, hay mo comment lenh "
                 "io_context.run();\n";
    // io_context.run();

  } catch (std::exception &e) {
    std::printf("Exception: %s\n", e.what());
  }
}

// ==========================================
// TỔNG KẾT VỀ TƯƠNG LAI CỦA C++26 / C++29
// ==========================================
/*
 MÔ HÌNH SENDERS/RECEIVERS (P2300 - std::execution):
 C++26 sẽ đưa vào `std::execution`. Thay vì dùng Coroutines (co_await) hay
 Callback, C++ mới sẽ quản lý mạng theo kiểu "Chuỗi ống nước" (Lazy Evaluation):

 auto cong_viec_mang =
    std::execution::schedule(io)
    | std::execution::then([]{ return ket_noi_mang(); })
    | std::execution::then([](auto data){ return doc_du_lieu(data); });

 std::this_thread::sync_wait(cong_viec_mang); // Lúc này mạng mới THỰC SỰ chạy!

 Điểm mạnh: Hiệu năng tuyệt đối, không tốn RAM cấp phát động (Zero Allocation)
 cho các biến môi trường như Callback/Coroutines, giúp C++ duy trì vị thế ngôn
 ngữ nhanh nhất thế giới.
*/

// ==========================================
// CHƯƠNG TRÌNH CHÍNH
// ==========================================
int main() {
  std::cout << "===== C++ NETWORKING TS (THONG QUA ASIO) =====\n";

  // 1. Chạy Client Đồng Bộ (Tải Web thực tế)
  client_dong_bo_co_ban();

  // 2. Chạy Client Bất Đồng Bộ (Callback)
  chay_demo_callback();

  // 3. Khởi chạy kiến trúc Server (Coroutine)
  chay_server_cxx20();

  std::cout << "\n===== HOAN THANH =====\n";
  return 0;
}