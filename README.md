# 🃏 Mô Phỏng Bài Liêng
### Biểu đồ Phân phối Xác suất Chiến thắng trong mô hình trò chơi Liêng theo Hệ số Tâm lý

![C++](https://img.shields.io/badge/C++-17-00599C?style=for-the-badge&logo=c%2B%2B)
![Simulation](https://img.shields.io/badge/Simulation-Monte_Carlo-brightgreen?style=for-the-badge)
![Data Analysis](https://img.shields.io/badge/Data_Analysis-HTML/JS-orange?style=for-the-badge)

Mô hình mô phỏng tài chính rủi ro hiệu suất cao áp dụng phương pháp **Monte Carlo** để phân tích sự phân phối xác suất chiến thắng và sự biến động tâm lý của người chơi. Trọng tâm của mô hình này là mô phỏng trò chơi bài truyền thống của Việt Nam "**Liêng**", đánh giá tỷ lệ thắng thua khi bị tác động bởi hệ số tâm lý trong các vòng cược.

---

## 🎭 1. Tổng quan & Luật chơi cơ bản

Mô hình mô phỏng trò chơi **Liêng** sử dụng bộ bài Tây 52 lá, giới hạn **6 người chơi**. Mỗi người nhận 3 lá bài ngẫu nhiên và tiến hành cược để tìm ra người chiến thắng.

### 🏆 Thứ tự độ mạnh của bài (Từ Cao xuống Thấp)
1. 🎴 **Liêng Sám Cô (3 lá Át):** Bài Thánh - Tỉ lệ thắng tuyệt đối.
2. 🎴 **Liêng (3 lá cùng số):** Ví dụ: 3 lá 8, 3 lá K.
3. 🎴 **Sảnh Rồng:** A-K-Q đồng chất.
4. 🎴 **Sảnh:** 3 lá liên tiếp đồng chất (Ví dụ: 7-8-9 cơ).
5. 🎴 **Thùng:** 3 lá đồng chất nhưng không liên tiếp.
6. 🎴 **Sảnh Rác:** 3 lá liên tiếp khác chất.
7. 🎴 **Đôi:** 2 lá giống nhau.
8. 🎴 **Bài Rác:** 3 lá không có liên kết (Không có giá trị).

### 💵 Cơ chế vòng cược
* 🪙 **Tiền chân (Ante):** Bắt buộc đóng trước khi chia bài.
* 🔄 **Theo:** Bỏ ra số tiền bằng với mức cược cao nhất hiện tại trên bàn.
* 📈 **Tố:** Nâng mức cược lên cao hơn.
* 🏳️ **Bỏ bài:** Chấp nhận thua, mất toàn bộ tiền chân và tiền đã cược trước đó.
* ⚖️ **Phân định (Showdown):** Ai trụ lại cuối cùng có bài cao nhất sẽ ăn toàn bộ tiền cược. Nếu có nhiều người cùng độ mạnh bài, tổng tiền cược được chia đều.

---

## 🧠 2. Ý tưởng Khảo sát & Thiết lập Mô hình

Mô hình đánh giá sự ảnh hưởng của **yếu tố tâm lý ban đầu** đến kết quả tài chính chung cuộc của người chơi. Hành vi cược (Theo/Tố/Bỏ) không chỉ phụ thuộc vào độ mạnh của bài mà còn bị chi phối bởi trạng thái tâm lý.

* 👥 **Số lượng:** 6 người chơi.
* 💰 **Vốn ban đầu:** 10 nghìn VND trên mỗi người chơi
* 🧘 **Tâm lý khởi điểm:** Mỗi người có một chỉ số tâm lý ngẫu nhiên từ 1 đến 10 ($1 \le x \le 10$).

---

## 📐 3. Công thức Lượng hóa Hành vi

Quyết định cược được tính toán dựa trên **Chỉ số Tiếp tục (C)**. Chỉ số này đại diện cho sự tự tin của người chơi trong ván đấu.

> **Công thức:** $C = \text{Tâm lý} \times \text{Tỉ lệ chiến thắng của tổ hợp bài}$

### 🛡️ Ngưỡng chịu đựng rủi ro (Giới hạn cược tối đa)

| Chỉ số Tiếp tục (C) | Giới hạn Cược Tối Đa | Ghi chú |
| :---: | :---: | :--- |
| **0 - 500** | 0 đồng | Chỉ đóng tiền chân, hễ có người Tố là Bỏ bài. |
| **501 - 600** | 50 đồng | Mức độ tự tin thấp |
| **601 - 700** | 150 đồng | |
| **701 - 800** | 300 đồng | |
| **801 - 850** | 500 đồng | |
| **851 - 880** | 650 đồng | |
| **881 - 910** | 750 đồng | |
| **911 - 940** | 850 đồng | |
| **941 - 970** | 920 đồng | |
| **971 - 990** | 960 đồng | |
| **990 - 1000** | **1000 đồng** | **Max Limit - Mức cược kịch trần.** |

> **⚠️ Luật ngoại lệ:** Nếu người chơi nhận được các tổ hợp đặc biệt: *Liêng Sám Cô, Liêng, hoặc Sảnh Rồng*, họ sẽ bỏ qua yếu tố tâm lý và luôn luôn Tố ở mức tối đa.

---

## 🏁 4. Luồng thực thi của một Ván bài (Round)

1. 🪙 **Thu tiền chân:** Bắt đầu ván, cả 6 người chơi đều bị trừ 5 đồng tiền chân vào tổng cược (Pot).
2. 🔢 **Tính toán mức cược:**
    * Nếu $C \le 500$: Người chơi cược 0 đồng (chờ Theo/Bỏ).
    * Nếu $C > 500$: $\text{Mức cược dự kiến} = C \times 0.01 \times (\text{Số Round})$.
    * *Mức cược thực tế không vượt quá Giới hạn Cược Tối Đa.*
3. ⚖️ **So khớp cược (Theo/Bỏ/Mở bài):**
    * Tìm ra mức cược cao nhất trên bàn. Người chơi muốn "Theo" phải bỏ thêm tiền cho bằng mức này.
    * Nếu mức cược cao nhất vượt qua "Giới hạn Cược Tối Đa" của một người chơi, tất cả những người còn lại sẽ bị buộc phải **Mở bài (Showdown)** ngay lập tức.
    * Người chơi chọn **Bỏ bài** sẽ mất toàn bộ tiền vốn đã góp.
4. 📈 **Cập nhật Trạng thái Tâm lý:**
    * **Người chiến thắng:** Tâm lý $+1$.
    * **Người thua cuộc:** Tâm lý $-0.2$.

---

## 📊 5. Khảo sát và Trực quan hóa Dữ liệu

Tiến hành chạy lặp lại chu trình trên qua **100.000 lượt chơi**. Dữ liệu tài chính và sự biến động tâm lý của 6 người chơi sẽ được ghi nhận để vẽ biểu đồ trực quan, từ đó phân tích tác động của tâm lý ban đầu đến lợi nhuận cuối cùng.

---

## 🛠️ Setup & Usage

### 📋 Prerequisites (Yêu cầu hệ thống)
* Trình biên dịch hỗ trợ **C++17** trở lên (MinGW/GCC).
* Đảm bảo thư mục `data/` tồn tại trong thư mục mã nguồn (`Lieng_Simulation/data`).

### 🚀 Quick Run (Chạy nhanh qua Terminal - Windows)
Nếu bạn đang dùng Windows (PowerShell), hãy copy đoạn mã này để chạy ngay lập tức:


# 1. Clone dự án và truy cập vào thư mục code
```
git clone https://github.com/truongphuc008/AnalysisLieng.git
```

```
cd AnalysisLieng/Lieng_Simulation
```


# 2. Biên dịch và chạy
```
g++ Card.cpp Deck.cpp GameRound.cpp Hand.cpp HandEvaluator.cpp main.cpp Player.cpp Simulation.cpp Visualizer.cpp -O3 -std=c++17 -o LiengSimulation_FastRun; .\LiengSimulation_FastRun.exe
```
