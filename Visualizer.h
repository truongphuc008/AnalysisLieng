#pragma once
#include "Player.h"
#include <vector>
#include <string>

// =============================================================
//  Class HienThi – Bieu do ASCII, HTML Chart.js & xuat CSV
// =============================================================
class HienThi {
public:
    // lichSu[snapshot][nguoi_choi] = tien
    // tenNguoiChoi : ten 6 nguoi choi
    // buocLuu      : buoc nhay (moi N van ghi 1 snapshot)
    static void inBieuDoASCII(
        const std::vector<std::vector<double>>& lichSu,
        const std::vector<std::string>&         tenNguoiChoi,
        int buocLuu,
        int rong  = 80,
        int cao   = 24
    );

    static void xuatCSV(
        const std::vector<std::vector<double>>& lichSu,
        const std::vector<std::string>&         tenNguoiChoi,
        int buocLuu,
        const std::string& duongDan
    );

    // Xuat bieu do HTML dep voi Chart.js
    static void xuatHTML(
        const std::vector<std::vector<double>>& lichSu,
        const std::vector<std::string>&         tenNguoiChoi,
        int buocLuu,
        const std::string& duongDan = "results.html"
    );

    // In bang tong ket cuoi cung
    static void inBangTongKet(
        const std::vector<NguoiChoi>& danhSach,
        double tienBanDau
    );
};
