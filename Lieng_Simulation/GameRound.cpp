#include "GameRound.h"
#include "HandEvaluator.h"
#include <algorithm>
#include <stdexcept>

// ─────────────────────────────────────────────────────────────
//  Constructor
// ─────────────────────────────────────────────────────────────
VanBai::VanBai(std::vector<NguoiChoi*>& danhSach, BoBai& boBai)
    : danhSach(danhSach), boBai(boBai)
{}

// ─────────────────────────────────────────────────────────────
//  Chia 3 lá cho mỗi người chơi đang hoạt động
// ─────────────────────────────────────────────────────────────
void VanBai::chiaBai() {
    for (NguoiChoi* ng : dangChoi) {
        ng->tay.xoa();
        for (int i = 0; i < 3; ++i)
            ng->tay.them(boBai.chia());
    }
}

// ─────────────────────────────────────────────────────────────
//  So bài: tìm người thắng hoặc danh sách hòa
// ─────────────────────────────────────────────────────────────
NguoiChoi* VanBai::soBai(std::vector<NguoiChoi*>& danhSachThang) {
    danhSachThang.clear();
    NguoiChoi* gioi = nullptr;
    for (NguoiChoi* ng : dangChoi) {
        if (ng->daBo) continue;
        if (!gioi) {
            gioi = ng;
            danhSachThang.push_back(ng);
            continue;
        }
        int ket = DanhGiaBai::soSanh(ng->tay, gioi->tay);
        if (ket > 0) {
            gioi = ng;
            danhSachThang.clear();
            danhSachThang.push_back(ng);
        } else if (ket == 0) {
            danhSachThang.push_back(ng);
        }
    }
    return gioi;
}

// ─────────────────────────────────────────────────────────────
//  Vòng cược hoàn chỉnh (theo mô hình Lieng.docx)
// ─────────────────────────────────────────────────────────────
double VanBai::voiCuoc() {
    // ── Bước 1: Tiền đặt ban đầu ─────────────────────────────
    double cuocCaoNhat = NguoiChoi::TIEN_DAT;
    for (NguoiChoi* ng : dangChoi) {
        double tienDat = std::min(NguoiChoi::TIEN_DAT, ng->tien);
        ng->tien        -= tienDat;
        ng->cuocHienTai  = tienDat;
    }

    // ── Bước 2: Vòng cược ─────────────────────────────────────
    const int SO_VONG_TOI_DA = 10;
    int soVong = 1;

    for (int vong = 0; vong < SO_VONG_TOI_DA; ++vong) {
        bool coNguoiTo = false;

        for (NguoiChoi* ng : dangChoi) {
            if (ng->daBo) continue;

            // Tính giới hạn cược của người chơi này
            HangBai hang     = DanhGiaBai::danhGia(ng->tay);
            double  tiLeThang = DanhGiaBai::tiLeThang(hang);
            double  gioiHan  = NguoiChoi::cuocToiDaTuChiSo(ng->chiSoTiepTuc(tiLeThang));

            // Luật đặc biệt: Sám Cô / Liêng / Sảnh Rồng → giới hạn 1000đ
            bool baiBacPhat = (hang == HangBai::SAM_CO ||
                               hang == HangBai::LIENG  ||
                               hang == HangBai::SANH_RONG);
            if (baiBacPhat) gioiHan = 1000.0;

            // Trigger so bài nếu mức cược vượt ngưỡng chịu đựng
            if (cuocCaoNhat > gioiHan) {
                goto SO_BAI;
            }

            // Người chơi quyết định
            HanhDong hanh = ng->quyetDinh(cuocCaoNhat, soVong, tiLeThang);

            if (hanh == HanhDong::BO) {
                ng->daBo = true;
            } else {
                if (ng->cuocHienTai > cuocCaoNhat) {
                    cuocCaoNhat = ng->cuocHienTai;
                    coNguoiTo   = true;
                }
            }

            // Đếm người còn lại
            int conLai = 0;
            for (NguoiChoi* q : dangChoi)
                if (!q->daBo) ++conLai;
            if (conLai <= 1) goto SO_BAI;
        }

        // Không ai tố thêm → kết thúc cược
        if (!coNguoiTo) goto SO_BAI;

        // Nếu tất cả đã bằng mức cao nhất → so bài
        {
            bool tatCaBang = true;
            for (NguoiChoi* ng : dangChoi)
                if (!ng->daBo && ng->cuocHienTai < cuocCaoNhat)
                    tatCaBang = false;
            if (tatCaBang) goto SO_BAI;
        }

        ++soVong;
    }

    SO_BAI:
    double tienPot = 0.0;
    for (NguoiChoi* ng : dangChoi)
        tienPot += ng->cuocHienTai;
    return tienPot;
}

// ─────────────────────────────────────────────────────────────
//  Chạy 1 ván bài Liêng hoàn chỉnh
// ─────────────────────────────────────────────────────────────
KetQua VanBai::chay() {
    // Xây dựng danh sách người chơi đang hoạt động
    dangChoi.clear();
    for (NguoiChoi* ng : danhSach) {
        if (!ng->biLoai && ng->tien >= NguoiChoi::TIEN_DAT) {
            ng->datLaiVan();
            dangChoi.push_back(ng);
        } else if (!ng->biLoai && ng->tien < NguoiChoi::TIEN_DAT) {
            ng->biLoai = true;
        }
    }

    if (dangChoi.size() < 2) {
        KetQua kq;
        kq.nguoiThang  = dangChoi.empty() ? nullptr : dangChoi[0];
        kq.tienPot     = 0.0;
        kq.hangBaiThang = HangBai::RAC;
        return kq;
    }

    // Xáo & chia bài
    boBai.datLai();
    chiaBai();

    // Cược
    double tienPot = voiCuoc();

    // So bài – xử lý hòa
    std::vector<NguoiChoi*> danhSachThang;
    NguoiChoi* nguoiThang = soBai(danhSachThang);

    KetQua ketQua;
    ketQua.tienPot    = tienPot;
    ketQua.nguoiThang = nguoiThang;

    if (!danhSachThang.empty()) {
        ketQua.hangBaiThang = DanhGiaBai::danhGia(danhSachThang[0]->tay);

        if (danhSachThang.size() == 1) {
            nguoiThang->khiThang(tienPot);
        } else {
            // Hòa – chia đều pot
            double phan = tienPot / static_cast<double>(danhSachThang.size());
            for (NguoiChoi* ng : danhSachThang)
                ng->khiThang(phan);
        }
    }

    // Cập nhật tâm lý người thua
    for (NguoiChoi* ng : dangChoi) {
        bool laThang = false;
        for (NguoiChoi* ng2 : danhSachThang)
            if (ng == ng2) { laThang = true; break; }
        if (!laThang) ng->khiThua();
    }

    return ketQua;
}
