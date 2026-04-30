#pragma once
#include "Hand.h"
#include "HandEvaluator.h"
#include <string>

// =============================================================
//  Hành động mà người chơi có thể thực hiện trong lượt cược
// =============================================================
enum class HanhDong {
    THEO,  // Theo (gọi / call)
    TO,    // Tố (tăng cược / raise)
    BO     // Bỏ bài (fold)
};

// =============================================================
//  Class NguoiChoi – Người chơi với tâm lý và chiến lược cược
// =============================================================
class NguoiChoi {
public:
    // ── Thông tin cơ bản ─────────────────────────────────────
    std::string ten;
    double      tien;            // Tiền hiện có
    double      tamLy;           // Tâm lý [1.0, 10.0]

    // ── Trạng thái trong ván bài ─────────────────────────────
    TayBai      tay;             // 3 lá bài
    bool        daBo;            // Đã bỏ bài chưa?
    double      cuocHienTai;     // Tổng tiền đã bỏ vào pot ván này
    bool        biLoai;          // Hết tiền, bị loại khỏi game

    // ── Constructor ──────────────────────────────────────────
    NguoiChoi(const std::string& ten, double tienBanDau, double tamLyBanDau);

    // ── Cài đặt lại trạng thái đầu ván ──────────────────────
    void datLaiVan();

    // Chỉ số Tiếp tục chơi = tamLy * tiLeThang * 100
    double chiSoTiepTuc(double tiLeThang) const;

    // Giới hạn cược tối đa tra bảng ngưỡng theo chỉ số C
    static double cuocToiDaTuChiSo(double chiSo);

    // Tiền cược tối đa = cuocToiDaTuChiSo(chiSoTiepTuc(tiLeThang))
    double cuocToiDa(double tiLeThang) const;

    // ── Quyết định hành động ────────────────────────────────
    // cuocCaoNhat  : mức cược cao nhất hiện tại trên bàn
    // soVong       : số vòng cược (1, 2, 3,...)
    // tiLeThang    : tỉ lệ thắng của bộ bài hiện tại
    HanhDong quyetDinh(double cuocCaoNhat, int soVong, double tiLeThang);

    // Số tiền cược theo vòng = C * 0.01 * soVong
    double soTienTo(int soVong, double tiLeThang) const;

    // ── Cập nhật sau ván ─────────────────────────────────────
    void khiThang(double tienNhan);    // Thắng: tâm lý +1
    void khiThua();                    // Thua: tâm lý -0.2

    // ── Hằng số thiết kế ─────────────────────────────────────
    static constexpr double TIEN_DAT         = 5.0;     // Tiền đặt cược ban đầu mỗi ván
    static constexpr double NGUONG_TIEP_TUC  = 500.0;   // Ngưỡng tiếp tục chơi
    static constexpr double TAM_LY_MIN       = 1.0;
    static constexpr double TAM_LY_MAX       = 10.0;

private:
    void giuTamLy();
};
