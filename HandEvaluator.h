#pragma once
#include "Hand.h"

// =============================================================
//  Thứ hạng bộ bài Liêng (thấp → cao)
// =============================================================
enum class HangBai {
    RAC        = 0,   // Bài rác / mậu thầu
    DOI        = 1,   // Đôi (2 lá giống nhau)
    SANH_RAC   = 2,   // Sảnh rác (3 liên tiếp, khác chất)
    THUNG      = 3,   // Thùng (3 cùng chất, không liên tiếp)
    SANH       = 4,   // Sảnh (3 liên tiếp, cùng chất)
    SANH_RONG  = 5,   // Sảnh rồng (A-K-Q cùng chất)
    LIENG      = 6,   // Liêng (3 lá cùng số, trừ 3 Ách)
    SAM_CO     = 7    // Sám cô – Bài THÁNH (3 con Ách)
};

// =============================================================
//  Class DanhGiaBai – Đánh giá và so sánh tay bài
// =============================================================
class DanhGiaBai {
public:
    // Xác định hạng bài của tay bài
    static HangBai danhGia(const TayBai& tay);

    // Tỉ lệ thắng thống kê theo hạng bài [0.0, 1.0]
    static double tiLeThang(HangBai hang);

    // So sánh hai tay bài.
    // Trả về: 1 nếu a > b, -1 nếu a < b, 0 nếu hoà
    static int soSanh(const TayBai& a, const TayBai& b);

    // Tên hiển thị của hạng bài
    static std::string tenHang(HangBai hang);

private:
    // Các hàm hỗ trợ
    static bool laCungChat(const TayBai& tay);        // 3 cùng chất?
    static bool laSanh(const TayBai& tay);            // 3 liên tiếp?
    static bool laSanhRong(const TayBai& tay);        // A-K-Q cùng chất?

    // So sánh khi cùng hạng bài
    static int soSanhLaCao(const TayBai& a, const TayBai& b);
    static int soSanhDoi(const TayBai& a, const TayBai& b);
    static int soSanhLieng(const TayBai& a, const TayBai& b);
};
