#include "Simulation.h"
#include "GameRound.h"
#include "Visualizer.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cstdlib>

// ─────────────────────────────────────────────────────────────
//  Constructor
// ─────────────────────────────────────────────────────────────
MoPhong::MoPhong(std::vector<NguoiChoi>& danhSach, CauHinh cauHinh)
    : danhSach(danhSach), cauHinh(cauHinh)
{
    for (const auto& ng : danhSach)
        tenNguoiChoi.push_back(ng.ten);
}

// ─────────────────────────────────────────────────────────────
//  Kiểm tra còn ai có thể chơi không (>= 2 người chưa bị loại)
// ─────────────────────────────────────────────────────────────
bool MoPhong::conNguoiChoi() const {
    int dem = 0;
    for (const auto& ng : danhSach)
        if (!ng.biLoai) ++dem;
    return dem >= 2;
}

// ─────────────────────────────────────────────────────────────
//  Chạy toàn bộ mô phỏng
// ─────────────────────────────────────────────────────────────
void MoPhong::chay() {
    // Chuẩn bị vector con trỏ cho VanBai
    std::vector<NguoiChoi*> danhSachPtr;
    for (auto& ng : danhSach)
        danhSachPtr.push_back(&ng);

    // Snapshot ban đầu (ván 0)
    {
        std::vector<double> snap;
        for (const auto& ng : danhSach) snap.push_back(ng.tien);
        lichSu.push_back(snap);
    }

    std::cout << "\n========================================\n";
    std::cout << "  MO PHONG BAI LIENG – " << cauHinh.tongSoVan << " van\n";
    std::cout << "========================================\n\n";

    for (int van = 1; van <= cauHinh.tongSoVan; ++van) {
        if (!conNguoiChoi()) {
            std::cout << "[!] Chi con 1 nguoi choi o van " << van
                      << ", dung mo phong.\n";
            break;
        }

        VanBai vanBai(danhSachPtr, boBai);
        vanBai.chay();

        // Ghi snapshot mỗi luuMoi ván
        if (van % cauHinh.luuMoi == 0) {
            std::vector<double> snap;
            for (const auto& ng : danhSach) snap.push_back(ng.tien);
            lichSu.push_back(snap);
        }

        // In tiến độ mỗi 10,000 ván
        if (van % 10000 == 0) {
            std::cout << "  Van " << std::setw(7) << van << " / "
                      << cauHinh.tongSoVan << "  |";
            for (const auto& ng : danhSach)
                std::cout << "  " << ng.ten << ": "
                          << std::fixed << std::setprecision(0)
                          << ng.tien;
            std::cout << "\n";
        }
    }

    std::cout << "\n========================================\n";
    std::cout << "  KET QUA SAU MO PHONG\n";
    std::cout << "========================================\n";

    xuatCSV();
    HienThi::xuatHTML(lichSu, tenNguoiChoi, cauHinh.luuMoi, cauHinh.duongDanHTML);

    HienThi::inBangTongKet(danhSach, 10000.0);

    std::cout << "\n[+] Da xuat file CSV : " << cauHinh.duongDanCSV << "\n";
    std::cout << "[+] Da xuat bieu do  : " << cauHinh.duongDanHTML << "\n";
    std::cout << "\n[*] Dang mo bieu do trong trinh duyet...\n";

    // Tự động mở file HTML trong trình duyệt mặc định
    std::string lenh = "start \"\" \"" + cauHinh.duongDanHTML + "\"";
    system(lenh.c_str());
}

// ─────────────────────────────────────────────────────────────
//  Xuất CSV
// ─────────────────────────────────────────────────────────────
void MoPhong::xuatCSV() const {
    HienThi::xuatCSV(lichSu, tenNguoiChoi, cauHinh.luuMoi, cauHinh.duongDanCSV);
}

// ─────────────────────────────────────────────────────────────
//  ASCII Chart (ủy quyền cho HienThi)
// ─────────────────────────────────────────────────────────────
void MoPhong::inBieuDo() const {
    HienThi::inBieuDoASCII(lichSu, tenNguoiChoi, cauHinh.luuMoi);
}
