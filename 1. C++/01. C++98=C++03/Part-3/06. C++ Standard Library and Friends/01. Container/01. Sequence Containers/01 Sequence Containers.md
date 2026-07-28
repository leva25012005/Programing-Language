# Sequence Containers trong C++

Sequence Containers trong C++ (như `std::vector`, `std::deque`, `std::list`, `std::array`, `std::forward_list`) lưu trữ dữ liệu theo cấu trúc tuyến tính.

Mặc dù mỗi loại có đặc thù riêng, tất cả đều chia sẻ một bộ các hàm cốt lõi để khởi tạo, truy xuất, sửa đổi và quản lý kích thước.

---

# 1. Truy xuất phần tử (Element Access)

Các hàm giúp lấy hoặc kiểm tra giá trị tại các vị trí cụ thể.

| Hàm                 | Chức năng                                                                             |
| ------------------- | ------------------------------------------------------------------------------------- |
| `front()`           | Lấy tham chiếu đến phần tử đầu tiên.                                                  |
| `back()`            | Lấy tham chiếu đến phần tử cuối cùng.                                                 |
| `operator[](index)` | Truy cập phần tử bằng chỉ số (**không kiểm tra biên**).                               |
| `at(index)`         | Truy cập phần tử có **kiểm tra biên**, sẽ phát sinh ngoại lệ nếu vượt quá kích thước. |

---

# 2. Dung lượng & Kích thước (Capacity)

Các hàm kiểm tra số lượng phần tử hiện tại và không gian bộ nhớ của container.

| Hàm          | Chức năng                                                                                               |
| ------------ | ------------------------------------------------------------------------------------------------------- |
| `size()`     | Trả về số lượng phần tử hiện có.                                                                        |
| `max_size()` | Trả về số lượng phần tử tối đa mà container có thể chứa.                                                |
| `empty()`    | Kiểm tra container có rỗng hay không (`true` nếu rỗng).                                                 |
| `capacity()` | _(Chỉ có ở `vector`, `deque`, `string`)_ Trả về dung lượng hiện tại trước khi cần cấp phát thêm bộ nhớ. |
| `reserve(n)` | _(Chỉ có ở `vector`)_ Cấp phát trước bộ nhớ đủ chứa `n` phần tử.                                        |
| `resize(n)`  | Thay đổi kích thước container thành `n` phần tử.                                                        |

---

# 3. Sửa đổi nội dung (Modifiers)

Các hàm dùng để thêm, xóa hoặc chỉnh sửa phần tử.

| Hàm                     | Chức năng                                                                              |
| ----------------------- | -------------------------------------------------------------------------------------- |
| `push_back(val)`        | Thêm phần tử vào cuối container.                                                       |
| `pop_back()`            | Xóa phần tử cuối cùng.                                                                 |
| `insert(iterator, val)` | Chèn phần tử tại vị trí của iterator.                                                  |
| `erase(iterator)`       | Xóa phần tử tại vị trí của iterator.                                                   |
| `clear()`               | Xóa toàn bộ phần tử trong container.                                                   |
| `assign(n, val)`        | Gán lại container gồm `n` phần tử có giá trị `val`.                                    |
| `emplace(...)`          | Tạo trực tiếp đối tượng tại vị trí cần chèn, tránh tạo bản sao, giúp tối ưu hiệu năng. |

---

# 4. Trình lặp (Iterators)

Các hàm trả về iterator để duyệt hoặc thao tác với container.

| Hàm         | Chức năng                                           |
| ----------- | --------------------------------------------------- |
| `begin()`   | Iterator trỏ đến phần tử đầu tiên.                  |
| `end()`     | Iterator trỏ đến vị trí ngay sau phần tử cuối cùng. |
| `rbegin()`  | Iterator đảo ngược, bắt đầu từ phần tử cuối.        |
| `rend()`    | Iterator đảo ngược kết thúc trước phần tử đầu tiên. |
| `cbegin()`  | Iterator chỉ đọc (const) đến phần tử đầu tiên.      |
| `cend()`    | Iterator chỉ đọc đến vị trí sau phần tử cuối.       |
| `crbegin()` | Iterator đảo ngược chỉ đọc.                         |
| `crend()`   | Iterator đảo ngược chỉ đọc kết thúc.                |

---

# 5. Các hàm bổ sung (Tùy loại Container)

Một số container cung cấp thêm các hàm chuyên biệt.

| Hàm               | Container hỗ trợ                | Chức năng                                                                                 |
| ----------------- | ------------------------------- | ----------------------------------------------------------------------------------------- |
| `push_front(val)` | `deque`, `list`, `forward_list` | Thêm phần tử vào đầu.                                                                     |
| `pop_front()`     | `deque`, `list`, `forward_list` | Xóa phần tử đầu tiên.                                                                     |
| `sort()`          | `list`, `forward_list`          | Sắp xếp phần tử bằng thuật toán tối ưu dành riêng cho danh sách liên kết.                 |
| `splice()`        | `list`                          | Di chuyển các phần tử từ danh sách này sang danh sách khác mà không cần sao chép dữ liệu. |

---

# Tóm tắt nhanh

## 1. Element Access

- `front()`
- `back()`
- `[]`
- `at()`

## 2. Capacity

- `size()`
- `max_size()`
- `empty()`
- `capacity()`
- `reserve()`
- `resize()`

## 3. Modifiers

- `push_back()`
- `pop_back()`
- `insert()`
- `erase()`
- `clear()`
- `assign()`
- `emplace()`

## 4. Iterators

- `begin()`
- `end()`
- `rbegin()`
- `rend()`
- `cbegin()`
- `cend()`
- `crbegin()`
- `crend()`

## 5. Hàm chuyên biệt

- `push_front()`
- `pop_front()`
- `sort()`
- `splice()`

---

# Ghi chú

Không phải tất cả các **Sequence Containers** đều hỗ trợ mọi hàm.

Ví dụ:

| Container           | Đặc điểm                                                           |
| ------------------- | ------------------------------------------------------------------ |
| `std::vector`       | Truy cập ngẫu nhiên nhanh, có `capacity()` và `reserve()`.         |
| `std::deque`        | Thêm/xóa đầu và cuối nhanh.                                        |
| `std::list`         | Danh sách liên kết đôi, hỗ trợ `sort()`, `splice()`.               |
| `std::forward_list` | Danh sách liên kết đơn, chỉ duyệt một chiều, không có `size()`.    |
| `std::array`        | Kích thước cố định, không có `push_back()`, `insert()`, `erase()`. |
