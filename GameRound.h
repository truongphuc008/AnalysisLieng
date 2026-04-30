#pragma once
#include "Player.h"
#include "Deck.h"
#include <vector>

// =============================================================
//  Kết quả của một ván bài
// =============================================================
struct KetQua {
    NguoiChoi*  nguoiThang;   // nullptr nếu tất cả bỏ
    double      tienPot;      // Tổng tiền thắng
    HangBai     hangBaiThang; // Hạng bài người thắng
};

// =============================================================
//  Class VanBai – Điều phối một ván bài Liêng hoàn chỉnh
// =============================================================
class VanBai {
public:
    // danhSach : danh sách TẤT CẢ người chơi (kể cả bị loại)
    // boBai    : bộ bài dùng chung (sẽ reset & xáo)
    explicit VanBai(std::vector<NguoiChoi*>& danhSach, BoBai& boBai);

    // Chạy 1 ván và trả về kết quả
    KetQua chay();

private:
    std::vector<NguoiChoi*>& danhSach;
    BoBai& boBai;

    // Danh sách người chơi còn hoạt động trong ván này
    std::vector<NguoiChoi*> dangChoi;

    // ── Các bước trong 1 ván ─────────────────────────────────
    void chiaBai();                          // Chia 3 lá cho mỗi người
    void thuTien(double& pot);               // Thu tiền ante ban đầu

    // Vòng cược chính; trả về tổng pot cuối cùng
    double voiCuoc();

    // So bài, tìm người thắng hoặc danh sách hòa
    NguoiChoi* soBai(std::vector<NguoiChoi*>& danhSachThang);
};
