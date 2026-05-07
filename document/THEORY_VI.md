# 📚 Lý thuyết Mô hình Mô phỏng Monte Carlo - Trò chơi Liêng

## 1. Phương pháp Monte Carlo (Monte Carlo Method)
Mô hình này sử dụng phương pháp **Monte Carlo** – một thuật toán thống kê dựa trên việc lặp lại việc lấy mẫu ngẫu nhiên nhiều lần để thu được kết quả ước lượng. Trong dự án này, chúng ta mô phỏng hàng trăm nghìn ván bài để quan sát quy luật số lớn (Law of Large Numbers), giúp xác định xu hướng tài chính và tỉ lệ thắng của các chiến thuật khác nhau.

## 2. Mô hình Tâm lý người chơi (Psychological Model)
Mỗi người chơi được gán một hệ số **Tâm lý ($P$)** từ 1.0 đến 10.0.
- **P cao (8-10)**: Người chơi hung hăng, sẵn sàng tố (bet) mạnh dù bài không quá tốt.
- **P thấp (1-3)**: Người chơi thận trọng, chỉ theo bài khi có tỉ lệ thắng cao.

### Công thức tính Chỉ số Tiếp tục ($C$):
$$C = P \times W \times 100$$
Trong đó:
- $P$: Hệ số tâm lý hiện tại.
- $W$: Tỉ lệ thắng lý thuyết của hạng bài đang nắm giữ (ví dụ: Sám cô = 99.9%, Bài rác = 10%).

### Quyết định hành động:
- **$C \le 500$**: Bỏ bài (Fold).
- **$C > 500$**: Tiếp tục chơi.
  - Nếu bài cực mạnh (Liêng, Sám cô): Tố tối đa (All-in).
  - Nếu bài bình thường: Tố một lượng tiền dựa trên $C$ và số vòng cược.

## 3. Cơ chế thích nghi (Dynamic Adaptation)
Tâm lý không cố định mà thay đổi sau mỗi ván:
- **Thắng**: $P = P + 1.0$ (Tự tin hơn, hung hăng hơn).
- **Thua**: $P = P - 0.2$ (Lo sợ hơn, thận trọng hơn).
Điều này mô phỏng sự biến động cảm xúc thực tế của con người trong đánh bạc.

## 4. Cấu trúc mô phỏng 2 giai đoạn
Dự án được chia thành hai phần để tối ưu hóa việc phân tích:

### Giai đoạn 1: Phân tích xu hướng (100.000 ván)
- **Mục tiêu**: Xem ai là người còn trụ lại cuối cùng và sự biến động tổng tài sản.
- **Đầu ra**: Biểu đồ đường (Line Chart) thể hiện dòng tiền.

### Giai đoạn 2: Phân tích hiệu suất (200 ván)
- **Mục tiêu**: Đếm số lần thắng thực tế trong một mẫu ngắn hạn để xem sự hiệu quả của các lối chơi.
- **Đầu ra**: Biểu đồ cột (Bar Chart) thống kê số ván thắng.

---
*Tài liệu này thuộc dự án AnalysisLieng - Mô phỏng Tài chính & Tâm lý.*
