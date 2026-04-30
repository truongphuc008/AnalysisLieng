#pragma once
#include "Player.h"
#include "Deck.h"
#include <vector>
#include <string>

// =============================================================
//  Cấu hình mô phỏng
// =============================================================
struct CauHinh {
    int         tongSoVan   = 100'000;
    int         luuMoi      = 100;
    std::string duongDanCSV  = "results.csv";
    std::string duongDanHTML = "results.html";
};

// =============================================================
//  Class MoPhong – Vòng lặp 100,000 ván bài
// =============================================================
class MoPhong {
public:
    explicit MoPhong(std::vector<NguoiChoi>& danhSach, CauHinh cauHinh = CauHinh{});

    // Chạy toàn bộ mô phỏng
    void chay();

    // Truy xuất lịch sử tiền
    const std::vector<std::vector<double>>& layLichSu() const { return lichSu; }

    // Xuất CSV
    void xuatCSV() const;

    // Vẽ ASCII chart trong terminal
    void inBieuDo() const;

private:
    std::vector<NguoiChoi>& danhSach;
    CauHinh                 cauHinh;
    BoBai                   boBai;

    // lichSu[snapshot][nguoi_choi] = tiền tại snapshot đó
    std::vector<std::vector<double>> lichSu;
    std::vector<std::string>         tenNguoiChoi;

    // Kiểm tra còn ai có thể chơi không
    bool conNguoiChoi() const;
};
