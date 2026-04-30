# 🃏 Mô phỏng Xác suất Chiến thắng Liêng theo Hệ số Tâm lý

Dự án nghiên cứu và mô phỏng sự tác động của **Chỉ số Tâm lý (Psychological Factor)** kết hợp với **Xác suất Toán học** để phân tích biến động tài chính của người chơi qua 100.000 ván bài Liêng.

---

## 1. Tổng quan & Luật chơi cơ bản
Mô hình mô phỏng trò chơi Liêng sử dụng bộ bài Tây 52 lá, giới hạn **6 người chơi**. Mỗi người nhận 3 lá bài ngẫu nhiên và tiến hành cược để tìm ra người chiến thắng.

### Thứ tự độ mạnh của bài (Từ Cao xuống Thấp):
1.  **Liêng Sám Cô (3 lá Át):** Bài Thánh - Tỉ lệ thắng tuyệt đối.
2.  **Liêng (3 lá cùng số):** Ví dụ: 3 lá 8, 3 lá K.
3.  **Sảnh Rồng:** A-K-Q đồng chất.
4.  **Sảnh:** 3 lá liên tiếp đồng chất (Ví dụ: 7-8-9 cơ).
5.  **Thùng:** 3 lá đồng chất nhưng không liên tiếp.
6.  **Sảnh Rác:** 3 lá liên tiếp khác chất.
7.  **Đôi:** 2 lá giống nhau.
8.  **Bài Rác:** 3 lá không có liên kết (Không có giá trị).

### Cơ chế vòng cược:
* **Tiền chân (Ante):** Bắt buộc đóng trước khi chia bài.
* **Theo:** Bỏ ra số tiền bằng với mức cược cao nhất hiện tại trên bàn.
* **Tố:** Nâng mức cược lên cao hơn.
* **Bỏ bài:** Chấp nhận thua, mất toàn bộ tiền chân và tiền đã cược trước đó.
* **Phân định (Showdown):** Ai trụ lại cuối cùng có bài cao nhất sẽ ăn toàn bộ tiền cược (Pot).

---

## 2. Thiết lập Mô hình & Công thức Lượng hóa
Mô hình đánh giá sự ảnh hưởng của yếu tố tâm lý ban đầu đến kết quả tài chính chung cuộc.

### Tham số khởi tạo:
* **Số lượng:** 6 người chơi.
* **Tâm lý khởi điểm ($x$):** Ngẫu nhiên từ 1 đến 10 ($1 \le x \le 10$).

### Chỉ số Tiếp tục ($C$):
Quyết định cược dựa trên sự tự tin của người chơi được tính theo công thức:
$$C = \text{Tâm lý} \times \text{Tỉ lệ chiến thắng của tổ hợp bài}$$

### Ngưỡng chịu đựng rủi ro (Giới hạn cược tối đa):

| Chỉ số Tiếp tục ($C$) | Giới hạn Cược Tối Đa | Ghi chú |
| :--- | :--- | :--- |
| 0 - 500 | 0 đồng | Chỉ đóng tiền chân, hễ có người Tố là Bỏ bài. |
| 501 - 800 | 50 - 300 đồng | Mức độ thận trọng trung bình. |
| 801 - 970 | 500 - 920 đồng | Trạng thái tự tin cao. |
| 971 - 1000 | 1000 đồng | **Max Limit** - Mức cược kịch trần. |

> **Luật ngoại lệ:** Với các tổ hợp bài đặc biệt (**Sám Cô, Liêng, Sảnh Rồng**), người chơi sẽ bỏ qua yếu tố tâm lý và luôn luôn Tố ở mức tối đa.

---

## 3. Luồng thực thi của một Ván bài (Round)
1.  **Thu tiền chân:** Bắt đầu ván, cả 6 người chơi đều đóng 5 đồng vào Pot.
2.  **Tính toán mức cược:**
    * Nếu $C \le 500$: Người chơi cược 0 đồng (chỉ chờ xem xét Theo/Bỏ).
    * Nếu $C > 500$: $\text{Mức cược dự kiến} = C \times 0.01 \times \text{Round}$.
    * *Mức cược thực tế không được vượt quá Giới hạn Cược Tối Đa.*
3.  **So khớp cược:**
    * Để "Theo", người chơi phải bỏ ra số tiền bằng mức cược cao nhất hiện tại.
    * Nếu mức cược cao nhất vượt qua "Giới hạn Cược Tối Đa" của bất kỳ ai, tất cả sẽ buộc phải **Mở bài (Showdown)** ngay lập tức.
4.  **Cập nhật Trạng thái Tâm lý:**
    * **Người thắng:** Tâm lý được cộng thêm $+1$.
    * **Người thua:** Tâm lý bị trừ đi $-0.2$.

---

## 4. Khảo sát và Trực quan hóa
Hệ thống thực hiện mô phỏng qua **100.000 lượt chơi**. Kết quả thu thập giúp phân tích:
* Biến động tài chính và sự tích lũy tài sản theo thời gian.
* Mối tương quan giữa chỉ số tâm lý khởi đầu và lợi nhuận thực tế.
* Hiệu quả của chiến thuật "Tố" dựa trên xác suất kết hợp tâm lý.
