# 🎲 Monte Carlo Casino Simulation: Biểu đồ Phân phối Xác suất Chiến thắng trong mô hình trò chơi Liêng theo Hệ số Tâm lý

![C++](https://img.shields.io/badge/C++-17-00599C?style=flat-square&logo=c%2B%2B)
![Simulation](https://img.shields.io/badge/Simulation-Monte_Carlo-brightgreen?style=flat-square)
![Data Analysis](https://img.shields.io/badge/Data_Analysis-HTML/JS-orange?style=flat-square)

Mô hình mô phỏng tài chính rủi ro hiệu suất cao áp dụng phương pháp Monte Carlo để phân tích sự phân phối xác suất chiến thắng và sự biến động tâm lý của người chơi. Trọng tâm của mô hình này là mô phỏng trò chơi bài truyền thống của Việt Nam "**Liêng**", đánh giá tỷ lệ thắng thua khi bị tác động bởi hệ số tâm lý trong các vòng cược.

---

## 🧮 1. Tổng quan & Luật chơi cơ bản

Mô hình mô phỏng trò chơi **Liêng** sử dụng bộ bài Tây 52 lá, giới hạn 6 người chơi. Mỗi người nhận 3 lá bài ngẫu nhiên và tiến hành cược để tìm ra người chiến thắng.

**Thứ tự độ mạnh của bài (Từ Cao xuống Thấp):**
- **Liêng Sám Cô (3 lá Át):** Bài Thánh - Tỉ lệ thắng tuyệt đối.
- **Liêng (3 lá cùng số):** Ví dụ: 3 lá 8, 3 lá K.
- **Sảnh Rồng:** A-K-Q đồng chất.
- **Sảnh:** 3 lá liên tiếp đồng chất (Ví dụ: 7-8-9 cơ).
- **Thùng:** 3 lá đồng chất nhưng không liên tiếp.
- **Sảnh Rác:** 3 lá liên tiếp khác chất.
- **Đôi:** 2 lá giống nhau.
- **Bài Rác:** 3 lá không có liên kết (Không có giá trị).

**Cơ chế vòng cược:**
- **Tiền chân (Ante):** Bắt buộc đóng trước khi chia bài.
- **Theo:** Bỏ ra số tiền bằng với mức cược cao nhất hiện tại trên bàn.
- **Tố:** Nâng mức cược lên cao hơn.
- **Bỏ bài:** Chấp nhận thua, mất toàn bộ tiền chân và tiền đã cược trước đó.
- **Phân định (Showdown):** Ai trụ lại cuối cùng có bài cao nhất sẽ ăn toàn bộ tiền cược. Nếu có nhiều người cùng độ mạnh bài, tổng tiền cược được chia đều.

---

## 💡 2. Ý tưởng Khảo sát & Thiết lập Mô hình

Mô hình đánh giá sự ảnh hưởng của **yếu tố tâm lý ban đầu** đến kết quả tài chính chung cuộc của người chơi. Hành vi cược (Theo/Tố/Bỏ) không chỉ phụ thuộc vào độ mạnh của bài mà còn bị chi phối bởi trạng thái tâm lý.

**Tham số khởi tạo (Initial State):**
- **Số lượng:** 6 người chơi.
- **Vốn ban đầu:** Một số tiền rất lớn (không giới hạn).
- **Tâm lý khởi điểm:** Mỗi người có một chỉ số tâm lý ngẫu nhiên từ 1 đến 10 (Ký hiệu: $x$, với $1 \le x \le 10$).

---

## 📊 3. Công thức Lượng hóa Hành vi

Quyết định cược được tính toán dựa trên **Chỉ số Tiếp tục (C)**. Chỉ số này đại diện cho sự tự tin của người chơi trong ván đấu.

**Công thức:**
> **Chỉ số Tiếp tục (C) = Tâm lý × Tỉ lệ chiến thắng của tổ hợp bài**

Dựa vào Chỉ số (C), mỗi người chơi sẽ có một **Ngưỡng chịu đựng rủi ro (Giới hạn cược tối đa)** như sau:

| Chỉ số Tiếp tục (C) | Giới hạn Cược Tối Đa | Ghi chú |
| :---: | :---: | :--- |
| **0 - 500** | 0 đồng | Chỉ đóng tiền chân, hễ có người Tố là Bỏ bài. |
| **501 - 600** | 50 đồng | |
| **601 - 700** | 150 đồng | |
| **701 - 800** | 300 đồng | |
| **801 - 850** | 500 đồng | |
| **851 - 880** | 650 đồng | |
| **881 - 910** | 750 đồng | |
| **911 - 940** | 850 đồng | |
| **941 - 970** | 920 đồng | |
| **971 - 990** | 960 đồng | |
| **990 - 1000** | 1000 đồng | Max Limit - Mức cược kịch trần. |

> **Luật ngoại lệ (Exception):** Nếu người chơi nhận được các tổ hợp đặc biệt: *Liêng Sám Cô, Liêng, hoặc Sảnh Rồng*, họ sẽ bỏ qua yếu tố tâm lý và luôn luôn Tố ở mức tối đa.

---

## 🔄 4. Luồng thực thi của một Ván bài (Round)

1. **Thu tiền chân:** Bắt đầu ván, cả 6 người chơi đều bị trừ 5 đồng tiền chân vào tổng cược (Pot).
2. **Tính toán mức cược:**
   - Nếu Chỉ số (C) $\le$ 500: Người chơi cược 0 đồng (chỉ chờ xem xét Theo/Bỏ).
   - Nếu Chỉ số (C) $>$ 500: Người chơi muốn cược một số tiền được tính bằng công thức: 
     **Mức cược dự kiến = C × 0.01 × (Số Round)**
   - *Mức cược thực tế được đưa ra sẽ không được vượt quá Giới hạn Cược Tối Đa trong bảng trên.*
3. **So khớp cược (Theo/Bỏ/Mở bài):**
   - Tổng hợp các mức cược để tìm ra "Số tiền cược cao nhất" trên bàn.
   - Nếu muốn tiếp tục (Theo), người chơi phải bỏ ra số tiền bằng với mức cược cao nhất này.
   - Nếu mức cược cao nhất vượt qua "Giới hạn Cược Tối Đa" của một người chơi, tất cả những người còn lại trên bàn sẽ bị buộc phải Mở bài (Showdown) để phân định thắng thua ngay lập tức.
   - Người chơi chọn Bỏ bài sẽ mất toàn bộ tiền vốn đã góp vào ván đó.
4. **Cập nhật Trạng thái Tâm lý:** Sau khi có kết quả thắng thua:
   - **Người chiến thắng:** Tâm lý được cộng thêm +1.
   - **Người thua cuộc:** Tâm lý bị trừ đi -0.2.

---

## 📈 5. Khảo sát và Trực quan hóa Dữ liệu

Tiến hành chạy lặp lại chu trình trên qua **100.000 lượt chơi (ván bài)**. Dữ liệu tài chính (số tiền kiếm được/mất đi) và sự biến động tâm lý của 6 người chơi sẽ được ghi nhận sau mỗi lượt để vẽ biểu đồ trực quan, từ đó phân tích tác động của tâm lý ban đầu đến kết quả lợi nhuận cuối cùng.

---

## ⚙️ Setup & Usage

### 1. Prerequisites (Yêu cầu hệ thống)
- Một trình biên dịch C++ hiện đại hỗ trợ **C++17** trở lên.
- Đảm bảo bạn đã có thư mục `data/` trống trong thư mục mã nguồn (`Lieng_Simulation/data`) để engine C++ có thể lưu trữ logs đầu ra.

### 2. Quick Run (Dành cho Windows - Chạy nhanh không cần cài đặt)

Nếu bạn đã có sẵn **MinGW** hoặc **g++** trên Windows, hãy mở **PowerShell** và chạy dòng lệnh dưới đây để biên dịch và chạy ngay lập tức. Chương trình sẽ tự xóa file thực thi sau khi hoàn tất:

```powershell
# 1. Clone dự án
git clone https://github.com/truongphuc008/AnalysisLieng.git
cd AnalysisLieng/Lieng_Simulation

# 2. Biên dịch và chạy (PowerShell)
g++ *.cpp -O3 -std=c++17 -o LiengSimulation_FastRun; .\LiengSimulation_FastRun.exe

# 3. (Tùy chọn) Xóa file sau khi chạy để sạch máy
rm LiengSimulation_FastRun.exe
```

*Lưu ý: Nếu bạn dùng CMD, hãy thay `./` bằng `.\` và đảm bảo trình biên dịch g++ đã được thêm vào PATH.*

### 3. Execution (Chạy với Visual Studio hoặc CMake)

**Lựa chọn A: Chạy bằng Visual Studio**
- Mở file dự án, sử dụng tính năng **Open Folder** của Visual Studio 2022 để mở thư mục `Lieng_Simulation`.
- Bấm **F5** hoặc click **Local Windows Debugger** để tiến hành biên dịch và chạy.
- Chờ vài giây để engine C++ biên dịch và hoàn thành 100.000 ván đấu; báo cáo trực quan sẽ tự động xuất ra.

**Lựa chọn B: Sử dụng CMake qua Terminal**
```bash
cd Lieng_Simulation
mkdir build
cd build
cmake ..
cmake --build . --config Release

# Chạy bản Release
./Release/LiengSimulation.exe
```
