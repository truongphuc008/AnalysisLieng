# 🃏 Slide Thuyết trình: Mô phỏng Trò chơi Liêng

---

## 🚩 1. Giới thiệu Dự án
- **Tên dự án**: AnalysisLieng Simulation.
- **Mục tiêu**: Sử dụng lập trình hướng đối tượng (OOP) và phương pháp Monte Carlo để phân tích sự tác động của tâm lý đến xác suất thắng trong trò chơi Liêng.
- **Ngôn ngữ**: C++17.
- **Công nghệ**: Chart.js cho visualization.

---

## 🎯 2. Bài toán đặt ra
- Làm sao để dự đoán được người thắng trong một trò chơi có tính may rủi cao?
- Hệ số tâm lý (liều lĩnh vs thận trọng) ảnh hưởng thế nào đến túi tiền sau hàng vạn ván bài?
- Tại sao người chơi giỏi vẫn có thể cháy túi?

---

## 🎲 3. Phương pháp Monte Carlo
- **Khái niệm**: Mô phỏng lặp lại hàng triệu lần các biến số ngẫu nhiên.
- **Áp dụng**: 
  - Giai đoạn 1: 100.000 ván bài (Phân tích dài hạn).
  - Giai đoạn 2: 200 ván bài (Thống kê hiệu suất).

---

## 🧠 4. Mô hình Quyết định & Tâm lý
- **Tham số**: Tiền, Bộ bài, Hạng bài, Hệ số tâm lý.
- **Logic**: 
  - Thắng -> Tăng liều lĩnh.
  - Thua -> Tăng thận trọng.
- **Công thức**: $C = P \times W \times 100$ (Chỉ số quyết định cược).

---

## 🏗️ 5. Kiến trúc Hệ thống (OOP)
- `Card / Deck`: Quản lý bài tây 52 lá.
- `Player`: Lưu trữ trạng thái, tâm lý và tiền.
- `GameRound`: Điều phối vòng cược và so bài.
- `Simulation`: Vòng lặp Monte Carlo.
- `Visualizer`: Xuất dữ liệu CSV/HTML.

---

## 📊 6. Kết quả trực quan
- **Line Chart**: Theo dõi dòng tiền, xác định điểm "cháy túi".
- **Bar Chart**: So sánh trực diện số ván thắng giữa các phong cách chơi (Hung hăng, Thận trọng, Liều lĩnh).

---

## 💡 7. Kết luận
- Tâm lý ổn định là chìa khóa để tồn tại lâu dài.
- Phương pháp Monte Carlo là công cụ mạnh mẽ trong phân tích tài chính và trò chơi rủi ro.

---
**Cảm ơn các bạn đã lắng nghe!**
