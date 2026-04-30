#include "Player.h"
#include <algorithm>
#include <cmath>

// ─────────────────────────────────────────────────────────────
//  Constructor
// ─────────────────────────────────────────────────────────────
NguoiChoi::NguoiChoi(const std::string& ten, double tienBanDau, double tamLyBanDau)
    : ten(ten)
    , tien(tienBanDau)
    , tamLy(tamLyBanDau)
    , daBo(false)
    , cuocHienTai(0.0)
    , biLoai(false)
{}

// ─────────────────────────────────────────────────────────────
//  Reset trạng thái đầu ván
// ─────────────────────────────────────────────────────────────
void NguoiChoi::datLaiVan() {
    tay.xoa();
    daBo         = false;
    cuocHienTai  = 0.0;
}

// ─────────────────────────────────────────────────────────────
//  Chỉ số Tiếp tục chơi = tamLy * tiLeThang * 100
// ─────────────────────────────────────────────────────────────
double NguoiChoi::chiSoTiepTuc(double tiLeThang) const {
    return tamLy * tiLeThang * 100.0;
}

// ─────────────────────────────────────────────────────────────
//  Giới hạn cược tối đa dựa trên Bảng Ngưỡng từ Lieng.docx
// ─────────────────────────────────────────────────────────────
double NguoiChoi::cuocToiDaTuChiSo(double chiSo) {
    if (chiSo <= 500.0)  return 0.0;
    if (chiSo <= 600.0)  return 50.0;
    if (chiSo <= 700.0)  return 150.0;
    if (chiSo <= 800.0)  return 300.0;
    if (chiSo <= 850.0)  return 500.0;
    if (chiSo <= 880.0)  return 650.0;
    if (chiSo <= 910.0)  return 750.0;
    if (chiSo <= 940.0)  return 850.0;
    if (chiSo <= 970.0)  return 920.0;
    if (chiSo <= 990.0)  return 960.0;
    return 1000.0;
}

// ─────────────────────────────────────────────────────────────
//  Tiền cược tối đa theo tâm lý + tỉ lệ thắng
// ─────────────────────────────────────────────────────────────
double NguoiChoi::cuocToiDa(double tiLeThang) const {
    return cuocToiDaTuChiSo(chiSoTiepTuc(tiLeThang));
}

// ─────────────────────────────────────────────────────────────
//  Số tiền cược theo vòng = C * 0.01 * soVong
// ─────────────────────────────────────────────────────────────
double NguoiChoi::soTienTo(int soVong, double tiLeThang) const {
    return chiSoTiepTuc(tiLeThang) * 0.01 * soVong;
}

// ─────────────────────────────────────────────────────────────
//  Quyết định hành động:
//
//  Luật đặc biệt (ưu tiên cao nhất):
//    Nếu người chơi có Sám Cô, Liêng, hoặc Sảnh Rồng
//    → Bỏ qua yếu tố tâm lý, luôn luôn Tố ở mức tối đa (1000đ)
//
//  Bước 1: Tính chỉ số C = Tâm lý * Tỉ lệ thắng * 100
//  Bước 2: Nếu C <= 500 → Bỏ bài
//  Bước 3: Nếu C > 500 → Tiếp tục chơi
//    - Số tiền cược thiết kế = C * 0.01 * soVong
//    - Giới hạn cược tối đa = cuocToiDaTuChiSo(C)
//    - Nếu cược hiện tại vượt giới hạn → Bỏ bài
//    - Nếu mucTo > cuocCaoNhat → Tố, ngược lại → Theo
// ─────────────────────────────────────────────────────────────
HanhDong NguoiChoi::quyetDinh(double cuocCaoNhat, int soVong, double tiLeThang) {
    if (daBo || biLoai) return HanhDong::BO;

    // ─ Luật đặc biệt: Sám Cô / Liêng / Sảnh Rồng → Tố tối đa ────────
    HangBai hang = DanhGiaBai::danhGia(tay);
    if (hang == HangBai::SAM_CO ||
        hang == HangBai::LIENG  ||
        hang == HangBai::SANH_RONG)
    {
        constexpr double CUOC_TOI_DA = 1000.0;
        double canThem = cuocCaoNhat - cuocHienTai;

        if (CUOC_TOI_DA > cuocCaoNhat) {
            double toThem      = CUOC_TOI_DA - cuocCaoNhat;
            double tongCanTra  = canThem + toThem;
            if (tien >= tongCanTra) {
                tien         -= tongCanTra;
                cuocHienTai  += tongCanTra;
                return HanhDong::TO;
            }
            // Không đủ tiền tố tối đa → tố hết số còn lại
            if (tien > canThem && tien > 0.0) {
                cuocHienTai += tien;
                tien         = 0.0;
                return HanhDong::TO;
            }
        }
        // Đã ở mức tối đa hoặc không đủ tiền → Theo nếu có thể
        if (canThem <= 0.0) return HanhDong::THEO;
        if (tien >= canThem) {
            tien        -= canThem;
            cuocHienTai  = cuocCaoNhat;
            return HanhDong::THEO;
        }
        return HanhDong::BO;
    }

    double chiSo      = chiSoTiepTuc(tiLeThang);
    double gioiHanCuoc = cuocToiDaTuChiSo(chiSo);

    // ─ Bước 2: Nếu C <= 500, Bỏ bài ──────────────────────────────────
    if (chiSo <= NGUONG_TIEP_TUC) {
        return HanhDong::BO;
    }

    // ─ Bước 3: C > 500, Tiếp tục chơi ────────────────────────────────
    double mucTo    = chiSo * 0.01 * soVong;
    double canThem  = cuocCaoNhat - cuocHienTai;

    if (cuocCaoNhat > gioiHanCuoc) {
        return HanhDong::BO;
    }

    if (mucTo > cuocCaoNhat) {
        double toThem     = mucTo - cuocCaoNhat;
        double tongCanTra = canThem + toThem;
        if (tien >= tongCanTra && mucTo <= gioiHanCuoc) {
            tien         -= tongCanTra;
            cuocHienTai  += tongCanTra;
            return HanhDong::TO;
        }
    }

    // Theo (call)
    if (canThem <= 0.0) {
        return HanhDong::THEO;
    }
    if (tien >= canThem) {
        tien        -= canThem;
        cuocHienTai  = cuocCaoNhat;
        return HanhDong::THEO;
    }

    return HanhDong::BO;
}

// ─────────────────────────────────────────────────────────────
//  Cập nhật sau khi thắng
// ─────────────────────────────────────────────────────────────
void NguoiChoi::khiThang(double tienNhan) {
    tien  += tienNhan;
    tamLy += 1.0;
    giuTamLy();
}

// ─────────────────────────────────────────────────────────────
//  Cập nhật sau khi thua
// ─────────────────────────────────────────────────────────────
void NguoiChoi::khiThua() {
    tamLy -= 0.2;
    giuTamLy();
    if (tien <= 0.0) {
        tien    = 0.0;
        biLoai  = true;
    }
}

// ─────────────────────────────────────────────────────────────
//  Giữ tâm lý trong [TAM_LY_MIN, TAM_LY_MAX]
// ─────────────────────────────────────────────────────────────
void NguoiChoi::giuTamLy() {
    tamLy = std::max(TAM_LY_MIN, std::min(TAM_LY_MAX, tamLy));
}
